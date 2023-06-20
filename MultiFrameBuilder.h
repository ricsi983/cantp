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
  using microseconds = std::chrono::microseconds;
  using milliseconds = std::chrono::milliseconds;

private:
  frame_type _buffer;
  uint32_t _length;
  uint32_t _index;
  uint8_t _sequenceNumber;
  microseconds _separationTime;
  uint8_t _blockSize;
  E_FLOW_STATUS _flowStatus;

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

  void
  FillExtendedLength ()
  {
    _buffer[2] = static_cast<uint8_t> (_length >> 24);
    _buffer[3] = static_cast<uint8_t> (_length >> 16);
    _buffer[4] = static_cast<uint8_t> (_length >> 8);
    _buffer[5] = static_cast<uint8_t> (_length);
  }

  uint8_t
  CalculateFrameLength ()
  {
    return (_length - _index
            >= _buffer.size () - CONSECUTIVE_FRAME_HEADER_LENGTH)
               ? _buffer.size () - CONSECUTIVE_FRAME_HEADER_LENGTH
               : (_length - _index);
  }

  microseconds
  ConvertSeparationTime (uint8_t st)
  {
    if (st <= ST_MS_RANGE_MAX)
      {
        return std::chrono::duration_cast<microseconds> ((milliseconds (st)));
      }
    else if (st > ST_MS_RESERVED_MAX && st <= ST_US_RANGE_MAX)
      {
        return std::chrono::microseconds (st - ST_US_RANGE_RESERVED_MIN) * 100;
      }
    else
      {
        return std::chrono::duration_cast<microseconds> (
            (milliseconds (ST_MS_RANGE_MAX)));
      }
  }

public:
  MultiFrameBuilder ()
      : _length (0), _index (0), _sequenceNumber (1), _separationTime (0),
        _blockSize (0), _flowStatus (E_FLOW_STATUS::Continue)
  {
  }

  uint8_t *
  BuildFirstFrame (uint8_t *payload, uint32_t payloadLength)
  {
    _length = payloadLength;

    if (_length <= MULTI_FRAME_MAX_SIZE)
      {
        _buffer[0] = CreateFirstFrameFirstByte (_length);
        _buffer[1] = (static_cast<uint8_t> (_length));
        _index += (_buffer.size () - FIRST_FRAME_HEADER_LENGTH);
        std::copy_n (payload, _index,
                     std::begin (_buffer) + FIRST_FRAME_HEADER_LENGTH);
      }
    else
      {
        _buffer[0] = CreateFirstFrameFirstByteExtended ();
        _buffer[1] = 0;
        FillExtendedLength ();
        _index += (_buffer.size () - EXTENDED_FIRST_FRAME_HEADER_LENGTH);
        std::copy_n (payload, _index,
                     std::begin (_buffer)
                         + EXTENDED_FIRST_FRAME_HEADER_LENGTH);
      }
    return _buffer.data ();
  }

  uint8_t *
  BuildConsecutiveFrame (uint8_t *payload, uint8_t &frameLength)
  {
    _buffer[0] = CreateConsecutiveFrameFirstByte ();
    frameLength = CalculateFrameLength ();
    std::copy_n (payload + _index, frameLength,
                 std::begin (_buffer) + CONSECUTIVE_FRAME_HEADER_LENGTH);
    _index += frameLength;
    frameLength += CONSECUTIVE_FRAME_HEADER_LENGTH;
    CanUtils::AddPadding (_buffer.data (), frameLength);
    _sequenceNumber = (_sequenceNumber + 1) % (MAX_SEQUENCE_NUMBER + 1);
    return _buffer.data ();
  }

  void
  ParseFlowControlFrame (uint8_t *frame)
  {
    _flowStatus = static_cast<E_FLOW_STATUS> (frame[0] & 0x0F);
    _blockSize = frame[1];
    _separationTime = ConvertSeparationTime (frame[3]);
  }

  microseconds
  GetSeparationTime ()
  {
    return _separationTime;
  }
  uint8_t
  GetBlockSize ()
  {
    return _blockSize;
  }

  E_FLOW_STATUS
  GetFlowStatus () { return _flowStatus; }

  bool
  IsFinished ()
  {
    return (_index == _length);
  }
};