#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Ast.hpp"
#include "Predef.hpp"
#include "TypeChecker.hpp"

using Byte = uint8_t;

enum class Op
{
    PUSH_CONSTANT_INT,
    ADD_INT,
    SUB_INT,
    MUL_INT,
    DIV_INT,
    MOD_INT,

    GT_INT,
    LT_INT,
    GE_INT,
    LE_INT,
    EQ_INT,
    NE_INT,

    JUMP,
    JUMP_IF_TRUE,
    JUMP_IF_FALSE,

    RETURN_INT,
    INVOKE
};

class Compiler
{
private:

    class Rule
    {
    public:
        Vector<Ptr<Value>> values;
        Op instruction;

        Rule(Vector<Ptr<Value>> values, Op instruction)
                : values{std::move(values)}, instruction{instruction}
        {

        }
    };

    HashMap<Kind, Vector<Rule>> rules;
public:
    const HashMap<int, Ptr<Value>> &typeRecord;

    explicit Compiler(const HashMap<int, Ptr<Value>> &typeRecord) : typeRecord{typeRecord}
    {
        Register();
    }

    void CompileNode(const Ptr<Ast> &node, Vector<Byte> &code)
    {
        switch (node->kind)
        {
            case Kind::Add:
                return CompileBinary(Cast<Add>(node), code);
            case Kind::Subtract:
                return CompileBinary(Cast<Subtract>(node), code);
            case Kind::Multiply:
                return CompileBinary(Cast<Multiply>(node), code);
            case Kind::Divide:
                return CompileBinary(Cast<Divide>(node), code);
            case Kind::Modulo:
                return CompileBinary(Cast<Modulo>(node), code);
            case Kind::GreaterThan:
                return CompileBinary(Cast<GreaterThan>(node), code);
            case Kind::LessThan:
                return CompileBinary(Cast<LessThan>(node), code);
            case Kind::GreaterThanOrEqual:
                return CompileBinary(Cast<GreaterThanOrEqual>(node), code);
            case Kind::LessThanOrEqual:
                return CompileBinary(Cast<LessThanOrEqual>(node), code);
            case Kind::Equal:
                return CompileBinary(Cast<Equal>(node), code);
            case Kind::NotEqual:
                return CompileBinary(Cast<NotEqual>(node), code);
            case Kind::And:
                break;
            case Kind::Or:
                break;
            case Kind::Not:
                break;
            case Kind::UnaryPlus:
                return CompileUnary(Cast<UnaryPlus>(node), code);
            case Kind::UnaryMinus:
                return CompileUnary(Cast<UnaryMinus>(node), code);
            case Kind::IfThen:
                break;
            case Kind::IfElse:
                break;
            case Kind::Constant:
                break;
            case Kind::Block:
                break;
            case Kind::Name:
                break;
            case Kind::Return:
                break;
            case Kind::Var:
                break;
            case Kind::Def:
                break;
            case Kind::Assign:
                break;
            case Kind::Call:
                break;
            case Kind::While:
                break;
            case Kind::DefClass:
                break;
            case Kind::DefModule:
                break;
        }
    }

    const Ptr<Value> &TypeOf(const Ptr<Ast> &node) const
    {
        return typeRecord.at(node->id);
    }


    template<Kind kind>
    void CompileBinary(const Ptr<Binary<kind>> &node, Vector<Byte> &code)
    {
        CompileNode(node->left, code);
        CompileNode(node->right, code);
        Op op = Match(kind, {TypeOf(node->left), TypeOf(node->right)});
        code.push_back(static_cast<Byte>(op));
    }

    template<Kind kind>
    void CompileUnary(const Ptr<Unary<kind>> &node, Vector<Byte> &code)
    {
        CompileNode(node->operand, code);
        Op op = Match(kind, {TypeOf(node->operand)});
        code.push_back(static_cast<Byte>(op));
    }

    Op Match(Kind kind, const Vector<Ptr<Value>> &values)
    {
        auto comparator = [](const Ptr<Value> &x, const Ptr<Value> &y) -> bool
        {
            return x->Equals(y);
        };
        for (const auto &items: rules.at(kind))
        {
            if (std::equal(items.values.begin(), items.values.end(), values.begin(), values.end(), comparator))
            {
                return items.instruction;
            }
        }
        throw NotSupportedException();
    }

    void Register()
    {
        rules.insert({Kind::Add, {
                {{Value::IntValue, Value::IntValue}, Op::ADD_INT}
        }});
        rules.insert({Kind::Subtract, {
                {{Value::IntValue, Value::IntValue}, Op::SUB_INT}
        }});
        rules.insert({Kind::Multiply, {
                {{Value::IntValue, Value::IntValue}, Op::MUL_INT}
        }});
        rules.insert({Kind::Divide, {
                {{Value::IntValue, Value::IntValue}, Op::DIV_INT}
        }});
        rules.insert({Kind::Modulo, {
                {{Value::IntValue, Value::IntValue}, Op::MOD_INT}
        }});
        rules.insert({Kind::GreaterThan, {
                {{Value::IntValue, Value::IntValue}, Op::GT_INT}
        }});
        rules.insert({Kind::LessThan, {
                {{Value::IntValue, Value::IntValue}, Op::LT_INT}
        }});
        rules.insert({Kind::GreaterThanOrEqual, {
                {{Value::IntValue, Value::IntValue}, Op::GE_INT}
        }});
        rules.insert({Kind::LessThanOrEqual, {
                {{Value::IntValue, Value::IntValue}, Op::LE_INT}
        }});
        rules.insert({Kind::Equal, {
                {{Value::IntValue, Value::IntValue}, Op::EQ_INT}
        }});
        rules.insert({Kind::NotEqual, {
                {{Value::IntValue, Value::IntValue}, Op::NE_INT}
        }});
        rules.insert({Kind::Return, {
                {{Value::IntValue}, Op::RETURN_INT}
        }});
    }
};


#endif //COMPILER_HPP
