cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 17)

# set the project name
project(myPie)

find_package (Python REQUIRED COMPONENTS Development)

# add the executable
add_executable(myPie main.cpp)

target_link_libraries(myPie ${Python_LIBRARIES})

target_include_directories(myPie PUBLIC ${Python_INCLUDE_DIRS})

message(STATUS "Output is ${CMAKE_BINARY_DIR}")

install(FILES ./example.py DESTINATION ${CMAKE_BINARY_DIR})
