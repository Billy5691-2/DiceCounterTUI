#ifndef TUI_INPUT_WINDOW_H
#define TUI_INPUT_WINDOW_H

#include "display/window.h"

#include <memory>
#include <vector>

namespace display {

class InputWindow : Window {
public:
    static std::unique_ptr<InputWindow> Create(const WindowDimension& dimensions,
                                               const std::shared_ptr<common::ErrorLogger>& logger);

    ~InputWindow() = default;

    InputWindow(const InputWindow&) = delete;
    InputWindow& operator=(const InputWindow&) = delete;

    InputWindow(InputWindow&&) = delete;
    InputWindow& operator=(InputWindow&&) = delete;

    bool DrawData(const std::vector<int>& prevInputs, int currentValue);

private:
    InputWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger);

    bool DrawPrevInputs(const std::vector<int>& prevInputs);
    bool DrawCurrentInput(int currentValue);
};

}  // namespace display

#endif