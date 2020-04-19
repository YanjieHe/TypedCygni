#ifndef CYGNI_ENUM_HPP
#define CYGNI_ENUM_HPP

#include <string>
#include <iostream>

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
		New,
		Private,
		Protected,
		Module,
		Package,
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
		MemberAccess,
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
		Function

	};

	enum class ParameterType {
		Unknown,
		ModuleName,
		ModuleField,
		ModuleMethod,
		ClassName,
		ClassField,
		ClassMethod,
		LocalVariable
	};

	enum class OpCode {
		PUSH_INT_1BYTE,
		PUSH_INT_2BYTE,
		PUSH_INT,

		PUSH_DOUBLE_0,
		PUSH_DOUBLE_1,
		PUSH_DOUBLE,
		PUSH_NULL,

		PUSH_STACK,
		POP_STACK,
		PUSH_CONSTANT,
		POP_CONSTANT,
		PUSH_ARRAY,
		POP_ARRAY,
		PUSH_FIELD,
		POP_FIELD,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		BIT_AND,
		BIT_OR,
		BIT_XOR,
		MINUS,
		BIT_NOT,
		CAST,
		UP_CAST,
		DOWN_CAST,
		EQ,
		NE,
		GT,
		LT,
		GE,
		LE,
		LOGICAL_AND,
		LOGICAL_OR,
		LOGICAL_NOT,
		POP,
		DUPLICATE,
		DUPLICATE_OFFSET,
		JUMP,
		JUMP_IF_TRUE,
		JUMP_IF_FALSE,
		PUSH_MODULE,
		PUSH_FUNCTION,
		INOVKE,
		RETURN,
		NEW,
		SUPER,
		INSTANCE_OF,
		THROW,
		RETHROW,
		GO_FINALLY,
		FINALLY_END
	};

	enum class TypeTag {
		TYPE_I32,
		TYPE_I64,
		TYPE_F32,
		TYPE_F64,
		TYPE_STRING,
		TYPE_OBJECT
	};
	template <typename TEnum> class Enum {
	public:
		static std::u32string ToString(TEnum) {
			std::cout << __FUNCTION__ << std::endl;
			exit(1);
		}
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

			default:
				std::cout << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<ParameterType> {
	public:
		static std::u32string ToString(ParameterType type) {
			switch (type) {
			case ParameterType::Unknown:
				return U"Unknown";
			case ParameterType::ModuleName:
				return U"ModuleName";
			case ParameterType::ModuleField:
				return U"ModuleField";
			case ParameterType::ModuleMethod:
				return U"ModuleMethod";
			case ParameterType::ClassName:
				return U"ClassName";
			case ParameterType::ClassField:
				return U"ClassField";
			case ParameterType::ClassMethod:
				return U"ClassMethod";
			case ParameterType::LocalVariable:
				return U"LocalVariable";
			default:
				std::cout << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<OpCode> {
	public:
		static std::u32string ToString(OpCode code) {
			swtich(code) {
			case OpCode::PUSH_INT_1BYTE:
				return L"PUSH_INT_1BYTE";
			case OpCode::PUSH_INT_2BYTE:
				return L"PUSH_INT_2BYTE";
			case OpCode::PUSH_INT:
				return L"PUSH_INT";
			case OpCode::PUSH_DOUBLE_0:
				return L"PUSH_DOUBLE_0";
			case OpCode::PUSH_DOUBLE_1:
				return L"PUSH_DOUBLE_1";
			case OpCode::PUSH_DOUBLE:
				return L"PUSH_DOUBLE";
			case OpCode::PUSH_NULL:
				return L"PUSH_NULL";
			case OpCode::PUSH_STACK:
				return L"PUSH_STACK";
			case OpCode::POP_STACK:
				return L"POP_STACK";
			case OpCode::PUSH_CONSTANT:
				return L"PUSH_CONSTANT";
			case OpCode::POP_CONSTANT:
				return L"POP_CONSTANT";
			case OpCode::PUSH_ARRAY:
				return L"PUSH_ARRAY";
			case OpCode::POP_ARRAY:
				return L"POP_ARRAY";
			case OpCode::PUSH_FIELD:
				return L"PUSH_FIELD";
			case OpCode::POP_FIELD:
				return L"POP_FIELD";
			case OpCode::ADD:
				return L"ADD";
			case OpCode::SUB:
				return L"SUB";
			case OpCode::MUL:
				return L"MUL";
			case OpCode::DIV:
				return L"DIV";
			case OpCode::MOD:
				return L"MOD";
			case OpCode::BIT_AND:
				return L"BIT_AND";
			case OpCode::BIT_OR:
				return L"BIT_OR";
			case OpCode::BIT_XOR:
				return L"BIT_XOR";
			case OpCode::MINUS:
				return L"MINUS";
			case OpCode::BIT_NOT:
				return L"BIT_NOT";
			case OpCode::CAST:
				return L"CAST";
			case OpCode::UP_CAST:
				return L"UP_CAST";
			case OpCode::DOWN_CAST:
				return L"DOWN_CAST";
			case OpCode::EQ:
				return L"EQ";
			case OpCode::NE:
				return L"NE";
			case OpCode::GT:
				return L"GT";
			case OpCode::LT:
				return L"LT";
			case OpCode::GE:
				return L"GE";
			case OpCode::LE:
				return L"LE";
			case OpCode::LOGICAL_AND:
				return L"LOGICAL_AND";
			case OpCode::LOGICAL_OR:
				return L"LOGICAL_OR";
			case OpCode::LOGICAL_NOT:
				return L"LOGICAL_NOT";
			case OpCode::POP:
				return L"POP";
			case OpCode::DUPLICATE:
				return L"DUPLICATE";
			case OpCode::DUPLICATE_OFFSET:
				return L"DUPLICATE_OFFSET";
			case OpCode::JUMP:
				return L"JUMP";
			case OpCode::JUMP_IF_TRUE:
				return L"JUMP_IF_TRUE";
			case OpCode::JUMP_IF_FALSE:
				return L"JUMP_IF_FALSE";
			case OpCode::PUSH_MODULE:
				return L"PUSH_MODULE";
			case OpCode::PUSH_FUNCTION:
				return L"PUSH_FUNCTION";
			case OpCode::INOVKE:
				return L"INOVKE";
			case OpCode::RETURN:
				return L"RETURN";
			case OpCode::NEW:
				return L"NEW";
			case OpCode::SUPER:
				return L"SUPER";
			case OpCode::INSTANCE_OF:
				return L"INSTANCE_OF";
			case OpCode::THROW:
				return L"THROW";
			case OpCode::RETHROW:
				return L"RETHROW";
			case OpCode::GO_FINALLY:
				return L"GO_FINALLY";
			case OpCode::FINALLY_END:
				return L"FINALLY_END";
			default:
				std::cout << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};

	template <> class Enum<TypeTag> {
	public:
		static std::u32string ToString(TypeTag tag) {
			switch (tag) {
			case TypeTag::TYPE_I32:
				return L"TYPE_I32";
			case TypeTag::TYPE_I64:
				return L"TYPE_I64";
			case TypeTag::TYPE_F32:
				return L"TYPE_F32";
			case TypeTag::TYPE_F64:
				return L"TYPE_F64";
			case TypeTag::TYPE_STRING:
				return L"TYPE_STRING";
			case TypeTag::TYPE_OBJECT:
				return L"TYPE_OBJECT";
			default:
				std::cout << __FUNCTION__ << std::endl;
				exit(1);
			}
		}
	};
} // namespace cygni

#endif // CYGNI_ENUM_HPP
