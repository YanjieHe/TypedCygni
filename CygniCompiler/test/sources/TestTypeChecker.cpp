#include "TestTypeChecker.hpp"
#include <fstream>
#include <iostream>
#include "File.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "TestParser.hpp"

using namespace std;

void TestTypeChecker::TestAll() {
  Test1();
}

TypeChecker::ScopeCollection TestTypeChecker::InitializeScopeCollection() {
  TypeChecker::ScopeCollection scopes;
  scopes.functionScope = New<Scope<Ptr<FunctionType>>>();
  scopes.typeScope = InitializeTypeScope();
  scopes.identifierScope = New<Scope<Ptr<Type>>>();
  scopes.operatorScope = InitializeOperatorScope();
  return scopes;
}
Ptr<Scope<Vector<Ptr<FunctionType>>>>
TestTypeChecker::InitializeOperatorScope() {
  auto Function = [](const Vector<Ptr<Type>>& parameters,
                     const Ptr<Type>& returnValue) {
    return New<FunctionType>(parameters, returnValue);
  };
  auto numberComputation = Vector<Ptr<FunctionType>>{
      Function({Type::INT, Type::INT}, Type::INT),
      Function({Type::FLOAT, Type::FLOAT}, Type::FLOAT),
      Function({Type::LONG, Type::LONG}, Type::LONG),
      Function({Type::DOUBLE, Type::DOUBLE}, Type::DOUBLE)};

  auto comparision = Vector<Ptr<FunctionType>>{
      Function({Type::INT, Type::INT}, Type::BOOL),
      Function({Type::FLOAT, Type::FLOAT}, Type::BOOL),
      Function({Type::LONG, Type::LONG}, Type::BOOL),
      Function({Type::DOUBLE, Type::DOUBLE}, Type::BOOL),
  };

  HashMap<String, Vector<Ptr<FunctionType>>> operators = {
      {"+", numberComputation}, {"-", numberComputation},
      {"*", numberComputation}, {"/", numberComputation},
      {"%", numberComputation},

      {">", comparision},       {"<", comparision},
      {">=", comparision},      {"<=", comparision},
      {"==", comparision},      {"!=", comparision},
  };

  auto scope = New<Scope<Vector<Ptr<FunctionType>>>>();
  for (const auto& item : operators) {
    scope->Put(item.first, item.second);
  }
  return scope;
}

Ptr<Scope<Ptr<Type>>> TestTypeChecker::InitializeTypeScope() {
  auto scope = New<Scope<Ptr<Type>>>();
  scope->Put("Int", Type::INT);
  scope->Put("Float", Type::FLOAT);
  scope->Put("Long", Type::LONG);
  scope->Put("Double", Type::DOUBLE);
  scope->Put("Bool", Type::BOOL);
  scope->Put("Void", Type::VOID);
  return scope;
}

void TestTypeChecker::Test1() {
  auto scopes = InitializeScopeCollection();
  Lexer lexer(File::ReadFile("test_code/code3.txt"));
  std::vector<Token> tokens = lexer.ReadAll();
  Parser parser(String(), tokens);
  try {
    auto program = parser.ParseProgram();
    cout << "type checking start" << endl;
    TypeChecker typeChecker;
    typeChecker.CheckProgram(program, scopes);
    cout << "type checking finished" << endl;
    // OutputTypeRecord("test_output/code3-types.txt", typeChecker.typeRecord);
    std::ofstream stream("test_output/ast-test3.json");
    auto json = TestParser::ProgramToJson(program);
    auto toString = [](const Ptr<Type>& type) { return type->ToString(); };
    AttachToJson<Ptr<Type>, decltype(toString)>(typeChecker.typeRecord, "type",
                                                json, toString);
    cout << __FUNCTION__ << ": successfully convert ast to json" << endl;
    JsonToFile(json, stream);
    stream.close();
  } catch (ParserException& ex) {
    cout << ex.line << ", " << ex.column << ": " << ex.Message() << endl;
  } catch (TypeException& ex) {
    cout << ex.position.ToString() << ": " << ex.Message() << endl;
  } catch (KeyNotFoundException& ex) {
    cout << ex.Message() << endl;
  }
}

void TestTypeChecker::OutputTypeRecord(
    const std::string& path,
    const HashMap<int, Ptr<Type>>& typeRecord) {
  std::ofstream stream(path);
  for (const auto& item : typeRecord) {
    stream << item.first << ": " << item.second->ToString() << endl;
  }
  stream.close();
}