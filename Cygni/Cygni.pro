TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    TokenKind.cpp \
    Token.cpp \
    Lexer.cpp \
    StringBuilder.cpp \
    Test.cpp \
    Exception.cpp \
    ExpressionKind.cpp \
    Expression.cpp \
    Type.cpp \
    TypeTag.cpp \
    DebugInfo.cpp \
    Parser.cpp \
    Visitor.cpp \
    Scope.cpp \
    Location.cpp \
    TypeEnv.cpp \
    TypeChecker.cpp \
    OpCode.cpp \
    Compiler.cpp \
    Function.cpp \

HEADERS +=  TokenKind.h \
    Token.h \
    Lexer.h \
    StringBuilder.h \
    Test.h \
    Exception.h \
    ExpressionKind.h \
    Expression.h \
    Type.h \
    TypeTag.h \
    DebugInfo.h \
    Parser.h \
    Visitor.h \
    Scope.h \
    Location.h \
    TypeEnv.h \
    TypeChecker.h \
    OpCode.h \
    Compiler.h \
    Function.h \
