cmake_minimum_required(VERSION 3.1.0)
PROJECT(Cygni)
SET(CMAKE_CXX_STANDARD 11)
ADD_EXECUTABLE(Cygni main.cpp 
	TokenKind.h TokenKind.cpp 
	Token.h Token.cpp 
	Lexer.h Lexer.cpp 
	StringBuilder.h StringBuilder.cpp 
	Test.h Test.cpp
	Exception.h Exception.cpp
	ExpressionKind.h ExpressionKind.cpp
	Expression.h Expression.cpp
	Type.h Type.cpp
	TypeTag.h TypeTag.cpp
	DebugInfo.h DebugInfo.cpp
	Parser.h Parser.cpp
	Visitor.h Visitor.cpp)
