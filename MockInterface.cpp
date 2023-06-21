#include "MockInterface.h"

MockInterface::MockInterface () : _timeout (false)
{
  _txFrames[0xDC] = std::vector<std::vector<uint8_t> > ();
  _rxFrames[0x1C9] = std::vector<std::vector<uint8_t> > ();
}

uint32_t
MockInterface::Send (uint8_t *buffer, uint8_t length, uint32_t id)
{
  std::vector<uint8_t> frame;
  frame.resize (length);
  memcpy (&frame[0], buffer, length);
  _txFrames.at (id).push_back (frame);
  return _sendResult;
}

std::vector<std::vector<uint8_t> >
MockInterface::GetTxMessages (uint32_t id)
{
  return _txFrames.at (id);
}

void
MockInterface::SetSendResult (uint32_t result)
{
  _sendResult = result;
}

void
MockInterface::SetReceiveResult (uint32_t result)
{
  _receiveResult = result;
}

void
MockInterface::SetTimeout (bool timeout)
{
  _timeout = timeout;
}

void
MockInterface::ClearTxMessages (uint32_t id)
{
  if (!_txFrames.at (id).empty ())
    _txFrames.at (id).clear ();
}

void
MockInterface::ClearRxMessages (uint32_t id)
{
  if (!_rxFrames.at (id).empty ())
    _rxFrames.at (id).clear ();
}

void
MockInterface::AddMessageToRx (uint32_t id, uint8_t *buffer, uint8_t length)
{
  std::vector<uint8_t> frame;
  frame.resize (length);
  memcpy (&frame[0], buffer, length);
  _rxFrames.at (id).push_back (frame);
}

uint32_t
MockInterface::Receive (uint8_t *buffer, uint8_t *length, uint32_t id)
{
  if (_rxFrames.at (id).size () != 0)
    {
      auto frame = _rxFrames.at (id).front ();
      *length = frame.size ();
      memcpy (buffer, &frame[0], frame.size ());
      _rxFrames.at (id).erase (_rxFrames.at (id).begin ());
    }
  return _receiveResult;
}

std::vector<uint8_t>
MockInterface::GetTxFrame (uint32_t id, uint32_t index)
{
  return _txFrames[id][index];
}

static MockInterface *mockInterfacePointer;

void
SetMockInterface (MockInterface *mockInterface)
{
  mockInterfacePointer = mockInterface;
}

uint32_t
MockSendInterface (uint8_t *buffer, uint8_t length, uint32_t id)
{
  return mockInterfacePointer->Send (buffer, length, id);
}

uint32_t
MockReceiveInterface (uint8_t *buffer, uint8_t *length, uint32_t id)
{
  return mockInterfacePointer->Receive (buffer, length, id);
}

void
MockSetTimeout (uint32_t timeout)
{
  mockInterfacePointer->SetTimeout (timeout);
}
