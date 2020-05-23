#ifndef CYGNI_VISITOR_HPP
#define CYGNI_VISITOR_HPP
#include "Expression.hpp"
#include <nlohmann/json.hpp>
#include <functional>

namespace cygni
{
	using json = nlohmann::json;

	class AstToJsonSerialization
	{
	public:
		json VisitSourceLocation(SourceLocation location);
		json VisitBinary(std::shared_ptr<BinaryExpression> node);
		json VisitBlock(std::shared_ptr<BlockExpression> node);
		json VisitExpression(ExpPtr node);
		json VisitConstant(std::shared_ptr<ConstantExpression> node);
		json VisitClassInfo(std::shared_ptr<ClassInfo> info);
		json VisitModuleInfo(std::shared_ptr<ModuleInfo> info);
		json VisitFieldDef(const FieldDef &field);
		json VisitMethodDef(const MethodDef &method);
		json VisitParameter(std::shared_ptr<ParameterExpression> parameter);
		json VisitReturn(std::shared_ptr<ReturnExpression> node);
		json VisitConditional(std::shared_ptr<ConditionalExpression> node);
		json VisitDefault(std::shared_ptr<DefaultExpression> node);
		json VisitInvocation(std::shared_ptr<InvocationExpression> node);
		json VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node);
		json VisitNewExpression(std::shared_ptr<NewExpression> node);
		json VisitVarDefExpression(std::shared_ptr<VarDefExpression> node);
		json VisitWhileLoop(std::shared_ptr<WhileExpression> node);
		void AttachNodeInformation(json &obj, ExpPtr node);
		json VisitPackage(std::shared_ptr<Package> package);
		json VisitProject(Project &project);
		std::vector<json> VisitArgumentList(const std::vector<Argument> &arguments);
		json VisitAnnotationList(const Table<std::u32string, AnnotationInfo> &annotations);
		std::vector<json> VisitExpressionList(const std::vector<ExpPtr> &expressions);
		json VisitParameterLocation(const ParameterLocation& location);
	};

	class TypeChecker
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
		Project &project;
		std::shared_ptr<Package> package;
		explicit TypeChecker(Project &project);

		TypePtr VisitBinary(std::shared_ptr<BinaryExpression> node, ScopePtr scope);
		TypePtr VisitBlock(std::shared_ptr<BlockExpression> node, ScopePtr outerScope);
		TypePtr VisitExpression(ExpPtr node, ScopePtr scope);
		TypePtr VisitConstant(std::shared_ptr<ConstantExpression> node);
		TypePtr VisitClassInfo(std::shared_ptr<ClassInfo> info, ScopePtr outerScope);
		TypePtr VisitModuleInfo(std::shared_ptr<ModuleInfo> info, ScopePtr outerScope);
		TypePtr VisitFieldDef(const FieldDef &field, ScopePtr scope);
		TypePtr VisitMethodDef(const MethodDef &method, ScopePtr outerScope);
		TypePtr VisitParameter(std::shared_ptr<ParameterExpression> parameter, ScopePtr scope);
		TypePtr VisitReturn(std::shared_ptr<ReturnExpression> node, ScopePtr scope);
		TypePtr VisitConditional(std::shared_ptr<ConditionalExpression> node, ScopePtr scope);
		TypePtr VisitDefault(std::shared_ptr<DefaultExpression> node);
		TypePtr VisitInvocation(std::shared_ptr<InvocationExpression> node, ScopePtr scope);
		TypePtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, ScopePtr scope);
		TypePtr VisitNewExpression(std::shared_ptr<NewExpression> node, ScopePtr scope);
		TypePtr VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, ScopePtr scope);
		void RegisterModulesAndClasses(std::shared_ptr<Package>& pkg, std::unordered_set<PackageRoute>& visited);
		void VisitPackage(ScopePtr globalScope);
		void VisitProject(ScopePtr globalScope);
		TypePtr Attach(ExpPtr node, TypePtr type);
		TypePtr VisitAssign(std::shared_ptr<BinaryExpression> node, ScopePtr scope);
		TypePtr VisitWhile(std::shared_ptr<WhileExpression> node, ScopePtr scope);
	};

	class TreeTraverser
	{
	public:
		std::function<bool(ExpPtr)> filter;
		explicit TreeTraverser(std::function<bool(ExpPtr)> filter);
		void VisitExpression(ExpPtr node, std::vector<ExpPtr>& nodeList);

		void VisitBinary(std::shared_ptr<BinaryExpression> node, std::vector<ExpPtr>& nodeList);
		void VisitBlock(std::shared_ptr<BlockExpression>node, std::vector<ExpPtr>&nodeList);
		void VisitReturn(std::shared_ptr<ReturnExpression>node, std::vector<ExpPtr>&nodeList);
		void VisitConditional(std::shared_ptr<ConditionalExpression>node, std::vector<ExpPtr>&nodeList);
		void VisitInvocation(std::shared_ptr<InvocationExpression> node, std::vector<ExpPtr>&nodeList);
		void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, std::vector<ExpPtr>&nodeList);
		void VisitNewExpression(std::shared_ptr<NewExpression>node, std::vector<ExpPtr>& nodeList);
		void VisitVarDefExpression(std::shared_ptr<VarDefExpression>node, std::vector<ExpPtr>&nodeList);
		void VisitWhileExpression(std::shared_ptr<WhileExpression>node, std::vector<ExpPtr>&nodeList);
	};

	class LocalVariableCollector
	{
	public:
		void VisitMethodDef(MethodDef &method);
		void VisitPackage(std::shared_ptr<Package> package);
		void VisitProject(Project& project);
	};

	class VariableLocator
	{
	public:
		Project& project;
		explicit VariableLocator(Project& project);
		void VisitExpression(ExpPtr node, ScopePtr scope);
		void VisitBlock(std::shared_ptr<BlockExpression> node, ScopePtr outerScope);
		void VisitBinary(std::shared_ptr<BinaryExpression> node, ScopePtr scope);
		void VisitClassInfo(std::shared_ptr<ClassInfo> info, ScopePtr outerScope);
		void VisitModuleInfo(std::shared_ptr<ModuleInfo> info, ScopePtr outerScope);
		void VisitMethodDef(const MethodDef &method, ScopePtr outerScope);
		void VisitParameter(std::shared_ptr<ParameterExpression> parameter, ScopePtr scope);
		void VisitReturn(std::shared_ptr<ReturnExpression> node, ScopePtr scope);
		void VisitConditional(std::shared_ptr<ConditionalExpression> node, ScopePtr scope);
		void VisitInvocation(std::shared_ptr<InvocationExpression> node, ScopePtr scope);
		void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, ScopePtr scope);
		void VisitNewExpression(std::shared_ptr<NewExpression> node, ScopePtr scope);
		void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, ScopePtr scope);
		void VisitWhileExpression(std::shared_ptr<WhileExpression> node, ScopePtr scope);
		void VisitPackage(std::shared_ptr<Package> package, ScopePtr globalScope);
		void VisitProject();
	};

	class ConstantCollector
	{
	public:
		void VisitMethodDef(MethodDef &method, std::unordered_set<ConstantKey>& constantSet);
		void VisitPackage(std::shared_ptr<Package> package);
		void VisitProject(Project& project);
	};

	class ClassAndModuleLocator
	{
	public:
		void VisitProject(Project& project);
	};
} // namespace cygni

#endif // CYGNI_VISITOR_HPP
