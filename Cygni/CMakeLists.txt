cmake_minimum_required(VERSION 3.5)

project(Cygni LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(src/ include/)

file(GLOB SOURCES "src/*.cpp")

add_executable(Cygni main.cpp ${SOURCES})
