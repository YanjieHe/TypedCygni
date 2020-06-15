#ifndef CYGNI_NANOPASS
#define CYGNI_NANOPASS
#include "Expression.hpp"
#include "Visitor.hpp"
#include <algorithm>

namespace cygni
{
	template <typename ... ArgTypes>
	class Nanopass : public Visitor<ExpPtr, ArgTypes...>
	{
	public:
		virtual ExpPtr VisitUnary(std::shared_ptr<UnaryExpression> node, ArgTypes... args) override
		{
			auto operand = VisitExpression(node->operand, args...);
			auto newNode = std::make_shared<UnaryExpression>(node->position, node->nodeType, operand);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitBinary(std::shared_ptr<BinaryExpression> node, ArgTypes... args) override
		{
			auto left = VisitExpression(node->left, args...);
			auto right = VisitExpression(node->right, args...);
			auto newNode = std::make_shared<BinaryExpression>(node->position, node->nodeType, left, right);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitBlock(std::shared_ptr<BlockExpression> node, ArgTypes... args) override
		{
			std::vector<ExpPtr> expressions;
			std::transform(
				node->expressions.begin(),
				node->expressions.end(),
				std::back_inserter(expressions),
				[this, &args...](ExpPtr exp)->ExpPtr { return VisitExpression(exp, args...); });
			auto newNode = std::make_shared<BlockExpression>(node->position, expressions);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitConstant(std::shared_ptr<ConstantExpression> node, ArgTypes... args) override
		{
			return node;
		}
		virtual ExpPtr VisitParameter(std::shared_ptr<ParameterExpression> parameter, ArgTypes... args) override
		{
			return parameter;
		}
		virtual ExpPtr VisitReturn(std::shared_ptr<ReturnExpression> node, ArgTypes... args) override
		{
			auto value = VisitExpression(node->value, args...);
			auto newNode = std::make_shared<ReturnExpression>(node->position, value);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitConditional(std::shared_ptr<ConditionalExpression> node, ArgTypes... args) override
		{
			auto condition = VisitExpression(node->condition, args...);
			auto ifTrue = VisitExpression(node->ifTrue, args...);
			auto ifFalse = VisitExpression(node->ifFalse, args...);
			auto newNode = std::make_shared<ConditionalExpression>(node->position, condition, ifTrue, ifFalse);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitDefault(std::shared_ptr<DefaultExpression> node, ArgTypes... args) override
		{
			return node;
		}
		virtual ExpPtr VisitInvocation(std::shared_ptr<InvocationExpression> node, ArgTypes... args) override
		{
			auto exp = VisitExpression(node->expression, args...);
			std::vector<Argument> arguments;
			std::transform(
				node->arguments.begin(),
				node->arguments.end(),
				std::back_inserter(arguments),
				[this, &args...](Argument arg)->Argument
			{
				Argument newArg(VisitExpression(arg.value, args...));
				newArg.index = arg.index;
				newArg.name = arg.name;
				return newArg;
			}
			);
			auto newNode = std::make_shared<InvocationExpression>(node->position,
				exp, arguments);
			newNode->type = node->type;
			return newNode;
		}
		virtual ExpPtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, ArgTypes... args) override
		{
			auto object = VisitExpression(node->object, args...);
			auto newNode = std::make_shared<MemberAccessExpression>(node->position,
				object, node->field);
			newNode->type = node->type;
			newNode->location = node->location;
			return newNode;
		}
		virtual ExpPtr VisitNewExpression(std::shared_ptr<NewExpression> node, ArgTypes... args) override
		{
			std::vector<Argument> arguments;
			std::transform(
				node->arguments.begin(),
				node->arguments.end(),
				std::back_inserter(arguments),
				[this, &args...](Argument arg)->Argument
			{
				Argument newArg(VisitExpression(arg.value, args...));
				newArg.index = arg.index;
				newArg.name = arg.name;
				return newArg;
			}
			);
			auto newNode = std::make_shared<NewExpression>(node->position,
				node->type, arguments);
			newNode->location = node->location;
			return newNode;
		}
		virtual ExpPtr VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, ArgTypes... args) override
		{
			auto value = VisitExpression(node->value, args...);
			auto newNode = std::make_shared<VarDefExpression>(node->position,
				node->variable, node->type, value);
			return newNode;
		}
		virtual ExpPtr VisitAssign(std::shared_ptr<BinaryExpression> node, ArgTypes... args) override
		{
			return VisitBinary(node, args...);
		}
		virtual ExpPtr VisitWhile(std::shared_ptr<WhileExpression> node, ArgTypes... args) override
		{
			auto condition = VisitExpression(node->condition, args...);
			auto body = VisitExpression(node->body, args...);
			auto newNode = std::make_shared<WhileExpression>(node->position,
				condition, body);
			newNode->type = node->type;
			return newNode;
		}
		virtual void VisitMethod(MethodInfo& method, ArgTypes... args)
		{
			method.body = VisitExpression(method.body, args...);
		}

		virtual void VisitField(FieldInfo& field, ArgTypes... args)
		{
			field.value = VisitExpression(field.value, args...);
		}

		virtual void VisitClass(std::shared_ptr<ClassInfo> classInfo, ArgTypes...args)
		{
			for (auto& method : classInfo->methodDefs)
			{
				VisitMethod(method, args...);
			}
		}
		virtual void VisitModule(std::shared_ptr<ModuleInfo> moduleInfo, ArgTypes...args)
		{
			for (auto& method : moduleInfo->methods)
			{
				VisitMethod(method, args...);
			}
		}
		virtual void VisitInterface(std::shared_ptr<InterfaceInfo> moduleInfo, ArgTypes...args)
		{
		}
		virtual void VisitPackage(std::shared_ptr<Package>& package, ArgTypes...args)
		{
			for (auto classInfo : package->classDefs)
			{
				VisitClass(classInfo, args...);
			}
			for (auto moduleInfo : package->moduleDefs)
			{
				VisitModule(moduleInfo, args...);
			}
			for (auto interfaceInfo : package->interfaceDefs)
			{
				VisitInterface(interfaceInfo, args...);
			}
		}
		virtual void VisitProject(Project& project, ArgTypes...args)
		{
			for (auto pkg : project.packages)
			{
				VisitPackage(pkg, args...);
			}
		}
	};

	class TypeRenamer
	{
	public:
		void RenameAll(Project& project);
		void RenameMethod(MethodInfo& method, Table<std::u32string, TypeAlias>& typeAliases);
		void RenameField(FieldInfo& field, Table<std::u32string, TypeAlias>& typeAliases);
		TypePtr RenameType(TypePtr type, Table<std::u32string, TypeAlias>& typeAliases);
	};

	class InheritanceTypeResolver
	{
	public:
		// resolve the super types of classes
		void VisitProject(Project& project);
		void VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo);
	};

	class InheritanceProcessor
	{
	public:
		void VisitProject(Project& project);
		void VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo);
	};

	class ArrayLengthPass : public Nanopass<>
	{
	public:
		ExpPtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node) override;
	};


	class VirtualTableGenerator : public Nanopass<>
	{
	public:
		Project& project;
		VirtualTableGenerator(Project& project);
		void VisitClass(std::shared_ptr<ClassInfo> classInfo) override;
		int GetClassId(SourcePosition position, TypePtr classType);
	};

	class HandleThisPointerPass : public Nanopass<>
	{
	public:
		Project& project;
		MethodInfo* currentMethod = nullptr;
		HandleThisPointerPass(Project& project);
		void VisitMethod(MethodInfo& method) override;
		ExpPtr VisitParameter(std::shared_ptr<ParameterExpression> parameter) override;
	};
} // namespace cygni
#endif // CYGNI_NANOPASS