#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "String.hpp"
#include "Predef.hpp"
#include "Any.hpp"
#include "Optional.hpp"

class Scope
{
public:
    Ptr<Scope> parent;
    HashMap<String, HashMap<String, Any>> values;

    Scope() = default;

    explicit Scope(Ptr<Scope> parent);

    Optional<Any> Lookup(const String &name, const String &key);

    void Put(const String &name, const String &key, const Any &value);

};

#endif //SCOPE_HPP
