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

TEST (FrameBuilderTest, BuildStandardSingleFrameSmallest)
{
  constexpr uint32_t length = 1;
  std::array<uint8_t, length> payload = { 2 };
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFrame = { 1, 2 };
  uint8_t frameLength = 0;

  SingleFrameBuilder<StandardCan> frameBuilder;
  auto resultFrame = frameBuilder.BuildFrame (payload.data (), payload.size (),
                                              frameLength);
  ASSERT_EQ (frameLength,
             STANDARD_SINGLE_FRAME_HEADER_LENGTH + payload.size ());
  ASSERT_FALSE (memcmp (expectedFrame.data (), resultFrame, frameLength));
}

TEST (FrameBuilderTest, BuildStandardSingleFrameLargest)
{
  constexpr uint32_t length
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH;
  std::array<uint8_t, length> payload = { 1, 2, 3, 4, 5, 6, 7 };
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFrame
      = { 7, 1, 2, 3, 4, 5, 6, 7 };
  uint8_t frameLength = 0;

  SingleFrameBuilder<StandardCan> frameBuilder;
  auto resultFrame = frameBuilder.BuildFrame (payload.data (), payload.size (),
                                              frameLength);
  ASSERT_EQ (frameLength,
             STANDARD_SINGLE_FRAME_HEADER_LENGTH + payload.size ());
  ASSERT_FALSE (std::memcmp (expectedFrame.data (), resultFrame, frameLength));
}

TEST (FrameBuilderTest, BuildExtendedSingleFrameSmallest)
{
  constexpr uint32_t length = 1;
  std::array<uint8_t, length> payload;
  payload.fill (2);
  std::array<uint8_t, FD_SINGLE_FRAME_HEADER_LENGTH + length> expectedFrame;
  expectedFrame.fill (2);
  expectedFrame[0] = 0;
  expectedFrame[1] = length;

  SingleFrameBuilder<CanFd> frameBuilder;
  uint8_t frameLength = 0;
  auto resultFrame = frameBuilder.BuildFrame (payload.data (), payload.size (),
                                              frameLength);
  ASSERT_EQ (frameLength, expectedFrame.size ());
  ASSERT_FALSE (std::memcmp (expectedFrame.data (), resultFrame, frameLength));
}

TEST (FrameBuilderTest, BuildExtendedSingleFrameLargest)
{
  constexpr uint32_t length = CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH;
  std::array<uint8_t, length> payload;
  payload.fill (3);
  std::array<uint8_t, FD_SINGLE_FRAME_HEADER_LENGTH + length> expectedFrame;
  expectedFrame.fill (3);
  expectedFrame[0] = 0;
  expectedFrame[1] = length;

  SingleFrameBuilder<CanFd> frameBuilder;
  uint8_t frameLength = 0;
  auto resultFrame = frameBuilder.BuildFrame (payload.data (), payload.size (),
                                              frameLength);
  ASSERT_EQ (frameLength, expectedFrame.size ());
  ASSERT_FALSE (std::memcmp (expectedFrame.data (), resultFrame, frameLength));
}

