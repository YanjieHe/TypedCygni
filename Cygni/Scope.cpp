#include "Scope.h"

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

int Scope::Size()
{
	return table.size();
}

GlobalScope::GlobalScope()
	:Scope()
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

FunctionScope::FunctionScope(Scope* parent)
	:Scope(), parent{parent}
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
		return parent->Find(name);
	}
}

LocationRecord::LocationRecord()
{
	this->record = map<int, Location>();
}

void LocationRecord::Record(Expression* expression, Location location)
{
	record.insert(map<int, Location>::value_type(expression->ID, location));
}

Location LocationRecord::Find(Expression* expression)
{
	auto iter = record.find(expression->ID);
	if (iter != record.end())
	{
		return record[expression->ID];
	}
	else
	{
		return Location();
	}
}
