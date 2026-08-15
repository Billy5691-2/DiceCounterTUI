#include "main_app.h"
#include "common/error_logger.h"
#include "display/window.h"
#include "display/window_constants.h"

#include <ncurses.h>
#include <chrono>
#include <csignal>
#include <stop_token>
#include <thread>

namespace app {

bool MainApp::InitialiseNCurses(std::shared_ptr<common::ErrorLogger>& logger) {
    WINDOW* terminalWin = initscr();
    if (terminalWin == nullptr) {
        logger->LogError("Fatal: initscr() get nullptr\n");
        return false;
    }
    if (cbreak() == ERR) {
        logger->LogError("Fatal: cbreak() returned ERR\n");
        return false;
    }
    if (noecho() == ERR) {
        logger->LogError("Fatal: noecho() returned ERR\n");
        return false;
    }
    if (keypad(stdscr, TRUE) == ERR) {
        logger->LogError("Fatal: keypad() returned ERR\n");
        return false;
    }
    if (curs_set(0) == ERR) {
        logger->LogError("Fatal: curs_set() returned ERR\n");
        return false;
    }
    if (start_color() == ERR) {
        logger->LogError("Fatal: start_color() returned ERR\n");
        return false;
    }
    if (init_pair(display::RedBlackId, COLOR_BLACK, COLOR_RED)) {
        if (nodelay(terminalWin, TRUE) == ERR) {
            logger->LogError("Fatal: nodelay() returned ERR\n");
            return false;
        }
    }
    if (printw("Welcome to the Dice Counter TUI. Press F1 to exit\n") == ERR) {
        logger->LogError("Fatal: printw() returned ERR\n");
        return false;
    }
    if (refresh() == ERR) {
        logger->LogError("Fatal: refresh() returned ERR\n");
        return false;
    }
    return true;
}

MainApp::MainApp(std::shared_ptr<common::ErrorLogger>& logger) : m_logger(logger) {
    m_dataHandler = std::make_unique<data::DataHandler>();

    display::WindowDimension dataWindowDimensions = { .width = display::DataWindowWidth + display::BorderSpaces,
                                                      .height = display::DataWindowHeight + display::BorderSpaces,
                                                      .topEdge = 1,
                                                      .leftEdge = 0 };
    m_dataWindow = display::DataWindow::Create(dataWindowDimensions, logger);
    display::WindowDimension histogramWindowDimensions = { .width = COLS - dataWindowDimensions.width - 1,
                                                           .height = display::DataWindowHeight + display::BorderSpaces,
                                                           .topEdge = 1,
                                                           .leftEdge = dataWindowDimensions.width + 1 };
    m_histogramWindow = display::HistogramWindow::Create(histogramWindowDimensions, logger);
    display::WindowDimension inputWindowDimensions = { .width = COLS,
                                                       .height = display::InputWindowHeight + display::BorderSpaces,
                                                       .topEdge = 1 + dataWindowDimensions.height,
                                                       .leftEdge = 0 };
    m_inputWindow = display::InputWindow::Create(inputWindowDimensions, logger);
}

MainApp::~MainApp() {
    m_inputThread.request_stop();
    if (m_inputThread.joinable()) {
        m_inputThread.join();
    }
    // Destroy windows before calling endwin();
    m_inputWindow = nullptr;
    m_dataWindow = nullptr;
    m_histogramWindow = nullptr;
    endwin();
}

std::unique_ptr<MainApp> MainApp::Create() {
    std::shared_ptr<common::ErrorLogger> logger = std::make_shared<common::ErrorLogger>();
    if (!InitialiseNCurses(logger)) {
        endwin();
        return nullptr;
    }
    std::unique_ptr<MainApp> appPointer = std::unique_ptr<MainApp>(new MainApp(logger));
    if (!appPointer->DidAllWindowsStart()) {
        logger->LogError("Fatal: Not all windows initialised successfully\n");
        return nullptr;
    }
    return appPointer;
}

bool MainApp::DidAllWindowsStart() {
    bool success = (m_inputWindow != nullptr);
    success &= (m_dataWindow != nullptr);
    success &= (m_histogramWindow != nullptr);
    return success;
}

bool MainApp::Start() {
    if (!RefreshAllWindows()) {
        return false;
    }
    m_inputThread = std::jthread([this](std::stop_token stopToken) { InputThreadLoop(stopToken); });
    return true;
}

void MainApp::InputThreadLoop(std::stop_token stopToken) {
    int input;
    bool healthy = true;
    do {
        input = getch();
        if (input != ERR && input != KEY_F(1)) {
            if (input >= '0' && input <= '9') {
                m_dataHandler->AccumulateInput(input);
                healthy &= RefreshInputWindow();
            } else if (input == '\n' || input == KEY_ENTER) {
                m_dataHandler->EnterNewValue();
                healthy &= RefreshAllWindows();
            } else if (input == KEY_BACKSPACE) {
                m_dataHandler->ResetInputValue();
                healthy &= RefreshInputWindow();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    } while (input != KEY_F(1) && !stopToken.stop_requested() && healthy);
    if (input == KEY_F(1) || !healthy) {
        raise(SIGINT);
    }
}

bool MainApp::RefreshInputWindow() {
    return m_inputWindow->DrawData(m_dataHandler->GetPreviousInputs(), m_dataHandler->GetCurrentInput());
}

bool MainApp::RefreshAllWindows() {
    bool healthy = RefreshInputWindow();
    healthy &= m_dataWindow->DrawData(m_dataHandler->GetProbabilities(), m_dataHandler->GetTally());
    healthy &= m_histogramWindow->DrawHistogram(m_dataHandler->GetTally());
    return healthy;
}

}  // namespace app