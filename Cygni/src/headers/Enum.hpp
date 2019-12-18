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

enum class ExpressionType {
	Add,
	Subtract,
	Multiply,
	Divide,
	Modulo,
	GreaterThan,
	LessThan,
	GreaterThanOrEqual,
	LessThanOrEqual,
	Equal,
	NotEqual,
	And,
	Or,
	Not,
	UnaryPlus,
	UnaryMinus,
	Invoke,
	Conditional,
	Default,
	Constant,
	Block,
	Name,
	Return,
	Var,
	Def,
	Assign,
	MethodCall,
	New,
	Parameter,
	Loop,
	Goto,
	VariableDefinition,
	While,
	Break
};

enum class AccessModifier { Public, Private, Protected };

enum class TypeCode {
	Unknown,
	Boolean,
	Byte,
	Char,
	Float64,
	Empty,
	Int16,
	Int32,
	Int64,
	Object,
	Float32,
	String,
	UInt16,
	UInt32,
	UInt64,
	Array
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

template <> class Enum<ExpressionType> {
public:
	static std::u32string ToString(ExpressionType kind) {
		switch (kind) {
		case ExpressionType::Add:
			return U"Add";
		case ExpressionType::Subtract:
			return U"Subtract";
		case ExpressionType::Multiply:
			return U"Multiply";
		case ExpressionType::Divide:
			return U"Divide";
		case ExpressionType::Modulo:
			return U"Modulo";
		case ExpressionType::GreaterThan:
			return U"GreaterThan";
		case ExpressionType::LessThan:
			return U"LessThan";
		case ExpressionType::GreaterThanOrEqual:
			return U"GreaterThanOrEqual";
		case ExpressionType::LessThanOrEqual:
			return U"LessThanOrEqual";
		case ExpressionType::Equal:
			return U"Equal";
		case ExpressionType::NotEqual:
			return U"NotEqual";
		case ExpressionType::And:
			return U"And";
		case ExpressionType::Or:
			return U"Or";
		case ExpressionType::Not:
			return U"Not";
		case ExpressionType::UnaryPlus:
			return U"UnaryPlus";
		case ExpressionType::UnaryMinus:
			return U"UnaryMinus";
		case ExpressionType::Invoke:
			return U"Invoke";
		case ExpressionType::Conditional:
			return U"Conditional";
		case ExpressionType::Default:
			return U"Default";
		case ExpressionType::Constant:
			return U"Constant";
		case ExpressionType::Block:
			return U"Block";
		case ExpressionType::Name:
			return U"Name";
		case ExpressionType::Return:
			return U"Return";
		case ExpressionType::Var:
			return U"Var";
		case ExpressionType::Def:
			return U"Def";
		case ExpressionType::Assign:
			return U"Assign";
		case ExpressionType::MethodCall:
			return U"MethodCall";
		case ExpressionType::New:
			return U"New";
		case ExpressionType::Parameter:
			return U"Parameter";
		case ExpressionType::Loop:
			return U"Loop";
		case ExpressionType::Goto:
			return U"Goto";
		case ExpressionType::VariableDefinition:
			return U"VariableDefinition";
		case ExpressionType::While:
			return U"While";
		case ExpressionType::Break:
			return U"Break";

		default:
			std::cout << __FUNCTION__ << std::endl;
			exit(1);
		}
	}
};
template <> class Enum<AccessModifier> {
public:
	static std::u32string ToString(AccessModifier modifier) {
		switch (modifier) {
		case AccessModifier::Public:
			return U"Public";
		case AccessModifier::Private:
			return U"Private";
		case AccessModifier::Protected:
			return U"Protected";
		default:
			std::cout << __FUNCTION__ << std::endl;
			exit(1);
		}
	}
};

template <> class Enum<TypeCode> {
public:
	static std::u32string ToString(TypeCode typeCode) {
		switch (typeCode) {
		case TypeCode::Unknown:
			return U"Unknown";
		case TypeCode::Boolean:
			return U"Boolean";
		case TypeCode::Byte:
			return U"Byte";
		case TypeCode::Char:
			return U"Char";
		case TypeCode::Float32:
			return U"Float32";
		case TypeCode::Empty:
			return U"Empty";
		case TypeCode::Int16:
			return U"Int16";
		case TypeCode::Int32:
			return U"Int32";
		case TypeCode::Int64:
			return U"Int64";
		case TypeCode::Object:
			return U"Object";
		case TypeCode::Float64:
			return U"Float64";
		case TypeCode::String:
			return U"String";
		case TypeCode::UInt16:
			return U"UInt16";
		case TypeCode::UInt32:
			return U"UInt32";
		case TypeCode::UInt64:
			return U"UInt64";
		case TypeCode::Array:
			return U"Array";

		default:
			std::cout << __FUNCTION__ << std::endl;
			exit(1);
		}
	}
};

} // namespace cygni

#endif // CYGNI_ENUM_HPP
