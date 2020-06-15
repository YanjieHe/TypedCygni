#ifndef CYGNI_TYPE_CHECKER
#define CYGNI_TYPE_CHECKER
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include <functional>
#include "Type.hpp"
#include "Scope.hpp"
#include "Expression.hpp"
#include "Visitor.hpp"

namespace cygni
{
	class TypeChecker: public Visitor<TypePtr, Scope<TypePtr>*>
	{
	public:
		class Rule
		{
		public:
			std::u32string functionName;
			std::vector<TypePtr> parameters;
			TypePtr returnType;

			Rule(std::u32string functionName, std::vector<TypePtr> parameters,
				TypePtr returnType);
		};

		class RuleSet
		{
		public:
			std::unordered_map<std::u32string, std::vector<Rule>> rules;

			RuleSet() = default;

			void Add(std::u32string functionName, std::vector<TypePtr> parameters,
				TypePtr returnType);

			std::optional<TypePtr> Match(std::u32string functionName,
				std::vector<TypePtr> parameters);
		};

		RuleSet ruleSet;
		TypeGraph& typeGraph;
		std::shared_ptr<ScopeFactory<TypePtr>> scopeFactory;
		std::shared_ptr<Package> package;

		Project &project;

		explicit TypeChecker(Project &project);

		TypePtr VisitUnary(std::shared_ptr<UnaryExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitBinary(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitBlock(std::shared_ptr<BlockExpression> node, Scope<TypePtr>* outerScope);
		TypePtr VisitConstant(std::shared_ptr<ConstantExpression> node, Scope<TypePtr>* outerScope);
		TypePtr VisitParameter(std::shared_ptr<ParameterExpression> parameter, Scope<TypePtr>* scope);
		TypePtr VisitReturn(std::shared_ptr<ReturnExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitConditional(std::shared_ptr<ConditionalExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitDefault(std::shared_ptr<DefaultExpression> node, Scope<TypePtr>* outerScope);
		TypePtr VisitInvocation(std::shared_ptr<InvocationExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitNewExpression(std::shared_ptr<NewExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitAssign(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope);
		TypePtr VisitWhile(std::shared_ptr<WhileExpression> node, Scope<TypePtr>* scope);


		TypePtr CheckClassInfo(std::shared_ptr<ClassInfo> info, Scope<TypePtr>* outerScope);
		TypePtr CheckModuleInfo(std::shared_ptr<ModuleInfo> info, Scope<TypePtr>* outerScope);
		TypePtr CheckFieldDef(FieldInfo &field, Scope<TypePtr>* scope);
		TypePtr CheckMethodDef(MethodInfo &method, Scope<TypePtr>* outerScope);
		TypePtr CheckType(SourcePosition position, TypePtr type);
		void CheckPackage(Scope<TypePtr>* globalScope);
		void CheckProject(Scope<TypePtr>* globalScope);
		TypePtr Attach(ExpPtr node, TypePtr type);
		void CheckInterfaceConstraint(std::shared_ptr<ClassInfo> classInfo, std::shared_ptr<InterfaceInfo> interfaceInfo);
	};
} // namespace cygni
#endif // CYGNI_TYPE_CHECKER