#pragma once
#include<stdint.h>
#include"CanTpConstants.h"

struct CanTpConnection
{
    uint32_t source;
    uint32_t destination;
    E_DIRECTION direction;
    E_STATE state;
    E_TIME_TYPE timeType;
    uint16_t separationTime;
    uint8_t blockSize;
};