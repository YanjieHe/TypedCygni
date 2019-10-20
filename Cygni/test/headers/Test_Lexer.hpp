#ifndef CYGNI_TEST_LEXER_HPP
#define CYGNI_TEST_LEXER_HPP
#include "Token.hpp"
#include <vector>
using namespace cygni;
using std::vector;

bool SameToken(const Token& token, Tag tag);
bool SameToken(const Token& token, Tag tag, std::u32string text);
vector<Token> ParseTokensFromText(std::u32string text);

std::string utf32_to_utf8(const std::u32string& utf32);
std::u32string to_utf32(const std::string &s);
vector<Token> ParseTokensFromSource(std::string path);

#endif // CYGNI_TEST_LEXER_HPP
