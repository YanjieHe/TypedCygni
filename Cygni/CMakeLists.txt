cmake_minimum_required(VERSION 3.10)
project(Cygni)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

include_directories(src/headers test/headers include/)

file(GLOB SOURCES "src/sources/*.cpp")

add_executable(Cygni main.cpp ${SOURCES})
