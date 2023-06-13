#include "CanTpConstants.h"
#include "MultiFrameBuilder.h"
#include "SingleFrameBuilder.h"
#include <gtest/gtest.h>

TEST (FrameBuilderTest, IsSingleFrameTestStandardLength)
{
  constexpr uint32_t testLength
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH;
  ASSERT_TRUE (SingleFrameBuilder<StandardCan>::IsSingleFrame (testLength));
}

TEST (FrameBuilderTest, IsSingleFrameTestStandardLengthFd)
{
  constexpr uint32_t testLength
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH;
  ASSERT_TRUE (SingleFrameBuilder<CanFd>::IsSingleFrame (testLength));
}

TEST (FrameBuilderTest, IsSingleFrameTestExtendedLengthFd)
{
  constexpr uint32_t testLength
      = CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH;
  ASSERT_TRUE (SingleFrameBuilder<CanFd>::IsSingleFrame (testLength));
}

TEST (FrameBuilderTest, IsSingleFrameTestStandardLengthFalse)
{
  constexpr uint32_t testLength = STANDARD_CAN_LENGTH;
  ASSERT_FALSE (SingleFrameBuilder<StandardCan>::IsSingleFrame (testLength));
}

TEST (FrameBuilderTest, IsSingleFrameTestStandardLengthFalseFd)
{
  constexpr uint32_t testLength
      = CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH + 1;
  ASSERT_FALSE (SingleFrameBuilder<CanFd>::IsSingleFrame (testLength));
}

TEST (FrameBuilderTest, BuildStandardSingleFrame)
{
  constexpr uint32_t length
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH;
  std::array<uint8_t, length> payload = { 1, 2, 3, 4, 5, 6, 7 };
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFrame
      = { 7, 1, 2, 3, 4, 5, 6, 7 };

  SingleFrameBuilder<StandardCan> frameBuilder;
  auto resultFrame
      = frameBuilder.BuildFrame (payload.data (), payload.size ());
  ASSERT_EQ (expectedFrame, resultFrame);
}

TEST (FrameBuilderTest, BuildExtendedSingleFrame)
{
  constexpr uint32_t length = CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH;
  std::array<uint8_t, length> payload;
  payload.fill (2);
  std::array<uint8_t, CAN_FD_LENGTH> expectedFrame;
  expectedFrame.fill (2);
  expectedFrame[0] = 0;
  expectedFrame[1] = length;

  SingleFrameBuilder<CanFd> frameBuilder;
  auto resultFrame
      = frameBuilder.BuildFrame (payload.data (), payload.size ());
  ASSERT_EQ (expectedFrame, resultFrame);
}

TEST (FrameBuilderTest, IsMultiFrameTestStandard)
{
  constexpr uint32_t testLength
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH + 1;
  ASSERT_TRUE (MultiFrameBuilder<StandardCan>::IsMultiFrame (testLength));
}

TEST (FrameBuilderTest, IsMultiFrameTestFd)
{
  constexpr uint32_t testLength
      = CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH + 1;
  ASSERT_TRUE (MultiFrameBuilder<StandardCan>::IsMultiFrame (testLength));
}

TEST (FrameBuilderTest, BuildMultiFrameStandard)
{
  // first frame + 3 * full consecutive frames + 1 consecutive frame with 1
  // byte
  constexpr uint32_t testLength
      = (STANDARD_CAN_LENGTH - FIRST_FRAME_HEADER_LENGTH)
        + 40 * (STANDARD_CAN_LENGTH - CONSECUTIVE_FRAME_HEADER_LENGTH) + 1;
  std::array<uint8_t, testLength> payload;
  uint32_t counter = 1;
  payload.fill (counter);

  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFirstFrame;
  expectedFirstFrame.fill (counter);
  expectedFirstFrame[0]
      = (static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame) << 4)
        | static_cast<uint8_t> (testLength >> 8);
  expectedFirstFrame[1] = static_cast<uint8_t> (testLength);

  MultiFrameBuilder<StandardCan> frameBuilder;

  auto firstFrame
      = frameBuilder.BuildFirstFrame (payload.data (), payload.size ());

  ASSERT_EQ (firstFrame, expectedFirstFrame);
  ++counter;
  while (frameBuilder.IsFinished () != true)
    {
      payload.fill (counter);
      auto consecutiveFrame
          = frameBuilder.BuildConsecutiveFrame (payload.data ());
      ++counter;
    }
}