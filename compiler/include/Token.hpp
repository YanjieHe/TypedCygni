#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "Enum.hpp"
#include <string>
#include <memory>

using std::string;

class Token {
public:
  int line;
  int col;
  Tag tag;
  string text;

  Token() : line{-1}, col{-1}, tag{}, text{} {}

  Token(int line, int col, Tag tag, string text)
      : line{line}, col{col}, tag{tag}, text{text} {}
};
#endif // TOKEN_HPP