#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Type.hpp"
#include "Ast.hpp"
#include "Predef.hpp"
#include "Scope.hpp"
#include "Exception.hpp"

class TypeException : public Exception
{
public:
    Position position;

    TypeException(Position position, String message)
            : Exception(std::move(message)), position{position}
    {
        AddInfo("position", position.ToString());
    }
};

class TypeChecker
{
public:
    HashMap<int, Ptr<Type>> typeRecord;

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

    Ptr<Type> Check(const Ptr<Ast> &node, const Ptr<Scope> &scope)
    {
        auto record = [this, node](const Ptr<Type> &type) -> Ptr<Type>
        {
            this->typeRecord.insert({node->id, type});
            return type;
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
            case Kind::TypeExpr:
                return record(CheckTypeExpression(Cast<TypeExpression>(node), scope));
        }
        throw NotImplementedException();
    }

    template<Kind kind>
    Ptr<Type> CheckBinaryArithmetic(Ptr<Binary<kind>> node, const Ptr<Scope> &scope)
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

        auto leftType = Check(node->left, scope);
        auto rightType = Check(node->right, scope);
        Optional<Any> result = scope->Lookup(binaryOperators[kind], "Operator");
        if (result)
        {
            const auto &functions = (*result).AnyCast<Vector<Ptr<FunctionType>>>();
            for (const auto &function: functions)
            {
                if (MatchParameters(function, {leftType, rightType}))
                {
                    return function->returnType;
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
    Ptr<Type> CheckUnaryArithmetic(Ptr<Unary<kind>> node, const Ptr<Scope> &scope)
    {
        static HashMap<Kind, String> unaryOperators = {
                {Kind::UnaryPlus,  "+"},
                {Kind::UnaryMinus, "-"},
                {Kind::Not,        "not"}
        };

        auto operandType = Check(node->operand, scope);
        auto opChar = unaryOperators[kind];
        Optional<Any> result = scope->Lookup(unaryOperators[kind], "Operator");
        if (result)
        {
            const auto &functions = (*result).AnyCast<Vector<Ptr<FunctionType>>>();
            for (const auto &function: functions)
            {
                if (MatchParameters(function, {operandType}))
                {
                    return function->returnType;
                }
            }
            throw TypeException(node->position, String("arithmetic type '") + KindToString(kind) + String("'"));
        }
        else
        {
            throw TypeException(node->position, String("arithmetic type '") + KindToString(kind) + String("'"));
        }
    }

    static bool MatchParameters(const Ptr<FunctionType> &function, const Vector<Ptr<Type>> &arguments)
    {
        auto comparator = [](const Ptr<Type> &x, const Ptr<Type> &y) -> bool
        {
            return x->Equals(y);
        };
        return std::equal(function->parameters.begin(),
                          function->parameters.end(),
                          arguments.begin(), arguments.end(),
                          comparator);
    }

    static Ptr<Type> CheckConstant(const Ptr<Constant> &node)
    {
        switch (node->constantType)
        {
            case Constant::ConstantType::Int32Type:
                return Type::INT;
            case Constant::ConstantType::Int64Type:
                return Type::LONG;
            case Constant::ConstantType::FloatType:
                return Type::FLOAT;
            case Constant::ConstantType::DoubleType:
                return Type::DOUBLE;
            case Constant::ConstantType::BooleanType:
                return Type::BOOL;
            case Constant::ConstantType::CharType:
                return Type::CHAR;
            case Constant::ConstantType::StringType:
                return Type::STRING;
            default:
                throw NotImplementedException();
        }
    }

    Ptr<Type> CheckBlock(const Ptr<Block> &node, const Ptr<Scope> &scope)
    {
        Ptr<Type> result = Type::VOID;
        for (const auto &expression: node->expressions)
        {
            result = Check(expression, scope);
        }
        return result;
    }

    static Ptr<Type> CheckName(const Ptr<Name> &node, const Ptr<Scope> &scope)
    {
        Optional<Any> result = scope->Lookup(node->name, "Identifier");
        if (result)
        {
            return (*result).AnyCast<Ptr<Type>>();
        }
        else
        {
            throw TypeException(node->position, "identifier '" + node->name + "' not defined");
        }
    }

    Ptr<Type> CheckTypeExpression(const Ptr<TypeExpression> &expression, const Ptr<Scope> &scope)
    {
        Optional<Any> result = scope->Lookup(expression->name, "Type");
        if (result)
        {
            Ptr<Type> type = (*result).AnyCast<Ptr<Type>>();
            Vector<Ptr<Type>> parameters = Enumerate::Map(expression->parameters,
                                                          [this, &scope](const Ptr<TypeExpression> &exp)
                                                          {
                                                              return Check(exp, scope);
                                                          });
            if (expression->parameters.empty())
            {
                return type;
            }
            else if (type->GetTypeCode() == TypeCode::FUNCTION)
            {
                auto functionType = Cast<FunctionType>(type);
                Vector<Ptr<Type>> functionParams = functionType->parameters;
                functionParams.push_back(functionType->returnType);
                auto comparator = [](const Ptr<Type> &x, const Ptr<Type> &y) -> bool
                {
                    return x->Equals(y);
                };
                if (std::equal(functionParams.begin(),
                               functionParams.end(),
                               parameters.begin(), parameters.end(),
                               comparator))
                {
                    return functionType;
                }
                else
                {
                    throw TypeException(expression->position, "Type not match");
                }
            }
            else
            {
                // Generic Classes
                throw NotImplementedException();
            }
        }
        else
        {
            throw TypeException(expression->position, "type not declared");
        }
    }

    Ptr<Type> CheckVar(const Ptr<Var> &node, const Ptr<Scope> &scope)
    {
        if (node->value)
        {
            auto value = Check(*(node->value), scope);
            if (node->type)
            {
                auto declaration = Check(*(node->type), scope);
                if (value->Equals(declaration))
                {
                    scope->Put(node->name, "Identifier", value);
                    return Type::VOID;
                }
                else
                {
                    throw TypeException(node->position, "variable type");
                }
            }
            else
            {
                return Type::VOID;
            }
        }
        else
        {
            if (node->type)
            {
                auto declaration = Check(*(node->type), scope);
                scope->Put(node->name, "Identifier", declaration);
                return Type::VOID;
            }
            else
            {
                throw TypeException(node->position, "variable type missing");
            }
        }
    }

    Ptr<Type> CheckCall(const Ptr<Call> &node, const Ptr<Scope> &scope)
    {
        auto function = Check(node->function, scope);
        Vector<Ptr<Type>> arguments;
        arguments.reserve(node->arguments.size());
        for (const auto &arg: node->arguments)
        {
            arguments.push_back(Check(arg, scope));
        }
        if (function->GetTypeCode() == TypeCode::FUNCTION)
        {
            if (MatchParameters(Cast<FunctionType>(function), arguments))
            {
                return Cast<FunctionType>(function)->returnType;
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

    Ptr<Type> CheckModule(const Ptr<DefModule> &node, const Ptr<Scope> &scope)
    {
        auto fieldTypes = Enumerate::Map(node->fields, [this, &scope](const Ptr<Var>& var)
        {
            return Check(*(var->type), scope);
        });
        auto methodTypes = Enumerate::Map(node->methods, [this, &scope](const Ptr<Def>& def)
        {
            return Check(def->type, scope);
        });
        Ptr<ObjectType> module = New<ObjectType>(fieldTypes, methodTypes);
        scope->Put(node->name, "**Module**", module);
        for (const auto &field: node->fields)
        {
            Check(field, scope);
        }
        for (const auto &method: node->methods)
        {
            Check(method, scope);
        }
        return Type::VOID;
    }

    Ptr<Type> CheckIfThen(const Ptr<IfThen> &node, const Ptr<Scope> &scope)
    {
        auto condition = Check(node->condition, scope);
        if (condition->GetTypeCode() == TypeCode::BOOL)
        {
            Check(node->ifTrue, scope);
            return Type::VOID;
        }
        else
        {
            throw TypeException(node->position, "condition must be boolean type");
        }
    }

    Ptr<Type> CheckIfElse(const Ptr<IfElse> &node, const Ptr<Scope> &scope)
    {
        auto condition = Check(node->condition, scope);
        if (condition->GetTypeCode() == TypeCode::BOOL)
        {
            Check(node->ifTrue, scope);
            Check(node->ifFalse, scope);
            return Type::VOID;
        }
        else
        {
            throw TypeException(node->position, "condition must be boolean type");
        }
    }

    Ptr<Type> CheckReturn(const Ptr<Return> &node, const Ptr<Scope> &scope)
    {
        auto value = Check(node->value, scope);
        Optional<Any> result = scope->Lookup("**Function**", "Type");
        if (result)
        {
            auto function = Cast<FunctionType>((*result).AnyCast<Ptr<Type>>());
            if (value->Equals(function->returnType))
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

    Ptr<Type> CheckDef(const Ptr<Def> &node, Ptr<Scope> scope)
    {
        auto functionType = Check(node->type, scope);
        scope->Put(node->name, "Identifier", functionType);
        auto functionScope = New<Scope>(scope);
        functionScope->Put("**Function**", "Type", functionType);
        for (const auto &parameter: node->parameters)
        {
            functionScope->Put(parameter.name, "Identifier", Check(parameter.type, functionScope));
        }
        Check(node->body, functionScope);
        return Type::VOID;
    }
};

#endif // TYPE_CHECKER