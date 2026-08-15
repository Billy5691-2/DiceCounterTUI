#include "display/dice_value_input_window.h"

#include <memory>
#include <sstream>
#include <string>

namespace display {

constexpr int PrevInputsRow = 1;
constexpr int UserInputRow = 2;

std::unique_ptr<DiceValueInputWindow> DiceValueInputWindow::Create(const WindowDimension& dimensions,
                                                                   const std::shared_ptr<common::ErrorLogger>& logger) {
    std::unique_ptr<DiceValueInputWindow> windowPtr =
        std::unique_ptr<DiceValueInputWindow>(new DiceValueInputWindow(dimensions, logger));
    if (windowPtr->IsNullPointer()) {
        logger->LogError("Warning: Failed to initialise DiceValueInputWindow window pointer\n");
        return nullptr;
    }
    return windowPtr;
}

DiceValueInputWindow::DiceValueInputWindow(const WindowDimension& dimensions,
                                           const std::shared_ptr<common::ErrorLogger>& logger)
    : Window(dimensions, logger) {}

bool DiceValueInputWindow::DrawData(const std::vector<int>& prevInputs, const int currentValue) {
    bool healthy = ClearWindow();
    healthy &= DrawPrevInputs(prevInputs);
    healthy &= DrawCurrentInput(currentValue);
    healthy &= RefreshWindow();
    return healthy;
}

bool DiceValueInputWindow::DrawPrevInputs(const std::vector<int>& prevInputs) {
    int err = wmove(m_windowPtr, PrevInputsRow, 1);
    std::stringstream vectorToString;
    for (int prevInput : prevInputs) {
        vectorToString << prevInput << " ";
    }
    err |= waddnstr(m_windowPtr, vectorToString.str().c_str(), m_width - 2);
    if (err == ERR) {
        m_logger->LogError("Warning: DrawPrevInputs experienced an error\n");
        return false;
    }
    return true;
}

bool DiceValueInputWindow::DrawCurrentInput(const int currentValue) {
    int err = mvwprintw(m_windowPtr, UserInputRow, 1, "Please input a value: ");
    if (currentValue != 0) {
        err |= wprintw(m_windowPtr, "%d", currentValue);
    } else {
        err |= wprintw(m_windowPtr, "_");
    }
    if (err == ERR) {
        m_logger->LogError("Warning: DrawCurrentInput experienced an error\n");
        return false;
    }
    return true;
}

}  // namespace display