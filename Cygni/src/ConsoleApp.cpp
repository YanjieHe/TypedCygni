#include "ConsoleApp.hpp"
#include "Parser.hpp"

namespace cygni
{
	Program ConsoleApp::ParseProgram(std::string path)
	{
		/* pass 1: read code in text format */
		std::string code{ ReadText() };
		std::u32string utf32Code = cygni::UTF8ToUTF32(code);

		/* pass 2: tokenize code */
		cygni::Lexer lexer(utf32Code);
		auto tokens = lexer.ReadAll();

		/* pass 3: parse the code and create an abstract syntax tree */
		auto document = std::make_shared<cygni::SourceDocument>(path, path);
		Parser parser(tokens, document);
		return parser.ParseProgram();
	}

	int ConsoleApp::Run(int argc, char ** argv)
	{
		return 0;
	}
}
