#ifndef I_JSON_SERIALIZABLE_HPP
#define I_JSON_SERIALIZABLE_HPP
#include <nlohmann/json.hpp>

using Json = nlohmann::json;
using JsonMap = std::unordered_map<std::string, Json>;
using JsonArray = std::vector<Json>;

class IJsonSerializable {
public:
  virtual Json ToJson() const = 0;
};

#endif // I_JSON_SERIALIZABLE_HPP