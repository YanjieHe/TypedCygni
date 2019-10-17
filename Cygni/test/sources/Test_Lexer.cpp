#include "Test_Lexer.hpp"
#include "Lexer.hpp"

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
