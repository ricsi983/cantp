#include <gtest/gtest.h>
#include <map>
#include "CanTp.h"
#include "Utils.h"

class MockInterface
{
private:
    std::map<uint32_t, std::vector<std::vector<uint8_t>>> _txFrames; 
    std::map<uint32_t, std::vector<std::vector<uint8_t>>> _rxFrames;
    bool _timeout;
    uint32_t _sendResult;
    uint32_t _receiveResult;
public:
    MockInterface() :
        _timeout(false)
    {
        _txFrames[0xDC] = std::vector<std::vector<uint8_t>>();
        _rxFrames[0x1C9] = std::vector<std::vector<uint8_t>>();
    }

    uint32_t Send(uint8_t* buffer, uint8_t length, uint32_t id)
    {
        std::vector<uint8_t> frame;
        frame.resize(length);
        memcpy(&frame[0], buffer, length);
        _txFrames.at(id).push_back(frame);
        return _sendResult;
    }

    std::vector<std::vector<uint8_t>> GetTxMessages(uint32_t id)
    {
        return  _txFrames.at(id);
    }

    void SetSendResult(uint32_t result)
    {
        _sendResult = result;
    }

    void SetReceiveResult(uint32_t result)
    {
        _receiveResult = result;
    }

    void SetTimeout(bool timeout)
    {
        _timeout = timeout;
    }

    void ClearTxMessages(uint32_t id)
    {
        _txFrames.at(id).clear();
    }

    void ClearRxMessages(uint32_t id)
    {
        _rxFrames.at(id).clear();
    }

    void AddMessageToRx(uint32_t id, uint8_t* buffer, uint8_t length)
    {
        std::vector<uint8_t> frame;
        frame.resize(length);
        memcpy(&frame[0], buffer, length);
        _rxFrames.at(id).push_back(frame);
    }

    uint32_t Receive(uint8_t* buffer, uint8_t* length, uint32_t id)
    {
        if(_rxFrames.at(id).size() != 0)
        {
            auto frame = _rxFrames.at(id).front();
            *length = frame.size();
            memcpy(buffer, &frame[0], frame.size());
            _rxFrames.at(id).erase(_rxFrames.at(id).begin());
        }
        return _receiveResult;
    }
};

static MockInterface mockInterface;

uint32_t MockSendInterface(uint8_t* buffer, uint8_t length, uint32_t id)
{
   return mockInterface.Send(buffer, length, id);
}

uint32_t MockReceiveInterface(uint8_t* buffer, uint8_t* length, uint32_t id)
{
   return mockInterface.Receive(buffer, length, id);
}

void MockSetTimeout(uint32_t timeout)
{
    mockInterface.SetTimeout(timeout);
}



TEST(CanTpTest, SingleFrameTest)
{
    uint8_t testPayload[5] = {1, 2, 3, 1, 2};
    uint8_t expectedPayload[6] = {5, 1, 2, 3, 1, 2};
    auto singleFrameMessage = SingleFrameMessage(testPayload, sizeof(testPayload));
    ASSERT_EQ(singleFrameMessage.singleFrame.size(), sizeof(expectedPayload));
    ASSERT_FALSE(memcmp(&singleFrameMessage.singleFrame[0], expectedPayload, singleFrameMessage.singleFrame.size()));
}

TEST(CanTpTest, MultiFrameTest)
{
    uint8_t testPayload[25] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    uint8_t expectedFirstFrame[8] = {0x10, 25, 1, 2, 3, 4, 5, 1};
    uint8_t expectedConsecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3};
    uint8_t expectedConsecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5};
    uint8_t expectedConsecutiveFrame3[6] = {0x22, 1, 2, 3, 4, 5};
    auto multiFrameMessage = MultiFrameMessage(testPayload, sizeof(testPayload));

    ASSERT_FALSE(memcmp(&multiFrameMessage.firstFrame[0], expectedFirstFrame, multiFrameMessage.firstFrame.size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[0][0], expectedConsecutiveFrame1, multiFrameMessage.consecutiveFrames[0].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[1][0], expectedConsecutiveFrame2, multiFrameMessage.consecutiveFrames[1].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[2][0], expectedConsecutiveFrame3, multiFrameMessage.consecutiveFrames[2].size()));
}

