#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Ast.hpp"
#include "Endian.hpp"
#include "Location.hpp"
#include "Locator.hpp"
#include "Predef.hpp"
#include "TypeChecker.hpp"

#define OP_NUMERIC(TYPE)                                                    \
  ADD_##TYPE, SUB_##TYPE, MUL_##TYPE, DIV_##TYPE, MOD_##TYPE, MINUS_##TYPE, \
      GT_##TYPE, LT_##TYPE, GE_##TYPE, LE_##TYPE, EQ_##TYPE, NE_##TYPE

#define OP_ACTION(TYPE)                                                  \
  PUSH_LOCAL_##TYPE, POP_LOCAL_##TYPE, RETURN_##TYPE, PUSH_FIELD_##TYPE, \
      POP_FIELD_##TYPE, PUSH_STATIC_##TYPE, POP_STATIC_##TYPE,           \
      NEW_ARRAY_##TYPE, PUSH_ARRAY_ELEMENT_##TYPE, POP_ARRAY_ELEMENT_##TYPE

#define OP_CONVERT(TYPE1, TYPE2) \
  CONVERT_##TYPE1##_TO_##TYPE2, CONVERT_##TYPE2##_TO_##TYPE1

#define OP_INTEGER(TYPE) BIT_AND_##TYPE, BIT_OR_##TYPE, BIT_XOR_##TYPE

enum Op {
  PUSH_INT_MINUS_ONE,
  PUSH_INT_1BYTE,
  PUSH_INT_2BYTE,
  PUSH_NULL,
  PUSH_CONSTANT_I32,
  PUSH_CONSTANT_I64,
  PUSH_CONSTANT_F32,
  PUSH_CONSTANT_F64,
  PUSH_CONSTANT_STRING,
  PUSH_CONSTANT_WIDE_I32,
  PUSH_CONSTANT_WIDE_I64,
  PUSH_CONSTANT_WIDE_F32,
  PUSH_CONSTANT_WIDE_F64,
  PUSH_CONSTANT_WIDE_STRING,

  OP_NUMERIC(I32),
  OP_NUMERIC(I64),
  OP_NUMERIC(F32),
  OP_NUMERIC(F64),

  OP_ACTION(I32),
  OP_ACTION(I64),
  OP_ACTION(F32),
  OP_ACTION(F64),
  OP_ACTION(OBJECT),

  OP_CONVERT(I32, I64),
  OP_CONVERT(I32, F32),
  OP_CONVERT(I32, F64),
  OP_CONVERT(I64, F32),
  OP_CONVERT(I64, F64),
  OP_CONVERT(F32, F64),

  OP_INTEGER(I32),
  OP_INTEGER(I64),

  POP,
  DUPLICATE,

  CALL,
  NATIVE_CALL,
  TAIL_CALL,

  UP_CAST,
  DOWN_CAST,

  PUSH_METHOD,
  PUSH_STATIC_METHOD,
  PUSH_FUNCTION,

  NEW,

  SUPER,
  INSTANCE_OF,

  THROW,
  RETHROW,

  JUMP,
  JUMP_IF_TRUE,
  JUMP_IF_FALSE
};

enum class OpTypeCode { INT_32, INT_64, FLOAT_32, FLOAT_64 };

enum class OpFlag {
  DEFINE_FUNCTION,
  DEFINE_CONSTANTS,
  DEFINE_MODULE,
  DEFINE_CLASS
};

typedef uint8_t Byte;

class CompilerException : Exception {
 public:
  Position position;

  CompilerException(Position position, String message)
      : Exception(std::move(message)), position{position} {
    AddInfo("startLine", String::ToString(position.startLine));
    AddInfo("startColumn", String::ToString(position.startColumn));
    AddInfo("endLine", String::ToString(position.endLine));
    AddInfo("endColumn", String::ToString(position.endColumn));
  }
};

class Compiler {
 private:
  class Rule {
   public:
    Vector<Ptr<Type>> types;
    Op instruction;

    Rule(Vector<Ptr<Type>> types, Op instruction)
        : types{std::move(types)}, instruction{instruction} {}
  };

  HashMap<Kind, Vector<Rule>> rules;

 public:
  const HashMap<int, Ptr<Type>>& typeRecord;
  const HashMap<int, Location>& locations;
  Locator& locator;

  Compiler(const HashMap<int, Ptr<Type>>& typeRecord,
           Locator& locator,
           const HashMap<int, Location>& locations)
      : typeRecord{typeRecord}, locator{locator}, locations{locations} {
    Register();
  }

  Vector<Byte> Compile(const Program& program) {
    Vector<Byte> code;
    CompileConstantPool(locator.constantPool, code);
    EmitUInt16(code, program.modules.size());
    EmitUInt16(code, program.classes.size());
    for (const auto& module : program.modules) {
      CompileNode(module, code);
    }
    for (const auto& _class : program.classes) {
      CompileNode(_class, code);
    }
    return code;
  }

