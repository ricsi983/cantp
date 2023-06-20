#include "Utils.h"

template <>
bool
CanUtils::IsSingleFrame<StandardCan> (uint32_t length)
{
  return (length
          <= (STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
bool
CanUtils::IsSingleFrame<CanFd> (uint32_t length)
{
  return (length <= (CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
bool
CanUtils::IsMultiFrame<StandardCan> (uint32_t length)
{
  return (length
          > (STANDARD_CAN_LENGTH - STANDARD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
bool
CanUtils::IsMultiFrame<CanFd> (uint32_t length)
{
  return (length > (CAN_FD_LENGTH - FD_SINGLE_FRAME_HEADER_LENGTH));
}

template <>
uint8_t
CanUtils::GetLength<CanFd> ()
{
  return CAN_FD_LENGTH;
}

template <>
uint8_t
CanUtils::GetLength<StandardCan> ()
{
  return STANDARD_CAN_LENGTH;
}