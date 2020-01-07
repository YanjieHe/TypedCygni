#ifndef CYGNI_TYPE_HPP
#define CYGNI_TYPE_HPP
#include "Enum.hpp"
#include "Utility.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cygni {
class Type;
using TypePtr = std::shared_ptr<Type>;

class UnknownType;
class Int32Type;
class Int64Type;
class Float32Type;
class Float64Type;
class CharType;
class BooleanType;
class StringType;
class VoidType;
class ObjectType;
class UnionType;

class Type {
public:
	TypeCode typeCode;

	explicit Type(TypeCode typeCode);

	virtual std::u32string ToString() const;
	virtual bool Equals(TypePtr other) const;

	static std::shared_ptr<Int32Type> Int32();
	static std::shared_ptr<Int64Type> Int64();
	static std::shared_ptr<Float32Type> Float32();
	static std::shared_ptr<Float64Type> Float64();
	static std::shared_ptr<VoidType> Void();
	static std::shared_ptr<BooleanType> Boolean();
	static std::shared_ptr<UnknownType> Unknown();

	static std::shared_ptr<Type> Unify(const std::vector<TypePtr> types);
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

class Float32Type : public Type {
public:
	Float32Type();
};

class Float64Type : public Type {
public:
	Float64Type();
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

class VoidType : public Type {
public:
	VoidType();
};

class ObjectType : public Type {
public:
	std::u32string name;
	explicit ObjectType(std::u32string name);

	std::u32string ToString() const override;

	bool Equals(TypePtr other) const override;
};

class ArrayType : public Type {
public:
	TypePtr elementType;
	explicit ArrayType(TypePtr elementType);

	std::u32string ToString() const override;

	bool Equals(TypePtr other) const override;
};

class FunctionType : public Type {
public:
	std::vector<TypePtr> parameters;
	TypePtr returnType;

	FunctionType(std::vector<TypePtr> parameters, TypePtr returnType);

	bool Match(const std::vector<TypePtr>& args) const;
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

template <> struct std::hash<cygni::TypePtr> {
public:
	size_t operator()(const cygni::TypePtr& type) const {
		if (type->typeCode == cygni::TypeCode::Object) {
			return static_cast<size_t>(type->typeCode);
		} else {
			return static_cast<size_t>(type->typeCode);
		}
	}
};

template <> struct std::equal_to<cygni::TypePtr> {
public:
	bool operator()(const cygni::TypePtr& x, const cygni::TypePtr& y) const {
		return x->Equals(y);
	}
};

namespace cygni {

class UnionType : public Type {
public:
	std::unordered_set<TypePtr, std::hash<TypePtr>, std::equal_to<TypePtr>>
		types;
	UnionType();
};
} // namespace cygni
#endif // CYGNI_TYPE_HPP