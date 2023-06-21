#include "CanTp.h"
#include "MockInterface.h"
#include <gtest/gtest.h>
#include <map>

constexpr auto testDestinationId = 0xDCU;
constexpr auto testSourceId = 0x1C9;

struct CanTpTest : public ::testing::Test
{
protected:
  MockInterface _mockInterface;
  CanInterface _canInterface;

public:
  CanTpTest ()
  {
    SetMockInterface (&_mockInterface);
    _canInterface.SetSendFunction (MockSendInterface);
    _canInterface.SetReceiveFunction (MockReceiveInterface);
    _canInterface.SetTimeoutFunction (MockSetTimeout);
    _canInterface.SetCanFd (false);
  }

  void
  SetUp ()
  {
  }

  void
  TearDown ()
  {
    _mockInterface.ClearRxMessages (testSourceId);
    _mockInterface.ClearTxMessages (testDestinationId);
  }
};

TEST_F (CanTpTest, SingleFrameTestStandard)
{
  constexpr auto length = 5U;
  std::array<uint8_t, length> testPayload = { 1, 2, 3, 1, 2 };
  std::array<uint8_t, length + STANDARD_SINGLE_FRAME_HEADER_LENGTH>
      expectedFrame;
  expectedFrame[0] = length;
  std::memcpy (expectedFrame.data () + STANDARD_SINGLE_FRAME_HEADER_LENGTH,
               testPayload.data (), length);
  CanTp<StandardCan> canTp (_canInterface);
  canTp.SendMessage (testPayload.data (), testPayload.size (), testSourceId,
                     testDestinationId);
  auto resultFrame = _mockInterface.GetTxFrame (testDestinationId, 0);
  ASSERT_EQ (resultFrame.size (), expectedFrame.size ());
  ASSERT_FALSE (memcmp (resultFrame.data (), expectedFrame.data (),
                        resultFrame.size ()));
}

TEST_F (CanTpTest, SingleFrameTestFd)
{
  constexpr auto length = 60U;
  std::array<uint8_t, length> testPayload
      = { 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2,
          1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2,
          1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2 };

  std::array<uint8_t, FD_SINGLE_FRAME_HEADER_LENGTH + length + 2> expectedFrame
      = { 0, 60, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3,    1,
          2, 1,  2, 3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1,    2,
          1, 2,  3, 1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2,    1,
          2, 3,  1, 2, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 0xCC, 0xCC };
  CanTp<CanFd> canTp (_canInterface);
  canTp.SendMessage (testPayload.data (), testPayload.size (), testSourceId,
                     testDestinationId);
  auto resultFrame = _mockInterface.GetTxFrame (testDestinationId, 0);
  ASSERT_EQ (resultFrame.size (), expectedFrame.size ());
  ASSERT_FALSE (memcmp (resultFrame.data (), expectedFrame.data (),
                        resultFrame.size ()));
}

TEST_F (CanTpTest, MultiFrameTestStandardWithoutFc)
{
  constexpr auto length = 25U;
  std::array<uint8_t, length> testPayload
      = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3,
          4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  std::array<uint8_t, 8> expectedFirstFrame = { 0x10, 25, 1, 2, 3, 4, 5, 1 };
  std::array<uint8_t, 8> expectedConsecutiveFrame1
      = { 0x21, 2, 3, 4, 5, 1, 2, 3 };
  std::array<uint8_t, 8> expectedConsecutiveFrame2
      = { 0x22, 4, 5, 1, 2, 3, 4, 5 };
  std::array<uint8_t, 6> expectedConsecutiveFrame3 = { 0x23, 1, 2, 3, 4, 5 };
  CanTp<StandardCan> canTp (_canInterface);

  std::array<uint8_t, STANDARD_CAN_LENGTH> flowControlFrame;
  flowControlFrame.fill (0);
  flowControlFrame[0] = 0x30;
  flowControlFrame[1] = 0;
  flowControlFrame[2] = 100;
  _mockInterface.AddMessageToRx (testSourceId, flowControlFrame.data (),
                                 flowControlFrame.size ());

  canTp.SendMessage (testPayload.data (), testPayload.size (), testSourceId,
                     testDestinationId);
  auto resultFirstFrame = _mockInterface.GetTxFrame (testDestinationId, 0);
  auto resultConsecutiveFrame1
      = _mockInterface.GetTxFrame (testDestinationId, 1);
  auto resultConsecutiveFrame2
      = _mockInterface.GetTxFrame (testDestinationId, 2);
  auto resultConsecutiveFrame3
      = _mockInterface.GetTxFrame (testDestinationId, 3);
  ASSERT_EQ (resultFirstFrame.size (), expectedFirstFrame.size ());
  ASSERT_FALSE (memcmp (resultFirstFrame.data (), expectedFirstFrame.data (),
                        resultFirstFrame.size ()));
  ASSERT_EQ (resultConsecutiveFrame1.size (),
             expectedConsecutiveFrame1.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame1.data (),
                        expectedConsecutiveFrame1.data (),
                        resultConsecutiveFrame1.size ()));
  ASSERT_EQ (resultConsecutiveFrame2.size (),
             expectedConsecutiveFrame2.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame2.data (),
                        expectedConsecutiveFrame2.data (),
                        resultConsecutiveFrame2.size ()));
  ASSERT_EQ (resultConsecutiveFrame3.size (),
             expectedConsecutiveFrame3.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame3.data (),
                        expectedConsecutiveFrame3.data (),
                        resultConsecutiveFrame3.size ()));
}

