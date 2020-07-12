#ifndef CYGNI_TYPE_CHECKER
#define CYGNI_TYPE_CHECKER
#include "Expression.hpp"
#include "Scope.hpp"
#include "Type.hpp"
#include "Visitor.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cygni {
class TypeChecker : public Visitor<TypePtr, std::shared_ptr<SourceDocument>,
                                   Scope<TypePtr> *> {
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
  Project &project;
  TypeGraph &typeGraph;
  std::shared_ptr<ScopeFactory<TypePtr>> scopeFactory;

  explicit TypeChecker(Project &project);

  TypePtr VisitUnary(std::shared_ptr<UnaryExpression> node,
                     std::shared_ptr<SourceDocument> program,
                     Scope<TypePtr> *scope);
  TypePtr VisitBinary(std::shared_ptr<BinaryExpression> node,
                      std::shared_ptr<SourceDocument> program,
                      Scope<TypePtr> *scope);
  TypePtr VisitBlock(std::shared_ptr<BlockExpression> node,
                     std::shared_ptr<SourceDocument> program,
                     Scope<TypePtr> *outerScope);
  TypePtr VisitConstant(std::shared_ptr<ConstantExpression> node,
                        std::shared_ptr<SourceDocument> program,
                        Scope<TypePtr> *outerScope);
  TypePtr VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                         std::shared_ptr<SourceDocument> program,
                         Scope<TypePtr> *scope);
  TypePtr VisitReturn(std::shared_ptr<ReturnExpression> node,
                      std::shared_ptr<SourceDocument> program,
                      Scope<TypePtr> *scope);
  TypePtr VisitConditional(std::shared_ptr<ConditionalExpression> node,
                           std::shared_ptr<SourceDocument> program,
                           Scope<TypePtr> *scope);
  TypePtr VisitDefault(std::shared_ptr<DefaultExpression> node,
                       std::shared_ptr<SourceDocument> program,
                       Scope<TypePtr> *outerScope);
  TypePtr VisitInvocation(std::shared_ptr<InvocationExpression> node,
                          std::shared_ptr<SourceDocument> program,
                          Scope<TypePtr> *scope);
  TypePtr VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                            std::shared_ptr<SourceDocument> program,
                            Scope<TypePtr> *scope);
  TypePtr VisitNewExpression(std::shared_ptr<NewExpression> node,
                             std::shared_ptr<SourceDocument> program,
                             Scope<TypePtr> *scope);
  TypePtr VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                                std::shared_ptr<SourceDocument> program,
                                Scope<TypePtr> *scope);
  TypePtr VisitAssign(std::shared_ptr<BinaryExpression> node,
                      std::shared_ptr<SourceDocument> program,
                      Scope<TypePtr> *scope);
  TypePtr VisitWhile(std::shared_ptr<WhileExpression> node,
                     std::shared_ptr<SourceDocument> program,
                     Scope<TypePtr> *scope);

  TypePtr CheckClassInfo(std::shared_ptr<ClassInfo> info,
                         std::shared_ptr<SourceDocument> program,
                         Scope<TypePtr> *outerScope);
  TypePtr CheckModuleInfo(std::shared_ptr<ModuleInfo> info,
                          std::shared_ptr<SourceDocument> program,
                          Scope<TypePtr> *outerScope);
  TypePtr CheckFieldDef(std::shared_ptr<FieldInfo> field,
                        std::shared_ptr<SourceDocument> program,
                        Scope<TypePtr> *scope);
  TypePtr CheckMethodDef(std::shared_ptr<MethodInfo> method,
                         std::shared_ptr<SourceDocument> program,
                         Scope<TypePtr> *outerScope);
  TypePtr CheckType(SourcePosition position, TypePtr type,
                    std::shared_ptr<SourceDocument> program,
                    Scope<TypePtr> *scope);
  void CheckPackage(std::shared_ptr<Package> package,
                    Scope<TypePtr> *globalScope);
  void CheckProgramFile(std::shared_ptr<SourceDocument> program,
                        Scope<TypePtr> *globalScope);
  void CheckProject();
  TypePtr Attach(ExpPtr node, TypePtr type);
  void CheckInterfaceConstraint(std::shared_ptr<ClassInfo> classInfo,
                                std::shared_ptr<InterfaceInfo> interfaceInfo);
  void InitializeTypeGraph(Scope<TypePtr> *scope);
  void ResolveSignatures(Scope<TypePtr> *globalScope);
  void ResolveMethodSignature(std::shared_ptr<MethodInfo> method,
                              std::shared_ptr<SourceDocument> program,
                              Scope<TypePtr> *scope);
  void ResolveFieldSignature(std::shared_ptr<FieldInfo> field,
                             std::shared_ptr<SourceDocument> program,
                             Scope<TypePtr> *scope);
  void SpecializeGenericType(std::shared_ptr<Type> type,
                             SourcePosition position,
                             std::shared_ptr<SourceDocument> program,
                             Scope<TypePtr> *scope);
  void VisitTemplateClass(std::shared_ptr<TemplateClass> templateClass,
                          std::vector<TypePtr> typeArguments,
                          std::shared_ptr<SourceDocument> program,
                          Scope<TypePtr> *outerScope);
};
} // namespace cygni
#endif // CYGNI_TYPE_CHECKER