#ifndef TYPE_H
#define TYPE_H

#include "TypeTag.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
using std::map;
using std::shared_ptr;
using std::vector;
using std::wstring;
class Type;
using TypePtr = shared_ptr<Type>;

class ValueType;
class ArrayType;
class FunctionType;

class Type
{
public:
    TypeTag tag;
    Type();

    bool Mathces(Type other);
    bool IsSubtypeOf(Type other);

    bool IsUnknown();
    bool IsInt();
    bool IsLong();
    bool IsFloat();
    bool IsDouble();
    bool IsBoolean();
    bool IsChar();
    bool IsString();
    bool IsArray();
    bool IsFunction();

    bool ParametersMatch(vector<Type> parameters);
    Type ReturnType();

    wstring ToString();

    static Type Unknown();
    static Type Int();
    static Type Long();
    static Type Float();
    static Type Double();
    static Type Boolean();
    static Type Char();
    static Type String();
    static Type Array(Type element);
    static Type Function(vector<Type> parameters, Type returnType);

    static Type FromString(wstring text);
    static bool IsBasicType(wstring text);

private:
    Type(TypeTag tag);
    Type(TypeTag tag, vector<Type> items);
    vector<Type> items;
    wstring name;
    static map<wstring, Type> table;
};

#endif // TYPE_H
