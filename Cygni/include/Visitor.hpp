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
  json VisitProgram(const Program &program);
  std::vector<json> VisitArgumentList(const std::vector<Argument> &arguments);
  json VisitAnnotationList(const std::vector<AnnotationInfo> &annotations);
  std::vector<json> VisitExpressionList(const std::vector<ExpPtr> &expressions);
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

    Rule(std::u32string functionName, std::vector<TypePtr> parameters,
         TypePtr returnType);
  };

  class RuleSet {
  public:
    std::unordered_map<std::u32string, std::vector<Rule>> rules;

    RuleSet() = default;

    void Add(std::u32string functionName, std::vector<TypePtr> parameters,
             TypePtr returnType);

    std::optional<TypePtr> Match(std::u32string functionName,
                                 std::vector<TypePtr> parameters);
  };

  RuleSet ruleSet;
  const Program &program;
  TypeChecker(const Program &program);

  TypePtr VisitBinary(std::shared_ptr<BinaryExpression> node, ScopePtr scope);
  TypePtr VisitBlock(std::shared_ptr<BlockExpression> node, ScopePtr scope);
  TypePtr VisitExpression(ExpPtr node, ScopePtr scope);
  TypePtr VisitConstant(std::shared_ptr<ConstantExpression> node);
  TypePtr VisitClassInfo(std::shared_ptr<ClassInfo> info, ScopePtr outerScope);
  TypePtr VisitModuleInfo(std::shared_ptr<ModuleInfo> info,
                          ScopePtr outerScope);
  TypePtr VisitFieldDef(const FieldDef &field, ScopePtr scope);
  TypePtr VisitMethodDef(const MethodDef &method, ScopePtr outerScope);
  TypePtr VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                         ScopePtr scope);
  TypePtr VisitReturn(std::shared_ptr<ReturnExpression> node, ScopePtr scope);
  TypePtr VisitConditional(std::shared_ptr<ConditionalExpression> node,
                           ScopePtr scope);
  TypePtr VisitDefault(std::shared_ptr<DefaultExpression> node);
  TypePtr VisitInvocation(std::shared_ptr<InvocationExpression> node,
                          ScopePtr scope);
  TypePtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                            ScopePtr scope);
  TypePtr VisitNewExpression(std::shared_ptr<NewExpression> node,
                             ScopePtr scope);
  TypePtr VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                                ScopePtr scope);
  void VisitProgram(ScopePtr scope);
  TypePtr Attach(ExpPtr node, TypePtr type);
  TypePtr VisitAssign(std::shared_ptr<BinaryExpression> node, ScopePtr scope);
};

} // namespace cygni

#endif // CYGNI_VISITOR_HPP
