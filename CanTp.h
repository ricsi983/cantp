#include "CanInterface.h"
#include "CanTpConstants.h"
#include "MultiFrameBuilder.h"
#include "SingleFrameBuilder.h"
#include "Utils.h"
#include <memory>
#include <stdlib.h>
#include <variant>
#include <vector>

template <typename T> class CanTp
{
private:
  struct FrameSendVisitor
  {
  private:
    uint8_t *_payload;
    uint32_t _length;
    uint32_t _source;
    uint32_t _destination;
    CanTp &_canTp;

  public:
    FrameSendVisitor (CanTp<T> &canTp, uint8_t *payload, uint32_t length,
                      uint32_t source, uint32_t destination)
        : _canTp (canTp), _payload (payload), _length (length),
          _source (source), _destination (destination)
    {
    }

    void
    operator() (SingleFrameBuilder<T> messageBuilder)
    {

      uint8_t frameLength = 0;
      auto singleFrameMessage
          = messageBuilder.BuildFrame (_payload, _length, frameLength);
      _canTp._interface->SendMessage (singleFrameMessage, frameLength,
                                      _destination);
    }

    void
    operator() (MultiFrameBuilder<T> messageBuilder)
    {
      // printf ("A string: %s\n", s.c_str ());
    }
  };

  CanInterface *_interface;
  bool
  Connect (uint8_t *firstFrame, uint8_t *flowControlFrame, uint32_t source,
           uint32_t destination)
  {
    bool success = false;
    uint8_t length = 0;
    if (_interface->SendMessage (firstFrame, CanUtils::GetLength<T> (),
                                 destination))
      {
        if (_interface->ReceiveMessage (flowControlFrame, &length, source))
          {
            success = (length != 0);
          }
      }

    return success;
  }

public:
  CanTp (CanInterface *interface) : _interface (interface) {}

  void
  SendMessage (uint8_t *payload, uint32_t length, uint32_t source,
               uint32_t destination)
  {
    std::variant<SingleFrameBuilder<T>, MultiFrameBuilder<T> > builder;
    builder = (CanUtils::IsSingleFrame<T> (length) ? SingleFrameBuilder<T> ()
                                                   : MultiFrameBuilder<T> ());
    std::visit (FrameSendVisitor (*this, payload, length, source, destination),
                builder);

    /*
    if (CanUtils::IsSingleFrame<T> (length))
      {
        SingleFrameBuilder<T> messageBuilder;
        uint8_t frameLength = 0;
        auto singleFrameMessage
            = messageBuilder.BuildFrame (payload, length, frameLength);
        _interface->SendMessage (singleFrameMessage, frameLength, destination);
      }
    else
      {
        MultiFrameBuilder<T> messageBuilder;
        std::array<uint8_t, CanUtils::GetLength<T> ()> flowControlFrame;
        auto firstFrame = messageBuilder.BuildFirstFrame (payload, length);

        if (Connect (firstFrame, flowControlFrame.data (), source,
                     destination))
          {
            uint8_t frameLength = 0;
            uint8_t blockNumber = 0;
            auto blockSize = messageBuilder.GetBlockSize ();
            auto separationTime = messageBuilder.GetSeparationTime ();
            while (!messageBuilder.IsFinished ())
              {
                auto consecutiveFrame = messageBuilder.BuildConsecutiveFrame (
                    payload, frameLength);
                _interface->SendMessage (consecutiveFrame, frameLength,
                                         destination);
                if (blockSize != 0)
                  {
                    ++blockNumber;
                    if (blockNumber == blockSize)
                      {
                        blockNumber = 0;
                        _interface->ReceiveMessage (flowControlFrame.data (),
                                                    frameLength, source);
                        messageBuilder.ParseFlowControlFrame (
                            flowControlFrame.data ());
                        blockSize = messageBuilder.GetBlockSize ();
                        separationTime = messageBuilder.GetSeparationTime ();
                        if (messageBuilder.GetFlowStatus ()
                            != E_FLOW_STATUS::Continue)
                          {
                            break;
                          }
                      }
                  }
                ++blockNumber;
              }
          }*/
  }
  /*
  if (length <= _frameSize - 1)
    {
      SingleFrameMessage singleFrameMessage
          = SingleFrameMessage (buffer, length, _frameSize);
      _interface->SendMessage (&singleFrameMessage.singleFrame[0],
                               singleFrameMessage.singleFrame.size (),
                               destination);
    }
  else
    {
      MultiFrameMessage multiFrameMessage
          = MultiFrameMessage (buffer, length, _frameSize);
      if (Connect (multiFrameMessage.firstFrame, source, destination))
        {
          // multiFrameMessage.AddFrameCounters(_sendBlockSize);
          while (multiFrameMessage.consecutiveFrames.size () != 0)
            if (!SendBlock (multiFrameMessage.consecutiveFrames, source,
                            destination))
              break;
        }
    }
    */
  void ReceiveMessage (uint8_t *payload, uint32_t &length, uint32_t source,
                       uint32_t destination);
};

/*
class SingleFrameMessage
{
private:
  void CreateSingleFrame (uint8_t *buffer, uint8_t length);
  uint8_t _frameSize;

public:
  std::vector<uint8_t> singleFrame;
  SingleFrameMessage (uint8_t *buffer, uint8_t length, uint8_t frameSize);
  SingleFrameMessage () = default;
  std::vector<uint8_t> GetPayload ();
};

class MultiFrameMessage
{
private:
  void CreateConsecutiveFrames (uint8_t *buffer, uint16_t length);
  void CreateFirstFrame (uint8_t *buffer, uint16_t length);
  uint8_t _frameSize;

public:
  std::vector<uint8_t> firstFrame;
  std::vector<std::vector<uint8_t> > consecutiveFrames;
  std::vector<uint8_t> flowControlFrame;
  MultiFrameMessage (uint8_t *buffer, uint16_t length, uint8_t frameSize);
  std::vector<uint8_t> GetPayload ();
  MultiFrameMessage () = default;
  void AddFrameCounters ();
};

class CanTp
{
private:
  uint8_t _receiveBlockSize;
  uint8_t _receiveSeparationTime;
  uint8_t _sendBlockSize;
  uint8_t _sendSeparationTime;
  std::shared_ptr<CanInterface> _interface;
  uint8_t _frameSize;

  void ParseFlowControlFrame (std::vector<uint8_t> &flowControlFrame);
  bool Connect (std::vector<uint8_t> &firstFrame, uint32_t source,
                uint32_t destination);
  bool SendBlock (std::vector<std::vector<uint8_t> > &block, uint32_t source,
                  uint32_t destination);
  E_FRAME_TYPE Listen (uint32_t source, uint32_t destination);

public:
  CanTp (std::shared_ptr<CanInterface> interface, uint8_t receiveBlockSize,
         uint8_t separationTime);
  void SendMessage (uint8_t *buffer, uint16_t length, uint32_t source,
                    uint32_t destination);
  void ReceiveMessage (uint8_t *buffer, uint16_t &length, uint32_t source,
                       uint32_t destination);
};
*/