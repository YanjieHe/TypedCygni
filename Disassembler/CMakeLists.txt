cmake_minimum_required(VERSION 3.13)
project(Disassembler)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/headers test/headers)

file(GLOB SOURCES "src/sources/*.c")
file(GLOB TESTS "test/sources/*.c")

add_executable(Disassembler main.cpp ${SOURCES} ${TESTS})
