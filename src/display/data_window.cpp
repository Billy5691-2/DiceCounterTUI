#include "display/data_window.h"
#include "data/constants.h"
#include "display/window_constants.h"

#include <memory>

namespace display {
constexpr char const* TitleValue = "Val: ";
constexpr char const* TitleProbability = "| P %%: ";
constexpr char const* TitleExpProb = "| Exp %%: ";
constexpr char const* TitleTally = "| Tally:";

constexpr int ValueColWidth = 5;
constexpr int ProbabilityColWidth = 7;
constexpr int ExpProbColWidth = 9;
constexpr int TallyColWidth = 6;

constexpr int ValueColStart = 1;
constexpr int ProbabilityColStart = ValueColStart + ValueColWidth;
constexpr int ExpProbColStart = ProbabilityColStart + ProbabilityColWidth;
constexpr int TallyColStart = ExpProbColStart + ExpProbColWidth;

constexpr int MinimumColWidth = TallyColStart + TallyColWidth + 1;
static_assert(MinimumColWidth <= DataWindowWidth);

constexpr int TitleRow = 1;
constexpr int DashRow = TitleRow + 1;
constexpr int FirstDataRow = DashRow + 1;
constexpr int DataRowSpaces = 2;
constexpr int MinimumHeight = DashRow + (data::NumValues * DataRowSpaces);
static_assert(MinimumHeight <= DataWindowHeight);

std::unique_ptr<DataWindow> DataWindow::Create(const WindowDimension& dimensions,
                                               const std::shared_ptr<common::ErrorLogger>& logger) {
    std::unique_ptr<DataWindow> windowPtr = std::unique_ptr<DataWindow>(new DataWindow(dimensions, logger));
    if (windowPtr->IsNullPointer()) {
        logger->LogError("Warning: Failed to initialise ProbabilitiesWindow window ptr\n");
        return nullptr;
    }
    return windowPtr;
}

DataWindow::DataWindow(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger)
    : Window(dimensions, logger) {}

bool DataWindow::DrawData(const std::array<double, data::NumValues>& probabilities,
                          const std::array<int, data::NumValues>& tally) {
    bool healthy = ClearWindow();
    healthy &= PrintTitleLine();
    healthy &= PrintDashLine();
    for (int index = 0; index < data::NumValues; ++index) {
        healthy &= PrintDataLine((index * DataRowSpaces) + FirstDataRow, (index + data::MinInputValue),
                                 probabilities[index], data::ExpectedProbabilites[index], tally[index]);
    }
    healthy &= RefreshWindow();
    return healthy;
}

bool DataWindow::PrintDataLine(int row, int value, double probability, double expectedProbability, int tally) {
    int err = mvwprintw(m_windowPtr, row, ValueColStart, " %d", value);
    err |= mvwprintw(m_windowPtr, row, ProbabilityColStart, "| %.2f", probability);
    err |= mvwprintw(m_windowPtr, row, ExpProbColStart, "| %.2f", expectedProbability);
    err |= mvwprintw(m_windowPtr, row, TallyColStart, "| %d", tally);
    if (err == ERR) {
        m_logger->LogError("Warning: Error while printing DataWindow data\n");
        return false;
    }
    return true;
}

bool DataWindow::PrintTitleLine() {
    int err = mvwprintw(m_windowPtr, TitleRow, ValueColStart, TitleValue);
    err |= mvwprintw(m_windowPtr, TitleRow, ProbabilityColStart, TitleProbability);
    err |= mvwprintw(m_windowPtr, TitleRow, ExpProbColStart, TitleExpProb);
    err |= mvwprintw(m_windowPtr, TitleRow, TallyColStart, TitleTally);
    if (err == ERR) {
        m_logger->LogError("Warning: Error while printing DataWindow title\n");
        return false;
    }
    return true;
}

bool DataWindow::PrintDashLine() {
    int err = wmove(m_windowPtr, DashRow, 1);
    for (int col = 1; col < m_width - 1; ++col) {
        err |= wprintw(m_windowPtr, "-");
    }
    if (err == ERR) {
        m_logger->LogError("Warning: Error while printing DataWindow dash line\n");
        return false;
    }
    return true;
}
}  // namespace display