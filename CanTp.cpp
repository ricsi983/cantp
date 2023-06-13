#include <unistd.h>
#include <thread>
#include"CanTp.h"
#include"Utils.h"

void SingleFrameMessage::CreateSingleFrame(uint8_t* buffer, uint8_t length)
{
    uint8_t firstByte = 0;
    Utils::SetBits(firstByte, 7, 4, static_cast<uint8_t>(E_FRAME_TYPE::SingleFrame));
    Utils::SetBits(firstByte, 3, 0, static_cast<uint8_t>(length));

    singleFrame.push_back(firstByte);

    for(uint8_t i = 0; i < length; i ++)
        singleFrame.push_back(buffer[i]);
}

SingleFrameMessage::SingleFrameMessage(uint8_t* buffer, uint8_t length, uint8_t frameSize) : 
    _frameSize(frameSize)
{
    CreateSingleFrame(buffer, length);
}

void MultiFrameMessage::CreateConsecutiveFrames(uint8_t* buffer, uint16_t length)
{
    uint8_t firstByte = 0;
    Utils::SetBits(firstByte, 7, 4, static_cast<uint8_t>(E_FRAME_TYPE::ConsecutiveFrame));
    Utils::SetBits(firstByte, 3, 0, static_cast<uint8_t>(0));
    uint8_t index = 0;
    uint8_t blockCounter = 0;
    std::vector<uint8_t> frame;

    for(uint16_t i = 0; i < length;)
    {
        if(index == _frameSize)
        {
            consecutiveFrames.push_back(frame);
            frame.clear();
            index = 0;
        }

        if(index == 0)
            frame.push_back(firstByte);
        else
        {
            frame.push_back(buffer[i]);
            ++ i;
        }
        ++ index;
    }

    if(frame.size() != 0)
    {
        consecutiveFrames.push_back(frame);
        frame.clear();
    }

    AddFrameCounters();
}

void MultiFrameMessage::CreateFirstFrame(uint8_t* buffer, uint16_t length)
{
    uint8_t firstByte = 0;
    Utils::SetBits(firstByte, 7, 4, static_cast<uint8_t>(E_FRAME_TYPE::FirstFrame));
    Utils::SetBits(firstByte, 3, 0, static_cast<uint8_t>(length >> 8));

    firstFrame.push_back(firstByte);
    firstFrame.push_back(static_cast<uint8_t>(length));

    for(uint16_t i = 0; i < _frameSize - 2; i ++)
        firstFrame.push_back(buffer[i]);
}

MultiFrameMessage::MultiFrameMessage(uint8_t* buffer, uint16_t length, uint8_t frameSize) :
    _frameSize(frameSize)
{
   CreateFirstFrame(buffer, length);
   CreateConsecutiveFrames(&buffer[frameSize - 2], length - frameSize + 2);
}

void MultiFrameMessage::AddFrameCounters()
{
    uint8_t blockCounter = 0;
    uint8_t maxBlockSize = 16;
    for(auto& frame : consecutiveFrames)
    {
        Utils::SetBits(frame[0], 3, 0, blockCounter);
        ++ blockCounter;
        if(blockCounter > maxBlockSize) 
            blockCounter = 1;
    }
}

std::vector<uint8_t> MultiFrameMessage::GetPayload()
{
    std::vector<uint8_t> payload;
    for(uint32_t i = 2; i < 8; i++)
        payload.push_back(firstFrame[i]);
    
    for(const auto& frame : consecutiveFrames)
    {
        for(uint32_t i = 1; i < frame.size(); i ++)
            payload.push_back(frame[i]);
    }

    return payload;
}

CanTp::CanTp(std::shared_ptr<CanInterface> interface, uint8_t receiveBlockSize, uint8_t separationTime) :
    _interface(interface),
    _receiveBlockSize(receiveBlockSize),
    _receiveSeparationTime(separationTime),
    _sendBlockSize(0),
    _sendSeparationTime(0)
{
    _frameSize = (_interface->IsCanFd()? 64 : 8);
}

bool CanTp::Connect(std::vector<uint8_t>& firstFrame, uint32_t source, uint32_t destination)
{
    std::vector<uint8_t> flowControlFrame;
    flowControlFrame.resize(8);
    uint32_t length;
    if(_interface->SendMessage(&firstFrame[0], firstFrame.size(), destination) == 0)
        if(_interface->ReceiveMessage(&flowControlFrame[0], &length, source) == 0)
        {
            ParseFlowControlFrame(flowControlFrame);
            return true;
        }
        else
        {
            return false;
        }
}

