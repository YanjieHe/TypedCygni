#ifndef CYGNI_ENUM_HPP
#define CYGNI_ENUM_HPP

#include <string>
#include <iostream>

namespace cygni
{
	enum class Tag
	{
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
		Void,

		If,
		Else,
		While,

		For,
		Break,
		Return,

		Import,
		Uses,
		Require,
		Rename,
		GoesTo,

		LowerBound,
		UpperBound,

		Class,
		New,
		Private,
		Protected,
		Module,
		Package,
		Extends,
		Interface,
		To,
		Eof
	};

	enum class ExpressionType
	{
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
		MemberAccess,
		New,
		Parameter,
		Loop,
		Goto,
		VariableDefinition,
		While,
		Break,
		Convert,
		ArrayLength,
		// UpCast,
		DownCast
	};

	enum class AccessModifier { Public, Private, Protected };

	enum class TypeCode
	{
		Unknown,
		Void,
		Any,
		Boolean,
		Byte,
		Char,
		Float64,
		Empty,
		Int16,
		Int32,
		Int64,
		Object,
		Class,
		Module,
		Float32,
		String,
		UInt16,
		UInt32,
		UInt64,
		Array,
		Union,
		Function,
		Interface,
		Unresolved
	};

	enum class LocationType
	{
		Unknown,
		ModuleName,
		ModuleField,
		ModuleMethod,
		ClassName,
		ClassField,
		ClassMethod,
		InterfaceField,
		InterfaceMethod,
		InterfaceName,
		LocalVariable
	};

