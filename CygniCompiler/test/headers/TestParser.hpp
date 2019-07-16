#ifndef TESTPARSER_HPP
#define TESTPARSER_HPP

#include <fstream>
#include "JSON.hpp"
#include "Parser.hpp"

namespace TestParser {
Ptr<JsonObject> TreeToJson(const Ptr<Ast>& node);

template <typename AstNode>
Ptr<JsonArray> NodesToJson(const Vector<Ptr<AstNode>>& nodes) {
  auto array = New<JsonArray>();
  for (const auto& node : nodes) {
    array->Add(TreeToJson(node));
  }
  return array;
}

Ptr<JsonObject> TypeToJson(const Ptr<TypeExpression>& type);

Ptr<JsonObject> ProgramToJson(const Program& program);

Ptr<JsonObject> ConvertBinaryNodeToJson(const Ptr<Ast>& node);

void Test1();

void Test2();

void Test3();

void TestAll();

};  // namespace TestParser

#endif  // TESTPARSER_HPP
