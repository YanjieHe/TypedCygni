#include "Expression.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Visitor.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

std::ostream& operator<<(std::ostream& stream, const std::u32string& utf32) {
	stream << cygni::utf32_to_utf8(utf32);
	return stream;
}

void ViewTokens(const std::vector<cygni::Token>& tokens) {
	for (const cygni::Token& token : tokens) {
		cout << cygni::Enum<cygni::Tag>::ToString(token.tag) << ": ";
		cout << token.text << endl;
	}
}

void WriteText(std::string path, std::string text) {
	std::ofstream stream(path);
	stream << text;
}

void CompileProgram(std::string path) {
	std::ifstream stream(path);
	if (stream) {
		std::string code{(std::istreambuf_iterator<char>(stream)),
						 std::istreambuf_iterator<char>()};
		std::u32string utf32Code = cygni::utf8_to_utf32(code);
		cout << cygni::utf32_to_utf8(utf32Code) << endl;
		cout << "文件读取成功！" << endl;
		cygni::Lexer lexer(utf32Code);
		auto tokens = lexer.ReadAll();
		cout << "词法分析完成！" << endl;
		ViewTokens(tokens);

		auto document = std::make_shared<cygni::SourceDocument>(path, path);
		cygni::Parser parser(tokens, document);
		auto program = parser.ParseProgram();
		cout << "语法分析完成！" << endl;

		cygni::AstToJsonSerialization astToJson;
		auto jsonObj  = astToJson.VisitProgram(program);
		auto jsonText = jsonObj.dump();
		WriteText("ast.json", jsonText);
		
	} else {
		cout << "文件读取失败！" << endl;
	}
}

int main(int argc, char** argv) {
	cout << "Hello Cygni!" << endl;
	std::u32string s = U"如何从零写一个正则表达式引擎？";
	try {
		CompileProgram("test1.cyg");
	} catch (cygni::ArgumentException& ex) {
		cout << cygni::utf32_to_utf8(ex.message) << endl;
	} catch (cygni::LexicalException& ex) {
		cout << cygni::utf32_to_utf8(ex.message) << endl;
	} catch (cygni::ParserException& ex) {
		cout << cygni::utf32_to_utf8(ex.message) << endl;
	}
	// cygni::Lexer lexer(U"var a = '\\x0058'");
	// try {
	// 	auto tokens = lexer.ReadAll();
	// 	for (const cygni::Token& token : tokens) {
	// 		cout << cygni::Enum<cygni::Tag>::ToString(token.tag) << ": ";
	// 		cout << token.text << endl;
	// 	}
	// } catch (cygni::LexicalException& ex) {
	// 	cout << "(" << ex.line << ", " << ex.column << "): ";
	// 	cout << ex.message << endl;
	// }
	// cout << "Press any key to close..." << endl;
	// cin.get();
	return 0;
}