	enum class OpCode
	{
		PUSH_I32_0 = 0,
		PUSH_I32_1 = 1,
		PUSH_I32_1BYTE = 2,
		PUSH_I32_2BYTE = 3,
		PUSH_I64_0 = 4,
		PUSH_I64_1 = 5,
		PUSH_F64_0 = 6,
		PUSH_F64_1 = 7,
		PUSH_I32 = 8,
		PUSH_I64 = 9,
		PUSH_F32 = 10,
		PUSH_F64 = 11,
		PUSH_STRING = 12,
		PUSH_NULL = 13,
		PUSH_LOCAL_I32 = 14,
		PUSH_LOCAL_I64 = 15,
		PUSH_LOCAL_F32 = 16,
		PUSH_LOCAL_F64 = 17,
		PUSH_LOCAL_OBJECT = 18,
		POP_LOCAL_I32 = 19,
		POP_LOCAL_I64 = 20,
		POP_LOCAL_F32 = 21,
		POP_LOCAL_F64 = 22,
		POP_LOCAL_OBJECT = 23,
		PUSH_STATIC_I32 = 24,
		PUSH_STATIC_I64 = 25,
		PUSH_STATIC_F32 = 26,
		PUSH_STATIC_F64 = 27,
		PUSH_STATIC_OBJECT = 28,
		POP_STATIC_I32 = 29,
		POP_STATIC_I64 = 30,
		POP_STATIC_F32 = 31,
		POP_STATIC_F64 = 32,
		POP_STATIC_OBJECT = 33,
		PUSH_ARRAY_I32 = 34,
		PUSH_ARRAY_I64 = 35,
		PUSH_ARRAY_F32 = 36,
		PUSH_ARRAY_F64 = 37,
		PUSH_ARRAY_OBJECT = 38,
		POP_ARRAY_I32 = 39,
		POP_ARRAY_I64 = 40,
		POP_ARRAY_F32 = 41,
		POP_ARRAY_F64 = 42,
		POP_ARRAY_OBJECT = 43,
		PUSH_FIELD_I32 = 44,
		PUSH_FIELD_I64 = 45,
		PUSH_FIELD_F32 = 46,
		PUSH_FIELD_F64 = 47,
		PUSH_FIELD_OBJECT = 48,
		POP_FIELD_I32 = 49,
		POP_FIELD_I64 = 50,
		POP_FIELD_F32 = 51,
		POP_FIELD_F64 = 52,
		POP_FIELD_OBJECT = 53,
		ADD_I32 = 54,
		ADD_I64 = 55,
		ADD_F32 = 56,
		ADD_F64 = 57,
		SUB_I32 = 58,
		SUB_I64 = 59,
		SUB_F32 = 60,
		SUB_F64 = 61,
		MUL_I32 = 62,
		MUL_I64 = 63,
		MUL_F32 = 64,
		MUL_F64 = 65,
		DIV_I32 = 66,
		DIV_I64 = 67,
		DIV_F32 = 68,
		DIV_F64 = 69,
		MOD_I32 = 70,
		MOD_I64 = 71,
		MOD_F32 = 72,
		MOD_F64 = 73,
		BIT_AND_I32 = 74,
		BIT_AND_I64 = 75,
		BIT_OR_I32 = 76,
		BIT_OR_I64 = 77,
		BIT_XOR_I32 = 78,
		BIT_XOR_I64 = 79,
		MINUS_I32 = 80,
		MINUS_I64 = 81,
		MINUS_F32 = 82,
		MINUS_F64 = 83,
		BIT_NOT_I32 = 84,
		BIT_NOT_I64 = 85,
		CAST_I32_TO_I64 = 86,
		CAST_I32_TO_F32 = 87,
		CAST_I32_TO_F64 = 88,
		CAST_I64_TO_I32 = 89,
		CAST_I64_TO_F32 = 90,
		CAST_I64_TO_F64 = 91,
		CAST_F32_TO_I32 = 92,
		CAST_F32_TO_I64 = 93,
		CAST_F32_TO_F64 = 94,
		CAST_F64_TO_I32 = 95,
		CAST_F64_TO_I64 = 96,
		CAST_F64_TO_F32 = 97,
		UP_CAST = 98,
		DOWN_CAST = 99,
		EQ_I32 = 100,
		EQ_I64 = 101,
		EQ_F32 = 102,
		EQ_F64 = 103,
		NE_I32 = 104,
		NE_I64 = 105,
		NE_F32 = 106,
		NE_F64 = 107,
		GT_I32 = 108,
		GT_I64 = 109,
		GT_F32 = 110,
		GT_F64 = 111,
		LT_I32 = 112,
		LT_I64 = 113,
		LT_F32 = 114,
		LT_F64 = 115,
		GE_I32 = 116,
		GE_I64 = 117,
		GE_F32 = 118,
		GE_F64 = 119,
		LE_I32 = 120,
		LE_I64 = 121,
		LE_F32 = 122,
		LE_F64 = 123,
		POP = 124,
		DUPLICATE = 125,
		DUPLICATE_OFFSET = 126,
		JUMP = 127,
		JUMP_IF_TRUE = 128,
		JUMP_IF_FALSE = 129,
		PUSH_FUNCTION = 130,
		PUSH_METHOD = 131,
		INVOKE = 132,
		RETURN_I32 = 133,
		RETURN_I64 = 134,
		RETURN_F32 = 135,
		RETURN_F64 = 136,
		RETURN_OBJECT = 137,
		NEW = 138,
		NEW_ARRAY = 139,
		NEW_ARRAY_LITERAL_I32 = 140,
		NEW_ARRAY_LITERAL_I64 = 141,
		NEW_ARRAY_LITERAL_F32 = 142,
		NEW_ARRAY_LITERAL_F64 = 143,
		NEW_ARRAY_LITERAL_OBJECT = 144,
		ARRAY_LENGTH = 145,
		SUPER = 146,
		INSTANCE_OF = 147,
		THROW = 148,
		RETHROW = 149,
		GO_FINALLY = 150,
		FINALLY_END = 151
	};

	enum class TypeTag
	{
		TYPE_I32,
		TYPE_I64,
		TYPE_F32,
		TYPE_F64,
		TYPE_STRING,
		TYPE_OBJECT
	};