TEST (FrameBuilderTest, BuildExtendedSingleFrameWithPadding)
{
  constexpr uint32_t length = 32;
  std::array<uint8_t, length> payload;
  payload.fill (4);
  std::array<uint8_t, 48> expectedFrame;
  expectedFrame.fill (4);
  expectedFrame[0] = 0;
  expectedFrame[1] = length;
  std::memset (expectedFrame.data () + length + FD_SINGLE_FRAME_HEADER_LENGTH,
               PADDING_BYTE,
               expectedFrame.size () - length - FD_SINGLE_FRAME_HEADER_LENGTH);

  SingleFrameBuilder<CanFd> frameBuilder;
  uint8_t frameLength = 0;

  auto resultFrame = frameBuilder.BuildFrame (payload.data (), payload.size (),
                                              frameLength);
  ASSERT_EQ (frameLength, expectedFrame.size ());
  ASSERT_FALSE (std::memcmp (expectedFrame.data (), resultFrame, frameLength));
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

TEST (FrameBuilderTest, BuildMultiFrameStandardSmallest)
{
  constexpr uint16_t payloadLength
      = STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH + 1;
  std::array<uint8_t, payloadLength> payload;
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFirstFrame;
  std::array<uint8_t, 3> expectedConsecutiveFrame;
  uint8_t payloadCounter = 1;
  uint8_t sequenceNumber = 1;
  expectedFirstFrame.fill (payloadCounter);
  expectedFirstFrame[0] = static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame) << 4
                          | static_cast<uint8_t> (payloadLength << 8);
  expectedFirstFrame[1] = static_cast<uint8_t> (payloadLength);
  expectedConsecutiveFrame.fill (++payloadCounter);
  expectedConsecutiveFrame[0]
      = static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame) << 4
        | sequenceNumber;

  MultiFrameBuilder<StandardCan> frameBuilder;
  payloadCounter = 1;
  payload.fill (payloadCounter);
  auto firstFrame
      = frameBuilder.BuildFirstFrame (payload.data (), payloadLength);
  ASSERT_FALSE (std::memcmp (firstFrame, expectedFirstFrame.data (),
                             STANDARD_CAN_LENGTH));
  ASSERT_FALSE (frameBuilder.IsFinished ());
  payload.fill (++payloadCounter);
  uint8_t frameLength;
  auto consecutiveFrame
      = frameBuilder.BuildConsecutiveFrame (payload.data (), frameLength);
  ASSERT_EQ (frameLength, expectedConsecutiveFrame.size ());
  ASSERT_FALSE (
      std::memcmp (consecutiveFrame, expectedConsecutiveFrame.data (), 2));
  ASSERT_TRUE (frameBuilder.IsFinished ());
}

TEST (FrameBuilderTest, BuildMultiFrameStandardLargest)
{
  constexpr uint16_t payloadLength = MULTI_FRAME_MAX_SIZE;
  std::array<uint8_t, payloadLength> payload;
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFirstFrame;
  std::array<std::array<uint8_t, STANDARD_CAN_LENGTH>,
             (payloadLength - STANDARD_CAN_LENGTH + FIRST_FRAME_HEADER_LENGTH)
                 / (STANDARD_CAN_LENGTH - CONSECUTIVE_FRAME_HEADER_LENGTH)>
      expectedConsecutiveFrames;
  std::array<uint8_t,
             (payloadLength - STANDARD_CAN_LENGTH + FIRST_FRAME_HEADER_LENGTH)
                     % (STANDARD_CAN_LENGTH - CONSECUTIVE_FRAME_HEADER_LENGTH)
                 + CONSECUTIVE_FRAME_HEADER_LENGTH>
      expectedLastFrame;
  uint8_t payloadCounter = 1;
  uint8_t sequenceNumber = 1;
  expectedFirstFrame.fill (payloadCounter);
  expectedFirstFrame[0] = static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame) << 4
                          | static_cast<uint8_t> (payloadLength >> 8);
  expectedFirstFrame[1] = static_cast<uint8_t> (payloadLength);
  for (uint32_t i = 0; i < expectedConsecutiveFrames.size (); ++i)
    {
      expectedConsecutiveFrames[i].fill (++payloadCounter);
      expectedConsecutiveFrames[i][0]
          = static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame) << 4
            | sequenceNumber++;
      if (sequenceNumber > MAX_SEQUENCE_NUMBER)
        sequenceNumber = 0;
    }
  expectedLastFrame.fill (payloadCounter);
  expectedLastFrame[0] = static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame)
                             << 4
                         | sequenceNumber;

  MultiFrameBuilder<StandardCan> frameBuilder;
  payloadCounter = 1;
  payload.fill (payloadCounter);
  auto firstFrame
      = frameBuilder.BuildFirstFrame (payload.data (), payloadLength);
  ASSERT_FALSE (std::memcmp (firstFrame, expectedFirstFrame.data (),
                             STANDARD_CAN_LENGTH));

  uint8_t frameLength;
  for (uint32_t i = 0; i < expectedConsecutiveFrames.size (); ++i)
    {
      payload.fill (++payloadCounter);
      auto consecutiveFrame
          = frameBuilder.BuildConsecutiveFrame (payload.data (), frameLength);
      ASSERT_EQ (frameLength, expectedConsecutiveFrames[i].size ());
      ASSERT_FALSE (std::memcmp (consecutiveFrame,
                                 expectedConsecutiveFrames[i].data (),
                                 frameLength));
    }

  auto lastFrame
      = frameBuilder.BuildConsecutiveFrame (payload.data (), frameLength);
  ASSERT_EQ (frameLength, expectedLastFrame.size ());
  ASSERT_FALSE (
      std::memcmp (expectedLastFrame.data (), lastFrame, frameLength));
  ASSERT_TRUE (frameBuilder.IsFinished ());
}

