#ifndef TYPEENV_H
#define TYPEENV_H

#include "Type.h"
#include <string>
#include <map>

using std::wstring;
using std::map;

class FunctionEnv;

class TypeEnv
{
public:
    TypeEnv(FunctionEnv* functionEnv, TypeEnv* parent = nullptr);
	TypeEnv* parent;
	FunctionEnv* functionEnv;
	map<wstring, Type*> table;
	bool Define(wstring name, Type* type);
	Type* Find(wstring name);
};


class FunctionEnv
{
public:
	FunctionEnv();
	map<wstring, int> table;
	vector<Type*> types;
	bool Define(wstring name, Type* type);
	int Find(wstring name);
	Type* ResolveType(wstring name);
};
#endif // TYPEENV_H 
