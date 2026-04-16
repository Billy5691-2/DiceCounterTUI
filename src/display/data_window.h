#ifndef TUI_DATA_WINDOW_H
#define TUI_DATA_WINDOW_H

#include "data/constants.h"
#include "display/window.h"

#include <array>
#include <memory>

namespace display {

class DataWindow : Window {
public:
    static std::unique_ptr<DataWindow> Create(const WindowDimension& dimensions,
                                              const std::shared_ptr<common::ErrorLogger>& logger);

    ~DataWindow() = default;

    DataWindow(const DataWindow&) = delete;
    DataWindow& operator=(const DataWindow&) = delete;

    DataWindow(DataWindow&&) = delete;
    DataWindow& operator=(DataWindow&&) = delete;

    bool DrawData(const std::array<double, data::NumValues>& probabilities,
                  const std::array<int, data::NumValues>& tally);

private:
    DataWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger);

    bool PrintDataLine(int row, int value, double probability, double expectedProbability, int tally);
    bool PrintTitleLine();
    bool PrintDashLine();
};

}  // namespace display

#endif