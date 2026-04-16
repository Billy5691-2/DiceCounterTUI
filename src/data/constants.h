#ifndef DATA_CONSTANTS_H
#define DATA_CONSTANTS_H

#include <array>

namespace data {

constexpr int MinInputValue = 2;
constexpr int MaxInputValue = 12;
constexpr int NumValues = MaxInputValue - MinInputValue + 1;

constexpr std::array<double, NumValues> ExpectedProbabilites = { 2.78,  5.56,  8.33, 11.11, 13.89, 16.87,
                                                                 13.89, 11.11, 8.33, 5.56,  2.78 };

}  // namespace data

#endif