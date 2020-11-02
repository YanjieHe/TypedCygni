#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Enum.hpp"
#include "Scope.hpp"
#include "Type.hpp"
#include <cstdint>
#include <list>
#include <memory>
#include "IJsonSerializable.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "Position.hpp"

#define BINARY_TYPE_MATCH(NODE_TYPE, LEFT_TYPE, RIGHT_TYPE, RETURN_TYPE)       \
  if (node->NodeType() == ExpressionType::NODE_TYPE &&                         \
      left->Equals(Type::LEFT_TYPE()) && right->Equals(Type::RIGHT_TYPE())) {  \
    return Type::RETURN_TYPE();                                                \
  }

#define UNARY_TYPE_MATCH(NODE_TYPE, OPERAND_TYPE, RETURN_TYPE)                 \
  if (node->NodeType() == ExpressionType::NODE_TYPE &&                         \
      operand->Equals(Type::OPERAND_TYPE())) {                                 \
    return Type::RETURN_TYPE();                                                \
  }

using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;
using Byte = uint8_t;
using nlohmann::json;

class Expression : public IJsonSerializable {
public:
  typedef shared_ptr<Expression> Ptr;

  virtual Position Pos() const = 0;
  virtual ExpressionType NodeType() const = 0;
};

/* Expressions */
class BinaryExpression : public Expression {
public:
  typedef shared_ptr<BinaryExpression> Ptr;

  Position pos;
  ExpressionType nodeType;
  Expression::Ptr left;
  Expression::Ptr right;

  BinaryExpression(Position pos, ExpressionType nodeType, Expression::Ptr left,
                   Expression::Ptr right)
      : pos{pos}, nodeType{nodeType}, left{left}, right{right} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  Json ToJson() const override;
};

class UnaryExpression : public Expression {
public:
  typedef shared_ptr<UnaryExpression> Ptr;

  Position pos;
  ExpressionType nodeType;
  Expression::Ptr operand;

  UnaryExpression(Position pos, ExpressionType nodeType,
                  Expression::Ptr operand)
      : pos{pos}, nodeType{nodeType}, operand{operand} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  Json ToJson() const override;
};

class ConstantExpression : public Expression {
public:
  typedef shared_ptr<ConstantExpression> Ptr;

  Position pos;
  ExpressionType nodeType;
  string value;

  ConstantExpression(Position pos, ExpressionType nodeType, string value)
      : pos{pos}, nodeType{nodeType}, value{value} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  Json ToJson() const override;
};

class InvocationExpression : public Expression {
public:
  typedef shared_ptr<InvocationExpression> Ptr;

  Position pos;
  Expression::Ptr function;
  vector<Expression::Ptr> args;

  InvocationExpression(Position pos, Expression::Ptr function,
                       vector<Expression::Ptr> args)
      : pos{pos}, function{function}, args{args} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::INVOKE; }
  Json ToJson() const override;
};

class IdentifierExpression : public Expression {
public:
  typedef shared_ptr<IdentifierExpression> Ptr;

  Position pos;
  string identifier;

  IdentifierExpression(Position pos, string identifier)
      : pos{pos}, identifier{identifier} {}
  Position Pos() const override { return pos; }
  Json ToJson() const override;
};

class MemberExpression : public Expression {
public:
  typedef shared_ptr<MemberExpression> Ptr;

  Position pos;
  Expression::Ptr object;
  string memberName;

  MemberExpression(Position pos, Expression::Ptr object, string memberName)
      : pos{pos}, object{object}, memberName{memberName} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::MEMBER; }
  Json ToJson() const override;
};

class NewExpression : public Expression {
public:
  typedef shared_ptr<NewExpression> Ptr;

  Position pos;
  string className;
  vector<Expression::Ptr> args;

  NewExpression(Position pos, string className, vector<Expression::Ptr> args)
      : pos{pos}, className{className}, args{args} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::NEW; }
  Json ToJson() const override;
};

class AsExpression : public Expression {
public:
  typedef shared_ptr<AsExpression> Ptr;

  Position pos;
  Expression::Ptr expression;
  Type::Ptr type;

  AsExpression(Position pos, Expression::Ptr expression, Type::Ptr type)
      : pos{pos}, expression{expression}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::AS; }
  Json ToJson() const override;
};

class IsExpression : public Expression {
public:
  typedef shared_ptr<IsExpression> Ptr;

  Position pos;
  Expression::Ptr expression;
  Type::Ptr type;

  IsExpression(Position pos, Expression::Ptr expression, Type::Ptr type)
      : pos{pos}, expression{expression}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::IS; }
  Json ToJson() const override;
};

class BlockExpression : public Expression {
public:
  typedef shared_ptr<BlockExpression> Ptr;

  Position pos;
  vector<Expression::Ptr> expressions;

