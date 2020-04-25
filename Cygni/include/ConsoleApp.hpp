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

		void CompileProject(Project& project ,std::string outputJsonPath);
		int Run(int argc, char **argv);
	};
}

#endif // CYGNI_CONSOLE_APP_HPP