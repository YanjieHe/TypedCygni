#ifndef CYGNI_CONSOLE_APP_HPP
#define CYGNI_CONSOLE_APP_HPP
#include <string>
#include "Expression.hpp"

namespace cygni
{
	class ConsoleApp
	{
	public:
		std::shared_ptr<SourceDocument> ParseProgram(std::string path);
		Project ParseProject(std::vector<std::string> fileList);

		void SemanticAnalysis(Project& project);

		void DumpAbstractSyntaxTree(Project& project, std::string outputJsonPath);

		void Compile(std::vector<std::string> fileList, std::string outputExePath);
	};
}

#endif // CYGNI_CONSOLE_APP_HPP