bool CanTp::SendBlock(std::vector<std::vector<uint8_t>>& block, uint32_t source, uint32_t destination)
{   
    uint32_t index = 0;
    for(uint8_t i = 0; i < _sendBlockSize; i ++)
    {
        if(block.size() == 0)
            break;
        auto frame = block.front();
        block.erase(block.begin());
        _interface->SendMessage(&frame[0], 8, destination);
        std::this_thread::sleep_for(std::chrono::milliseconds(_sendSeparationTime));
    }

    if(block.size() != 0)
    {
        std::vector<uint8_t> flowControlFrame;
        flowControlFrame.resize(8);
        uint32_t length = 0;
        _interface->ReceiveMessage(&flowControlFrame[0], &length, source);
        ParseFlowControlFrame(flowControlFrame);
    }
    return true;
}

E_FRAME_TYPE CanTp::Listen(uint32_t source, uint32_t destination)
{
    std::vector<uint8_t> firstFrame;
    firstFrame.resize(8);
    uint32_t length = 0;
    if(_interface->ReceiveMessage(&firstFrame[0], &length, source) == 0)
        return static_cast<E_FRAME_TYPE>(firstFrame[0] >> 4);
}

void CanTp::ParseFlowControlFrame(std::vector<uint8_t>& flowControlFrame)
{
    _sendBlockSize = flowControlFrame[1];
    _sendSeparationTime = flowControlFrame[2];
}

void CanTp::SendMessage(uint8_t* buffer, uint16_t length, uint32_t source, uint32_t destination)
{
    if(length <= _frameSize - 1)
    {
        SingleFrameMessage singleFrameMessage = SingleFrameMessage(buffer, length, _frameSize);
        _interface->SendMessage(&singleFrameMessage.singleFrame[0], singleFrameMessage.singleFrame.size(), destination);
    }
    else
    {
        MultiFrameMessage multiFrameMessage = MultiFrameMessage(buffer, length, _frameSize);
        if(Connect(multiFrameMessage.firstFrame, source, destination))
        {
            //multiFrameMessage.AddFrameCounters(_sendBlockSize);
            while(multiFrameMessage.consecutiveFrames.size() != 0)
                if(!SendBlock(multiFrameMessage.consecutiveFrames, source, destination))
                    break;
        }
    }
}

void CanTp::ReceiveMessage(uint8_t* buffer, uint16_t& length, uint32_t source, uint32_t destination)
{
    std::vector<uint8_t> firstFrame;
    firstFrame.resize(8);
    uint32_t firstFrameLength = 0;
    if(_interface->ReceiveMessage(&firstFrame[0], &firstFrameLength, source) == 0)
    {
        if(static_cast<E_FRAME_TYPE>(firstFrame[0] >> 4) == E_FRAME_TYPE::SingleFrame)
            memcpy(buffer, &firstFrame[1], firstFrame[0] & 0x0f);
        else
        {
            MultiFrameMessage multiFrameMessage;
            multiFrameMessage.firstFrame = firstFrame;
            uint8_t block[8] = {0};
            std::vector<uint8_t> flowControlFrame;
            flowControlFrame.resize(8);
            Utils::SetBits<uint8_t>(flowControlFrame[0], 7, 4, 3);
            Utils::SetBits<uint8_t>(flowControlFrame[0], 3, 0, 0);
            flowControlFrame[1] = _receiveBlockSize;
            flowControlFrame[2] = _receiveSeparationTime;
            _interface->SendMessage(&flowControlFrame[0], flowControlFrame.size(), destination);
            
            uint16_t lengthOfFrame = ((firstFrame[0] & 0x0f) | firstFrame[1]) - 6;
            uint16_t numberOfFrames = lengthOfFrame / 8 + 1;
            uint16_t numberOfBlocks = numberOfFrames / _receiveBlockSize;
            uint16_t blockIndex = 0;
            for(uint16_t i = 0; i < numberOfFrames;)
            {
                if(blockIndex == _receiveBlockSize)
                {
                    _interface->SendMessage(&flowControlFrame[0], flowControlFrame.size(), destination);
                    blockIndex = 0;
                }
                else
                {
                    uint32_t lengthOfBlock = 0;
                    _interface->ReceiveMessage(block, &lengthOfBlock, source);
                    multiFrameMessage.consecutiveFrames.emplace_back(block, block + lengthOfBlock);
                    ++blockIndex;
                    ++i;
                }
            }
            auto payload = multiFrameMessage.GetPayload();
            length = payload.size();
            memcpy(buffer, &payload[0], length);
        }
    }
}