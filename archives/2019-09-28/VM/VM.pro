TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Decoder.cpp \
    Disassembly.cpp \
    Function.cpp \
    Machine.cpp \
    OpCode.cpp \
    Test.cpp \
    Exception.cpp \
    Position.cpp \
    BinaryStreamReader.cpp \
    Value.cpp \
    ReadBinaryFile.cpp

HEADERS +=  \
    Decoder.h \
    Disassembly.h \
    Function.h \
    Machine.h \
    OpCode.h \
    Test.h \
    Exception.h \
    Position.h \
    PrimitiveType.h \
    Array \
    BinaryStreamReader.h \
    Value.h \
    ReadBinaryFile.h
