#ifndef CYGNI_VISITOR_HPP
#define CYGNI_VISITOR_HPP
#include "Enum.hpp"
#include "Exception.hpp"
#include "Expression.hpp"
#include "Utility.hpp"
#include <functional>
#include <nlohmann/json.hpp>

namespace cygni {
template <typename ReturnType, typename... ArgTypes> class Visitor {
public:
  virtual ReturnType VisitExpression(ExpPtr node, ArgTypes... args) {
    switch (node->nodeType) {
    case ExpressionType::Add:
    case ExpressionType::Subtract:
    case ExpressionType::Multiply:
    case ExpressionType::Divide:
    case ExpressionType::GreaterThan:
    case ExpressionType::LessThan:
    case ExpressionType::GreaterThanOrEqual:
    case ExpressionType::LessThanOrEqual:
    case ExpressionType::Equal:
    case ExpressionType::NotEqual:
      return VisitBinary(std::static_pointer_cast<BinaryExpression>(node),
                         args...);
    case ExpressionType::UnaryPlus:
    case ExpressionType::UnaryMinus:
    case ExpressionType::Convert:
    // case ExpressionType::UpCast:
    case ExpressionType::DownCast:
    case ExpressionType::ArrayLength:
      return VisitUnary(std::static_pointer_cast<UnaryExpression>(node),
                        args...);
    case ExpressionType::Assign:
      return VisitAssign(std::static_pointer_cast<BinaryExpression>(node),
                         args...);
    case ExpressionType::Block:
      return VisitBlock(std::static_pointer_cast<BlockExpression>(node),
                        args...);
    case ExpressionType::Constant:
      return VisitConstant(std::static_pointer_cast<ConstantExpression>(node),
                           args...);
    case ExpressionType::Parameter:
      return VisitParameter(std::static_pointer_cast<ParameterExpression>(node),
                            args...);
    case ExpressionType::Conditional:
      return VisitConditional(
          std::static_pointer_cast<ConditionalExpression>(node), args...);
    case ExpressionType::Default:
      return VisitDefault(std::static_pointer_cast<DefaultExpression>(node),
                          args...);
    case ExpressionType::Invoke:
      return VisitInvocation(
          std::static_pointer_cast<InvocationExpression>(node), args...);
    case ExpressionType::Return:
      return VisitReturn(std::static_pointer_cast<ReturnExpression>(node),
                         args...);
    case ExpressionType::MemberAccess:
      return VisitMemberAccess(
          std::static_pointer_cast<MemberAccessExpression>(node), args...);
    case ExpressionType::New:
      return VisitNewExpression(std::static_pointer_cast<NewExpression>(node),
                                args...);
    case ExpressionType::VariableDefinition:
      return VisitVarDefExpression(
          std::static_pointer_cast<VarDefExpression>(node), args...);
    case ExpressionType::While:
      return VisitWhile(std::static_pointer_cast<WhileExpression>(node),
                        args...);
    default:
      throw NotImplementedException(
          Format(U"not supported node type '{}' for visitor",
                 Enum<ExpressionType>::ToString(node->nodeType)));
    }
  }
  virtual ReturnType VisitUnary(std::shared_ptr<UnaryExpression> node,
                                ArgTypes... args) = 0;
  virtual ReturnType VisitBinary(std::shared_ptr<BinaryExpression> node,
                                 ArgTypes... args) = 0;
  virtual ReturnType VisitBlock(std::shared_ptr<BlockExpression> node,
                                ArgTypes... args) = 0;
  virtual ReturnType VisitConstant(std::shared_ptr<ConstantExpression> node,
                                   ArgTypes... args) = 0;
  virtual ReturnType
  VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                 ArgTypes... args) = 0;
  virtual ReturnType VisitReturn(std::shared_ptr<ReturnExpression> node,
                                 ArgTypes... args) = 0;
  virtual ReturnType
  VisitConditional(std::shared_ptr<ConditionalExpression> node,
                   ArgTypes... args) = 0;
  virtual ReturnType VisitDefault(std::shared_ptr<DefaultExpression> node,
                                  ArgTypes... args) = 0;
  virtual ReturnType VisitInvocation(std::shared_ptr<InvocationExpression> node,
                                     ArgTypes... args) = 0;
  virtual ReturnType
  VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                    ArgTypes... args) = 0;
  virtual ReturnType VisitNewExpression(std::shared_ptr<NewExpression> node,
                                        ArgTypes... args) = 0;
  virtual ReturnType
  VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                        ArgTypes... args) = 0;
  virtual ReturnType VisitAssign(std::shared_ptr<BinaryExpression> node,
                                 ArgTypes... args) = 0;
  virtual ReturnType VisitWhile(std::shared_ptr<WhileExpression> node,
                                ArgTypes... args) = 0;
};

