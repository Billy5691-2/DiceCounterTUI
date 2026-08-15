set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

set(HEADERS
    main_app.h
    common/error_logger.h
    data/constants.h
    data/data_handler.h
    display/window_constants.h
    display/window.h
    display/input_window.h
    display/data_window.h
    display/histogram_window.h
)

set(SOURCES
    main_app.cpp
    common/error_logger.cpp
    data/data_handler.cpp
    display/window.cpp
    display/input_window.cpp
    display/data_window.cpp
    display/histogram_window.cpp
    )

add_library(${PROJECT_NAME}_lib SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
    STATIC
    ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
    ${CURSES_LIBRARIES})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)