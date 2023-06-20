#include "Utils.h"
#include <gtest/gtest.h>

TEST (UtilTest, ModifyBitsOfByte)
{
  uint8_t testByte = 0x53;
  Utils::SetBits<uint8_t> (testByte, 6, 3, 6);

  ASSERT_EQ (testByte, 0x33);
}

TEST (UtilTest, ModifyByte)
{
  uint8_t testByte = 0x53;
  Utils::SetBits<uint8_t> (testByte, 7, 0, 6);

  ASSERT_EQ (testByte, 0x6);
}

TEST (UtilTest, GetDlcNoPadding)
{
  std::array<uint8_t, 16> lengths
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

  for (uint8_t i = 0; i < lengths.size (); ++i)
    {
      uint8_t dlc;
      ASSERT_TRUE (CanUtils::GetDlc (lengths[i], dlc));
      ASSERT_EQ (dlc, i);
    }
}

TEST (UtilTest, GetDlcMaxPadding)
{
  std::array<uint8_t, 16> lengths
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };
  for (uint8_t i = 0; i < lengths.size (); ++i)
    {
      ++lengths[i];
      uint8_t dlc;
      if (i == lengths.size () - 1)
        {
          ASSERT_FALSE (CanUtils::GetDlc (lengths[i], dlc));
        }
      else
        {
          ASSERT_TRUE (CanUtils::GetDlc (lengths[i], dlc));
          ASSERT_EQ (dlc, i + 1);
        }
    }
}

TEST (UtilTest, AddPaddingNoPadding)
{
  std::array<uint8_t, 16> lengths
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };
  std::array<uint8_t, CAN_FD_LENGTH> frame;
  frame.fill (1);
  std::array<uint8_t, CAN_FD_LENGTH> expectedFrame;
  expectedFrame.fill (1);

  for (uint8_t i = 0; i < lengths.size (); ++i)
    {
      CanUtils::AddPadding (frame.data (), lengths[i]);
      ASSERT_EQ (expectedFrame, frame);
    }
}

TEST (UtilTest, AddPaddingMaxPadding)
{
  std::array<uint8_t, 16> lengths
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };
  std::array<uint8_t, CAN_FD_LENGTH> frame;
  frame.fill (1);
  std::array<uint8_t, CAN_FD_LENGTH> expectedFrame;
  expectedFrame.fill (1);
  for (uint8_t i = 8; i < lengths.size () - 1; i++)
    {
      uint8_t padding = lengths[i + 1] - lengths[i] - 1;
      std::memset (expectedFrame.data () + lengths[i] + 1, PADDING_BYTE,
                   padding);
      uint8_t frameLength = lengths[i] + 1;
      CanUtils::AddPadding (frame.data (), frameLength);
      ASSERT_EQ (frame, expectedFrame);
      expectedFrame.fill (1);
      frame.fill (1);
    }
}