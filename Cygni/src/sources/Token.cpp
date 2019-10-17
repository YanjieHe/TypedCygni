#include "Token.hpp"
#include <iostream>

namespace cygni {

Token::Token(int line, int column, Tag tag, const std::u32string& text)
	: line{line}, column{column}, tag{IdentifyKeyword(tag, text)}, text{text} {
}

Tag Token::IdentifyKeyword(Tag tag, const std::u32string& text) {
	static std::unordered_map<std::u32string, Tag> keywords = {
		{U"if", Tag::If},		  {U"else", Tag::Else},
		{U"var", Tag::Var},		  {U"def", Tag::Def},
		{U"return", Tag::Return}, {U"true", Tag::True},
		{U"false", Tag::False},   {U"while", Tag::While},
		{U"class", Tag::Class},   {U"private", Tag::Private},
		{U"module", Tag::Module}, {U"protected", Tag::Protected}};
	if (tag == Tag::Identifier) {
		if (keywords.find(text) != keywords.end()) {
			return keywords[text];
		} else {
			return tag;
		}
	} else {
		return tag;
	}
}

std::string TagToString(Tag tag) {
	switch (tag) {
	case Tag::Identifier:
		return "Identifier";
	case Tag::Integer:
		return "Integer";
	case Tag::Float:
		return "Float";
	case Tag::Character:
		return "Character";
	case Tag::String:
		return "String";
	case Tag::Add:
		return "Add";
	case Tag::Subtract:
		return "Subtract";
	case Tag::Multiply:
		return "Multiply";
	case Tag::Divide:
		return "Divide";
	case Tag::Modulo:
		return "Modulo";
	case Tag::Equal:
		return "Equal";
	case Tag::NotEqual:
		return "NotEqual";
	case Tag::GreaterThan:
		return "GreaterThan";
	case Tag::GreaterThanOrEqual:
		return "GreaterThanOrEqual";
	case Tag::LessThan:
		return "LessThan";
	case Tag::LessThanOrEqual:
		return "LessThanOrEqual";
	case Tag::And:
		return "And";
	case Tag::Or:
		return "Or";
	case Tag::Not:
		return "Not";
	case Tag::Var:
		return "Var";
	case Tag::Def:
		return "Def";
	case Tag::Assign:
		return "Assign";
	case Tag::LeftBrace:
		return "LeftBrace";
	case Tag::RightBrace:
		return "RightBrace";
	case Tag::LeftBracket:
		return "LeftBracket";
	case Tag::RightBracket:
		return "RightBracket";
	case Tag::LeftParenthesis:
		return "LeftParenthesis";
	case Tag::RightParenthesis:
		return "RightParenthesis";
	case Tag::Dot:
		return "Dot";
	case Tag::Comma:
		return "Comma";
	case Tag::Semicolon:
		return "Semicolon";
	case Tag::Colon:
		return "Colon";
	case Tag::RightArrow:
		return "RightArrow";
	case Tag::True:
		return "True";
	case Tag::False:
		return "False";
	case Tag::If:
		return "If";
	case Tag::Else:
		return "Else";
	case Tag::While:
		return "While";
	case Tag::For:
		return "For";
	case Tag::Break:
		return "Break";
	case Tag::Return:
		return "Return";
	case Tag::Import:
		return "Import";
	case Tag::Uses:
		return "Uses";
	case Tag::LowerBound:
		return "LowerBound";
	case Tag::UpperBound:
		return "UpperBound";
	case Tag::Class:
		return "Class";
	case Tag::Private:
		return "Private";
	case Tag::Protected:
		return "Protected";
	case Tag::Module:
		return "Module";
	case Tag::Eof:
		return "Eof";
	default:
		std::cout << __FUNCTION__ << std::endl;
		exit(1);
	}
}

} // namespace cygni
