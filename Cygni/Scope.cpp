#include "Scope.h"
#include <sstream>

using namespace std;

Location::Location(LocationKind kind, int index)
	:kind{kind}, index{index}
{
}

Location::Location(const Location& location)
	:kind{location.kind}, index{location.index}
{
}

Location::Location()
	:kind{LocationKind::Unknown}, index{-1}
{
}

wstring Location::ToString()
{
	wostringstream oss;
	switch (kind)
	{
		case LocationKind::Unknown:
			oss << L"[Unknown, ";
			break;
		case LocationKind::Global:
			oss << L"[Global, ";
			break;
		case LocationKind::Function:
			oss << L"[Function, ";
			break;
		default:
		case LocationKind::Class:
			oss << L"[Class, ";
			break;
	}
	oss << index << L"]";
	return oss.str();
}

Scope::Scope()
{
	this->table = map<wstring, int>();
}

bool Scope::HasKey(wstring name)
{
	map<wstring, int>::iterator iter = table.find(name);
	return iter != table.end();
}

GlobalScope::GlobalScope()
	:Scope()
{
}

bool GlobalScope::Define(wstring name)
{
	if (HasKey(name))
	{
		return false;
	}
	else
	{
		table.insert(map<wstring, int>::value_type(name, number));
		number++;
		return true;
	}
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

LocalScope* GlobalScope::CreateLocal()
{
	return new LocalScope(this, LocationKind::Global);
}

void Global::Assign()
{
	number++;
}

FunctionScope::FunctionScope(Scope* parent)
	:Scope(), parent{parent}
{
}

bool FunctionScope::Define(wstring name)
{
	if (HasKey(name))
	{
		return false;
	}
	else
	{
		table.insert(map<wstring, int>::value_type(name, number));
		number++;
		return true;
	}
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

void FunctionScope::Assign()
{
	number++;
}

LocalScope::LocalScope(Scope* parent, LocationKind kind)
	:Scope(), parent{parent}, kind{kind}
{
}

bool LocalScope::Define(wstring name)
{
	// TO DO
}

Location LocalScope::Find(wstring name)
{
	if (HasKey(name))
	{
		return Location(kind, table[name]);
	}
	else
	{
		return parent->Find(name);
	}
}

LocalScope* LocalScope::CreateLocal()
{
	return new LocalScope(this, kind);
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
