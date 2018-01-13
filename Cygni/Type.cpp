#include "Type.h"

Type::Type() : tag{TypeTag::Unknown}, name{L""}
{
}

bool Type::Mathces(Type other)
{
    if (tag == other.tag && name == other.name)
    {
        if (items.size() == other.items.size())
        {
            for (unsigned int i = 0; i < items.size(); i++)
            {
                if (!items[i].Mathces(other.items[i]))
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Type::IsSubtypeOf(Type other)
{
    if (IsInt() && other.IsLong())
    {
        return true;
    }
    else if (IsInt() && other.IsDouble())
    {
        return true;
    }
    else if (IsFloat() && other.IsDouble())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Type::IsUnknown()
{
    return tag == TypeTag::Unknown;
}

bool Type::IsInt()
{
    return tag == TypeTag::Int;
}

bool Type::IsLong()
{
    return tag == TypeTag::Long;
}

bool Type::IsFloat()
{
    return tag == TypeTag::Float;
}

bool Type::IsDouble()
{
    return tag == TypeTag::Double;
}

bool Type::IsBoolean()
{
    return tag == TypeTag::Boolean;
}

bool Type::IsChar()
{
    return tag == TypeTag::Char;
}

bool Type::IsString()
{
    return tag == TypeTag::String;
}

bool Type::IsArray()
{
    return tag == TypeTag::Array;
}

bool Type::IsFunction()
{
    return tag == TypeTag::Function;
}

bool Type::ParametersMatch(vector<Type> parameters)
{
    if (items.size() == parameters.size() + 1)
    {
        for (unsigned int i = 0; i < parameters.size(); i++)
        {
            if (!items.at(i).Mathces(parameters.at(i)))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

Type Type::ReturnType()
{
    return items.at(items.size() - 1);
}

std::wstring Type::ToString()
{
    switch (tag)
    {
    case TypeTag::Unknown:
        return L"Unknown";
    case TypeTag::Int:
        return L"Int";
    case TypeTag::Long:
        return L"Long";
    case TypeTag::Float:
        return L"Float";
    case TypeTag::Double:
        return L"Double";
    case TypeTag::Boolean:
        return L"Boolean";
    case TypeTag::Char:
        return L"Char";
    case TypeTag::String:
        return L"String";
    case TypeTag::Array:
        return L"Array[" + items.at(0).ToString() + L"]";
    case TypeTag::Function:
    {
        wstring text = L"Function[" + items.at(0).ToString();
        for (unsigned int i = 1; i < items.size(); i++)
        {
            text += items.at(i).ToString();
        }
        text += L"]";
        return text;
    }
    default:
        throw L"not supported type tag";
    }
}

Type Type::Unknown()
{
    return Type(TypeTag::Unknown);
}

Type Type::Int()
{
    return Type(TypeTag::Int);
}

Type Type::Long()
{
    return Type(TypeTag::Long);
}

Type Type::Float()
{
    return Type(TypeTag::Float);
}

Type Type::Double()
{
    return Type(TypeTag::Double);
}

Type Type::Boolean()
{
    return Type(TypeTag::Boolean);
}

Type Type::Char()
{
    return Type(TypeTag::Char);
}

Type Type::String()
{
    return Type(TypeTag::String);
}

Type Type::Array(Type element)
{
    return Type(TypeTag::Array, vector<Type>{element});
}

Type Type::Function(vector<Type> parameters, Type returnType)
{
    parameters.push_back(returnType);
    return Type(TypeTag::Function, parameters);
}

Type Type::FromString(std::wstring text)
{
    if (table.find(text) != table.end())
    {
        return table[text];
    }
    else
    {
        return Type::Unknown();
    }
}

bool Type::IsBasicType(std::wstring text)
{
    return table.find(text) != table.end();
}

Type::Type(TypeTag tag) : tag{tag}, name{L""}
{
}

Type::Type(TypeTag tag, vector<Type> items) : tag{tag}, items{items}, name{L""}
{
}

map<wstring, Type> Type::table = {
    {L"Int", Type::Int()},		   {L"Long", Type::Long()},
    {L"Float", Type::Float()},	 {L"Double", Type::Double()},
    {L"Boolean", Type::Boolean()}, {L"Char", Type::Char()},
    {L"String", Type::String()}};
