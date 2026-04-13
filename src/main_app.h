#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <memory>
#include <thread>

namespace app {

class MainApp {
public:
    static std::unique_ptr<MainApp> Create();
    static bool InitialiseNCurses();
    bool DidAllWindowsStart();
    void Start();

    ~MainApp();

    MainApp(const MainApp&) = delete;
    MainApp& operator=(const MainApp&) = delete;

    MainApp(MainApp&&) = delete;
    MainApp& operator=(MainApp&&) = delete;

private:
    std::jthread m_inputThread;
    MainApp();

    void InputThreadLoop(std::stop_token stopToken);
};
}  // namespace app

#endif