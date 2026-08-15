#include "display/histogram_window.h"
#include "common/error_logger.h"
#include "data/constants.h"
#include "display/window_constants.h"

#include <cmath>

namespace display {

constexpr int YAxisLabelsCol = 1;
constexpr int YAxisLineCol = YAxisLabelsCol + 2;
constexpr int FirstBarPosition = YAxisLineCol + 2;

constexpr int XAxisLabelsRow = display::HistogramWindowHeight;
constexpr int XAxisLineRow = XAxisLabelsRow - 1;
constexpr int XAxisBarStart = XAxisLineRow - 1;

std::unique_ptr<HistogramWindow> HistogramWindow::Create(const WindowDimension& dimensions,
                                                         const std::shared_ptr<common::ErrorLogger>& logger) {
    std::unique_ptr<HistogramWindow> windowPtr =
        std::unique_ptr<HistogramWindow>(new HistogramWindow(dimensions, logger));
    if (windowPtr->IsNullPointer()) {
        logger->LogError("Warning: Failed to initialise HistogramWindow window ptr\n");
        return nullptr;
    }
    return windowPtr;
}

HistogramWindow::HistogramWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger)
    : Window(dimensions, logger) {}

bool HistogramWindow::DrawHistogram(const std::array<int, data::NumValues>& tallyData) {
    const int maxValue = GetMaximum(tallyData);
    const int yMax = GetYMaximum(maxValue);
    bool healthy = ClearWindow();
    healthy &= DrawXAxis();
    healthy &= DrawYAxis(yMax);
    int column = FirstBarPosition;
    for (const auto value : tallyData) {
        int scaledValue = GetScaledColumnValue(value, yMax);
        healthy &= DrawColumn(column, scaledValue);
        column += 3;
    }
    healthy &= RefreshWindow();
    return healthy;
}

bool HistogramWindow::DrawXAxis() {
    int err = 0;
    for (int colOffset = 0; colOffset < data::NumValues * 3 + 4; ++colOffset) {
        err |= mvwprintw(m_windowPtr, XAxisLineRow, YAxisLineCol + colOffset, "-");
    }
    for (int colOffset = 0; colOffset < data::NumValues; ++colOffset) {
        err |= mvwprintw(m_windowPtr, XAxisLabelsRow, FirstBarPosition + (colOffset * 3), "%d",
                         colOffset + data::MinInputValue);
    }
    if (err == ERR) {
        m_logger->LogError("Warning: DrawXAxis in HistogramWindow has errored\n");
        return false;
    }
    return true;
}

bool HistogramWindow::DrawYAxis(const int yMax) {
    const int notchIncrement = yMax / 4;
    int notchMultiplier = 4;
    int err = 0;
    for (int rowOffset = 0; rowOffset <= 16; ++rowOffset) {
        err |= mvwprintw(m_windowPtr, XAxisBarStart - rowOffset, YAxisLineCol, "|");
        if (rowOffset % 4 == 0) {
            int notchValue = yMax - (notchIncrement * notchMultiplier);
            --notchMultiplier;
            err |= mvwprintw(m_windowPtr, XAxisBarStart - rowOffset, YAxisLabelsCol, "%d", notchValue);
        }
    }
    if (err == ERR) {
        m_logger->LogError("Warning: DrawYAxis in HistogramWindow has errored\n");
        return false;
    }
    return true;
}

int HistogramWindow::GetMaximum(const std::array<int, data::NumValues>& tallyData) {
    int maxValue = 0;
    for (const auto value : tallyData) {
        if (value > maxValue) {
            maxValue = value;
        }
    }
    return maxValue;
}

int HistogramWindow::GetYMaximum(const int maxValue) {
    int yMaximum = maxValue;
    while (yMaximum % 4 != 0) {
        ++yMaximum;
    }
    return yMaximum;
}

int HistogramWindow::GetScaledColumnValue(const int value, const int yMax) {
    const double valueDouble = static_cast<double>(value);
    const double yMaxDouble = static_cast<double>(yMax);
    const double rowHeight = ceil((valueDouble / yMaxDouble) * 16.0);
    return static_cast<int>(rowHeight);
}

bool HistogramWindow::DrawColumn(int column, int height) {
    int err = wattron(m_windowPtr, RedBlackId);
    for (int rowOffset = height - 1; rowOffset >= 0; --rowOffset) {
        err |= mvwprintw(m_windowPtr, XAxisBarStart - rowOffset, column, " ");
    }
    err |= wattroff(m_windowPtr, RedBlackId);
    if (err == ERR) {
        m_logger->LogError("Warning: DrawColumn in HistogramWindow has errored\n");
        return false;
    }
    return true;
}

}  // namespace display