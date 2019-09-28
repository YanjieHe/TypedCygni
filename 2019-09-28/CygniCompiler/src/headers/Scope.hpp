#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "Exception.hpp"
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

#endif  // SCOPE_HPP