  BlockExpression(Position pos, vector<Expression::Ptr> expressions)
      : pos{pos}, expressions{expressions} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::BLOCK; }
  Json ToJson() const override;
};

class ConditionalExpression : public Expression {
public:
  typedef shared_ptr<ConditionalExpression> Ptr;

  Position pos;
  BlockExpression::Ptr condition;
  BlockExpression::Ptr ifTrue;
  BlockExpression::Ptr ifFalse;

  ConditionalExpression(Position pos, BlockExpression::Ptr condition,
                        BlockExpression::Ptr ifTrue,
                        BlockExpression::Ptr ifFalse)
      : pos{pos}, condition{condition}, ifTrue{ifTrue}, ifFalse{ifFalse} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override {
    return ExpressionType::CONDITIONAL;
  }
  Json ToJson() const override;
};

class AssignExpression : public Expression {
public:
  typedef shared_ptr<AssignExpression> Ptr;

  Position pos;
  AssignmentKind kind;
  Expression::Ptr left;
  Expression::Ptr value;

  AssignExpression(Position pos, AssignmentKind kind, Expression::Ptr left,
                   Expression::Ptr value)
      : pos{pos}, kind{kind}, left{left}, value{value} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::ASSIGN; }
  Json ToJson() const override;
};

class ResetExpression : public Expression {
public:
  typedef shared_ptr<ResetExpression> Ptr;

  Position pos;
  BlockExpression::Ptr body;

  ResetExpression(Position pos, BlockExpression::Ptr body)
      : pos{pos}, body{body} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::RESET; }
  Json ToJson() const override;
};

class Parameter : public IJsonSerializable {
public:
  typedef shared_ptr<Parameter> Ptr;

  Position pos;
  string name;
  Type::Ptr type;

  Parameter() {}
  Parameter(Position pos, string name, Type::Ptr type)
      : pos{pos}, name{name}, type{type} {}
  Json ToJson() const override;
};

class ShiftExpression : public Expression {
public:
  typedef shared_ptr<ShiftExpression> Ptr;

  Position pos;
  Parameter::Ptr parameter;
  BlockExpression::Ptr body;

  ShiftExpression(Position pos, Parameter::Ptr parameter,
                  BlockExpression::Ptr body)
      : pos{pos}, parameter{parameter}, body{body} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::SHIFT; }
  Json ToJson() const override;
};

class LambdaExpression : public Expression {
public:
  typedef shared_ptr<LambdaExpression> Ptr;

  Position pos;
  vector<Parameter::Ptr> parameters;
  BlockExpression::Ptr body;

  LambdaExpression(Position pos, vector<Parameter::Ptr> parameters,
                   BlockExpression::Ptr body)
      : parameters{parameters}, body{body} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::LAMBDA; }
  Json ToJson() const override;
};

class CatchBlock : public IJsonSerializable {
public:
  typedef shared_ptr<CatchBlock> Ptr;

  Position pos;
  string variable;
  Type::Ptr test;
  BlockExpression::Ptr body;

  CatchBlock(Position pos, string variable, Type::Ptr test,
             BlockExpression::Ptr body)
      : pos{pos}, variable{variable}, test{test}, body{body} {}
  Json ToJson() const override;
};

class TryExpression : public Expression {
public:
  typedef shared_ptr<TryExpression> Ptr;

  Position pos;
  BlockExpression::Ptr body;
  vector<CatchBlock::Ptr> handlers;
  BlockExpression::Ptr finally;

  TryExpression(Position pos, BlockExpression::Ptr body,
                vector<CatchBlock::Ptr> handlers, BlockExpression::Ptr finally)
      : pos{pos}, body{body}, handlers{handlers}, finally{finally} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::TRY; }
  Json ToJson() const override;
};

class VarExpression : public Expression {
public:
  typedef shared_ptr<VarExpression> Ptr;

  Position pos;
  bool isMutable;
  string identifier;
  optional<Type::Ptr> optType;
  Expression::Ptr value;

  VarExpression(Position pos, bool isMutable, string identifier,
                optional<Type::Ptr> optType, Expression::Ptr value)
      : pos{pos}, identifier{identifier}, optType{optType}, value{value} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::VAR; }
  Json ToJson() const override;
};

class MethodDecl {
public:
  typedef shared_ptr<MethodDecl> Ptr;

  Position pos;
  bool isStatic;
  bool isVirtual;
  bool isOverride;
  bool isAbstract;
  bool isConstructor;
  string identifier;
  vector<Parameter::Ptr> parameters;
  Type::Ptr returnType;
  BlockExpression::Ptr body;
  vector<VarExpression::Ptr> localVariables;

  MethodDecl(Position pos, string identifier, bool isStatic, bool isVirtual,
             bool isOverride, bool isAbstract, bool isConstructor,
             vector<Parameter::Ptr> parameters, Type::Ptr returnType,
             BlockExpression::Ptr body)
      : pos{pos}, identifier{identifier}, parameters{parameters},
        returnType{returnType}, body{body} {}
  Position Pos() const { return pos; }
};

