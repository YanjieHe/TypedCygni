#ifndef CYGNI_COMPILER_HPP
#define CYGNI_COMPILER_HPP
#include "Exception.hpp"
#include "Expression.hpp"
#include "Visitor.hpp"
#include <cstdint>
#include <functional>
#include <numeric>
#include <variant>
#include <vector>

namespace cygni {
using Byte = uint8_t;
using u16 = uint16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float_t;
using f64 = double_t;

class ByteCode {
public:
  std::vector<Byte> bytes;
  ByteCode() = default;
  void Append(Byte byte);
  void AppendOp(OpCode op);
  void AppendTypeTag(TypeTag tag);
  void
  AppendU16Checked(size_t value,
                   const std::function<CompilerException()> &exceptionHandler);
  void AppendU16Unchecked(size_t value);
  void WriteUShort(int index, int value);
  void AppendUInt(uint32_t value);
  void AppendTypeCode(TypeCode typeCode);
  void AppendString(const std::u32string &u32str);
  void AppendByteCode(const ByteCode &other);
  void AppendConstantKind(ConstantKind kind);
  int Size() const;
};

class Instruction {
public:
  OpCode op;
  std::vector<Byte> operands;
};

class CompilerRule {
public:
  std::vector<TypeCode> typeCodeList;
  OpCode op;
};

class CompilerRuleSet {
public:
  std::unordered_map<ExpressionType, std::vector<CompilerRule>> table;

  void AddRule(ExpressionType nodeType, std::vector<TypeCode> typeCodeList,
               OpCode op);

  std::optional<OpCode> Match(ExpressionType nodeType,
                              std::vector<TypeCode> typeCodeList);
};

class NativeMethod {
public:
  std::u32string libName;
  std::u32string entryPoint;

  NativeMethod() = default;
  NativeMethod(std::u32string libName, std::u32string entryPoint);
};

class ExecClass;

class ExecMethod {
public:
  FullQualifiedName className;
  MethodFlag flag;
  FullQualifiedName name;
  int argsSize;
  int localsSize;
  int needStackSize;
  ByteCode code;
  NativeMethod nativeMethod;
  std::shared_ptr<MethodInfo> methodInfo;
  ExecMethod() = default;
  // ExecMethod(FullQualifiedName className, MethodFlag flag,
  //	FullQualifiedName name, int argsSize, int localsSize, int
  // needStackSize);
};

class ExecField {
public:
  FullQualifiedName className;
  FullQualifiedName name;
  std::shared_ptr<FieldInfo> fieldInfo;
};
class ExecClass {
public:
  FullQualifiedName name;
  std::vector<ExecMethod> methods;
  std::vector<ExecField> fields;

  std::vector<ExecMethod> staticFunctions;
  std::vector<ExecField> staticVariables;
  std::vector<FullQualifiedName> inheritanceChain;
  std::vector<FullQualifiedName> interfaces;
  VirtualTable virtualTable;

  ConstantMap constantMap;

  ExecClass() = default;
  // ExecClass(FullQualifiedName name,
  //	std::vector<MethodInfo> methods,
  //	std::vector<FieldInfo> fields,
  //	std::vector<FieldInfo> staticFunctions,
  //	std::vector<FieldInfo> staticVariables,
  //	std::vector<std::u32string> inheritanceChain,
  //	std::vector<FullQualifiedName> superClasses,
  //	std::vector<FullQualifiedName> interfaces,
  //	VirtualTable virtualTable,
  //	ConstantMap constantMap);
};

class GlobalInformation {
public:
  int classesCount;
  FullQualifiedName mainFunction;
};

class Executable {
public:
  GlobalInformation globalInformation;
  std::vector<std::shared_ptr<ExecClass>> classes;
};

class Compiler : public Visitor<void, const ConstantMap &, ByteCode &> {
public:
  Project &project;
  CompilerRuleSet rules;
  Compiler(Project &project);
  Executable Compile();

  void VisitUnary(std::shared_ptr<UnaryExpression> node,
                  const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitBinary(std::shared_ptr<BinaryExpression> node,
                   const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitBlock(std::shared_ptr<BlockExpression> node,
                  const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitConstant(std::shared_ptr<ConstantExpression> node,
                     const ConstantMap &constantMap, ByteCode &byteCode);
  // void CompileFieldDef(const FieldDef &field, ByteCode& byteCode);
  void VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                      const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitReturn(std::shared_ptr<ReturnExpression> node,
                   const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitConditional(std::shared_ptr<ConditionalExpression> node,
                        const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitDefault(std::shared_ptr<DefaultExpression> node,
                    const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitInvocation(std::shared_ptr<InvocationExpression> node,
                       const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                         const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitNewExpression(std::shared_ptr<NewExpression> node,
                          const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                             const ConstantMap &constantMap,
                             ByteCode &byteCode);
  void VisitAssign(std::shared_ptr<BinaryExpression> node,
                   const ConstantMap &constantMap, ByteCode &byteCode);
  void VisitWhile(std::shared_ptr<WhileExpression> node,
                  const ConstantMap &constantMap, ByteCode &byteCode);

  void CompileMethodDef(ExecMethod &execMethod, const ConstantMap &constantMap);
  FullQualifiedName CompileMainFunction(Project &project);
  void CompileConstantPool(SourcePosition position,
                           const ConstantMap &constantMap, ByteCode &byteCode);
  GlobalInformation CompileGlobalInformation(Project &project, Executable &exe);
  void ConvertExp(std::shared_ptr<UnaryExpression> node,
                  const ConstantMap &constantMap, ByteCode &byteCode);

  std::optional<int> GetConstant(const ConstantMap &constantMap,
                                 ConstantKind kind, std::u32string text);
  ConstantMap MergeConstantPool(const ConstantMap &map1,
                                const ConstantMap &map2);
};

void ViewExe(Executable &exe);
ByteCode CompileExe(Executable &exe);
std::vector<std::tuple<ConstantKind, std::u32string>>
GetConstantList(const ConstantMap &constantMap);

} // namespace cygni

#endif // CYGNI_COMPILER_HPP