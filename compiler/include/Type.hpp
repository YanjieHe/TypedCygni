#ifndef TYPE_HPP
#define TYPE_HPP
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using std::make_optional;
using std::make_shared;
using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;



class Vec {
public:
  template <typename T, typename R>
  static vector<R> Map(const vector<T> &items, const std::function<R(T)> &f) {
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

  template <typename T>
  static vector<T> Filter(const vector<T> &items,
                          const std::function<bool(T)> &f) {
    vector<T> result;
    for (auto item : items) {
      if (f(item)) {
        result.push_back(item);
      }
    }
    return result;
  }

  template <typename T>
  static vector<T> Skip(const vector<T>& items, size_t n){
    vector<T> result;
    for (size_t i = n; i < items.size(); i++){
      result.push_back(items[i]);
    }
    return result;
  }
};

class Type;
using TypePtr = shared_ptr<Type>;
class ClassInfo;

class Type {
public:
  virtual TypeCode GetTypeCode() const = 0;
  virtual bool Equals(TypePtr other) const = 0;

  static TypePtr Void();
  static TypePtr Boolean();
  static TypePtr Int();
  static TypePtr Long();
  static TypePtr Float();
  static TypePtr Double();
  static TypePtr Char();
  static TypePtr String();
  static TypePtr Class(weak_ptr<ClassInfo> classInfo);
};

class BasicType : public Type {
public:
  TypeCode typeCode;
  explicit BasicType(TypeCode typeCode) : typeCode{typeCode} {}

  TypeCode GetTypeCode() const override { return typeCode; }
  bool Equals(TypePtr other) const override {
    return typeCode == other->GetTypeCode();
  }
};

TypePtr Type::Void() {
  static auto type = make_shared<BasicType>(TypeCode::VOID);
  return type;
}
TypePtr Type::Boolean() {
  static auto type = make_shared<BasicType>(TypeCode::BOOLEAN);
  return type;
}
TypePtr Type::Int() {
  static auto type = make_shared<BasicType>(TypeCode::INT);
  return type;
}
TypePtr Type::Long() {
  static auto type = make_shared<BasicType>(TypeCode::LONG);
  return type;
}
TypePtr Type::Float() {
  static auto type = make_shared<BasicType>(TypeCode::FLOAT);
  return type;
}
TypePtr Type::Double() {
  static auto type = make_shared<BasicType>(TypeCode::DOUBLE);
  return type;
}
TypePtr Type::Char() {
  static auto type = make_shared<BasicType>(TypeCode::CHAR);
  return type;
}
TypePtr Type::String() {
  static auto type = make_shared<BasicType>(TypeCode::STRING);
  return type;
}

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

class ClassType : public Type {
public:
  weak_ptr<ClassInfo> classInfo;
  explicit ClassType(weak_ptr<ClassInfo> classInfo) : classInfo{classInfo} {}
  TypeCode GetTypeCode() const override { return TypeCode::CLASS; }
  bool Equals(TypePtr other) const override {
    if (other->GetTypeCode() == TypeCode::CLASS) {
      return classInfo.owner_before(
          static_pointer_cast<ClassType>(other)->classInfo);
    } else {
      return false;
    }
  }
};

TypePtr Type::Class(weak_ptr<ClassInfo> classInfo) {
  return make_shared<ClassType>(classInfo);
}

#endif // TYPE_HPP