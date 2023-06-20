#include "CanInterface.h"

void
CanInterface::SetSendFunction (SendFunctionType sendFunction)
{
  _sendFunction = sendFunction;
}

void
CanInterface::SetReceiveFunction (ReceiveFunctionType receiveFunction)
{
  _receiveFunction = receiveFunction;
}

void
CanInterface::SetTimeoutFunction (SetTimeoutFunctionType setTimeoutFunction)
{
  _setTimeoutFunction = setTimeoutFunction;
}

uint32_t
CanInterface::SendMessage (uint8_t *buffer, uint8_t length,
                           uint32_t destination)
{
  return _sendFunction (buffer, length, destination);
}

uint32_t
CanInterface::ReceiveMessage (uint8_t *buffer, uint8_t *length,
                              uint32_t source)
{
  return _receiveFunction (buffer, length, source);
}

void
CanInterface::SetTimeout (uint32_t timeout)
{
  _setTimeoutFunction (timeout);
}

bool
CanInterface::IsCanFd ()
{
  return _canFd;
}

void
CanInterface::SetCanFd (bool canFd)
{
  _canFd = canFd;
}