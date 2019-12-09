#ifndef CYGNI_TYPE_HPP
#define CYGNI_TYPE_HPP
#include "Enum.hpp"
#include "Utility.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
namespace cygni {
class Type;
using TypePtr = std::shared_ptr<Type>;

class Type {
public:
	TypeCode typeCode;

	explicit Type(TypeCode typeCode);

	virtual std::u32string ToString() const;
};

class UnknownType : public Type {
public:
	UnknownType();
};

class Int32Type : public Type {
public:
	Int32Type();
};

class Int64Type : public Type {
public:
	Int64Type();
};

class DoubleType : public Type {
public:
	DoubleType();
};

class CharType : public Type {
public:
	CharType();
};

class BooleanType : public Type {
public:
	BooleanType();
};

class StringType : public Type {
public:
	StringType();
};

class ObjectType : public Type {
public:
	std::u32string name;
	explicit ObjectType(std::u32string name);
	
	std::u32string ToString() const override;
};

class ArrayType : public Type {
public:
	TypePtr elementType;
	explicit ArrayType(TypePtr elementType);

	std::u32string ToString() const override;
};

class FieldTypeInfo {
public:
	std::u32string name;
	TypePtr type;
};

class MethodTypeInfo {
public:
	std::u32string name;
	std::vector<TypePtr> parameters;
	TypePtr returnType;
};

class ClassTypeInfo {
public:
	Table<std::u32string, FieldTypeInfo> fields;
	Table<std::u32string, MethodTypeInfo> methods;
	ClassTypeInfo() = default;
};

class TypeParameter {
public:
	std::u32string name;
};

class GenericType {
public:
	std::vector<TypeParameter> parameters;
};

} // namespace cygni
#endif // CYGNI_TYPE_HPP