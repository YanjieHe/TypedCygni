#ifndef ENUM_HPP
#define ENUM_HPP
#include <string>

enum class Tag {
  Identifier = 0,
  Integer = 1,
  Float = 2,
  Character = 3,
  String = 4,
  Add = 5,
  Subtract = 6,
  Multiply = 7,
  Divide = 8,
  Modulo = 9,
  Equal = 10,
  NotEqual = 11,
  GreaterThan = 12,
  GreaterThanOrEqual = 13,
  LessThan = 14,
  LessThanOrEqual = 15,
  And = 16,
  Or = 17,
  Not = 18,
  Var = 19,
  Def = 20,
  Assign = 21,
  LeftBrace = 22,
  RightBrace = 23,
  LeftBracket = 24,
  RightBracket = 25,
  LeftParenthesis = 26,
  RightParenthesis = 27,
  Dot = 28,
  Comma = 29,
  Semicolon = 30,
  Colon = 31,
  RightArrow = 32,
  At = 33,
  True = 34,
  False = 35,
  Void = 36,
  If = 37,
  Else = 38,
  While = 39,
  For = 40,
  Break = 41,
  Return = 42,
  Import = 43,
  Uses = 44,
  Require = 45,
  Rename = 46,
  GoesTo = 47,
  LowerBound = 48,
  UpperBound = 49,
  Class = 50,
  New = 51,
  Private = 52,
  Protected = 53,
  Module = 54,
  Package = 55,
  Extends = 56,
  Interface = 57,
  To = 58,
  Eof = 59
};

enum class ExpressionType {
  INT = 0,
  LONG = 1,
  FLOAT = 2,
  DOUBLE = 3,
  BOOLEAN = 4,
  CHAR = 5,
  STRING = 6,
  ADD = 7,
  SUBTRACT = 8,
  MULTIPLY = 9,
  DIVIDE = 10,
  MODULO = 11,
  GT = 12,
  LT = 13,
  GE = 14,
  LE = 15,
  EQ = 16,
  NE = 17,
  LOGICAL_AND = 18,
  LOGICAL_OR = 19,
  LOGICAL_NOT = 20,
  PLUS = 21,
  MINUS = 22,
  INVOKE = 23,
  IDENTIFIER = 24,
  CONVERT = 25,
  MEMBER = 26,
  NEW = 27
};

enum class StatementType {
  EXPRESSION = 0,
  IF_THEN = 1,
  IF_ELSE = 2,
  WHILE = 3,
  BLOCK = 4,
  RETURN = 5,
  BREAK = 6,
  ASSIGN = 7,
  VAR_DECL = 8
};

enum class AssignmentKind {
  IDENTIFIER_ASSIGN = 0,
  CALL_ASSIGN = 1,
  MEMBER_ASSIGN = 2
};

enum class MemberKind {
  FIELD = 0,
  METHOD = 1,
  STATIC_VAR = 2,
  STATIC_FUNC = 3
};

enum class AccessModifier { PUBLIC = 0, PRIVATE = 1, PROTECTED = 2 };

enum class TypeCode {
  VOID = 0,
  BOOLEAN = 1,
  INT = 2,
  LONG = 3,
  FLOAT = 4,
  DOUBLE = 5,
  CHAR = 6,
  STRING = 7,
  FUNCTION = 8,
  METHOD = 9,
  ARRAY = 10,
  OBJECT = 11
};

