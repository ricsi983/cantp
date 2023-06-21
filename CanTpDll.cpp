#include "CanTpDll.h"
#include "CanTp.h"
#include <memory>
#include <variant>

using CanTpVariant = std::variant<CanTp<CanFd>, CanTp<StandardCan> >;
static CanInterface canFdInterface;
static CanInterface standardCanInterface;
static CanTp<CanFd> canFdChannel (canFdInterface);
static CanTp<StandardCan> standardCanChannel (standardCanInterface);

void
Init (bool fd, SendFunctionType canSend, ReceiveFunctionType canReceive)
{

  if (fd)
    {
      canFdInterface.SetSendFunction (canSend);
      canFdInterface.SetReceiveFunction (canReceive);
    }
  else
    {
      standardCanInterface.SetSendFunction (canSend);
      standardCanInterface.SetReceiveFunction (canReceive);
    }
}

uint32_t
SendMessage (bool fd, uint8_t *payload, uint32_t length, uint32_t source,
             uint32_t destination)
{
  if (fd)
    canFdChannel.SendMessage (payload, length, source, destination);
  else
    standardCanChannel.SendMessage (payload, length, source, destination);
  return SUCCESS;
}

uint32_t
ReceiveMessage (bool fd, uint8_t *payload, uint32_t &length, uint32_t source,
                uint32_t destination)
{
  if (fd)
    canFdChannel.ReceiveMessage (payload, length, source, destination);
  else
    standardCanChannel.ReceiveMessage (payload, length, source, destination);
  return SUCCESS;
}