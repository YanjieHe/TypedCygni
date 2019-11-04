#ifndef CYGNI_ENUM_HPP
#define CYGNI_ENUM_HPP

#include "PreDef.hpp"

namespace cygni {
enum class Tag {
	Identifier,
	Integer,
	Float,
	Character,
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
	At,

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

template <typename TEnum> class Enum {
public:
	static std::u32string ToString(TEnum);
};

template <> class Enum<Tag> {
public:
	static std::u32string ToString(Tag tag) {
		switch (tag) {
		case Tag::Identifier:
			return U"Identifier";
		case Tag::Integer:
			return U"Integer";
		case Tag::Float:
			return U"Float";
		case Tag::Character:
			return U"Character";
		case Tag::String:
			return U"String";
		case Tag::Add:
			return U"Add";
		case Tag::Subtract:
			return U"Subtract";
		case Tag::Multiply:
			return U"Multiply";
		case Tag::Divide:
			return U"Divide";
		case Tag::Modulo:
			return U"Modulo";
		case Tag::Equal:
			return U"Equal";
		case Tag::NotEqual:
			return U"NotEqual";
		case Tag::GreaterThan:
			return U"GreaterThan";
		case Tag::GreaterThanOrEqual:
			return U"GreaterThanOrEqual";
		case Tag::LessThan:
			return U"LessThan";
		case Tag::LessThanOrEqual:
			return U"LessThanOrEqual";
		case Tag::And:
			return U"And";
		case Tag::Or:
			return U"Or";
		case Tag::Not:
			return U"Not";
		case Tag::Var:
			return U"Var";
		case Tag::Def:
			return U"Def";
		case Tag::Assign:
			return U"Assign";
		case Tag::LeftBrace:
			return U"LeftBrace";
		case Tag::RightBrace:
			return U"RightBrace";
		case Tag::LeftBracket:
			return U"LeftBracket";
		case Tag::RightBracket:
			return U"RightBracket";
		case Tag::LeftParenthesis:
			return U"LeftParenthesis";
		case Tag::RightParenthesis:
			return U"RightParenthesis";
		case Tag::Dot:
			return U"Dot";
		case Tag::Comma:
			return U"Comma";
		case Tag::Semicolon:
			return U"Semicolon";
		case Tag::Colon:
			return U"Colon";
		case Tag::RightArrow:
			return U"RightArrow";
		case Tag::At:
			return U"At";
		case Tag::True:
			return U"True";
		case Tag::False:
			return U"False";
		case Tag::If:
			return U"If";
		case Tag::Else:
			return U"Else";
		case Tag::While:
			return U"While";
		case Tag::For:
			return U"For";
		case Tag::Break:
			return U"Break";
		case Tag::Return:
			return U"Return";
		case Tag::Import:
			return U"Import";
		case Tag::Uses:
			return U"Uses";
		case Tag::LowerBound:
			return U"LowerBound";
		case Tag::UpperBound:
			return U"UpperBound";
		case Tag::Class:
			return U"Class";
		case Tag::Private:
			return U"Private";
		case Tag::Protected:
			return U"Protected";
		case Tag::Module:
			return U"Module";
		case Tag::Eof:
			return U"Eof";
		default:
			std::cout << __FUNCTION__ << std::endl;
			exit(1);
		}
	}
};

} // namespace cygni

#endif // CYGNI_ENUM_HPP
