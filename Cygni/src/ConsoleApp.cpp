#include "ConsoleApp.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Visitor.hpp"
#include "Compiler.hpp"
#include <iostream>
using std::cout;
using std::endl;

namespace cygni
{
	SourceDocument ConsoleApp::ParseProgram(std::string path)
	{
		auto document = std::make_shared<FileLocation>(path, path);

		/* pass 1: read code in text format */
		std::string code{ ReadText(path) };
		std::u32string utf32Code = UTF8ToUTF32(code);

		/* pass 2: tokenize code */
		Lexer lexer(document, utf32Code);
		auto tokens = lexer.ReadAll();

		/* pass 3: parse the code and create an abstract syntax tree */
		Parser parser(tokens, document);
		return parser.ParseProgram();
	}

	Project ConsoleApp::ParseProject(std::vector<std::string> fileList)
	{
		Project project;
		for (auto path : fileList)
		{
			auto program = ParseProgram(path);
			project.programs.Add(path, program);
		}
		project.MergeAllPrograms();
		AssignIndex(project);
		return project;
	}

	void ConsoleApp::SemanticAnalysis(Project & project)
	{
		PackageImporter packageImporter;
		packageImporter.ImportPackages(project);

		/* pass 4: rename all the type that needs alias */
		TypeRenamer typeRenamer;
		typeRenamer.RenameAll(project);

		/* pass 5: process inheritance */
		InheritanceProcessor inheritanceProcesser;
		inheritanceProcesser.VisitProject(project);

		/* pass 6: check and infer types of each node */
		TypeChecker typeChecker(project);
		auto scopeFactory = ScopeFactory<TypePtr>::Create();
		typeChecker.scopeFactory = scopeFactory;
		auto globalScope = scopeFactory->New();
		typeChecker.CheckProject(globalScope);
		cout << "Complete Type Checking!" << endl;

		/* pass 7: collect local variables */
		LocalVariableCollector localVariableCollector;
		localVariableCollector.VisitProject(project);
		cout << "Complete Local Variable Collection!" << endl;

		/* pass 8: locate variables */
		VariableLocator variableLocator(project);
		variableLocator.VisitProject();
		cout << "Complete Local Variable Locatoring!" << endl;

		/* pass 9: collect constants */
		ConstantCollector constantCollector;
		constantCollector.VisitProject(project);
		cout << "Complete Constant Collection!" << endl;
	}

	void ConsoleApp::DumpAbstractSyntaxTree(std::vector<std::string> fileList, std::string outputJsonPath)
	{
		auto project = ParseProject(fileList);
		SemanticAnalysis(project);

		/* pass 10: convert the abstract syntax tree to json format */
		cygni::AstToJsonSerialization astToJson;
		auto jsonObj = astToJson.VisitProject(project);
		auto jsonText = jsonObj.dump();
		cygni::WriteText(outputJsonPath, jsonText);
		cout << "Output AST in JSON format!" << endl;
	}

	void ConsoleApp::Compile(std::vector<std::string> fileList, std::string outputExePath)
	{
		auto project = ParseProject(fileList);
		SemanticAnalysis(project);

		Compiler compiler(project);
		ByteCode byteCode = compiler.Compile();
		WriteBytes(outputExePath, byteCode.bytes);
		cout << "Total bytes in the compiled executable file: " << byteCode.Size() << endl;
	}

	int ConsoleApp::Run(int argc, char ** argv)
	{
		std::vector<std::string> fileList = {
			"sample_code/factorial.cyg"
		};
		DumpAbstractSyntaxTree(fileList, "sample_code/factorial.json");
		return 0;
	}
}
