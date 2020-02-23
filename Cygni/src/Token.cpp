#include "Token.hpp"
#include <iostream>

namespace cygni {

Token::Token(int line, int column, Tag tag, const std::u32string &text)
    : line{line}, column{column}, tag{IdentifyKeyword(tag, text)}, text{text} {}

Tag Token::IdentifyKeyword(Tag tag, const std::u32string &text) {
  static std::unordered_map<std::u32string, Tag> keywords = {
      {U"if", Tag::If},
      {U"else", Tag::Else},
      {U"var", Tag::Var},
      {U"def", Tag::Def},
      {U"return", Tag::Return},
      {U"true", Tag::True},
      {U"false", Tag::False},
      {U"while", Tag::While},
      {U"new", Tag::New},
      {U"class", Tag::Class},
      {U"private", Tag::Private},
      {U"module", Tag::Module},
      {U"protected", Tag::Protected},
      {U"package", Tag::Package}};
  if (tag == Tag::Identifier) {
    if (keywords.find(text) != keywords.end()) {
      return keywords[text];
    } else {
      return tag;
    }
  } else {
    return tag;
  }
}
} // namespace cygni
