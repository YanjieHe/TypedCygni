#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Type.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define BINARY_TYPE_MATCH(NODE_TYPE, LEFT_TYPE, RIGHT_TYPE, RETURN_TYPE)       \
  if (node->NodeType() == ExpressionType::##NODE_TYPE &&                       \
      left->Equals(Type::##LEFT_TYPE##()) &&                                   \
      right->Equals(Type::##RIGHT_TYPE##())) {                                 \
    return Type::##RETURN_TYPE##();                                            \
  }

#define UNARY_TYPE_MATCH(NODE_TYPE, OPERAND_TYPE, RETURN_TYPE)                 \
  if (node->NodeType() == ExpressionType::##NODE_TYPE &&                       \
      operand->Equals(Type::##OPERAND_TYPE##())) {                             \
    return Type::##RETURN_TYPE##();                                            \
  }

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
  /* unary operators */
  LOGICAL_NOT,
  PLUS,
  MINUS,
  /* invocations */
  INVOKE,
  /* other */
  IDENTIFIER,
  CONVERT,
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

  BinaryExpression(Position pos, ExpressionType nodeType, ExpPtr left,
                   ExpPtr right)
      : pos{pos}, nodeType{nodeType}, left{left}, right{right} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class ConstantExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  string value;

  ConstantExpression(Position pos, ExpressionType nodeType, string value)
      : pos{pos}, nodeType{nodeType}, value{value} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class UnaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  ExpPtr operand;

  UnaryExpression(Position pos, ExpressionType nodeType, ExpPtr operand)
      : pos{pos}, nodeType{nodeType}, operand{operand} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class InvocationExpression : public Expression {
public:
  Position pos;
  ExpPtr function;
  vector<ExpPtr> args;

  InvocationExpression(Position pos, ExpPtr function, vector<ExpPtr> args)
      : pos{pos}, function{function}, args{args} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::INVOKE; }
};

class IdentifierExpression : public Expression {
public:
  Position pos;
  string identifier;

  IdentifierExpression(Position pos, string identifier)
      : pos{pos}, identifier{identifier} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override {
    return ExpressionType::IDENTIFIER;
  }
};

class ConversionExpression : public Expression {
public:
  Position pos;
  ExpPtr expression;
  TypePtr type;

  ConversionExpression(Position pos, ExpPtr expression, TypePtr type)
      : pos{pos}, expression{expression}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::CONVERT; }
};

class IfThenStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  shared_ptr<BlockStatement> ifTrue;

  IfThenStatement(Position pos, ExpPtr condition,
                  shared_ptr<BlockStatement> ifTrue)
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
  shared_ptr<BlockStatement> ifTrue;
  shared_ptr<BlockStatement> ifFalse;

  IfElseStatement(Position pos, ExpPtr condition,
                  shared_ptr<BlockStatement> ifTrue,
                  shared_ptr<BlockStatement> ifFalse)
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
  shared_ptr<BlockStatement> body;

  WhileStatement(Position pos, ExpPtr condition,
                 shared_ptr<BlockStatement> body)
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
  weak_ptr<ClassInfo> classInfo;

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
  FunctionType functionType; // 'this' is the first parameter
  weak_ptr<ClassInfo> classInfo;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::METHOD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class MemberExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  shared_ptr<MemberDeclaration> declaration;

  MemberExpression(Position pos, ExpPtr object,
                   shared_ptr<MemberDeclaration> declaration)
      : pos{pos}, object{object}, declaration{declaration} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::MEMBER; }
};

class NewExpression : public Expression {
public:
  Position pos;
  shared_ptr<MethodMember> constructor;
  vector<ExpPtr> args;

  NewExpression(Position pos, shared_ptr<MethodMember> constructor,
                vector<ExpPtr> args)
      : pos{pos}, constructor{constructor}, args{args} {}

  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::NEW; }
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

template <typename ExpReturnType, typename StatementReturnType,
          typename... ArgTypes>