class FieldDecl {
public:
  typedef shared_ptr<FieldDecl> Ptr;

  Position pos;
  bool isStatic;
  string identifier;
  Type::Ptr type;

  FieldDecl(Position pos, bool isStatic, string identifier, Type::Ptr type)
      : pos{pos}, isStatic{isStatic}, identifier{identifier}, type{type} {}
  Position Pos() const { return pos; }
};

class ClassDecl {
public:
  typedef shared_ptr<ClassDecl> Ptr;

  Position pos;
  string identifier;
  bool isAbstract;
  bool isModule;
  bool isInterface;
  vector<string> extendList;
  vector<FieldDecl::Ptr> fields;
  vector<MethodDecl::Ptr> methods;
  vector<FieldDecl::Ptr> staticVariables;
  vector<MethodDecl::Ptr> staticFunctions;

  ClassDecl(Position pos, string identifier, bool isAbstract, bool isModule,
            bool isInterface, vector<string> extendList,
            vector<FieldDecl::Ptr> fields, vector<MethodDecl::Ptr> methods,
            vector<FieldDecl::Ptr> staticVariables,
            vector<MethodDecl::Ptr> staticFunctions)
      : pos{pos}, identifier{identifier}, isAbstract{isAbstract},
        isModule{isModule}, isInterface{isInterface}, extendList{extendList},
        fields{fields}, methods{methods}, staticVariables{staticVariables},
        staticFunctions{staticFunctions} {}
  Position Pos() const { return pos; }
};

// class MemberDeclaration
// {
// public:
//   virtual Position Pos() const = 0;
//   virtual MemberKind Kind() const = 0;
//   virtual AccessModifier GetAccessModifier() const = 0;
// };

// class FunctionInfo;

// class FieldMember : public MemberDeclaration
// {
// public:
//   Position pos;
//   AccessModifier accessModifier;
//   string name;
//   Expression *initializer;
//   int fieldIndex;
//   weak_ptr<ClassInfo> classInfo;

//   Position Pos() const override { return pos; }
//   MemberKind Kind() const override { return MemberKind::FIELD; }
//   AccessModifier GetAccessModifier() const override { return accessModifier;
//   }
// };

// class MethodMember : public MemberDeclaration
// {
// public:
//   Position pos;
//   AccessModifier accessModifier;
//   bool isConstructor;
//   bool isAbstract;
//   bool isVirtual;
//   bool isOverride;
//   int methodIndex;
//   FunctionType functionType; // 'this' is the first parameter
//   ClassInfo *classInfo;
//   FunctionInfo *functionInfo;

//   Position Pos() const override { return pos; }
//   MemberKind Kind() const override { return MemberKind::METHOD; }
//   AccessModifier GetAccessModifier() const override { return accessModifier;
//   }
// };

// class ClassInfo
// {
// public:
//   bool isAbstract;
//   bool isInterface;
//   string packageName;
//   string name;
//   vector<ClassInfo *> extendList;
//   ClassInfo *superClass;
//   vector<ClassInfo *> interfaceList;
//   vector<FieldMember *> fields;
//   vector<MethodMember *> methods;
//   vector<FieldMember *> staticVariables;
//   vector<MethodMember *> staticFunctions;
// };

// class FunctionInfo
// {
// public:
//   Type* type;
//   string packageName;
//   string name;
//   vector<Parameter> parameters;
//   shared_ptr<BlockStatement> block;
//   vector<shared_ptr<VarDeclStatement>> localVariables;
// };

class PackagePath {
public:
  vector<string> path;

  PackagePath() : path{} {}
  PackagePath(vector<string> path);

  string ToString() const;
};

class Import : public IJsonSerializable {
public:
  typedef shared_ptr<Import> Ptr;

  Position pos;
  PackagePath packageName;

  Import(Position pos, PackagePath packageName)
      : pos{pos}, packageName{packageName} {}

  Json ToJson() const override;
};

class Rename {
public:
  typedef shared_ptr<Rename> Ptr;

  Position pos;
  PackagePath packageName;
  string originalName;
  string newName;

  Rename(Position pos, PackagePath packageName, string originalName,
         string newName)
      : pos{pos}, packageName{packageName}, originalName{originalName},
        newName{newName} {}
};

class CodeFile {
public:
  typedef shared_ptr<CodeFile> Ptr;

  string path;
  string packageName;
  vector<Import::Ptr> importList;
  vector<Rename::Ptr> renameList;
  vector<ClassDecl::Ptr> classList;

  CodeFile(string path, string packageName, vector<Import::Ptr> importList,
           vector<Rename::Ptr> renameList, vector<ClassDecl::Ptr> classList)
      : path{path}, packageName{packageName}, importList{importList},
        renameList{renameList}, classList{classList} {}
};

