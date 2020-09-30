#include "ConsoleApp.hpp"
#include "Compiler.hpp"
#include "Lexer.hpp"
#include "Nanopass.hpp"
#include "Parser.hpp"
#include "TypeChecker.hpp"
#include "Visitor.hpp"
#include <iostream>

using std::cout;
using std::endl;

namespace cygni {
std::shared_ptr<SourceDocument> ConsoleApp::ParseProgram(std::string path) {
  auto document = std::make_shared<FileLocation>(path, path);

  /* pass 1: read code in text format */
  std::string code{ReadText(path)};
  std::u32string utf32Code = UTF8ToUTF32(code);

  /* pass 2: tokenize code */
  Lexer lexer(document, utf32Code);
  auto tokens = lexer.ReadAll();

  /* pass 3: parse the code and create an abstract syntax tree */
  Parser parser(tokens, document);
  return parser.ParseProgram();
}

Project ConsoleApp::ParseProject(std::vector<std::string> fileList) {
  Project project;
  for (auto path : fileList) {
    auto program = ParseProgram(path);
    project.programs.insert({path, program});
  }
  return project;
}

void ConsoleApp::SemanticAnalysis(Project &project) {
  /* pass 4: collect the information of all packages */
  PackageInfoCollectPass packageInfoCollectPass;
  packageInfoCollectPass.CollectInfo(project);

  ModuleFieldsInitializationPass moduleFieldsInitializationPass;
  moduleFieldsInitializationPass.VisitProject(project);

  AddVoidReturnPass addVoidReturnPass;
  addVoidReturnPass.VisitProject(project);

  /* pass 5: resolve the super types */
  InheritanceTypeResolver inheritanceTypeResolver;
  inheritanceTypeResolver.VisitProject(project);

  /* pass 6: process inheritance */
  InheritanceProcessor inheritanceProcesser;
  inheritanceProcesser.VisitProject(project);

  DumpAbstractSyntaxTree(project, "passes/inheritance-process-pass.json");

  /* pass 7: check and infer types of each node */
  TypeChecker typeChecker(project);
  typeChecker.CheckProject();
  cout << "Complete Type Checking!" << endl;

  /* pass 8: create ArrayLength node*/
  ArrayLengthPass arrayLengthPass;
  arrayLengthPass.VisitProject(project);

  /* pass 9: generate virtual tables */
  VirtualTableGenerator virtualTableGenerator(project);
  virtualTableGenerator.VisitProject(project);

  DumpAbstractSyntaxTree(project, "passes/pass-virtual-table.json");

  /* pass 10: collect local variables */
  LocalVariableCollector localVariableCollector;
  localVariableCollector.VisitProject(project);
  cout << "Complete Local Variable Collection!" << endl;

  /* pass 11: locate variables */
  VariableLocator variableLocator(project);
  variableLocator.VisitProject();
  cout << "Complete Local Variable Locatoring!" << endl;

  /* pass 12: add 'this' pointer */
  HandleThisPointerPass handleThisPointerPass(project);
  handleThisPointerPass.VisitProject(project);
  DumpAbstractSyntaxTree(project, "passes/pass-this-pointer.json");

  /* pass 13: collect constants */
  ConstantCollector constantCollector;
  constantCollector.VisitProject(project);
  cout << "Complete Constant Collection!" << endl;
}

void ConsoleApp::DumpAbstractSyntaxTree(Project &project,
                                        std::string outputJsonPath) {
  cygni::AstToJsonSerialization astToJson;
  auto jsonObj = astToJson.VisitProject(project);
  auto jsonText = jsonObj.dump();
  cygni::WriteText(outputJsonPath, jsonText);
  cout << "Output AST to JSON format file '" << outputJsonPath << "' !" << endl;
}

void ConsoleApp::Compile(std::vector<std::string> fileList,
                         std::string outputExePath) {
  auto project = ParseProject(fileList);
  SemanticAnalysis(project);

  Compiler compiler(project);
  Executable exe = compiler.Compile();
  ViewExe(exe);
  ByteCode byteCode = CompileExe(exe);
  WriteBytes(outputExePath, byteCode.bytes);
  cout << "Total bytes in the compiled executable file: " << byteCode.Size()
       << endl;
}
} // namespace cygni
