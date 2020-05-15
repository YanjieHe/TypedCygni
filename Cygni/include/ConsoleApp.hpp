#ifndef CYGNI_CONSOLE_APP_HPP
#define CYGNI_CONSOLE_APP_HPP
#include <string>
#include "Expression.hpp"

namespace cygni
{
	class ConsoleApp
	{
	public:
		Program ParseProgram(std::string path);
		Project ParseProject(std::vector<std::string> fileList);

		void SemanticAnalysis(Project& project);

		void DumpAbstractSyntaxTree(std::vector<std::string> fileList, std::string outputJsonPath);

		void Compile(std::vector<std::string> fileList, std::string outputExePath);
		int Run(int argc, char **argv);
	};
}

#endif // CYGNI_CONSOLE_APP_HPP