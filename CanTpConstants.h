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
constexpr auto ST_MS_RANGE_MIN = 0x00U;
constexpr auto ST_MS_RANGE_MAX = 0x7FU;
constexpr auto ST_MS_RESERVED_MIN = 0x80U;
constexpr auto ST_MS_RESERVED_MAX = 0xF0U;
constexpr auto ST_US_RANGE_MIN = 0xF1U;
constexpr auto ST_US_RANGE_MAX = 0xF9U;
constexpr auto ST_US_RANGE_RESERVED_MIN = 0xFAU;
constexpr auto ST_US_RANGE_RESERVED_MAX = 0xFFU;

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

enum class E_FLOW_STATUS : uint8_t
{
  Continue,
  Wait,
  Overflow
};