#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "Enum.hpp"

using Byte = uint8_t;
using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::string;

class Instruction {
public:
  virtual OpCode Operation() const;
  virtual int Length() const;
};

class ArithmeticInstruction : public Instruction {
public:
  OpCode operation;

  ArithmeticInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class ComparisonInstruction : public Instruction {
public:
  OpCode operation;

  ComparisonInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class BitOperationInstruction : public Instruction {
public:
  OpCode operation;

  BitOperationInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class PushCommonConstantInstruction : public Instruction {
public:
  OpCode operation;

  PushCommonConstantInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class PushSmallIntegerInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t value;

  PushSmallIntegerInstruction(OpCode operation, uint16_t value)
      : operation{operation}, value{value} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class ConstantPoolInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t cpOffset;

  ConstantPoolInstruction(OpCode operation, uint16_t cpOffset)
      : operation{operation}, cpOffset{cpOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class LocalStackInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t stackOffset;

  LocalStackInstruction(OpCode operation, uint16_t stackOffset)
      : operation{operation}, stackOffset{stackOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class StaticInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t cpOffset;

  StaticInstruction(OpCode operation, uint16_t cpOffset)
      : operation{operation}, cpOffset{cpOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class FieldInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t cpOffset;

  FieldInstruction(OpCode operation, uint16_t cpOffset)
      : operation{operation}, cpOffset{cpOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class ArrayInstruction : public Instruction {
public:
  OpCode operation;

  ArrayInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class JumpInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t byteCodeOffset;

  JumpInstruction(OpCode operation, uint16_t byteCodeOffset)
      : operation{operation}, byteCodeOffset{byteCodeOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class InvokeInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t stackOffset;

  InvokeInstruction(OpCode operation, uint16_t stackOffset)
      : operation{operation}, stackOffset{stackOffset} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 3; }
};

class ReturnInstruction : public Instruction {
public:
  OpCode operation;

  ReturnInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return operation; }
  int Length() const override { return 1; }
};

class NewInstruction : public Instruction {
public:
  OpCode operation;
  uint16_t cpOffset;

  NewInstruction(OpCode operation, uint16_t cpOffset)
      : operation{operation}, cpOffset{cpOffset} {}
  OpCode Operation() const override { return OpCode::NEW; }
  int Length() const override { return 3; }
};

class NewArrayInstruction : public Instruction {
public:
  OpCode operation;
  uint8_t elementType;

  NewArrayInstruction(OpCode operation, uint8_t elementType)
      : operation{operation}, elementType{elementType} {}
  OpCode Operation() const override { return OpCode::NEW_ARRAY; }
  int Length() const override { return 2; }
};

class ArrayLengthInstruction : public Instruction {
public:
  OpCode operation;

  ArrayLengthInstruction(OpCode operation) : operation{operation} {}
  OpCode Operation() const override { return OpCode::ARRAY_LENGTH; }
  int Length() const override { return 1; }
};

class ConstantValue {
public:
  ConstantKind kind;
  string value;

  ConstantValue() {}
  ConstantValue(ConstantKind kind, string value) : kind{kind}, value{value} {}
};

class ConstantPool {
public:
  vector<ConstantValue> constantList;
  unordered_map<ConstantKind, unordered_map<string, int>> constantMap;

  int RegisterConstant(ConstantKind kind, string value);
};

class ILGenerator {
public:
  shared_ptr<ConstantPool> constantPool;
  vector<shared_ptr<Instruction>> instructions;
  int offset;

  ILGenerator(shared_ptr<ConstantPool> constantPool)
      : constantPool{constantPool}, offset{0} {}

  virtual int ILOffset() const { return offset; }
};

#endif // INSTRUCTION_HPP
