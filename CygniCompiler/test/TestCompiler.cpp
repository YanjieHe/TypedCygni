#include "TestCompiler.hpp"
#include "TestTypeChecker.hpp"
#include "../File.hpp"
#include "../Lexer.hpp"
#include "../Parser.hpp"
#include "TestParser.hpp"
#include "../Locator.hpp"
#include <fstream>
#include "../Location.hpp"
#include "../Compiler.hpp"

void TestCompiler::TestAll()
{
    Test1();
}

void TestCompiler::Test1()
{
//    auto scope = TestTypeChecker::InitializeBasicTypes();
//    Lexer lexer(File::ReadFile("code3.txt"));
//    std::vector<Token> tokens = lexer.ReadAll();
//    Parser parser(String(), tokens);
//    try
//    {
//        auto program = parser.ParseProgram();
//        cout << "type checking start" << endl;
//        TypeChecker typeChecker;
//        typeChecker.CheckProgram(program, scope);
//        cout << "type checking finished" << endl;
//        TestTypeChecker::OutputTypeRecord("code3-types.txt", typeChecker.typeRecord);
//        std::ofstream stream("ast-test3.txt");
//        auto json = TestParser::ProgramToJson(program);
//        cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
//        JsonToFile(json, stream);
//        stream.close();
//        Locator locator;
//        locator.Locate(program, TestTypeChecker::InitializeBasicTypes());
//        for (const auto &item: locator.locations)
//        {
//            cout << item.first << ": " << item.second.ToString() << endl;
//        }
//        Compiler compiler(typeChecker.typeRecord, locator.locations);
//        auto code = compiler.Compile(program);
//        for (const auto &byte: code)
//        {
//            cout << static_cast<int>(byte) << endl;
//        }
//    }
//    catch (ParserException &ex)
//    {
//        cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
//    }
//    catch (TypeException &ex)
//    {
//        cout << ex.position.ToString() << ": " << ex.Message() << endl;
//    }
//    catch (KeyNotFoundException &ex)
//    {
//        cout << ex.Message() << endl;
//    }
//    catch (ArgumentException &ex)
//    {
//        cout << ex.Message() << endl;
//    }
}
