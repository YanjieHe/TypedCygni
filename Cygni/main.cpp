#include "Expression.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <string>

using namespace std;

std::ostream& operator<<(std::ostream& stream, const std::u32string& utf32) {
	stream << cygni::utf32_to_utf8(utf32);
	return stream;
}

int main(int argc, char** argv) {
	cout << "Hello Cygni!" << endl;
	std::u32string s = U"如何从零写一个正则表达式引擎？";
	cygni::Lexer lexer(U"var a = '\\x0058'");
	try {
		auto tokens = lexer.ReadAll();
		for (const cygni::Token& token : tokens) {
			cout << cygni::Enum<cygni::Tag>::ToString(token.tag) << ": ";
			cout << token.text << endl;
		}
	} catch (cygni::LexicalException& ex) {
		cout << "(" << ex.line << ", " << ex.column << "): ";
		cout << ex.message << endl;
	}
	cout << "Press any key to close..." << endl;
	cin.get();
	return 0;
}
