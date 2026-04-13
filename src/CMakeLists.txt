set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

set(HEADERS
    main_app.h
)

set(SOURCES
    main_app.cpp
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