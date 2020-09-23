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

/* singly linked list */
template <typename T>
class SLinkedList
{
public:
  T value;
  SLinkedList<T> *next;
  SLinkedList(T value, SLinkedList<T> *next) : value{value}, next{next} {}
};

class Vec
{
public:
  template <typename T, typename R>
  static vector<R> Map(const vector<T> &items, const std::function<R(T)> &f)
  {
    vector<R> result;
    result.reserve(items.size());
    for (auto item : items)
    {
      result.push_back(f(item));
    }
    return result;
  }

  template <typename T, typename Func>
  static bool SequenceEqual(const vector<T> &xs, const vector<T> &ys, Func f)
  {
    if (xs.size() == ys.size())
    {
      for (size_t i = 0; i < xs.size(); i++)
      {
        if (!f(xs[i], ys[i]))
        {
          return false;
        }
      }
      return true;
    }
    else
    {
      return false;
    }
  }

  template <typename T>
  static vector<T> Filter(const vector<T> &items,
                          const std::function<bool(T)> &f)
  {
    vector<T> result;
    for (auto item : items)
    {
      if (f(item))
      {
        result.push_back(item);
      }
    }
    return result;
  }

  template <typename T>
  static vector<T> Skip(const vector<T> &items, size_t n)
  {
    vector<T> result;
    for (size_t i = n; i < items.size(); i++)
    {
      result.push_back(items[i]);
    }
    return result;
  }

  template <typename T>
  static vector<T> SLinkedListToVec(SLinkedList<T> *head)
  {
    auto cur = head;
    vector<T> result;
    while (cur != nullptr)
    {
      result.push_back(cur->value);
      cur = cur->next;
    }
    std::reverse(result.begin(), result.end());
    return result;
  }
};

class Type;
class ClassInfo;

class Type
{
public:
  virtual TypeCode GetTypeCode() const = 0;
  virtual bool Equals(Type *other) const = 0;

  static Type *Void();
  static Type *Boolean();
  static Type *Int();
  static Type *Long();
  static Type *Float();
  static Type *Double();
  static Type *Char();
  static Type *String();
};

class BasicType : public Type
{
public:
  TypeCode typeCode;
  explicit BasicType(TypeCode typeCode) : typeCode{typeCode} {}

  TypeCode GetTypeCode() const override { return typeCode; }
  bool Equals(Type *other) const override
  {
    return typeCode == other->GetTypeCode();
  }
};

inline Type *Type::Void()
{
  static BasicType type(TypeCode::VOID);
  return &type;
}
inline Type *Type::Boolean()
{
  static BasicType type(TypeCode::BOOLEAN);
  return &type;
}
inline Type *Type::Int()
{
  static BasicType type(TypeCode::INT);
  return &type;
}
inline Type *Type::Long()
{
  static BasicType type(TypeCode::LONG);
  return &type;
}
inline Type *Type::Float()
{
  static BasicType type(TypeCode::FLOAT);
  return &type;
}
inline Type *Type::Double()
{
  static BasicType type(TypeCode::DOUBLE);
  return &type;
}
inline Type *Type::Char()
{
  static BasicType type(TypeCode::CHAR);
  return &type;
}
inline Type *Type::String()
{
  static BasicType type(TypeCode::STRING);
  return &type;
}

class ArrayType : public Type
{
public:
  Type *element;
  explicit ArrayType(Type *element) : element{element} {}
  TypeCode GetTypeCode() const override { return TypeCode::ARRAY; }
  bool Equals(Type *other) const override
  {
    if (other->GetTypeCode() == TypeCode::ARRAY)
    {
      return element->Equals(dynamic_cast<ArrayType *>(other)->element);
    }
    else
    {
      return false;
    }
  }
};

class FunctionType : public Type
{
public:
  vector<Type *> args;
  Type *ret;
  FunctionType(vector<Type *> args, Type *ret) : args{args}, ret{ret} {}

  TypeCode GetTypeCode() const override { return TypeCode::FUNCTION; }
  bool Equals(Type *other) const override
  {
    if (other->GetTypeCode() == TypeCode::FUNCTION)
    {
      auto ft = dynamic_cast<FunctionType *>(other);
      if (Vec::SequenceEqual(args, ft->args, [](Type *x, Type *y) -> bool {
            return x->Equals(y);
          }))
      {
        return ret->Equals(ft->ret);
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
};

class ObjectType : public Type
{
public:
  string name;
  unordered_map<string, Type *> fields;
  ObjectType(string name, unordered_map<string, Type *> fields)
      : name{name}, fields{fields} {}
  ObjectType(string name) : name{name} {}
  TypeCode GetTypeCode() const override { return TypeCode::OBJECT; }
  bool Equals(Type *other) const override
  {
    if (other->GetTypeCode() == TypeCode::OBJECT)
    {
      return name == dynamic_cast<ObjectType *>(other)->name;
    }
    else
    {
      return false;
    }
  }
};

class MethodType : public Type
{
public:
  ObjectType *obj;
  FunctionType *func;

  MethodType(ObjectType *obj, FunctionType *func)
      : obj{obj}, func{func} {}
  TypeCode GetTypeCode() const override { return TypeCode::METHOD; }
  bool Equals(Type *other) const override
  {
    if (other->GetTypeCode() == TypeCode::METHOD)
    {
      auto otherType = dynamic_cast<MethodType *>(other);
      return obj->Equals(otherType->obj) && func->Equals(otherType->func);
    }
    else
    {
      return false;
    }
  }
};
#endif // TYPE_HPP