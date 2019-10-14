#!/bin/bash

clang-format -style=file -i src/headers/*.hpp 
clang-format -style=file -i src/sources/*.cpp 
clang-format -style=file -i main.cpp
