#include "display/window.h"
#include "common/error_logger.h"

#include <ncurses.h>
#include <memory>

namespace display {

Window::Window(const WindowDimension& dimensions, const std::shared_ptr<common::ErrorLogger>& logger)
    : m_logger(logger),
      m_width(dimensions.width),
      m_height(dimensions.height),
      m_topEdge(dimensions.topEdge),
      m_leftEdge(dimensions.leftEdge) {
    m_windowPtr = newwin(m_height, m_width, m_topEdge, m_leftEdge);
    if (m_windowPtr == nullptr) {
        m_logger->LogError("Fatal: Window failed to assign pointer\n");
    }
    if (box(m_windowPtr, 0, 0) == ERR) {
        m_logger->LogError("Warning: Window failed to generate border\n");
    }
    RefreshWindow();
}

Window::~Window() {
    wborder(m_windowPtr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    delwin(m_windowPtr);
}

bool Window::IsNullPointer() {
    return (m_windowPtr == nullptr);
}

bool Window::RefreshWindow() {
    if (wrefresh(m_windowPtr) == ERR) {
        m_logger->LogError("Warning: Window failed to fresh\n");
        return false;
    }
    return true;
}

bool Window::ClearWindow() {
    int err = 0;
    for (int row = 1; row < m_height - 1; ++row) {
        err |= wmove(m_windowPtr, row, 1);
        for (int column = 1; column < m_width - 1; ++column) {
            err |= wprintw(m_windowPtr, " ");
        }
    }
    if (err == ERR) {
        m_logger->LogError("Warning: Clear window reported an ERR\n");
        return false;
    }
    return true;
}

}  // namespace display