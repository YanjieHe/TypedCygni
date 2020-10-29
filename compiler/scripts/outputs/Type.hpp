#ifndef TYPE_HPP
#define TYPE_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "Enum.hpp"

class Type;
class Value;
using std::vector;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using TypePtr = shared_ptr<Type>;
using ValuePtr = shared_ptr<Value>;

class Type {
public:
  virtual TypeCode GetTypeCode() const;
};

class Value {
public:
  virtual TypeCode GetTypeCode() const;
};

class BasicType : Type {
public:
  TypeCode typeCode;

  BasicType(TypeCode typeCode) : typeCode{typeCode} {}
  TypeCode GetTypeCode() const override { return typeCode; }
};

class UnknownType : Type {
public:
  string identifier;

  UnknownType(string identifier) : identifier{identifier} {}
  TypeCode GetTypeCode() const override { return TypeCode::UNKNOWN; }
};

class ArrayType : Type {
public:
  TypePtr elementType;

  ArrayType(TypePtr elementType) : elementType{elementType} {}
  TypeCode GetTypeCode() const override { return TypeCode::ARRAY; }
};

class BasicValue : Value {
public:
  TypeCode typeCode;

  BasicValue(TypeCode typeCode) : typeCode{typeCode} {}
  TypeCode GetTypeCode() const override { return typeCode; }
};

class ArrayValue : Value {
public:
  ValuePtr elementType;

  ArrayValue(ValuePtr elementType) : elementType{elementType} {}
  TypeCode GetTypeCode() const override { return TypeCode::ARRAY; }
};

class FunctionValue : Value {
public:
  vector<ValuePtr> parameters;
  ValuePtr returnType;

  FunctionValue(vector<ValuePtr> parameters, ValuePtr returnType)
      : parameters{parameters}, returnType{returnType} {}
  TypeCode GetTypeCode() const override { return TypeCode::FUNCTION; }
};

class RecordValue : Value {
public:
  string name;
  unordered_map<string, ValuePtr> fields;

  RecordValue(string name, unordered_map<string, ValuePtr> fields)
      : name{name}, fields{fields} {}
  TypeCode GetTypeCode() const override { return TypeCode::RECORD; }
};

#endif // TYPE_HPP