  void CompileNode(const Ptr<Ast>& node, Vector<Byte>& code) {
    switch (node->kind) {
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
        break;
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
        CompileClass(Cast<DefClass>(node), code);
        break;
      case Kind::DefModule:
        CompileModule(Cast<DefModule>(node), code);
        return;
        break;
      default:
        throw NotImplementedException();
    }
    throw NotImplementedException();
  }

  const Ptr<Type>& TypeOf(const Ptr<Ast>& node) const {
    return typeRecord.at(node->id);
  }

  void CompileConstantPool(const Locator::ConstantPool& constantPool,
                           Vector<Byte>& code) {
    EmitInt32(code, constantPool.constants.size());
    for (const Ptr<Constant>& constant : constantPool.constants) {
      EmitByte(code, static_cast<int>(constant->constantType));
      switch (constant->constantType) {
        case Constant::ConstantType::Int32Type: {
          auto value = String::ParseInt(constant->value);
          EmitInt32(code, value);
          break;
        }
        case Constant::ConstantType::Int64Type: {
          auto value = String::ParseLong(constant->value);
          EmitInt64(code, value);
          break;
        }
        case Constant::ConstantType::FloatType: {
          auto value = String::ParseFloat(constant->value);
          EmitFloat32(code, value);
          break;
        }
        case Constant::ConstantType::DoubleType: {
          auto value = String::ParseDouble(constant->value);
          EmitFloat64(code, value);
          break;
        }
        case Constant::ConstantType::BooleanType: {
          if (constant->value == String("true")) {
            EmitByte(code, 1);
          } else {
            EmitByte(code, 0);
          }
          break;
        }
        case Constant::ConstantType::CharType: {
          Char value = constant->value.at(0);
          EmitInt32(code, static_cast<int32_t>(value));
          break;
        }
        case Constant::ConstantType::StringType: {
          EmitString(code, constant->value);
          break;
        }
      }
    }
  }

  template <Kind kind>
  void CompileBinary(const Ptr<Binary<kind>>& node, Vector<Byte>& code) {
    CompileNode(node->left, code);
    CompileNode(node->right, code);
    Op op = Match(node, {TypeOf(node->left), TypeOf(node->right)});
    EmitOp(code, op);
  }

  template <Kind kind>
  void CompileUnary(const Ptr<Unary<kind>>& node, Vector<Byte>& code) {
    CompileNode(node->operand, code);
    Op op = Match(node, {TypeOf(node->operand)});
    EmitOp(code, op);
  }

  Op Match(const Ptr<Ast>& node, const Vector<Ptr<Type>>& values) {
    auto comparator = [](const Ptr<Type>& x, const Ptr<Type>& y) -> bool {
      return x->Equals(y);
    };
    Kind kind = node->kind;
    for (const auto& items : rules.at(kind)) {
      if (std::equal(items.types.begin(), items.types.end(), values.begin(),
                     values.end(), comparator)) {
        return items.instruction;
      }
    }
    throw CompilerException(node->position, "type not match");
  }

  void CompileConstant(const Ptr<Constant>& node, Vector<Byte>& code) {
    int index = locator.locations.at(node->id).Index();
    Op op = Match(node, {TypeOf(node)});
    EmitOp(code, op);
    if (index < 65536) {
      EmitUInt16(code, index);
    } else {
      // PUSH_CONSTANT_WIDE_...
      throw NotImplementedException();
    }
  }

  void CompileBlock(const Ptr<Block>& node, Vector<Byte>& code) {
    for (const auto& exp : node->expressions) {
      CompileNode(exp, code);
    }
  }

  void CompileModule(const Ptr<DefModule>& node, Vector<Byte>& code) {
    EmitFlag(code, OpFlag::DEFINE_MODULE);
    EmitString(code, node->name);
    EmitUInt16(code, locations.at(node->id).Index());
    EmitUInt16(code, node->fields.size());
    EmitUInt16(code, node->methods.size());
    for (const auto& field : node->fields) {
      CompileNode(field, code);
    }
    for (const auto& method : node->methods) {
      CompileNode(method, code);
    }
  }

  void CompileDef(const Ptr<Def>& node, Vector<Byte>& code) {
    EmitFlag(code, OpFlag::DEFINE_FUNCTION);
    EmitString(code, node->name);
    EmitUInt16(code, locations.at(node->id).Index());
    int locals = locator.functionLocals[node->id];
    EmitUInt16(code, locals);                  /* locals */
    EmitUInt16(code, 0);                       /* TO DO: stack */
    EmitUInt16(code, node->parameters.size()); /* args_size */
    int pos = code.size();
    CompileNode(node->body, code);
    int codeSize = static_cast<int>(code.size()) - pos;
    if (codeSize >= 65536) {
      throw CompilerException(node->position, "function code size too large");
    }
  }

