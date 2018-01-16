TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    TokenKind.cpp \
    Token.cpp \
    Lexer.cpp \
    Test.cpp \
    Exception.cpp \
    ExpressionKind.cpp \
    Expression.cpp \
    Type.cpp \
    TypeTag.cpp \
    Parser.cpp \
    Scope.cpp \
    Location.cpp \
    TypeEnv.cpp \
    TypeChecker.cpp \
    OpCode.cpp \
    Function.cpp \
    Position.cpp \
    ByteCodeGenerator.cpp \
    Compiler.cpp \
    DefinitionCollector.cpp \
    TreeViewer.cpp

HEADERS +=  TokenKind.h \
    Token.h \
    Lexer.h \
    Test.h \
    Exception.h \
    ExpressionKind.h \
    Expression.h \
    Type.h \
    TypeTag.h \
    Parser.h \
    Visitor.h \
    Scope.h \
    Location.h \
    TypeEnv.h \
    TypeChecker.h \
    OpCode.h \
    Function.h \
    Position.h \
    ByteCodeGenerator.h \
    Compiler.h \
    DefinitionCollector.h \
    PrimitiveType.h \
    TreeViewer.h
