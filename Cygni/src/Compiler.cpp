#include "Compiler.hpp"
#include <algorithm>

using std::cout;
using std::endl;
using namespace std;

namespace cygni {
void ByteCode::Append(Byte byte) { bytes.push_back(byte); }
void ByteCode::AppendOp(OpCode op) { bytes.push_back(static_cast<Byte>(op)); }
void ByteCode::AppendTypeTag(TypeTag tag) {
  bytes.push_back(static_cast<Byte>(tag));
}
void ByteCode::AppendU16Checked(
    size_t value, const std::function<CompilerException()> &exceptionHandler) {
  if (value > std::numeric_limits<uint16_t>::max()) {
    // throw std::invalid_argument("unsigned 16-bit integer overflow");
    throw exceptionHandler();
  } else {
    bytes.push_back(static_cast<Byte>(value / 256));
    bytes.push_back(static_cast<Byte>(value % 256));
  }
}
void ByteCode::AppendU16Unchecked(size_t value) {
  bytes.push_back(static_cast<Byte>(value / 256));
  bytes.push_back(static_cast<Byte>(value % 256));
}
void ByteCode::WriteUShort(int index, int value) {
  bytes.at(index) = (value / 256);
  bytes.at(index + 1) = (value % 256);
}
void ByteCode::AppendUInt(uint32_t value) {
  for (int i = 0; i < 4; i++) {
    bytes.push_back(value % 256);
    value = value / 256;
  }
}
void ByteCode::AppendTypeCode(TypeCode typeCode) {
  switch (typeCode) {
  case TypeCode::Char:
  case TypeCode::Boolean:
  case TypeCode::Int32: {
    AppendTypeTag(TypeTag::TYPE_I32);
    break;
  }
  case TypeCode::Int64: {
    AppendTypeTag(TypeTag::TYPE_I64);
    break;
  }
  case TypeCode::Float32: {
    AppendTypeTag(TypeTag::TYPE_F32);
    break;
  }
  case TypeCode::Float64: {
    AppendTypeTag(TypeTag::TYPE_F64);
    break;
  }
  case TypeCode::String: {
    AppendTypeTag(TypeTag::TYPE_STRING);
    break;
  }
  case TypeCode::Class: {
    AppendTypeTag(TypeTag::TYPE_OBJECT);
    break;
  }
  default:
    throw NotImplementedException(Format(U"not supported type code: {}",
                                         Enum<TypeCode>::ToString(typeCode)));
  }
}
void ByteCode::AppendString(const std::u32string &u32str) {
  std::string u8str = UTF32ToUTF8(u32str);
  AppendU16Unchecked(u8str.size());
  for (Byte c : u8str) {
    Append(c);
  }
}
void ByteCode::AppendByteCode(const ByteCode &other) {
  bytes.insert(bytes.end(), other.bytes.begin(), other.bytes.end());
}
void ByteCode::AppendConstantKind(ConstantKind kind) {
  bytes.push_back(static_cast<Byte>(kind));
}
int ByteCode::Size() const { return static_cast<int>(bytes.size()); }
Compiler::Compiler(Project &project) : project{project} {
  rules.AddRule(ExpressionType::UnaryMinus, {TypeCode::Int32},
                OpCode::MINUS_I32);
  rules.AddRule(ExpressionType::UnaryMinus, {TypeCode::Int64},
                OpCode::MINUS_I64);
  rules.AddRule(ExpressionType::UnaryMinus, {TypeCode::Float32},
                OpCode::MINUS_F32);
  rules.AddRule(ExpressionType::UnaryMinus, {TypeCode::Float64},
                OpCode::MINUS_F64);

  rules.AddRule(ExpressionType::Add, {TypeCode::Int32, TypeCode::Int32},
                OpCode::ADD_I32);
  rules.AddRule(ExpressionType::Add, {TypeCode::Int64, TypeCode::Int64},
                OpCode::ADD_I64);
  rules.AddRule(ExpressionType::Add, {TypeCode::Float32, TypeCode::Float32},
                OpCode::ADD_F32);
  rules.AddRule(ExpressionType::Add, {TypeCode::Float64, TypeCode::Float64},
                OpCode::ADD_F64);

  rules.AddRule(ExpressionType::Subtract, {TypeCode::Int32, TypeCode::Int32},
                OpCode::SUB_I32);
  rules.AddRule(ExpressionType::Subtract, {TypeCode::Int64, TypeCode::Int64},
                OpCode::SUB_I64);
  rules.AddRule(ExpressionType::Subtract,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::SUB_F32);
  rules.AddRule(ExpressionType::Subtract,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::SUB_F64);

  rules.AddRule(ExpressionType::Multiply, {TypeCode::Int32, TypeCode::Int32},
                OpCode::MUL_I32);
  rules.AddRule(ExpressionType::Multiply, {TypeCode::Int64, TypeCode::Int64},
                OpCode::MUL_I64);
  rules.AddRule(ExpressionType::Multiply,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::MUL_F32);
  rules.AddRule(ExpressionType::Multiply,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::MUL_F64);

  rules.AddRule(ExpressionType::Divide, {TypeCode::Int32, TypeCode::Int32},
                OpCode::DIV_I32);
  rules.AddRule(ExpressionType::Divide, {TypeCode::Int64, TypeCode::Int64},
                OpCode::DIV_I64);
  rules.AddRule(ExpressionType::Divide, {TypeCode::Float32, TypeCode::Float32},
                OpCode::DIV_F32);
  rules.AddRule(ExpressionType::Divide, {TypeCode::Float64, TypeCode::Float64},
                OpCode::DIV_F64);

  rules.AddRule(ExpressionType::GreaterThan, {TypeCode::Int32, TypeCode::Int32},
                OpCode::GT_I32);
  rules.AddRule(ExpressionType::GreaterThan, {TypeCode::Char, TypeCode::Char},
                OpCode::GT_I32);
  rules.AddRule(ExpressionType::GreaterThan, {TypeCode::Int64, TypeCode::Int64},
                OpCode::GT_I64);
  rules.AddRule(ExpressionType::GreaterThan,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::GT_F32);
  rules.AddRule(ExpressionType::GreaterThan,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::GT_F64);

  rules.AddRule(ExpressionType::LessThan, {TypeCode::Int32, TypeCode::Int32},
                OpCode::LT_I32);
  rules.AddRule(ExpressionType::LessThan, {TypeCode::Char, TypeCode::Char},
                OpCode::LT_I32);
  rules.AddRule(ExpressionType::LessThan, {TypeCode::Int64, TypeCode::Int64},
                OpCode::LT_I64);
  rules.AddRule(ExpressionType::LessThan,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::LT_F32);
  rules.AddRule(ExpressionType::LessThan,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::LT_F64);

  rules.AddRule(ExpressionType::GreaterThanOrEqual,
                {TypeCode::Int32, TypeCode::Int32}, OpCode::GE_I32);
  rules.AddRule(ExpressionType::GreaterThanOrEqual,
                {TypeCode::Char, TypeCode::Char}, OpCode::GE_I32);
  rules.AddRule(ExpressionType::GreaterThanOrEqual,
                {TypeCode::Int64, TypeCode::Int64}, OpCode::GE_I64);
  rules.AddRule(ExpressionType::GreaterThanOrEqual,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::GE_F32);
  rules.AddRule(ExpressionType::GreaterThanOrEqual,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::GE_F64);

  rules.AddRule(ExpressionType::LessThanOrEqual,
                {TypeCode::Int32, TypeCode::Int32}, OpCode::LE_I32);
  rules.AddRule(ExpressionType::LessThanOrEqual,
                {TypeCode::Char, TypeCode::Char}, OpCode::LE_I32);
  rules.AddRule(ExpressionType::LessThanOrEqual,
                {TypeCode::Int64, TypeCode::Int64}, OpCode::LE_I64);
  rules.AddRule(ExpressionType::LessThanOrEqual,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::LE_F32);
  rules.AddRule(ExpressionType::LessThanOrEqual,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::LE_F64);

  rules.AddRule(ExpressionType::Equal, {TypeCode::Int32, TypeCode::Int32},
                OpCode::EQ_I32);
  rules.AddRule(ExpressionType::Equal, {TypeCode::Char, TypeCode::Char},
                OpCode::EQ_I32);
  rules.AddRule(ExpressionType::Equal, {TypeCode::Boolean, TypeCode::Boolean},
                OpCode::EQ_I32);
  rules.AddRule(ExpressionType::Equal, {TypeCode::Int64, TypeCode::Int64},
                OpCode::EQ_I64);
  rules.AddRule(ExpressionType::Equal, {TypeCode::Float32, TypeCode::Float32},
                OpCode::EQ_F32);
  rules.AddRule(ExpressionType::Equal, {TypeCode::Float64, TypeCode::Float64},
                OpCode::EQ_F64);

  rules.AddRule(ExpressionType::NotEqual, {TypeCode::Int32, TypeCode::Int32},
                OpCode::NE_I32);
  rules.AddRule(ExpressionType::NotEqual, {TypeCode::Char, TypeCode::Char},
                OpCode::NE_I32);
  rules.AddRule(ExpressionType::NotEqual,
                {TypeCode::Boolean, TypeCode::Boolean}, OpCode::NE_I32);
  rules.AddRule(ExpressionType::NotEqual, {TypeCode::Int64, TypeCode::Int64},
                OpCode::NE_I64);
  rules.AddRule(ExpressionType::NotEqual,
                {TypeCode::Float32, TypeCode::Float32}, OpCode::NE_F32);
  rules.AddRule(ExpressionType::NotEqual,
                {TypeCode::Float64, TypeCode::Float64}, OpCode::NE_F64);

  rules.AddRule(ExpressionType::Convert, {TypeCode::Int32, TypeCode::Int64},
                OpCode::CAST_I32_TO_I64);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Int32, TypeCode::Float32},
                OpCode::CAST_I32_TO_F32);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Int32, TypeCode::Float64},
                OpCode::CAST_I32_TO_F64);

  rules.AddRule(ExpressionType::Convert, {TypeCode::Int64, TypeCode::Int32},
                OpCode::CAST_I64_TO_I32);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Int64, TypeCode::Float32},
                OpCode::CAST_I64_TO_F32);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Int64, TypeCode::Float64},
                OpCode::CAST_I64_TO_F64);

  rules.AddRule(ExpressionType::Convert, {TypeCode::Float32, TypeCode::Int32},
                OpCode::CAST_F32_TO_I32);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Float32, TypeCode::Int64},
                OpCode::CAST_F32_TO_I64);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Float32, TypeCode::Float64},
                OpCode::CAST_F32_TO_F64);

  rules.AddRule(ExpressionType::Convert, {TypeCode::Float64, TypeCode::Int32},
                OpCode::CAST_F64_TO_I32);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Float64, TypeCode::Int64},
                OpCode::CAST_F64_TO_I64);
  rules.AddRule(ExpressionType::Convert, {TypeCode::Float64, TypeCode::Float32},
                OpCode::CAST_F64_TO_F32);

  rules.AddRule(ExpressionType::Return, {TypeCode::Void}, OpCode::RETURN_I32);
  rules.AddRule(ExpressionType::Return, {TypeCode::Int32}, OpCode::RETURN_I32);
  rules.AddRule(ExpressionType::Return, {TypeCode::Char}, OpCode::RETURN_I32);
  rules.AddRule(ExpressionType::Return, {TypeCode::Boolean},
                OpCode::RETURN_I32);
  rules.AddRule(ExpressionType::Return, {TypeCode::Int64}, OpCode::RETURN_I64);
  rules.AddRule(ExpressionType::Return, {TypeCode::Float32},
                OpCode::RETURN_F32);
  rules.AddRule(ExpressionType::Return, {TypeCode::Float64},
                OpCode::RETURN_F64);
  rules.AddRule(ExpressionType::Return, {TypeCode::String},
                OpCode::RETURN_OBJECT);
  rules.AddRule(ExpressionType::Return, {TypeCode::Class},
                OpCode::RETURN_OBJECT);
  rules.AddRule(ExpressionType::Return, {TypeCode::Array},
                OpCode::RETURN_OBJECT);

  rules.AddRule(ExpressionType::Constant, {TypeCode::Int32}, OpCode::PUSH_I32);
  rules.AddRule(ExpressionType::Constant, {TypeCode::Int64}, OpCode::PUSH_I64);
  rules.AddRule(ExpressionType::Constant, {TypeCode::Float32},
                OpCode::PUSH_F32);
  rules.AddRule(ExpressionType::Constant, {TypeCode::Float64},
                OpCode::PUSH_F64);
}
Executable Compiler::Compile() {
  Executable exe;
  std::unordered_map<FullQualifiedName, std::shared_ptr<ExecClass>> classes;

  for (auto [_, pkg] : project.packages) {
    for (auto [_, classInfo] : pkg->classes) {
      auto className =
          FullQualifiedName(classInfo->route).Concat(classInfo->name);
      std::shared_ptr<ExecClass> execClass = std::make_shared<ExecClass>();
      execClass->name = className;
      for (auto field : classInfo->fields.values) {
        ExecField execField;
        execField.className = className;
        execField.name = className.Concat(field->name);
        execField.fieldInfo = field;
        execClass->fields.push_back(execField);
      }
      for (auto method : classInfo->methodDefs.values) {
        ExecMethod execMethod;
        execMethod.flag = MethodFlag::InstanceMethod;
        execMethod.className = className;
        execMethod.methodInfo = method;
        execClass->methods.push_back(execMethod);
      }
      for (auto superClass : classInfo->inheritanceChain) {
        auto name =
            FullQualifiedName(superClass->route).Concat(superClass->name);
        execClass->inheritanceChain.push_back(name);
      }
      for (auto interfaceInfo : classInfo->interfaceList) {
        auto name =
            FullQualifiedName(interfaceInfo->route).Concat(interfaceInfo->name);
        execClass->interfaces.push_back(name);
      }
      execClass->virtualTable = classInfo->virtualTable;
      execClass->constantMap = classInfo->constantMap;
      classes.insert({className, execClass});
    }
    for (auto [_, interfaceInfo] : pkg->interfaces) {
      auto className =
          FullQualifiedName(interfaceInfo->route).Concat(interfaceInfo->name);
      if (classes.find(className) != classes.end()) {
        throw CompilerException(
            interfaceInfo->position,
            Format(U"redefined interface {}", interfaceInfo->name));
      } else {
        std::shared_ptr<ExecClass> execClass = std::make_shared<ExecClass>();
        classes.insert({className, execClass});
        execClass->name = className;
        for (auto method : interfaceInfo->methodDefs.values) {
          ExecMethod execMethod;
          execMethod.flag = MethodFlag::InstanceMethod;
          execMethod.className = className;
          execMethod.methodInfo = method;
          execClass->methods.push_back(execMethod);
        }
        for (auto superType : interfaceInfo->superInterfaces) {
          auto superInterfaceType =
              std::static_pointer_cast<InterfaceType>(superType);
          auto name = FullQualifiedName(superInterfaceType->route)
                          .Concat(superInterfaceType->name);
          execClass->interfaces.push_back(name);
        }
      }
    }
    for (auto [_, moduleInfo] : pkg->modules) {
      auto className =
          FullQualifiedName(moduleInfo->route).Concat(moduleInfo->name);
      std::shared_ptr<ExecClass> execClass;
      if (classes.find(className) != classes.end()) {
        execClass = classes[className];
        execClass->constantMap =
            MergeConstantPool(execClass->constantMap, moduleInfo->constantMap);
      } else {
        execClass = std::make_shared<ExecClass>();
        execClass->constantMap = moduleInfo->constantMap;
        classes.insert({className, execClass});
      }
      execClass->name = className;
      for (auto staticVar : moduleInfo->fields.values) {
        ExecField execVar;
        execVar.className = className;
        execVar.name = className.Concat(staticVar->name);
        execVar.fieldInfo = staticVar;
        execClass->staticVariables.push_back(execVar);
      }
      for (const auto &method : moduleInfo->methods.values) {
        ExecMethod execMethod;
        if (method->annotations.ContainsKey(U"LibraryImport")) {
          execMethod.flag = MethodFlag::NativeFunction;
        } else {
          execMethod.flag = MethodFlag::ModuleFunction;
        }
        execMethod.className = className;
        execMethod.methodInfo = method;
        execClass->staticFunctions.push_back(execMethod);
      }
    }
  }

  for (auto classPair : classes) {
    exe.classes.push_back(classPair.second);
  }

  for (auto execClass : exe.classes) {
    for (auto &method : execClass->methods) {
      CompileMethodDef(method, execClass->constantMap);
    }
    for (auto &function : execClass->staticFunctions) {
      CompileMethodDef(function, execClass->constantMap);
    }
  }
  exe.globalInformation = CompileGlobalInformation(project, exe);
  return exe;
}
void Compiler::VisitUnary(std::shared_ptr<UnaryExpression> node,
                          const ConstantMap &constantMap, ByteCode &byteCode) {
  VisitExpression(node->operand, constantMap, byteCode);
  switch (node->nodeType) {
  case ExpressionType::UnaryPlus: {
    switch (node->operand->type->typeCode) {
    case TypeCode::Int32:
    case TypeCode::Int64:
    case TypeCode::Float32:
    case TypeCode::Float64: {
      break;
    }
    default: {
      throw TypeException(node->position,
                          Format(U"unary plus operand type '{}' not suported",
                                 node->operand->type->ToString()));
    }
    }
    break;
  }
  case ExpressionType::UnaryMinus: {
    if (auto op = rules.Match(ExpressionType::UnaryMinus,
                              {node->operand->type->typeCode})) {
      byteCode.AppendOp(op.value());
    } else {
      throw TypeException(node->position,
                          Format(U"unary minus operand type '{}' not suported",
                                 node->operand->type->ToString()));
    }
    break;
  }
  case ExpressionType::Convert: {
    auto from = node->operand->type->typeCode;
    auto to = node->type->typeCode;

    if (auto op = rules.Match(ExpressionType::Convert, {from, to})) {
      byteCode.AppendOp(op.value());
    } else {
      throw CompilerException(
          node->position,
          Format(U"not supported conversion from '{}' to '{}",
                 node->operand->type->ToString(), node->type->ToString()));
    }
    break;
  }
  // case ExpressionType::UpCast: {
  //	byteCode.AppendOp(OpCode::UP_CAST);
  //	ConvertExp(node, constantMap, byteCode);
  //	break;
  //}
  case ExpressionType::DownCast: {
    byteCode.AppendOp(OpCode::DOWN_CAST);
    ConvertExp(node, constantMap, byteCode);
    break;
  }
  case ExpressionType::ArrayLength: {
    byteCode.AppendOp(OpCode::ARRAY_LENGTH);
    break;
  }
  default: { throw NotImplementedException(); }
  }
}
void Compiler::VisitBinary(std::shared_ptr<BinaryExpression> node,
                           const ConstantMap &constantMap, ByteCode &byteCode) {
  VisitExpression(node->left, constantMap, byteCode);
  VisitExpression(node->right, constantMap, byteCode);
  TypeCode lt = node->left->type->typeCode;
  TypeCode rt = node->right->type->typeCode;
  if (auto op = rules.Match(node->nodeType, {lt, rt})) {
    byteCode.AppendOp(op.value());
  } else {
    throw CompilerException(
        node->position,
        Format(U"not supported type for '{}' operation. left: '{}', right: '{}",
               Enum<ExpressionType>::ToString(node->nodeType),
               node->left->type->ToString(), node->right->type->ToString()));
  }
}
void Compiler::VisitBlock(std::shared_ptr<BlockExpression> node,
                          const ConstantMap &constantMap, ByteCode &byteCode) {
  for (auto exp : node->expressions) {
    VisitExpression(exp, constantMap, byteCode);
  }
}
void Compiler::VisitConstant(std::shared_ptr<ConstantExpression> node,
                             const ConstantMap &constantMap,
                             ByteCode &byteCode) {
  if (node->type->typeCode == TypeCode::Boolean) {
    if (node->constant == U"true") {
      byteCode.AppendOp(OpCode::PUSH_I32_1);
    } else if (node->constant == U"false") {
      byteCode.AppendOp(OpCode::PUSH_I32_0);
    } else {
      throw CompilerException(
          node->position,
          U"illegal format of boolean value. should be 'true' or 'false'");
    }
  } else if (node->type->typeCode == TypeCode::String) {
    if (auto strIndex = GetConstant(
            constantMap, ConstantKind::CONSTANT_FLAG_STRING, node->constant)) {
      if (auto strNewIndex =
              GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_METHOD,
                          U"Predef.String.New")) {
        byteCode.AppendOp(OpCode::PUSH_STRING);
        byteCode.AppendU16Unchecked(strIndex.value());
        byteCode.AppendOp(OpCode::PUSH_FUNCTION);
        byteCode.AppendU16Unchecked(strNewIndex.value());
        byteCode.AppendOp(OpCode::INVOKE);
      } else {
        throw CompilerException(node->position,
                                U"missing 'Predef.String.New' function");
      }
    } else {
      throw CompilerException(node->position,
                              U"the given constant is not in the constant map");
    }
  } else if (node->type->typeCode == TypeCode::Void) {
    byteCode.AppendOp(OpCode::PUSH_I32_0);
  } else {
    ConstantKind kind;
    if (node->type->typeCode == TypeCode::Int32) {
      kind = ConstantKind::CONSTANT_FLAG_I32;
    } else if (node->type->typeCode == TypeCode::Int64) {
      kind = ConstantKind::CONSTANT_FLAG_I64;
    } else if (node->type->typeCode == TypeCode::Float32) {
      kind = ConstantKind::CONSTANT_FLAG_F32;
    } else if (node->type->typeCode == TypeCode::Float64) {
      kind = ConstantKind::CONSTANT_FLAG_F64;
    } else if (node->type->typeCode == TypeCode::Boolean) {
      kind = ConstantKind::CONSTANT_FLAG_BOOLEAN;
    } else if (node->type->typeCode == TypeCode::Char) {
      kind = ConstantKind::CONSTANT_FLAG_CHAR;
    } else {
      throw CompilerException(
          node->position,
          Format(U"constant type '{}' not supported", node->type->ToString()));
    }
    if (auto index = GetConstant(constantMap, kind, node->constant)) {
      if (auto op =
              rules.Match(ExpressionType::Constant, {node->type->typeCode})) {
        byteCode.AppendOp(op.value());
      } else {
        throw CompilerException(node->position,
                                Format(U"constant type '{}' not supported",
                                       node->type->ToString()));
      }
      byteCode.AppendU16Unchecked(index.value());
    } else {
      throw CompilerException(node->position,
                              U"the given constant is not in the constant map");
    }
  }
}
// void Compiler::CompileClassInfo(std::shared_ptr<ClassInfo> info, ByteCode &
// byteCode)
//{
//	byteCode.AppendString(info->name);
//	byteCode.AppendU16Checked(info->fields.Size(),
//[info]()->CompilerException
//	{
//		return CompilerException(info->position, U"too many fields in
// the class");
//	});
//	for (const auto& field : info->fields.values)
//	{
//		byteCode.AppendString(field->name);
//	}