  void CompileClass(const Ptr<DefClass>& node, Vector<Byte>& code) {
    EmitFlag(code, OpFlag::DEFINE_CLASS);
    EmitString(code, node->name);
    EmitUInt16(code, locations.at(node->id).Index());
    EmitUInt16(code, node->fields.size());
    EmitUInt16(code, node->methods.size());
    for (const auto& field : node->fields) {
      CompileNode(field, code);
    }
    for (const auto& method : node->methods) {
      CompileNode(method, code);
    }
  }

  void CompileIfThen(const Ptr<IfThen>& node, Vector<Byte>& code) {
    // TO DO
  }

  void EmitUInt16(Vector<Byte>& code, int number) {
    Byte* bytes = Endian::UInt16ToBytes(static_cast<uint16_t>(number));
    code.push_back(bytes[0]);
    code.push_back(bytes[1]);
  }

  void EmitInt32(Vector<Byte>& code, int32_t number) {
    Byte* bytes = Endian::Int32ToBytes(number);
    for (int i = 0; i < 4; i++) {
      code.push_back(bytes[i]);
    }
  }

  void EmitInt64(Vector<Byte>& code, int64_t number) {
    Byte* bytes = Endian::Int64ToBytes(number);
    for (int i = 0; i < 8; i++) {
      code.push_back(bytes[i]);
    }
  }

  void EmitFloat32(Vector<Byte>& code, float number) {
    Byte* bytes = Endian::Float32ToBytes(number);
    for (int i = 0; i < 4; i++) {
      code.push_back(bytes[i]);
    }
  }

  void EmitFloat64(Vector<Byte>& code, double number) {
    Byte* bytes = Endian::Float64ToBytes(number);
    for (int i = 0; i < 8; i++) {
      code.push_back(bytes[i]);
    }
  }

  void EmitFlag(Vector<Byte>& code, OpFlag flag) {
    code.push_back(static_cast<Byte>(flag));
  }

  void EmitByte(Vector<Byte>& code, int value) {
    code.push_back(static_cast<Byte>(value));
  }

  void EmitOp(Vector<Byte>& code, Op op) {
    code.push_back(static_cast<Byte>(op));
  }

  void EmitString(Vector<Byte>& code, String s) {
    EmitInt32(code, s.size());
    auto cppString = s.ToCppString();
    for (const auto& c : cppString) {
      code.push_back(static_cast<Byte>(c));
    }
  }

  void Register() {
#define RULE_NUMERIC(AST_KIND, OP_KIND)                      \
  {                                                          \
    Kind::AST_KIND, {                                        \
      {{Type::INT, Type::INT}, Op::OP_KIND##_I32},           \
          {{Type::LONG, Type::LONG}, Op::OP_KIND##_I64},     \
          {{Type::FLOAT, Type::FLOAT}, Op::OP_KIND##_F32}, { \
        {Type::DOUBLE, Type::DOUBLE}, Op::OP_KIND##_F64      \
      }                                                      \
    }                                                        \
  }

    decltype(rules) rulesToAdd = {RULE_NUMERIC(Add, ADD),
                                  RULE_NUMERIC(Subtract, SUB),
                                  RULE_NUMERIC(Multiply, MUL),
                                  RULE_NUMERIC(Divide, DIV),
                                  RULE_NUMERIC(Modulo, MOD),
                                  RULE_NUMERIC(GreaterThan, GT),
                                  RULE_NUMERIC(LessThan, LT),
                                  RULE_NUMERIC(GreaterThanOrEqual, GE),
                                  RULE_NUMERIC(LessThanOrEqual, LE),
                                  RULE_NUMERIC(Equal, EQ),
                                  RULE_NUMERIC(NotEqual, NE),
                                  {Kind::UnaryMinus,
                                   {
                                       {{Type::INT}, Op::MINUS_I32},
                                       {{Type::LONG}, Op::MINUS_I64},
                                       {{Type::FLOAT}, Op::MINUS_F32},
                                       {{Type::DOUBLE}, Op::MINUS_F64},
                                   }},
                                  {Kind::Constant,
                                   {
                                       {{Type::INT}, Op::PUSH_CONSTANT_I32},
                                       {{Type::LONG}, Op::PUSH_CONSTANT_I64},
                                       {{Type::FLOAT}, Op::PUSH_CONSTANT_F32},
                                       {{Type::DOUBLE}, Op::PUSH_CONSTANT_F64},
                                   }}};
    for (const auto& pair : rulesToAdd) {
      rules.insert({pair.first, pair.second});
    }
  }
};

#endif  // COMPILER_HPP
