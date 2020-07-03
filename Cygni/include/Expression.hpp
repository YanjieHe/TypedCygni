#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "Enum.hpp"
#include "Location.hpp"
#include "Scope.hpp"
#include "SourcePosition.hpp"
#include "Type.hpp"
#include <iostream>

namespace cygni {

class Expression;
using ExpPtr = std::shared_ptr<Expression>;
using ExpList = std::vector<std::shared_ptr<Expression>>;

class Expression {
public:
  int id;
  SourcePosition position;
  ExpressionType nodeType;
  TypePtr type;

  Expression(SourcePosition position, ExpressionType nodeType);
};

class ConstantExpression : public Expression {
public:
  std::u32string constant;
  ConstantExpression(SourcePosition position, TypePtr type,
                     std::u32string constant);
};

class BinaryExpression : public Expression {
public:
  ExpPtr left;
  ExpPtr right;

  BinaryExpression(SourcePosition position, ExpressionType nodeType,
                   ExpPtr left, ExpPtr right);
};

class UnaryExpression : public Expression {
public:
  ExpPtr operand;

  UnaryExpression(SourcePosition position, ExpressionType nodeType,
                  ExpPtr operand);
};

class BlockExpression : public Expression {
public:
  ExpList expressions;
  BlockExpression(SourcePosition position, ExpList expressions);
};

class ConditionalExpression : public Expression {
public:
  ExpPtr condition;
  ExpPtr ifTrue;
  ExpPtr ifFalse;

  ConditionalExpression(SourcePosition position, ExpPtr condition,
                        ExpPtr ifTrue, ExpPtr ifFalse);
};

class DefaultExpression : public Expression {
public:
  DefaultExpression(SourcePosition position, TypePtr type);
};

class Argument {
public:
  std::optional<std::u32string> name;
  ExpPtr value;
  std::optional<int> index; // parameter index

  Argument() = default;
  explicit Argument(ExpPtr value);
  Argument(std::u32string name, ExpPtr value);
};

class InvocationExpression : public Expression {
public:
  ExpPtr expression;
  std::vector<Argument> arguments;
  InvocationExpression(SourcePosition position, ExpPtr expression,
                       std::vector<Argument> arguments);
};

class ParameterExpression : public Expression {
public:
  std::u32string name;
  LocationPtr location;
  ParameterExpression(SourcePosition position, std::u32string name,
                      TypePtr type);
};

class VarDefExpression : public Expression {
public:
  std::shared_ptr<ParameterExpression> variable;
  ExpPtr value;

  VarDefExpression(SourcePosition position,
                   std::shared_ptr<ParameterExpression> variable, TypePtr type,
                   ExpPtr value);
};

class AnnotationInfo {
public:
  SourcePosition position;
  std::u32string name;
  std::vector<Argument> arguments;

  AnnotationInfo(SourcePosition position, std::u32string name,
                 std::vector<Argument> arguments);
};

class FieldInfo {
public:
  SourcePosition position;
  AccessModifier modifier;
  bool isStatic;
  Table<std::u32string, AnnotationInfo> annotations;
  std::u32string name;
  TypePtr type;
  ExpPtr value;
  std::optional<int> index;
  FieldInfo() = default;
  FieldInfo(SourcePosition position, AccessModifier modifier, bool isStatic,
            Table<std::u32string, AnnotationInfo> annotations,
            std::u32string name, TypePtr type, ExpPtr value);
};

class MethodInfo {
public:
  SourcePosition position;
  AccessModifier modifier;
  bool isStatic;
  TypePtr selfType;
  Table<std::u32string, AnnotationInfo> annotations;
  std::u32string name;
  std::vector<std::shared_ptr<ParameterExpression>> parameters;
  TypePtr returnType;
  ExpPtr body;
  TypePtr signature;
  std::vector<std::shared_ptr<VarDefExpression>> localVariables;

  MethodInfo() = default;
  MethodInfo(SourcePosition position, AccessModifier modifier, bool isStatic,
             TypePtr selfType,
             Table<std::u32string, AnnotationInfo> annotations,
             std::u32string name,
             std::vector<std::shared_ptr<ParameterExpression>> parameters,
             TypePtr returnType, ExpPtr body);
};

class MemberAccessExpression : public Expression {
public:
  ExpPtr object;
  std::u32string field;
  LocationPtr location;
  MemberAccessExpression(SourcePosition position, ExpPtr object,
                         std::u32string field);
};

// class MethodCallExpression : public Expression
//{
// public:
//	ExpPtr object;
//	std::u32string method;
//	std::vector<Argument> arguments;
//	LocationPtr location;
//	MethodCallExpression(SourcePosition position, ExpPtr object,
//		std::u32string method, std::vector<Argument> arguments);
//};

class ReturnExpression : public Expression {
public:
  ExpPtr value;
  ReturnExpression(SourcePosition position, ExpPtr value);
};

class BreakExpression : public Expression {
public:
  BreakExpression(SourcePosition position);
};

class WhileExpression : public Expression {
public:
  ExpPtr condition;
  ExpPtr body;
  WhileExpression(SourcePosition position, ExpPtr condition, ExpPtr body);
};

class NewExpression : public Expression {
public:
  std::vector<Argument> arguments;
  LocationPtr location;
  NewExpression(SourcePosition position, TypePtr type,
                std::vector<Argument> arguments);
};

class VirtualMethods {
public:
  FullQualifiedName className;
  std::vector<FullQualifiedName> methodNames;
};

using VirtualTable = std::vector<VirtualMethods>;

using ConstantMap =
    std::unordered_map<ConstantKind, std::unordered_map<std::u32string, int>>;

class ClassInfo {
public:
  SourcePosition position;
  PackageRoute route;
  std::u32string name;

