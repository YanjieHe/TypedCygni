#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>
#include "Expression.h"
#include "Location.h"

using std::map;
using std::wstring;

class Scope
{
public:
	map<wstring, int> table;
    Scope();
	bool Define(wstring name);
	virtual Location Find(wstring name) = 0;
	int Size();

protected:
	bool HasKey(wstring name);
};

class GlobalScope: public Scope
{
public:
	GlobalScope();
	Location Find(wstring name) override;
};

class FunctionScope: public Scope
{
public:
	Scope* parent;
	FunctionScope(Scope* parent);
	Location Find(wstring name) override;
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
