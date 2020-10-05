#include "Expression.hpp"

static shared_ptr<BlockStatement> Block(Position pos,
                                        vector<StatementPtr> statements) {
  return make_shared<BlockStatement>(pos, statements);
}

static shared_ptr<IfThenStatement> IfThen(Position pos, ExpPtr condition,
                                          BlockPtr ifTrue) {
  return make_shared<IfThenStatement>(pos, condition, ifTrue);
}

static shared_ptr<IfElseStatement> IfElse(Position pos, ExpPtr condition,
                                          BlockPtr ifTrue, BlockPtr ifFalse) {
  return make_shared<IfElseStatement>(pos, condition, ifTrue, ifFalse);
}

static shared_ptr<WhileStatement> While(Position pos, ExpPtr condition,
                                        BlockPtr body) {
  return make_shared<WhileStatement>(pos, condition, body);
}

static shared_ptr<AssignStatement> Assign(Position pos, ExpPtr left,
                                          ExpPtr value) {
  return make_shared<AssignStatement>(pos, left, value);
}

static shared_ptr<TryStatement> Try(Position pos, BlockPtr body,
                                    vector<CatchBlockPtr> handlers,
                                    BlockPtr finally) {
  return make_shared<TryStatement>(pos, body, handlers, finally);
}

static shared_ptr<ReturnStatement> Return(Position pos, ExpPtr value) {
  return make_shared<ReturnStatement>(pos, value);
}

static shared_ptr<BreakStatement> Break(Position pos) {
  return make_shared<BreakStatement>(pos);
}

static shared_ptr<VarDeclStatement> VarDecl(Position pos, string variableName,
                                            optional<TypePtr> type,
                                            optional<ExpPtr> value) {
  return make_shared<VarDeclStatement>(pos, variableName, type, value);
}

static shared_ptr<BinaryExpression>
Binary(Position pos, ExpressionType nodeType, ExpPtr left, ExpPtr right) {
  return make_shared<BinaryExpression>(pos, nodeType, left, right);
}

static shared_ptr<ConstantExpression>
Constant(Position pos, ExpressionType nodeType, string value) {
  return make_shared<ConstantExpression>(pos, nodeType, value);
}

static shared_ptr<UnaryExpression> Unary(Position pos, ExpressionType nodeType,
                                         ExpPtr operand) {
  return make_shared<UnaryExpression>(pos, nodeType, operand);
}

static shared_ptr<InvocationExpression>
Invocation(Position pos, ExpPtr function, vector<ExpPtr> arguments) {
  return make_shared<InvocationExpression>(pos, function, arguments);
}

static shared_ptr<IdentifierExpression> Identifier(Position pos,
                                                   string identifier) {
  return make_shared<IdentifierExpression>(pos, identifier);
}

static shared_ptr<MemberExpression> Member(Position pos, ExpPtr object,
                                           string memberName) {
  return make_shared<MemberExpression>(pos, object, memberName);
}

static shared_ptr<NewExpression> New(Position pos, string className) {
  return make_shared<NewExpression>(pos, className);
}

static shared_ptr<IsExpression> Is(Position pos, ExpPtr object, TypePtr type) {
  return make_shared<IsExpression>(pos, object, type);
}

static shared_ptr<AsExpression> As(Position pos, ExpPtr object, TypePtr type) {
  return make_shared<AsExpression>(pos, object, type);
}