enum class OpCode {
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

enum class TypeTag {
  TYPE_I32 = 0,
  TYPE_I64 = 1,
  TYPE_F32 = 2,
  TYPE_F64 = 3,
  TYPE_STRING = 4,
  TYPE_OBJECT = 5
};

enum class ConstantKind {
  CONSTANT_FLAG_I32 = 0,
  CONSTANT_FLAG_I64 = 1,
  CONSTANT_FLAG_F32 = 2,
  CONSTANT_FLAG_F64 = 3,
  CONSTANT_FLAG_BOOLEAN = 4,
  CONSTANT_FLAG_CHAR = 5,
  CONSTANT_FLAG_STRING = 6,
  CONSTANT_FLAG_METHOD = 7,
  CONSTANT_FLAG_CLASS = 8,
  CONSTANT_FLAG_STATIC_VAR = 9,
  CONSTANT_FLAG_STATIC_FUNCTION = 10
};

enum class MethodFlag {
  NativeFunction = 0,
  InstanceMethod = 1,
  ModuleFunction = 2
};

template <typename TEnum> class Enum {
public:
  static std::string ToString(TEnum) {
    return "ENUMERATION VALUE OUT OF BOUND";
  }
};

template <> class Enum<Tag> {
public:
  static std::string ToString(Tag value) {
    switch (value) {
    case Tag::Identifier:
      return "Tag::Identifier";
    case Tag::Integer:
      return "Tag::Integer";
    case Tag::Float:
      return "Tag::Float";
    case Tag::Character:
      return "Tag::Character";
    case Tag::String:
      return "Tag::String";
    case Tag::Add:
      return "Tag::Add";
    case Tag::Subtract:
      return "Tag::Subtract";
    case Tag::Multiply:
      return "Tag::Multiply";
    case Tag::Divide:
      return "Tag::Divide";
    case Tag::Modulo:
      return "Tag::Modulo";
    case Tag::Equal:
      return "Tag::Equal";
    case Tag::NotEqual:
      return "Tag::NotEqual";
    case Tag::GreaterThan:
      return "Tag::GreaterThan";
    case Tag::GreaterThanOrEqual:
      return "Tag::GreaterThanOrEqual";
    case Tag::LessThan:
      return "Tag::LessThan";
    case Tag::LessThanOrEqual:
      return "Tag::LessThanOrEqual";
    case Tag::And:
      return "Tag::And";
    case Tag::Or:
      return "Tag::Or";
    case Tag::Not:
      return "Tag::Not";
    case Tag::Var:
      return "Tag::Var";
    case Tag::Def:
      return "Tag::Def";
    case Tag::Assign:
      return "Tag::Assign";
    case Tag::LeftBrace:
      return "Tag::LeftBrace";
    case Tag::RightBrace:
      return "Tag::RightBrace";
    case Tag::LeftBracket:
      return "Tag::LeftBracket";
    case Tag::RightBracket:
      return "Tag::RightBracket";
    case Tag::LeftParenthesis:
      return "Tag::LeftParenthesis";
    case Tag::RightParenthesis:
      return "Tag::RightParenthesis";
    case Tag::Dot:
      return "Tag::Dot";
    case Tag::Comma:
      return "Tag::Comma";
    case Tag::Semicolon:
      return "Tag::Semicolon";
    case Tag::Colon:
      return "Tag::Colon";
    case Tag::RightArrow:
      return "Tag::RightArrow";
    case Tag::At:
      return "Tag::At";
    case Tag::True:
      return "Tag::True";
    case Tag::False:
      return "Tag::False";
    case Tag::Void:
      return "Tag::Void";
    case Tag::If:
      return "Tag::If";
    case Tag::Else:
      return "Tag::Else";
    case Tag::While:
      return "Tag::While";
    case Tag::For:
      return "Tag::For";
    case Tag::Break:
      return "Tag::Break";
    case Tag::Return:
      return "Tag::Return";
    case Tag::Import:
      return "Tag::Import";
    case Tag::Uses:
      return "Tag::Uses";
    case Tag::Require:
      return "Tag::Require";
    case Tag::Rename:
      return "Tag::Rename";
    case Tag::GoesTo:
      return "Tag::GoesTo";
    case Tag::LowerBound:
      return "Tag::LowerBound";
    case Tag::UpperBound:
      return "Tag::UpperBound";
    case Tag::Class:
      return "Tag::Class";
    case Tag::New:
      return "Tag::New";
    case Tag::Private:
      return "Tag::Private";
    case Tag::Protected:
      return "Tag::Protected";
    case Tag::Module:
      return "Tag::Module";
    case Tag::Package:
      return "Tag::Package";
    case Tag::Extends:
      return "Tag::Extends";
    case Tag::Interface:
      return "Tag::Interface";
    case Tag::To:
      return "Tag::To";
    case Tag::Eof:
      return "Tag::Eof";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<ExpressionType> {
public:
  static std::string ToString(ExpressionType value) {
    switch (value) {
    case ExpressionType::INT:
      return "ExpressionType::INT";
    case ExpressionType::LONG:
      return "ExpressionType::LONG";
    case ExpressionType::FLOAT:
      return "ExpressionType::FLOAT";
    case ExpressionType::DOUBLE:
      return "ExpressionType::DOUBLE";
    case ExpressionType::BOOLEAN:
      return "ExpressionType::BOOLEAN";
    case ExpressionType::CHAR:
      return "ExpressionType::CHAR";
    case ExpressionType::STRING:
      return "ExpressionType::STRING";
    case ExpressionType::ADD:
      return "ExpressionType::ADD";
    case ExpressionType::SUBTRACT:
      return "ExpressionType::SUBTRACT";
    case ExpressionType::MULTIPLY:
      return "ExpressionType::MULTIPLY";
    case ExpressionType::DIVIDE:
      return "ExpressionType::DIVIDE";
    case ExpressionType::MODULO:
      return "ExpressionType::MODULO";
    case ExpressionType::GT:
      return "ExpressionType::GT";
    case ExpressionType::LT:
      return "ExpressionType::LT";
    case ExpressionType::GE:
      return "ExpressionType::GE";
    case ExpressionType::LE:
      return "ExpressionType::LE";
    case ExpressionType::EQ:
      return "ExpressionType::EQ";
    case ExpressionType::NE:
      return "ExpressionType::NE";
    case ExpressionType::LOGICAL_AND:
      return "ExpressionType::LOGICAL_AND";
    case ExpressionType::LOGICAL_OR:
      return "ExpressionType::LOGICAL_OR";
    case ExpressionType::LOGICAL_NOT:
      return "ExpressionType::LOGICAL_NOT";
    case ExpressionType::PLUS:
      return "ExpressionType::PLUS";
    case ExpressionType::MINUS:
      return "ExpressionType::MINUS";
    case ExpressionType::INVOKE:
      return "ExpressionType::INVOKE";
    case ExpressionType::IDENTIFIER:
      return "ExpressionType::IDENTIFIER";
    case ExpressionType::CONVERT:
      return "ExpressionType::CONVERT";
    case ExpressionType::MEMBER:
      return "ExpressionType::MEMBER";
    case ExpressionType::NEW:
      return "ExpressionType::NEW";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<StatementType> {
public:
  static std::string ToString(StatementType value) {
    switch (value) {
    case StatementType::EXPRESSION:
      return "StatementType::EXPRESSION";
    case StatementType::IF_THEN:
      return "StatementType::IF_THEN";
    case StatementType::IF_ELSE:
      return "StatementType::IF_ELSE";
    case StatementType::WHILE:
      return "StatementType::WHILE";
    case StatementType::BLOCK:
      return "StatementType::BLOCK";
    case StatementType::RETURN:
      return "StatementType::RETURN";
    case StatementType::BREAK:
      return "StatementType::BREAK";
    case StatementType::ASSIGN:
      return "StatementType::ASSIGN";
    case StatementType::VAR_DECL:
      return "StatementType::VAR_DECL";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<AssignmentKind> {
public:
  static std::string ToString(AssignmentKind value) {
    switch (value) {
    case AssignmentKind::IDENTIFIER_ASSIGN:
      return "AssignmentKind::IDENTIFIER_ASSIGN";
    case AssignmentKind::CALL_ASSIGN:
      return "AssignmentKind::CALL_ASSIGN";
    case AssignmentKind::MEMBER_ASSIGN:
      return "AssignmentKind::MEMBER_ASSIGN";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<MemberKind> {
public:
  static std::string ToString(MemberKind value) {
    switch (value) {
    case MemberKind::FIELD:
      return "MemberKind::FIELD";
    case MemberKind::METHOD:
      return "MemberKind::METHOD";
    case MemberKind::STATIC_VAR:
      return "MemberKind::STATIC_VAR";
    case MemberKind::STATIC_FUNC:
      return "MemberKind::STATIC_FUNC";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<AccessModifier> {
public:
  static std::string ToString(AccessModifier value) {
    switch (value) {
    case AccessModifier::PUBLIC:
      return "AccessModifier::PUBLIC";
    case AccessModifier::PRIVATE:
      return "AccessModifier::PRIVATE";
    case AccessModifier::PROTECTED:
      return "AccessModifier::PROTECTED";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<TypeCode> {
public:
  static std::string ToString(TypeCode value) {
    switch (value) {
    case TypeCode::VOID:
      return "TypeCode::VOID";
    case TypeCode::BOOLEAN:
      return "TypeCode::BOOLEAN";
    case TypeCode::INT:
      return "TypeCode::INT";
    case TypeCode::LONG:
      return "TypeCode::LONG";
    case TypeCode::FLOAT:
      return "TypeCode::FLOAT";
    case TypeCode::DOUBLE:
      return "TypeCode::DOUBLE";
    case TypeCode::CHAR:
      return "TypeCode::CHAR";
    case TypeCode::STRING:
      return "TypeCode::STRING";
    case TypeCode::FUNCTION:
      return "TypeCode::FUNCTION";
    case TypeCode::METHOD:
      return "TypeCode::METHOD";
    case TypeCode::ARRAY:
      return "TypeCode::ARRAY";
    case TypeCode::OBJECT:
      return "TypeCode::OBJECT";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<OpCode> {
public:
  static std::string ToString(OpCode value) {
    switch (value) {
    case OpCode::PUSH_I32_0:
      return "OpCode::PUSH_I32_0";
    case OpCode::PUSH_I32_1:
      return "OpCode::PUSH_I32_1";
    case OpCode::PUSH_I32_1BYTE:
      return "OpCode::PUSH_I32_1BYTE";
    case OpCode::PUSH_I32_2BYTE:
      return "OpCode::PUSH_I32_2BYTE";
    case OpCode::PUSH_I64_0:
      return "OpCode::PUSH_I64_0";
    case OpCode::PUSH_I64_1:
      return "OpCode::PUSH_I64_1";
    case OpCode::PUSH_F64_0:
      return "OpCode::PUSH_F64_0";
    case OpCode::PUSH_F64_1:
      return "OpCode::PUSH_F64_1";
    case OpCode::PUSH_I32:
      return "OpCode::PUSH_I32";
    case OpCode::PUSH_I64:
      return "OpCode::PUSH_I64";
    case OpCode::PUSH_F32:
      return "OpCode::PUSH_F32";
    case OpCode::PUSH_F64:
      return "OpCode::PUSH_F64";
    case OpCode::PUSH_STRING:
      return "OpCode::PUSH_STRING";
    case OpCode::PUSH_NULL:
      return "OpCode::PUSH_NULL";
    case OpCode::PUSH_LOCAL_I32:
      return "OpCode::PUSH_LOCAL_I32";
    case OpCode::PUSH_LOCAL_I64:
      return "OpCode::PUSH_LOCAL_I64";
    case OpCode::PUSH_LOCAL_F32:
      return "OpCode::PUSH_LOCAL_F32";
    case OpCode::PUSH_LOCAL_F64:
      return "OpCode::PUSH_LOCAL_F64";
    case OpCode::PUSH_LOCAL_OBJECT:
      return "OpCode::PUSH_LOCAL_OBJECT";
    case OpCode::POP_LOCAL_I32:
      return "OpCode::POP_LOCAL_I32";
    case OpCode::POP_LOCAL_I64:
      return "OpCode::POP_LOCAL_I64";
    case OpCode::POP_LOCAL_F32:
      return "OpCode::POP_LOCAL_F32";
    case OpCode::POP_LOCAL_F64:
      return "OpCode::POP_LOCAL_F64";
    case OpCode::POP_LOCAL_OBJECT:
      return "OpCode::POP_LOCAL_OBJECT";
    case OpCode::PUSH_STATIC_I32:
      return "OpCode::PUSH_STATIC_I32";
    case OpCode::PUSH_STATIC_I64:
      return "OpCode::PUSH_STATIC_I64";
    case OpCode::PUSH_STATIC_F32:
      return "OpCode::PUSH_STATIC_F32";
    case OpCode::PUSH_STATIC_F64:
      return "OpCode::PUSH_STATIC_F64";
    case OpCode::PUSH_STATIC_OBJECT:
      return "OpCode::PUSH_STATIC_OBJECT";
    case OpCode::POP_STATIC_I32:
      return "OpCode::POP_STATIC_I32";
    case OpCode::POP_STATIC_I64:
      return "OpCode::POP_STATIC_I64";
    case OpCode::POP_STATIC_F32:
      return "OpCode::POP_STATIC_F32";
    case OpCode::POP_STATIC_F64:
      return "OpCode::POP_STATIC_F64";
    case OpCode::POP_STATIC_OBJECT:
      return "OpCode::POP_STATIC_OBJECT";
    case OpCode::PUSH_ARRAY_I32:
      return "OpCode::PUSH_ARRAY_I32";
    case OpCode::PUSH_ARRAY_I64:
      return "OpCode::PUSH_ARRAY_I64";
    case OpCode::PUSH_ARRAY_F32:
      return "OpCode::PUSH_ARRAY_F32";
    case OpCode::PUSH_ARRAY_F64:
      return "OpCode::PUSH_ARRAY_F64";
    case OpCode::PUSH_ARRAY_OBJECT:
      return "OpCode::PUSH_ARRAY_OBJECT";
    case OpCode::POP_ARRAY_I32:
      return "OpCode::POP_ARRAY_I32";
    case OpCode::POP_ARRAY_I64:
      return "OpCode::POP_ARRAY_I64";
    case OpCode::POP_ARRAY_F32:
      return "OpCode::POP_ARRAY_F32";
    case OpCode::POP_ARRAY_F64:
      return "OpCode::POP_ARRAY_F64";
    case OpCode::POP_ARRAY_OBJECT:
      return "OpCode::POP_ARRAY_OBJECT";
    case OpCode::PUSH_FIELD_I32:
      return "OpCode::PUSH_FIELD_I32";
    case OpCode::PUSH_FIELD_I64:
      return "OpCode::PUSH_FIELD_I64";
    case OpCode::PUSH_FIELD_F32:
      return "OpCode::PUSH_FIELD_F32";
    case OpCode::PUSH_FIELD_F64:
      return "OpCode::PUSH_FIELD_F64";
    case OpCode::PUSH_FIELD_OBJECT:
      return "OpCode::PUSH_FIELD_OBJECT";
    case OpCode::POP_FIELD_I32:
      return "OpCode::POP_FIELD_I32";
    case OpCode::POP_FIELD_I64:
      return "OpCode::POP_FIELD_I64";
    case OpCode::POP_FIELD_F32:
      return "OpCode::POP_FIELD_F32";
    case OpCode::POP_FIELD_F64:
      return "OpCode::POP_FIELD_F64";
    case OpCode::POP_FIELD_OBJECT:
      return "OpCode::POP_FIELD_OBJECT";
    case OpCode::ADD_I32:
      return "OpCode::ADD_I32";
    case OpCode::ADD_I64:
      return "OpCode::ADD_I64";
    case OpCode::ADD_F32:
      return "OpCode::ADD_F32";
    case OpCode::ADD_F64:
      return "OpCode::ADD_F64";
    case OpCode::SUB_I32:
      return "OpCode::SUB_I32";
    case OpCode::SUB_I64:
      return "OpCode::SUB_I64";
    case OpCode::SUB_F32:
      return "OpCode::SUB_F32";
    case OpCode::SUB_F64:
      return "OpCode::SUB_F64";
    case OpCode::MUL_I32:
      return "OpCode::MUL_I32";
    case OpCode::MUL_I64:
      return "OpCode::MUL_I64";
    case OpCode::MUL_F32:
      return "OpCode::MUL_F32";
    case OpCode::MUL_F64:
      return "OpCode::MUL_F64";
    case OpCode::DIV_I32:
      return "OpCode::DIV_I32";
    case OpCode::DIV_I64:
      return "OpCode::DIV_I64";
    case OpCode::DIV_F32:
      return "OpCode::DIV_F32";
    case OpCode::DIV_F64:
      return "OpCode::DIV_F64";
    case OpCode::MOD_I32:
      return "OpCode::MOD_I32";
    case OpCode::MOD_I64:
      return "OpCode::MOD_I64";
    case OpCode::MOD_F32:
      return "OpCode::MOD_F32";
    case OpCode::MOD_F64:
      return "OpCode::MOD_F64";
    case OpCode::BIT_AND_I32:
      return "OpCode::BIT_AND_I32";
    case OpCode::BIT_AND_I64:
      return "OpCode::BIT_AND_I64";
    case OpCode::BIT_OR_I32:
      return "OpCode::BIT_OR_I32";
    case OpCode::BIT_OR_I64:
      return "OpCode::BIT_OR_I64";
    case OpCode::BIT_XOR_I32:
      return "OpCode::BIT_XOR_I32";
    case OpCode::BIT_XOR_I64:
      return "OpCode::BIT_XOR_I64";
    case OpCode::MINUS_I32:
      return "OpCode::MINUS_I32";
    case OpCode::MINUS_I64:
      return "OpCode::MINUS_I64";
    case OpCode::MINUS_F32:
      return "OpCode::MINUS_F32";
    case OpCode::MINUS_F64:
      return "OpCode::MINUS_F64";
    case OpCode::BIT_NOT_I32:
      return "OpCode::BIT_NOT_I32";
    case OpCode::BIT_NOT_I64:
      return "OpCode::BIT_NOT_I64";
    case OpCode::CAST_I32_TO_I64:
      return "OpCode::CAST_I32_TO_I64";
    case OpCode::CAST_I32_TO_F32:
      return "OpCode::CAST_I32_TO_F32";
    case OpCode::CAST_I32_TO_F64:
      return "OpCode::CAST_I32_TO_F64";
    case OpCode::CAST_I64_TO_I32:
      return "OpCode::CAST_I64_TO_I32";
    case OpCode::CAST_I64_TO_F32:
      return "OpCode::CAST_I64_TO_F32";
    case OpCode::CAST_I64_TO_F64:
      return "OpCode::CAST_I64_TO_F64";
    case OpCode::CAST_F32_TO_I32:
      return "OpCode::CAST_F32_TO_I32";
    case OpCode::CAST_F32_TO_I64:
      return "OpCode::CAST_F32_TO_I64";
    case OpCode::CAST_F32_TO_F64:
      return "OpCode::CAST_F32_TO_F64";
    case OpCode::CAST_F64_TO_I32:
      return "OpCode::CAST_F64_TO_I32";
    case OpCode::CAST_F64_TO_I64:
      return "OpCode::CAST_F64_TO_I64";
    case OpCode::CAST_F64_TO_F32:
      return "OpCode::CAST_F64_TO_F32";
    case OpCode::UP_CAST:
      return "OpCode::UP_CAST";
    case OpCode::DOWN_CAST:
      return "OpCode::DOWN_CAST";
    case OpCode::EQ_I32:
      return "OpCode::EQ_I32";
    case OpCode::EQ_I64:
      return "OpCode::EQ_I64";
    case OpCode::EQ_F32:
      return "OpCode::EQ_F32";
    case OpCode::EQ_F64:
      return "OpCode::EQ_F64";
    case OpCode::NE_I32:
      return "OpCode::NE_I32";
    case OpCode::NE_I64:
      return "OpCode::NE_I64";
    case OpCode::NE_F32:
      return "OpCode::NE_F32";
    case OpCode::NE_F64:
      return "OpCode::NE_F64";
    case OpCode::GT_I32:
      return "OpCode::GT_I32";
    case OpCode::GT_I64:
      return "OpCode::GT_I64";
    case OpCode::GT_F32:
      return "OpCode::GT_F32";
    case OpCode::GT_F64:
      return "OpCode::GT_F64";
    case OpCode::LT_I32:
      return "OpCode::LT_I32";
    case OpCode::LT_I64:
      return "OpCode::LT_I64";
    case OpCode::LT_F32:
      return "OpCode::LT_F32";
    case OpCode::LT_F64:
      return "OpCode::LT_F64";
    case OpCode::GE_I32:
      return "OpCode::GE_I32";
    case OpCode::GE_I64:
      return "OpCode::GE_I64";
    case OpCode::GE_F32:
      return "OpCode::GE_F32";
    case OpCode::GE_F64:
      return "OpCode::GE_F64";
    case OpCode::LE_I32:
      return "OpCode::LE_I32";
    case OpCode::LE_I64:
      return "OpCode::LE_I64";
    case OpCode::LE_F32:
      return "OpCode::LE_F32";
    case OpCode::LE_F64:
      return "OpCode::LE_F64";
    case OpCode::POP:
      return "OpCode::POP";
    case OpCode::DUPLICATE:
      return "OpCode::DUPLICATE";
    case OpCode::DUPLICATE_OFFSET:
      return "OpCode::DUPLICATE_OFFSET";
    case OpCode::JUMP:
      return "OpCode::JUMP";
    case OpCode::JUMP_IF_TRUE:
      return "OpCode::JUMP_IF_TRUE";
    case OpCode::JUMP_IF_FALSE:
      return "OpCode::JUMP_IF_FALSE";
    case OpCode::PUSH_FUNCTION:
      return "OpCode::PUSH_FUNCTION";
    case OpCode::PUSH_METHOD:
      return "OpCode::PUSH_METHOD";
    case OpCode::INVOKE:
      return "OpCode::INVOKE";
    case OpCode::RETURN_I32:
      return "OpCode::RETURN_I32";
    case OpCode::RETURN_I64:
      return "OpCode::RETURN_I64";
    case OpCode::RETURN_F32:
      return "OpCode::RETURN_F32";
    case OpCode::RETURN_F64:
      return "OpCode::RETURN_F64";
    case OpCode::RETURN_OBJECT:
      return "OpCode::RETURN_OBJECT";
    case OpCode::NEW:
      return "OpCode::NEW";
    case OpCode::NEW_ARRAY:
      return "OpCode::NEW_ARRAY";
    case OpCode::NEW_ARRAY_LITERAL_I32:
      return "OpCode::NEW_ARRAY_LITERAL_I32";
    case OpCode::NEW_ARRAY_LITERAL_I64:
      return "OpCode::NEW_ARRAY_LITERAL_I64";
    case OpCode::NEW_ARRAY_LITERAL_F32:
      return "OpCode::NEW_ARRAY_LITERAL_F32";
    case OpCode::NEW_ARRAY_LITERAL_F64:
      return "OpCode::NEW_ARRAY_LITERAL_F64";
    case OpCode::NEW_ARRAY_LITERAL_OBJECT:
      return "OpCode::NEW_ARRAY_LITERAL_OBJECT";
    case OpCode::ARRAY_LENGTH:
      return "OpCode::ARRAY_LENGTH";
    case OpCode::SUPER:
      return "OpCode::SUPER";
    case OpCode::INSTANCE_OF:
      return "OpCode::INSTANCE_OF";
    case OpCode::THROW:
      return "OpCode::THROW";
    case OpCode::RETHROW:
      return "OpCode::RETHROW";
    case OpCode::GO_FINALLY:
      return "OpCode::GO_FINALLY";
    case OpCode::FINALLY_END:
      return "OpCode::FINALLY_END";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<TypeTag> {
public:
  static std::string ToString(TypeTag value) {
    switch (value) {
    case TypeTag::TYPE_I32:
      return "TypeTag::TYPE_I32";
    case TypeTag::TYPE_I64:
      return "TypeTag::TYPE_I64";
    case TypeTag::TYPE_F32:
      return "TypeTag::TYPE_F32";
    case TypeTag::TYPE_F64:
      return "TypeTag::TYPE_F64";
    case TypeTag::TYPE_STRING:
      return "TypeTag::TYPE_STRING";
    case TypeTag::TYPE_OBJECT:
      return "TypeTag::TYPE_OBJECT";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<ConstantKind> {
public:
  static std::string ToString(ConstantKind value) {
    switch (value) {
    case ConstantKind::CONSTANT_FLAG_I32:
      return "ConstantKind::CONSTANT_FLAG_I32";
    case ConstantKind::CONSTANT_FLAG_I64:
      return "ConstantKind::CONSTANT_FLAG_I64";
    case ConstantKind::CONSTANT_FLAG_F32:
      return "ConstantKind::CONSTANT_FLAG_F32";
    case ConstantKind::CONSTANT_FLAG_F64:
      return "ConstantKind::CONSTANT_FLAG_F64";
    case ConstantKind::CONSTANT_FLAG_BOOLEAN:
      return "ConstantKind::CONSTANT_FLAG_BOOLEAN";
    case ConstantKind::CONSTANT_FLAG_CHAR:
      return "ConstantKind::CONSTANT_FLAG_CHAR";
    case ConstantKind::CONSTANT_FLAG_STRING:
      return "ConstantKind::CONSTANT_FLAG_STRING";
    case ConstantKind::CONSTANT_FLAG_METHOD:
      return "ConstantKind::CONSTANT_FLAG_METHOD";
    case ConstantKind::CONSTANT_FLAG_CLASS:
      return "ConstantKind::CONSTANT_FLAG_CLASS";
    case ConstantKind::CONSTANT_FLAG_STATIC_VAR:
      return "ConstantKind::CONSTANT_FLAG_STATIC_VAR";
    case ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION:
      return "ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

template <> class Enum<MethodFlag> {
public:
  static std::string ToString(MethodFlag value) {
    switch (value) {
    case MethodFlag::NativeFunction:
      return "MethodFlag::NativeFunction";
    case MethodFlag::InstanceMethod:
      return "MethodFlag::InstanceMethod";
    case MethodFlag::ModuleFunction:
      return "MethodFlag::ModuleFunction";
    default:
      return "ENUMERATION VALUE OUT OF BOUND";
    }
  }
};

#endif // ENUM_HPP
