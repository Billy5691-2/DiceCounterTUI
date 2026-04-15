#include "main_app.h"

#include <atomic>
#include <csignal>
#include <cstdlib>
#include <memory>

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic_flag shutdownRequested;

/**
 * @brief Signal handler to shutdown the app if we recieve SIGINT, SIGTERM, SIGABRT
 * @param signal Either SIGINT, SIGTERM, or SIGABRT - unused
 */
void SignalHandler(int /*signal*/) {
    shutdownRequested.test_and_set();
    shutdownRequested.notify_all();
}

int main() {
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGABRT, SignalHandler);

    std::unique_ptr<app::MainApp> mainApp = app::MainApp::Create();

    if (mainApp == nullptr) {
        return EXIT_FAILURE;
    }

    if (!mainApp->Start()) {
        return EXIT_FAILURE;
    }

    shutdownRequested.wait(false);

    return EXIT_SUCCESS;
}