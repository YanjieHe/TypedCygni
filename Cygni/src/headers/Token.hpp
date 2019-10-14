#ifndef CYGNI_TOKEN_HPP
#define CYGNI_TOKEN_HPP

#include <string>
#include <unordered_map>

namespace cygni {

enum class Tag {
	Identifier,
	Integer,
	Float,
	String,

	Add,
	Subtract,
	Multiply,
	Divide,
	Modulo,

	Equal,
	NotEqual,
	GreaterThan,
	GreaterThanOrEqual,
	LessThan,
	LessThanOrEqual,

	And,
	Or,
	Not,

	Var,
	Def,
	Assign,

	LeftBrace,
	RightBrace,
	LeftBracket,
	RightBracket,
	LeftParenthesis,
	RightParenthesis,

	Dot,
	Comma,
	Semicolon,
	Colon,
	RightArrow,

	True,
	False,

	If,
	Else,
	While,

	For,
	Break,
	Return,

	Import,
	Uses,

	LowerBound,
	UpperBound,

	Class,
	Private,
	Protected,
	Module,
	Eof
};

std::string TagToString(Tag tag);

class Token {
public:
	Tag tag;
	int line;
	int column;
	std::u32string text;

	Token();

	Token(int line, int column, Tag tag, const std::u32string& text);

private:
	static Tag IdentifyKeyword(Tag tag, const std::u32string& text);
};

} // namespace cygni
#endif // CYGNI_TOKEN_HPP