//	// constant pool
//	CompileConstantPool(info->position, info->constantMap, byteCode);
//	byteCode.AppendU16Unchecked(info->inheritanceChain.size());
//	for (const auto& superClassType : info->inheritanceChain)
//	{
//		if (auto superClassInfo = project.GetClass(superClassType))
//		{
//			auto name =
// FullQualifiedName(superClassInfo.value()->route)
//				.Concat(superClassInfo.value()->name);
//			byteCode.AppendString(name.ToString());
//		}
//		else
//		{
//			throw CompilerException(info->position,
//				Format(U"missing super class '{}' for
// inheritance", superClassType->ToString()));
//		}
//	}
//	byteCode.AppendU16Unchecked(info->virtualTable.size());
//	for (auto methodList : info->virtualTable)
//	{
//		byteCode.AppendString(methodList.className.ToString());
//		byteCode.AppendU16Unchecked(methodList.methodNames.size());
//		for (auto methodName : methodList.methodNames)
//		{
//			byteCode.AppendString(methodName.ToString());
//		}
//	}
//	if (info->methodDefs.Size() > std::numeric_limits<uint16_t>::max())
//	{
//		throw  CompilerException(info->position, U"too many methods in
// the class");
//	}
//	byteCode.AppendU16Unchecked(info->methodDefs.Size());
//	for (const auto& method : info->methodDefs.values)
//	{
//		CompileMethodDef(method, info->constantMap);
//	}
//}
// void Compiler::CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode&
// byteCode)
//{
//	byteCode.AppendString(info->name);
//	if (info->fields.Size() > std::numeric_limits<uint16_t>::max())
//	{
//		throw CompilerException(info->position, U"too many member
// variables in the module");
//	}
//	if (info->methods.Size() > std::numeric_limits<uint16_t>::max())
//	{
//		throw CompilerException(info->position, U"too many member
// functions in the module");
//	}
//	CompileConstantPool(info->position, info->constantMap, byteCode);

