#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "String.hpp"
#include "Predef.hpp"
#include "Any.hpp"
#include "Optional.hpp"

class Scope
{
private:
    Ptr<Scope> parent;
    HashMap<String, HashMap<String, Any>> values;

public:

    Scope() = default;

    explicit Scope(Ptr<Scope> parent);

    Optional<Any> Lookup(const String &name, const String &key);

    void Put(const String &name, const String &key, const Any &value);

    const Ptr<Scope> &Parent();
};

#endif //SCOPE_HPP
