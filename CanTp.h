#include "CanInterface.h"
#include "CanTpConstants.h"
#include <memory>
#include <stdlib.h>
#include <vector>

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