class Program {
public:
  typedef shared_ptr<Program> Ptr;

  vector<CodeFile::Ptr> codeFiles;

  explicit Program(vector<CodeFile::Ptr> codeFiles) : codeFiles{codeFiles} {}
};

class Error {
public:
  Position pos;
  string message;
  Error(Position pos, string message) : pos{pos}, message{message} {}
};

template <typename ReturnType, typename... ArgTypes> class Visitor {
public:
  virtual ReturnType VisitExpression(Expression::Ptr node, ArgTypes... args) {
    switch (node->NodeType()) {
    case ExpressionType::INT:
    case ExpressionType::LONG:
    case ExpressionType::FLOAT:
    case ExpressionType::DOUBLE:
    case ExpressionType::BOOLEAN:
    case ExpressionType::CHAR:
    case ExpressionType::STRING:
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    case ExpressionType::ADD:
    case ExpressionType::SUBTRACT:
    case ExpressionType::MULTIPLY:
    case ExpressionType::DIVIDE:
    case ExpressionType::MODULO:
    case ExpressionType::GT:
    case ExpressionType::LT:
    case ExpressionType::GE:
    case ExpressionType::LE:
    case ExpressionType::EQ:
    case ExpressionType::NE:
    case ExpressionType::LOGICAL_AND:
    case ExpressionType::LOGICAL_OR:
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    case ExpressionType::LOGICAL_NOT:
    case ExpressionType::PLUS:
    case ExpressionType::MINUS:
      return VisitUnary(static_pointer_cast<UnaryExpression>(node), args...);
    case ExpressionType::INVOKE:
      return VisitInvocation(static_pointer_cast<InvocationExpression>(node),
                             args...);
    case ExpressionType::IDENTIFIER:
      return VisitIdentifier(static_pointer_cast<IdentifierExpression>(node),
                             args...);
    case ExpressionType::AS:
      return VisitAs(static_pointer_cast<AsExpression>(node), args...);
    case ExpressionType::IS:
      return VisitIs(static_pointer_cast<IsExpression>(node), args...);
    case ExpressionType::MEMBER:
      return VisitMember(static_pointer_cast<MemberExpression>(node), args...);
    case ExpressionType::NEW:
      return VisitNew(static_pointer_cast<NewExpression>(node), args...);
    default:
      throw Error(node->Pos(), "unsupported node type for visitor");
    }
  }
  virtual ReturnType VisitBinary(BinaryExpression::Ptr node,
                                 ArgTypes... args) = 0;
  virtual ReturnType VisitConstant(ConstantExpression::Ptr node,
                                   ArgTypes... args) = 0;
  virtual ReturnType VisitUnary(UnaryExpression::Ptr node,
                                ArgTypes... args) = 0;
  virtual ReturnType VisitInvocation(InvocationExpression::Ptr node,
                                     ArgTypes... args) = 0;
  virtual ReturnType VisitIdentifier(IdentifierExpression::Ptr node,
                                     ArgTypes... args) = 0;
  virtual ReturnType VisitAs(AsExpression::Ptr node, ArgTypes... args) = 0;
  virtual ReturnType VisitIs(IsExpression::Ptr node, ArgTypes... args) = 0;
  virtual ReturnType VisitMember(MemberExpression::Ptr node,
                                 ArgTypes... args) = 0;
  virtual ReturnType VisitNew(NewExpression::Ptr node, ArgTypes... args) = 0;
  virtual ReturnType VisitConditional(ConditionalExpression::Ptr node,
                                      ArgTypes... args) = 0;
  virtual ReturnType VisitBlock(BlockExpression::Ptr node,
                                ArgTypes... args) = 0;
  virtual ReturnType VisitAssign(AssignExpression::Ptr node,
                                 ArgTypes... args) = 0;
  virtual ReturnType VisitVar(VarExpression::Ptr node, ArgTypes... args) = 0;
};

