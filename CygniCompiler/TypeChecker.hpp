#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Type.hpp"
#include "Ast.hpp"
#include "Predef.hpp"
#include "Scope.hpp"
#include <iostream>

using std::cout;
using std::endl;

class TypeException : std::exception
{
public:
    Position position;
    String message;

    TypeException(Position position, String message)
            : position{position}, message{std::move(message)}
    {

    }
};

class NotSupportedException : std::exception
{

};

class TypeChecker
{
public:
    HashMap<int, Ptr<Value>> typeRecord;

    void CheckProgram(const Program &program, const Ptr<Scope> &scope)
    {
        for (const auto &_class: program.classes)
        {
            Check(_class, scope);
        }
        for (const auto &module:program.modules)
        {
            Check(module, scope);
        }
    }

    Ptr<Value> Check(const Ptr<Ast> &node, const Ptr<Scope> &scope)
    {
        auto record = [this, node](const Ptr<Value> &value) -> Ptr<Value>
        {
            this->typeRecord.insert({node->id, value});
            return value;
        };
        switch (node->kind)
        {
            case Kind::Add:
                return record(CheckBinaryArithmetic(Cast<Add>(node), scope));
            case Kind::Subtract:
                return record(CheckBinaryArithmetic(Cast<Subtract>(node), scope));
            case Kind::Multiply:
                return record(CheckBinaryArithmetic(Cast<Multiply>(node), scope));
            case Kind::Divide:
                return record(CheckBinaryArithmetic(Cast<Divide>(node), scope));
            case Kind::Modulo:
                return record(CheckBinaryArithmetic(Cast<Modulo>(node), scope));
            case Kind::GreaterThan:
                return record(CheckBinaryArithmetic(Cast<GreaterThan>(node), scope));
            case Kind::LessThan:
                return record(CheckBinaryArithmetic(Cast<LessThan>(node), scope));
            case Kind::GreaterThanOrEqual:
                return record(CheckBinaryArithmetic(Cast<GreaterThanOrEqual>(node), scope));
            case Kind::LessThanOrEqual:
                return record(CheckBinaryArithmetic(Cast<LessThanOrEqual>(node), scope));
            case Kind::Equal:
                return record(CheckBinaryArithmetic(Cast<Equal>(node), scope));
            case Kind::NotEqual:
                return record(CheckBinaryArithmetic(Cast<NotEqual>(node), scope));
            case Kind::UnaryPlus:
                return record(CheckUnaryArithmetic(Cast<UnaryPlus>(node), scope));
            case Kind::UnaryMinus:
                return record(CheckUnaryArithmetic(Cast<UnaryMinus>(node), scope));
            case Kind::And:
                throw NotSupportedException();
            case Kind::Or:
                throw NotSupportedException();
            case Kind::Not:
                return record(CheckUnaryArithmetic(Cast<Not>(node), scope));
            case Kind::IfThen:
                return record(CheckIfThen(Cast<IfThen>(node), scope));
            case Kind::IfElse:
                return record(CheckIfElse(Cast<IfElse>(node), scope));
            case Kind::Constant:
                return record(CheckConstant(Cast<Constant>(node)));
            case Kind::Block:
                return record(CheckBlock(Cast<Block>(node), scope));
            case Kind::Name:
                return record(CheckName(Cast<Name>(node), scope));
            case Kind::Return:
                return record(CheckReturn(Cast<Return>(node), scope));
            case Kind::Var:
                return record(CheckVar(Cast<Var>(node), scope));
            case Kind::Def:
                return record(CheckDef(Cast<Def>(node), scope));
            case Kind::Assign:
                break;
            case Kind::Call:
                return record(CheckCall(Cast<Call>(node), scope));
            case Kind::While:
                break;
            case Kind::DefClass:
                break;
            case Kind::DefModule:
                return record(CheckModule(Cast<DefModule>(node), scope));
        }
        throw NotSupportedException();
    }