class Visitor {
public:
  virtual ExpReturnType VisitExpression(ExpPtr node, ArgTypes... args) {
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
      return VisitUnaryExp(static_pointer_cast<UnaryExpression>(node), args...);
    case ExpressionType::INVOKE:
      return VisitInvocation(static_pointer_cast<InvocationExpression>(node),
                             args...);
    case ExpressionType::IDENTIFIER:
      return VisitIdentifier(static_pointer_cast<IdentifierExpression>(node),
                             args...);
    case ExpressionType::CONVERT:
      return VisitConversion(static_pointer_cast<ConversionExpression>(node),
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
  virtual StatementReturnType VisitStatement(shared_ptr<Statement> statement,
                                             ArgTypes... args) {
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
  virtual ExpReturnType VisitConstant(shared_ptr<ConstantExpression> node,
                                      ArgTypes... args) = 0;
  virtual ExpReturnType VisitBinaryExp(shared_ptr<BinaryExpression> node,
                                       ArgTypes... args) = 0;
  virtual ExpReturnType VisitUnaryExp(shared_ptr<UnaryExpression> node,
                                      ArgTypes... args) = 0;
  virtual ExpReturnType VisitInvocation(shared_ptr<InvocationExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitIdentifier(shared_ptr<IdentifierExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitConversion(shared_ptr<ConversionExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitMemberExp(shared_ptr<MemberExpression> node,
                                       ArgTypes... args) = 0;
  virtual ExpReturnType VisitNewExp(shared_ptr<NewExpression> node,
                                    ArgTypes... args) = 0;

  virtual StatementReturnType VisitIfThen(shared_ptr<IfThenStatement> node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitIfElse(shared_ptr<IfElseStatement> node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitWhile(shared_ptr<WhileStatement> node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitBlock(shared_ptr<BlockStatement> node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitReturn(shared_ptr<ReturnStatement> node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitAssign(shared_ptr<AssignStatement> node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitVarDecl(shared_ptr<VariableDeclaration> node,
                                           ArgTypes... args) = 0;
};

template <typename T> class Scope {
public:
  unordered_map<string, T> table;
  weak_ptr<Scope<T>> parent;

  Scope() : parent() {}

  Scope(shared_ptr<Scope<T>> parent) : parent(parent) {}

  optional<T> Get(string key) {
    if (table.count(key)) {
      return table[key];
    } else if (parent.expired()) {
      return make_optional<T>();
    } else {
      return parent->Get(key);
    }
  }

  /* true: success, false: failed */
  bool Put(string key, const T &value) {
    if (table.count(key)) {
      return false;
    } else {
      table.insert({key, value});
      return true;
    }
  }
};

template <typename T> using ScopePtr = shared_ptr<Scope<T>>;

template <typename... ArgTypes>
class NanoPass : public Visitor<ExpPtr, StatmentPtr, ArgTypes...> {
  virtual ExpPtr VisitConstant(shared_ptr<ConstantExpression> node,
                               ArgTypes... args) {
    return node;
  }
  virtual ExpPtr VisitBinaryExp(shared_ptr<BinaryExpression> node,
                                ArgTypes... args) {
    auto left = VisitExpression(node->left, args...);
    auto right = VisitExpression(node->right, args...);
    return make_shared<BinaryExpression>(node->Pos(), node->NodeType(), left,
                                         right);
  }
  virtual ExpPtr VisitUnaryExp(shared_ptr<UnaryExpression> node,
                               ArgTypes... args) {
    auto operand = VisitExpression(node->operand, args...);
    return make_shared<UnaryExpression>(node->Pos(), node->NodeType(), operand);
  }
  virtual ExpPtr VisitInvocation(shared_ptr<InvocationExpression> node,
                                 ArgTypes... args) {
    auto function = VisitExpression(node->function, args...);
    auto arguments =
        Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
          return VisitExpression(arg, ... args);
        });
    return make_shared<InvocationExpression>(node->Pos(), function, arguments);
  }
  virtual ExpPtr VisitIdentifier(shared_ptr<IdentifierExpression> node,
                                 ArgTypes... args) {
    return node;
  }
  virtual ExpPtr VisitConversion(shared_ptr<ConversionExpression> node,
                                 ArgTypes... args) {
    auto expression = VisitExpression(node, args...);
    return make_shared<ConversionExpression>(node->Pos(), expression,
                                             node->type);
  }
  virtual ExpPtr VisitMemberExp(shared_ptr<MemberExpression> node,
                                ArgTypes... args) {
    auto object = VisitExpression(node->object, args...);
    return make_shared<MemberExpression>(node->Pos(), object,
                                         node->declaration);
  }
  virtual ExpPtr VisitNewExp(shared_ptr<NewExpression> node, ArgTypes... args) {
    auto arguments =
        Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
          return VisitExpression(arg, ... args);
        });
    return make_shared<NewExpression>(node->Pos(), node->constructor,
                                      arguments);
  }

  virtual StatmentPtr VisitIfThen(shared_ptr<IfThenStatement> node,
                                  ArgTypes... args) {
    auto condition = VisitExpression(node->condition, args...);
    auto ifTrue = VisitStatement(node->ifTrue, args...);
    return make_shared<IfThenStatement>(node->Pos(), condition, ifTrue);
  }
  virtual StatmentPtr VisitIfElse(shared_ptr<IfElseStatement> node,
                                  ArgTypes... args) {
    auto condition = VisitExpression(node->condition, args...);
    auto ifTrue = VisitStatement(node->ifTrue, args...);
    auto ifFalse = VisitStatement(node->ifFalse, args...);
    return make_shared<IfElseStatement>(node->Pos(), condition, ifTrue,
                                        ifFalse);
  }
  virtual StatmentPtr VisitWhile(shared_ptr<WhileStatement> node,
                                 ArgTypes... args) {
    auto condition = VisitExpression(node->condition, args...);
    auto body = VisitStatement(node->body, args...);
    return make_shared<WhileStatement>(node->Pos(), condition, body);
  }
  virtual StatmentPtr VisitBlock(shared_ptr<BlockStatement> node,
                                 ArgTypes... args) {
    auto statements = Vec::Map<StatementPtr, StatementPtr>(
        node->statements, [&](StatementPtr x) -> StatementPtr {
          return VisitStatement(x, args...);
        });
    return make_shared<BlockStatement>(node->Pos(), statements);
  }
  virtual StatmentPtr VisitReturn(shared_ptr<ReturnStatement> node,
                                  ArgTypes... args) {
    auto value = VisitExpression(node->value, args...);
    return make_shared<ReturnStatement>(node->Pos(), value);
  }
  virtual StatmentPtr VisitAssign(shared_ptr<AssignStatement> node,
                                  ArgTypes... args) {
    auto left = VisitExpression(node->left, args...);
    auto value = VisitExpression(node->value, args...);
    return make_shared<AssignStatement>(node->Pos(), node->kind, left, value);
  }
  virtual StatmentPtr VisitVarDecl(shared_ptr<VariableDeclaration> node,
                                   ArgTypes... args) {
    if (node->value.has_value()) {
      auto value = VisitExpression(node->value.value(), args...);
      return make_shared<VariableDeclaration>(node->Pos(), node->identifier,
                                              node->type, value);
    } else {
      return make_shared<VariableDeclaration>(
          node->Pos(), node->identifier, node->type, make_shared<ExpPtr>());
    }
  }
};

class TypeChecker : public Visitor<TypePtr, void, ScopePtr<TypePtr>> {
public:
  TypePtr VisitConstant(shared_ptr<ConstantExpression> node,
                        ScopePtr<TypePtr> scope) override {
    switch (node->NodeType()) {
    case ExpressionType::INT:
      return Type::Int();
    case ExpressionType::BOOLEAN:
      return Type::Boolean();
    case ExpressionType::LONG:
      return Type::Long();
    case ExpressionType::DOUBLE:
      return Type::Double();
    case ExpressionType::CHAR:
      return Type::Char();
    case ExpressionType::STRING:
      return Type::String();
    default:
      throw Error(node->Pos(), "error constant node type");
    }
  }

  TypePtr VisitBinaryExp(shared_ptr<BinaryExpression> node,
                         ScopePtr<TypePtr> scope) override {
    auto left = VisitExpression(node->left, scope);
    auto right = VisitExpression(node->right, scope);

    BINARY_TYPE_MATCH(ADD, Int, Int, Int);
    BINARY_TYPE_MATCH(ADD, Long, Long, Long);
    BINARY_TYPE_MATCH(ADD, Float, Float, Float);
    BINARY_TYPE_MATCH(ADD, Double, Double, Double);

    BINARY_TYPE_MATCH(SUBTRACT, Int, Int, Int);
    BINARY_TYPE_MATCH(SUBTRACT, Long, Long, Long);
    BINARY_TYPE_MATCH(SUBTRACT, Float, Float, Float);
    BINARY_TYPE_MATCH(SUBTRACT, Double, Double, Double);

    BINARY_TYPE_MATCH(MULTIPLY, Int, Int, Int);
    BINARY_TYPE_MATCH(MULTIPLY, Long, Long, Long);
    BINARY_TYPE_MATCH(MULTIPLY, Float, Float, Float);
    BINARY_TYPE_MATCH(MULTIPLY, Double, Double, Double);

    BINARY_TYPE_MATCH(DIVIDE, Int, Int, Int);
    BINARY_TYPE_MATCH(DIVIDE, Long, Long, Long);
    BINARY_TYPE_MATCH(DIVIDE, Float, Float, Float);
    BINARY_TYPE_MATCH(DIVIDE, Double, Double, Double);

    BINARY_TYPE_MATCH(MODULO, Int, Int, Int);
    BINARY_TYPE_MATCH(MODULO, Long, Long, Long);

    BINARY_TYPE_MATCH(GT, Int, Int, Boolean);
    BINARY_TYPE_MATCH(GT, Long, Long, Boolean);
    BINARY_TYPE_MATCH(GT, Float, Float, Boolean);
    BINARY_TYPE_MATCH(GT, Double, Double, Boolean);
    BINARY_TYPE_MATCH(GT, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LT, Int, Int, Boolean);
    BINARY_TYPE_MATCH(LT, Long, Long, Boolean);
    BINARY_TYPE_MATCH(LT, Float, Float, Boolean);
    BINARY_TYPE_MATCH(LT, Double, Double, Boolean);
    BINARY_TYPE_MATCH(LT, Char, Char, Boolean);

    BINARY_TYPE_MATCH(GE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(GE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(GE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(GE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(GE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(LE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(LE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(LE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(LE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(EQ, Int, Int, Boolean);
    BINARY_TYPE_MATCH(EQ, Long, Long, Boolean);
    BINARY_TYPE_MATCH(EQ, Float, Float, Boolean);
    BINARY_TYPE_MATCH(EQ, Double, Double, Boolean);
    BINARY_TYPE_MATCH(EQ, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(EQ, Char, Char, Boolean);

    BINARY_TYPE_MATCH(NE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(NE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(NE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(NE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(NE, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(NE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LOGICAL_AND, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(LOGICAL_OR, Boolean, Boolean, Boolean);

    throw Error(node->Pos(), "binary operation type mismatch");
  }
  TypePtr VisitUnaryExp(shared_ptr<UnaryExpression> node,
                        ScopePtr<TypePtr> scope) override {
    auto operand = VisitExpression(node->operand, scope);

    UNARY_TYPE_MATCH(PLUS, Int, Int);
    UNARY_TYPE_MATCH(PLUS, Long, Long);
    UNARY_TYPE_MATCH(PLUS, Float, Float);
    UNARY_TYPE_MATCH(PLUS, Double, Double);

    UNARY_TYPE_MATCH(MINUS, Int, Int);
    UNARY_TYPE_MATCH(MINUS, Long, Long);
    UNARY_TYPE_MATCH(MINUS, Float, Float);
    UNARY_TYPE_MATCH(MINUS, Double, Double);

    UNARY_TYPE_MATCH(LOGICAL_NOT, Boolean, Boolean);

    throw Error(node->Pos(), "unary operation type mismtach");
  }

  TypePtr VisitInvocation(shared_ptr<InvocationExpression> node,
                          ScopePtr<TypePtr> scope) override {
    auto args = Vec::Map<ExpPtr, TypePtr>(node->args, [&](ExpPtr e) -> TypePtr {
      return VisitExpression(e, scope);
    });
    if (node->NodeType() == ExpressionType::MEMBER) {
      auto memberAccess = static_pointer_cast<MemberExpression>(node);
      auto object = VisitExpression(memberAccess->object, scope);
      if (memberAccess->declaration->Kind() == MemberKind::METHOD) {
        // TO DO
        auto method =
            static_pointer_cast<MethodMember>(memberAccess->declaration);
        if (method->functionType.args.front()->Equals(object) &&
            Vec::SequenceEqual(
                Vec::Skip(method->functionType->args, 1), args,
                [](TypePtr x, TypePtr y) -> bool { return x->Equals(y); })) {
          return method->functionType->ret;
        } else {
          throw Error(node->Pos(), "method arguments' type mismtach");
        }
      } else {
        // TO DO
      }
    } else {
      auto function = VisitExpression(node->function, scope);
      if (function->GetTypeCode() == TypeCode::FUNCTION) {
        auto ft = static_pointer_cast<FunctionType>(function);
        if (Vec::SequenceEqual(
                ft->args, args,
                [](TypePtr x, TypePtr y) -> bool { return x->Equals(y); })) {
          return ft->ret;
        } else {
          throw Error(node->Pos(), "function arguments' type mismtach");
        }
      } else {
        throw Error(node->Pos(), "caller is not a function");
      }
    }
  }
};
#endif // EXPRESSION_HPP