#ifndef TESTPARSER_HPP
#define TESTPARSER_HPP

#include "../Parser.hpp"
#include <fstream>
#include "JSON.hpp"

namespace TestParser
{
    template<typename AstNode>
    Ptr<JsonArray> NodesToJson(const Vector<Ptr<AstNode>> &nodes)
    {
        auto array = New<JsonArray>();
        for (const auto &node: nodes)
        {
            array->Add(TreeToJson(node));
        }
        return array;
    }

    Ptr<JsonObject> TreeToJson(const Ptr<Ast> &node);

    Ptr<JsonObject> TypeToJson(const Ptr<TypeExpression> &type);

    Ptr<JsonObject> ProgramToJson(const Program &program);

    template<Kind kind>
    Ptr<JsonObject> ConvertBinaryNodeToJson(const Ptr<Ast> &node)
    {
        auto expr = Cast<Binary<kind>>(node);
        auto map = New<JsonMap>();
        map->Add("kind", New<JsonValue>(KindToString(kind)));
        map->Add("left", TreeToJson(expr->left));
        map->Add("right", TreeToJson(expr->right));
        map->Add("id", New<JsonValue>(String::ToString(node->id)));
        return map;
    }

    void Test1();

    void Test2();

    void Test3();

    void TestAll();

};


#endif //TESTPARSER_HPP
