#include "Compiler.hpp"
#include <algorithm>

namespace cygni
{
	void ByteCode::Append(Byte byte)
	{
		bytes.push_back(byte);
	}
	void ByteCode::AppendOp(OpCode op)
	{
		bytes.push_back(static_cast<Byte>(op));
	}
	void ByteCode::AppendTypeTag(TypeTag tag)
	{
		bytes.push_back(static_cast<Byte>(tag));
	}
	void ByteCode::AppendU16Checked(size_t value, const std::function<CompilerException()>& exceptionHandler)
	{
		if (value > std::numeric_limits<uint16_t>::max())
		{
			//throw std::invalid_argument("unsigned 16-bit integer overflow");
			throw exceptionHandler();
		}
		else
		{
			bytes.push_back(static_cast<Byte>(value / 256));
			bytes.push_back(static_cast<Byte>(value % 256));
		}
	}
	void ByteCode::AppendU16Unchecked(size_t value)
	{
		bytes.push_back(static_cast<Byte>(value / 256));
		bytes.push_back(static_cast<Byte>(value % 256));
	}
	void ByteCode::WriteUShort(int index, int value)
	{
		bytes.at(index) = (value / 256);
		bytes.at(index + 1) = (value % 256);
	}
	void ByteCode::AppendUInt(uint32_t value)
	{
		for (int i = 0; i < 4; i++)
		{
			bytes.push_back(value % 256);
			value = value / 256;
		}
	}
	void ByteCode::AppendTypeCode(TypeCode typeCode)
	{
		switch (typeCode)
		{
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
			throw NotImplementedException(
				Format(U"not supported type code: {}", Enum<TypeCode>::ToString(typeCode)));
		}
	}
	void ByteCode::AppendType(TypePtr type)
	{
		switch (type->typeCode)
		{
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
			throw NotImplementedException(
				Format(U"not supported type code: {}", Enum<TypeCode>::ToString(type->typeCode)));
		}
	}
	void ByteCode::AppendString(const std::u32string & u32str)
	{
		std::string u8str = UTF32ToUTF8(u32str);
		AppendU16Unchecked(u8str.size());
		for (Byte c : u8str)
		{
			Append(c);
		}
	}
	void ByteCode::AppendByteCode(const ByteCode & other)
	{
		bytes.insert(bytes.end(), other.bytes.begin(), other.bytes.end());
	}
	int ByteCode::Size() const
	{
		return static_cast<int>(bytes.size());
	}
	Compiler::Compiler(Project & project) : project{ project }
	{
	}
	ByteCode Compiler::Compile()
	{
		ByteCode byteCode;
		std::vector<std::shared_ptr<ClassInfo>> classes;
		std::vector<std::shared_ptr<ModuleInfo>> modules;

		std::tie(classes, modules) = CompileGlobalInformation(project, byteCode);

		for (auto classInfo : classes)
		{
			CompileClassInfo(classInfo, byteCode);
		}

		for (auto moduleInfo : modules)
		{
			CompileModuleInfo(moduleInfo, byteCode);
		}

		return byteCode;
	}
	void Compiler::VisitUnary(std::shared_ptr<UnaryExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		VisitExpression(node->operand, constantMap, byteCode);
		switch (node->nodeType)
		{
		case ExpressionType::UnaryPlus:
		{
			switch (node->operand->type->typeCode)
			{
			case TypeCode::Int32:
			case TypeCode::Int64:
			case TypeCode::Float32:
			case TypeCode::Float64: {
				break;
			}
			default:
			{
				throw TypeException(node->position,
					Format(U"unary plus operand type '{}' not suported", node->operand->type->ToString()));
			}
			}
			break;
		}
		case ExpressionType::UnaryMinus: {
			switch (node->operand->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::MINUS_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::MINUS_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::MINUS_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::MINUS_F64);
				break;
			}
			default:
			{
				throw TypeException(node->position,
					Format(U"unary minus operand type '{}' not suported", node->operand->type->ToString()));
			}
			}
			break;
		}
		case ExpressionType::Convert: {
			auto from = node->operand->type->typeCode;
			auto to = node->type->typeCode;
			if (from == TypeCode::Int32 && to == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::CAST_I32_TO_I64);
			}
			else if (from == TypeCode::Int64 && to == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::CAST_I64_TO_I32);
			}
			else if (from == TypeCode::Int32 && to == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::CAST_I32_TO_F32);
			}
			else if (from == TypeCode::Float32 && to == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::CAST_F32_TO_I32);
			}
			else if (from == TypeCode::Int32 && to == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::CAST_I32_TO_F64);
			}
			else if (from == TypeCode::Float64 && to == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::CAST_F64_TO_I32);
			}
			else if (from == TypeCode::Int64 && to == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::CAST_I64_TO_F32);
			}
			else if (from == TypeCode::Float32 && to == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::CAST_F32_TO_I64);
			}
			else if (from == TypeCode::Int64 && to == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::CAST_I64_TO_F64);
			}
			else if (from == TypeCode::Float64 && to == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::CAST_F64_TO_I64);
			}
			else if (from == TypeCode::Float32 && to == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::CAST_F32_TO_F64);
			}
			else if (from == TypeCode::Float64 && to == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::CAST_F64_TO_F32);
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"not supported conversion from '{}' to '{}", node->operand->type->ToString(), node->type->ToString()));
			}
			break;
		}
		case ExpressionType::UpCast: {
			auto from = node->operand->type->typeCode;
			auto to = node->type->typeCode;
			byteCode.AppendOp(OpCode::UP_CAST);
			if (to == TypeCode::Class)
			{
				byteCode.Append(0);
				auto classType = std::static_pointer_cast<ClassType>(node->type);
				if (auto classInfo = project.GetClass(classType))
				{
					if ((*classInfo)->index)
					{
						byteCode.AppendU16Checked(*((*classInfo)->index), [classInfo]()->CompilerException
						{
							return CompilerException((*classInfo)->position, U"the index of class cannot exceed 65535");
						});
					}
					else
					{
						throw CompilerException(node->position,
							Format(U"type '{}' index not assigned", classType->ToString()));
					}
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"type '{}' not found", classType->ToString()));
				}
			}
			else if (to == TypeCode::Interface)
			{
				byteCode.Append(1);
				auto interfaceType = std::static_pointer_cast<InterfaceType>(node->type);
				if (auto interfaceInfo = project.GetInterface(interfaceType))
				{
					if ((*interfaceInfo)->index)
					{
						byteCode.AppendU16Checked(*((*interfaceInfo)->index), [interfaceInfo]()->CompilerException
						{
							return CompilerException((*interfaceInfo)->position, U"the index of interface cannot exceed 65535");
						});
					}
					else
					{
						throw CompilerException(node->position,
							Format(U"type '{}' index not assigned", interfaceType->ToString()));
					}
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"type '{}' not found", interfaceType->ToString()));
				}
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"cannot convert the object from '{}' to '{}'", node->operand->type->ToString(), node->type->ToString()));
			}
			break;
		}
		case ExpressionType::DownCast: {
			auto from = node->operand->type->typeCode;
			auto to = node->type->typeCode;
			byteCode.AppendOp(OpCode::DOWN_CAST);
			if (to == TypeCode::Class)
			{
				byteCode.Append(0);
				auto classType = std::static_pointer_cast<ClassType>(node->type);
				if (auto classInfo = project.GetClass(classType))
				{
					if ((*classInfo)->index)
					{
						byteCode.AppendU16Checked(*((*classInfo)->index), [classInfo]()->CompilerException
						{
							return CompilerException((*classInfo)->position, U"the index of class cannot exceed 65535");
						});
					}
					else
					{
						throw CompilerException(node->position,
							Format(U"type '{}' index not assigned", classType->ToString()));
					}
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"type '{}' not found", classType->ToString()));
				}
			}
			else if (to == TypeCode::Interface)
			{
				byteCode.Append(1);
				auto interfaceType = std::static_pointer_cast<InterfaceType>(node->type);
				if (auto interfaceInfo = project.GetInterface(interfaceType))
				{
					if ((*interfaceInfo)->index)
					{
						byteCode.AppendU16Checked(*((*interfaceInfo)->index), [interfaceInfo]()->CompilerException
						{
							return CompilerException((*interfaceInfo)->position, U"the index of interface cannot exceed 65535");
						});
					}
					else
					{
						throw CompilerException(node->position,
							Format(U"type '{}' index not assigned", interfaceType->ToString()));
					}
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"type '{}' not found", interfaceType->ToString()));
				}
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"cannot convert the object from '{}' to '{}'", node->operand->type->ToString(), node->type->ToString()));
			}
			break;
		}
		case ExpressionType::ArrayLength: {
			byteCode.AppendOp(OpCode::ARRAY_LENGTH);
			break;
		}
		}
	}
	void Compiler::VisitBinary(std::shared_ptr<BinaryExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		VisitExpression(node->left, constantMap, byteCode);
		VisitExpression(node->right, constantMap, byteCode);
		TypeCode lt = node->left->type->typeCode;
		TypeCode rt = node->right->type->typeCode;
		switch (node->nodeType)
		{
		case ExpressionType::Add: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::ADD_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::ADD_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::ADD_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::ADD_F64);
				break;
			}
			default: {
				throw CompilerException(node->position, U"not supported type for addition");
			}
			}
			break;
		}
		case ExpressionType::Subtract: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::SUB_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::SUB_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::SUB_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::SUB_F64);
				break;
			}
			default: {
				throw CompilerException(node->position, U"not supported type for subtraction");
			}
			}
			break;
		}
		case ExpressionType::Multiply: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::MUL_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::MUL_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::MUL_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::MUL_F64);
				break;
			}
			default: {
				throw CompilerException(node->position, U"not supported type for multiplication");
			}
			}
			break;
		}
		case ExpressionType::Divide: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::DIV_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::DIV_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::DIV_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::DIV_F64);
				break;
			}
			default: {
				throw CompilerException(node->position, U"not supported type for division");
			}
			}
			break;
		}
		case ExpressionType::GreaterThan: {
			if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::GT_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::GT_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::GT_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::GT_F64);
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"not supported type for '>', left: '{}', right: '{}'", node->left->type->ToString(), node->right->type->ToString()));
			}
			break;
		}
		case ExpressionType::LessThan: {
			if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::LT_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::LT_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::LT_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::LT_F64);
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"not supported type for '<', left: '{}', right: '{}'", node->left->type->ToString(), node->right->type->ToString()));
			}
			break;
		}
		case ExpressionType::GreaterThanOrEqual: {
			if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::GE_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::GE_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::GE_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::GE_F64);
			}
			else
			{
				throw CompilerException(node->position, U"not supported type for '>='");
			}
			break;
		}
		case ExpressionType::LessThanOrEqual: {
			if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::LE_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::LE_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::LE_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::LE_F64);
			}
			else
			{
				throw CompilerException(node->position, U"not supported type for '<='");
			}
			break;
		}
		case ExpressionType::Equal: {
			if (lt == TypeCode::Char && rt == TypeCode::Char)
			{
				byteCode.AppendOp(OpCode::EQ_I32);
			}
			else if (lt == TypeCode::Boolean && rt == TypeCode::Boolean)
			{
				byteCode.AppendOp(OpCode::EQ_I32);
			}
			else if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::EQ_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::EQ_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::EQ_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::EQ_F64);
			}
			else
			{
				throw CompilerException(node->position, U"not supported type for '=='");
			}
			break;
		}
		case ExpressionType::NotEqual: {
			if (lt == TypeCode::Char && rt == TypeCode::Char)
			{
				byteCode.AppendOp(OpCode::NE_I32);
			}
			else if (lt == TypeCode::Boolean && rt == TypeCode::Boolean)
			{
				byteCode.AppendOp(OpCode::NE_I32);
			}
			else if (lt == TypeCode::Int32 && rt == TypeCode::Int32)
			{
				byteCode.AppendOp(OpCode::NE_I32);
			}
			else if (lt == TypeCode::Int64 && rt == TypeCode::Int64)
			{
				byteCode.AppendOp(OpCode::NE_I64);
			}
			else if (lt == TypeCode::Float32 && rt == TypeCode::Float32)
			{
				byteCode.AppendOp(OpCode::NE_F32);
			}
			else if (lt == TypeCode::Float64 && rt == TypeCode::Float64)
			{
				byteCode.AppendOp(OpCode::NE_F64);
			}
			else
			{
				throw CompilerException(node->position, U"not supported type for '!='");
			}
			break;
		}
		default: {
			throw NotImplementedException(
				Format(U"not supported binary operation: {}", Enum<ExpressionType>::ToString(node->nodeType)));
		}
		}
	}
	void Compiler::VisitBlock(std::shared_ptr<BlockExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		for (auto exp : node->expressions)
		{
			VisitExpression(exp, constantMap, byteCode);
		}
	}
	void Compiler::VisitConstant(
		std::shared_ptr<ConstantExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		if (node->type->typeCode == TypeCode::Boolean)
		{
			if (node->constant == U"true")
			{
				byteCode.AppendOp(OpCode::PUSH_I32_1);
			}
			else if (node->constant == U"false")
			{
				byteCode.AppendOp(OpCode::PUSH_I32_0);
			}
			else
			{
				throw CompilerException(node->position,
					U"illegal format of boolean value. should be 'true' or 'false'");
			}
		}
		else if (node->type->typeCode == TypeCode::String)
		{
			ConstantKey key{ node->type->typeCode, node->constant };
			if (constantMap.find(key) != constantMap.end())
			{
				int index = constantMap.at(key);
				if (auto moduleInfo = project.GetModule(std::make_shared<ModuleType>(PackageRoute{ U"Predef" }, U"String")))
				{
					if ((*moduleInfo)->methods.ContainsKey(U"New"))
					{
						auto& methodInfo = (*moduleInfo)->methods.GetValueByKey(U"New");
						// TO DO: call 'New' function

						byteCode.AppendOp(OpCode::PUSH_STRING);
						byteCode.AppendU16Checked(index, [moduleInfo]()->CompilerException
						{
							return CompilerException((*moduleInfo)->position, U"the index of module constant cannot exceed 65535");
						});

						byteCode.AppendOp(OpCode::PUSH_FUNCTION);
						byteCode.AppendU16Checked(*(*moduleInfo)->index, [moduleInfo]()->CompilerException
						{
							return CompilerException((*moduleInfo)->position, U"the index of module cannot exceed 65535");
						});
						byteCode.AppendU16Checked(*methodInfo.index, [&methodInfo]()->CompilerException
						{
							return CompilerException(methodInfo.position, U"the index of method cannot exceed 65535");
						});
						byteCode.AppendOp(OpCode::INVOKE);
					}
					else
					{
						throw CompilerException(node->position, U"missing 'New' method in the 'Predef.String' module");
					}
				}
				else
				{
					throw CompilerException(node->position, U"missing 'Predef.String' module");
				}
			}
			else
			{
				throw CompilerException(node->position,
					U"the given constant is not in the constant map");
			}
		}
		else
		{
			ConstantKey key{ node->type->typeCode, node->constant };
			if (constantMap.find(key) != constantMap.end())
			{
				int index = constantMap.at(key);

				switch (node->type->typeCode)
				{
				case TypeCode::Int32: {
					byteCode.AppendOp(OpCode::PUSH_I32);
					byteCode.AppendU16Checked(index, [&node]()->CompilerException
					{
						return CompilerException(node->position, U"the index of constant cannot exceed 65535");
					});
					break;
				}
				case TypeCode::Int64: {
					byteCode.AppendOp(OpCode::PUSH_I64);
					byteCode.AppendU16Checked(index, [&node]()->CompilerException
					{
						return CompilerException(node->position, U"the index of constant cannot exceed 65535");
					});
					break;
				}
				case TypeCode::Float32: {
					byteCode.AppendOp(OpCode::PUSH_F32);
					byteCode.AppendU16Checked(index, [&node]()->CompilerException
					{
						return CompilerException(node->position, U"the index of constant cannot exceed 65535");
					});
					break;
				}
				case TypeCode::Float64: {
					byteCode.AppendOp(OpCode::PUSH_F64);
					byteCode.AppendU16Checked(index, [&node]()->CompilerException
					{
						return CompilerException(node->position, U"the index of constant cannot exceed 65535");
					});
					break;
				}
				default: {
					throw CompilerException(node->position,
						Format(U"constant type '{}' not supported", node->type->ToString()));
				}
				}
			}
			else
			{
				throw CompilerException(node->position,
					U"the given constant is not in the constant map");
			}
		}
	}
	void Compiler::CompileClassInfo(std::shared_ptr<ClassInfo> info, ByteCode & byteCode)
	{
		byteCode.AppendString(info->name);
		byteCode.AppendU16Checked(info->fields.Size(), [info]()->CompilerException
		{
			return CompilerException(info->position, U"too many fields in the class");
		});
		for (const auto& field : info->fields.values)
		{
			byteCode.AppendString(field.name);
		}

		// constant pool
		CompileConstantPool(info->position, info->constantMap, byteCode);

		byteCode.AppendU16Unchecked(info->inheritanceChain.size());
		for (const auto& superClassType : info->inheritanceChain)
		{
			if (auto superClassInfo = project.GetClass(superClassType))
			{
				byteCode.AppendU16Unchecked(*(*superClassInfo)->index);
			}
			else
			{
				throw CompilerException(info->position,
					Format(U"missing super class '{}' for inheritance", superClassType->ToString()));
			}
		}
		byteCode.AppendU16Unchecked(info->interfaceList.size());
		for (const auto& superInterfaceType : info->interfaceList)
		{
			if (auto superInterfaceInfo = project.GetInterface(superInterfaceType))
			{
				byteCode.AppendU16Unchecked(*(*superInterfaceInfo)->index);
				byteCode.AppendU16Unchecked((*superInterfaceInfo)->allMethods.size());
				for (const auto& method : (*superInterfaceInfo)->allMethods)
				{
					if (info->methods.ContainsKey(method.name))
					{
						auto classMethod = info->methods.GetValueByKey(method.name);
						if (classMethod.selfType->typeCode == TypeCode::Class)
						{
							auto selfType = std::static_pointer_cast<ClassType>(classMethod.selfType);
							if (auto selfClassInfo = project.GetClass(selfType))
							{
								byteCode.AppendU16Unchecked(*(*selfClassInfo)->index);
								byteCode.AppendU16Unchecked(*classMethod.index);
							}
							else
							{
								throw CompilerException(classMethod.position,
									Format(U"cannot find the class '{}' of the method '{}", classMethod.selfType->ToString(),
										method.name));
							}
						}
						else
						{
							throw CompilerException(classMethod.position,
								Format(U"cannot find the class type '{}' of the method '{}", classMethod.selfType->ToString(),
									method.name));
						}
					}
					else
					{
						throw CompilerException(info->position,
							Format(U"function '{}' is not implemented", method.name));
					}
				}
			}
			else
			{
				throw CompilerException(info->position,
					Format(U"missing super interface '{}'", superInterfaceType->ToString()));
			}
		}
		byteCode.AppendU16Checked(info->methodDefs.Size(), [info]()->CompilerException
		{
			return  CompilerException(info->position, U"too many methods in the class");
		});
		for (const auto& method : info->methodDefs.values)
		{
			CompileMethodDef(method, info->constantMap, byteCode);
		}
	}
	void Compiler::CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode& byteCode)
	{
		byteCode.AppendString(info->name);
		if (info->fields.Size() > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(info->position, U"too many member variables in the module");
		}
		if (info->methods.Size() > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(info->position, U"too many member functions in the module");
		}
		CompileConstantPool(info->position, info->constantMap, byteCode);

		byteCode.AppendU16Unchecked(info->fields.Size());
		for (const auto& field : info->fields.values)
		{
			byteCode.AppendString(field.name);
		}

		byteCode.AppendU16Unchecked(info->methods.Size());
		for (const auto& method : info->methods.values)
		{
			CompileMethodDef(method, info->constantMap, byteCode);
		}
	}
	void Compiler::CompileMethodDef(const MethodDef & method, const ConstantMap& constantMap, ByteCode& byteCode)
	{
		if (method.annotations.ContainsKey(U"LibraryImport"))
		{
			if (method.parameters.size() > std::numeric_limits<uint16_t>::max())
			{
				throw CompilerException(method.position, U"too many parameters");
			}
			byteCode.Append(1); // native function
			auto annotation = method.annotations.GetValueByKey(U"LibraryImport");
			byteCode.AppendString(method.name);
			byteCode.AppendU16Unchecked(method.parameters.size());
			auto libName = std::static_pointer_cast<ConstantExpression>(annotation.arguments.at(0).value)->constant;
			auto funcName = std::static_pointer_cast<ConstantExpression>(annotation.arguments.at(1).value)->constant;
			byteCode.AppendString(libName);
			byteCode.AppendString(funcName);
		}
		else
		{
			byteCode.Append(0); // user-defined function
			byteCode.AppendString(method.name);
			if (method.selfType->typeCode == TypeCode::Class)
			{
				if (method.parameters.size() + 1 > std::numeric_limits<uint16_t>::max())
				{
					throw CompilerException(method.position, U"too many parameters");
				}
				byteCode.AppendU16Unchecked(method.parameters.size() + 1); // add 'this'
			}
			else
			{
				if (method.parameters.size() > std::numeric_limits<uint16_t>::max())
				{
					throw CompilerException(method.position, U"too many parameters");
				}
				byteCode.AppendU16Unchecked(method.parameters.size());
			}
			if (method.localVariables.size() > std::numeric_limits<uint16_t>::max())
			{
				throw CompilerException(method.position, U"too many variables");
			}
			byteCode.AppendU16Unchecked(method.localVariables.size());
			ByteCode funcCode;
			VisitExpression(method.body, constantMap, funcCode);
			if (funcCode.Size() > std::numeric_limits<uint16_t>::max())
			{
				throw CompilerException(method.position,
					U"the function is too complicated. Consider split it into smaller functions");
			}
			byteCode.AppendByteCode(funcCode);
		}
	}
	void Compiler::VisitParameter(std::shared_ptr<ParameterExpression> parameter, const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		auto location = parameter->location;
		switch (location->type)
		{
		case LocationType::LocalVariable: {
			switch (parameter->type->typeCode)
			{
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
			case TypeCode::Class: {
				byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
				break;
			}
			default: {
				throw CompilerException(parameter->position,
					Format(U"not supported local variable type '{}'", parameter->type->ToString()));
			}
			}
			byteCode.AppendU16Unchecked(std::static_pointer_cast<ParameterLocation>(location)->offset);
			break;
		}
		case LocationType::ClassField: {
			byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
			byteCode.AppendU16Unchecked(0); /* this */
			switch (parameter->type->typeCode)
			{
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
				throw CompilerException(parameter->position, Format(U"not supported class field access type '{}'", parameter->type->ToString()));
			}
			}
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
			break;
		}
		case LocationType::ModuleMethod: {
			std::cout << "push module method: " << parameter->name << std::endl;
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
			break;
		}
		case LocationType::ClassMethod: {
			std::cout << "push class method: " << parameter->name << std::endl;
			//byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
			//byteCode.AppendUShort(0); /* this */
			//byteCode.AppendOp(OpCode::PUSH_METHOD);
			//byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			//byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
			break;
		}
		case LocationType::ModuleName: {
			std::cout << "module name: " << parameter->name << std::endl;
			break;
		}
		default:
			throw NotImplementedException(
				Format(U"not implemented parameter type: {}", Enum<LocationType>::ToString(location->type)));
		}
	}
	void Compiler::VisitReturn(std::shared_ptr<ReturnExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		VisitExpression(node->value, constantMap, byteCode);
		switch (node->type->typeCode)
		{
		case TypeCode::Boolean:
		case TypeCode::Char:
		case TypeCode::Int32: {
			byteCode.AppendOp(OpCode::RETURN_I32);
			break;
		}
		case TypeCode::Int64: {
			byteCode.AppendOp(OpCode::RETURN_I64);
			break;
		}
		case TypeCode::Float32: {
			byteCode.AppendOp(OpCode::RETURN_F32);
			break;
		}
		case TypeCode::Float64: {
			byteCode.AppendOp(OpCode::RETURN_F64);
			break;
		}
		case TypeCode::String: {
			byteCode.AppendOp(OpCode::RETURN_OBJECT);
			break;
		}
		case TypeCode::Class: {
			byteCode.AppendOp(OpCode::RETURN_OBJECT);
			break;
		}
		default: {
			throw CompilerException(node->position,
				Format(U"not supported return type '{}'", node->type->ToString()));
		}
		}
	}
	void Compiler::VisitConditional(std::shared_ptr<ConditionalExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
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

		if (target2 > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(node->position, U"the function is too complicated. Consider split it into smaller functions");
		}
		else
		{
			byteCode.WriteUShort(index1, target1);
			byteCode.WriteUShort(index2, target2);
		}
	}
	void Compiler::VisitDefault(std::shared_ptr<DefaultExpression> node, const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		switch (node->type->typeCode)
		{
		case TypeCode::Int32: {
			byteCode.AppendOp(OpCode::PUSH_I32_0);
			break;
		}
		default: {
			throw CompilerException(node->position,
				Format(U"not supported type of default expression of type '{}'", node->type->ToString()));
		}
		}
	}
	void Compiler::VisitInvocation(std::shared_ptr<InvocationExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		if (node->expression->type->typeCode == TypeCode::Function)
		{
			auto function = std::static_pointer_cast<FunctionType>(node->expression->type);
			if (function->selfType->typeCode == TypeCode::Array)
			{
				// omit
				// Size
			}
			else
			{
				for (auto arg : node->arguments)
				{
					VisitExpression(arg.value, constantMap, byteCode);
				}
				VisitExpression(node->expression, constantMap, byteCode);
				byteCode.AppendOp(OpCode::INVOKE);
			}
			//else if (function->selfType->typeCode == TypeCode::Module)
			//{
			//	for (auto arg : node->arguments)
			//	{
			//		VisitExpression(arg.value, constantMap, byteCode);
			//	}
			//	VisitExpression(node->expression, constantMap, byteCode);
			//	auto moduleType = std::static_pointer_cast<ModuleType>(function->selfType);
			//	if (auto moduleInfo = project.GetModule(moduleType))
			//	{
			//		byteCode.AppendOp(OpCode::INVOKE_FUNCTION);
			//		if ((*moduleInfo)->methods.ContainsKey(function->name))
			//		{
			//			auto& method = (*moduleInfo)->methods.GetValueByKey(function->name);
			//			byteCode.AppendU16Unchecked(*(*moduleInfo)->index);
			//			byteCode.AppendU16Unchecked(*method.index);
			//		}
			//		else
			//		{
			//			throw CompilerException(node->position,
			//				Format(U"missing module function '{}'", function->name));
			//		}
			//	}
			//	else
			//	{
			//		throw CompilerException(node->position,
			//			Format(U"missing module '{}'", moduleType->ToString()));
			//	}
			//}
			//else if (function->selfType->typeCode == TypeCode::Class)
			//{
			//	if (node->expression->nodeType == ExpressionType::MemberAccess)
			//	{
			//		auto memberAccessExp = std::static_pointer_cast<MemberAccessExpression>(node->expression);
			//		VisitExpression(memberAccessExp->object, constantMap, byteCode);
			//	}
			//	else if (node->expression->nodeType == ExpressionType::Parameter)
			//	{
			//		byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
			//		byteCode.AppendU16Unchecked(0); /* this */
			//	}
			//	else
			//	{
			//		throw CompilerException(node->position, U"not supported method call");
			//	}
			//	for (auto arg : node->arguments)
			//	{
			//		VisitExpression(arg.value, constantMap, byteCode);
			//	}
			//	auto classType = std::static_pointer_cast<ClassType>(function->selfType);
			//	if (auto classInfo = project.GetClass(classType))
			//	{
			//		byteCode.AppendOp(OpCode::PUSH_METHOD);
			//		if ((*classInfo)->methods.ContainsKey(function->name))
			//		{
			//			auto& method = (*classInfo)->methods.GetValueByKey(function->name);
			//			byteCode.AppendU16Unchecked(method.parameters.size());
			//			byteCode.AppendU16Unchecked(*method.index);
			//		}
			//		else
			//		{
			//			throw CompilerException(node->position,
			//				Format(U"missing class method '{}'", function->name));
			//		}
			//	}
			//	else
			//	{
			//		throw CompilerException(node->position,
			//			Format(U"missing class '{}'", classType->ToString()));
			//	}

			//}
			//else if (function->selfType->typeCode == TypeCode::Interface)
			//{

			//}
			//else
			//{
			//	throw CompilerException(node->position,
			//		Format(U"not callable object type '{}'", function->selfType->ToString()));
			//}
		}
		else if (node->expression->type->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(node->expression->type);
			if (node->arguments.size() == 1)
			{
				VisitExpression(node->expression, constantMap, byteCode);
				for (auto arg : node->arguments)
				{
					VisitExpression(arg.value, constantMap, byteCode);
				}
				switch (arrayType->elementType->typeCode)
				{
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
			}
			else
			{
				throw CompilerException(node->position,
					Format(U"the number of array index should be 1 instead of {}", static_cast<int>(node->arguments.size())));
			}
		}
		else
		{
			throw CompilerException(node->position, U"object is not callable");
		}
	}
	void Compiler::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		VisitExpression(node->object, constantMap, byteCode);
		auto location = node->location;
		if (node->object->type->typeCode == TypeCode::Array && node->field == U"Size")
		{
			byteCode.AppendOp(OpCode::ARRAY_LENGTH);
		}
		else if (location->type == LocationType::ModuleField)
		{
			switch (node->type->typeCode)
			{
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
				throw CompilerException(node->position, U"not supported member access type");
			}
			}
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else if (location->type == LocationType::ModuleMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else if (location->type == LocationType::ClassField)
		{
			switch (node->type->typeCode)
			{
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
				throw CompilerException(node->position, Format(U"not supported member '{}' access type '{}'",
					node->field, node->type->ToString()));
			}
			}
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else if (location->type == LocationType::ClassMethod)
		{
			std::cout << "push class method: " << node->field << std::endl;
			byteCode.AppendOp(OpCode::PUSH_METHOD);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else
		{
			throw CompilerException(node->position,
				Format(U"member access: member '{}' parameter location type '{}' is not supported",
					node->field, Enum<LocationType>::ToString(location->type)));
		}
	}
	void Compiler::VisitNewExpression(std::shared_ptr<NewExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		byteCode.AppendOp(OpCode::NEW);
		byteCode.AppendU16Unchecked(std::static_pointer_cast<TypeLocation>(node->location)->index);
		std::unordered_set<std::u32string> initializedFields;
		for (auto arg : node->arguments)
		{
			if (arg.name)
			{
				initializedFields.insert(*arg.name);
			}
		}
		auto classType = std::static_pointer_cast<ClassType>(node->type);
		if (auto classInfo = project.GetClass(classType))
		{
			for (auto& field : (*classInfo)->fields)
			{
				// not found
				if (initializedFields.find(field.name) == initializedFields.end())
				{
					byteCode.AppendOp(OpCode::DUPLICATE);
					VisitExpression(field.value, constantMap, byteCode);
					switch (field.value->type->typeCode)
					{
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
						throw CompilerException(node->position,
							Format(U"not supported field initializer type '{}'", field.value->type->ToString()));
					}
					}
					if (field.index)
					{
						byteCode.AppendU16Unchecked(*field.index);
					}
					else
					{
						throw CompilerException(node->position,
							Format(U"field '{}' index not assigned", field.name));
					}
				}
			}
			for (auto arg : node->arguments)
			{
				byteCode.AppendOp(OpCode::DUPLICATE);
				//byteCode.AppendUShort(1); // get the initialized class object
				VisitExpression(arg.value, constantMap, byteCode);
				switch (arg.value->type->typeCode)
				{
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
					throw CompilerException(node->position,
						Format(U"not supported field initializer type '{}'", arg.value->type->ToString()));
				}
				}
				if (arg.index)
				{
					byteCode.AppendU16Unchecked(*arg.index);
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"field '{}' index not assigned", *arg.name));
				}
			}
		}
	}
	void Compiler::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
		const ConstantMap & constantMap, ByteCode & byteCode)
	{
		VisitExpression(node->value, constantMap, byteCode);
		switch (node->variable->type->typeCode)
		{
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
			throw CompilerException(node->position, U"not supported member access type");
		}
		}
		if (node->variable->location->type == LocationType::LocalVariable)
		{
			byteCode.AppendU16Unchecked(std::static_pointer_cast<ParameterLocation>(node->variable->location)->offset);
		}
		else
		{
			throw CompilerException(node->position, U"cannot compile non-local variable definition");
		}
	}
	void Compiler::VisitAssign(std::shared_ptr<BinaryExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		VisitExpression(node->right, constantMap, byteCode);

		if (node->left->nodeType == ExpressionType::Parameter)
		{
			auto parameter = std::static_pointer_cast<ParameterExpression>(node->left);
			if (parameter->location->type == LocationType::LocalVariable)
			{
				switch (node->left->type->typeCode)
				{
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
				case TypeCode::Class: {
					byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
					break;
				}
				default: {
					throw CompilerException(node->position, U"not supported assignment type");
				}
				}
				byteCode.AppendU16Unchecked(std::static_pointer_cast<ParameterLocation>(parameter->location)->offset);
			}
			else
			{
				throw CompilerException(node->position, U"cannot compile non-local variable definition");
			}
		}
		else if (node->left->nodeType == ExpressionType::MemberAccess)
		{
			auto memberAccess = std::static_pointer_cast<MemberAccessExpression>(node->left);
			auto location = memberAccess->location;
			VisitExpression(memberAccess->object, constantMap, byteCode);
			if (location->type == LocationType::ModuleField)
			{
				switch (memberAccess->type->typeCode)
				{
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
					throw CompilerException(node->position, U"not supported module field type");
				}
				}
				byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->index);
				byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
			}
			else if (location->type == LocationType::ClassField)
			{
				switch (memberAccess->type->typeCode)
				{
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
					throw CompilerException(node->position, U"not supported class field type");
				}
				}
				byteCode.AppendU16Unchecked(std::static_pointer_cast<MemberLocation>(location)->offset);
			}
			else
			{
				throw CompilerException(memberAccess->position, U"member access: parameter location type is not supported");
			}
		}
		else
		{
			throw CompilerException(node->position, U"wrong assignment");
		}
	}
	void Compiler::VisitWhile(std::shared_ptr<WhileExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		int index1 = byteCode.Size();
		VisitExpression(node->condition, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
		int index2 = byteCode.Size();
		if (index2 > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(node->position, U"the function is too complicated. Consider split it into smaller functions");
		}
		byteCode.AppendU16Unchecked(0);

		VisitExpression(node->body, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP);
		byteCode.AppendU16Unchecked(index1);
		int target2 = byteCode.Size();
		if (target2 > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(node->position, U"the function is too complicated. Consider split it into smaller functions");
		}
		byteCode.WriteUShort(index2, target2);
	}
	void Compiler::CompileMainFunction(const std::vector<std::shared_ptr<ModuleInfo>>& modules, ByteCode & byteCode)
	{
		bool found = false;
		for (auto moduleInfo : modules)
		{
			if (moduleInfo->methods.ContainsKey(U"Main"))
			{
				auto& method = moduleInfo->methods.GetValueByKey(U"Main");
				if (found)
				{
					throw CompilerException(method.position, U"multiple definitions of the Main function");
				}
				else
				{
					byteCode.AppendU16Unchecked(*(moduleInfo->index));
					byteCode.AppendU16Unchecked(*(method.index));
					found = true;
				}
			}
		}
		if (found == false)
		{
			throw CompilerException(SourcePosition(), U"cannot find Main function");
		}
	}
	void Compiler::CompileConstantPool(SourcePosition position, const ConstantMap & constantMap, ByteCode& byteCode)
	{
		byteCode.AppendU16Checked(static_cast<uint32_t>(constantMap.size()), [=]()->CompilerException
		{
			return CompilerException(position, U"too many constants");
		});
		std::vector<ConstantKey> constants(constantMap.size());
		for (auto pair : constantMap)
		{
			auto key = pair.first;
			auto index = pair.second;
			constants[index] = key;
		}
		for (ConstantKey constant : constants)
		{
			byteCode.AppendTypeCode(constant.typeCode);
			byteCode.AppendString(constant.constant);
		}
	}
	std::tuple<std::vector<std::shared_ptr<ClassInfo>>, std::vector<std::shared_ptr<ModuleInfo>>>
		Compiler::CompileGlobalInformation(Project & project, ByteCode& byteCode)
	{
		int classCount = 0;
		int moduleCount = 0;
		for (auto pkg : project.packages)
		{
			classCount = classCount + pkg->classDefs.Size();
			moduleCount = moduleCount + pkg->moduleDefs.Size();
		}

		if (classCount > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(SourcePosition(), U"number of class cannot exceed 65535");
		}
		if (moduleCount > std::numeric_limits<uint16_t>::max())
		{
			throw CompilerException(SourcePosition(), U"number of module cannot exceed 65535");
		}

		std::vector<std::shared_ptr<ClassInfo>> classes(classCount);
		std::vector<std::shared_ptr<ModuleInfo>> modules(moduleCount);

		for (auto pkg : project.packages)
		{
			for (auto classInfo : pkg->classDefs)
			{
				classes.at(*classInfo->index) = classInfo;
			}
			for (auto moduleInfo : pkg->moduleDefs)
			{
				modules.at(*moduleInfo->index) = moduleInfo;
			}
		}

		CompileMainFunction(modules, byteCode);

		byteCode.AppendU16Unchecked(classes.size());
		byteCode.AppendU16Unchecked(modules.size());
		for (auto classInfo : classes)
		{
			if (classInfo->fields.Size() > std::numeric_limits<uint16_t>::max())
			{
				throw CompilerException(classInfo->position, U"too many methods");
			}
			if (classInfo->methods.Size() > std::numeric_limits<uint16_t>::max())
			{
				throw CompilerException(classInfo->position, U"too many methods");
			}
			byteCode.AppendU16Unchecked(classInfo->fields.Size());
			byteCode.AppendU16Unchecked(classInfo->methods.Size());
		}

		return std::make_tuple(classes, modules);
	}
} // namespace cygni