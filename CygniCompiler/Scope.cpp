#include "Scope.hpp"
#include "Exception.hpp"

Scope::Scope(Ptr<Scope> parent)
        : parent{std::move(parent)}
{

}

Optional<Any> Scope::Lookup(const String &name, const String &key)
{
    auto tryLookupParent = [this](const String &name, const String &key)
    {
        if (parent)
        {
            return parent->Lookup(name, key);
        }
        else
        {
            return Optional<Any>();
        }
    };
    if (values.find(name) != values.end()) // found name
    {
        if (values[name].find(key) != values[name].end()) // found key
        {
            return Optional<Any>(values[name][key]);
        }
        else
        {
            return tryLookupParent(name, key);
        }
    }
    else
    {
        return tryLookupParent(name, key);
    }
}

void Scope::Put(const String &name, const String &key, const Any &value)
{
    if (values.find(name) != values.end()) // found name
    {
        if (values[name].find(key) != values[name].end()) // found key
        {
            throw ArgumentException("ArgumentException: An item with the same key has already been added.");
        }
        else
        {
            values[name].insert({key, value}); // insert value if (name, key) does not exists
        }
    }
    else
    {
        values.insert({name, HashMap<String, Any>()}); // create key first
        values[name].insert({key, value});
    }
}

const Ptr<Scope> &Scope::Parent()
{
    return parent;
}