TEST_F (CanTpTest, MultiFrameTestFdWithoutFc)
{

  std::array<uint8_t, 200> testPayload = {
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
  };
  std::array<uint8_t, 64> expectedFirstFrame
      = { 0x10, 200, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4,
          5,    1,   2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
          1,    2,   3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1,
          2,    3,   4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2 };
  std::array<uint8_t, 64> expectedConsecutiveFrame1
      = { 0x21, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3,
          4,    5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5,
          1,    2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  std::array<uint8_t, 64> expectedConsecutiveFrame2
      = { 0x22, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1,
          2,    3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3,
          4,    5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3 };
  std::array<uint8_t, 16> expectedConsecutiveFrame3
      = { 0x23, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 0xCC, 0xCC, 0xCC };
  CanTp<CanFd> canTp (_canInterface);

  std::array<uint8_t, CAN_FD_LENGTH> flowControlFrame;
  flowControlFrame.fill (0);
  flowControlFrame[0] = 0x30;
  flowControlFrame[1] = 0;
  flowControlFrame[2] = 100;
  _mockInterface.AddMessageToRx (testSourceId, flowControlFrame.data (),
                                 flowControlFrame.size ());

  canTp.SendMessage (testPayload.data (), testPayload.size (), testSourceId,
                     testDestinationId);
  auto resultFirstFrame = _mockInterface.GetTxFrame (testDestinationId, 0);
  auto resultConsecutiveFrame1
      = _mockInterface.GetTxFrame (testDestinationId, 1);
  auto resultConsecutiveFrame2
      = _mockInterface.GetTxFrame (testDestinationId, 2);
  auto resultConsecutiveFrame3
      = _mockInterface.GetTxFrame (testDestinationId, 3);
  ASSERT_EQ (resultFirstFrame.size (), expectedFirstFrame.size ());
  ASSERT_FALSE (memcmp (resultFirstFrame.data (), expectedFirstFrame.data (),
                        resultFirstFrame.size ()));
  ASSERT_EQ (resultConsecutiveFrame1.size (),
             expectedConsecutiveFrame1.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame1.data (),
                        expectedConsecutiveFrame1.data (),
                        resultConsecutiveFrame1.size ()));
  ASSERT_EQ (resultConsecutiveFrame2.size (),
             expectedConsecutiveFrame2.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame2.data (),
                        expectedConsecutiveFrame2.data (),
                        resultConsecutiveFrame2.size ()));
  ASSERT_EQ (resultConsecutiveFrame3.size (),
             expectedConsecutiveFrame3.size ());
  ASSERT_FALSE (memcmp (resultConsecutiveFrame3.data (),
                        expectedConsecutiveFrame3.data (),
                        resultConsecutiveFrame3.size ()));
}

