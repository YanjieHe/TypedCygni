#include "TypeEnv.h"

TypeEnv::TypeEnv(FunctionEnv* functionEnv, TypeEnv* parent)
{
	this->parent = parent;
	this->table = map<wstring, Type*>();
	this->functionEnv = functionEnv;
}

bool TypeEnv::Define(wstring name, Type* type)
{
	if (table.find(name) != table.end())
	{
		return false;
	}
	else
	{
		table.insert(map<wstring, Type*>::value_type(name, type));
		return true;
	}
}


Type* TypeEnv::Find(wstring name)
{
	if (table.find(name) != table.end())
	{
		return table[name];
	}
	else if (parent != nullptr)
	{
		return parent->Find(name);
	}
	else
	{
		return functionEnv->Find(name);
	}
}

FunctionEnv::FunctionEnv()
{
	this->table = map<wstring, Type*>();
}

bool FunctionEnv::Define(wstring name, Type* type)
{
	if (table.find(name) != table.end())
	{
		return false;
	}
	else
	{
		table.insert(map<wstring, Type*>::value_type(name, type));
		return true;
	}
}

Type* FunctionEnv::Find(wstring name)
{
	if (table.find(name) != table.end())
	{
		return table[name];
	}
	else
	{
		return nullptr;
	}
}
