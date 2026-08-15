#ifndef DISPLAY_WINDOW_CONSTANTS_H
#define DISPLAY_WINDOW_CONSTANTS_H

#include "data/constants.h"

namespace display {

constexpr int BorderSpaces = 2;
constexpr int InputWindowHeight = 2;
constexpr int DataWindowHeight = data::NumValues * 2 + 2;
constexpr int HistogramWindowHeight = DataWindowHeight;

constexpr int DataWindowWidth = 29;

constexpr int DefaultColour = 0;
constexpr int RedBlackId = 1;

}  // namespace display

#endif