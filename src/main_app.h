#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "common/error_logger.h"

#include <memory>
#include <thread>

namespace app {

class MainApp {
public:
    static std::unique_ptr<MainApp> Create();
    static bool InitialiseNCurses(std::shared_ptr<common::ErrorLogger>& logger);
    bool DidAllWindowsStart();
    void Start();

    ~MainApp();

    MainApp(const MainApp&) = delete;
    MainApp& operator=(const MainApp&) = delete;

    MainApp(MainApp&&) = delete;
    MainApp& operator=(MainApp&&) = delete;

private:
    std::jthread m_inputThread;
    std::shared_ptr<common::ErrorLogger> m_logger;
    explicit MainApp(std::shared_ptr<common::ErrorLogger>& logger);

    void InputThreadLoop(std::stop_token stopToken);
};
}  // namespace app

#endif