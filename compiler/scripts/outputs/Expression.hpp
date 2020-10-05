#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>
#include "Enum.hpp"
#include "Position.hpp"
#include "Type.hpp"

using std::vector;
using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::optional;
using std::static_pointer_cast;
using std::make_shared;

class Statement;
class Expression;
class BlockStatement;
class CatchBlock;
using ExpPtr = shared_ptr<Expression>;
using StatementPtr = shared_ptr<Statement>;
using BlockPtr = shared_ptr<BlockStatement>;
using CatchBlockPtr = shared_ptr<CatchBlock>;

class Statement {
public:
  virtual Position Pos() const = 0;
  virtual StatementType GetStatementType() const = 0;
  static shared_ptr<BlockStatement> Block(Position pos,
                                          vector<StatementPtr> statements);

  static shared_ptr<IfThenStatement> IfThen(Position pos, ExpPtr condition,
                                            BlockPtr ifTrue);

  static shared_ptr<IfElseStatement> IfElse(Position pos, ExpPtr condition,
                                            BlockPtr ifTrue, BlockPtr ifFalse);

  static shared_ptr<WhileStatement> While(Position pos, ExpPtr condition,
                                          BlockPtr body);

  static shared_ptr<AssignStatement> Assign(Position pos, ExpPtr left,
                                            ExpPtr value);

  static shared_ptr<TryStatement> Try(Position pos, BlockPtr body,
                                      vector<CatchBlockPtr> handlers,
                                      BlockPtr finally);

  static shared_ptr<ReturnStatement> Return(Position pos, ExpPtr value);

  static shared_ptr<BreakStatement> Break(Position pos);

  static shared_ptr<VarDeclStatement> VarDecl(Position pos, string variableName,
                                              optional<TypePtr> type,
                                              optional<ExpPtr> value);
};

class Expression : public Statement {
public:
  virtual ExpressionType NodeType() const = 0;
  StatementType GetStatementType() const override {
    return StatementType::EXPRESSION;
  }
  static shared_ptr<BinaryExpression>
  Binary(Position pos, ExpressionType nodeType, ExpPtr left, ExpPtr right);

  static shared_ptr<ConstantExpression>
  Constant(Position pos, ExpressionType nodeType, string value);

  static shared_ptr<UnaryExpression>
  Unary(Position pos, ExpressionType nodeType, ExpPtr operand);

  static shared_ptr<InvocationExpression>
  Invocation(Position pos, ExpPtr function, vector<ExpPtr> arguments);

  static shared_ptr<IdentifierExpression> Identifier(Position pos,
                                                     string identifier);

  static shared_ptr<MemberExpression> Member(Position pos, ExpPtr object,
                                             string memberName);

  static shared_ptr<NewExpression> New(Position pos, string className);

  static shared_ptr<IsExpression> Is(Position pos, ExpPtr object, TypePtr type);

  static shared_ptr<AsExpression> As(Position pos, ExpPtr object, TypePtr type);
};

class CatchBlock {
public:
  string variable;
  TypePtr test;
  BlockPtr body;

  CatchBlock(string variable, TypePtr test, BlockPtr body)
      : variable{variable}, test{test}, body{body} {}
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
  vector<ExpPtr> arguments;

  InvocationExpression(Position pos, ExpPtr function, vector<ExpPtr> arguments)
      : pos{pos}, function{function}, arguments{arguments} {}
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

class MemberExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  string memberName;

  MemberExpression(Position pos, ExpPtr object, string memberName)
      : pos{pos}, object{object}, memberName{memberName} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::MEMBER; }
};

class NewExpression : public Expression {
public:
  Position pos;
  string className;

  NewExpression(Position pos, string className)
      : pos{pos}, className{className} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::NEW; }
};

class IsExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  TypePtr type;

  IsExpression(Position pos, ExpPtr object, TypePtr type)
      : pos{pos}, object{object}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::IS; }
};

class AsExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  TypePtr type;

  AsExpression(Position pos, ExpPtr object, TypePtr type)
      : pos{pos}, object{object}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::AS; }
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

class IfThenStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  BlockPtr ifTrue;

  IfThenStatement(Position pos, ExpPtr condition, BlockPtr ifTrue)
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
  BlockPtr ifTrue;
  BlockPtr ifFalse;

  IfElseStatement(Position pos, ExpPtr condition, BlockPtr ifTrue,
                  BlockPtr ifFalse)
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
  BlockPtr body;

  WhileStatement(Position pos, ExpPtr condition, BlockPtr body)
      : pos{pos}, condition{condition}, body{body} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::WHILE;
  }
};

class AssignStatement : public Statement {
public:
  Position pos;
  ExpPtr left;
  ExpPtr value;

  AssignStatement(Position pos, ExpPtr left, ExpPtr value)
      : pos{pos}, left{left}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::ASSIGN;
  }
};

class TryStatement : public Statement {
public:
  Position pos;
  BlockPtr body;
  vector<CatchBlockPtr> handlers;
  BlockPtr finally;

  TryStatement(Position pos, BlockPtr body, vector<CatchBlockPtr> handlers,
               BlockPtr finally)
      : pos{pos}, body{body}, handlers{handlers}, finally{finally} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override { return StatementType::TRY; }
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

class VarDeclStatement : public Statement {
public:
  Position pos;
  string variableName;
  optional<TypePtr> type;
  optional<ExpPtr> value;

  VarDeclStatement(Position pos, string variableName, optional<TypePtr> type,
                   optional<ExpPtr> value)
      : pos{pos}, variableName{variableName}, type{type}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::VAR_DECL;
  }
};

#endif // EXPRESSION_HPP