TEST(CanTpTest, MultiFrameTestWithBlockSize)
{
    uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    uint8_t expectedFirstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1};
    uint8_t expectedConsecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3};
    uint8_t expectedConsecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5};
    uint8_t expectedConsecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2};
    uint8_t expectedConsecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4};
    uint8_t expectedConsecutiveFrame5[8] = {0x24, 5, 1, 2, 3, 4, 5};
    auto multiFrameMessage = MultiFrameMessage(testPayload, sizeof(testPayload));

    ASSERT_FALSE(memcmp(&multiFrameMessage.firstFrame[0], expectedFirstFrame, multiFrameMessage.firstFrame.size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[0][0], expectedConsecutiveFrame1, multiFrameMessage.consecutiveFrames[0].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[1][0], expectedConsecutiveFrame2, multiFrameMessage.consecutiveFrames[1].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[2][0], expectedConsecutiveFrame3, multiFrameMessage.consecutiveFrames[2].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[3][0], expectedConsecutiveFrame4, multiFrameMessage.consecutiveFrames[3].size()));
    ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[4][0], expectedConsecutiveFrame5, multiFrameMessage.consecutiveFrames[4].size()));
}

TEST(CanTpTest, SingleFrameSendTest)
{
   std::shared_ptr<CanInterface> canInterface = std::make_shared<CanInterface>();
   canInterface->SetSendFunction(MockSendInterface);
   canInterface->SetReceiveFunction(MockReceiveInterface);
   canInterface->SetTimeoutFunction(MockSetTimeout);
   std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

   uint8_t testPayload[5] = {1, 2, 3, 1, 2};
   uint8_t expectedPayload[6] = {5, 1, 2, 3, 1, 2};
   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   canTp->SendMessage(testPayload, sizeof(testPayload), testSourceId, testDestinationId);
   auto txFrames = mockInterface.GetTxMessages(0xDC);
   ASSERT_FALSE(memcmp(&txFrames[0][0], expectedPayload, txFrames[0].size()));
}

TEST(CanTpTest, MultiFrameSendTest)
{
   mockInterface.ClearRxMessages(0x1C9);
   mockInterface.ClearTxMessages(0xDC);
   std::shared_ptr<CanInterface> canInterface = std::make_shared<CanInterface>();
   canInterface->SetSendFunction(MockSendInterface);
   canInterface->SetReceiveFunction(MockReceiveInterface);
   canInterface->SetTimeoutFunction(MockSetTimeout);
   std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

   uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
   uint8_t expectedFirstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1};
   uint8_t expectedConsecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3};
   uint8_t expectedConsecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5};
   uint8_t expectedConsecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2};
   uint8_t expectedConsecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4};
   uint8_t expectedConsecutiveFrame5[8] = {0x24, 5, 1, 2, 3, 4, 5};
   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   std::vector<uint8_t> flowControlFrame1 = {0, 0, 0, 0, 0, 0, 0, 0}; 
   Utils::SetBits<uint8_t>(flowControlFrame1[0], 7, 4, 3);
   Utils::SetBits<uint8_t>(flowControlFrame1[0], 3, 0, 0);
   flowControlFrame1[1] = 2;
   flowControlFrame1[2] = 100;
   mockInterface.AddMessageToRx(testSourceId, &flowControlFrame1[0], flowControlFrame1.size());
   mockInterface.AddMessageToRx(testSourceId, &flowControlFrame1[0], flowControlFrame1.size());
   mockInterface.AddMessageToRx(testSourceId, &flowControlFrame1[0], flowControlFrame1.size());
   canTp->SendMessage(testPayload, sizeof(testPayload), testSourceId, testDestinationId);
   auto txFrames = mockInterface.GetTxMessages(0xDC);

   ASSERT_FALSE(memcmp(&txFrames[0][0], expectedFirstFrame, txFrames[0].size()));
   ASSERT_FALSE(memcmp(&txFrames[1][0], expectedConsecutiveFrame1, txFrames[1].size()));
   ASSERT_FALSE(memcmp(&txFrames[2][0], expectedConsecutiveFrame2, txFrames[2].size()));
   ASSERT_FALSE(memcmp(&txFrames[3][0], expectedConsecutiveFrame3, txFrames[3].size()));
   ASSERT_FALSE(memcmp(&txFrames[4][0], expectedConsecutiveFrame4, txFrames[4].size()));
   ASSERT_FALSE(memcmp(&txFrames[5][0], expectedConsecutiveFrame5, txFrames[5].size()));
}

