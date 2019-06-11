#ifndef TYPE_HPP
#define TYPE_HPP

#include "String.hpp"
#include "Predef.hpp"

/*
 * Type Leaf: Int, Float, Bool, Car, Rectangle, ...
 * Type List: Function[Int, Int, Int], List[Double]
 */

enum class TypeCode
{
    INT,
    FLOAT,
    LONG,
    DOUBLE,
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

    explicit ArrayType(const Ptr<Type> &element) : element{element}
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

    FunctionType(const Vector<Ptr<Type>> &parameters, const Ptr<Type> &returnType) : parameters{parameters},
                                                                                     returnType{returnType}
    {

    }
};

class ObjectType : public Type
{
    
};

class Value
{
public:
    virtual bool IsLeaf() = 0;

    virtual bool Equals(const Ptr<Value> &other) = 0;

    virtual String ToString() = 0;

    static Ptr<Value> IntValue;
    static Ptr<Value> FloatValue;
    static Ptr<Value> LongValue;
    static Ptr<Value> DoubleValue;
    static Ptr<Value> BoolValue;
    static Ptr<Value> CharValue;
    static Ptr<Value> StringValue;
    static Ptr<Value> UnitValue;

};

class ValueLeaf : public Value
{
public:
    String name;

    explicit ValueLeaf(String name) noexcept
    {

    }

    bool IsLeaf() override
    {
        return true;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        if (other->IsLeaf())
        {
            return name == Cast<ValueLeaf>(other)->name;
        }
        else
        {
            return false;
        }
    }

    String ToString() override
    {
        return name;
    }
};

class ValueList : public Value
{
public:
    Ptr<Value> typeConstructor;
    Vector<Ptr<Value>> values;

    ValueList(Ptr<Value> typeConstructor, Vector<Ptr<Value>> values)
            : typeConstructor{typeConstructor}, values{std::move(values)}
    {

    }

    bool IsLeaf() override
    {
        return false;
    }

    bool Equals(const Ptr<Value> &other) override
    {
        if (other->IsLeaf())
        {
            return false;
        }
        else
        {
            auto list = Cast<ValueList>(other);
            if (typeConstructor->Equals(list->typeConstructor))
            {
                auto comparator = [](const Ptr<Value> &x, const Ptr<Value> &y) -> bool
                {
                    return x->Equals(y);
                };
                return std::equal(values.begin(), values.end(), list->values.begin(),
                                  list->values.end(), comparator);
            }
            else
            {
                return false;
            }
        }
    }

    String ToString() override
    {
        Vector<String> items;
        items.reserve(values.size());
        for (const auto &value: values)
        {
            items.push_back(value->ToString());
        }
        return typeConstructor->ToString() + "[" + String::Join(", ", items.begin(), items.end()) + "]";
    }

};

class ClassType
{
public:
    String className;
    HashMap<String, Ptr<Type>> fields;
    HashMap<String, Ptr<Type>> methods;

    explicit ClassType(String className) : className{std::move(className)}
    {

    }

    void AddField(const String &name, const Ptr<Type> &type)
    {
        this->fields.insert({name, type});
    }

    void AddMethod(const String &name, const Ptr<Type> &type)
    {
        this->methods.insert({name, type});
    }
};

class ClassValue
{
public:
    String className;
    HashMap<String, Ptr<Value>> fields;
    HashMap<String, Ptr<Value>> methods;

    explicit ClassValue(String className) : className{std::move(className)}
    {

    }

    void AddField(const String &name, const Ptr<Value> &value)
    {
        this->fields.insert({name, value});
    }

    void AddMethod(const String &name, const Ptr<Value> &value)
    {
        this->methods.insert({name, value});
    }
};

#endif // TYPE_HPP