// TEST (CanTpTest, MultiFrameTestWithBlockSize)
//{
/*
uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1,
2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
uint8_t expectedFirstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1};
uint8_t expectedConsecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3};
uint8_t expectedConsecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5};
uint8_t expectedConsecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2};
uint8_t expectedConsecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4};
uint8_t expectedConsecutiveFrame5[8] = {0x24, 5, 1, 2, 3, 4, 5};
auto multiFrameMessage = MultiFrameMessage(testPayload, sizeof(testPayload),
8);

ASSERT_FALSE(memcmp(&multiFrameMessage.firstFrame[0], expectedFirstFrame,
multiFrameMessage.firstFrame.size()));
ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[0][0],
expectedConsecutiveFrame1, multiFrameMessage.consecutiveFrames[0].size()));
ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[1][0],
expectedConsecutiveFrame2, multiFrameMessage.consecutiveFrames[1].size()));
ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[2][0],
expectedConsecutiveFrame3, multiFrameMessage.consecutiveFrames[2].size()));
ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[3][0],
expectedConsecutiveFrame4, multiFrameMessage.consecutiveFrames[3].size()));
ASSERT_FALSE(memcmp(&multiFrameMessage.consecutiveFrames[4][0],
expectedConsecutiveFrame5, multiFrameMessage.consecutiveFrames[4].size()));*/
//}

// TEST (CanTpTest, SingleFrameSendTest)
//{
/*
std::shared_ptr<CanInterface> canInterface =
std::make_shared<CanInterface>();
canInterface->SetSendFunction(MockSendInterface);
canInterface->SetReceiveFunction(MockReceiveInterface);
canInterface->SetTimeoutFunction(MockSetTimeout);
canInterface->SetCanFd(false);
std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

uint8_t testPayload[5] = {1, 2, 3, 1, 2};
uint8_t expectedPayload[6] = {5, 1, 2, 3, 1, 2};
uint32_t testSourceId = 0x1C9;
uint32_t testDestinationId = 0xDC;
canTp->SendMessage(testPayload, sizeof(testPayload), testSourceId,
testDestinationId); auto txFrames = mockInterface.GetTxMessages(0xDC);
ASSERT_FALSE(memcmp(&txFrames[0][0], expectedPayload, txFrames[0].size()));*/
//}

// TEST (CanTpTest, MultiFrameSendTest)
//{
/*
mockInterface.ClearRxMessages(0x1C9);
mockInterface.ClearTxMessages(0xDC);
std::shared_ptr<CanInterface> canInterface =
std::make_shared<CanInterface>();
canInterface->SetSendFunction(MockSendInterface);
canInterface->SetReceiveFunction(MockReceiveInterface);
canInterface->SetTimeoutFunction(MockSetTimeout);
canInterface->SetCanFd(false);
std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1,
2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
uint8_t expectedFirstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1}; uint8_t
expectedConsecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3}; uint8_t
expectedConsecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5}; uint8_t
expectedConsecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2}; uint8_t
expectedConsecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4}; uint8_t
expectedConsecutiveFrame5[8] = {0x24, 5, 1, 2, 3, 4, 5}; uint32_t
testSourceId = 0x1C9; uint32_t testDestinationId = 0xDC; std::vector<uint8_t>
flowControlFrame1 = {0, 0, 0, 0, 0, 0, 0, 0};
Utils::SetBits<uint8_t>(flowControlFrame1[0], 7, 4, 3);
Utils::SetBits<uint8_t>(flowControlFrame1[0], 3, 0, 0);
flowControlFrame1[1] = 2;
flowControlFrame1[2] = 100;
mockInterface.AddMessageToRx(testSourceId, &flowControlFrame1[0],
flowControlFrame1.size()); mockInterface.AddMessageToRx(testSourceId,
&flowControlFrame1[0], flowControlFrame1.size());
mockInterface.AddMessageToRx(testSourceId, &flowControlFrame1[0],
flowControlFrame1.size()); canTp->SendMessage(testPayload,
sizeof(testPayload), testSourceId, testDestinationId); auto txFrames =
mockInterface.GetTxMessages(0xDC);

ASSERT_FALSE(memcmp(&txFrames[0][0], expectedFirstFrame,
txFrames[0].size())); ASSERT_FALSE(memcmp(&txFrames[1][0],
expectedConsecutiveFrame1, txFrames[1].size()));
ASSERT_FALSE(memcmp(&txFrames[2][0], expectedConsecutiveFrame2,
txFrames[2].size())); ASSERT_FALSE(memcmp(&txFrames[3][0],
expectedConsecutiveFrame3, txFrames[3].size()));
ASSERT_FALSE(memcmp(&txFrames[4][0], expectedConsecutiveFrame4,
txFrames[4].size())); ASSERT_FALSE(memcmp(&txFrames[5][0],
expectedConsecutiveFrame5, txFrames[5].size()));*/
//}

