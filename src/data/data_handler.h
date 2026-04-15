#ifndef CALCULATION_DATA_HANDLER_H
#define CALCULATION_DATA_HANDLER_H

#include "data/constants.h"

#include <array>
#include <optional>
#include <vector>

namespace data {

class DataHandler {
public:
    DataHandler() = default;
    ~DataHandler() = default;

    DataHandler(const DataHandler&) = delete;
    DataHandler& operator=(const DataHandler&) = delete;

    DataHandler(DataHandler&&) = delete;
    DataHandler& operator=(DataHandler&&) = delete;

    void AccumulateInput(int charInput);
    void ResetInputValue();
    void EnterNewValue();
    const std::array<int, NumValues>& GetTally();
    const std::array<double, NumValues>& GetProbabilities();
    const std::vector<int>& GetPreviousInputs();
    int GetCurrentInput();

private:
    int m_currentInput = 0;
    // std::array<int, NumValues> m_valueTally;
    // std::array<double, NumValues> m_valueProbabilities;
    std::vector<int> m_previousInputs;

    bool ValidateInput();
    // void CalculateProbabilities();
    // void UpdateTally();
};

}  // namespace data

#endif