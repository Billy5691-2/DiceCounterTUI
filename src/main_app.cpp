#include "main_app.h"
#include "common/error_logger.h"

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
    if (nodelay(terminalWin, TRUE) == ERR) {
        logger->LogError("Fatal: nodelay() returned ERR\n");
        return false;
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

MainApp::MainApp(std::shared_ptr<common::ErrorLogger>& logger) : m_logger(logger) {}

MainApp::~MainApp() {
    m_inputThread.request_stop();
    if (m_inputThread.joinable()) {
        m_inputThread.join();
    }
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
    return true;
}

void MainApp::Start() {
    m_inputThread = std::jthread([this](std::stop_token stopToken) { InputThreadLoop(stopToken); });
}

void MainApp::InputThreadLoop(std::stop_token stopToken) {
    int input;
    do {
        input = getch();
        if (input != ERR && input != KEY_F(1)) {
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    } while (input != KEY_F(1) && !stopToken.stop_requested());
    if (input == KEY_F(1)) {
        raise(SIGINT);
    }
}

}  // namespace app