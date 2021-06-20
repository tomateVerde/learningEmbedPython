cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 17)

# set the project name
project(myPie)

find_package (Python 3.9 REQUIRED COMPONENTS Development)


# add the executable
add_executable(myPie main.cpp)

target_link_libraries(myPie PUBLIC
  pie
  ${Python_LIBRARIES}
)

target_include_directories(myPie
PUBLIC
  ${Python_INCLUDE_DIRS}
)

install(FILES ./example.py DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(pie)
