#ifndef TYPE_HPP
#define TYPE_HPP
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "Enum.hpp"

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
        if (!f(xs[i], ys[i])) {
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
  static vector<T> Skip(const vector<T> &items, size_t n) {
    vector<T> result;
    for (size_t i = n; i < items.size(); i++) {
      result.push_back(items[i]);
    }
    return result;
  }
};

class Type {
public:
  typedef shared_ptr<Type> Ptr;

  virtual TypeCode GetTypeCode() const = 0;
  virtual bool Equals(Type::Ptr other) const = 0;

  static Type::Ptr Void();
  static Type::Ptr Boolean();
  static Type::Ptr Int();
  static Type::Ptr Long();
  static Type::Ptr Float();
  static Type::Ptr Double();
  static Type::Ptr Char();
  static Type::Ptr String();
  static Type::Ptr Any();
};

class BasicType : public Type {
public:
  typedef shared_ptr<BasicType> Ptr;

  TypeCode typeCode;
  explicit BasicType(TypeCode typeCode) : typeCode{typeCode} {}

  TypeCode GetTypeCode() const override { return typeCode; }
  bool Equals(Type::Ptr other) const override;
};

class ArrayType : public Type {
public:
  typedef shared_ptr<ArrayType> Ptr;

  Type::Ptr element;

  explicit ArrayType(Type *element) : element{element} {}
  TypeCode GetTypeCode() const override { return TypeCode::ARRAY; }
  bool Equals(Type::Ptr other) const override;
};

class FunctionType : public Type {
public:
  typedef shared_ptr<FunctionType> Ptr;

  vector<Type::Ptr> args;
  Type::Ptr ret;
  FunctionType(vector<Type::Ptr> args, Type::Ptr ret) : args{args}, ret{ret} {}

  TypeCode GetTypeCode() const override { return TypeCode::FUNCTION; }
  bool Equals(Type::Ptr other) const override;
};

class ObjectType : public Type {
public:
  typedef shared_ptr<ObjectType> Ptr;

  string name;
  unordered_map<string, Type::Ptr> fields;

  ObjectType(string name, unordered_map<string, Type::Ptr> fields)
      : name{name}, fields{fields} {}
  ObjectType(string name) : name{name} {}
  TypeCode GetTypeCode() const override { return TypeCode::OBJECT; }
  bool Equals(Type::Ptr other) const override;
};

class MethodType : public Type {
public:
  typedef shared_ptr<MethodType> Ptr;

  ObjectType::Ptr obj;
  FunctionType::Ptr func;

  MethodType(ObjectType::Ptr obj, FunctionType::Ptr func)
      : obj{obj}, func{func} {}
  TypeCode GetTypeCode() const override { return TypeCode::METHOD; }
  bool Equals(Type::Ptr other) const override;
};
#endif // TYPE_HPP