using json = nlohmann::json;

class AstToJsonSerialization : public Visitor<json> {
public:
  json VisitUnary(std::shared_ptr<UnaryExpression> node) override;
  json VisitBinary(std::shared_ptr<BinaryExpression> node) override;
  json VisitBlock(std::shared_ptr<BlockExpression> node) override;
  json VisitConstant(std::shared_ptr<ConstantExpression> node) override;
  json VisitParameter(std::shared_ptr<ParameterExpression> parameter) override;
  json VisitReturn(std::shared_ptr<ReturnExpression> node) override;
  json VisitConditional(std::shared_ptr<ConditionalExpression> node) override;
  json VisitDefault(std::shared_ptr<DefaultExpression> node) override;
  json VisitInvocation(std::shared_ptr<InvocationExpression> node) override;
  json VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node) override;
  json VisitNewExpression(std::shared_ptr<NewExpression> node) override;
  json VisitVarDefExpression(std::shared_ptr<VarDefExpression> node) override;
  json VisitWhile(std::shared_ptr<WhileExpression> node) override;
  json VisitAssign(std::shared_ptr<BinaryExpression> node) override;

  json VisitClassInfo(std::shared_ptr<ClassInfo> info);
  json VisitModuleInfo(std::shared_ptr<ModuleInfo> info);
  json VisitInterfaceInfo(std::shared_ptr<InterfaceInfo> info);
  json VisitSourceLocation(SourcePosition location);
  json VisitFieldDef(const std::shared_ptr<FieldInfo> field);
  json VisitMethodDef(const std::shared_ptr<MethodInfo> method);
  void AttachNodeInformation(json &obj, ExpPtr node);
  json VisitPackage(std::shared_ptr<Package> package);
  json VisitProject(Project &project);
  std::vector<json> VisitArgumentList(const std::vector<Argument> &arguments);
  json
  VisitAnnotationList(const Table<std::u32string, AnnotationInfo> &annotations);
  std::vector<json> VisitExpressionList(const std::vector<ExpPtr> &expressions);
  json VisitLocation(LocationPtr location);
  json VisitVirtualTable(const VirtualTable &virtualTable);
};

class TreeTraverser : public Visitor<void, std::vector<ExpPtr> &> {
public:
  std::function<bool(ExpPtr)> filter;
  explicit TreeTraverser(std::function<bool(ExpPtr)> filter);

  void VisitConstant(std::shared_ptr<ConstantExpression> node,
                     std::vector<ExpPtr> &nodeList);
  void VisitParameter(std::shared_ptr<ParameterExpression> node,
                      std::vector<ExpPtr> &nodeList);
  void VisitAssign(std::shared_ptr<BinaryExpression> node,
                   std::vector<ExpPtr> &nodeList);
  void VisitDefault(std::shared_ptr<DefaultExpression> node,
                    std::vector<ExpPtr> &nodeList);
  void VisitWhile(std::shared_ptr<WhileExpression> node,
                  std::vector<ExpPtr> &nodeList);
  void VisitUnary(std::shared_ptr<UnaryExpression> node,
                  std::vector<ExpPtr> &nodeList);
  void VisitBinary(std::shared_ptr<BinaryExpression> node,
                   std::vector<ExpPtr> &nodeList);
  void VisitBlock(std::shared_ptr<BlockExpression> node,
                  std::vector<ExpPtr> &nodeList);
  void VisitReturn(std::shared_ptr<ReturnExpression> node,
                   std::vector<ExpPtr> &nodeList);
  void VisitConditional(std::shared_ptr<ConditionalExpression> node,
                        std::vector<ExpPtr> &nodeList);
  void VisitInvocation(std::shared_ptr<InvocationExpression> node,
                       std::vector<ExpPtr> &nodeList);
  void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                         std::vector<ExpPtr> &nodeList);
  void VisitNewExpression(std::shared_ptr<NewExpression> node,
                          std::vector<ExpPtr> &nodeList);
  void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                             std::vector<ExpPtr> &nodeList);
  void VisitWhileExpression(std::shared_ptr<WhileExpression> node,
                            std::vector<ExpPtr> &nodeList);

  void VisitField(const std::shared_ptr<FieldInfo> field,
                  std::vector<ExpPtr> &nodeList);
  void VisitMethod(const std::shared_ptr<MethodInfo> method,
                   std::vector<ExpPtr> &nodeList);
  void VisitClass(std::shared_ptr<ClassInfo> classInfo,
                  std::vector<ExpPtr> &nodeList);
  void VisitModule(std::shared_ptr<ModuleInfo> moduleInfo,
                   std::vector<ExpPtr> &nodeList);

  void AddIfNeeded(ExpPtr node, std::vector<ExpPtr> &nodeList);
};

