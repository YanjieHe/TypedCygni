#include "Scope.hpp"

Scope::Scope(Ptr<Scope> parent) : parent{std::move(parent)}
{

}

Optional<Any> Scope::Lookup(const String &name, const String &key)
{
    if (values.find(name) != values.end() && values[name].find(key) != values[name].end())
    {
        return Optional<Any>(values[name][key]);
    }
    else if (parent)
    {
        return parent->Lookup(name, key);
    }
    else
    {
        return Optional<Any>();
    }
}

void Scope::Put(const String &name, const String &key, const Any &value)
{
    if (values.find(name) != values.end())
    {
        if (values[name].find(key) != values[name].end())
        {
            throw NameAlreadyExists();
        }
        else
        {
            values[name].insert({key, value});
        }
    }
    else
    {
        values.insert({name, HashMap<String, Any>()});
        values[name].insert({key, value});
    }
}