// template <typename... ArgTypes>
// class NanoPass : public Visitor<ExpPtr, StatementPtr, ArgTypes...> {
//   virtual ExpPtr VisitConstant(shared_ptr<ConstantExpression> node,
//                                ArgTypes... args) {
//     return node;
//   }
//   virtual ExpPtr VisitBinary(shared_ptr<BinaryExpression> node,
//                              ArgTypes... args) {
//     auto left = VisitExpression(node->left, args...);
//     auto right = VisitExpression(node->right, args...);
//     return make_shared<BinaryExpression>(node->Pos(), node->NodeType(), left,
//                                          right);
//   }
//   virtual ExpPtr VisitUnary(shared_ptr<UnaryExpression> node,
//                             ArgTypes... args) {
//     auto operand = VisitExpression(node->operand, args...);
//     return make_shared<UnaryExpression>(node->Pos(), node->NodeType(),
//     operand);
//   }
//   virtual ExpPtr VisitInvocation(shared_ptr<InvocationExpression> node,
//                                  ArgTypes... args) {
//     auto function = VisitExpression(node->function, args...);
//     auto arguments =
//         Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
//           return VisitExpression(arg, args...);
//         });
//     return make_shared<InvocationExpression>(node->Pos(), function,
//     arguments);
//   }
//   virtual ExpPtr VisitIdentifier(shared_ptr<IdentifierExpression> node,
//                                  ArgTypes... args) {
//     return node;
//   }
//   virtual ExpPtr VisitConversion(shared_ptr<ConversionExpression> node,
//                                  ArgTypes... args) {
//     auto expression = VisitExpression(node, args...);
//     return make_shared<ConversionExpression>(node->Pos(), expression,
//                                              node->type);
//   }
//   virtual ExpPtr VisitMember(shared_ptr<MemberExpression> node,
//                              ArgTypes... args) {
//     auto object = VisitExpression(node->object, args...);
//     return make_shared<MemberExpression>(node->Pos(), object, node->name);
//   }
//   virtual ExpPtr VisitNew(shared_ptr<NewExpression> node, ArgTypes... args) {
//     auto arguments =
//         Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
//           return VisitExpression(arg, args...);
//         });
//     return make_shared<NewExpression>(node->Pos(), node->constructor,
//                                       arguments);
//   }
//   virtual StatementPtr VisitIfThen(shared_ptr<IfThenStatement> node,
//                                    ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto ifTrue = VisitStatement(node->ifTrue, args...);
//     return make_shared<IfThenStatement>(node->Pos(), condition, ifTrue);
//   }
//   virtual StatementPtr VisitIfElse(shared_ptr<IfElseStatement> node,
//                                    ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto ifTrue = VisitStatement(node->ifTrue, args...);
//     auto ifFalse = VisitStatement(node->ifFalse, args...);
//     return make_shared<IfElseStatement>(node->Pos(), condition, ifTrue,
//                                         ifFalse);
//   }
//   virtual StatementPtr VisitWhile(shared_ptr<WhileStatement> node,
//                                   ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto body = VisitStatement(node->body, args...);
//     return make_shared<WhileStatement>(node->Pos(), condition, body);
//   }
//   virtual StatementPtr VisitBlock(shared_ptr<BlockStatement> node,
//                                   ArgTypes... args) {
//     auto statements = Vec::Map<StatementPtr, StatementPtr>(
//         node->statements, [&](StatementPtr x) -> StatementPtr {
//           return VisitStatement(x, args...);
//         });
//     return make_shared<BlockStatement>(node->Pos(), statements);
//   }
//   virtual StatementPtr VisitReturn(shared_ptr<ReturnStatement> node,
//                                    ArgTypes... args) {
//     auto value = VisitExpression(node->value, args...);
//     return make_shared<ReturnStatement>(node->Pos(), value);
//   }
//   virtual StatementPtr VisitAssign(shared_ptr<AssignStatement> node,
//                                    ArgTypes... args) {
//     auto left = VisitExpression(node->left, args...);
//     auto value = VisitExpression(node->value, args...);
//     return make_shared<AssignStatement>(node->Pos(), node->kind, left,
//     value);
//   }
//   virtual StatementPtr VisitVarDecl(shared_ptr<VarDeclStatement> node,
//                                     ArgTypes... args) {
//     if (node->value.has_value()) {
//       auto value = VisitExpression(node->value.value(), args...);
//       return make_shared<VarDeclStatement>(node->Pos(), node->identifier,
//                                            node->type, value);
//     } else {
//       return make_shared<VarDeclStatement>(node->Pos(), node->identifier,
//                                            node->type,
//                                            make_shared<ExpPtr>());
//     }
//   }
//   virtual StatementPtr VisitExpStatement(shared_ptr<Expression> node,
//                                          ArgTypes... args) {
//     return VisitExpression(node, args...);
//   }
// };

class ConstantMap {
public:
  int N;
  unordered_map<ExpressionType, unordered_map<string, int>> table;

  ConstantMap() : N{0}, table() {}

  void Add(ExpressionType type, string key) {
    if (table.count(type)) {
      if (table[type].count(key)) {
        /* pass */
      } else {
        table[type][key] = N;
        N = N + 1;
      }
    } else {
      table[type][key] = N;
      N++;
    }
  }

  int Get(ExpressionType type, string key) {
    if (table.count(type)) {
      if (table[type].count(key)) {
        return table[type][key];
      } else {
        table[type][key] = N;
        N = N + 1;
        return table[type][key];
      }
    } else {
      table[type][key] = N;
      N = N + 1;
      return table[type][key];
    }
  }

  void Clear() {
    table.clear();
    N = 0;
  }
};

class ByteCode {
public:
  vector<Byte> bytes;
  void Add(OpCode op) { bytes.push_back(static_cast<Byte>(op)); }
};

