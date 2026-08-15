#ifndef TUI_DICE_VALUE_INPUT_WINDOW_H
#define TUI_DICE_VALUE_INPUT_WINDOW_H

#include "display/window.h"

#include <memory>
#include <vector>

namespace display {

class DiceValueInputWindow : Window {
public:
    static std::unique_ptr<DiceValueInputWindow> Create(const WindowDimension& dimensions,
                                                        const std::shared_ptr<common::ErrorLogger>& logger);

    ~DiceValueInputWindow() = default;

    DiceValueInputWindow(const DiceValueInputWindow&) = delete;
    DiceValueInputWindow& operator=(const DiceValueInputWindow&) = delete;

    DiceValueInputWindow(DiceValueInputWindow&&) = delete;
    DiceValueInputWindow& operator=(DiceValueInputWindow&&) = delete;

    bool DrawData(const std::vector<int>& prevInputs, int currentValue);

private:
    DiceValueInputWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger);

    bool DrawPrevInputs(const std::vector<int>& prevInputs);
    bool DrawCurrentInput(int currentValue);
};

}  // namespace display

#endif