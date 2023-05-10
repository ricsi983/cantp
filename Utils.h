#include <limits>
#include <stdint.h>

struct Utils
{

    template <typename T>
    static void SetBits(T &valueToModify, uint8_t from, uint8_t to, T value)
    {
        T maxValue = std::numeric_limits<T>::max();
        T part1 = maxValue << to;
        T part2 = maxValue >> (sizeof(T) * 8 - 1 - from);
        T mask = ~(part1 & part2);
        T maskedValue = mask & valueToModify;
        T valueToAdd = value << to;
        valueToModify = maskedValue | valueToAdd;
    }
};