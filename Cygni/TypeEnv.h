#ifndef TYPEENV_H
#define TYPEENV_H

#include "Type.h"
#include <map>
#include <string>

using std::map;
using std::wstring;

class TypeEnv
{
public:
    TypeEnv();
    map<wstring, Type> table;
    bool Define(wstring name, Type type);
    virtual Type Find(wstring name) = 0;
    virtual ~TypeEnv();
    virtual bool IsGlobal() = 0;
};

class GlobalTypeEnv : public TypeEnv
{
public:
    GlobalTypeEnv();
    Type Find(wstring name) override;
    bool IsGlobal() override;
};

class FunctionTypeEnv : public TypeEnv
{
public:
    Type type;
    TypeEnv* parent;
    FunctionTypeEnv(Type type, TypeEnv* parent);
    Type Find(wstring name) override;
    bool IsGlobal() override;
};

class FunctionList
{
public:
    FunctionList();
	map<wstring, int> table;
    vector<Type> types;
    bool Define(wstring name, Type type);
	int Find(wstring name);
    Type ResolveType(wstring name);
};
#endif // TYPEENV_H
