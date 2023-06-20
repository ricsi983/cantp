#include "SingleFrameBuilder.h"

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