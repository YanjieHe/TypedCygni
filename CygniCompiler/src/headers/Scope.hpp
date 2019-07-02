#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "Location.hpp"
#include "Optional.hpp"
#include "Predef.hpp"
#include "String.hpp"
#include "Table.hpp"

class Scope {
private:
  Ptr<Scope> parent;
  HashMap<String, HashMap<String, Ptr<void>>> values;

public:
  Scope() = default;

  explicit Scope(Ptr<Scope> parent);

  Optional<Ptr<void>> Lookup(const String &name, const String &key);

  void Put(const String &name, const String &key, const Ptr<void> &value);

  const Ptr<Scope> &Parent();
};

template <typename AstType> class LocationScope {
public:
  Ptr<AstType> node;
  Table<String, Ptr<AstType>> table;
};
#endif // SCOPE_HPP
