// #include "CanTp_2.h"
#include "MockInterface.h"
#include "Utils.h"
#include <gtest/gtest.h>
#include <map>
#include <memory>

/*
static CanInterface _canInterface;
static std::unique_ptr<CanTp_2<STANDARD_CAN_LENGTH>>  _canTp;
static std::unique_ptr<CanTp_2<CAN_FD_LENGTH>> _canTpFd;

class CanTp_2Test : public ::testing::Test
{

    public:
        static void SetUpTestSuite()
        {
            _canTp =
std::make_unique<CanTp_2<STANDARD_CAN_LENGTH>>(_canInterface); _canTpFd =
std::make_unique<CanTp_2<CAN_FD_LENGTH>>(_canInterface);
            _canInterface.SetSendFunction(MockSendInterface);
            _canInterface.SetReceiveFunction(MockReceiveInterface);
            _canInterface.SetTimeoutFunction(MockSetTimeout);
            _canInterface.SetCanFd(false);
        }

        static void TearDownTestSuite()
        {

        }

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }
};
÷/

TEST_F(CanTp_2Test, SingleFrameSendTest)
{
   uint8_t testPayload[5] = {1, 2, 3, 1, 2};
   uint8_t expectedPayload[6] = {5, 1, 2, 3, 1, 2};
   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   //canTp->SendMessage(testPayload, sizeof(testPayload), testSourceId,
testDestinationId);
   //auto txFrames = mockInterface.GetTxMessages(0xDC);
   //ASSERT_FALSE(memcmp(&txFrames[0][0], expectedPayload,
txFrames[0].size()));

/*
   auto canInterface = CanInterface();
   canInterface.SetSendFunction(MockSendInterface);
   canInterface.SetReceiveFunction(MockReceiveInterface);
   canInterface.SetTimeoutFunction(MockSetTimeout);
   canInterface.SetCanFd(false);

   auto canTp = canInterface.IsCanFd() ? CanTp_2<CAN_FD_LENGTH>(canInterface) :
CanTp_2<STANDARD_CAN_LENGTH>(canInterface); std::unique_ptr<CanTp> canTp =
std::make_unique<CanTp>(canInterface, 5, 100);

   uint8_t testPayload[5] = {1, 2, 3, 1, 2};
   uint8_t expectedPayload[6] = {5, 1, 2, 3, 1, 2};
   uint32_t testSourceId = 0x1C9;
   uint32_t testDestinationId = 0xDC;
   canTp->SendMessage(testPayload, sizeof(testPayload), testSourceId,
testDestinationId); auto txFrames = mockInterface.GetTxMessages(0xDC);
   ASSERT_FALSE(memcmp(&txFrames[0][0], expectedPayload, txFrames[0].size()));
   */
//}