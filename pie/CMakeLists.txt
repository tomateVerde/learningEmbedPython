set (LIB_NAME pie)

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} PUBLIC INTERFACE .)