//	byteCode.AppendU16Unchecked(info->fields.Size());
//	for (const auto& field : info->fields.values)
//	{
//		byteCode.AppendString(field->name);
//	}

//	byteCode.AppendU16Unchecked(info->methods.Size());
//	for (const auto& method : info->methods.values)
//	{
//		CompileMethodDef(method, info->constantMap);
//	}
//}
void Compiler::CompileMethodDef(ExecMethod &execMethod,
                                const ConstantMap &constantMap) {
  auto &method = execMethod.methodInfo;
  if (method->selfType->typeCode == TypeCode::Class ||
      method->selfType->typeCode == TypeCode::Interface) {
    if (method->parameters.size() + 1 > std::numeric_limits<uint16_t>::max()) {
      throw CompilerException(method->position, U"too many parameters");
    }
    execMethod.argsSize =
        static_cast<int>(method->parameters.size() + 1); // add 'this'
    auto classType = std::static_pointer_cast<ClassType>(method->selfType);
    execMethod.className =
        FullQualifiedName(classType->route).Concat(classType->name);
    execMethod.name = FullQualifiedName(classType->route)
                          .Concat(classType->name)
                          .Concat(method->name);
    execMethod.flag = MethodFlag::InstanceMethod;
  } else {
    if (method->parameters.size() > std::numeric_limits<uint16_t>::max()) {
      throw CompilerException(method->position, U"too many parameters");
    }
    execMethod.argsSize = static_cast<int>(method->parameters.size());
    auto moduleType = std::static_pointer_cast<ModuleType>(method->selfType);
    FullQualifiedName name = moduleType->route;
    execMethod.className =
        FullQualifiedName(moduleType->route).Concat(moduleType->name);
    execMethod.name = FullQualifiedName(moduleType->route)
                          .Concat(moduleType->name)
                          .Concat(method->name);
    execMethod.flag = MethodFlag::ModuleFunction;
  }

  if (method->annotations.ContainsKey(U"LibraryImport")) {
    auto annotation = method->annotations.GetValueByKey(U"LibraryImport");
    auto libName = std::static_pointer_cast<ConstantExpression>(
                       annotation.arguments.at(0).value)
                       ->constant;
    auto entryPoint = std::static_pointer_cast<ConstantExpression>(
                          annotation.arguments.at(1).value)
                          ->constant;
    execMethod.flag = MethodFlag::NativeFunction;
    execMethod.nativeMethod.libName = libName;
    execMethod.nativeMethod.entryPoint = entryPoint;

    if (method->parameters.size() > std::numeric_limits<uint16_t>::max()) {
      throw CompilerException(method->position, U"too many parameters");
    }
  } else {
    if (method->selfType->typeCode == TypeCode::Interface) {
      // Do nothing
      execMethod.localsSize = 0;
    } else {
      if (method->localVariables.size() >
          std::numeric_limits<uint16_t>::max()) {
        throw CompilerException(method->position, U"too many variables");
      }
      execMethod.localsSize = static_cast<int>(method->localVariables.size());
      ByteCode funcCode;
      VisitExpression(method->body, constantMap, funcCode);
      if (funcCode.Size() > std::numeric_limits<uint16_t>::max()) {
        throw CompilerException(method->position,
                                U"the function is too complicated. Consider "
                                U"split it into smaller functions");
      }
      execMethod.code = funcCode;
    }
  }
}
void Compiler::VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                              const ConstantMap &constantMap,
                              ByteCode &byteCode) {
  auto location = parameter->location;
  switch (location->type) {
  case LocationType::LocalVariable: {
    switch (parameter->type->typeCode) {
    case TypeCode::Boolean:
    case TypeCode::Char:
    case TypeCode::Int32: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_I32);
      break;
    }
    case TypeCode::Int64: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_I64);
      break;
    }
    case TypeCode::Float32: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_F32);
      break;
    }
    case TypeCode::Float64: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_F64);
      break;
    }
    case TypeCode::String: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
      break;
    }
    case TypeCode::Array:
    case TypeCode::Class:
    case TypeCode::Interface: {
      byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
      break;
    }
    default: {
      throw CompilerException(parameter->position,
                              Format(U"not supported local variable type '{}'",
                                     parameter->type->ToString()));
    }
    }
    byteCode.AppendU16Unchecked(
        std::static_pointer_cast<ParameterLocation>(location)->offset);
    break;
  }
  case LocationType::ModuleMethod: {
    byteCode.AppendOp(OpCode::PUSH_FUNCTION);
    auto loc = std::static_pointer_cast<MemberLocation>(parameter->location);
    if (auto index = GetConstant(constantMap,
                                 ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION,
                                 loc->name.ToString())) {
      byteCode.AppendU16Unchecked(index.value());
    } else {
      throw CompilerException(
          parameter->position,
          Format(U"missing module method '{}'", loc->name.ToString()));
    }
    break;
  }
  case LocationType::ModuleName: {
    std::cout << "module name: " << parameter->name << std::endl;
    break;
  }
  default:
    throw NotImplementedException(
        Format(U"not implemented parameter type: {}",
               Enum<LocationType>::ToString(location->type)));
  }
}
void Compiler::VisitReturn(std::shared_ptr<ReturnExpression> node,
                           const ConstantMap &constantMap, ByteCode &byteCode) {
  VisitExpression(node->value, constantMap, byteCode);
  if (auto op = rules.Match(ExpressionType::Return, {node->type->typeCode})) {
    byteCode.AppendOp(op.value());
  } else {
    throw CompilerException(
        node->position,
        Format(U"not supported return type '{}'", node->type->ToString()));
  }
}
void Compiler::VisitConditional(std::shared_ptr<ConditionalExpression> node,
                                const ConstantMap &constantMap,
                                ByteCode &byteCode) {
  VisitExpression(node->condition, constantMap, byteCode);
  byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
  int index1 = byteCode.Size();
  byteCode.AppendU16Unchecked(0);

  VisitExpression(node->ifTrue, constantMap, byteCode);
  byteCode.AppendOp(OpCode::JUMP);
  int index2 = byteCode.Size();
  byteCode.AppendU16Unchecked(0);

  int target1 = byteCode.Size();
  VisitExpression(node->ifFalse, constantMap, byteCode);
  int target2 = byteCode.Size();

  if (target2 > std::numeric_limits<uint16_t>::max()) {
    throw CompilerException(node->position,
                            U"the function is too complicated. Consider split "
                            U"it into smaller functions");
  } else {
    byteCode.WriteUShort(index1, target1);
    byteCode.WriteUShort(index2, target2);
  }
}
void Compiler::VisitDefault(std::shared_ptr<DefaultExpression> node,
                            const ConstantMap &constantMap,
                            ByteCode &byteCode) {
  switch (node->type->typeCode) {
  case TypeCode::Void:
  case TypeCode::Int32: {
    byteCode.AppendOp(OpCode::PUSH_I32_0);
    break;
  }
  default: {
    std::cout << node->type->ToString() << std::endl;
    throw CompilerException(
        node->position,
        Format(U"not supported type of default expression of type '{}'",
               node->type->ToString()));
  }
  }
}
void Compiler::VisitInvocation(std::shared_ptr<InvocationExpression> node,
                               const ConstantMap &constantMap,
                               ByteCode &byteCode) {
  if (node->expression->type->typeCode == TypeCode::Function) {
    auto function =
        std::static_pointer_cast<FunctionType>(node->expression->type);
    for (auto arg : node->arguments) {
      VisitExpression(arg.value, constantMap, byteCode);
    }
    VisitExpression(node->expression, constantMap, byteCode);
    byteCode.AppendOp(OpCode::INVOKE);
  } else if (node->expression->type->typeCode == TypeCode::Array) {
    auto arrayType =
        std::static_pointer_cast<ArrayType>(node->expression->type);
    if (node->arguments.size() == 1) {
      VisitExpression(node->expression, constantMap, byteCode);
      for (auto arg : node->arguments) {
        VisitExpression(arg.value, constantMap, byteCode);
      }
      switch (arrayType->elementType->typeCode) {
      case TypeCode::Boolean:
      case TypeCode::Char:
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::PUSH_ARRAY_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::PUSH_ARRAY_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::PUSH_ARRAY_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::PUSH_ARRAY_F64);
        break;
      }
      case TypeCode::String:
      case TypeCode::Object: {
        byteCode.AppendOp(OpCode::PUSH_ARRAY_OBJECT);
        break;
      }
      default: {
        throw CompilerException(node->position, U"wrong type of array element");
      }
      }
    } else {
      throw CompilerException(
          node->position,
          Format(U"the number of array index should be 1 instead of {}",
                 static_cast<int>(node->arguments.size())));
    }
  } else {
    throw CompilerException(node->position, U"object is not callable");
  }
}
void Compiler::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                                 const ConstantMap &constantMap,
                                 ByteCode &byteCode) {
  VisitExpression(node->object, constantMap, byteCode);
  auto location = node->location;
  if (location->type == LocationType::ModuleField) {
    switch (node->type->typeCode) {
    case TypeCode::Int32: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_I32);
      break;
    }
    case TypeCode::Int64: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_I64);
      break;
    }
    case TypeCode::Float32: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_F32);
      break;
    }
    case TypeCode::Float64: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_F64);
      break;
    }
    case TypeCode::String: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_OBJECT);
      break;
    }
    case TypeCode::Class: {
      byteCode.AppendOp(OpCode::PUSH_STATIC_OBJECT);
      break;
    }
    default: {
      throw CompilerException(node->position,
                              U"not supported member access type");
    }
    }
    auto loc = std::static_pointer_cast<MemberLocation>(node->location);
    if (auto index =
            GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_STATIC_VAR,
                        loc->name.ToString())) {
      byteCode.AppendU16Unchecked(index.value());
    } else {
      throw CompilerException(
          node->position,
          Format(U"module '{}' not found", node->object->type->ToString()));
    }
    byteCode.AppendU16Unchecked(
        std::static_pointer_cast<MemberLocation>(location)->offset);
  } else if (location->type == LocationType::ModuleMethod) {
    byteCode.AppendOp(OpCode::PUSH_FUNCTION);
    auto loc = std::static_pointer_cast<MemberLocation>(node->location);
    if (auto index = GetConstant(constantMap,
                                 ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION,
                                 loc->name.ToString())) {
      byteCode.AppendU16Unchecked(index.value());
    } else {
      throw CompilerException(
          node->position,
          Format(U"missing module method '{}'", loc->name.ToString()));
    }
  } else if (location->type == LocationType::ClassField) {
    switch (node->type->typeCode) {
    case TypeCode::Int32: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_I32);
      break;
    }
    case TypeCode::Int64: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_I64);
      break;
    }
    case TypeCode::Float32: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_F32);
      break;
    }
    case TypeCode::Float64: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_F64);
      break;
    }
    case TypeCode::String: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_OBJECT);
      break;
    }
    case TypeCode::Array:
    case TypeCode::Class: {
      byteCode.AppendOp(OpCode::PUSH_FIELD_OBJECT);
      break;
    }
    default: {
      throw CompilerException(
          node->position, Format(U"not supported member '{}' access type '{}'",
                                 node->field, node->type->ToString()));
    }
    }
    byteCode.AppendU16Unchecked(
        std::static_pointer_cast<MemberLocation>(location)->offset);
  } else if (location->type == LocationType::ClassMethod ||
             location->type == LocationType::InterfaceMethod) {
    byteCode.AppendOp(OpCode::PUSH_METHOD);
    auto loc = std::static_pointer_cast<MemberLocation>(node->location);
    if (auto index =
            GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_METHOD,
                        loc->name.ToString())) {
      byteCode.AppendU16Unchecked(index.value());
    } else {
      throw CompilerException(
          node->position,
          Format(U"class '{}' not found", node->object->type->ToString()));
    }
    byteCode.AppendU16Unchecked(loc->offset);
  } else {
    throw CompilerException(
        node->position,
        Format(U"member access: member '{}' parameter location type '{}' is "
               U"not supported",
               node->field, Enum<LocationType>::ToString(location->type)));
  }
}
void Compiler::VisitNewExpression(std::shared_ptr<NewExpression> node,
                                  const ConstantMap &constantMap,
                                  ByteCode &byteCode) {
  byteCode.AppendOp(OpCode::NEW);
  auto loc = std::static_pointer_cast<TypeLocation>(node->location);
  if (auto index = GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_CLASS,
                               loc->name.ToString())) {
    byteCode.AppendU16Unchecked(index.value());
  } else {
    throw CompilerException(node->position,
                            Format(U"missing new '{}'", loc->name.ToString()));
  }
  std::unordered_set<std::u32string> initializedFields;
  for (auto arg : node->arguments) {
    if (arg.name) {
      initializedFields.insert(*arg.name);
    }
  }
  auto classType = std::static_pointer_cast<ClassType>(node->type);
  if (auto classInfo = project.GetClass(classType)) {
    for (auto &field : (*classInfo)->fields) {
      // not found
      if (initializedFields.find(field->name) == initializedFields.end()) {
        byteCode.AppendOp(OpCode::DUPLICATE);
        VisitExpression(field->value, constantMap, byteCode);
        switch (field->value->type->typeCode) {
        case TypeCode::Char:
        case TypeCode::Boolean:
        case TypeCode::Int32: {
          byteCode.AppendOp(OpCode::POP_FIELD_I32);
          break;
        }
        case TypeCode::Int64: {
          byteCode.AppendOp(OpCode::POP_FIELD_I64);
          break;
        }
        case TypeCode::Float32: {
          byteCode.AppendOp(OpCode::POP_FIELD_F32);
          break;
        }
        case TypeCode::Float64: {
          byteCode.AppendOp(OpCode::POP_FIELD_F64);
          break;
        }
        case TypeCode::String: {
          byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
          break;
        }
        case TypeCode::Array:
        case TypeCode::Class: {
          byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
          break;
        }
        default: {
          throw CompilerException(
              node->position,
              Format(U"not supported field initializer type '{}'",
                     field->value->type->ToString()));
        }
        }
        if (field->index) {
          byteCode.AppendU16Unchecked(*field->index);
        } else {
          throw CompilerException(
              node->position,
              Format(U"field '{}' index not assigned", field->name));
        }
      }
    }
    for (auto arg : node->arguments) {
      byteCode.AppendOp(OpCode::DUPLICATE);
      // byteCode.AppendUShort(1); // get the initialized class object
      VisitExpression(arg.value, constantMap, byteCode);
      switch (arg.value->type->typeCode) {
      case TypeCode::Char:
      case TypeCode::Boolean:
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::POP_FIELD_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::POP_FIELD_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::POP_FIELD_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::POP_FIELD_F64);
        break;
      }
      case TypeCode::String: {
        byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
        break;
      }
      case TypeCode::Array:
      case TypeCode::Class: {
        byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
        break;
      }
      default: {
        throw CompilerException(
            node->position, Format(U"not supported field initializer type '{}'",
                                   arg.value->type->ToString()));
      }
      }
      if (arg.index) {
        byteCode.AppendU16Unchecked(*arg.index);
      } else {
        throw CompilerException(
            node->position,
            Format(U"field '{}' index not assigned", *arg.name));
      }
    }
  }
}
void Compiler::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                                     const ConstantMap &constantMap,
                                     ByteCode &byteCode) {
  VisitExpression(node->value, constantMap, byteCode);
  switch (node->variable->type->typeCode) {
  case TypeCode::Int32: {
    byteCode.AppendOp(OpCode::POP_LOCAL_I32);
    break;
  }
  case TypeCode::Int64: {
    byteCode.AppendOp(OpCode::POP_LOCAL_I64);
    break;
  }
  case TypeCode::Float32: {
    byteCode.AppendOp(OpCode::POP_LOCAL_F32);
    break;
  }
  case TypeCode::Float64: {
    byteCode.AppendOp(OpCode::POP_LOCAL_F64);
    break;
  }
  case TypeCode::String: {
    byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
    break;
  }
  case TypeCode::Class:
  case TypeCode::Interface: {
    byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
    break;
  }
  default: {
    throw CompilerException(node->position,
                            U"not supported member access type");
  }
  }
  if (node->variable->location->type == LocationType::LocalVariable) {
    byteCode.AppendU16Unchecked(
        std::static_pointer_cast<ParameterLocation>(node->variable->location)
            ->offset);
  } else {
    throw CompilerException(node->position,
                            U"cannot compile non-local variable definition");
  }
}
void Compiler::VisitAssign(std::shared_ptr<BinaryExpression> node,
                           const ConstantMap &constantMap, ByteCode &byteCode) {
  VisitExpression(node->right, constantMap, byteCode);

  if (node->left->nodeType == ExpressionType::Parameter) {
    auto parameter = std::static_pointer_cast<ParameterExpression>(node->left);
    if (parameter->location->type == LocationType::LocalVariable) {
      switch (node->left->type->typeCode) {
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::POP_LOCAL_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::POP_LOCAL_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::POP_LOCAL_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::POP_LOCAL_F64);
        break;
      }
      case TypeCode::String: {
        byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
        break;
      }
      case TypeCode::Interface:
      case TypeCode::Class: {
        byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
        break;
      }
      default: {
        throw CompilerException(node->position,
                                U"not supported assignment type");
      }
      }
      byteCode.AppendU16Unchecked(
          std::static_pointer_cast<ParameterLocation>(parameter->location)
              ->offset);
    } else if (parameter->location->type == LocationType::ModuleField) {
      switch (node->left->type->typeCode) {
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::POP_STATIC_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::POP_STATIC_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::POP_STATIC_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::POP_STATIC_F64);
        break;
      }
      case TypeCode::String: {
        byteCode.AppendOp(OpCode::POP_STATIC_OBJECT);
        break;
      }
      case TypeCode::Interface:
      case TypeCode::Class: {
        byteCode.AppendOp(OpCode::POP_STATIC_OBJECT);
        break;
      }
      default: {
        throw CompilerException(node->position,
                                U"not supported assignment type");
      }
      }
      auto loc = static_pointer_cast<MemberLocation>(parameter->location);
      if (auto index =
              GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_STATIC_VAR,
                          loc->name.ToString())) {
        byteCode.AppendU16Unchecked(index.value());
      } else {
        throw CompilerException(node->position, Format(U"module '{}' not found",
                                                       loc->name.ToString()));
      }
    } else {
      throw CompilerException(node->position,
                              U"cannot compile non-local variable definition");
    }
  } else if (node->left->nodeType == ExpressionType::MemberAccess) {
    auto memberAccess =
        std::static_pointer_cast<MemberAccessExpression>(node->left);
    auto location = memberAccess->location;
    VisitExpression(memberAccess->object, constantMap, byteCode);
    if (location->type == LocationType::ModuleField) {
      switch (memberAccess->type->typeCode) {
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::POP_STATIC_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::POP_STATIC_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::POP_STATIC_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::POP_STATIC_F64);
        break;
      }
      case TypeCode::String: {
        byteCode.AppendOp(OpCode::POP_STATIC_OBJECT);
        break;
      }
      case TypeCode::Class: {
        byteCode.AppendOp(OpCode::POP_STATIC_OBJECT);
        break;
      }
      default: {
        throw CompilerException(node->position,
                                U"not supported module field type");
      }
      }

      auto loc = static_pointer_cast<MemberLocation>(memberAccess->location);
      if (auto index =
              GetConstant(constantMap, ConstantKind::CONSTANT_FLAG_STATIC_VAR,
                          loc->name.ToString())) {
        byteCode.AppendU16Unchecked(index.value());
      } else {
        throw CompilerException(node->position,
                                Format(U"module '{}' not found",
                                       memberAccess->object->type->ToString()));
      }
    } else if (location->type == LocationType::ClassField) {
      switch (memberAccess->type->typeCode) {
      case TypeCode::Int32: {
        byteCode.AppendOp(OpCode::POP_FIELD_I32);
        break;
      }
      case TypeCode::Int64: {
        byteCode.AppendOp(OpCode::POP_FIELD_I64);
        break;
      }
      case TypeCode::Float32: {
        byteCode.AppendOp(OpCode::POP_FIELD_F32);
        break;
      }
      case TypeCode::Float64: {
        byteCode.AppendOp(OpCode::POP_FIELD_F64);
        break;
      }
      case TypeCode::String: {
        byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
        break;
      }
      case TypeCode::Class: {
        byteCode.AppendOp(OpCode::POP_FIELD_OBJECT);
        break;
      }
      default: {
        throw CompilerException(node->position,
                                U"not supported class field type");
      }
      }
      byteCode.AppendU16Unchecked(
          std::static_pointer_cast<MemberLocation>(location)->offset);
    } else {
      throw CompilerException(
          memberAccess->position,
          U"member access: parameter location type is not supported");
    }
  } else {
    throw CompilerException(node->position, U"wrong assignment");
  }
}
void Compiler::VisitWhile(std::shared_ptr<WhileExpression> node,
                          const ConstantMap &constantMap, ByteCode &byteCode) {
  int index1 = byteCode.Size();
  VisitExpression(node->condition, constantMap, byteCode);
  byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
  int index2 = byteCode.Size();
  if (index2 > std::numeric_limits<uint16_t>::max()) {
    throw CompilerException(node->position,
                            U"the function is too complicated. Consider split "
                            U"it into smaller functions");
  }
  byteCode.AppendU16Unchecked(0);

  VisitExpression(node->body, constantMap, byteCode);
  byteCode.AppendOp(OpCode::JUMP);
  byteCode.AppendU16Unchecked(index1);
  int target2 = byteCode.Size();
  if (target2 > std::numeric_limits<uint16_t>::max()) {
    throw CompilerException(node->position,
                            U"the function is too complicated. Consider split "
                            U"it into smaller functions");
  }
  byteCode.WriteUShort(index2, target2);
}
FullQualifiedName Compiler::CompileMainFunction(Project &project) {
  bool found = false;
  FullQualifiedName mainFunction;
  for (auto [_, package] : project.packages) {
    for (auto [_, moduleInfo] : package->modules) {
      if (moduleInfo->methods.ContainsKey(U"Main")) {
        auto &method = moduleInfo->methods.GetValueByKey(U"Main");
        if (found) {
          throw CompilerException(method->position,
                                  U"multiple definitions of the Main function");
        } else {
          mainFunction = FullQualifiedName(moduleInfo->route)
                             .Concat(moduleInfo->name)
                             .Concat(method->name);
          found = true;
        }
      }
    }
  }
  if (found == false) {
    throw CompilerException(SourcePosition(), U"cannot find Main function");
  } else {
    return mainFunction;
  }
}
void Compiler::CompileConstantPool(SourcePosition position,
                                   const ConstantMap &constantMap,
                                   ByteCode &byteCode) {
  byteCode.AppendU16Checked(
      static_cast<uint32_t>(constantMap.size()), [=]() -> CompilerException {
        return CompilerException(position, U"too many constants");
      });
  std::vector<std::tuple<ConstantKind, std::u32string>> constants;
  for (auto pair : constantMap) {
    ConstantKind kind = pair.first;
    for (auto p : pair.second) {
      auto text = p.first;
      int index = p.second;
      while (index >= constants.size()) {
        constants.push_back(std::tuple<ConstantKind, std::u32string>());
      }
      constants[index] = std::make_tuple(kind, text);
    }
  }
  for (auto t : constants) {
    ConstantKind kind;
    std::u32string text;

    std::tie(kind, text) = t;
    byteCode.AppendConstantKind(kind);
    byteCode.AppendString(text);
  }
}
GlobalInformation Compiler::CompileGlobalInformation(Project &project,
                                                     Executable &exe) {
  // if (exe.classes.size() > std::numeric_limits<uint16_t>::max())
  //{
  //	throw CompilerException(SourcePosition(), U"number of classes and
  // modules cannot exceed 65535");
  //}

  GlobalInformation globalInformation;
  globalInformation.classesCount = static_cast<int>(exe.classes.size());
  globalInformation.mainFunction = CompileMainFunction(project);

  for (auto execClass : exe.classes) {
    if (execClass->fields.size() > std::numeric_limits<uint16_t>::max()) {
      throw CompilerException(SourcePosition(),
                              Format(U"class '{}' has too many fields",
                                     execClass->name.ToString()));
    }
    if (execClass->methods.size() > std::numeric_limits<uint16_t>::max()) {
      throw CompilerException(SourcePosition(),
                              Format(U"class '{}' has too many methods",
                                     execClass->name.ToString()));
    }
  }

  return globalInformation;
}
void Compiler::ConvertExp(std::shared_ptr<UnaryExpression> node,
                          const ConstantMap &constantMap, ByteCode &byteCode) {
  auto from = node->operand->type;
  auto to = node->type;
  if (auto fromIndex = GetConstant(
          constantMap, ConstantKind::CONSTANT_FLAG_CLASS, from->ToString())) {
    byteCode.AppendU16Unchecked(fromIndex.value());
    if (auto toIndex = GetConstant(
            constantMap, ConstantKind::CONSTANT_FLAG_CLASS, to->ToString())) {
      byteCode.AppendU16Unchecked(toIndex.value());
    } else {
      throw CompilerException(node->position,
                              Format(U"type '{}' not found", to->ToString()));
    }
  } else {
    throw CompilerException(node->position,
                            Format(U"type '{}' not found", from->ToString()));
  }
}
std::optional<int> Compiler::GetConstant(const ConstantMap &constantMap,
                                         ConstantKind kind,
                                         std::u32string text) {
  if (constantMap.find(kind) != constantMap.end()) {
    if (constantMap.at(kind).find(text) != constantMap.at(kind).end()) {
      return constantMap.at(kind).at(text);
    } else {
      return {};
    }
  } else {
    return {};
  }
}
ConstantMap Compiler::MergeConstantPool(const ConstantMap &map1,
                                        const ConstantMap &map2) {
  ConstantMap resultMap;
  int index = 0;
  for (const auto &pair : map1) {
    ConstantKind kind = pair.first;
    const std::unordered_map<std::u32string, int> &textMap = pair.second;
    for (const auto &p : textMap) {
      const std::u32string text = p.first;
      resultMap[kind].insert({text, index});
      index++;
    }
  }
  for (const auto &pair : map2) {
    ConstantKind kind = pair.first;
    const std::unordered_map<std::u32string, int> &textMap = pair.second;
    for (const auto &p : textMap) {
      const std::u32string text = p.first;
      resultMap[kind].insert({text, index});
      index++;
    }
  }
  return resultMap;
}
std::vector<std::tuple<ConstantKind, std::u32string>>
GetConstantList(const ConstantMap &constantMap) {
  std::vector<std::tuple<ConstantKind, std::u32string>> constList;
  for (auto pair : constantMap) {
    ConstantKind kind = pair.first;
    for (auto p : pair.second) {
      std::u32string text = p.first;
      int index = p.second;
      while (index >= constList.size()) {
        constList.push_back(std::tuple<ConstantKind, std::u32string>());
      }
      constList[index] = std::make_tuple(kind, text);
    }
  }
  return constList;
}
void CompilerRuleSet::AddRule(ExpressionType nodeType,
                              std::vector<TypeCode> typeCodeList, OpCode op) {
  if (table.find(nodeType) != table.end()) {
    table[nodeType].push_back(CompilerRule{typeCodeList, op});
  } else {
    table.insert({nodeType, {CompilerRule{typeCodeList, op}}});
  }
}
std::optional<OpCode>
CompilerRuleSet::Match(ExpressionType nodeType,
                       std::vector<TypeCode> typeCodeList) {
  if (table.find(nodeType) != table.end()) {
    for (const auto &rule : table[nodeType]) {
      if (rule.typeCodeList == typeCodeList) {
        return rule.op;
      }
    }
    return {};
  } else {
    return {};
  }
}
NativeMethod::NativeMethod(std::u32string libName, std::u32string entryPoint)
    : libName{libName}, entryPoint{entryPoint} {}
