#include "Expression.hpp"

shared_ptr<BlockStatement> Statement::Block(Position pos,
                                            vector<StatementPtr> statements) {
  return make_shared<BlockStatement>(pos, statements);
}

shared_ptr<IfThenStatement> Statement::IfThen(Position pos, ExpPtr condition,
                                              BlockPtr ifTrue) {
  return make_shared<IfThenStatement>(pos, condition, ifTrue);
}

shared_ptr<IfElseStatement> Statement::IfElse(Position pos, ExpPtr condition,
                                              BlockPtr ifTrue,
                                              BlockPtr ifFalse) {
  return make_shared<IfElseStatement>(pos, condition, ifTrue, ifFalse);
}

shared_ptr<WhileStatement> Statement::While(Position pos, ExpPtr condition,
                                            BlockPtr body) {
  return make_shared<WhileStatement>(pos, condition, body);
}

shared_ptr<AssignStatement> Statement::Assign(Position pos, ExpPtr left,
                                              ExpPtr value) {
  return make_shared<AssignStatement>(pos, left, value);
}

shared_ptr<TryStatement> Statement::Try(Position pos, BlockPtr body,
                                        vector<CatchBlockPtr> handlers,
                                        BlockPtr finally) {
  return make_shared<TryStatement>(pos, body, handlers, finally);
}

shared_ptr<ReturnStatement> Statement::Return(Position pos, ExpPtr value) {
  return make_shared<ReturnStatement>(pos, value);
}

shared_ptr<BreakStatement> Statement::Break(Position pos) {
  return make_shared<BreakStatement>(pos);
}

shared_ptr<VarDeclStatement> Statement::VarDecl(Position pos,
                                                string variableName,
                                                optional<TypePtr> type,
                                                optional<ExpPtr> value) {
  return make_shared<VarDeclStatement>(pos, variableName, type, value);
}

shared_ptr<BinaryExpression> Expression::Binary(Position pos,
                                                ExpressionType nodeType,
                                                ExpPtr left, ExpPtr right) {
  return make_shared<BinaryExpression>(pos, nodeType, left, right);
}

shared_ptr<ConstantExpression>
Expression::Constant(Position pos, ExpressionType nodeType, string value) {
  return make_shared<ConstantExpression>(pos, nodeType, value);
}

shared_ptr<UnaryExpression>
Expression::Unary(Position pos, ExpressionType nodeType, ExpPtr operand) {
  return make_shared<UnaryExpression>(pos, nodeType, operand);
}

shared_ptr<InvocationExpression>
Expression::Invocation(Position pos, ExpPtr function,
                       vector<ExpPtr> arguments) {
  return make_shared<InvocationExpression>(pos, function, arguments);
}

shared_ptr<IdentifierExpression> Expression::Identifier(Position pos,
                                                        string identifier) {
  return make_shared<IdentifierExpression>(pos, identifier);
}

shared_ptr<MemberExpression> Expression::Member(Position pos, ExpPtr object,
                                                string memberName) {
  return make_shared<MemberExpression>(pos, object, memberName);
}

shared_ptr<NewExpression> Expression::New(Position pos, string className) {
  return make_shared<NewExpression>(pos, className);
}

shared_ptr<IsExpression> Expression::Is(Position pos, ExpPtr object,
                                        TypePtr type) {
  return make_shared<IsExpression>(pos, object, type);
}

shared_ptr<AsExpression> Expression::As(Position pos, ExpPtr object,
                                        TypePtr type) {
  return make_shared<AsExpression>(pos, object, type);
}
