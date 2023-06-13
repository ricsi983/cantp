#pragma once

#include "CanInterface.h"
#include "CanTpConnection.h"
#include "SingleFrameBuilder.h"

template <typename T> class CanTp_2
{
private:
  CanInterface &_canInterface;

public:
  CanTp_2 (CanInterface &canInterface);
};

/*
template <uint8_t frameLength> class CanTp_2 {
private:
  CanInterface &_canInterface;
  CanTpConnection Connect(uint32_t source, uint32_t destination,
                          uint8_t *payload, uint32_t length);
  CanTpConnection Listen(uint32_t source, uint32_t destination,
                         uint8_t *payload, uint32_t &length);

public:
  CanTp_2(CanInterface &canInterface);
  E_ERROR_CODE Send(uint32_t source, uint32_t destination, uint8_t *payload,
                    uint32_t length);
  E_ERROR_CODE Receive(uint32_t source, uint32_t destination, uint8_t *payload,
                       uint32_t &length);
};

template <uint8_t frameLength>
CanTp_2<frameLength>::CanTp_2(CanInterface &canInterface)
    : _canInterface(canInterface) {}

template <uint8_t frameLength>
CanTpConnection
CanTp_2<frameLength>::Connect(uint32_t source, uint32_t destination,
                              uint8_t *payload, uint32_t length) {
  CanTpConnection connection;
  connection.destination = destination;
  connection.source = source;
  connection.direction = E_DIRECTION::Send;
}

template <uint8_t frameLength>
CanTpConnection
CanTp_2<frameLength>::Listen(uint32_t source, uint32_t destination,
                             uint8_t *payload, uint32_t &length) {
  CanTpConnection connection;
  connection.destination = destination;
  connection.source = source;
  connection.direction = E_DIRECTION::Send;
}

template <uint8_t frameLength>
E_ERROR_CODE CanTp_2<frameLength>::Send(uint32_t source, uint32_t destination,
                                        uint8_t *payload, uint32_t length) {
  if (SingleFrameBuilder<frameLength>::IsSingleFrame(length)) {
    auto singleFrameBuilder = SingleFrameBuilder<frameLength>();
    auto frame = singleFrameBuilder.BuildFrame(payload, length);
  } else {
  }
}

template <uint8_t frameLength>
E_ERROR_CODE CanTp_2<frameLength>::Receive(uint32_t source,
                                           uint32_t destination,
                                           uint8_t *payload, uint32_t &length)
{

}*/