void ViewExe(Executable &exe) {
  std::cout << "number of classes: " << exe.globalInformation.classesCount
            << std::endl;
  std::cout << "main function: "
            << exe.globalInformation.mainFunction.ToString() << std::endl;

  for (auto execClass : exe.classes) {
    cout << execClass->name.ToString() << endl;
    for (auto &field : execClass->fields) {
      cout << "field: " << field.name.ToString() << endl;
    }
    for (auto &method : execClass->methods) {
      cout << "method: " << method.name.ToString() << endl;
      cout << "code size = " << method.code.Size() << endl;
    }
    for (auto &staticVar : execClass->staticVariables) {
      cout << "static var: " << staticVar.name.ToString() << endl;
    }
    for (auto &staticFunc : execClass->staticFunctions) {
      cout << "static function: " << staticFunc.name.ToString() << endl;
      cout << "code size = " << staticFunc.code.Size() << endl;
    }
  }
}
ByteCode CompileExe(Executable &exe) {
  ByteCode byteCode;

  /* global information */
  byteCode.AppendU16Unchecked(exe.globalInformation.classesCount);
  byteCode.AppendString(exe.globalInformation.mainFunction.ToString());

  for (auto execClass : exe.classes) {
    auto constList = GetConstantList(execClass->constantMap);
    byteCode.AppendString(execClass->name.ToString());
    byteCode.AppendU16Unchecked(execClass->fields.size());
    byteCode.AppendU16Unchecked(execClass->methods.size());
    byteCode.AppendU16Unchecked(execClass->staticVariables.size());
    byteCode.AppendU16Unchecked(execClass->staticFunctions.size());
    byteCode.AppendU16Unchecked(execClass->inheritanceChain.size());
    byteCode.AppendU16Unchecked(execClass->virtualTable.size());
    byteCode.AppendU16Unchecked(constList.size());

    for (auto &field : execClass->fields) {
      byteCode.AppendString(field.name.ToString());
    }
    for (auto &method : execClass->methods) {
      byteCode.Append(static_cast<Byte>(method.flag));
      byteCode.AppendString(method.name.ToString());
      byteCode.AppendU16Unchecked(method.argsSize);
      byteCode.AppendU16Unchecked(method.localsSize);
      byteCode.AppendU16Unchecked(method.needStackSize);
      byteCode.AppendU16Unchecked(method.code.Size());

      byteCode.AppendByteCode(method.code);
    }

    for (auto &staticVar : execClass->staticVariables) {
      byteCode.AppendString(staticVar.name.ToString());
    }
    for (auto &staticFunction : execClass->staticFunctions) {
      byteCode.Append(static_cast<Byte>(staticFunction.flag));
      byteCode.AppendString(staticFunction.name.ToString());
      byteCode.AppendU16Unchecked(staticFunction.argsSize);
      byteCode.AppendU16Unchecked(staticFunction.localsSize);
      byteCode.AppendU16Unchecked(staticFunction.needStackSize);
      byteCode.AppendU16Unchecked(staticFunction.code.Size());

      if (staticFunction.flag == MethodFlag::NativeFunction) {
        byteCode.AppendString(staticFunction.nativeMethod.libName);
        byteCode.AppendString(staticFunction.nativeMethod.entryPoint);
      } else {
        byteCode.AppendByteCode(staticFunction.code);
      }
    }

    for (auto superClass : execClass->inheritanceChain) {
      byteCode.AppendString(superClass.ToString());
    }

    for (auto virtualMethods : execClass->virtualTable) {
      byteCode.AppendString(virtualMethods.className.ToString());
      std::cout << "CLASS: " << virtualMethods.className.ToString()
                << std::endl;
      byteCode.AppendU16Unchecked(virtualMethods.methodNames.size());
      for (auto methodName : virtualMethods.methodNames) {
        std::cout << "METHOD: " << methodName.ToString() << std::endl;
        byteCode.AppendString(methodName.ToString());
      }
    }

    for (auto tuple : constList) {
      auto kind = std::get<0>(tuple);
      auto text = std::get<1>(tuple);
      byteCode.Append(static_cast<Byte>(kind));
      byteCode.AppendString(text);
    }
  }

  return byteCode;
}
// ExecMethod::ExecMethod(FullQualifiedName className, MethodFlag flag,
//	FullQualifiedName name, int argsSize, int localsSize, int needStackSize)
//	: className{ className }, flag{ flag }, name{ name },
//	argsSize{ argsSize }, localsSize{ localsSize }, needStackSize{
// needStackSize }
//{
//}
// ExecClass::ExecClass(FullQualifiedName name, std::vector<MethodInfo> methods,
// std::vector<FieldInfo> fields, 	std::vector<MethodInfo> staticFunctions,
// std::vector<FieldInfo> staticVariables, 	std::vector<std::u32string>
// inheritanceChain, std::vector<FullQualifiedName> superClasses,
//	std::vector<FullQualifiedName> interfaces, VirtualTable virtualTable,
// ConstantMap constantMap) 	: name{ name }, methods{ methods }, fields{
// fields }, staticFunctions{ staticFunctions }, 	inheritanceChain{
// inheritanceChain }, superClasses{ superClasses }, interfaces{ interfaces },
// virtualTable{ virtualTable }, constantMap{ constantMap }
//{
//}
} // namespace cygni