#pragma once

#include<map>
#include<stdint.h>

class MockInterface
{
private:
    std::map<uint32_t, std::vector<std::vector<uint8_t>>> _txFrames; 
    std::map<uint32_t, std::vector<std::vector<uint8_t>>> _rxFrames;
    bool _timeout;
    uint32_t _sendResult;
    uint32_t _receiveResult;
public:
    MockInterface();
    uint32_t Send(uint8_t* buffer, uint32_t length, uint32_t id);
    void SetSendResult(uint32_t result);
    void SetReceiveResult(uint32_t result);
    void SetTimeout(bool timeout);
    void ClearTxMessages(uint32_t id);
    void ClearRxMessages(uint32_t id);
    std::vector<std::vector<uint8_t>> GetTxMessages(uint32_t id);
    void AddMessageToRx(uint32_t id, uint8_t* buffer, uint8_t length);
    uint32_t Receive(uint8_t* buffer, uint32_t* length, uint32_t id);
};


void SetMockInterface(MockInterface* mockInterface);
uint32_t MockSendInterface(uint8_t* buffer, uint32_t length, uint32_t id);
uint32_t MockReceiveInterface(uint8_t* buffer, uint32_t* length, uint32_t id);
void MockSetTimeout(uint32_t timeout);