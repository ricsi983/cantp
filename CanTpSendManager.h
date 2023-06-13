#pragma once
#include<stdint.h>
#include"CanTpConstants.h"
#include"CanTpConnection.h"

template<uint8_t frameLength>
class CanTpSendManager
{
    private:
        uint32_t _index;
        uint8_t _block;
        uint8_t _buffer[frameLength];
        CanTpConnection& _connection;
    public:
        CanTpSendManager(CanTpConnection& connection);
        void SendMessage(uint8_t* payload, uint32_t length);
        
};