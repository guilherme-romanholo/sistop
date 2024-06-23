# Cmake Version
cmake_minimum_required(VERSION 3.12)

# Project name
project(sistop)

# Find the ncurses package
find_package(Curses REQUIRED)

# Include the ncurses headers
include_directories(${CURSES_INCLUDE_DIR})

# Source files
add_executable(sistop
        ./src/main.c
        ./src/kernel/kernel.c
        ./src/kernel/kernel.h
        ./src/memory/memory.c
        ./src/memory/memory.h
        ./src/process/process.c
        ./src/process/process.h
        ./src/semaph/semaph.c
        ./src/semaph/semaph.h
        ./src/utils/list.c
        ./src/utils/list.h
        ./src/utils/utils.c
        ./src/utils/utils.h
        ./src/interface/interface.c
        ./src/interface/interface.h
        ./src/scheduler/scheduler.c
        ./src/scheduler/scheduler.h
        ./src/disk/disk.c
        ./src/disk/disk.h
        src/disk/disk.h
)

# Link libraries
target_link_libraries(sistop ${CURSES_LIBRARIES} form menu m pthread)

add_custom_target(run
        COMMAND sistop
        DEPENDS sistop
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

