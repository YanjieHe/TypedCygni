#include "Scope.h"

Scope::Scope()
{
	this->table = map<wstring, int>();
}

bool Scope::Define(wstring name)
{
    int n = static_cast<int>(table.size());
	if (HasKey(name))
	{
		return false;
	}
	else
	{
		table.insert(map<wstring, int>::value_type(name, n));
		return true;
	}
}

bool Scope::HasKey(wstring name)
{
	map<wstring, int>::iterator iter = table.find(name);
	return iter != table.end();
}

int Scope::Size()
{
    return static_cast<int>(table.size());
}

Scope::~Scope()
{
}

GlobalScope::GlobalScope() : Scope()
{
}

Location GlobalScope::Find(wstring name)
{
	if (HasKey(name))
	{
		return Location(LocationKind::Global, table[name]);
	}
	else
	{
		return Location();
	}
}

FunctionScope::FunctionScope(ScopePtr parent) : Scope(), parent{parent}
{
}

Location FunctionScope::Find(wstring name)
{
	if (HasKey(name))
	{
        return Location(LocationKind::InFunction, table[name]);
	}
	else
	{
		return parent->Find(name);
	}
}
