#ifndef TUI_HISTOGRAM_WINDOW_H
#define TUI_HISTOGRAM_WINDOW_H

#include "data/constants.h"
#include "display/window.h"

#include <memory>

namespace display {

class HistogramWindow : Window {
public:
    static std::unique_ptr<HistogramWindow> Create(const WindowDimension& dimensions,
                                                   const std::shared_ptr<common::ErrorLogger>& logger);
    ~HistogramWindow() = default;

    HistogramWindow(const HistogramWindow&) = delete;
    HistogramWindow& operator=(const HistogramWindow&) = delete;

    HistogramWindow(HistogramWindow&&) = delete;
    HistogramWindow& operator=(HistogramWindow&&) = delete;

    bool DrawHistogram(const std::array<int, data::NumValues>& tallyData);

private:
    HistogramWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger);

    bool DrawColumn(int column, int height);
    bool DrawXAxis();
    bool DrawYAxis(int maxValue);
    int GetScaledColumnValue(int value, int maxValue);
    int GetMaximum(const std::array<int, data::NumValues>& tallyData);
    int GetYMaximum(int maxValue);
};

}  // namespace display

#endif