class LocalVariableCollector {
public:
  void VisitMethodDef(std::shared_ptr<MethodInfo> method);
  void VisitPackage(std::shared_ptr<Package> package);
  void VisitProject(Project &project);
};

class VariableLocator : public Visitor<void, Scope<LocationPtr> *> {
public:
  Project &project;
  std::shared_ptr<ScopeFactory<LocationPtr>> scopeFactory;

  explicit VariableLocator(Project &project);

  void VisitConstant(std::shared_ptr<ConstantExpression> node,
                     Scope<LocationPtr> *scope);
  void VisitDefault(std::shared_ptr<DefaultExpression> node,
                    Scope<LocationPtr> *scope);
  void VisitAssign(std::shared_ptr<BinaryExpression> node,
                   Scope<LocationPtr> *scope);
  void VisitWhile(std::shared_ptr<WhileExpression> node,
                  Scope<LocationPtr> *scope);
  void VisitBlock(std::shared_ptr<BlockExpression> node,
                  Scope<LocationPtr> *outerScope);
  void VisitUnary(std::shared_ptr<UnaryExpression> node,
                  Scope<LocationPtr> *scope);
  void VisitBinary(std::shared_ptr<BinaryExpression> node,
                   Scope<LocationPtr> *scope);
  void VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                      Scope<LocationPtr> *scope);
  void VisitReturn(std::shared_ptr<ReturnExpression> node,
                   Scope<LocationPtr> *scope);
  void VisitConditional(std::shared_ptr<ConditionalExpression> node,
                        Scope<LocationPtr> *scope);
  void VisitInvocation(std::shared_ptr<InvocationExpression> node,
                       Scope<LocationPtr> *scope);
  void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                         Scope<LocationPtr> *scope);
  void VisitNewExpression(std::shared_ptr<NewExpression> node,
                          Scope<LocationPtr> *scope);
  void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                             Scope<LocationPtr> *scope);
  void VisitWhileExpression(std::shared_ptr<WhileExpression> node,
                            Scope<LocationPtr> *scope);
  void VisitPackage(std::shared_ptr<Package> package,
                    Scope<LocationPtr> *globalScope);

  void VisitClassInfo(std::shared_ptr<ClassInfo> info,
                      Scope<LocationPtr> *outerScope);
  void VisitModuleInfo(std::shared_ptr<ModuleInfo> info,
                       Scope<LocationPtr> *outerScope);
  void VisitMethodDef(const std::shared_ptr<MethodInfo> method,
                      Scope<LocationPtr> *outerScope);
  void VisitProgramFile(std::shared_ptr<SourceDocument> program,
                        Scope<LocationPtr> *outerScope);
  void VisitProject();
};

using ConstantSet =
    std::unordered_map<ConstantKind, std::unordered_set<std::u32string>>;

class ConstantCollector {
public:
  void VisitMethodDef(std::shared_ptr<MethodInfo> method,
                      ConstantSet &constantSet);
  void VisitPackage(std::shared_ptr<Package> package);
  void VisitProject(Project &project);
};

// class PackageImporter
// {
// public:
// 	void ImportPackages(Project& project);
// 	void CollectInfo(Project& project,
// 		std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ClassInfo>>>& classMap,
// 		std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ModuleInfo>>>& moduleMap,
// 		std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<InterfaceInfo>>>& interfaceMap, 		PackageRoute
// currentRoute, 		std::shared_ptr<Package> pkg, std::unordered_set<PackageRoute>&
// visited);
// };

} // namespace cygni

#endif // CYGNI_VISITOR_HPP
