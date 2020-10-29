#include "Position.hpp"
#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

Json Position::ToJson() const {
  return unordered_map<string, Json>{{"Start Line", startLine},
                                     {"Start Column", startCol},
                                     {"End Line", endLine},
                                     {"End Column", endCol}};
}
