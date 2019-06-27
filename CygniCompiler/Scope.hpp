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
    HashMap<String, HashMap<String, Ptr<void>>> values;

public:

    Scope() = default;

    explicit Scope(Ptr<Scope> parent);

    Optional<Ptr<void>> Lookup(const String &name, const String &key);

    void Put(const String &name, const String &key, const Ptr<void> &value);

    const Ptr<Scope> &Parent();
};

#endif //SCOPE_HPP
