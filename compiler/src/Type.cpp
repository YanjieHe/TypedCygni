#include "Type.hpp"
#include <linq/Linq.hpp>

using std::make_shared;
using std::static_pointer_cast;

Type::Ptr Type::Void() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::VOID);
  return type;
}

Type::Ptr Type::Boolean() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::BOOLEAN);
  return type;
}

Type::Ptr Type::Int() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::INT);
  return type;
}

Type::Ptr Type::Long() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::LONG);
  return type;
}

Type::Ptr Type::Float() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::FLOAT);
  return type;
}

Type::Ptr Type::Double() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::DOUBLE);
  return type;
}

Type::Ptr Type::Char() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::CHAR);
  return type;
}

Type::Ptr Type::String() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::STRING);
  return type;
}

Type::Ptr Type::Any() {
  static BasicType::Ptr type = make_shared<BasicType>(TypeCode::ANY);
  return type;
}

bool BasicType::Equals(Type::Ptr other) const {
  return typeCode == other->GetTypeCode();
}

Json BasicType::ToJson() const {
  return unordered_map<string, Json>(
      {{"Type Code", Enum<TypeCode>::ToString(GetTypeCode())}});
}

bool ArrayType::Equals(Type::Ptr other) const {
  if (other->GetTypeCode() == TypeCode::ARRAY) {
    return element->Equals(static_pointer_cast<ArrayType>(other)->element);
  } else {
    return false;
  }
}

Json ArrayType::ToJson() const {
  return unordered_map<string, Json>(
      {{"Type Code", Enum<TypeCode>::ToString(GetTypeCode())},
       {"Element", element->ToJson()}});
}

bool FunctionType::Equals(Type::Ptr other) const {
  if (other->GetTypeCode() == TypeCode::FUNCTION) {
    auto ft = static_pointer_cast<FunctionType>(other);
    int nX = static_cast<int>(this->args.size());
    int nY = static_cast<int>(ft->args.size());
    if (nX == nY) {
      for (int i = 0; i < nX; i++) {
        if (!args[i]->Equals(ft->args[i])) {
          return false;
        }
      }
      return ret->Equals(ft->ret);
    } else {
      return false;
    }
  } else {
    return false;
  }
}

Json FunctionType::ToJson() const {
  return unordered_map<string, Json>(
      {{"Type Code", Enum<TypeCode>::ToString(GetTypeCode())},
       {"Arguments",
        linq::from(args)
            .select([](Type::Ptr arg) -> Json { return arg->ToJson(); })
            .to_vector()},
       {"Return Type", ret->ToJson()}});
}

bool ObjectType::Equals(Type::Ptr other) const {
  if (other->GetTypeCode() == TypeCode::OBJECT) {
    return name == static_pointer_cast<ObjectType>(other)->name;
  } else {
    return false;
  }
}

Json ObjectType::ToJson() const {
  unordered_map<string, Json> membersJson;
  for (auto[memberName, memberType] : members) {
    membersJson.insert({memberName, memberType->ToJson()});
  }
  return unordered_map<string, Json>(
      {{"Type Code", Enum<TypeCode>::ToString(GetTypeCode())},
       {"Name", name},
       {"Members", membersJson}});
}

bool MethodType::Equals(Type::Ptr other) const {
  if (other->GetTypeCode() == TypeCode::METHOD) {
    auto otherType = static_pointer_cast<MethodType>(other);
    return obj->Equals(otherType->obj) && func->Equals(otherType->func);
  } else {
    return false;
  }
}

Json MethodType::ToJson() const {
  return unordered_map<string, Json>(
      {{"Type Code", Enum<TypeCode>::ToString(GetTypeCode())},
       {"Object", obj->ToJson()},
       {"Function Type", func->ToJson()}});
}
