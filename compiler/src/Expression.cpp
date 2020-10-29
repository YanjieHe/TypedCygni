#include "Expression.hpp"

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
    // TO DO: args
  return unordered_map<string, Json>(
      {{"Position", pos.ToJson()},
       {"Node Type", Enum<ExpressionType>::ToString(NodeType())},
       {"function", function->ToJson()}});
}