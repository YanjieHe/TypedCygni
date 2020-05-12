cmake_minimum_required(VERSION 3.5)

project(VirtualMachine LANGUAGES C)

set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include/)

file(GLOB SOURCES "src/*.c")


add_executable(VirtualMachine main.c ${SOURCES})

set_target_properties(VirtualMachine PROPERTIES LINKER_LANGUAGE C)