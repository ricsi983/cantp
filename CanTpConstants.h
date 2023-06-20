#pragma once
#include <stdint.h>

constexpr auto STANDARD_CAN_LENGTH = 8U;
constexpr auto CAN_FD_LENGTH = 64U;
constexpr auto STANDARD_SINGLE_FRAME_HEADER_LENGTH = 1U;
constexpr auto FD_SINGLE_FRAME_HEADER_LENGTH = 2U;
constexpr auto MULTI_FRAME_MAX_SIZE = 0xFFFU;
constexpr auto EXTENDED_MULTI_FRAME_MAX_SIZE = 0xFFFFFFFFU;
constexpr auto FIRST_FRAME_HEADER_LENGTH = 2U;
constexpr auto EXTENDED_FIRST_FRAME_HEADER_LENGTH = 6U;
constexpr auto CONSECUTIVE_FRAME_HEADER_LENGTH = 1U;
constexpr auto MAX_SEQUENCE_NUMBER = 15U;
constexpr auto PADDING_BYTE = 0xCCU;

enum class E_DIRECTION
{
  Send,
  Receive,
};

enum class E_STATE
{
  Connected,
  ConnectionFailed,
};

enum class E_TIME_TYPE
{
  Milliseconds,
  Microseconds
};

enum class E_ERROR_CODE
{
  Success,
};

enum class E_FRAME_TYPE : uint8_t
{
  SingleFrame,
  FirstFrame,
  ConsecutiveFrame,
  FlowControlFrame
};
