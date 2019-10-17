#ifndef CYGNI_TEST_LEXER_HPP
#define CYGNI_TEST_LEXER_HPP
#include "Token.hpp"
#include <vector>
using namespace cygni;
using std::vector;

bool SameToken(const Token& token, Tag tag);
bool SameToken(const Token& token, Tag tag, std::u32string text);
vector<Token> ParseTokensFromText(std::u32string text);

#endif // CYGNI_TEST_LEXER_HPP
