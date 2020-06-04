#ifndef CYGNI_TYPE_HPP
#define CYGNI_TYPE_HPP
#include "Enum.hpp"
#include "Utility.hpp"
#include <unordered_map>
#include <unordered_set>
#include <list>
#include "SourcePosition.hpp"

namespace cygni
{
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
	class AnyType;

	class Type
	{
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
		static std::shared_ptr<CharType> Char();
		static std::shared_ptr<StringType> String();
		static std::shared_ptr<UnknownType> Unknown();
		static std::shared_ptr<AnyType> Any();

		static std::shared_ptr<Type> Unify(const std::vector<TypePtr> types);
	};

	class UnknownType : public Type
	{
	public:
		UnknownType();
	};

	class Int32Type : public Type
	{
	public:
		Int32Type();
	};

	class Int64Type : public Type
	{
	public:
		Int64Type();
	};

	class Float32Type : public Type
	{
	public:
		Float32Type();
	};

	class Float64Type : public Type
	{
	public:
		Float64Type();
	};

	class CharType : public Type
	{
	public:
		CharType();
	};

	class BooleanType : public Type
	{
	public:
		BooleanType();
	};

	class StringType : public Type
	{
	public:
		StringType();

		std::u32string ToString() const override;
	};

	class VoidType : public Type
	{
	public:
		VoidType();
	};

	class AnyType : public Type
	{
	public:
		AnyType();
	};

	class InterfaceType;

	class TypeParameter
	{
	public:
		std::u32string name;
		std::vector<std::shared_ptr<InterfaceType>> interfaces;
	};

	class GenericType
	{
	public:
		Table<std::u32string, TypeParameter> parameters;
		TypePtr type;
	};

	class ClassType : public Type
	{
	public:
		PackageRoute route;
		std::u32string name;

		ClassType(PackageRoute route, std::u32string name);
		std::u32string ToString() const override;
		bool Equals(TypePtr other) const override;
		TypePtr Replace(const Table<std::u32string, TypeParameter>& parameters) const;
	};

	class ModuleType : public Type
	{
	public:
		PackageRoute route;
		std::u32string name;

		ModuleType(PackageRoute route, std::u32string name);
		std::u32string ToString() const override;
		bool Equals(TypePtr other) const override;
	};

	class InterfaceType : public Type
	{
	public:
		PackageRoute route;
		std::u32string name;

		InterfaceType(PackageRoute route, std::u32string name);
		std::u32string ToString() const override;
		bool Equals(TypePtr other) const override;
	};

	class ArrayType : public Type
	{
	public:
		TypePtr elementType;

		explicit ArrayType(TypePtr elementType);
		std::u32string ToString() const override;
		bool Equals(TypePtr other) const override;
	};

	class FunctionType : public Type
	{
	public:
		TypePtr selfType;
		std::vector<TypePtr> parameters;
		TypePtr returnType;

		FunctionType(TypePtr selfType, std::vector<TypePtr> parameters, TypePtr returnType);

		bool Match(const std::vector<TypePtr> &args) const;

		std::u32string ToString() const override;
	};

} // namespace cygni

template <> struct std::hash<cygni::TypePtr>
{
public:
	std::hash<cygni::PackageRoute> h1;
	std::hash<std::u32string> h2;

	size_t operator()(const cygni::TypePtr &type) const
	{
		if (type->typeCode == cygni::TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<cygni::ClassType>(type);
			return h1(classType->route) ^ h2(classType->name);
		}
		else
		{
			return static_cast<size_t>(type->typeCode);
		}
	}
};

template <> struct std::equal_to<cygni::TypePtr>
{
public:
	bool operator()(const cygni::TypePtr &x, const cygni::TypePtr &y) const
	{
		return x->Equals(y);
	}
};

namespace cygni
{
	class UnionType : public Type
	{
	public:
		std::unordered_set<TypePtr, std::hash<TypePtr>, std::equal_to<TypePtr>> types;
		UnionType();
	};
} // namespace cygni
#endif // CYGNI_TYPE_HPP
