#pragma once
#include <stdint.h>

using SendFunctionType = uint32_t (*) (uint8_t *, uint8_t, uint32_t);
using ReceiveFunctionType = uint32_t (*) (uint8_t *, uint8_t *, uint32_t);
using SetTimeoutFunctionType = void (*) (uint32_t);

class CanInterface
{
private:
  SendFunctionType _sendFunction;
  ReceiveFunctionType _receiveFunction;
  SetTimeoutFunctionType _setTimeoutFunction;
  bool _canFd;

public:
  void SetSendFunction (SendFunctionType sendFunction);
  void SetReceiveFunction (ReceiveFunctionType receiveFunction);
  void SetTimeoutFunction (SetTimeoutFunctionType setTimeoutFunction);
  uint32_t SendMessage (uint8_t *buffer, uint8_t length, uint32_t destination);
  uint32_t ReceiveMessage (uint8_t *bufer, uint8_t *length, uint32_t source);
  void SetTimeout (uint32_t timeout);
  bool IsCanFd ();
  void SetCanFd (bool canFd);
};