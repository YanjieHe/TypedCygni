#include "TypeEnv.h"

TypeEnv::TypeEnv()
{
}

bool TypeEnv::Define(wstring name, Type type)
{
	if (table.find(name) != table.end())
	{
		return false;
	}
	else
	{
        table.insert(map<wstring, Type>::value_type(name, type));
		return true;
    }
}

TypeEnv::~TypeEnv()
{
}

FunctionList::FunctionList()
{
}

bool FunctionList::Define(wstring name, Type type)
{
	if (table.find(name) != table.end())
	{
		return false;
	}
	else
	{
        int n = static_cast<int>(table.size());
		table.insert(map<wstring, int>::value_type(name, n));
		types.push_back(type);
		return true;
	}
}

int FunctionList::Find(wstring name)
{
	if (table.find(name) != table.end())
	{
		return table[name];
	}
	else
	{
		return -1;
	}
}

Type FunctionList::ResolveType(wstring name)
{
	if (table.find(name) != table.end())
	{
        return types[static_cast<unsigned int>(table[name])];
	}
	else
	{
        return Type::Unknown();
	}
}

GlobalTypeEnv::GlobalTypeEnv() : TypeEnv()
{
}

Type GlobalTypeEnv::Find(std::wstring name)
{
    if (table.find(name) != table.end())
    {
        return table[name];
    }
    else
    {
        return Type::Unknown();
    }
}

bool GlobalTypeEnv::IsGlobal()
{
    return true;
}

FunctionTypeEnv::FunctionTypeEnv(Type type, TypeEnvPtr parent)
    : TypeEnv(), type{type}, parent{parent}
{
}

Type FunctionTypeEnv::Find(std::wstring name)
{
    if (table.find(name) != table.end())
    {
        return table[name];
    }
    else
    {
        return parent->Find(name);
    }
}

bool FunctionTypeEnv::IsGlobal()
{
    return false;
}
