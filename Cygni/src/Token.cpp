#include "Token.hpp"
#include <iostream>

namespace cygni
{
	Token::Token() :line{ -1 }, column{ -1 }, tag{ Tag::Eof }, text{}
	{
	}
	Token::Token(int line, int column, Tag tag, const std::u32string &text)
		: line{ line }, column{ column }, tag{ IdentifyKeyword(tag, text) }, text{ text } {}

	Tag Token::IdentifyKeyword(Tag tag, const std::u32string &text)
	{
		if (tag == Tag::Identifier)
		{
			if (keywords.find(text) != keywords.end())
			{
				return keywords[text];
			}
			else
			{
				return tag;
			}
		}
		else
		{
			return tag;
		}
	}

	std::unordered_map<std::u32string, Tag> Token::keywords = {
			{U"if", Tag::If},
			{U"else", Tag::Else},
			{U"var", Tag::Var},
			{U"def", Tag::Def},
			{U"return", Tag::Return},
			{U"true", Tag::True},
			{U"false", Tag::False},
			{U"void", Tag::Void},
			{U"while", Tag::While},
			{U"new", Tag::New},
			{U"class", Tag::Class},
			{U"private", Tag::Private},
			{U"module", Tag::Module},
			{U"protected", Tag::Protected},
			{U"package", Tag::Package},
			{U"import", Tag::Import},
			{U"require", Tag::Require},
			{U"rename", Tag::Rename},
			{U"to", Tag::To},
			{U"interface", Tag::Interface}
	};

	std::unordered_map<std::u32string, Tag> Token::operators = {
			{U"+", Tag::Add},
			{U"-", Tag::Subtract},
			{U"*", Tag::Multiply},
			{U"/", Tag::Divide},
			{U"%", Tag::Modulo},
			{U">", Tag::GreaterThan},
			{U"<", Tag::LessThan},
			{U">=", Tag::GreaterThanOrEqual},
			{U"<=", Tag::LessThanOrEqual},
			{U"==", Tag::Equal},
			{U"!=", Tag::NotEqual},
			{U"(", Tag::LeftParenthesis},
			{U")", Tag::RightParenthesis},
			{U"[", Tag::LeftBracket},
			{U"]", Tag::RightBracket},
			{U"{", Tag::LeftBrace},
			{U"}", Tag::RightBrace},
			{U":", Tag::Colon},
			{U",", Tag::Comma},
			{U".", Tag::Dot},
			{U";", Tag::Semicolon},
			{U"=", Tag::Assign},
			{U"->", Tag::RightArrow},
			{U"=>", Tag::GoesTo},
			{U"@", Tag::At},
			{U"<:", Tag::UpperBound},
			{U":>", Tag::LowerBound}
	};
} // namespace cygni
