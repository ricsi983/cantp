#pragma once

#include "CanTpConstants.h"
#include "CanTypes.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <limits>
#include <stdint.h>

struct Utils
{

  template <typename T>
  static void
  SetBits (T &valueToModify, uint8_t from, uint8_t to, T value)
  {
    T maxValue = std::numeric_limits<T>::max ();
    T part1 = maxValue << to;
    T part2 = maxValue >> (sizeof (T) * 8 - 1 - from);
    T mask = ~(part1 & part2);
    T maskedValue = mask & valueToModify;
    T valueToAdd = value << to;
    valueToModify = maskedValue | valueToAdd;
  }
};

struct CanUtils
{
  static bool
  GetDlc (uint8_t length, uint8_t &dlc)
  {
    std::array<uint8_t, 16> dlcs
        = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };
    bool found = false;

    for (uint8_t i = 0; i < dlcs.size (); ++i)
      {
        if (length <= dlcs[i])
          {
            found = true;
            dlc = i;
            break;
          }
      }

    return found;
  }

  static bool
  AddPadding (uint8_t *frame, uint8_t &length)
  {
    std::array<uint8_t, 16> dlcs
        = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };
    bool found = false;

    for (uint8_t i = 0; i < dlcs.size (); ++i)
      {
        if (length <= dlcs[i])
          {
            uint8_t padding = dlcs[i] - length;
            found = true;
            std::memset (frame + length, PADDING_BYTE, padding);
            length += padding;
            break;
          }
      }

    return found;
  }

  template <typename T> constexpr static uint8_t GetLength ();
  template <typename T> static bool IsMultiFrame (uint32_t length);
  template <typename T> static bool IsSingleFrame (uint32_t length);
};
