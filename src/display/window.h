#ifndef TUI_WINDOW_H
#define TUI_WINDOW_H

#include "common/error_logger.h"

#include <ncurses.h>
#include <memory>

namespace display {

struct WindowDimension {
    const int width;
    const int height;
    const int topEdge;
    const int leftEdge;
};

class Window {
protected:
    WINDOW* m_windowPtr = nullptr;
    std::shared_ptr<common::ErrorLogger> m_logger;
    const int m_width;
    const int m_height;

    Window(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool IsNullPointer();
    bool RefreshWindow();
    bool ClearWindow();

private:
    const int m_topEdge;
    const int m_leftEdge;
};

}  // namespace display

#endif