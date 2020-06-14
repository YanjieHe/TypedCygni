#include "Nanopass.hpp"
#include "Exception.hpp"
#include <functional>
#include <stack>
#include <memory>
#include "Type.hpp"
#include "Visitor.hpp"

namespace cygni
{
	void TypeRenamer::RenameAll(Project & project)
	{
		for (auto pkg : project.packages)
		{
			auto& typeAliases = pkg->typeAliases;
			for (auto classInfo : pkg->classes)
			{
				for (auto& superType : classInfo->superTypes)
				{
					superType = RenameType(superType, typeAliases);
				}
				for (auto& field : classInfo->fieldDefs.values)
				{
					RenameField(field, typeAliases);
				}
				for (auto& method : classInfo->methodDefs.values)
				{
					RenameMethod(method, typeAliases);
				}
			}
			for (auto moduleInfo : pkg->modules)
			{
				for (auto& field : moduleInfo->fields.values)
				{
					RenameField(field, typeAliases);
				}
				for (auto& method : moduleInfo->methods.values)
				{
					RenameMethod(method, typeAliases);
				}
			}
		}
	}
	void TypeRenamer::RenameMethod(MethodDef& method, Table<std::u32string, TypeAlias>& typeAliases)
	{
		std::function<bool(ExpPtr)> filter = [](ExpPtr node) -> bool
		{
			return node->nodeType == ExpressionType::VariableDefinition || node->nodeType == ExpressionType::New;
		};

		TreeTraverser traverser(filter);
		std::vector<ExpPtr> nodeList;
		traverser.VisitExpression(method.body, nodeList);

		for (auto node : nodeList)
		{
			if (node->nodeType == ExpressionType::VariableDefinition)
			{
				auto varDef = std::static_pointer_cast<VarDefExpression>(node);
				varDef->variable->type = RenameType(varDef->variable->type, typeAliases);
			}
			else
			{
				// New
				auto newExp = std::static_pointer_cast<NewExpression>(node);
				newExp->type = RenameType(newExp->type, typeAliases);
			}
		}

		method.selfType = RenameType(method.selfType, typeAliases);
		method.returnType = RenameType(method.returnType, typeAliases);
		method.signature = RenameType(method.signature, typeAliases);
		for (auto parameter : method.parameters)
		{
			parameter->type = RenameType(parameter->type, typeAliases);
		}
	}
	void TypeRenamer::RenameField(FieldDef & field, Table<std::u32string, TypeAlias>& typeAliases)
	{
		field.type = RenameType(field.type, typeAliases);
	}
	TypePtr TypeRenamer::RenameType(TypePtr type, Table<std::u32string, TypeAlias>& typeAliases)
	{
		if (type->typeCode == TypeCode::Unresolved)
		{
			auto unresolvedType = std::static_pointer_cast<UnresolvedType>(type);
			if (typeAliases.ContainsKey(unresolvedType->name))
			{
				auto typeAlias = typeAliases.GetValueByKey(unresolvedType->name);
				return std::make_shared<UnresolvedType>(typeAlias.route, typeAlias.typeName);
			}
			else
			{
				return type;
			}
		}
		else if (type->typeCode == TypeCode::Function)
		{
			auto functionType = std::static_pointer_cast<FunctionType>(type);
			std::vector<TypePtr> parameters;
			std::transform(
				functionType->parameters.begin(),
				functionType->parameters.end(),
				std::back_inserter(parameters),
				[&](TypePtr t) -> TypePtr { return RenameType(t, typeAliases); });
			return std::make_shared<FunctionType>(
				RenameType(functionType->selfType, typeAliases),
				functionType->name,
				parameters,
				RenameType(functionType->returnType, typeAliases));
		}
		else if (type->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(type);
			return std::make_shared<ArrayType>(
				RenameType(arrayType->elementType, typeAliases));
		}
		else
		{
			// TO DO: GENERIC TYPE
			return type;
		}
	}

	void InheritanceTypeResolver::VisitProject(Project & project)
	{
		for (const auto& pkg : project.packages)
		{
			for (const auto& classInfo : pkg->classDefs)
			{
				VisitClass(project, classInfo);
			}
		}
	}