// class TypeChecker : public Visitor<Type::Ptr, Scope<Type::Ptr>::Ptr> {
// public:
//   ByteCode bytes;
//   ConstantMap constantMap;

//   Type::Ptr VisitConstant(ConstantExpression *node,
//                           Scope<Type::Ptr>::Ptr scope) override {
//     constantMap.Get(node->NodeType(), node->value);
//     switch (node->NodeType()) {
//     case ExpressionType::INT: {
//       bytes.Add(OpCode::PUSH_I32);
//       return Type::Int();
//     }
//     case ExpressionType::BOOLEAN: {
//       bytes.Add(OpCode::PUSH_I32);
//       return Type::Boolean();
//     }
//     case ExpressionType::LONG: {
//       bytes.Add(OpCode::PUSH_I64);
//       return Type::Long();
//     }
//     case ExpressionType::DOUBLE: {
//       bytes.Add(OpCode::PUSH_F64);
//       return Type::Double();
//     }
//     case ExpressionType::CHAR: {
//       bytes.Add(OpCode::PUSH_I32);
//       return Type::Char();
//     }
//     case ExpressionType::STRING: {
//       bytes.Add(OpCode::PUSH_STRING);
//       return Type::String();
//     }
//     default:
//       throw Error(node->Pos(), "error constant node type");
//     }
//   }

//   Type *VisitBinary(BinaryExpression *node, ScopePtr<Type *> scope) override
//   {
//     auto left = VisitExpression(node->left, scope);
//     auto right = VisitExpression(node->right, scope);

//     BINARY_TYPE_MATCH(ADD, Int, Int, Int);
//     BINARY_TYPE_MATCH(ADD, Long, Long, Long);
//     BINARY_TYPE_MATCH(ADD, Float, Float, Float);
//     BINARY_TYPE_MATCH(ADD, Double, Double, Double);

//     BINARY_TYPE_MATCH(SUBTRACT, Int, Int, Int);
//     BINARY_TYPE_MATCH(SUBTRACT, Long, Long, Long);
//     BINARY_TYPE_MATCH(SUBTRACT, Float, Float, Float);
//     BINARY_TYPE_MATCH(SUBTRACT, Double, Double, Double);

//     BINARY_TYPE_MATCH(MULTIPLY, Int, Int, Int);
//     BINARY_TYPE_MATCH(MULTIPLY, Long, Long, Long);
//     BINARY_TYPE_MATCH(MULTIPLY, Float, Float, Float);
//     BINARY_TYPE_MATCH(MULTIPLY, Double, Double, Double);

//     BINARY_TYPE_MATCH(DIVIDE, Int, Int, Int);
//     BINARY_TYPE_MATCH(DIVIDE, Long, Long, Long);
//     BINARY_TYPE_MATCH(DIVIDE, Float, Float, Float);
//     BINARY_TYPE_MATCH(DIVIDE, Double, Double, Double);

//     BINARY_TYPE_MATCH(MODULO, Int, Int, Int);
//     BINARY_TYPE_MATCH(MODULO, Long, Long, Long);

//     BINARY_TYPE_MATCH(GT, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(GT, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(GT, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(GT, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(GT, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(LT, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(LT, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(LT, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(LT, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(LT, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(GE, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(GE, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(GE, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(GE, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(GE, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(LE, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(LE, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(LE, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(LE, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(LE, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(EQ, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(EQ, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(EQ, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(EQ, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(EQ, Boolean, Boolean, Boolean);
//     BINARY_TYPE_MATCH(EQ, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(NE, Int, Int, Boolean);
//     BINARY_TYPE_MATCH(NE, Long, Long, Boolean);
//     BINARY_TYPE_MATCH(NE, Float, Float, Boolean);
//     BINARY_TYPE_MATCH(NE, Double, Double, Boolean);
//     BINARY_TYPE_MATCH(NE, Boolean, Boolean, Boolean);
//     BINARY_TYPE_MATCH(NE, Char, Char, Boolean);

//     BINARY_TYPE_MATCH(LOGICAL_AND, Boolean, Boolean, Boolean);
//     BINARY_TYPE_MATCH(LOGICAL_OR, Boolean, Boolean, Boolean);
//     throw Error(node->Pos(), "binary operation type mismatch");
//   }
//   Type *VisitUnary(UnaryExpression *node, ScopePtr<Type *> scope) override {
//     auto operand = VisitExpression(node->operand, scope);

//     UNARY_TYPE_MATCH(PLUS, Int, Int);
//     UNARY_TYPE_MATCH(PLUS, Long, Long);
//     UNARY_TYPE_MATCH(PLUS, Float, Float);
//     UNARY_TYPE_MATCH(PLUS, Double, Double);

//     UNARY_TYPE_MATCH(MINUS, Int, Int);
//     UNARY_TYPE_MATCH(MINUS, Long, Long);
//     UNARY_TYPE_MATCH(MINUS, Float, Float);
//     UNARY_TYPE_MATCH(MINUS, Double, Double);

