#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "Enum.hpp"
#include <string>
#include "IJsonSerializable.hpp"
#include "Position.hpp"

using std::string;

class Token : public IJsonSerializable {
public:
  Position pos;
  Tag tag;
  string text;

  Token() : pos{}, tag{}, text{} {}

  Token(Position pos, Tag tag, string text) : pos{pos}, tag{tag}, text{text} {}
  Json ToJson() const override;
};
#endif // TOKEN_HPP