	void InheritanceTypeResolver::VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo)
	{
		for (auto & superType : classInfo->superTypes)
		{
			if (superType->typeCode == TypeCode::Unresolved)
			{
				auto unresolvedType = std::static_pointer_cast<UnresolvedType>(superType);
				if (auto typeInfo = project.ResolveType(unresolvedType))
				{
					superType = *typeInfo;
				}
				else
				{
					throw TypeException(classInfo->position, Format(U"missing super type '{}'", superType->ToString()));
				}
			}
			else
			{
				throw TypeException(classInfo->position, Format(U"cannot inherit type '{}'", superType->ToString()));
			}
		}
	}

	void InheritanceProcessor::VisitProject(Project & project)
	{
		for (auto& program : project.programs.values)
		{
			for (auto& classInfo : program.classDefs.values)
			{
				int n = static_cast<int>(classInfo->superTypes.size());
				for (int i = 0; i < n; i++)
				{
					auto superType = classInfo->superTypes.at(i);
					if (superType->typeCode == TypeCode::Class)
					{
						if (i != 0)
						{
							throw TypeException(
								classInfo->position,
								U"the super class must be at the first of the extends list"
							);
						}
					}
					else if (superType->typeCode == TypeCode::Interface)
					{
						// pass
					}
					else
					{
						throw TypeException(
							classInfo->position,
							Format(U"missing super class '{}", superType->ToString())
						);
					}
				}

			}
		}
		for (auto& program : project.programs.values)
		{
			for (auto& classInfo : program.classDefs.values)
			{
				VisitClass(project, classInfo);
			}
		}
	}
	void InheritanceProcessor::VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo)
	{
		std::stack<FieldDef> fieldStack;
		std::stack<MethodDef> methodStack;
		std::unordered_set<TypePtr> typeSet;
		std::unordered_set<std::u32string> fieldNames;
		std::unordered_set<std::u32string> methodNames;

		auto originalClass = classInfo;
		bool done = false;
		while (!done)
		{
			if (typeSet.find(
				std::make_shared<ClassType>(classInfo->route, classInfo->name)) != typeSet.end())
			{
				throw TypeException(originalClass->position,
					U"cycle detected in the inheritance chain");
			}
			else
			{
				typeSet.insert(
					std::make_shared<ClassType>(classInfo->route, classInfo->name));
			}
			int nFields = static_cast<int>(classInfo->fieldDefs.Size());
			for (int i = nFields - 1; i >= 0; i--)
			{
				auto& field = classInfo->fieldDefs.values[i];
				if (fieldNames.find(field.name) != fieldNames.end())
				{
					throw TypeException(originalClass->position,
						Format(
							U"duplicate field definition '{}' detected in the inheritance chain", field.name));
				}
				else
				{
					fieldNames.insert(field.name);
					fieldStack.push(field);
				}
			}
			int nMethods = static_cast<int>(classInfo->methodDefs.Size());
			for (int i = nMethods - 1; i >= 0; i--)
			{
				auto& method = classInfo->methodDefs.values[i];
				if (methodNames.find(method.name) != methodNames.end())
				{
					// override support
					throw TypeException(originalClass->position,
						Format(
							U"duplicate method definition '{}' detected in the inheritance chain", method.name));
				}
				else
				{
					methodNames.insert(method.name);
					methodStack.push(method);
				}
			}
			if (classInfo->superTypes.size() >= 1)
			{
				auto type = classInfo->superTypes.front();
				if (type->typeCode == TypeCode::Class)
				{
					if (auto superClassInfo = project.GetClass(std::static_pointer_cast<ClassType>(type)))
					{
						classInfo = *superClassInfo;
					}
					else
					{
						throw TypeException(classInfo->position,
							Format(U"missing super class '{}' deifinition", type->ToString())
						);
					}
				}
				else
				{
					done = true;
				}
			}
			else
			{
				done = true;
			}
		}
		while (!(fieldStack.empty()))
		{
			auto field = fieldStack.top();
			originalClass->fields.Add(field.name, field);
			fieldStack.pop();
		}
		while (!(methodStack.empty()))
		{
			auto method = methodStack.top();
			originalClass->methods.Add(method.name, method);
			methodStack.pop();
		}
	}
	ExpPtr ArrayLengthPass::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node)
	{
		auto object = VisitExpression(node->object);
		if (object->type->typeCode == TypeCode::Array)
		{
			if (node->field == U"Size" || node->field == U"Length")
			{
				auto newNode = std::make_shared<UnaryExpression>(node->position, ExpressionType::ArrayLength, object);
				newNode->type = Type::Int32();
				return newNode;
			}
			else
			{
				throw TypeException(node->position,
					Format(U"not supported array field '{}'", node->field));
			}
		}
		else
		{
			auto newNode = std::make_shared<MemberAccessExpression>(node->position,
				object, node->field);
			newNode->type = node->type;
			return newNode;
		}
	}
} // namespace cygni