cmake_minimum_required(VERSION 3.10)
project(Cygni_Test)

set(CMAKE_CXX_STANDARD 17)

include_directories(../src/headers ../test/headers ../include/)

file(GLOB SOURCES "../src/sources/*.cpp")
file(GLOB TESTS "../test/sources/*.cpp")

add_executable(Cygni_Test test.cpp ${SOURCES} ${TESTS})
