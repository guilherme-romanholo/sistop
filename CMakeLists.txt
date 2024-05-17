# Cmake Version
cmake_minimum_required(VERSION 3.12)

# Project name
project(project)

# Source files
add_executable(sistop
        ./src/main.c
        ./src/kernel/kernel.c
        ./src/kernel/kernel.h
        ./src/memory/memory.c
        ./src/memory/memory.h
        ./src/process/process.c
        ./src/process/process.h
        ./src/terminal/terminal.c
        ./src/terminal/terminal.h
        ./src/semaph/semaph.c
        ./src/semaph/semaph.h
        ./src/utils/list.c
        ./src/utils/list.h
        ./src/utils/utils.h
)

# Link libraries
# target_link_libraries(project_exec pthread)