	enum class ConstantKind
	{
		CONSTANT_FLAG_I32,
		CONSTANT_FLAG_I64,
		CONSTANT_FLAG_F32,
		CONSTANT_FLAG_F64,
		CONSTANT_FLAG_BOOLEAN,
		CONSTANT_FLAG_CHAR,
		CONSTANT_FLAG_STRING,
		CONSTANT_FLAG_METHOD,
		CONSTANT_FLAG_CLASS,
		CONSTANT_FLAG_STATIC_VAR,
		CONSTANT_FLAG_STATIC_FUNCTION,
	};

	enum class MethodFlag
	{
		NativeFunction = 0,
		InstanceMethod,
		ModuleFunction,
	};

	template <typename TEnum>
	class Enum
	{
	public:
		static std::u32string ToString(TEnum)
		{
			std::cerr << __FUNCTION__ << std::endl;
			exit(1);
		}
	};

	template <>
	class Enum<Tag>
	{
	public:
		static std::u32string ToString(Tag tag)
		{
			switch (tag)
			{
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
			case Tag::Void:
				return U"Void";
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
			case Tag::Require:
				return U"Require";
			case Tag::Rename:
				return U"Rename";
			case Tag::GoesTo:
				return  U"GoesTo";
			case Tag::LowerBound:
				return U"LowerBound";
			case Tag::UpperBound:
				return U"UpperBound";
			case Tag::Class:
				return U"Class";
			case Tag::New:
				return U"New";
			case Tag::Private:
				return U"Private";
			case Tag::Protected:
				return U"Protected";
			case Tag::Module:
				return U"Module";
			case Tag::Package:
				return U"Package";
			case Tag::Extends:
				return U"Extends";
			case Tag::Interface:
				return U"Interface";
			case Tag::To:
				return U"To";
			case Tag::Eof:
				return U"Eof";
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <>
	class Enum<ExpressionType>
	{
	public:
		static std::u32string ToString(ExpressionType kind)
		{
			switch (kind)
			{
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
			case ExpressionType::MemberAccess:
				return U"MemberAccess";
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
			case ExpressionType::Convert:
				return U"Convert";
			case ExpressionType::ArrayLength:
				return U"ArrayLength";
			// case ExpressionType::UpCast:
			// 	return U"UpCast";
			case ExpressionType::DownCast:
				return U"DownCast";

			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};
	template <> class Enum<AccessModifier>
	{
	public:
		static std::u32string ToString(AccessModifier modifier)
		{
			switch (modifier)
			{
			case AccessModifier::Public:
				return U"Public";
			case AccessModifier::Private:
				return U"Private";
			case AccessModifier::Protected:
				return U"Protected";
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<TypeCode>
	{
	public:
		static std::u32string ToString(TypeCode typeCode)
		{
			switch (typeCode)
			{
			case TypeCode::Unknown:
				return U"Unknown";
			case TypeCode::Void:
				return U"Void";
			case TypeCode::Any:
				return U"Any";
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
			case TypeCode::Class:
				return U"Class";
			case TypeCode::Module:
				return U"Module";
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
			case TypeCode::Union:
				return U"Union";
			case TypeCode::Function:
				return U"Function";
			case TypeCode::Interface:
				return U"Interface";
			case TypeCode::Unresolved:
				return U"Unresolved";

			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<LocationType>
	{
	public:
		static std::u32string ToString(LocationType type)
		{
			switch (type)
			{
			case LocationType::Unknown:
				return U"Unknown";
			case LocationType::ModuleName:
				return U"ModuleName";
			case LocationType::ModuleField:
				return U"ModuleField";
			case LocationType::ModuleMethod:
				return U"ModuleMethod";
			case LocationType::ClassName:
				return U"ClassName";
			case LocationType::ClassField:
				return U"ClassField";
			case LocationType::ClassMethod:
				return U"ClassMethod";
			case LocationType::InterfaceField:
				return U"InterfaceField";
			case LocationType::InterfaceMethod:
				return U"InterfaceMethod";
			case LocationType::InterfaceName:
				return U"InterfaceName";
			case LocationType::LocalVariable:
				return U"LocalVariable";
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<OpCode>
	{
	public:
		static std::u32string ToString(OpCode code)
		{
			switch (code)
			{
			case OpCode::PUSH_I32_0:
				return U"PUSH_I32_0";
			case OpCode::PUSH_I32_1:
				return U"PUSH_I32_1";
			case OpCode::PUSH_I32_1BYTE:
				return U"PUSH_I32_1BYTE";
			case OpCode::PUSH_I32_2BYTE:
				return U"PUSH_I32_2BYTE";
			case OpCode::PUSH_I64_0:
				return U"PUSH_I64_0";
			case OpCode::PUSH_I64_1:
				return U"PUSH_I64_1";
			case OpCode::PUSH_F64_0:
				return U"PUSH_F64_0";
			case OpCode::PUSH_F64_1:
				return U"PUSH_F64_1";
			case OpCode::PUSH_I32:
				return U"PUSH_I32";
			case OpCode::PUSH_I64:
				return U"PUSH_I64";
			case OpCode::PUSH_F32:
				return U"PUSH_F32";
			case OpCode::PUSH_F64:
				return U"PUSH_F64";
			case OpCode::PUSH_STRING:
				return U"PUSH_STRING";
			case OpCode::PUSH_NULL:
				return U"PUSH_NULL";
			case OpCode::PUSH_LOCAL_I32:
				return U"PUSH_LOCAL_I32";
			case OpCode::PUSH_LOCAL_I64:
				return U"PUSH_LOCAL_I64";
			case OpCode::PUSH_LOCAL_F32:
				return U"PUSH_LOCAL_F32";
			case OpCode::PUSH_LOCAL_F64:
				return U"PUSH_LOCAL_F64";
			case OpCode::PUSH_LOCAL_OBJECT:
				return U"PUSH_LOCAL_OBJECT";
			case OpCode::POP_LOCAL_I32:
				return U"POP_LOCAL_I32";
			case OpCode::POP_LOCAL_I64:
				return U"POP_LOCAL_I64";
			case OpCode::POP_LOCAL_F32:
				return U"POP_LOCAL_F32";
			case OpCode::POP_LOCAL_F64:
				return U"POP_LOCAL_F64";
			case OpCode::POP_LOCAL_OBJECT:
				return U"POP_LOCAL_OBJECT";
			case OpCode::PUSH_STATIC_I32:
				return U"PUSH_STATIC_I32";
			case OpCode::PUSH_STATIC_I64:
				return U"PUSH_STATIC_I64";
			case OpCode::PUSH_STATIC_F32:
				return U"PUSH_STATIC_F32";
			case OpCode::PUSH_STATIC_F64:
				return U"PUSH_STATIC_F64";
			case OpCode::PUSH_STATIC_OBJECT:
				return U"PUSH_STATIC_OBJECT";
			case OpCode::POP_STATIC_I32:
				return U"POP_STATIC_I32";
			case OpCode::POP_STATIC_I64:
				return U"POP_STATIC_I64";
			case OpCode::POP_STATIC_F32:
				return U"POP_STATIC_F32";
			case OpCode::POP_STATIC_F64:
				return U"POP_STATIC_F64";
			case OpCode::POP_STATIC_OBJECT:
				return U"POP_STATIC_OBJECT";
			case OpCode::PUSH_ARRAY_I32:
				return U"PUSH_ARRAY_I32";
			case OpCode::PUSH_ARRAY_I64:
				return U"PUSH_ARRAY_I64";
			case OpCode::PUSH_ARRAY_F32:
				return U"PUSH_ARRAY_F32";
			case OpCode::PUSH_ARRAY_F64:
				return U"PUSH_ARRAY_F64";
			case OpCode::PUSH_ARRAY_OBJECT:
				return U"PUSH_ARRAY_OBJECT";
			case OpCode::POP_ARRAY_I32:
				return U"POP_ARRAY_I32";
			case OpCode::POP_ARRAY_I64:
				return U"POP_ARRAY_I64";
			case OpCode::POP_ARRAY_F32:
				return U"POP_ARRAY_F32";
			case OpCode::POP_ARRAY_F64:
				return U"POP_ARRAY_F64";
			case OpCode::POP_ARRAY_OBJECT:
				return U"POP_ARRAY_OBJECT";
			case OpCode::PUSH_FIELD_I32:
				return U"PUSH_FIELD_I32";
			case OpCode::PUSH_FIELD_I64:
				return U"PUSH_FIELD_I64";
			case OpCode::PUSH_FIELD_F32:
				return U"PUSH_FIELD_F32";
			case OpCode::PUSH_FIELD_F64:
				return U"PUSH_FIELD_F64";
			case OpCode::PUSH_FIELD_OBJECT:
				return U"PUSH_FIELD_OBJECT";
			case OpCode::POP_FIELD_I32:
				return U"POP_FIELD_I32";
			case OpCode::POP_FIELD_I64:
				return U"POP_FIELD_I64";
			case OpCode::POP_FIELD_F32:
				return U"POP_FIELD_F32";
			case OpCode::POP_FIELD_F64:
				return U"POP_FIELD_F64";
			case OpCode::POP_FIELD_OBJECT:
				return U"POP_FIELD_OBJECT";
			case OpCode::ADD_I32:
				return U"ADD_I32";
			case OpCode::ADD_I64:
				return U"ADD_I64";
			case OpCode::ADD_F32:
				return U"ADD_F32";
			case OpCode::ADD_F64:
				return U"ADD_F64";
			case OpCode::SUB_I32:
				return U"SUB_I32";
			case OpCode::SUB_I64:
				return U"SUB_I64";
			case OpCode::SUB_F32:
				return U"SUB_F32";
			case OpCode::SUB_F64:
				return U"SUB_F64";
			case OpCode::MUL_I32:
				return U"MUL_I32";
			case OpCode::MUL_I64:
				return U"MUL_I64";
			case OpCode::MUL_F32:
				return U"MUL_F32";
			case OpCode::MUL_F64:
				return U"MUL_F64";
			case OpCode::DIV_I32:
				return U"DIV_I32";
			case OpCode::DIV_I64:
				return U"DIV_I64";
			case OpCode::DIV_F32:
				return U"DIV_F32";
			case OpCode::DIV_F64:
				return U"DIV_F64";
			case OpCode::MOD_I32:
				return U"MOD_I32";
			case OpCode::MOD_I64:
				return U"MOD_I64";
			case OpCode::MOD_F32:
				return U"MOD_F32";
			case OpCode::MOD_F64:
				return U"MOD_F64";
			case OpCode::BIT_AND_I32:
				return U"BIT_AND_I32";
			case OpCode::BIT_AND_I64:
				return U"BIT_AND_I64";
			case OpCode::BIT_OR_I32:
				return U"BIT_OR_I32";
			case OpCode::BIT_OR_I64:
				return U"BIT_OR_I64";
			case OpCode::BIT_XOR_I32:
				return U"BIT_XOR_I32";
			case OpCode::BIT_XOR_I64:
				return U"BIT_XOR_I64";
			case OpCode::MINUS_I32:
				return U"MINUS_I32";
			case OpCode::MINUS_I64:
				return U"MINUS_I64";
			case OpCode::MINUS_F32:
				return U"MINUS_F32";
			case OpCode::MINUS_F64:
				return U"MINUS_F64";
			case OpCode::BIT_NOT_I32:
				return U"BIT_NOT_I32";
			case OpCode::BIT_NOT_I64:
				return U"BIT_NOT_I64";
			case OpCode::CAST_I32_TO_I64:
				return U"CAST_I32_TO_I64";
			case OpCode::CAST_I32_TO_F32:
				return U"CAST_I32_TO_F32";
			case OpCode::CAST_I32_TO_F64:
				return U"CAST_I32_TO_F64";
			case OpCode::CAST_I64_TO_I32:
				return U"CAST_I64_TO_I32";
			case OpCode::CAST_I64_TO_F32:
				return U"CAST_I64_TO_F32";
			case OpCode::CAST_I64_TO_F64:
				return U"CAST_I64_TO_F64";
			case OpCode::CAST_F32_TO_I32:
				return U"CAST_F32_TO_I32";
			case OpCode::CAST_F32_TO_I64:
				return U"CAST_F32_TO_I64";
			case OpCode::CAST_F32_TO_F64:
				return U"CAST_F32_TO_F64";
			case OpCode::CAST_F64_TO_I32:
				return U"CAST_F64_TO_I32";
			case OpCode::CAST_F64_TO_I64:
				return U"CAST_F64_TO_I64";
			case OpCode::CAST_F64_TO_F32:
				return U"CAST_F64_TO_F32";
			case OpCode::UP_CAST:
				return U"UP_CAST";
			case OpCode::DOWN_CAST:
				return U"DOWN_CAST";
			case OpCode::EQ_I32:
				return U"EQ_I32";
			case OpCode::EQ_I64:
				return U"EQ_I64";
			case OpCode::EQ_F32:
				return U"EQ_F32";
			case OpCode::EQ_F64:
				return U"EQ_F64";
			case OpCode::NE_I32:
				return U"NE_I32";
			case OpCode::NE_I64:
				return U"NE_I64";
			case OpCode::NE_F32:
				return U"NE_F32";
			case OpCode::NE_F64:
				return U"NE_F64";
			case OpCode::GT_I32:
				return U"GT_I32";
			case OpCode::GT_I64:
				return U"GT_I64";
			case OpCode::GT_F32:
				return U"GT_F32";
			case OpCode::GT_F64:
				return U"GT_F64";
			case OpCode::LT_I32:
				return U"LT_I32";
			case OpCode::LT_I64:
				return U"LT_I64";
			case OpCode::LT_F32:
				return U"LT_F32";
			case OpCode::LT_F64:
				return U"LT_F64";
			case OpCode::GE_I32:
				return U"GE_I32";
			case OpCode::GE_I64:
				return U"GE_I64";
			case OpCode::GE_F32:
				return U"GE_F32";
			case OpCode::GE_F64:
				return U"GE_F64";
			case OpCode::LE_I32:
				return U"LE_I32";
			case OpCode::LE_I64:
				return U"LE_I64";
			case OpCode::LE_F32:
				return U"LE_F32";
			case OpCode::LE_F64:
				return U"LE_F64";
			case OpCode::POP:
				return U"POP";
			case OpCode::DUPLICATE:
				return U"DUPLICATE";
			case OpCode::DUPLICATE_OFFSET:
				return U"DUPLICATE_OFFSET";
			case OpCode::JUMP:
				return U"JUMP";
			case OpCode::JUMP_IF_TRUE:
				return U"JUMP_IF_TRUE";
			case OpCode::JUMP_IF_FALSE:
				return U"JUMP_IF_FALSE";
			case OpCode::PUSH_FUNCTION:
				return U"PUSH_FUNCTION";
			case OpCode::PUSH_METHOD:
				return U"PUSH_METHOD";
			case OpCode::INVOKE:
				return U"INVOKE";
			case OpCode::RETURN_I32:
				return U"RETURN_I32";
			case OpCode::RETURN_I64:
				return U"RETURN_I64";
			case OpCode::RETURN_F32:
				return U"RETURN_F32";
			case OpCode::RETURN_F64:
				return U"RETURN_F64";
			case OpCode::RETURN_OBJECT:
				return U"RETURN_OBJECT";
			case OpCode::NEW:
				return U"NEW";
			case OpCode::NEW_ARRAY:
				return U"NEW_ARRAY";
			case OpCode::NEW_ARRAY_LITERAL_I32:
				return U"NEW_ARRAY_LITERAL_I32";
			case OpCode::NEW_ARRAY_LITERAL_I64:
				return U"NEW_ARRAY_LITERAL_I64";
			case OpCode::NEW_ARRAY_LITERAL_F32:
				return U"NEW_ARRAY_LITERAL_F32";
			case OpCode::NEW_ARRAY_LITERAL_F64:
				return U"NEW_ARRAY_LITERAL_F64";
			case OpCode::NEW_ARRAY_LITERAL_OBJECT:
				return U"NEW_ARRAY_LITERAL_OBJECT";
			case OpCode::ARRAY_LENGTH:
				return U"ARRAY_LENGTH";
			case OpCode::SUPER:
				return U"SUPER";
			case OpCode::INSTANCE_OF:
				return U"INSTANCE_OF";
			case OpCode::THROW:
				return U"THROW";
			case OpCode::RETHROW:
				return U"RETHROW";
			case OpCode::GO_FINALLY:
				return U"GO_FINALLY";
			case OpCode::FINALLY_END:
				return U"FINALLY_END";
			default: {
				std::cout << __FUNCTION__ << std::endl;
				exit(1);
			}
			}

		}
	};

	template <> class Enum<TypeTag>
	{
	public:
		static std::u32string ToString(TypeTag tag)
		{
			switch (tag)
			{
			case TypeTag::TYPE_I32:
				return U"TYPE_I32";
			case TypeTag::TYPE_I64:
				return U"TYPE_I64";
			case TypeTag::TYPE_F32:
				return U"TYPE_F32";
			case TypeTag::TYPE_F64:
				return U"TYPE_F64";
			case TypeTag::TYPE_STRING:
				return U"TYPE_STRING";
			case TypeTag::TYPE_OBJECT:
				return U"TYPE_OBJECT";
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};


	template <> class Enum<ConstantKind>
	{
	public:
		static std::u32string ToString(ConstantKind kind)
		{
			switch (kind)
			{
			case ConstantKind::CONSTANT_FLAG_I32:
				return U"Int32";
			case ConstantKind::CONSTANT_FLAG_I64:
				return U"CONSTANT_FLAG_I64";
			case ConstantKind::CONSTANT_FLAG_F32:
				return U"CONSTANT_FLAG_F32";
			case ConstantKind::CONSTANT_FLAG_F64:
				return U"CONSTANT_FLAG_F64";
			case ConstantKind::CONSTANT_FLAG_CHAR:
				return U"CONSTANT_FLAG_CHAR";
			case ConstantKind::CONSTANT_FLAG_BOOLEAN:
				return U"CONSTANT_FLAG_BOOLEAN";
			case ConstantKind::CONSTANT_FLAG_STRING:
				return U"CONSTANT_FLAG_STRING";
			case ConstantKind::CONSTANT_FLAG_METHOD:
				return U"CONSTANT_FLAG_METHOD";
			case ConstantKind::CONSTANT_FLAG_CLASS:
				return U"CONSTANT_FLAG_CLASS";
			case ConstantKind::CONSTANT_FLAG_STATIC_VAR:
				return U"CONSTANT_FLAG_STATIC_VAR";
			case ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION:
				return U"CONSTANT_FLAG_STATIC_FUNCTION";
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <>
	class Enum <MethodFlag>
	{
	public:
		static std::u32string ToString(MethodFlag flag)
		{
			switch (flag)
			{
			case MethodFlag::NativeFunction: {
				return U"NativeFunction";
			}
			case MethodFlag::InstanceMethod: {
				return U"InstanceMethod";
			}
			case MethodFlag::ModuleFunction: {
				return U"ModuleFunction";
			}
			default:
				std::cerr << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};
} // namespace cygni

#endif // CYGNI_ENUM_HPP
