#ifndef CYGNI_VISITOR_HPP
#define CYGNI_VISITOR_HPP
#include "Expression.hpp"
#include <nlohmann/json.hpp>

namespace cygni {
using json = nlohmann::json;

class AstToJsonSerialization {
public:
	json VisitSourceLocation(SourceLocation location);
	json VisitBinary(std::shared_ptr<BinaryExpression> node);
	json VisitBlock(std::shared_ptr<BlockExpression> node);
	json VisitExpression(ExpPtr node);
	json VisitConstant(std::shared_ptr<ConstantExpression> node);
	json VisitClassInfo(std::shared_ptr<ClassInfo> info);
	json VisitFieldDef(const FieldDef& field);
	json VisitMethodDef(const MethodDef& method);
	json VisitParameter(std::shared_ptr<ParameterExpression> parameter);
	json VisitReturn(std::shared_ptr<ReturnExpression> node);
	json VisitConditional(std::shared_ptr<ConditionalExpression> node);
	json VisitDefault(std::shared_ptr<DefaultExpression> node);
	json VisitInvocation(std::shared_ptr<InvocationExpression> node);
	void AttachNodeInformation(json& obj, ExpPtr node);
	json VisitProgram(const Program& program);
	std::vector<json> VisitArgumentList(const std::vector<ExpPtr>& arguments);
};

// class LocalVariableCollector {
// public:
// 	void VisitExpression(ExpPtr node);
// 	void VisitProgram(Program& program);
// 	void VisitMethodDef(MethodDef& method);
// 	void VisitBlockExpression(std::shared_ptr<BlockExpression> node);
// };

class TypeChecker {
public:
	class Rule {
	public:
		std::u32string functionName;
		std::vector<TypePtr> parameters;
		TypePtr returnType;

		Rule(std::u32string functionName, std::vector<TypePtr> parameters, TypePtr returnType);
	};

	class RuleSet {
	public:
		std::unordered_map<std::u32string, std::vector<Rule>> rules;

		RuleSet() = default;

		void Add(std::u32string functionName,
						std::vector<TypePtr> parameters, TypePtr returnType);
		
		std::optional<TypePtr> Match(std::u32string functionName, std::vector<TypePtr> parameters);
	};

	RuleSet ruleSet;

	TypeChecker();

	TypePtr VisitBinary(std::shared_ptr<BinaryExpression> node, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitBlock(std::shared_ptr<BlockExpression> node, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitExpression(ExpPtr node, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitConstant(std::shared_ptr<ConstantExpression> node);
	TypePtr VisitClassInfo(std::shared_ptr<ClassInfo> info, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitFieldDef(const FieldDef& field, Program& program, ClassInfo& info);
	TypePtr VisitMethodDef(const MethodDef& method, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitParameter(std::shared_ptr<ParameterExpression> parameter, Program& program, ClassInfo& info);
	TypePtr VisitReturn(std::shared_ptr<ReturnExpression> node, ScopePtr scope, Program& program, ClassInfo& info);
	TypePtr VisitConditional(std::shared_ptr<ConditionalExpression> node, Program& program, ClassInfo& info);
	TypePtr VisitDefault(std::shared_ptr<DefaultExpression> node);
	TypePtr VisitInvocation(std::shared_ptr<InvocationExpression> node, Program& program, ClassInfo& info);
	void VisitProgram(const Program& program);
	TypePtr Attach(ExpPtr node, TypePtr type);
};

} // namespace cygni

#endif // CYGNI_VISITOR_HPP