// TEST (CanTpTest, SingleFrameReceiveTest)
//{
/*
std::shared_ptr<CanInterface> canInterface =
std::make_shared<CanInterface>();
canInterface->SetSendFunction(MockSendInterface);
canInterface->SetReceiveFunction(MockReceiveInterface);
canInterface->SetTimeoutFunction(MockSetTimeout);
canInterface->SetCanFd(false);
std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 5, 100);

uint8_t testPayload[6] = {5, 1, 2, 3, 1, 2};

uint32_t testSourceId = 0x1C9;
uint32_t testDestinationId = 0xDC;
mockInterface.AddMessageToRx(testSourceId, testPayload, sizeof(testPayload));
std::vector<uint8_t> payloadReceived;
payloadReceived.resize(4096);
uint16_t receivedPayloadLength = 0;
canTp->ReceiveMessage(&payloadReceived[0], receivedPayloadLength,
testSourceId, testDestinationId); auto txFrames =
mockInterface.GetTxMessages(0xDC); ASSERT_FALSE(memcmp(&testPayload[1],
&payloadReceived[0], 5));*/
//}

// TEST (CanTpTest, MultiFrameReceiveTest)
//{
/*
mockInterface.ClearRxMessages(0x1C9);
mockInterface.ClearTxMessages(0xDC);
std::shared_ptr<CanInterface> canInterface =
std::make_shared<CanInterface>();
canInterface->SetSendFunction(MockSendInterface);
canInterface->SetReceiveFunction(MockReceiveInterface);
canInterface->SetTimeoutFunction(MockSetTimeout);
canInterface->SetCanFd(false);
std::unique_ptr<CanTp> canTp = std::make_unique<CanTp>(canInterface, 2, 100);

uint8_t testPayload[40] =  {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1,
2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
uint8_t firstFrame[8] = {0x10, 40, 1, 2, 3, 4, 5, 1}; uint8_t
consecutiveFrame1[8] = {0x20, 2, 3, 4, 5, 1, 2, 3}; uint8_t
consecutiveFrame2[8] = {0x21, 4, 5, 1, 2, 3, 4, 5}; uint8_t
consecutiveFrame3[8] = {0x22, 1, 2, 3, 4, 5, 1, 2}; uint8_t
consecutiveFrame4[8] = {0x23, 3, 4, 5, 1, 2, 3, 4}; uint8_t
consecutiveFrame5[7] = {0x24, 5, 1, 2, 3, 4, 5}; uint32_t testSourceId =
0x1C9; uint32_t testDestinationId = 0xDC; std::vector<uint8_t>
flowControlFrame1 = {0, 0, 0, 0, 0, 0, 0, 0};
Utils::SetBits<uint8_t>(flowControlFrame1[0], 7, 4, 3);
Utils::SetBits<uint8_t>(flowControlFrame1[0], 3, 0, 0);
flowControlFrame1[1] = 2;
flowControlFrame1[2] = 100;
mockInterface.AddMessageToRx(testSourceId, firstFrame, sizeof(firstFrame));
mockInterface.AddMessageToRx(testSourceId, consecutiveFrame1,
sizeof(consecutiveFrame1)); mockInterface.AddMessageToRx(testSourceId,
consecutiveFrame2, sizeof(consecutiveFrame2));
mockInterface.AddMessageToRx(testSourceId, consecutiveFrame3,
sizeof(consecutiveFrame3)); mockInterface.AddMessageToRx(testSourceId,
consecutiveFrame4, sizeof(consecutiveFrame4));
mockInterface.AddMessageToRx(testSourceId, consecutiveFrame5,
sizeof(consecutiveFrame5));

std::vector<uint8_t> payloadReceived;
payloadReceived.resize(4096);
uint16_t receivedPayloadLength = 0;
canTp->ReceiveMessage(&payloadReceived[0], receivedPayloadLength,
testSourceId, testDestinationId); auto txFrames =
mockInterface.GetTxMessages(0xDC);

ASSERT_FALSE(memcmp(&txFrames[0][0], &flowControlFrame1[0],
flowControlFrame1.size())); ASSERT_FALSE(memcmp(&txFrames[1][0],
&flowControlFrame1[0], flowControlFrame1.size()));
ASSERT_FALSE(memcmp(&txFrames[2][0], &flowControlFrame1[0],
flowControlFrame1.size())); ASSERT_FALSE(memcmp(&payloadReceived[0],
testPayload, receivedPayloadLength));*/
//}