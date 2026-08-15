#include "data/data_handler.h"
#include "constants.h"
#include "data/constants.h"

#include <filesystem>

#include <ctime>
#include <fstream>
#include <sstream>

namespace data {

static constexpr int MinimumDiceTotal = 2;
static constexpr int MaximumDiceTotal = 12;
static constexpr int NumDiceTotals = MaximumDiceTotal - MinimumDiceTotal + 1;

DataHandler::DataHandler(const std::shared_ptr<common::ErrorLogger>& logger) : m_logger(logger) {
    std::filesystem::path filePath = InfiniteFileName;

    if (!std::filesystem::exists(filePath)) {
        std::ofstream fd(filePath);
        if (!fd) {
            m_logger->LogError("DataHandler: Failed to initialize infinite tally file\n");
        } else {
            for (int i = MinimumDiceTotal; i <= MaximumDiceTotal; ++i) {
                fd << i;
                if (i < MaximumDiceTotal) {
                    fd << ",";
                }
            }
            fd << '\n';
        }
    }
}

DataHandler::~DataHandler() {
    SaveCountsToFile();
    AppendCountsToEternalFile();
}

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
    UpdateTally();
    CalculateProbabilities();
    m_currentInput = 0;
}

bool DataHandler::ValidateInput() {
    if (m_currentInput < MinInputValue || m_currentInput > MaxInputValue) {
        return false;
    }
    return true;
}

void DataHandler::CalculateProbabilities() {
    double total = 0;
    for (const auto tally : m_valueTally) {
        total += static_cast<double>(tally);
    }

    for (int index = 0; index < NumValues; ++index) {
        m_valueProbabilities[index] = (static_cast<double>(m_valueTally[index]) / total) * 100.0;
    }
}

void DataHandler::UpdateTally() {
    ++m_valueTally[m_currentInput - MinInputValue];
}

const std::array<int, NumValues>& DataHandler::GetTally() {
    return m_valueTally;
}

const std::array<double, NumValues>& DataHandler::GetProbabilities() {
    return m_valueProbabilities;
}

const std::vector<int>& DataHandler::GetPreviousInputs() {
    return m_previousInputs;
}

int DataHandler::GetCurrentInput() {
    return m_currentInput;
}

void DataHandler::SaveCountsToFile() {
    const std::time_t t = std::time(0);
    const std::tm* now = std::localtime(&t);
    std::stringstream filename;
    filename << OutputFileDirectory << "Games_" << (now->tm_year + 1900) << '_' << (now->tm_mon + 1) << '_'
             << now->tm_mday << "_" << now->tm_hour << ".csv";
    std::ofstream fd(filename.str());

    if (!fd) {
        m_logger->LogError("DataHandler: Failed to save counts to file\n");
        return;
    }

    // Add column headers
    for (int i = MinimumDiceTotal; i <= MaximumDiceTotal; ++i) {
        fd << i;
        if (i < MaximumDiceTotal) {
            fd << ",";
        }
    }
    fd << "\n";
    for (int i = 0; i < NumDiceTotals; i++) {
        fd << m_valueTally[i];
        if (i < NumDiceTotals - 1) {
            fd << ",";
        }
    }
    fd << '\n';
}
void DataHandler::AppendCountsToEternalFile() {
    std::ofstream fd(InfiniteFileName, std::ios::app);
    if (!fd) {
        m_logger->LogError("DataHandler: Failed to open infinite tally file\n");
        return;
    }
    for (int i = 0; i < NumDiceTotals; i++) {
        fd << m_valueTally[i];
        if (i < NumDiceTotals - 1) {
            fd << ",";
        }
    }
    fd << '\n';
}
}  // namespace data