    template<Kind kind>
    Ptr<Value> CheckBinaryArithmetic(Ptr<Binary<kind>> node, const Ptr<Scope> &scope)
    {
        static HashMap<Kind, String> binaryOperators = {
                {Kind::Add,                "+"},
                {Kind::Subtract,           "-"},
                {Kind::Multiply,           "*"},
                {Kind::Divide,             "/"},
                {Kind::Modulo,             "%"},
                {Kind::GreaterThan,        ">"},
                {Kind::LessThan,           "<"},
                {Kind::GreaterThanOrEqual, ">="},
                {Kind::LessThanOrEqual,    "<="},
                {Kind::Equal,              "=="},
                {Kind::NotEqual,           "!="},
        };

        auto leftValue = Check(node->left, scope);
        auto rightValue = Check(node->right, scope);
        Optional<Any> result = scope->Lookup(binaryOperators[kind], "Operator");
        if (result)
        {
            const Vector<Ptr<ValueList>> &functions = (*result).AnyCast<Vector<Ptr<ValueList>>>();
            for (const auto &function: functions)
            {
                if (MatchParameters(function, {leftValue, rightValue}))
                {
                    return function->values.back();
                }
            }
            throw TypeException(node->position, String("arithmetic type '") + KindToString(kind) + String("'"));
        }
        else
        {
            throw TypeException(node->position,
                                String("arithmetic computation '") + KindToString(kind) + String("' not supported"));
        }
    }

    template<Kind kind>
    Ptr<Value> CheckUnaryArithmetic(Ptr<Unary<kind>> node, const Ptr<Scope> &scope)
    {
        static HashMap<Kind, String> unaryOperators = {
                {Kind::UnaryPlus,  "+"},
                {Kind::UnaryMinus, "-"},
                {Kind::Not,        "not"}
        };

        auto operandValue = Check(node->operand, scope);
        auto opChar = unaryOperators[kind];
        Optional<Any> result = scope->Lookup(unaryOperators[kind], "Operator");
        if (result)
        {
            Vector<Ptr<ValueList>> functions = (*result).AnyCast<Vector<Ptr<ValueList>>>();
            for (const auto &function: functions)
            {
                if (MatchParameters(function, {operandValue}))
                {
                    return function->values.back();
                }
            }
            throw TypeException(node->position, String("arithmetic type '") + KindToString(kind) + String("'"));
        }
        else
        {
            throw TypeException(node->position, String("arithmetic type '") + KindToString(kind) + String("'"));
        }
    }

    static bool MatchParameters(const Ptr<ValueList> &function, const Vector<Ptr<Value>> &arguments)
    {
        auto comparator = [](const Ptr<Value> &x, const Ptr<Value> &y) -> bool
        {
            return x->Equals(y);
        };
        return std::equal(function->values.begin(), function->values.end() - 1, arguments.begin(), arguments.end(),
                          comparator);
    }

    Ptr<Value> CheckConstant(const Ptr<Constant> &node)
    {
        switch (node->constantType)
        {
            case Constant::ConstantType::Int32Type:
                return Value::IntValue;
            case Constant::ConstantType::Int64Type:
                return Value::LongValue;
            case Constant::ConstantType::FloatType:
                return Value::FloatValue;
            case Constant::ConstantType::DoubleType:
                return Value::DoubleValue;
            case Constant::ConstantType::BooleanType:
                return Value::BoolValue;
            case Constant::ConstantType::CharType:
                return Value::CharValue;
            case Constant::ConstantType::StringType:
                return Value::StringValue;
        }
    }

    Ptr<Value> CheckBlock(const Ptr<Block> &node, const Ptr<Scope> &scope)
    {
        for (const Ptr<Ast> &expression: node->expressions)
        {
            Check(expression, scope);
        }
        return Value::UnitValue;
    }

    Ptr<Value> CheckName(const Ptr<Name> &node, const Ptr<Scope> &scope)
    {
        Optional<Any> result = scope->Lookup(node->name, "Identifier");
        if (result)
        {
            return (*result).AnyCast<Ptr<Value>>();
        }
        else
        {
            throw TypeException(node->position, "identifier '" + node->name + "' not defined");
        }
    }

