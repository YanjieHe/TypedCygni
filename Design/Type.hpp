#ifndef TYPE_HPP
#define TYPE_HPP
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using std::make_optional;
using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;

enum class TypeCode {
  VOID,
  BOOLEAN,
  INT,
  LONG,
  FLOAT,
  DOUBLE,
  CHAR,
  STRING,
  CLASS,
  FUNCTION,
  ARRAY,
  NAME,
  APPLY
};

class Vec {
public:
  template <typename T, typename R, typename Func>
  static vector<R> Map(const vector<T> &items, Func f) {
    vector<R> result;
    result.reserve(items.size());
    for (auto item : items) {
      result.push_back(f(item));
    }
    return result;
  }

  template <typename T, typename Func>
  static bool SequenceEqual(const vector<T> &xs, const vector<T> &ys, Func f) {
    if (xs.size() == ys.size()) {
      for (size_t i = 0; i < xs.size(); i++) {
        if (!fs(xs[i], ys[i])) {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
  }

  template <typename T, typename Func>
  static bool Filter(const vector<T> &items, Func f) {
    vector<T> result;
    for (auto item : items) {
      if (f(item)) {
        result.push_back(item);
      }
    }
    return result;
  }
};

class Type {
public:
  virtual TypeCode GetTypeCode() const = 0;
  virtual bool Equals(TypePtr other) const = 0;
};

using TypePtr = shared_ptr<Type>;

class BasicType : public Type {
public:
  TypeCode typeCode;
  explicit BasicType(TypeCode typeCode) : typeCode{typeCode} {}

  TypeCode GetTypeCode() const override { return typeCode; }
  bool Equals(TypePtr other) const override {
    return typeCode == other->GetTypeCode();
  }
};

class ArrayType : public Type {
public:
  TypePtr element;
  explicit ArrayType(TypePtr element) : element{element} {}
  TypeCode GetTypeCode() const override { return TypeCode::ARRAY; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::ARRAY) {
      return element->Equals(static_pointer_cast<ArrayType>(other)->element);
    } else {
      return false;
    }
  }
};

class FunctionType : public Type {
public:
  vector<TypePtr> args;
  TypePtr ret;
  FunctionType(vector<TypePtr> args, TypePtr ret) : args{args}, ret{ret} {}

  TypeCode GetTypeCode() const override { return TypeCode::FUNCTION; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::FUNCTION) {
      auto ft = static_pointer_cast<FunctionType>(other);
      if (Vec::SequenceEqual(args, ft->args, [](TypePtr x, TypePtr y) -> bool {
            return x->Equals(y);
          })) {
        return ret->Equals(ft->ret);
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
};

class ClassInfo;

class ClassType : public Type {
public:
  weak_ptr<ClassInfo> classInfo;
  TypeCode GetTypeCode() const override { return TypeCode::CLASS; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::CLASS) {
      return classInfo.owner_before(static_pointer_cast<ClassType>(other)->classInfo);
    } else {
      return false;
    }
  }
};

class TypeName : public Type {
public:
  int typeID;

  TypeCode GetTypeCode() const override { return TypeCode::NAME; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::NAME) {
      auto otherType = static_pointer_cast<TypeName>(other);
      return typeID == otherType->typeID;
    } else {
      return false;
    }
  }
};

class TypeApply : public Type {
public:
  TypePtr type;
  vector<TypePtr> args;

  TypeCode GetTypeCode() const override { return TypeCode::APPLY; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::APPLY) {
      auto otherType = static_pointer_cast<TypeApply>(other);
      return Vec::SequenceEqual(
          args, otherType->args,
          [](TypePtr x, TypePtr y) -> bool { return x->Equals(y); });
    } else {
      return false;
    }
  }
};

class TypeManager {
public:
  unordered_map<string, int> table;
  vector<string> names;

  int TypeID(string typeName) {
    if (table.count(typeName)) {
      return table[typeName];
    } else {
      int n = names.size();
      names.push_back(typeName);
      table.insert({typeName, n});
      return n;
    }
  }

  optional<const string &> Name(int typeID) {
    if (typeID >= 0 && typeID < names.size()) {
      return names[typeID];
    } else {
      return make_optional<const string &>();
    }
  }
};

#endif // TYPE_HPP