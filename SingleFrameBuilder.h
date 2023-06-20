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
  uint8_t *BuildFrame (uint8_t *payload, uint32_t payloadLength,
                       uint8_t &frameLength);
};
