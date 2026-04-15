#include "data/data_handler.h"
#include "data/constants.h"

namespace data {

void DataHandler::AccumulateInput(const int charInput) {
    m_currentInput = (m_currentInput * 10) + (charInput - '0');
}

void DataHandler::ResetInputValue() {
    m_currentInput = 0;
}

void DataHandler::EnterNewValue() {
    if (!ValidateInput()) {
        m_currentInput = 0;
        return;
    }
    m_previousInputs.insert(m_previousInputs.begin(), m_currentInput);
    m_currentInput = 0;
    // UpdateTally(input);
    // CalculateProbabilities();
}

bool DataHandler::ValidateInput() {
    if (m_currentInput < MinInputValue || m_currentInput > MaxInputValue) {
        return false;
    }
    return true;
}

// void DataHandler::CalculateProbabilities() {
//     double total;
//     for (const auto tally : m_valueTally) {
//         total += static_cast<double>(tally);
//     }

//     for (int index = 0; index < NumValues; ++index) {
//         m_valueProbability[index] = static_cast<double>(m_valueTally[index]) / total;
//     }
// }

// void DataHandler::UpdateTally(const int input) {
//     ++m_valueTally[input];
// }

// const std::array<int, NumValues>& DataHandler::GetTally() {
//     return m_valueTally;
// }

// const std::array<double, NumValues>& DataHandler::GetProbabilities() {
//     return m_valueProbabilities;
// }

const std::vector<int>& DataHandler::GetPreviousInputs() {
    return m_previousInputs;
}

int DataHandler::GetCurrentInput() {
    return m_currentInput;
}

}  // namespace data