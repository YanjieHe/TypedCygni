cmake_minimum_required(VERSION 3.13)
project(CygniCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(CygniCompiler main.cpp Token.cpp Token.hpp Encoding.cpp Encoding.hpp String.hpp Lexer.cpp Lexer.hpp test/TestLexer.cpp test/TestLexer.hpp Ast.cpp Ast.hpp Type.cpp Type.hpp Parser.cpp Parser.hpp test/TestParser.cpp test/TestParser.hpp test/JSON.cpp test/JSON.hpp TypeChecker.cpp TypeChecker.hpp Predef.cpp Predef.hpp Scope.cpp Scope.hpp test/TestString.cpp test/TestString.hpp File.cpp File.hpp Any.hpp test/TestTypeChecker.cpp test/TestTypeChecker.hpp Optional.hpp Compiler.cpp Compiler.hpp Endian.hpp test/TestEndian.cpp test/TestEndian.hpp Locator.cpp Locator.hpp Exception.cpp Exception.hpp test/TestLocator.cpp test/TestLocator.hpp)