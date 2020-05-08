cmake_minimum_required(VERSION 3.5)

project(VirtualMachine LANGUAGES CXX)

set(CMAKE_C_FLAGS "-std=c89")
set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)


include_directories(src/ include/)

file(GLOB SOURCES "src/*.cpp")

add_executable(VirtualMachine main.cpp ${SOURCES})
