#ifndef TYPE_HPP
#define TYPE_HPP

#include "String.hpp"
#include "Predef.hpp"

enum class TypeCode
{
    INT,
    FLOAT,
    LONG,
    DOUBLE,
    CHAR,
    STRING,
    BOOL,
    ARRAY,
    FUNCTION,
    OBJECT
};

class Type
{
public:
    virtual TypeCode GetTypeCode() = 0;

    virtual bool Equals(const Ptr<Type> &other) = 0;

    virtual String ToString() = 0;
};

class IntType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::INT;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::INT;
    }

    String ToString() override
    {
        return "Int";
    }
};

class FloatType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::FLOAT;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::FLOAT;
    }

    String ToString() override
    {
        return "FLOAT";
    }
};

class LongType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::LONG;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::LONG;
    }

    String ToString() override
    {
        return "Long";
    }
};

class DoubleType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::DOUBLE;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::DOUBLE;
    }

    String ToString() override
    {
        return "Double";
    }
};

class CharType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::CHAR;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::CHAR;
    }

    String ToString() override
    {
        return "Char";
    }
};

class StringType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::STRING;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::STRING;
    }

    String ToString() override
    {
        return "String";
    }
};

class BoolType : public Type
{
public:
    TypeCode GetTypeCode() override
    {
        return TypeCode::BOOL;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        return other->GetTypeCode() == TypeCode::BOOL;
    }

    String ToString() override
    {
        return "Bool";
    }
};

class ArrayType : public Type
{
public:
    Ptr<Type> element;

    explicit ArrayType(Ptr<Type> element)
            : element{std::move(element)}
    {

    }

    TypeCode GetTypeCode() override
    {
        return TypeCode::ARRAY;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        if (other->GetTypeCode() == TypeCode::ARRAY)
        {
            auto array = Cast<ArrayType>(other);
            return element->Equals(array->element);
        }
        else
        {
            return false;
        }
    }

    String ToString() override
    {
        return "Array[" + element->ToString() + "]";
    }
};

class FunctionType : public Type
{
public:
    Vector<Ptr<Type>> parameters;
    Ptr<Type> returnType;

    FunctionType(Vector<Ptr<Type>> parameters, Ptr<Type> returnType)
            : parameters{std::move(parameters)},
              returnType{std::move(returnType)}
    {

    }

    TypeCode GetTypeCode() override
    {
        return TypeCode::FUNCTION;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        if (other->GetTypeCode() == TypeCode::FUNCTION)
        {
            auto function = Cast<FunctionType>(other);
            auto compare = [](const Ptr<Type> &x, const Ptr<Type> &y)
            {
                return x->Equals(y);
            };
            if (parameters.size() == function->parameters.size())
            {
                return std::equal(parameters.begin(), parameters.end(), function->parameters.begin(),
                                  function->parameters.end(), compare)
                       && returnType->Equals(function->returnType);
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

    String ToString() override
    {
        auto items = Enumerate::Map(parameters, [](const Ptr<Type> &t)
        {
            return t->ToString();
        });
        items.push_back(returnType->ToString());
        return "Function[" + String::Join(", ", items.begin(), items.end()) + "]";
    }
};

enum class ValueCode
{
    INT,
    FLOAT,
    LONG,
    DOUBLE,
    CHAR,
    STRING,
    BOOL,
    ARRAY,
    FUNCTION,
    OBJECT
};

class Value
{
public:

    virtual ValueCode GetValueCode() = 0;

    virtual bool Equals(const Ptr<Value> &other) = 0;

    virtual String ToString() = 0;
};

class IntValue : public Value
{
public:

    ValueCode GetValueCode() override
    {
        return ValueCode::INT;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::INT;
    }

    String ToString() override
    {
        return "Int";
    }
};

class FloatValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::FLOAT;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::FLOAT;
    }

    String ToString() override
    {
        return "Float";
    }
};

class LongValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::LONG;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::LONG;
    }

    String ToString() override
    {
        return "Long";
    }
};

class DoubleValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::DOUBLE;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::DOUBLE;
    }

    String ToString() override
    {
        return "Double";
    }
};

class BoolValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::BOOL;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::BOOL;
    }

    String ToString() override
    {
        return "Bool";
    }
};

class CharValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::CHAR;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::CHAR;
    }

    String ToString() override
    {
        return "Char";
    }
};

class StringValue : public Value
{
public:
    ValueCode GetValueCode() override
    {
        return ValueCode::STRING;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        return other->GetValueCode() == ValueCode::STRING;
    }

    String ToString() override
    {
        return "String";
    }
};

class ArrayValue : public Value
{
public:
    Ptr<Value> element;

    explicit ArrayValue(Ptr<Value> element)
            : element{std::move(element)}
    {

    }

    ValueCode GetValueCode() override
    {
        return ValueCode::ARRAY;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        if (other->GetValueCode() == ValueCode::ARRAY)
        {
            auto array = Cast<ArrayValue>(other);
            return element->Equals(array->element);
        }
        else
        {
            return false;
        }
    }

    String ToString() override
    {
        return "Array[" + element->ToString() + "]";
    }
};

class FunctionValue : public Value
{
public:
    Vector<Ptr<Value>> parameters;
    Ptr<Value> returnValue;

    FunctionValue(Vector<Ptr<Value>> parameters, Ptr<Value> returnValue)
            : parameters{std::move(parameters)},
              returnValue{std::move(returnValue)}
    {

    }

    ValueCode GetValueCode() override
    {
        return ValueCode::FUNCTION;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        if (other->GetValueCode() == ValueCode::FUNCTION)
        {
            auto function = Cast<FunctionValue>(other);
            auto compare = [](const Ptr<Value> &x, const Ptr<Value> &y)
            {
                return x->Equals(y);
            };
            if (parameters.size() == function->parameters.size())
            {
                return std::equal(parameters.begin(), parameters.end(), function->parameters.begin(),
                                  function->parameters.end(), compare)
                       && returnValue->Equals(function->returnValue);
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

    String ToString() override
    {
        auto items = Enumerate::Map(parameters, [](const Ptr<Value> &t)
        {
            return t->ToString();
        });
        items.push_back(returnValue->ToString());
        return "Function[" + String::Join(", ", items.begin(), items.end()) + "]";
    }
};

#endif // TYPE_HPP