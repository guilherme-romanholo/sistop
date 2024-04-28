# Cmake Version
cmake_minimum_required(VERSION 3.12)
# Project name
project(sistop)
# Source files
add_executable(sistop 
    src/main.c
    src/interface/interface.c
    src/interface/interface.h
    src/memory/memory.c
    src/memory/memory.h
    src/process/process.c
    src/process/process.h
    src/scheduler/scheduler.c
    src/scheduler/scheduler.h
    )

# Link libraries
# target_link_libraries(sistop pthread)