TEST (FrameBuilderTest, BuildMultiFrameStandardExtented)
{
  constexpr uint16_t payloadLength = MULTI_FRAME_MAX_SIZE + 1;
  std::array<uint8_t, payloadLength> payload;
  std::array<uint8_t, STANDARD_CAN_LENGTH> expectedFirstFrame;
  std::array<std::array<uint8_t, STANDARD_CAN_LENGTH>,
             (payloadLength - STANDARD_CAN_LENGTH
              + EXTENDED_FIRST_FRAME_HEADER_LENGTH)
                 / (STANDARD_CAN_LENGTH - CONSECUTIVE_FRAME_HEADER_LENGTH)>
      expectedConsecutiveFrames;
  std::array<uint8_t,
             (payloadLength - STANDARD_CAN_LENGTH
              + EXTENDED_FIRST_FRAME_HEADER_LENGTH)
                     % (STANDARD_CAN_LENGTH - CONSECUTIVE_FRAME_HEADER_LENGTH)
                 + CONSECUTIVE_FRAME_HEADER_LENGTH>
      expectedLastFrame;
  uint8_t payloadCounter = 1;
  uint8_t sequenceNumber = 1;
  expectedFirstFrame.fill (payloadCounter);
  expectedFirstFrame[0]
      = static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame) << 4 | 0;
  expectedFirstFrame[1] = 0;
  expectedFirstFrame[2] = static_cast<uint8_t> (payloadLength >> 24);
  expectedFirstFrame[3] = static_cast<uint8_t> (payloadLength >> 16);
  expectedFirstFrame[4] = static_cast<uint8_t> (payloadLength >> 8);
  expectedFirstFrame[5] = static_cast<uint8_t> (payloadLength);
  for (uint32_t i = 0; i < expectedConsecutiveFrames.size (); ++i)
    {
      expectedConsecutiveFrames[i].fill (++payloadCounter);
      expectedConsecutiveFrames[i][0]
          = static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame) << 4
            | sequenceNumber++;
      if (sequenceNumber > MAX_SEQUENCE_NUMBER)
        sequenceNumber = 0;
    }
  expectedLastFrame.fill (payloadCounter);
  expectedLastFrame[0] = static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame)
                             << 4
                         | sequenceNumber;

  MultiFrameBuilder<StandardCan> frameBuilder;
  payloadCounter = 1;
  payload.fill (payloadCounter);
  auto firstFrame
      = frameBuilder.BuildFirstFrame (payload.data (), payloadLength);
  ASSERT_FALSE (std::memcmp (firstFrame, expectedFirstFrame.data (),
                             STANDARD_CAN_LENGTH));

  uint8_t frameLength;
  for (uint32_t i = 0; i < expectedConsecutiveFrames.size (); ++i)
    {
      payload.fill (++payloadCounter);
      auto consecutiveFrame
          = frameBuilder.BuildConsecutiveFrame (payload.data (), frameLength);
      ASSERT_EQ (frameLength, expectedConsecutiveFrames[i].size ());
      ASSERT_FALSE (std::memcmp (consecutiveFrame,
                                 expectedConsecutiveFrames[i].data (),
                                 frameLength));
    }

  auto lastFrame
      = frameBuilder.BuildConsecutiveFrame (payload.data (), frameLength);
  ASSERT_EQ (frameLength, expectedLastFrame.size ());
  ASSERT_FALSE (
      std::memcmp (expectedLastFrame.data (), lastFrame, frameLength));
  ASSERT_TRUE (frameBuilder.IsFinished ());
}

TEST (FrameBuilderTest, BuildMultiFrameFd) {}

TEST (FrameBuilderTest, BuildMultiFrameFdExtended) {}

/*
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
*/