TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Any.cpp \
    Decoder.cpp \
    Disassembly.cpp \
    Function.cpp \
    Machine.cpp \
    OpCode.cpp \
    Test.cpp \
    ByteCode.cpp \
    Memory.cpp

HEADERS +=  Any.h \
    Decoder.h \
    Disassembly.h \
    Function.h \
    Machine.h \
    OpCode.h \
    Test.h \
    ByteCode.h \
    Memory.h
