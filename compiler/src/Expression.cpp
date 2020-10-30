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
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()}, {"Identifier", identifier}});
}

Json MemberExpression::ToJson() const {
  return unordered_map<string, json>({{"Position", pos.ToJson()},
                                      {"Object", object->ToJson()},
                                      {"Member Name", memberName}});
}

Json NewExpression::ToJson() const {
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()},
       {"Class Name", className},
       {"Arguments",
        linq::from(args)
            .select([](Expression::Ptr arg) -> Json { return arg->ToJson(); })
            .to_vector()}});
}

Json AsExpression::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()}, {"Expression", expression->ToJson()}});
}

Json IsExpression::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()}, {"Expression", expression->ToJson()}});
}

Json BlockExpression::ToJson() const {
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()},
       {"Expressions",
        linq::from(expressions)
            .select([](Expression::Ptr exp) -> Json { return exp->ToJson(); })
            .to_vector()}});
}

Json ConditionalExpression::ToJson() const {
  return unordered_map<string, json>({{"Position", pos.ToJson()},
                                      {"Condition", condition->ToJson()},
                                      {"If True", ifTrue->ToJson()},
                                      {"If False", ifFalse->ToJson()}});
}

Json AssignExpression::ToJson() const {
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()},
       {"Kind", Enum<AssignmentKind>::ToString(kind)},
       {"Left", left->ToJson()},
       {"Value", value->ToJson()}});
}

Json ResetExpression::ToJson() const {
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()}, {"Body", body->ToJson()}});
}

Json Parameter::ToJson() const {
  // TO DO: type to json
  return unordered_map<string, json>(
      {{"Position", pos.ToJson()}, {"Name", name}});
}

Json ShiftExpression::ToJson() const {
  return unordered_map<string, json>({{"Position", pos.ToJson()},
                                      {"Parameter", parameter->ToJson()},
                                      {"Body", body->ToJson()}});
}