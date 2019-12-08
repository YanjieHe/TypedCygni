#include "Type.hpp"

namespace cygni {
Type::Type(TypeCode typeCode) : typeCode{typeCode} {
}

UnknownType::UnknownType() : Type(TypeCode::Unknown) {
}

Int32Type::Int32Type() : Type(TypeCode::Int32) {
}

Int64Type::Int64Type() : Type(TypeCode::Int64) {
}

DoubleType::DoubleType() : Type(TypeCode::Double) {
}

CharType::CharType() : Type(TypeCode::Char) {
}

BooleanType::BooleanType() : Type(TypeCode::Boolean) {
}

StringType::StringType() : Type(TypeCode::String) {
}

ObjectType::ObjectType(std::u32string name)
	: Type(TypeCode::Object), name{name} {
}

ArrayType::ArrayType(TypePtr elementType)
	: Type(TypeCode::Array), elementType{elementType} {
}

} // namespace cygni