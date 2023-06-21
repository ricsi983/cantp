#pragma once
#include "CanInterface.h"
#include "MultiFrameBuilder.h"
#include "SingleFrameBuilder.h"
#include <stdint.h>
#include <thread>

struct MessageHandle
{
  uint8_t *payload;
  uint32_t length;
  uint32_t source;
  uint32_t destination;
};

template <typename T> class FrameSender
{
private:
  MessageHandle _handle;
  CanInterface &_interface;

  bool
  FlowControl (MultiFrameBuilder<T> &messageBuilder)
  {
    bool success = false;
    uint8_t frameLength = 0;
    std::array<uint8_t, CanUtils::GetLength<T> ()> flowControlFrame;
    if (!_interface.ReceiveMessage (flowControlFrame.data (), &frameLength,
                                    _handle.source))
      {
        success = (frameLength != 0);
        success &= (CanUtils::GetFrameType (flowControlFrame[0])
                    == E_FRAME_TYPE::FlowControlFrame);
      }

    if (success)
      {
        messageBuilder.ParseFlowControlFrame (flowControlFrame.data ());
        success
            &= (messageBuilder.GetFlowStatus () == E_FLOW_STATUS::Continue);
      }

    return success;
  }

  bool
  Connect (MultiFrameBuilder<T> &messageBuilder)
  {
    bool success = false;
    uint8_t length = 0;
    auto firstFrame
        = messageBuilder.BuildFirstFrame (_handle.payload, _handle.length);
    if (!_interface.SendMessage (firstFrame, CanUtils::GetLength<T> (),
                                 _handle.destination))
      {
        success = FlowControl (messageBuilder);
      }

    return success;
  }

  bool
  SendBlocks (MultiFrameBuilder<T> messageBuilder)
  {
    uint8_t frameLength = 0;
    bool success = true;
    auto separationTime = messageBuilder.GetSeparationTime ();
    auto blockSize = messageBuilder.GetBlockSize ();
    uint8_t blockNumber = 0;
    while (!messageBuilder.IsFinished ())
      {
        auto consecutiveFrame = messageBuilder.BuildConsecutiveFrame (
            _handle.payload, frameLength);
        _interface.SendMessage (consecutiveFrame, frameLength,
                                _handle.destination);
        std::this_thread::sleep_for (separationTime);
        if (blockSize != 0)
          {
            ++blockNumber;
            if (blockNumber == blockSize)
              {
                if (!FlowControl (messageBuilder))
                  {
                    success = false;
                    break;
                  }
                blockNumber = 0;
                separationTime = messageBuilder.GetSeparationTime ();
                blockSize = messageBuilder.GetBlockSize ();
              }
          }
      }
    return success;
  }

public:
  FrameSender (CanInterface &interface, MessageHandle handle)
      : _interface (interface), _handle (handle)
  {
  }

  void
  operator() (SingleFrameBuilder<T> messageBuilder)
  {

    uint8_t frameLength = 0;
    auto singleFrameMessage = messageBuilder.BuildFrame (
        _handle.payload, _handle.length, frameLength);
    _interface.SendMessage (singleFrameMessage, frameLength,
                            _handle.destination);
  }

  void
  operator() (MultiFrameBuilder<T> messageBuilder)
  {
    if (Connect (messageBuilder))
      {
        SendBlocks (messageBuilder);
      }
  }
};