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
  frame_type &BuildFrame (uint8_t *payload, uint32_t length);
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
SingleFrameBuilder<StandardCan>::frame_type &
SingleFrameBuilder<StandardCan>::BuildFrame (uint8_t *payload, uint32_t length)
{
  _buffer[0] = CreateFirstByte (length);
  std::copy_n (payload, length,
               std::begin (_buffer) + STANDARD_SINGLE_FRAME_HEADER_LENGTH);

  return _buffer;
}

template <>
SingleFrameBuilder<CanFd>::frame_type &
SingleFrameBuilder<CanFd>::BuildFrame (uint8_t *payload, uint32_t length)
{

  _buffer[0] = CreateFirstByteExtended ();
  _buffer[1] = static_cast<uint8_t> (length);

  std::copy_n (payload, length,
               std::begin (_buffer) + FD_SINGLE_FRAME_HEADER_LENGTH);
  return _buffer;
}