cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 17)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial main.cpp)

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )