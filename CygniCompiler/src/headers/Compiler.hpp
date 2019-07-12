#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Ast.hpp"
#include "Endian.hpp"
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
  Locator& locator;

  Compiler(const HashMap<int, Ptr<Type>>& typeRecord, Locator& locator)
      : typeRecord{typeRecord}, locator{locator} {
    Register();
  }

  Vector<Byte> Compile(const Program& program) {
    Vector<Byte> code;
    CompileConstantPool(locator.constantPool, code);
    EmitU16(code, program.modules.size());
    EmitU16(code, program.classes.size());
    for (const auto& module : program.modules) {
      CompileNode(module, code);
    }
    // for (const auto& _class : program.classes) {
    //   CompileNode(_class, code);
    // }
    return code;
  }

  void CompileNode(const Ptr<Ast>& node, Vector<Byte>& code) {
    switch (node->kind) {
      case Kind::Add:
        CompileBinary(Cast<Add>(node), code);
        break;
      case Kind::Subtract:
        CompileBinary(Cast<Subtract>(node), code);
        break;
      case Kind::Multiply:
        CompileBinary(Cast<Multiply>(node), code);
        break;
      case Kind::Divide:
        CompileBinary(Cast<Divide>(node), code);
        break;
      case Kind::Modulo:
        CompileBinary(Cast<Modulo>(node), code);
        break;
      case Kind::GreaterThan:
        CompileBinary(Cast<GreaterThan>(node), code);
        break;
      case Kind::LessThan:
        CompileBinary(Cast<LessThan>(node), code);
        break;
      case Kind::GreaterThanOrEqual:
        CompileBinary(Cast<GreaterThanOrEqual>(node), code);
        break;
      case Kind::LessThanOrEqual:
        CompileBinary(Cast<LessThanOrEqual>(node), code);
        break;
      case Kind::Equal:
        CompileBinary(Cast<Equal>(node), code);
        break;
      case Kind::NotEqual:
        CompileBinary(Cast<NotEqual>(node), code);
        break;
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
        CompileIfThen(Cast<IfThen>(node), code);
        break;
      case Kind::IfElse:
        CompileIfElse(Cast<IfElse>(node), code);
        break;
      case Kind::Constant:
        CompileConstant(Cast<Constant>(node), code);
        break;
      case Kind::Block:
        CompileBlock(Cast<Block>(node), code);
        break;
      case Kind::Name:
        throw NotImplementedException();
        break;
      case Kind::Return:
        CompileReturn(Cast<Return>(node), code);
        break;
      case Kind::Var:
        throw NotImplementedException();
        break;
      case Kind::Def:
        CompileDef(Cast<Def>(node), code);
        break;
      case Kind::Assign:
        break;
      case Kind::Call:
        throw NotImplementedException();
        break;
      case Kind::While:
        break;
      case Kind::DefClass:
        // TO DO
        // CompileClass(Cast<DefClass>(node), code);
        break;
      case Kind::DefModule:
        CompileModule(Cast<DefModule>(node), code);
        break;
      default:
        throw NotImplementedException();
    }
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
      EmitU16(code, index);
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
    EmitString(code, node->name);
    // EmitU16(code, locator.locations.at(node->id).Index());
    EmitU16(code, node->fields.size());
    EmitU16(code, node->methods.size());
    for (const auto& field : node->fields) {
      CompileNode(field, code);
    }
    for (const auto& method : node->methods) {
      CompileNode(method, code);
    }
  }

  void CompileDef(const Ptr<Def>& node, Vector<Byte>& code) {
    EmitString(code, node->name);
    // EmitU16(code, locator.locations.at(node->id).Index());
    int locals = locator.functionLocals[node->id];
    EmitU16(code, locals);                  /* locals */
    EmitU16(code, 0);                       /* TO DO: stack */
    EmitU16(code, node->parameters.size()); /* args_size */
    Vector<Byte> functionCode;
    CompileNode(node->body, functionCode);
    if (functionCode.size() >= 65536) {
      throw CompilerException(node->position, "function code size too large");
    } else {
      EmitU16(code, functionCode.size());
      for (const auto& byte : functionCode) {
        code.push_back(byte);
      }
    }
  }

  void CompileClass(const Ptr<DefClass>& node, Vector<Byte>& code) {
    EmitFlag(code, OpFlag::DEFINE_CLASS);
    EmitString(code, node->name);
    EmitU16(code, locator.locations.at(node->id).Index());
    EmitU16(code, node->fields.size());
    EmitU16(code, node->methods.size());
    for (const auto& field : node->fields) {
      CompileNode(field, code);
    }
    for (const auto& method : node->methods) {
      CompileNode(method, code);
    }
  }

  void CompileIfThen(const Ptr<IfThen>& node, Vector<Byte>& code) {
    CompileNode(node->condition, code);
    EmitOp(code, Op::JUMP_IF_FALSE);
    int index = code.size();
    EmitU16(code, 0);  // hold the place (2 bytes)
    CompileNode(node->ifTrue, code);
    int indexJump = code.size();
    RewriteU16(code, indexJump, index);
  }

  void CompileIfElse(const Ptr<IfElse>& node, Vector<Byte>& code) {
    CompileNode(node->condition, code);
    EmitOp(code, Op::JUMP_IF_FALSE);
    int index1 = code.size();
    EmitU16(code, 0);  // hold the place (2 bytes)
    CompileNode(node->ifTrue, code);
    EmitOp(code, Op::JUMP);
    int index2 = code.size();
    EmitU16(code, 0);  // hold the place (2 bytes)
    int index1Jump = code.size();
    RewriteU16(code, index1Jump, index1);
    CompileNode(node->ifFalse, code);
    int index2Jump = code.size();
    RewriteU16(code, index2Jump, index2);
  }

  void CompileReturn(const Ptr<Return>& node, Vector<Byte>& code) {
    CompileNode(node->value, code);
    Op op = Match(node, {TypeOf(node->value)});
    EmitOp(code, op);
  }

  void CompileName(const Ptr<Name>& node, Vector<Byte>& code) {
    const Location& location = locator.locations[node->id];
    if (location.kind == LocationKind::Function) {
      auto type = TypeOf(node);
      if (type->GetTypeCode() == TypeCode::INT) {
        EmitOp(code, Op::PUSH_LOCAL_I32);
      } else if (type->GetTypeCode() == TypeCode::LONG) {
        EmitOp(code, Op::PUSH_LOCAL_I64);
      } else if (type->GetTypeCode() == TypeCode::FLOAT) {
        EmitOp(code, Op::PUSH_LOCAL_F32);
      } else if (type->GetTypeCode() == TypeCode::DOUBLE) {
        EmitOp(code, Op::PUSH_LOCAL_F64);
      } else {
        throw NotSupportedException();
      }
      EmitU16(code, location.Index());
    } else if (location.kind == LocationKind::Module) {
      auto type = TypeOf(node);
      if (type.GetTypeCode() == TypeCode::FUNCTION) {
        EmitOp(code, Op::PUSH_FUNCTION);
      } else {
        throw NotSupportedException();
      }
    } else {
      throw NotImplementedException();
    }
  }
  void EmitU16(Vector<Byte>& code, int number) {
    Byte* bytes = Endian::UInt16ToBytes(static_cast<uint16_t>(number));
    code.push_back(bytes[0]);
    code.push_back(bytes[1]);
  }

  void RewriteU16(Vector<Byte>& code, int number, int index) {
    Byte* bytes = Endian::UInt16ToBytes(static_cast<uint16_t>(number));
    code.at(index) = bytes[0];
    code.at(index + 1) = bytes[1];
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
                                   }},
                                  {Kind::Return,
                                   {
                                       {{Type::INT}, Op::RETURN_I32},
                                       {{Type::LONG}, Op::RETURN_I64},
                                       {{Type::FLOAT}, Op::RETURN_F32},
                                       {{Type::DOUBLE}, Op::RETURN_F64},
                                   }}};
    for (const auto& pair : rulesToAdd) {
      rules.insert({pair.first, pair.second});
    }
  }
};

#endif  // COMPILER_HPP
