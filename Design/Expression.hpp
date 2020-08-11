#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Type.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;

enum class ExpressionType {
  /* constants */
  INT,
  LONG,
  FLOAT,
  DOUBLE,
  BOOLEAN,
  CHAR,
  STRING,
  /* binary operators */
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULO,
  GT,
  LT,
  GE,
  LE,
  EQ,
  NE,
  LOGICAL_AND,
  LOGICAL_OR,
  CONVERT,
  /* unary operators */
  LOGICAL_NOT,
  PLUS,
  MINUS,
  ARRAY_LENGTH,
  /* invocations */
  INVOKE,
  /* other */
  IDENTIFIER,
  MEMBER,
  NEW
};

enum class StatementType {
  EXPRESSION,
  IF_THEN,
  IF_ELSE,
  WHILE,
  BLOCK,
  RETURN,
  BREAK,
  ASSIGN,
  VARIABLE_DECLARATION
};

enum class AssignmentKind { IDENTIFIER_ASSIGN, CALL_ASSIGN, MEMBER_ASSIGN };

enum class MemberKind { FIELD, METHOD, STATIC_VAR, STATIC_FUNC };

enum class AccessModifier { PUBLIC, PRIVATE, PROTECTED };

class Position {
public:
  int startLine;
  int startCol;
  int endLine;
  int endCol;

  Position() : startLine{0}, startCol{0}, endLine{0}, endCol{0} {}
  Position(int startLine, int startCol, int endLine, int endCol)
      : startLine{startLine}, startCol{startCol}, endLine{endLine},
        endCol{endCol} {}
};

class Statement {
public:
  virtual Position Pos() const = 0;
  virtual StatementType GetStatementType() const = 0;
};

class Expression : public Statement {
public:
  virtual ExpressionType NodeType() const = 0;
  virtual TypePtr GetType() const = 0;
  StatementType GetStatementType() const override {
    return StatementType::EXPRESSION;
  }
};

using ExpPtr = shared_ptr<Expression>;
using StatementPtr = shared_ptr<Statement>;

class BinaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  ExpPtr left;
  ExpPtr right;
  TypePtr type;

  BinaryExpression(Position pos, ExpressionType nodeType, ExpPtr left,
                   ExpPtr right)
      : pos{pos}, nodeType{nodeType}, left{left}, right{right} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  TypePtr GetType() const override { return type; }
};

class ConstantExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  string value;
  TypePtr type;

  ConstantExpression(Position pos, ExpressionType nodeType, string value)
      : pos{pos}, nodeType{nodeType}, value{value} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  TypePtr GetType() const override { return type; }
};

class UnaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  ExpPtr operand;
  TypePtr type;

  UnaryExpression(Position pos, ExpressionType nodeType, ExpPtr operand)
      : pos{pos}, nodeType{nodeType}, operand{operand} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
  TypePtr GetType() const override { return type; }
};

class InvokeExpression : public Expression {
public:
  Position pos;
  ExpPtr function;
  vector<ExpPtr> args;
  TypePtr type;

  InvokeExpression(Position pos, ExpPtr function, vector<ExpPtr> args)
      : pos{pos}, function{function}, args{args} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::INVOKE; }
  TypePtr GetType() const override { return type; }
};

class IdentifierExpression : public Expression {
public:
  Position pos;
  string identifier;
  TypePtr type;

  IdentifierExpression(Position pos, string identifier)
      : pos{pos}, identifier{identifier} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override {
    return ExpressionType::IDENTIFIER;
  }
  TypePtr GetType() const override { return type; }
};

class IfThenStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  ExpPtr ifTrue;

  IfThenStatement(Position pos, ExpPtr condition, ExpPtr ifTrue)
      : pos{pos}, condition{condition}, ifTrue{ifTrue} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::IF_THEN;
  }
};

class IfElseStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  ExpPtr ifTrue;
  ExpPtr ifFalse;

  IfElseStatement(Position pos, ExpPtr condition, ExpPtr ifTrue, ExpPtr ifFalse)
      : pos{pos}, condition{condition}, ifTrue{ifTrue}, ifFalse{ifFalse} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::IF_ELSE;
  }
};

class WhileStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  ExpPtr body;

  WhileStatement(Position pos, ExpPtr condition, ExpPtr body)
      : pos{pos}, condition{condition}, body{body} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::WHILE;
  }
};

class BlockStatement : public Statement {
public:
  Position pos;
  vector<StatementPtr> statements;

  BlockStatement(Position pos, vector<StatementPtr> statements)
      : pos{pos}, statements{statements} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::BLOCK;
  }
};

class ReturnStatement : public Statement {
public:
  Position pos;
  ExpPtr value;

  ReturnStatement(Position pos, ExpPtr value) : pos{pos}, value{value} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::RETURN;
  }
};

class AssignStatement : public Statement {
public:
  Position pos;
  AssignmentKind kind;
  ExpPtr left;
  ExpPtr value;

  AssignStatement(Position pos, AssignmentKind kind, ExpPtr left, ExpPtr value)
      : pos{pos}, kind{kind}, left{left}, value{value} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::ASSIGN;
  }
};

class VariableDeclaration : public Statement {
public:
  Position pos;
  string identifier;
  optional<TypePtr> type;
  optional<ExpPtr> value;

  VariableDeclaration(Position pos, string identifier, optional<TypePtr> type,
                      optional<ExpPtr> value)
      : pos{pos}, identifier{identifier}, type{type}, value{value} {}

  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::VARIABLE_DECLARATION;
  }
};

class MemberDeclaration {
public:
  virtual Position Pos() const = 0;
  virtual MemberKind Kind() const = 0;
  virtual AccessModifier GetAccessModifier() const = 0;
};

class FieldMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  string name;
  ExpPtr initializer;
  int fieldIndex;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::FIELD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class MethodMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  bool isConstructor;
  bool isAbstract;
  bool isVirtual;
  bool isOverride;
  int methodIndex;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::METHOD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class MemberExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  string memberName;
  shared_ptr<MemberDeclaration> declaration;

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::MEMBER; }
};

class NewExpression : public Expression {
public:
  Position pos;
  shared_ptr<MethodMember> constructor;
  vector<ExpPtr> args;
  TypePtr type;

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::NEW; }
  TypePtr GetType() const override { return type; }
};

class ClassInfo {
public:
  bool isAbstract;
  bool isInterface;
  string packageName;
  string name;
  vector<weak_ptr<ClassInfo>> extendList;
  weak_ptr<ClassInfo> superClass;
  vector<weak_ptr<ClassInfo>> interfaceList;
  vector<shared_ptr<FieldMember>> fields;
  vector<shared_ptr<MethodMember>> methods;
  vector<shared_ptr<FieldMember>> staticVariables;
  vector<shared_ptr<MethodMember>> staticFunctions;
};

class Parameter {
public:
  Position pos;
  string name;
  TypePtr type;
};

class FunctionInfo {
public:
  TypePtr type;
  string packageName;
  string name;
  vector<Parameter> parameters;
  shared_ptr<BlockStatement> block;
  vector<shared_ptr<VariableDeclaration>> localVariables;
};

class Import {
public:
  Position pos;
  string packageName;
};

class Rename {
public:
  Position pos;
  string packageName;
  string originalName;
  string newName;
};

class CodeFile {
public:
  string path;
  string packageName;
  vector<Import> importList;
  vector<Rename> renameList;
  vector<shared_ptr<ClassInfo>> classList;
};

class Program {
public:
  vector<shared_ptr<CodeFile>> codeFiles;
};

class Error {
public:
  Position pos;
  string message;
  Error(Position pos, string message) : pos{pos}, message{message} {}
};

template <typename ReturnType, typename... ArgTypes> class Visitor {
public:
  virtual ReturnType VisitExpression(ExpPtr node, ArgTypes... args) {
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
      return VisitBinaryExp(static_pointer_cast<BinaryExpression>(node),
                            args...);
    case ExpressionType::LOGICAL_NOT:
    case ExpressionType::PLUS:
    case ExpressionType::MINUS:
    case ExpressionType::ARRAY_LENGTH:
    case ExpressionType::CONVERT:
      return VisitUnaryExp(static_pointer_cast<UnaryExpression>(node), args...);
    case ExpressionType::INVOKE:
      return VisitInvocation(static_pointer_cast<InvocationExpression>(node),
                             args...);
    case ExpressionType::IDENTIFIER:
      return VisitIdentifier(static_pointer_cast<IdentifierExpression>(node),
                             args...);
    case ExpressionType::MEMBER:
      return VisitMemberExp(static_pointer_cast<MemberExpression>(node),
                            args...);
    case ExpressionType::NEW:
      return VisitNewExp(static_pointer_cast<NewExpression>(node), args...);
    default:
      throw Error(node->Pos(), "unsupported node type for visitor");
    }
  }
  virtual VisitStatement(shared_ptr<Statement> statement, ArgTypes... args) {
    switch (statement->GetStatementType()) {
    case StatementType::EXPRESSION:
      return VisitExpression(static_pointer_cast<Expression>(statement),
                             args...);
    case StatementType::IF_THEN:
      return VisitIfThen(static_pointer_cast<IfThenStatement>(statement),
                         args...);
    case StatementType::IF_ELSE:
      return VisitIfElse(static_pointer_cast<IfElseStatement>(statement),
                         args...);
    case StatementType::WHILE:
      return VisitWhile(static_pointer_cast<WhileStatement>(statement),
                        args...);
    case StatementType::BLOCK:
      return VisitBlock(static_pointer_cast<BlockStatement>(statement),
                        args...);
    case StatementType::RETURN:
      return VisitReturn(static_pointer_cast<BlockStatement>(statement),
                         args...);
    case StatementType::ASSIGN:
      return VisitAssign(static_pointer_cast<AssignStatement>(statement),
                         args...);
    case StatementType::VARIABLE_DECLARATION:
      return VisitVarDecl(static_pointer_cast<VariableDeclaration>(statement),
                          args...);
    default:
      throw Error(node->Pos(), "unsupported statement type for visitor");
    }
  }
  virtual VisitConstant(shared_ptr<ConstantExpression> node,
                        ArgTypes... args) = 0;
  virtual VisitBinaryExp(shared_ptr<BinaryExpression> node,
                         ArgTypes... args) = 0;
  virtual VisitUnaryExp(shared_ptr<UnaryExpression> node, ArgTypes... args) = 0;
  virtual VisitInvocation(shared_ptr<InvocationExpression> node,
                          ArgTypes... args) = 0;
  virtual VisitIdentifier(shared_ptr<IdentifierExpression> node,
                          ArgTypes... args) = 0;
  virtual VisitMemberExp(shared_ptr<MemberExpression> node,
                         ArgTypes... args) = 0;
  virtual VisitNewExp(shared_ptr<NewExpression> node, ArgTypes... args) = 0;

  virtual VisitIfThen(shared_ptr<IfThenStatement> node, ArgTypes... args) = 0;
  virtual VisitIfElse(shared_ptr<IfElseStatement> node, ArgTypes... args) = 0;
  virtual VisitWhile(shared_ptr<WhileStatement> node, ArgTypes... args) = 0;
  virtual VisitBlock(shared_ptr<BlockStatement> node, ArgTypes... args) = 0;
};
#endif // EXPRESSION_HPP