  /* definitions */
  Table<std::u32string, std::shared_ptr<FieldInfo>> fieldDefs;
  Table<std::u32string, std::shared_ptr<MethodInfo>> methodDefs;

  /* all information */
  Table<std::u32string, std::shared_ptr<FieldInfo>> fields;
  Table<std::u32string, std::shared_ptr<MethodInfo>> methods;

  std::vector<TypePtr> superTypes;
  ConstantMap constantMap;

  std::vector<std::shared_ptr<ClassType>> inheritanceChain;
  std::vector<std::shared_ptr<InterfaceType>> interfaceList;

  VirtualTable virtualTable;

  ClassInfo() = default;
  ClassInfo(SourcePosition position, PackageRoute route, std::u32string name);
};

class ModuleInfo {
public:
  SourcePosition position;
  PackageRoute route;
  std::u32string name;
  Table<std::u32string, std::shared_ptr<FieldInfo>> fields;
  Table<std::u32string, std::shared_ptr<MethodInfo>> methods;
  ConstantMap constantMap;
  ModuleInfo() = default;
  ModuleInfo(SourcePosition position, PackageRoute route, std::u32string name);
};

class InterfaceInfo {
public:
  SourcePosition position;
  PackageRoute route;
  std::u32string name;
  Table<std::u32string, std::shared_ptr<MethodInfo>> methodDefs;
  std::vector<TypePtr> superInterfaces;

  std::vector<std::shared_ptr<InterfaceType>> allSuperInterfaces;
  Table<std::u32string, std::shared_ptr<MethodInfo>> allMethods;

  VirtualTable virtualTable;

  InterfaceInfo() = default;
  InterfaceInfo(SourcePosition position, PackageRoute route,
                std::u32string name);
};

class TypeAlias {
public:
  SourcePosition position;
  PackageRoute route;
  std::u32string typeName;
  std::u32string alias;

  TypeAlias() = default;
  TypeAlias(SourcePosition position, PackageRoute route,
            std::u32string typeName, std::u32string alias);
};

class PackageRouteStatement {
public:
  SourcePosition position;
  PackageRoute route;
  PackageRouteStatement() = default;
  PackageRouteStatement(SourcePosition position, PackageRoute route);
};

class ImportStatement {
public:
  SourcePosition position;
  PackageRoute route;

  ImportStatement();
  ImportStatement(SourcePosition position, PackageRoute route);
};

class TemplateClass {
public:
  std::shared_ptr<ClassInfo> classInfo;
  std::vector<std::shared_ptr<TypeParameter>> parameters;

  TemplateClass(std::shared_ptr<ClassInfo> classInfo, std::vector<std::shared_ptr<TypeParameter>> parameters);
};

class Package {
public:
  PackageRoute route;

  std::unordered_map<std::u32string, std::shared_ptr<ClassInfo>> classes;
  std::unordered_map<std::u32string, std::shared_ptr<ModuleInfo>> modules;
  std::unordered_map<std::u32string, std::shared_ptr<InterfaceInfo>> interfaces;

  std::unordered_map<std::u32string, std::shared_ptr<TemplateClass>> templateClasses;

  explicit Package(PackageRoute route);
};

class SourceDocument {
public:
  std::shared_ptr<FileLocation> fileLocation;
  PackageRouteStatement packageRoute;
  std::vector<ImportStatement> importedPackages;
  Table<std::u32string, TypeAlias> typeAliases;
  std::unordered_map<std::u32string, std::shared_ptr<ClassInfo>> classDefs;
  std::unordered_map<std::u32string, std::shared_ptr<ModuleInfo>> moduleDefs;
  std::unordered_map<std::u32string, std::shared_ptr<InterfaceInfo>>
      interfaceDefs;

  std::unordered_map<std::u32string, std::shared_ptr<TemplateClass>> templateClassDefs;

  explicit SourceDocument(std::shared_ptr<FileLocation> document);
};

class Project {
public:
  std::unordered_map<std::string, std::shared_ptr<SourceDocument>> programs;
  std::unordered_map<PackageRoute, std::shared_ptr<Package>> packages;
  TypeGraph typeGraph;

  std::optional<std::shared_ptr<ModuleInfo>>
  GetModule(std::shared_ptr<ModuleType> moduleType);
  std::optional<std::shared_ptr<ClassInfo>>
  GetClass(std::shared_ptr<ClassType> classType);
  std::optional<std::shared_ptr<InterfaceInfo>>
  GetInterface(std::shared_ptr<InterfaceType> interfaceType);
  std::optional<TypePtr>
  ResolveType(std::shared_ptr<UnresolvedType> unresolvedType);
};

} // namespace cygni

#endif // CYGNI_EXPRESSION_HPP
