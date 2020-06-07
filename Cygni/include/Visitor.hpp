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
		json VisitSourceLocation(SourcePosition location);
		json VisitUnary(std::shared_ptr<UnaryExpression> node);
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
		json VisitLocation(LocationPtr location);
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
		std::shared_ptr<ScopeFactory<TypePtr>> scopeFactory;
		std::shared_ptr<Package> package;
		TypeGraph typeGraph;

		explicit TypeChecker(Project &project);

		TypePtr CheckUnary(std::shared_ptr<UnaryExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckBinary(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckBlock(std::shared_ptr<BlockExpression> node, Scope<TypePtr>* outerScope);
		TypePtr CheckExpression(ExpPtr node, Scope<TypePtr>* scope);
		TypePtr CheckConstant(std::shared_ptr<ConstantExpression> node);
		TypePtr CheckClassInfo(std::shared_ptr<ClassInfo> info, Scope<TypePtr>* outerScope);
		TypePtr CheckModuleInfo(std::shared_ptr<ModuleInfo> info, Scope<TypePtr>* outerScope);
		TypePtr CheckFieldDef(FieldDef &field, Scope<TypePtr>* scope);
		TypePtr CheckMethodDef(MethodDef &method, Scope<TypePtr>* outerScope);
		TypePtr CheckParameter(std::shared_ptr<ParameterExpression> parameter, Scope<TypePtr>* scope);
		TypePtr CheckReturn(std::shared_ptr<ReturnExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckConditional(std::shared_ptr<ConditionalExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckDefault(std::shared_ptr<DefaultExpression> node);
		TypePtr CheckInvocation(std::shared_ptr<InvocationExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckNewExpression(std::shared_ptr<NewExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckAssign(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckWhile(std::shared_ptr<WhileExpression> node, Scope<TypePtr>* scope);
		TypePtr CheckType(SourcePosition position, TypePtr type);
		void CheckPackage(Scope<TypePtr>* globalScope);
		void CheckProject(Scope<TypePtr>* globalScope);
		TypePtr Attach(ExpPtr node, TypePtr type);
		void CheckInterfaceConstraint(std::shared_ptr<ClassInfo> classInfo, std::shared_ptr<InterfaceInfo> interfaceInfo);
	};

	class TreeTraverser
	{
	public:
		std::function<bool(ExpPtr)> filter;
		explicit TreeTraverser(std::function<bool(ExpPtr)> filter);
		void VisitExpression(ExpPtr node, std::vector<ExpPtr>& nodeList);

		void VisitUnary(std::shared_ptr<UnaryExpression> node, std::vector<ExpPtr>& nodeList);
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
		std::shared_ptr<ScopeFactory<LocationPtr>> scopeFactory;

		explicit VariableLocator(Project& project);


		void VisitExpression(ExpPtr node, Scope<LocationPtr>* scope);
		void VisitBlock(std::shared_ptr<BlockExpression> node, Scope<LocationPtr>* outerScope);
		void VisitUnary(std::shared_ptr<UnaryExpression> node, Scope<LocationPtr>* scope);
		void VisitBinary(std::shared_ptr<BinaryExpression> node, Scope<LocationPtr>* scope);
		void VisitClassInfo(std::shared_ptr<ClassInfo> info, Scope<LocationPtr>* outerScope);
		void VisitModuleInfo(std::shared_ptr<ModuleInfo> info, Scope<LocationPtr>* outerScope);
		void VisitMethodDef(const MethodDef &method, Scope<LocationPtr>* outerScope);
		void VisitParameter(std::shared_ptr<ParameterExpression> parameter, Scope<LocationPtr>* scope);
		void VisitReturn(std::shared_ptr<ReturnExpression> node, Scope<LocationPtr>* scope);
		void VisitConditional(std::shared_ptr<ConditionalExpression> node, Scope<LocationPtr>* scope);
		void VisitInvocation(std::shared_ptr<InvocationExpression> node, Scope<LocationPtr>* scope);
		void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<LocationPtr>* scope);
		void VisitNewExpression(std::shared_ptr<NewExpression> node, Scope<LocationPtr>* scope);
		void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<LocationPtr>* scope);
		void VisitWhileExpression(std::shared_ptr<WhileExpression> node, Scope<LocationPtr>* scope);
		void VisitPackage(std::shared_ptr<Package> package, Scope<LocationPtr>* globalScope);
		void VisitProject();
	};

	class ConstantCollector
	{
	public:
		void VisitMethodDef(MethodDef &method, std::unordered_set<ConstantKey>& constantSet);
		void VisitPackage(std::shared_ptr<Package> package);
		void VisitProject(Project& project);
	};

	class TypeRenamer
	{
	public:
		void RenameAll(Project& project);
		void RenameMethod(MethodDef& method, Table<std::u32string, TypeAlias>& typeAliases);
		void RenameField(FieldDef& field, Table<std::u32string, TypeAlias>& typeAliases);
		TypePtr RenameType(TypePtr type, Table<std::u32string, TypeAlias>& typeAliases);
	};

	void AssignIndex(Project& project);

	class InheritanceProcessor
	{
	public:
		void VisitProject(Project& project);
		void VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo);
	};

	class PackageImporter
	{
	public:
		void ImportPackages(Project& project);
		void CollectInfo(Project& project, 
			std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ClassInfo>>>& classMap,
			std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ModuleInfo>>>& moduleMap,
			std::unordered_map<PackageRoute, std::vector<std::shared_ptr<InterfaceInfo>>>& interfaceMap,
			PackageRoute currentRoute,
			std::shared_ptr<Package> pkg, std::unordered_set<PackageRoute>& visited);
	};

} // namespace cygni

#endif // CYGNI_VISITOR_HPP
