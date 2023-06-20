#pragma once

#include "CanTpConstants.h"
#include "CanTypes.h"
#include "Utils.h"
#include <algorithm>
#include <array>
#include <stdint.h>
#include <type_traits>

template <typename T> class SingleFrameBuilder
{
  using frame_type =
      typename std::conditional<std::is_same<T, StandardCan>::value,
                                std::array<uint8_t, STANDARD_CAN_LENGTH>,
                                std::array<uint8_t, CAN_FD_LENGTH> >::type;

private:
  frame_type _buffer;
  uint8_t
  CreateFirstByte (uint32_t length)
  {
    uint8_t firstByte = 0;
    Utils::SetBits (firstByte, 7, 4,
                    static_cast<uint8_t> (E_FRAME_TYPE::SingleFrame));
    Utils::SetBits (firstByte, 3, 0, static_cast<uint8_t> (length));
    return firstByte;
  }
  constexpr uint8_t
  CreateFirstByteExtended ()
  {
    uint8_t firstByte = 0;
    Utils::SetBits (firstByte, 7, 4,
                    static_cast<uint8_t> (E_FRAME_TYPE::SingleFrame));
    return firstByte;
  }

public:
  SingleFrameBuilder () = default;
  static bool IsSingleFrame (uint32_t length);
  uint8_t *BuildFrame (uint8_t *payload, uint32_t payloadLength,
                       uint8_t &frameLength);
};

template <>
bool
SingleFrameBuilder<StandardCan>::IsSingleFrame (uint32_t length)
{
  return (length
          <= (STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
bool
SingleFrameBuilder<CanFd>::IsSingleFrame (uint32_t length)
{
  return (length <= (CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
uint8_t *
SingleFrameBuilder<StandardCan>::BuildFrame (uint8_t *payload,
                                             uint32_t payloadLength,
                                             uint8_t &frameLength)
{
  frameLength = 0;
  _buffer[0] = CreateFirstByte (payloadLength);
  frameLength += STANDARD_SINGLE_FRAME_HEADER_LENGTH;
  std::copy_n (payload, payloadLength,
               std::begin (_buffer) + STANDARD_SINGLE_FRAME_HEADER_LENGTH);
  frameLength += payloadLength;
  return _buffer.data ();
}

template <>
uint8_t *
SingleFrameBuilder<CanFd>::BuildFrame (uint8_t *payload,
                                       uint32_t payloadLength,
                                       uint8_t &frameLength)
{

  frameLength = 0;
  _buffer[0] = CreateFirstByteExtended ();
  _buffer[1] = static_cast<uint8_t> (payloadLength);
  frameLength += FD_SINGLE_FRAME_HEADER_LENGTH;

  std::copy_n (payload, payloadLength,
               std::begin (_buffer) + FD_SINGLE_FRAME_HEADER_LENGTH);
  frameLength += payloadLength;
  CanUtils::AddPadding (_buffer.data (), frameLength);
  return _buffer.data ();
}