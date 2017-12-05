#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>
#include "Expression.h"

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
	Location(const Location& location);
	Location();
	wstring ToString();
};

class LocalScope;

class Scope
{
public:
	map<wstring, int> table;
    Scope();
	virtual bool Define(wstring name) = 0;
	virtual Location Find(wstring name) = 0;
	virtual LocalScope* CreateLocal() = 0;
	virtual Assign() = 0;

protected:
	bool HasKey(wstring name);
};

class GlobalScope: public Scope
{
public:
	int number;
	GlobalScope();
	bool Define(wstring name) override;
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
	void Assign() override;
};

class FunctionScope: public Scope
{
public:
	Scope* parent;
	int number;
	FunctionScope(Scope* parent);
	bool Define(wstring name) override;
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
	void Assign() override;
};

class LocalScope: public Scope
{
public:
	Scope* parent;
	LocationKind kind;
	LocalScope(Scope* parent, LocationKind kind);
	bool Define(wstring name) override;
	Location Find(wstring name) override;
	LocalScope* CreateLocal() override;
	void Assign() override;
};

class LocationRecord
{
public:
	LocationRecord();
	void Record(Expression* expression, Location location);
	Location Find(Expression* expression);
private:
	map<int, Location> record;
};
#endif // SCOPE_H 
