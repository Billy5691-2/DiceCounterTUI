#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "common/error_logger.h"
#include "data/data_handler.h"
#include "display/input_window.h"

#include <memory>
#include <thread>

namespace app {

class MainApp {
public:
    static std::unique_ptr<MainApp> Create();
    static bool InitialiseNCurses(std::shared_ptr<common::ErrorLogger>& logger);
    bool DidAllWindowsStart();
    bool Start();

    ~MainApp();

    MainApp(const MainApp&) = delete;
    MainApp& operator=(const MainApp&) = delete;

    MainApp(MainApp&&) = delete;
    MainApp& operator=(MainApp&&) = delete;

private:
    std::jthread m_inputThread;
    std::shared_ptr<common::ErrorLogger> m_logger;
    std::unique_ptr<data::DataHandler> m_dataHandler;
    std::unique_ptr<display::InputWindow> m_inputWindow;

    explicit MainApp(std::shared_ptr<common::ErrorLogger>& logger);

    void InputThreadLoop(std::stop_token stopToken);
    bool HandleNumberInput(int input);

    bool RefreshInputWindow();
    bool RefreshAllWindows();
};
}  // namespace app

#endif