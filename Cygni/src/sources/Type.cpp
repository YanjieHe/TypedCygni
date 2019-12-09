#include "Type.hpp"

namespace cygni {
Type::Type(TypeCode typeCode) : typeCode{typeCode} {
}

std::u32string Type::ToString() const {
	return Enum<TypeCode>::ToString(typeCode);
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

std::u32string ObjectType::ToString() const {
	return name;
}

ArrayType::ArrayType(TypePtr elementType)
	: Type(TypeCode::Array), elementType{elementType} {
}

std::u32string ArrayType::ToString() const {
	return U"Array[" + elementType->ToString() +  U"]";
}

} // namespace cygni