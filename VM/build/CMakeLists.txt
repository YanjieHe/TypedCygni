cmake_minimum_required(VERSION 3.1.0)
PROJECT(VM)
SET(CMAKE_CXX_STANDARD 11)
ADD_EXECUTABLE(VM main.cpp
	OpCode.h OpCode.cpp
	Machine.h Machine.cpp
	Any.h Any.cpp
	Test.h Test.cpp
	Function.h Function.cpp
	Decoder.h Decoder.cpp
	Disassembly.h Disassembly.cpp)
