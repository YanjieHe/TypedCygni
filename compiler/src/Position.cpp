#include "Position.hpp"
#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

Json Source::ToJson() const { return JsonMap{{"Path", path}}; }

Json Position::ToJson() const {
  return JsonMap{{"Source", source->ToJson()},
                 {"Start Line", startLine},
                 {"Start Column", startCol},
                 {"End Line", endLine},
                 {"End Column", endCol}};
}
