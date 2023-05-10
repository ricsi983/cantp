#include<vector>
#include<memory>
#include <stdlib.h>
#include"CanInterface.h"

enum class E_FRAME_TYPE : uint8_t
{
    SingleFrame,
    FirstFrame,
    ConsecutiveFrame,
    FlowControlFrame
};


class SingleFrameMessage
{
private:
    void CreateSingleFrame(uint8_t* buffer, uint32_t length);    
public:
    std::vector<uint8_t> singleFrame;
    SingleFrameMessage(uint8_t* buffer, uint8_t length);
    SingleFrameMessage() = default;
    std::vector<uint8_t> GetPayload();
};

class MultiFrameMessage
{
private:
    void CreateConsecutiveFrames(uint8_t* buffer, uint16_t length);
    void CreateFirstFrame(uint8_t* buffer, uint16_t length);
public:
    std::vector<uint8_t> firstFrame;
    std::vector<std::vector<uint8_t>> consecutiveFrames;
    std::vector<uint8_t> flowControlFrame;
    MultiFrameMessage(uint8_t* buffer, uint16_t length);
    std::vector<uint8_t> GetPayload();
    MultiFrameMessage() = default;
    void AddFrameCounters(uint8_t blockSize);
};

class CanTp
{
    private:
        uint8_t _receiveBlockSize;
        uint8_t _receiveSeparationTime;
        uint8_t _sendBlockSize;
        uint8_t _sendSeparationTime;
        std::shared_ptr<CanInterface> _interface;

        void ParseFlowControlFrame(std::vector<uint8_t>& flowControlFrame);
        bool Connect(std::vector<uint8_t>& firstFrame, uint32_t source, uint32_t destination);
        bool SendBlock(std::vector<std::vector<uint8_t>>& block, uint32_t source, uint32_t destination);
        E_FRAME_TYPE Listen(uint32_t source, uint32_t destination);
    public:
        CanTp(std::shared_ptr<CanInterface> interface, uint32_t timeout, uint8_t blockSize, uint8_t separationTime);
        void SendMessage(uint8_t* buffer, uint16_t length, uint32_t source, uint32_t destination);
        void ReceiveMessage(uint8_t* buffer, uint16_t& length, uint32_t source, uint32_t destination);
};