    Ptr<Value> CheckVar(const Ptr<Var> &node, const Ptr<Scope> &scope)
    {
        if (node->value)
        {
            auto value = Check(*(node->value), scope);
            if (node->type)
            {
                auto declaration = CheckType(*(node->type), scope);
                if (value->Equals(declaration))
                {
                    scope->Put(node->name, "Identifier", value);
                    return Value::UnitValue;
                }
                else
                {
                    throw TypeException(node->position, "variable type");
                }
            }
            else
            {
                return Value::UnitValue;
            }
        }
        else
        {
            if (node->type)
            {
                auto declaration = CheckType(*(node->type), scope);
                scope->Put(node->name, "Identifier", declaration);
                return Value::UnitValue;
            }
            else
            {
                throw TypeException(node->position, "variable type missing");
            }
        }
    }

    Ptr<Value> CheckType(const Ptr<Type> &type, const Ptr<Scope> &scope)
    {
        // TO DO: search in the scope
        if (type->IsLeaf())
        {
            return New<ValueLeaf>(Cast<TypeLeaf>(type)->name);
        }
        else
        {
            auto list = Cast<TypeList>(type);
            Vector<Ptr<Value>> values;
            values.reserve(list->parameters.size());
            for (const auto &parameter: list->parameters)
            {
                values.push_back(CheckType(parameter, scope));
            }
            return New<ValueList>(list->name, values);
        }
    }

    Ptr<Value> CheckCall(const Ptr<Call> &node, const Ptr<Scope> &scope)
    {
        auto function = Check(node->function, scope);
        Vector<Ptr<Value>> arguments;
        arguments.reserve(node->arguments.size());
        for (const auto &arg: node->arguments)
        {
            arguments.push_back(Check(arg, scope));
        }
        if (!(function->IsLeaf()) && function->name == "Function")
        {
            if (MatchParameters(Cast<ValueList>(function), arguments))
            {
                return Cast<ValueList>(function)->values.back();
            }
            else
            {
                throw TypeException(node->position, "function call arguments' type");
            }
        }
        else
        {
            throw TypeException(node->position, "unable to call");
        }
    }

    Ptr<Value> CheckModule(const Ptr<DefModule> &node, const Ptr<Scope> &scope)
    {
        scope->Put(node->name, "Identifier", New<TypeLeaf>(node->name));
        for (const auto &field: node->fields)
        {
            Check(field, scope);
        }
        for (const auto &method: node->methods)
        {
            Check(method, scope);
        }
        return Value::UnitValue;
    }

    Ptr<Value> CheckIfThen(const Ptr<IfThen> &node, const Ptr<Scope> &scope)
    {
        auto condition = Check(node->condition, scope);
        if (condition->IsLeaf() && condition->name == "Bool")
        {
            Check(node->ifTrue, scope);
            return Value::UnitValue;
        }
        else
        {
            throw TypeException(node->position, "condition must be boolean type");
        }
    }

    Ptr<Value> CheckIfElse(const Ptr<IfElse> &node, const Ptr<Scope> &scope)
    {
        auto condition = Check(node->condition, scope);
        if (condition->IsLeaf() && condition->name == "Bool")
        {
            Check(node->ifTrue, scope);
            Check(node->ifFalse, scope);
            return Value::UnitValue;
        }
        else
        {
            throw TypeException(node->position, "condition must be boolean type");
        }
    }

    Ptr<Value> CheckReturn(const Ptr<Return> &node, const Ptr<Scope> &scope)
    {
        auto value = Check(node->value, scope);
        Optional<Any> result = scope->Lookup("**Function**", "Type");
        if (result)
        {
            auto function = Cast<ValueList>((*result).AnyCast<Ptr<Value>>());
            if (value->Equals(function->values.back()))
            {
                return value;
            }
            else
            {
                throw TypeException(node->position, "return type does not match");
            }
        }
        else
        {
            throw TypeException(node->position, "return statement should be in a function");
        }
    }

    Ptr<Value> CheckDef(const Ptr<Def> &node, Ptr<Scope> scope)
    {
        auto functionValue = CheckType(node->type, scope);
        scope->Put(node->name, "Identifier", functionValue);
        auto functionScope = New<Scope>(scope);
        functionScope->Put("**Function**", "Type", functionValue);
        for (const auto &parameter: node->parameters)
        {
            functionScope->Put(parameter.name, "Identifier", CheckType(parameter.type, functionScope));
        }
        Check(node->body, functionScope);
        return Value::UnitValue;
    }
};

#endif // TYPE_CHECKER