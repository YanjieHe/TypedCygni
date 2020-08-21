#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Enum.hpp"
#include <cstdint>
#include <list>
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

class Statement;
class Expression;

using ExpPtr = shared_ptr<Expression>;
using StatementPtr = shared_ptr<Statement>;

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
  ConversionExpression(Position pos, ExpPtr expression)
      : pos{pos}, expression{expression} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::CONVERT; }
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

class BreakStatement : public Statement {
public:
  Position pos;
  BreakStatement(Position pos) : pos{pos} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::BREAK;
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

class VarDeclStatement : public Statement {
public:
  Position pos;
  string identifier;
  optional<TypePtr> type;
  optional<ExpPtr> value;
  VarDeclStatement(Position pos, string identifier, optional<TypePtr> type,
                   optional<ExpPtr> value)
      : pos{pos}, identifier{identifier}, type{type}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::VAR_DECL;
  }
};

template <typename ExpReturnType, typename StatementReturnType,
          typename... ArgTypes>
class Visitor {
public:
  virtual ExpReturnType VisitExpression(ExpPtr node, ArgTypes... args) {
    switch (node->NodeType()) {
    case ExpressionType::ADD: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::SUBTRACT: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::MULTIPLY: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::DIVIDE: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::MODULO: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::GT: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::LT: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::GE: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::LE: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::EQ: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::NE: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::LOGICAL_AND: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::LOGICAL_OR: {
      return VisitBinary(static_pointer_cast<BinaryExpression>(node), args...);
    }
    case ExpressionType::INT: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::LONG: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::FLOAT: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::DOUBLE: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::BOOLEAN: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::CHAR: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::STRING: {
      return VisitConstant(static_pointer_cast<ConstantExpression>(node),
                           args...);
    }
    case ExpressionType::PLUS: {
      return VisitUnary(static_pointer_cast<UnaryExpression>(node), args...);
    }
    case ExpressionType::MINUS: {
      return VisitUnary(static_pointer_cast<UnaryExpression>(node), args...);
    }
    case ExpressionType::LOGICAL_NOT: {
      return VisitUnary(static_pointer_cast<UnaryExpression>(node), args...);
    }
    case ExpressionType::INVOKE: {
      return VisitInvocation(static_pointer_cast<InvocationExpression>(node),
                             args...);
    }
    case ExpressionType::IDENTIFIER: {
      return VisitIdentifier(static_pointer_cast<IdentifierExpression>(node),
                             args...);
    }
    case ExpressionType::CONVERT: {
      return VisitConversion(static_pointer_cast<ConversionExpression>(node),
                             args...);
    }
    case ExpressionType::MEMBER: {
      return VisitMember(static_pointer_cast<MemberExpression>(node), args...);
    }
    case ExpressionType::NEW: {
      return VisitNew(static_pointer_cast<NewExpression>(node), args...);
    }
    default:
      throw Error(node->Pos(), "unsupported node type for visitor");
    }
  }
  virtual StatementReturnType VisitStatement(shared_ptr<Statement> statement,
                                             ArgTypes... args) {
    switch (statement->GetStatementType()) {
    case StatementType::IF_THEN: {
      return VisitIfThen(static_pointer_cast<IfThenStatement>(node), args...);
    }
    case StatementType::IF_ELSE: {
      return VisitIfElse(static_pointer_cast<IfElseStatement>(node), args...);
    }
    case StatementType::WHILE: {
      return VisitWhile(static_pointer_cast<WhileStatement>(node), args...);
    }
    case StatementType::BLOCK: {
      return VisitBlock(static_pointer_cast<BlockStatement>(node), args...);
    }
    case StatementType::RETURN: {
      return VisitReturn(static_pointer_cast<ReturnStatement>(node), args...);
    }
    case StatementType::BREAK: {
      return VisitBreak(static_pointer_cast<BreakStatement>(node), args...);
    }
    case StatementType::ASSIGN: {
      return VisitAssign(static_pointer_cast<AssignStatement>(node), args...);
    }
    case StatementType::VAR_DECL: {
      return VisitVarDecl(static_pointer_cast<VarDeclStatement>(node), args...);
    }
    default: {
      throw Error(node->Pos(), "unsupported statement type for visitor");
    }
    }
  }
  virtual ExpReturnType VisitBinary(shared_ptr<BinaryExpression> node,
                                    ArgTypes... args) = 0;
  virtual ExpReturnType VisitConstant(shared_ptr<ConstantExpression> node,
                                      ArgTypes... args) = 0;
  virtual ExpReturnType VisitUnary(shared_ptr<UnaryExpression> node,
                                   ArgTypes... args) = 0;
  virtual ExpReturnType VisitInvocation(shared_ptr<InvocationExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitIdentifier(shared_ptr<IdentifierExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitConversion(shared_ptr<ConversionExpression> node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitMember(shared_ptr<MemberExpression> node,
                                    ArgTypes... args) = 0;
  virtual ExpReturnType VisitNew(shared_ptr<NewExpression> node,
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
  virtual StatementReturnType VisitBreak(shared_ptr<BreakStatement> node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitAssign(shared_ptr<AssignStatement> node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitVarDecl(shared_ptr<VarDeclStatement> node,
                                           ArgTypes... args) = 0;
};

#endif // EXPRESSION_HPP
