#include <gtest/gtest.h>
#include "Utils.h"

TEST(UtilTest, ModifyBitsOfByte) 
{
    uint8_t testByte = 0x53;
    Utils::SetBits<uint8_t>(testByte, 6, 3, 6);

    ASSERT_EQ(testByte, 0x33);
}

TEST(UtilTest, ModifyByte) 
{
    uint8_t testByte = 0x53;
    Utils::SetBits<uint8_t>(testByte, 7, 0, 6);

    ASSERT_EQ(testByte, 0x6);
}