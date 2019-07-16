#include "TestParser.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <algorithm>
#include "File.hpp"

using namespace std;


void TestParser::Test1()
{
    Lexer lexer("a + b * 2");
    std::vector<Token> tokens = lexer.ReadAll();
    Parser parser(String(), tokens);
    auto tree = parser.ParseAssign();
    std::ofstream stream("test_output/ast-test1.txt");
    auto json = TreeToJson(tree);
    cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
    JsonToFile(json, stream);
    stream.close();
}

void TestParser::TestAll()
{
    Test1();
    Test2();
    Test3();
}

Ptr<JsonObject> TestParser::TreeToJson(const Ptr<Ast> &node)
{
    auto properties = [](const Ptr<Ast> &node, Ptr<JsonMap> &map)
    {
        map->Add("kind", New<JsonValue>(KindToString(node->kind)));
        map->Add("id", New<JsonValue>(String::ToString(node->id)));
    };
    switch (node->kind)
    {
        case Kind::Constant:
        {
            auto constant = Cast<Constant>(node);
            auto map = New<JsonMap>();
            map->Add("constant", New<JsonValue>(constant->value));
            properties(constant, map);
            return map;
        }
        case Kind::Name:
        {
            auto name = Cast<Name>(node);
            auto map = New<JsonMap>();
            properties(name, map);
            map->Add("name", New<JsonValue>(name->name));
            return map;
        }
        case Kind::Block:
        {
            auto block = Cast<Block>(node);
            auto map = New<JsonMap>();
            properties(block, map);
            map->Add("expressions", NodesToJson(block->expressions));
            return map;
        }
        case Kind::IfThen:
        {
            auto ifThen = Cast<IfThen>(node);
            auto map = New<JsonMap>();
            properties(ifThen, map);
            map->Add("condition", TreeToJson(ifThen->condition));
            map->Add("if true", TreeToJson(ifThen->ifTrue));
            return map;
        }
        case Kind::IfElse:
        {
            auto ifElse = Cast<IfElse>(node);
            auto map = New<JsonMap>();
            properties(ifElse, map);
            map->Add("condition", TreeToJson(ifElse->condition));
            map->Add("if true", TreeToJson(ifElse->ifTrue));
            map->Add("if false", TreeToJson(ifElse->ifFalse));
            return map;
        }
        case Kind::Return:
        {
            auto returnNode = Cast<Return>(node);
            auto map = New<JsonMap>();
            properties(returnNode, map);
            map->Add("value", TreeToJson(returnNode->value));
            return map;
        }
        case Kind::Var:
        {
            auto var = Cast<Var>(node);
            auto map = New<JsonMap>();
            properties(var, map);
            map->Add("name", New<JsonValue>(var->name));
            if (var->value)
            {
                map->Add("value", TreeToJson(*(var->value)));
            }
            if (var->type)
            {
                map->Add("type", TypeToJson(*(var->type)));
            }
            return map;
        }
        case Kind::Def:
        {
            auto def = Cast<Def>(node);
            auto map = New<JsonMap>();
            properties(def, map);
            map->Add("name", New<JsonValue>(def->name));
            map->Add("body", TreeToJson(def->body));
            map->Add("type", TypeToJson(def->type));
            return map;
        }
        case Kind::DefClass:
        {
            auto defClass = Cast<DefClass>(node);
            auto map = New<JsonMap>();
            properties(defClass, map);
            map->Add("name", New<JsonValue>(defClass->name));
            map->Add("fields", NodesToJson(defClass->fields));
            map->Add("methods", NodesToJson(defClass->methods));
            return map;
        };
        case Kind::DefModule:
        {
            auto defModule = Cast<DefModule>(node);
            auto map = New<JsonMap>();
            properties(defModule, map);
            map->Add("name", New<JsonValue>(defModule->name));
            map->Add("fields", NodesToJson(defModule->fields));
            map->Add("methods", NodesToJson(defModule->methods));
            return map;
        };
        case Kind::Call:
        {
            auto call = Cast<Call>(node);
            auto map = New<JsonMap>();
            properties(call, map);
            map->Add("function", TreeToJson(call->function));
            map->Add("arguments", NodesToJson(call->arguments));
            return map;
        }
        case Kind::Add:
        case Kind::Subtract:
        case Kind::Multiply:
        case Kind::Divide:
        case Kind::Modulo:
        case Kind::GreaterThan:
        case Kind::LessThan:
        case Kind::GreaterThanOrEqual:
        case Kind::LessThanOrEqual:
        case Kind::Equal:
        case Kind::NotEqual:
        case Kind::And:
        case Kind::Or:
        case Kind::Assign:
            return ConvertBinaryNodeToJson(node);
        default:
        {
            cout << KindToString(node->kind) << endl;
            cout << "exit" << endl;
            exit(1);
        }
    }
}

void TestParser::Test2()
{
    Lexer lexer(File::ReadFile("test_code/code2.txt"));
    std::vector<Token> tokens = lexer.ReadAll();
    Parser parser(String(), tokens);
    try
    {
        auto program = parser.ParseProgram();
        std::ofstream stream("test_output/ast-test2.txt");
        auto json = TreeToJson(program.classes.front());
        cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
        JsonToFile(json, stream);
        stream.close();
    } catch (ParserException &ex)
    {
        cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
    }
}
    Ptr<JsonObject> TestParser::ConvertBinaryNodeToJson(const Ptr<Ast> &node)
    {
        auto expr = Cast<Binary>(node);
        auto map = New<JsonMap>();
        Kind kind = node->kind;
        map->Add("kind", New<JsonValue>(KindToString(kind)));
        map->Add("left", TreeToJson(expr->left));
        map->Add("right", TreeToJson(expr->right));
        map->Add("id", New<JsonValue>(String::ToString(node->id)));
        return map;
    }
Ptr<JsonObject> TestParser::TypeToJson(const Ptr<TypeExpression> &type)
{
    auto map = New<JsonMap>();
    auto array = New<JsonArray>();
    if (!(type->parameters.empty()))
    {
        map->Add("name", New<JsonValue>(type->name));
        map->Add("kind", New<JsonValue>("Type List"));
        for (const auto &item: type->parameters)
        {
            array->items.push_back(TypeToJson(item));
        }
        map->Add("parameters", array);
    }
    else
    {
        map->Add("name", New<JsonValue>(type->name));
        map->Add("kind", New<JsonValue>("Type Leaf"));
    }
    return map;
}

void TestParser::Test3()
{
    Lexer lexer(File::ReadFile("test_code/code3.txt"));
    std::vector<Token> tokens = lexer.ReadAll();
    Parser parser(String(), tokens);
    try
    {
        auto program = parser.ParseProgram();
        std::ofstream stream("test_output/ast-test3.json");
        auto json = ProgramToJson(program);
        cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
        JsonToFile(json, stream);
        stream.close();
    } catch (ParserException &ex)
    {
        cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
    }
}

Ptr<JsonObject> TestParser::ProgramToJson(const Program &program)
{
    auto map = New<JsonMap>();
    auto classes = New<JsonArray>();
    auto modules = New<JsonArray>();
    for (const auto &_class: program.classes)
    {
        classes->items.push_back(TreeToJson(_class));
    }
    for (const auto &module:program.modules)
    {
        modules->items.push_back(TreeToJson(module));
    }
    map->Add("classes", classes);
    map->Add("modules", modules);
    return map;
}