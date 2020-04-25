#include "ConsoleApp.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Visitor.hpp"
#include <iostream>
using std::cout;
using std::endl;

namespace cygni
{
	Program ConsoleApp::ParseProgram(std::string path)
	{
		/* pass 1: read code in text format */
		std::string code{ ReadText(path) };
		std::u32string utf32Code = cygni::UTF8ToUTF32(code);

		/* pass 2: tokenize code */
		Lexer lexer(utf32Code);
		auto tokens = lexer.ReadAll();

		/* pass 3: parse the code and create an abstract syntax tree */
		auto document = std::make_shared<cygni::SourceDocument>(path, path);
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
		return project;
	}

	void ConsoleApp::CompileProject(Project & project, std::string outputJsonPath)
	{
		/* pass 4: check and infer types of each node */
		TypeChecker typeChecker(project);
		ScopePtr globalScope = std::make_shared<cygni::Scope>();
		typeChecker.VisitProject(globalScope);
		cout << "Complete Type Checking!" << endl;

		/* pass 5: collect local variables */
		LocalVariableCollector localVariableCollector;
		localVariableCollector.VisitProject(project);
		cout << "Complete Local Variable Collection!" << endl;

		/* pass 6: locate variables */
		VariableLocator variableLocator;
		variableLocator.VisitProject(project);
		cout << "Complete Local Variable Locatoring!" << endl;

		/* pass 7: collect constants */
		ConstantCollector constantCollector;
		constantCollector.VisitProject(project);
		cout << "Complete Constant Collection!" << endl;

		/* pass 8: convert the abstract syntax tree to json format */
		cygni::AstToJsonSerialization astToJson;
		auto jsonObj = astToJson.VisitProject(project);
		auto jsonText = jsonObj.dump();
		cygni::WriteText(outputJsonPath, jsonText);
		cout << "Output AST in JSON format!" << endl;
	}

	int ConsoleApp::Run(int argc, char ** argv)
	{
		std::vector<std::string> fileList = {
			"sample_code/factorial.cyg"
		};
		Project project = ParseProject(fileList);
		CompileProject(project, "sample_code/factorial.json");
		return 0;
	}
}
