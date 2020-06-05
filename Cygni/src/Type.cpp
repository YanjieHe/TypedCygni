#include "Type.hpp"

namespace cygni
{
	Type::Type(TypeCode typeCode) : typeCode{ typeCode } {}

	std::u32string Type::ToString() const
	{
		return Enum<TypeCode>::ToString(typeCode);
	}

	bool Type::Equals(TypePtr other) const { return typeCode == other->typeCode; }

	std::shared_ptr<Int32Type> Type::Int32()
	{
		static std::shared_ptr<Int32Type> type = std::make_shared<Int32Type>();
		return type;
	}
	std::shared_ptr<Int64Type> Type::Int64()
	{
		static std::shared_ptr<Int64Type> type = std::make_shared<Int64Type>();
		return type;
	}
	std::shared_ptr<Float32Type> Type::Float32()
	{
		static std::shared_ptr<Float32Type> type = std::make_shared<Float32Type>();
		return type;
	}
	std::shared_ptr<Float64Type> Type::Float64()
	{
		static std::shared_ptr<Float64Type> type = std::make_shared<Float64Type>();
		return type;
	}
	std::shared_ptr<VoidType> Type::Void()
	{
		static std::shared_ptr<VoidType> type = std::make_shared<VoidType>();
		return type;
	}
	std::shared_ptr<BooleanType> Type::Boolean()
	{
		static std::shared_ptr<BooleanType> type = std::make_shared<BooleanType>();
		return type;
	}
	std::shared_ptr<CharType> Type::Char()
	{
		static std::shared_ptr<CharType> type = std::make_shared<CharType>();
		return type;
	}
	std::shared_ptr<StringType> Type::String()
	{
		static std::shared_ptr<StringType> type = std::make_shared<StringType>();
		return type;
	}
	std::shared_ptr<UnknownType> Type::Unknown()
	{
		static std::shared_ptr<UnknownType> type = std::make_shared<UnknownType>();
		return type;
	}
	std::shared_ptr<AnyType> Type::Any()
	{
		static std::shared_ptr<AnyType> type = std::make_shared<AnyType>();
		return type;
	}
	std::shared_ptr<Type> Type::Unify(const std::vector<TypePtr> types)
	{
		// Assume that the number of types is greater than 0
		if (std::equal(types.begin() + 1, types.end(), types.begin(),
			[](const TypePtr &x, const TypePtr &y) -> bool
		{
			return x->Equals(y);
		}))
		{
			return types.front();
		}
		else
		{
			std::shared_ptr<UnionType> unionType = std::make_shared<UnionType>();
			for (const auto &type : types)
			{
				if (type->typeCode == TypeCode::Union)
				{
					auto otherUnionType = std::static_pointer_cast<UnionType>(type);
					for (const auto &_type : otherUnionType->types)
					{
						unionType->types.insert(_type);
					}
				}
				else
				{
					unionType->types.insert(type);
				}
			}
			return unionType;
		}
	}
	UnknownType::UnknownType() : Type(TypeCode::Unknown) {}

	Int32Type::Int32Type() : Type(TypeCode::Int32) {}

	Int64Type::Int64Type() : Type(TypeCode::Int64) {}

	Float32Type::Float32Type() : Type(TypeCode::Float32) {}

	Float64Type::Float64Type() : Type(TypeCode::Float64) {}

	CharType::CharType() : Type(TypeCode::Char) {}

	BooleanType::BooleanType() : Type(TypeCode::Boolean) {}

	StringType::StringType() : Type(TypeCode::String) {}

	std::u32string StringType::ToString() const
	{
		return U"Predef.String";
	}

	VoidType::VoidType() : Type(TypeCode::Void) {}

	AnyType::AnyType() : Type(TypeCode::Any) {}

	ArrayType::ArrayType(TypePtr elementType)
		: Type(TypeCode::Array), elementType{ elementType } {}

	bool ArrayType::Equals(TypePtr other) const
	{
		if (typeCode == other->typeCode)
		{
			std::shared_ptr<ArrayType> arrayType =
				std::static_pointer_cast<ArrayType>(other);
			return elementType->Equals(arrayType->elementType);
		}
		else
		{
			return false;
		}
	}

	std::u32string ArrayType::ToString() const
	{
		return U"Array[" + elementType->ToString() + U"]";
	}

	FunctionType::FunctionType(TypePtr selfType, std::u32string name, std::vector<TypePtr> parameters, TypePtr returnType)
		: Type(TypeCode::Function), selfType{ selfType }, name{ name }, parameters{ parameters }, returnType{ returnType } {
	}

	bool FunctionType::Match(const std::vector<TypePtr> &args) const
	{
		if (args.size() == parameters.size())
		{
			int n = static_cast<int>(args.size());
			for (int i = 0; i < n; i++)
			{
				if (!parameters[i]->Equals(args[i]))
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	std::u32string FunctionType::ToString() const
	{
		std::u32string text = U"Function[";
		if (parameters.size() != 0)
		{
			text += parameters.front()->ToString();
			for (int i = 1; i < parameters.size(); i++)
			{
				text += U", ";
				text += parameters.at(i)->ToString();
			}
			return text + U", " + returnType->ToString() + U"]";
		}
		else
		{
			return U"Function[" + returnType->ToString() + U"]";
		}
	}
	UnionType::UnionType() : Type(TypeCode::Union) {}

	ClassType::ClassType(PackageRoute route, std::u32string name) : Type(TypeCode::Class), route{ route }, name{ name } {}

	std::u32string ClassType::ToString() const
	{
		return PackageRouteToString(route) + U"." + name;
	}

	bool ClassType::Equals(TypePtr other) const
	{
		if (typeCode == other->typeCode)
		{
			auto classType = std::static_pointer_cast<ClassType>(other);
			return name == classType->name;
		}
		else
		{
			return false;
		}
	}

	TypePtr ClassType::Replace(const Table<std::u32string, TypeParameter>& parameters) const
	{
		if (parameters.ContainsKey(name))
		{
			return std::make_shared<ClassType>(route, parameters.GetValueByKey(name).name);
		}
		else
		{
			return std::make_shared<ClassType>(route, name);
		}
	}

	ModuleType::ModuleType(PackageRoute route, std::u32string name)
		: Type(TypeCode::Module), route{ route }, name{ name } {}

	std::u32string ModuleType::ToString() const
	{
		return PackageRouteToString(route) + U"." + name;
	}

	bool ModuleType::Equals(TypePtr other) const
	{
		if (typeCode == other->typeCode)
		{
			auto moduleType = std::static_pointer_cast<ModuleType>(other);
			return name == moduleType->name;
		}
		else
		{
			return false;
		}
	}

	InterfaceType::InterfaceType(PackageRoute route, std::u32string name) : Type(TypeCode::Interface), route{ route }, name{ name }
	{
	}

	std::u32string InterfaceType::ToString() const
	{
		return PackageRouteToString(route) + U"." + name;
	}

	bool InterfaceType::Equals(TypePtr other) const
	{
		if (typeCode == other->typeCode)
		{
			auto interfaceType = std::static_pointer_cast<InterfaceType>(other);
			return name == interfaceType->name;
		}
		else
		{
			return false;
		}
	}

} // namespace cygni
