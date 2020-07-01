#ifndef CYGNI_TYPE_HPP
#define CYGNI_TYPE_HPP
#include "Enum.hpp"
#include "SourcePosition.hpp"
#include "Utility.hpp"
#include <list>
#include <optional>
#include <unordered_map>
#include <unordered_set>


namespace cygni {
class Type;
using TypePtr = std::shared_ptr<Type>;

class UnknownType;
class Int32Type;
class Int64Type;
class Float32Type;
class Float64Type;
class CharType;
class BooleanType;
class StringType;
class VoidType;
class ObjectType;
class UnionType;
class AnyType;
class InterfaceType;

class Type {
public:
  TypeCode typeCode;

  explicit Type(TypeCode typeCode);

  virtual std::u32string ToString() const;
  virtual bool Equals(TypePtr other) const;

  static std::shared_ptr<Int32Type> Int32();
  static std::shared_ptr<Int64Type> Int64();
  static std::shared_ptr<Float32Type> Float32();
  static std::shared_ptr<Float64Type> Float64();
  static std::shared_ptr<VoidType> Void();
  static std::shared_ptr<BooleanType> Boolean();
  static std::shared_ptr<CharType> Char();
  static std::shared_ptr<StringType> String();
  static std::shared_ptr<UnknownType> Unknown();
  static std::shared_ptr<AnyType> Any();

  static std::shared_ptr<Type> Unify(const std::vector<TypePtr> types);

  static bool IsPrimitive(TypePtr type);

  virtual ~Type() {}
  // virtual ~Type() = 0;
};

class UnknownType : public Type {
public:
  UnknownType();
  // ~UnknownType() = default;
};

class Int32Type : public Type {
public:
  Int32Type();
  // ~Int32Type() = default;
};

class Int64Type : public Type {
public:
  Int64Type();
  // ~Int64Type() = default;
};

class Float32Type : public Type {
public:
  Float32Type();
  // ~Float32Type() = default;
};

class Float64Type : public Type {
public:
  Float64Type();
  // ~Float64Type() = default;
};

class CharType : public Type {
public:
  CharType();
  // ~CharType()  =default;
};

class BooleanType : public Type {
public:
  BooleanType();
  // ~BooleanType() = default;
};

class StringType : public Type {
public:
  StringType();

  // ~StringType() = default;

  std::u32string ToString() const override;
};

class VoidType : public Type {
public:
  VoidType();

  // ~VoidType() = default;
};

class AnyType : public Type {
public:
  AnyType();

  // ~AnyType() = default;
};

class TypeParameter {
public:
  std::u32string name;
  std::vector<std::shared_ptr<InterfaceType>> interfaces;
};

class GenericType: public Type {
public:
  TypePtr type;
  std::vector<TypePtr> parameters;
  GenericType(TypePtr type, std::vector<TypePtr> parameters);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
};

class ClassType : public Type {
public:
  PackageRoute route;
  std::u32string name;

  ClassType(PackageRoute route, std::u32string name);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
  TypePtr Replace(const Table<std::u32string, TypeParameter> &parameters) const;
  ~ClassType() = default;
};

class ModuleType : public Type {
public:
  PackageRoute route;
  std::u32string name;

  ModuleType(PackageRoute route, std::u32string name);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
  ~ModuleType() = default;
};

class InterfaceType : public Type {
public:
  PackageRoute route;
  std::u32string name;

  InterfaceType(PackageRoute route, std::u32string name);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
  ~InterfaceType() = default;
};

class UnresolvedType : public Type {
public:
  PackageRoute route;
  std::u32string name;

  UnresolvedType(PackageRoute route, std::u32string name);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
  ~UnresolvedType() = default;
};

class ArrayType : public Type {
public:
  TypePtr elementType;

  explicit ArrayType(TypePtr elementType);
  std::u32string ToString() const override;
  bool Equals(TypePtr other) const override;
  // ~ArrayType() = default;
};

class FunctionType : public Type {
public:
  TypePtr selfType;
  std::u32string name;
  std::vector<TypePtr> parameters;
  TypePtr returnType;

  FunctionType(TypePtr selfType, std::u32string name,
               std::vector<TypePtr> parameters, TypePtr returnType);

  bool Match(const std::vector<TypePtr> &args) const;

  std::u32string ToString() const override;
  ~FunctionType() = default;
};

} // namespace cygni

template <> struct std::hash<cygni::TypePtr> {
public:
  std::hash<cygni::PackageRoute> h1;
  std::hash<std::u32string> h2;

  size_t GetHashCode(const cygni::TypePtr &type) const {
    if (type->typeCode == cygni::TypeCode::Class) {
      auto classType = std::static_pointer_cast<cygni::ClassType>(type);
      return h1(classType->route) ^ h2(classType->name);
    } else if (type->typeCode == cygni::TypeCode::Module) {
      auto moduleType = std::static_pointer_cast<cygni::ModuleType>(type);
      return h1(moduleType->route) ^ h2(moduleType->name);
    } else if (type->typeCode == cygni::TypeCode::Interface) {
      auto interfaceType = std::static_pointer_cast<cygni::InterfaceType>(type);
      return h1(interfaceType->route) ^ h2(interfaceType->name);
    } else if (type->typeCode == cygni::TypeCode::Unresolved) {
      auto unresolvedType =
          std::static_pointer_cast<cygni::UnresolvedType>(type);
      return h1(unresolvedType->route) ^ h2(unresolvedType->name);
    } else if (type->typeCode == cygni::TypeCode::Array) {
      auto arrayType = std::static_pointer_cast<cygni::ArrayType>(type);
      return static_cast<size_t>(type->typeCode) ^
             (GetHashCode(arrayType->elementType));
    } else {
      return static_cast<size_t>(type->typeCode);
    }
  }
  size_t operator()(const cygni::TypePtr &type) const {
    return GetHashCode(type);
  }
};

template <> struct std::equal_to<cygni::TypePtr> {
public:
  bool operator()(const cygni::TypePtr &x, const cygni::TypePtr &y) const {
    return x->Equals(y);
  }
};

namespace cygni {
class TypeGraph {
public:
  class Edge {
  public:
    int src;
    int dest;
    bool isSubtypeOf;
  };
  int V; // number of vertices
  int E; // number of edges

  std::vector<std::vector<Edge>> adj;
  std::vector<TypePtr> types;
  std::unordered_map<TypePtr, int> table;

  TypeGraph();

  void AddEdge(TypePtr type, TypePtr superType);

  bool IsSubTypeof(TypePtr type, TypePtr superType);
  bool IsSuperTypeof(TypePtr superType, TypePtr type);

  std::vector<std::shared_ptr<ClassType>>
  InheritanceChain(std::shared_ptr<ClassType> originalType);
  std::vector<std::shared_ptr<InterfaceType>>
  GetAllInterfaces(std::shared_ptr<ClassType> originalType);
  std::vector<std::shared_ptr<InterfaceType>>
  GetAllSuperInterfaces(std::shared_ptr<InterfaceType> originalType);
  std::vector<std::shared_ptr<Type>>
  GetAllSuperTypes(std::shared_ptr<Type> originalType);
};

class UnionType : public Type {
public:
  std::unordered_set<TypePtr, std::hash<TypePtr>, std::equal_to<TypePtr>> types;
  UnionType();
  ~UnionType() = default;
};
} // namespace cygni
#endif // CYGNI_TYPE_HPP