//     UNARY_TYPE_MATCH(LOGICAL_NOT, Boolean, Boolean);

//     throw Error(node->Pos(), "unary operation type mismtach");
//   }

//   Type *VisitInvocation(InvocationExpression *node,
//                         ScopePtr<Type *> scope) override {
//     auto funcType = VisitExpression(node->function, scope);
//     if (funcType->GetTypeCode() == TypeCode::METHOD) {
//       return VisitMethodCall(node, dynamic_cast<MethodType *>(funcType),
//       scope);
//     } else {
//       return VisitFunctionCall(node, dynamic_cast<FunctionType *>(funcType),
//                                scope);
//     }
//   }
//   Type *VisitMethodCall(InvocationExpression *node, MethodType *methodType,
//                         ScopePtr<Type *> scope) {
//     auto args = Vec::Map<Expression *, Type *>(
//         node->args,
//         [&](Expression *e) -> Type * { return VisitExpression(e, scope); });
//     if (Vec::SequenceEqual(
//             methodType->func->args, args,
//             [](Type *x, Type *y) -> bool { return x->Equals(y); })) {
//       return methodType->func->ret;
//     } else {
//       throw Error(node->Pos(), "method arguments' type mismtach");
//     }
//   }
//   Type *VisitFunctionCall(InvocationExpression *node, FunctionType *function,
//                           ScopePtr<Type *> scope) {
//     auto args = Vec::Map<Expression *, Type *>(
//         node->args,
//         [&](Expression *e) -> Type * { return VisitExpression(e, scope); });
//     if (function->GetTypeCode() == TypeCode::FUNCTION) {
//       auto ft = dynamic_cast<FunctionType *>(function);
//       if (Vec::SequenceEqual(ft->args, args, [](Type *x, Type *y) -> bool {
//             return x->Equals(y);
//           })) {
//         return ft->ret;
//       } else {
//         throw Error(node->Pos(), "function arguments' type mismtach");
//       }
//     } else {
//       throw Error(node->Pos(), "caller is not a function");
//     }
//   }
//   Type *VisitIdentifier(IdentifierExpression *node,
//                         ScopePtr<Type *> scope) override {
//     if (auto value = scope->Get(node->identifier)) {
//       return value.value();
//     } else {
//       throw Error(node->Pos(), "identifier not defined");
//     }
//   }
//   Type *VisitAs(AsExpression *node, ScopePtr<Type *> scope) {
//     // TO DO
//     auto expression = VisitExpression(node->expression, scope);
//     return nullptr;
//   }
//   Type *VisitIs(IsExpression *node, ScopePtr<Type *> scope) {
//     // TO DO
//     auto expression = VisitExpression(node->expression, scope);
//     return nullptr;
//   }
//   Type *VisitMember(MemberExpression *node, ScopePtr<Type *> scope) override
//   {
//     auto object = VisitExpression(node->object, scope);
//     if (object->GetTypeCode() == TypeCode::OBJECT) {
//       auto objType = dynamic_cast<ObjectType *>(object);
//       if (objType->fields.count(node->memberName)) {
//         return objType->fields[node->memberName];
//       } else {
//         throw Error(node->Pos(), "member exp: field doesn't exist");
//       }
//     } else {
//       throw Error(node->Pos(), "member exp: not object type");
//     }
//   }
//   Type *VisitNew(NewExpression *node, ScopePtr<Type *> scope) override {
//     return nullptr;
//   }
//   monostate VisitIfThen(IfThenStatement *node,
//                         ScopePtr<Type *> scope) override {
//     auto condition = VisitExpression(node->condition, scope);
//     if (condition->Equals(Type::Boolean())) {
//       auto subScope = make_shared<Scope<Type *>>(scope);
//       VisitBlock(node->ifTrue, subScope);
//     } else {
//       throw Error(node->pos, "if then: condition type must be boolean");
//     }
//   }
//   monostate VisitIfElse(IfElseStatement *node,
//                         ScopePtr<Type *> scope) override {
//     auto condition = VisitExpression(node->condition, scope);
//     if (condition->Equals(Type::Boolean())) {
//       auto subScope = make_shared<Scope<Type *>>(scope);
//       VisitBlock(node->ifTrue, subScope);
//       subScope = make_shared<Scope<Type *>>(scope);
//       VisitBlock(node->ifFalse, subScope);
//     } else {
//       throw Error(node->pos, "if then: condition type must be boolean");
//     }
//   }
//   monostate VisitWhile(WhileStatement *node, ScopePtr<Type *> scope) override
//   {
//     auto condition = VisitExpression(node->condition, scope);
//     if (condition->Equals(Type::Boolean())) {
//       auto subScope = make_shared<Scope<Type *>>(scope);
//       VisitBlock(node->body, subScope);
//     } else {
//       throw Error(node->pos, "if then: condition type must be boolean");
//     }
//   }
//   monostate VisitBlock(BlockStatement *node, ScopePtr<Type *> scope) override
//   {
//     for (auto statement : node->statements) {
//       VisitStatement(statement, scope);
//     }
//     return monostate();
//   }
//   monostate VisitReturn(ReturnStatement *node,
//                         ScopePtr<Type *> scope) override {
//     VisitExpression(node->value, scope);
//     return monostate();
//   }
//   monostate VisitAssign(AssignStatement *node,
//                         ScopePtr<Type *> scope) override {
//     return monostate();
//   }
//   monostate VisitVarDecl(VarDeclStatement *node,
//                          ScopePtr<Type *> scope) override {
//     return monostate();
//   }
//   monostate VisitMethodDecl(MethodDeclStatement *node,
//                             ScopePtr<Type *> scope) override {
//     return monostate();
//   }
//   monostate VisitExpStatement(Expression *node, ScopePtr<Type *> scope) {
//     VisitExpression(node, scope);
//     return monostate();
//   }
// };

