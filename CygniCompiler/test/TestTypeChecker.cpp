#include "TestTypeChecker.hpp"
#include "../Lexer.hpp"
#include "../File.hpp"
#include "../Parser.hpp"
#include "TestParser.hpp"
#include <iostream>
#include <fstream>

using namespace std;

void TestTypeChecker::TestAll()
{
    Test1();
}

Ptr<Scope> TestTypeChecker::InitializeBasicTypes()
{
    auto Function = [](const Vector<Ptr<Type>> &parameters, const Ptr<Type> &returnValue)
    {
        return New<FunctionType>(parameters, returnValue);
    };
    auto intValue = New<IntType>();
    auto floatValue = New<FloatType>();
    auto longValue = New<LongType>();
    auto doubleValue = New<DoubleType>();
    auto boolValue = New<BoolType>();
    auto numberComputation = Vector<Ptr<FunctionType>>{
            Function({intValue, intValue}, intValue),
            Function({floatValue, floatValue}, floatValue),
            Function({longValue, longValue}, longValue),
            Function({doubleValue, doubleValue}, doubleValue)
    };

    auto comparision = Vector<Ptr<FunctionType>>{
            Function({intValue, intValue}, boolValue),
            Function({floatValue, floatValue}, boolValue),
            Function({longValue, longValue}, boolValue),
            Function({doubleValue, doubleValue}, boolValue),
    };
    HashMap<String, Vector<Ptr<FunctionType>>> operators = {
            {"+",  numberComputation},
            {"-",  numberComputation},
            {"*",  numberComputation},
            {"/",  numberComputation},
            {"%",  numberComputation},

            {">",  comparision},
            {"<",  comparision},
            {">=", comparision},
            {"<=", comparision},
            {"==", comparision},
            {"!=", comparision},
    };
    auto scope = New<Scope>();
    for (const auto &item: operators)
    {
        scope->Put(item.first, "**Operator**", New<Vector<Ptr<FunctionType>>>(item.second));
    }
    scope->Put("Int", "**Type**", New<IntType>());
    scope->Put("Float", "**Type**", New<FloatType>());
    scope->Put("Long", "**Type**", New<LongType>());
    scope->Put("Double", "**Type**", New<DoubleType>());
    scope->Put("Bool", "**Type**", New<BoolType>());
    return scope;
}

void TestTypeChecker::Test1()
{
    auto scope = InitializeBasicTypes();
    Lexer lexer(File::ReadFile("code3.txt"));
    std::vector<Token> tokens = lexer.ReadAll();
    Parser parser(String(), tokens);
    try
    {
        auto program = parser.ParseProgram();
        cout << "type checking start" << endl;
        TypeChecker typeChecker;
        typeChecker.CheckProgram(program, scope);
        cout << "type checking finished" << endl;
        OutputTypeRecord("code3-types.txt", typeChecker.typeRecord);
        std::ofstream stream("ast-test3.json");
        auto json = TestParser::ProgramToJson(program);
        cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
        JsonToFile(json, stream);
        stream.close();
    }
    catch (ParserException &ex)
    {
        cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
    }
    catch (TypeException &ex)
    {
        cout << ex.position.ToString() << ": " << ex.Message() << endl;
    }
    catch (KeyNotFoundException &ex)
    {
        cout << ex.Message() << endl;
    }
}

void TestTypeChecker::OutputTypeRecord(const std::string &path, const HashMap<int, Ptr<Type>> &typeRecord)
{
    std::ofstream stream(path);
    for (const auto &item: typeRecord)
    {
        stream << item.first << ": " << item.second->ToString() << endl;
    }
    stream.close();
}