cmake_minimum_required (VERSION 3.10)

project ("Game Of Life" C)
set(TARGET golsim)

find_package (SDL2 REQUIRED)

set (SRCDIR "${PROJECT_SOURCE_DIR}/src")

set (SRC ${SRCDIR}/main.c)
list (APPEND SRC "${SRCDIR}/video.c")
list (APPEND SRC "${SRCDIR}/gui/gui_win32.c")
list (APPEND SRC "${SRCDIR}/gui/win32_mainwindow.c")

include_directories (${SRCDIR})
include_directories ("${PROJECT_BINARY_DIR}")

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s -mconsole")

option (USE_ANSI "Use ANSI color codes" ON)

configure_file ("${PROJECT_SOURCE_DIR}/features.h.in" "${PROJECT_BINARY_DIR}/features.h")

add_executable (${TARGET} ${SRC})
target_link_libraries (${TARGET} ${SDL2_LIBRARIES})
set_property (TARGET ${TARGET} PROPERTY C_STANDARD 11)
