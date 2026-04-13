#include "main_app.h"

#include <ncurses.h>
#include <chrono>
#include <csignal>
#include <stop_token>
#include <thread>

namespace app {

bool MainApp::InitialiseNCurses() {
    WINDOW* terminalWin = initscr();
    if (terminalWin == nullptr) {
        return false;
    }
    if (cbreak() == ERR) {
        return false;
    }
    if (noecho() == ERR) {
        return false;
    }
    if (keypad(stdscr, TRUE) == ERR) {
        return false;
    }
    if (curs_set(0) == ERR) {
        return false;
    }
    if (start_color() == ERR) {
        return false;
    }
    if (nodelay(terminalWin, TRUE) == ERR) {
        return false;
    }
    if (printw("Welcome to the Dice Counter TUI. Press F1 to exit\n") == ERR) {
        return false;
    }
    if (refresh() == ERR) {
        return false;
    }
    return true;
}

MainApp::MainApp() {}

MainApp::~MainApp() {
    m_inputThread.request_stop();
    if (m_inputThread.joinable()) {
        m_inputThread.join();
    }
    endwin();
}

std::unique_ptr<MainApp> MainApp::Create() {
    if (!InitialiseNCurses()) {
        endwin();
        return nullptr;
    }
    std::unique_ptr<MainApp> appPointer = std::unique_ptr<MainApp>(new MainApp());
    if (!appPointer->DidAllWindowsStart()) {
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