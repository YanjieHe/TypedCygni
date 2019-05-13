#ifndef TYPE_HPP
#define TYPE_HPP

#include "String.hpp"
#include "Predef.hpp"

/*
 * Type Leaf: Int, Float, Bool, Car, Rectangle, ...
 * Type List: Function[Int, Int, Int], List[Double]
 */

class Type
{
public:
    String name;

    explicit Type(String name) : name{std::move(name)}
    {

    }

    virtual bool IsLeaf() = 0;

    virtual bool Equals(const Ptr<Type> &other) = 0;
};

class TypeLeaf : public Type
{
public:
    explicit TypeLeaf(String name) : Type(std::move(name))
    {

    }

    bool IsLeaf() override
    {
        return true;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        if (other->IsLeaf())
        {
            return name == Cast<TypeLeaf>(other)->name;
        }
        else
        {
            return false;
        }
    }
};

class TypeList : public Type
{
public:
    Vector<Ptr<Type>> parameters;

    TypeList(String name, Vector<Ptr<Type>> parameters)
            : Type(std::move(name)), parameters{std::move(parameters)}
    {

    }

    bool IsLeaf() override
    {
        return false;
    }

    bool Equals(const Ptr<Type> &other) override
    {
        if (other->IsLeaf())
        {
            return false;
        }
        else
        {
            auto list = Cast<TypeList>(other);
            if (name == list->name)
            {
                auto comparator = [](const Ptr<Type> &x, const Ptr<Type> &y) -> bool
                {
                    return x->Equals(y);
                };
                return std::equal(parameters.begin(), parameters.end(), list->parameters.begin(),
                                  list->parameters.end(),
                                  comparator);
            }
            else
            {
                return false;
            }
        }
    }
};

class Value
{
public:
    String name;

    explicit Value(String name) noexcept : name{std::move(name)}
    {

    }

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
    explicit ValueLeaf(String name) noexcept : Value(std::move(name))
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
    Vector<Ptr<Value>> values;

    ValueList(String name, Vector<Ptr<Value>> values)
            : Value(std::move(name)), values{std::move(values)}
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
            if (name == list->name)
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
        return name + "[" + String::Join(",", items.begin(), items.end()) + "]";
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