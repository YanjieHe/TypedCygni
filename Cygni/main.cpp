#include "Expression.hpp"
#include "Lexer.hpp"
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

using namespace std;

std::string utf32_to_utf8(const std::u32string& utf32) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
	std::string utf8 = cnv.to_bytes(utf32);
	if (cnv.converted() < utf32.size()) {
		throw std::runtime_error("incomplete conversion");
	} else {
		return utf8;
	}
}

std::ostream& operator<<(std::ostream& stream, const std::u32string& utf32) {
	stream << utf32_to_utf8(utf32);
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