// class AstJsonSerializer : public Visitor<json, json> {
// public:
//   typedef unordered_map<string, json> JsonMap;

//   json VisitBinary(BinaryExpression *node) override {
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Left", VisitExpression(node->left)},
//          {"Right", VisitExpression(node->right)}});
//   }
//   json VisitConstant(ConstantExpression *node) override {
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Value", node->value}});
//   }
//   json VisitUnary(UnaryExpression *node) override {
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Operand", VisitExpression(node->operand)}});
//   }
//   json VisitInvocation(InvocationExpression *node) override {
//     vector<json> args;
//     for (auto arg : node->args) {
//       args.push_back(VisitExpression(arg));
//     }
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Function", VisitExpression(node->function)},
//          {"Arguments", args}});
//   }
//   json VisitIdentifier(IdentifierExpression *node) override {
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Identifier", node->identifier}});
//   }
//   json VisitAs(AsExpression *node) override {
//     return JsonMap({
//         {"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//     });
//   }
//   json VisitIs(IsExpression *node) override {
//     return JsonMap({{"Expression Type",
//                      Enum<ExpressionType>::ToString(node->NodeType())}});
//   }
//   json VisitMember(MemberExpression *node) override {
//     return JsonMap(
//         {{"Expression Type",
//         Enum<ExpressionType>::ToString(node->NodeType())},
//          {"Object", VisitExpression(node->object)},
//          {"Member Name", node->memberName}});
//   }
//   json VisitNew(NewExpression *node) override { return json(); }
//   json VisitIfThen(IfThenStatement *node) override { return json(); }
//   json VisitIfElse(IfElseStatement *node) override { return json(); }
//   json VisitWhile(WhileStatement *node) override { return json(); }
//   json VisitBlock(BlockStatement *node) override {
//     vector<json> statements;
//     for (auto statement : node->statements) {
//       statements.push_back(VisitStatement(statement));
//     }
//     return JsonMap({{"Statement Type",
//                      Enum<StatementType>::ToString(node->GetStatementType())},
//                     {"Statements", statements}});
//   }
//   json VisitReturn(ReturnStatement *node) override {
//     return JsonMap({{"Statement Type",
//                      Enum<StatementType>::ToString(node->GetStatementType())},
//                     {"Value", VisitExpression(node->value)}});
//   }
//   json VisitBreak(BreakStatement *node) override { return json(); }
//   json VisitAssign(AssignStatement *node) override { return json(); }
//   json VisitVarDecl(VarDeclStatement *node) override {
//     return JsonMap({{"Statement Type",
//                      Enum<StatementType>::ToString(node->GetStatementType())},
//                     {"Identifier", node->identifier}});
//   }
//   json VisitMethodDecl(MethodDeclStatement *node) override {
//     return JsonMap({{"Identifier", node->identifier},
//                     {"Body", VisitStatement(node->body)}});
//   }
//   json VisitExpStatement(Expression *node) override {
//     return VisitExpression(node);
//   }
//   json VisitClassDecl(ClassDeclStatement *node) override {
//     vector<json> staticFunctions;
//     for (auto function : node->staticFunctions) {
//       staticFunctions.push_back(VisitMethodDecl(function));
//     }
//     return JsonMap({{"Statement Type",
//                      Enum<StatementType>::ToString(node->GetStatementType())},
//                     {"Static Functions", staticFunctions}});
//   }
// };

class SyntaxError {
public:
  string path;
  Position pos;
  string message;
  SyntaxError() : path(), pos(), message() {}
  SyntaxError(string path, Position pos, string message)
      : path{path}, pos{pos}, message{message} {}
  string ToString() const {
    return path + ":" + std::to_string(pos.startLine + 1) + ":" +
           std::to_string(pos.startCol + 1) + ": " + message;
  }
};

#endif // EXPRESSION_HPP