#ifndef CYGNI_CONSOLE_APP_HPP
#define CYGNI_CONSOLE_APP_HPP
#include <string>

namespace cygni
{
	class ConsoleApp
	{
	public:
		Program ParseProgram(std::string path);
		int Run(int argc, char **argv);
	};
}

#endif // CYGNI_CONSOLE_APP_HPP