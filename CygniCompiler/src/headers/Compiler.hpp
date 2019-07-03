#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Ast.hpp"
#include "Predef.hpp"
#include "TypeChecker.hpp"
#include "Location.hpp"
#include "Endian.hpp"

using Byte = uint8_t;

enum class Op
{
    PUSH_NULL,
    PUSH_CONSTANT,
    PUSH_INT_MINUS_ONE,
    PUSH_INT_1BYTE,
    PUSH_INT_2BYTE,

    PUSH_STACK,
    POP_STACK,

    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    GT,
    LT,
    GE,
    LE,
    EQ,
    NE,

    POP,
    DUPLICATE,

    CALL,
    NATIVE_CALL,
    TAIL_CALL,

    PUSH_ARRAY_ELEMENT,
    POP_ARRAY_ELEMENT,

    PUSH_FIELD,
    POP_FIELD,

    PUSH_STATIC,
    POP_STATIC,

    PUSH_CHARACTER_IN_STRING,

    BIT_AND,
    BIT_OR,
    BIT_XOR,

    CAST,
    UP_CAST,
    DOWN_CAST,

    PUSH_METHOD,
    PUSH_STATIC_METHOD,

    NEW,
    NEW_ARRAY,

    SUPER,
    INSTANCE_OF,

    THROW,
    RETHROW,

    JUMP,
    JUMP_IF_TRUE,
    JUMP_IF_FALSE,

    RETURN
};

enum class OpTypeCode
{
    INT_32,
    INT_64,
    FLOAT_32,
    FLOAT_64
};

class Compiler
{
private:

    class Rule
    {
    public:
        Vector<Ptr<Type>> types;
        Op instruction;

        Rule(Vector<Ptr<Type>> types, Op instruction)
                : types{std::move(types)}, instruction{instruction}
        {

        }
    };

    HashMap<Kind, Vector<Rule>> rules;
public:
    const HashMap<int, Ptr<Type>> &typeRecord;
    const HashMap<int, Location> &locations;

    Compiler(const HashMap<int, Ptr<Type>> &typeRecord, const HashMap<int, Location> &locations)
            : typeRecord{typeRecord}, locations{locations}
    {
        Register();
    }

    Vector<Byte> Compile(const Program &program)
    {
        Vector<Byte> code;
        for (const auto &module: program.modules)
        {
            CompileNode(module, code);
        }
        for (const auto &_class: program.classes)
        {
            CompileNode(_class, code);
        }
        return code;
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
                CompileConstant(Cast<Constant>(node), code);
                break;
            case Kind::Block:
                CompileBlock(Cast<Block>(node), code);
                break;
            case Kind::Name:
                break;
            case Kind::Return:
                break;
            case Kind::Var:
                break;
            case Kind::Def:
                CompileDef(Cast<Def>(node), code);
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
                CompileModule(Cast<DefModule>(node), code);
                break;
            default:
                throw NotImplementedException();
        }
    }

    const Ptr<Type> &TypeOf(const Ptr<Ast> &node) const
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

    Op Match(Kind kind, const Vector<Ptr<Type>> &values)
    {
        auto comparator = [](const Ptr<Type> &x, const Ptr<Type> &y) -> bool
        {
            return x->Equals(y);
        };
        for (const auto &items: rules.at(kind))
        {
            if (std::equal(items.types.begin(), items.types.end(), values.begin(), values.end(), comparator))
            {
                return items.instruction;
            }
        }
        throw NotSupportedException();
    }

    void CompileConstant(const Ptr<Constant> &node, Vector<Byte> &code)
    {
        int index = locations.at(node->id).Index();
        Op op = Match(node->kind, {TypeOf(node)});
        code.push_back(static_cast<Byte>(op));
        EmitUInt16(code, index);
    }

    void CompileBlock(const Ptr<Block> &node, Vector<Byte> &code)
    {
        for (const auto &exp: node->expressions)
        {
            CompileNode(exp, code);
        }
    }

    void CompileModule(const Ptr<DefModule> &node, Vector<Byte> &code)
    {
        EmitString(code, node->name);
        EmitUInt16(code, locations.at(node->id).Index());
        EmitUInt16(code, node->fields.size());
        EmitUInt16(code, node->methods.size());
        for (const auto &field: node->fields)
        {
            CompileNode(field, code);
        }
        for (const auto &method: node->methods)
        {
            CompileNode(method, code);
        }
    }

    void CompileDef(const Ptr<Def> &node, Vector<Byte> &code)
    {
        EmitString(code, node->name);
        EmitUInt16(code, locations.at(node->id).Index());
        EmitUInt16(code, node->parameters.size());
        CompileNode(node->body, code);
    }

    void EmitUInt16(Vector<Byte> &code, int number)
    {
        Byte *bytes = Endian::UInt16ToBytes(static_cast<uint16_t>(number));
        code.push_back(bytes[0]);
        code.push_back(bytes[1]);
    }

    void EmitInt32(Vector<Byte> &code, int32_t number)
    {
        Byte *bytes = Endian::Int32ToBytes(number);
        for (int i = 0; i < 4; i++)
        {
            code.push_back(bytes[i]);
        }
    }

    void EmitString(Vector<Byte> &code, String s)
    {
        EmitInt32(code, s.size());
        auto cppString = s.ToCppString();
        for (const auto &c: cppString)
        {
            code.push_back(static_cast<Byte>(c));
        }
    }

    void Register()
    {
//        rules.insert({Kind::Add, {
//                {{Value::INT, Value::INT}, Op::ADD_INT}
//        }});
//        rules.insert({Kind::Subtract, {
//                {{Value::INT, Value::INT}, Op::SUB_INT}
//        }});
//        rules.insert({Kind::Multiply, {
//                {{Value::INT, Value::INT}, Op::MUL_INT}
//        }});
//        rules.insert({Kind::Divide, {
//                {{Value::INT, Value::INT}, Op::DIV_INT}
//        }});
//        rules.insert({Kind::Modulo, {
//                {{Value::INT, Value::INT}, Op::MOD_INT}
//        }});
//        rules.insert({Kind::GreaterThan, {
//                {{Value::INT, Value::INT}, Op::GT_INT}
//        }});
//        rules.insert({Kind::LessThan, {
//                {{Value::INT, Value::INT}, Op::LT_INT}
//        }});
//        rules.insert({Kind::GreaterThanOrEqual, {
//                {{Value::INT, Value::INT}, Op::GE_INT}
//        }});
//        rules.insert({Kind::LessThanOrEqual, {
//                {{Value::INT, Value::INT}, Op::LE_INT}
//        }});
//        rules.insert({Kind::Equal, {
//                {{Value::INT, Value::INT}, Op::EQ_INT}
//        }});
//        rules.insert({Kind::NotEqual, {
//                {{Value::INT, Value::INT}, Op::NE_INT}
//        }});
//        rules.insert({Kind::Return, {
//                {{Value::INT}, Op::RETURN_INT}
//        }});
//        rules.insert({Kind::Constant, {
//                {{Value::INT}, Op::PUSH_CONSTANT_INT}
//        }});
    }
};


#endif //COMPILER_HPP
