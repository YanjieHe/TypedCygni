#include "Expression.hpp"
#include "Linq.hpp"

Json BinaryExpression::ToJson() const {
  return unordered_map<string, Json>({
      {"Position", pos.ToJson()},
      {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
      {"Left", left->ToJson()},
      {"Right", right->ToJson()},
  });
}

Json UnaryExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Operand", operand->ToJson()}});
}

Json ConstantExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"value", value}});
}

Json InvocationExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"function", function->ToJson()},
       {"Arguments",
        linq::from(args)
            .select([](Expression::Ptr arg) -> Json { return arg->ToJson(); })
            .to_vector()}});
}

Json IdentifierExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Identifier", identifier}});
}

Json MemberExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Object", object->ToJson()},
       {"Member Name", memberName}});
}

Json NewExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Class Name", className},
       {"Arguments",
        linq::from(args)
            .select([](Expression::Ptr arg) -> Json { return arg->ToJson(); })
            .to_vector()}});
}

Json AsExpression::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Expression", expression->ToJson()}});
}

Json IsExpression::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Expression", expression->ToJson()}});
}

Json BlockExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Expressions",
        linq::from(expressions)
            .select([](Expression::Ptr exp) -> Json { return exp->ToJson(); })
            .to_vector()}});
}

Json ConditionalExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Condition", condition->ToJson()},
       {"If True", ifTrue->ToJson()},
       {"If False", ifFalse->ToJson()}});
}

Json AssignExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Kind", Enum<AssignmentKind>::ToString(kind)},
       {"Left", left->ToJson()},
       {"Value", value->ToJson()}});
}

Json ResetExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Body", body->ToJson()}});
}

Json Parameter::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()}, {"Name", name}});
}

Json ShiftExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Parameter", parameter->ToJson()},
       {"Body", body->ToJson()}});
}

Json LambdaExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Parameters", linq::from(parameters)
                          .select([](Parameter::Ptr parameter) -> Json {
                            return parameter->ToJson();
                          })
                          .to_vector()},
       {"Body", body->ToJson()}});
}

json CatchBlock::ToJson() const {
  // TO DO: type test to json
  return unordered_map<string, Json>({{"Position", pos.ToJson()},
                                      {"Variable", variable},
                                      {"Body", body->ToJson()}});
}

json TryExpression::ToJson() const {
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Body", body->ToJson()},
       {"Handlers", linq::from(handlers)
                        .select([](CatchBlock::Ptr catchBlock) -> Json {
                          return catchBlock->ToJson();
                        })
                        .to_vector()},
       {"Finally", finally->ToJson()}});
}
Json VarExpression::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"Is Mutable", isMutable},
       {"Identifier", identifier},
       {"Value", value->ToJson()}});
}