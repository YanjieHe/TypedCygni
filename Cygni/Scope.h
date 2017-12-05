#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>

using std::map;
using std::wstring;

enum class LocationKind
{
	Unknown, Global, Function, Class
};

class Location
{
public:
	LocationKind kind;
	int index;
	Location(LocationKind kind, int index);
};

class LocalScope;

class Scope
{
public:
	map<wstring, int> table;
    Scope();
	bool Define(wstring name);
	virtual Location Find(wstring name) = 0;
	virtual LocalScope* CreateLocal() = 0;

protected:
	bool HasKey(wstring name);
};

class GlobalScope: public Scope
{
public:
	int size;
	GlobalScope();
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
};

class FunctionScope: public Scope
{
public:
	Scope* parent;
	int size;
	FunctionScope(Scope* parent);
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
};

class LocalScope: public Scope
{
public:
	Scope* parent;
	LocationKind kind;
	LocalScope(Scope* parent, LocationKind kind);
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
};

#endif // SCOPE_H 
