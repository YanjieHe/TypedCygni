#include "Scope.h"

Location::Location(LocationKind kind, int index)
	:kind{kind}, index{index}
{
}

Scope::Scope()
{
	this->table = map<wstring, int>();
}

bool Scope::Define(wstring name)
{
	int n = table.size();
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

GlobalScope::GlobalScope()
	:Scope(), size{0}
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
		return Location(LocationKind::Unknown, -1);
	}
}

LocalScope* GlobalScope::CreateLocal()
{
	return new LocalScope(this, LocationKind::Global);
}

FunctionScope::FunctionScope(Scope* parent)
	:Scope(), parent{parent}, size{0}
{
}

Location FunctionScope::Find(wstring name)
{
	if (HasKey(name))
	{
		return Location(LocationKind::Function, table[name]);
	}
	else
	{
		return this->parent->Find(name);
	}
}

LocalScope* FunctionScope::CreateLocal()
{
	return new LocalScope(this, LocationKind::Function);
}

LocalScope::LocalScope(Scope* parent, LocationKind kind)
	:Scope(), parent{parent}, kind{kind}
{
}

Location LocalScope::Find(wstring name)
{
	if (HasKey(name))
	{
		return Location(kind, table[name]);
	}
	else
	{
		return this->parent->Find(name);
	}
}

LocalScope* LocalScope::CreateLocal()
{
	return new LocalScope(this, kind);
}
