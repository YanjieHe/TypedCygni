#include "Test_Lexer.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

bool SameToken(const Token& token, Tag tag) {
	return token.tag == tag;
}
bool SameToken(const Token& token, Tag tag, std::u32string text) {
	return token.tag == tag && token.text == text;
}
vector<Token> ParseTokensFromText(std::u32string text) {
	try {
		Lexer lexer(text);
		auto tokens = lexer.ReadAll();
		return tokens;
	} catch (LexicalException& ex) {
		return { };
	}
}
std::string utf32_to_utf8(const std::u32string& utf32) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
	std::string utf8 = cnv.to_bytes(utf32);
	if (cnv.converted() < utf32.size()) {
		throw std::runtime_error("incomplete conversion");
	} else {
		return utf8;
	}
}
std::u32string to_utf32(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

vector<Token> ParseTokensFromSource(std::string path) {
	std::ifstream stream(path);
	if (stream.is_open()) {
		// std::streampos size = stream.tellg();
		
		std::string text{(std::istreambuf_iterator<char>(stream)),
						std::istreambuf_iterator<char>()};
		std::u32string code = to_utf32(text);
		return ParseTokensFromText(code);
	} else {
		return {};
	}
}