TEST(CanTpTest, SingleFrameReceiveTest)
{
   std::shared_ptr<CanInterface> canInterface = std::make_shared<CanInterface>();
   canInterface->SetSendFunction(MockSendInterface);
   canInterface->SetReceiveFunction(MockReceiveInterface);
   canInterface->SetTimeoutFunction(MockSetTimeout);
   std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

   uint8_t testPayload[6] = {5, 1, 2, 3, 1, 2};

   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   mockInterface.AddMessageToRx(testSourceId, testPayload, sizeof(testPayload));
   std::vector<uint8_t> payloadReceived;
   payloadReceived.resize(4096);
   uint16_t receivedPayloadLength = 0;
   canTp->ReceiveMessage(&payloadReceived[0], receivedPayloadLength, testSourceId, testDestinationId);
   auto txFrames = mockInterface.GetTxMessages(0xDC);
   ASSERT_FALSE(memcmp(&testPayload[1], &payloadReceived[0], 5));
}

TEST(CanTpTest, MultiFrameReceiveTest)
{
   mockInterface.ClearRxMessages(0x1C9);
   mockInterface.ClearTxMessages(0xDC);
   std::shared_ptr<CanInterface> canInterface = std::make_shared<CanInterface>();
   canInterface->SetSendFunction(MockSendInterface);
   canInterface->SetReceiveFunction(MockReceiveInterface);
   canInterface->SetTimeoutFunction(MockSetTimeout);
   std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 2, 100);

   uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
   uint8_t firstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1};
   uint8_t consecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3};
   uint8_t consecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5};
   uint8_t consecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2};
   uint8_t consecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4};
   uint8_t consecutiveFrame5[7] = {0x24, 5, 1, 2, 3, 4, 5};
   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   std::vector<uint8_t> flowControlFrame1 = {0, 0, 0, 0, 0, 0, 0, 0}; 
   Utils::SetBits<uint8_t>(flowControlFrame1[0], 7, 4, 3);
   Utils::SetBits<uint8_t>(flowControlFrame1[0], 3, 0, 0);
   flowControlFrame1[1] = 2;
   flowControlFrame1[2] = 100;
   mockInterface.AddMessageToRx(testSourceId, firstFrame, sizeof(firstFrame));
   mockInterface.AddMessageToRx(testSourceId, consecutiveFrame1, sizeof(consecutiveFrame1));
   mockInterface.AddMessageToRx(testSourceId, consecutiveFrame2, sizeof(consecutiveFrame2));
   mockInterface.AddMessageToRx(testSourceId, consecutiveFrame3, sizeof(consecutiveFrame3));
   mockInterface.AddMessageToRx(testSourceId, consecutiveFrame4, sizeof(consecutiveFrame4));
   mockInterface.AddMessageToRx(testSourceId, consecutiveFrame5, sizeof(consecutiveFrame5));

   std::vector<uint8_t> payloadReceived;
   payloadReceived.resize(4096);
   uint16_t receivedPayloadLength = 0;
   canTp->ReceiveMessage(&payloadReceived[0], receivedPayloadLength, testSourceId, testDestinationId);
   auto txFrames = mockInterface.GetTxMessages(0xDC);

   ASSERT_FALSE(memcmp(&txFrames[0][0], &flowControlFrame1[0], flowControlFrame1.size()));
   ASSERT_FALSE(memcmp(&txFrames[1][0], &flowControlFrame1[0], flowControlFrame1.size()));
   ASSERT_FALSE(memcmp(&txFrames[2][0], &flowControlFrame1[0], flowControlFrame1.size()));
   ASSERT_FALSE(memcmp(&payloadReceived[0], testPayload, receivedPayloadLength));
}