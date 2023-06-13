#pragma once

#include "CanTpConstants.h"
#include "CanTypes.h"
#include "Utils.h"
#include <array>
#include <stdint.h>

template <typename T> class MultiFrameBuilder
{
  using frame_type =
      typename std::conditional<std::is_same<T, StandardCan>::value,
                                std::array<uint8_t, STANDARD_CAN_LENGTH>,
                                std::array<uint8_t, CAN_FD_LENGTH> >::type;

private:
  frame_type _buffer;
  uint32_t _length;
  uint32_t _index;
  uint8_t _sequenceNumber;

  uint8_t
  CreateFirstFrameFirstByte (uint32_t length)
  {
    uint8_t firstByte = 0;
    Utils::SetBits (firstByte, 7, 4,
                    static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame));
    Utils::SetBits (firstByte, 3, 0, static_cast<uint8_t> (length >> 8));
    return firstByte;
  }

  constexpr uint8_t
  CreateFirstFrameFirstByteExtended ()
  {
    uint8_t firstByte = 0;
    Utils::SetBits (firstByte, 7, 4,
                    static_cast<uint8_t> (E_FRAME_TYPE::FirstFrame));
    return firstByte;
  }

  uint8_t
  CreateConsecutiveFrameFirstByte ()
  {
    uint8_t firstByte = 0;
    Utils::SetBits (firstByte, 7, 4,
                    static_cast<uint8_t> (E_FRAME_TYPE::ConsecutiveFrame));
    Utils::SetBits (firstByte, 3, 0, _sequenceNumber);

    return firstByte;
  }

  uint8_t
  CalculateFrameLength ()
  {
    return (_length - _index
            >= _buffer.size () - CONSECUTIVE_FRAME_HEADER_LENGTH)
               ? _buffer.size () - CONSECUTIVE_FRAME_HEADER_LENGTH
               : (_length - _index);
  }

public:
  MultiFrameBuilder () : _length (0), _index (0), _sequenceNumber (1) {}

  frame_type &
  BuildFirstFrame (uint8_t *payload, uint32_t length)
  {
    _length = length;

    if (_length <= MULTI_FRAME_MAX_SIZE)
      {
        _buffer[0] = CreateFirstFrameFirstByte (length);
        _buffer[1] = (static_cast<uint8_t> (length));
        _index += _buffer.size () - FIRST_FRAME_HEADER_LENGTH;
        std::copy_n (payload, _index,
                     std::begin (_buffer) + FIRST_FRAME_HEADER_LENGTH);
      }
    else
      {
        _buffer[0] = CreateFirstFrameFirstByteExtended ();
        _buffer[1] = 0;
        std::copy_n (&length, sizeof (length),
                     std::begin (_buffer) + FIRST_FRAME_HEADER_LENGTH);
      }
    return _buffer;
  }

  frame_type &
  BuildConsecutiveFrame (uint8_t *payload)
  {
    _buffer[0] = CreateConsecutiveFrameFirstByte ();
    uint8_t frameLength = CalculateFrameLength ();
    std::copy_n (payload + _index, frameLength,
                 std::begin (_buffer) + CONSECUTIVE_FRAME_HEADER_LENGTH);
    _index += frameLength;
    _sequenceNumber = (_sequenceNumber + 1) % (MAX_SEQUENCE_NUMBER + 1);
    return _buffer;
  }

  bool
  IsFinished ()
  {
    return (_index >= _length);
  }
  static bool IsMultiFrame (uint32_t length);
};

template <>
bool
MultiFrameBuilder<StandardCan>::IsMultiFrame (uint32_t length)
{
  return (length
          > (STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
bool
MultiFrameBuilder<CanFd>::IsMultiFrame (uint32_t length)
{
  return (length > (CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH));
}