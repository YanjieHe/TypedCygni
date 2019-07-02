#ifndef TEST_TYPE_CHECKER_HPP
#define TEST_TYPE_CHECKER_HPP

#include "JSON.hpp"
#include "TypeChecker.hpp"

namespace TestTypeChecker {
void TestAll();

TypeChecker::ScopeCollection InitializeScopeCollection();

Ptr<Scope<Vector<Ptr<FunctionType>>>> InitializeOperatorScope();

Ptr<Scope<Ptr<Type>>> InitializeTypeScope();

void OutputTypeRecord(const std::string& path,
                      const HashMap<int, Ptr<Type>>& typeRecord);

template <typename T, typename ToString>
void AttachToJson(HashMap<int, T>& map, String key, Ptr<JsonObject> json, ToString toString) {
  if (json->jsonType == JsonType::JsonMapType) {
    auto jsonMap = Cast<JsonMap>(json);
    if (jsonMap->ContainsKey("id")) {
      int id = String::ParseInt(
          Cast<JsonValue>(jsonMap->GetValueByKey("id"))->value);
      if (map.find(id) != map.end()) {
        jsonMap->Add(key, New<JsonValue>(toString(map[id])));
      } else {
        // pass
      }
    }
    for (const auto& pair : jsonMap->map) {
      AttachToJson<T, ToString>(map, key, pair.second, toString);
    }
  } else if (json->jsonType == JsonType::JsonArrayType) {
    auto jsonArray = Cast<JsonArray>(json);
    for (const auto& item : jsonArray->items) {
      AttachToJson<T, ToString>(map, key, item, toString);
    }
  } else {
    return;
  }
}
void Test1();
}  // namespace TestTypeChecker
#endif  // TEST_TYPE_CHECKER_HPP
