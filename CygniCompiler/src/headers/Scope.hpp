#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "Exception.hpp"
#include "Location.hpp"
#include "Optional.hpp"
#include "Predef.hpp"
#include "String.hpp"
#include "Table.hpp"

template <typename TValue>
class Scope {
 private:
  Ptr<Scope> parent;
  Table<String, TValue> table;

 public:
  Scope() : parent(), table() {}

  explicit Scope(Ptr<Scope> parent) : parent{parent}, table() {}

  Optional<TValue> LookupInLocal(const String& key) {
    if (table.HasKey(key)) {
      return table.GetByKey(key);
    } else {
      return Optional<TValue>();
    }
  }

  Optional<TValue> Lookup(const String& key) {
    if (table.HasKey(key)) {
      return Optional<TValue>(table.GetByKey(key));
    } else if (parent) {
      return parent->Lookup(key);
    } else {
      return Optional<TValue>();
    }
  }

  void Put(String key, TValue value) {
    if (table.HasKey(key)) {
      throw ArgumentException(
          "ArgumentException: An item with the same key has already been "
          "added.");
    } else {
      table.Add(key, value);
    }
  }

  inline const Ptr<Scope>& Parent() { return parent; }
};

// class Scope {
//  private:
//   Ptr<Scope> parent;
//   HashMap<String, HashMap<String, Ptr<void>>> values;

//  public:
//   Scope() = default;

//   explicit Scope(Ptr<Scope> parent);

//   Optional<Ptr<void>> Lookup(const String& name, const String& key);

//   void Put(const String& name, const String& key, const Ptr<void>& value);

//   const Ptr<Scope>& Parent();
// };

// template <typename AstType>
// class LocationScope {
//  private:
//   Ptr<LocationScope> parent;
//   Ptr<AstType> node;
//   Table<String, Ptr<AstType>> table;

//  public:
//   LocationScope() : parent(), node(), table() {}
//   LocationScope(Ptr<LocationScope> parent) : parent{parent}, node(), table()
//   {} Optional<Ptr<AstType>> LookUpInLocal(String key) {
//     if (table.HasKey(key)) {
//       return table.GetByKey(key);
//     } else {
//       return Optional<Ptr<AstType>>();
//     }
//   }

//   Optional<Ptr<AstType>> LookUp(String key) {
//     if (table.HasKey(key)) {
//       return table.GetByKey(key);
//     } else if (parent) {
//       return parent->LookUp(key);
//     } else {
//       return Optional<Ptr<AstType>>();
//     }
//   }

//   void Put(String key, Ptr<AstType> value) {
//     if (table.HasKey(key)) {
//       throw ArgumentException(
//           "ArgumentException: An item with the same key has already been "
//           "added.");
//     } else {
//       table.Add(key, value);
//     }
//   }

//   void SetNode(Ptr<AstType> node) { this->node = node; }
//   inline const Ptr<LocationScope>& Parent() { return parent; }
// };
#endif  // SCOPE_HPP
