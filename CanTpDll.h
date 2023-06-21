#include <stdint.h>

constexpr uint32_t SUCCESS = 0U;

using SendFunctionType = uint32_t (*) (uint8_t *, uint8_t, uint32_t);
using ReceiveFunctionType = uint32_t (*) (uint8_t *, uint8_t *, uint32_t);

void Init (bool fd, SendFunctionType canSend, ReceiveFunctionType canReceive);
uint32_t SendMessage (bool fd, uint8_t *payload, uint32_t length,
                      uint32_t source, uint32_t destination);
uint32_t ReceiveMessage (bool fd, uint8_t *payload, uint32_t &length,
                         uint32_t source, uint32_t destination);