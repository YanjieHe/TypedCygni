#ifndef SCOPE_H
#define SCOPE_H

#include "Expression.h"
#include "Location.h"
#include <map>
#include <memory>
#include <string>

using std::map;
using std::shared_ptr;
using std::wstring;

class Scope;
class GlobalScope;
class FunctionScope;

using ScopePtr = shared_ptr<Scope>;
using GlobalScopePtr = shared_ptr<GlobalScope>;
using FunctionScopePtr = shared_ptr<FunctionScope>;

class Scope
{
public:
	map<wstring, int> table;
    Scope();
    void Define(wstring name);
	virtual Location Find(wstring name) = 0;
	int Size();
    virtual ~Scope();

protected:
	bool HasKey(wstring name);
};

class GlobalScope : public Scope
{
public:
    GlobalScope();
    Location Find(wstring name) override;
};

class FunctionScope : public Scope
{
public:
    ScopePtr parent;
    FunctionScope(ScopePtr parent);
    Location Find(wstring name) override;
};

#endif // SCOPE_H
