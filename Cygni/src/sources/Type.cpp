#include "Type.hpp"

namespace cygni {
Type::Type(TypeCode typeCode) : typeCode{typeCode} {
}

std::u32string Type::ToString() const {
	return Enum<TypeCode>::ToString(typeCode);
}

bool Type::Equals(TypePtr other) const {
	return typeCode == other->typeCode;
}

std::shared_ptr<Int32Type> Type::Int32() {
	static std::shared_ptr<Int32Type> type = std::make_shared<Int32Type>();
	return type;
}
std::shared_ptr<Int64Type> Type::Int64() {
	static std::shared_ptr<Int64Type> type = std::make_shared<Int64Type>();
	return type;
}
std::shared_ptr<Float32Type> Type::Float32() {
	static std::shared_ptr<Float32Type> type = std::make_shared<Float32Type>();
	return type;
}
std::shared_ptr<Float64Type> Type::Float64() {
	static std::shared_ptr<Float64Type> type = std::make_shared<Float64Type>();
	return type;
}
std::shared_ptr<VoidType> Type::Void() {
	static std::shared_ptr<VoidType> type = std::make_shared<VoidType>();
	return type;
}
std::shared_ptr<BooleanType> Type::Boolean() {
	static std::shared_ptr<BooleanType> type = std::make_shared<BooleanType>();
	return type;
}
UnknownType::UnknownType() : Type(TypeCode::Unknown) {
}

Int32Type::Int32Type() : Type(TypeCode::Int32) {
}

Int64Type::Int64Type() : Type(TypeCode::Int64) {
}

Float32Type::Float32Type() : Type(TypeCode::Float32) {
}
Float64Type::Float64Type() : Type(TypeCode::Float64) {
}

CharType::CharType() : Type(TypeCode::Char) {
}

BooleanType::BooleanType() : Type(TypeCode::Boolean) {
}

StringType::StringType() : Type(TypeCode::String) {
}

VoidType::VoidType() : Type(TypeCode::Void) {
}

ObjectType::ObjectType(std::u32string name)
	: Type(TypeCode::Object), name{name} {
}

std::u32string ObjectType::ToString() const {
	return name;
}

bool ObjectType::Equals(TypePtr other) const {
	if (typeCode == other->typeCode) {
		std::shared_ptr<ObjectType> objType =
			std::static_pointer_cast<ObjectType>(other);
		return name == objType->name;
	} else {
		return false;
	}
}
ArrayType::ArrayType(TypePtr elementType)
	: Type(TypeCode::Array), elementType{elementType} {
}

bool ArrayType::Equals(TypePtr other) const {
	if (typeCode == other->typeCode) {
		std::shared_ptr<ArrayType> arrayType =
			std::static_pointer_cast<ArrayType>(other);
		return elementType->Equals(arrayType->elementType);
	} else {
		return false;
	}
}

std::u32string ArrayType::ToString() const {
	return U"Array[" + elementType->ToString() + U"]";
}

} // namespace cygni