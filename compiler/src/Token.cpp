#include "Token.hpp"
#include <unordered_map>

using std::unordered_map;

Json Token::ToJson() const {
  return unordered_map<string, Json>({{"Position", pos.ToJson()},
                                      {"Tag", Enum<Tag>::ToString(tag)},
                                      {"Text", text}});
}