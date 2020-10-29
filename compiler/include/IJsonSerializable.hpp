#ifndef I_JSON_SERIALIZABLE_HPP
#define I_JSON_SERIALIZABLE_HPP
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

class IJsonSerializable {
public:
  virtual Json ToJson() const = 0;
};

#endif // I_JSON_SERIALIZABLE_HPP