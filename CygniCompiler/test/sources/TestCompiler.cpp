#include "TestCompiler.hpp"
#include <fstream>
#include "Compiler.hpp"
#include "File.hpp"
#include "Lexer.hpp"
#include "Locator.hpp"
#include "Parser.hpp"
#include "TestParser.hpp"
#include "TestTypeChecker.hpp"

void TestCompiler::TestAll() {
  Test1();
}

void TestCompiler::Test1() {
  auto scopes = TestTypeChecker::InitializeScopeCollection();
  Lexer lexer(File::ReadFile("test_code/code3.txt"));
  std::vector<Token> tokens = lexer.ReadAll();
  Parser parser(String(), tokens);
  try {
    auto program = parser.ParseProgram();
    cout << "type checking start" << endl;
    TypeChecker typeChecker;
    typeChecker.CheckProgram(program, scopes);
    cout << "type checking finished" << endl;
    auto locatorScopes = Locator::ScopeCollection();
    Locator locator;
    locator.Locate(program, locatorScopes);
    for (const auto& item : locator.locations) {
      cout << item.first << ": " << item.second.ToString() << endl;
    }
    std::ofstream stream("test_output/ast-test3.json");
    auto json = TestParser::ProgramToJson(program);
    auto toString = [](const Location& location) {
      return location.ToString();
    };
    TestTypeChecker::AttachToJson<Location, decltype(toString)>(
        locator.locations, "location", json, toString);
    JsonToFile(json, stream);
    stream.close();
    Compiler compiler(typeChecker.typeRecord, locator, locator.locations);
    compiler.Compile(program);
  } catch (ParserException& ex) {
    cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
  } catch (TypeException& ex) {
    cout << ex.position.ToString() << ": " << ex.Message() << endl;
  } catch (KeyNotFoundException& ex) {
    cout << ex.Message() << endl;
  } catch (ArgumentException& ex) {
    cout << ex.Message() << endl;
  }
}
