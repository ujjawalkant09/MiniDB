cmake_minimum_required(VERSION 3.20)
project(mini_db LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compile flags for systems programming
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
)

# Source files
add_executable(mini_db
    src/main.cpp
    src/storage/disk_manager.cpp
)

target_include_directories(mini_db PRIVATE src)
