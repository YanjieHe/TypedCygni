#include "Compiler.hpp"
#include "Exception.hpp"
#include <functional>
#include <algorithm>

namespace cygni
{
	ByteCode::ByteCode(std::u32string u32str)
	{
		AppendString(u32str);
	}
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
	void ByteCode::AppendUShort(size_t value)
	{
		if (value > 65535)
		{
			// TO DO: better location information
			throw CompilerException(SourcePosition(), U"unsigned 16-bit integer overflow");
		}
		else
		{
			bytes.push_back(static_cast<int>(value) / 256);
			bytes.push_back(static_cast<int>(value) % 256);
		}
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
			throw NotImplementedException(Format(U"not supported type code: {}", Enum<TypeCode>::ToString(typeCode)));
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
		AppendUShort(static_cast<int>(u8str.size()));
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
		int classCount = 0;
		int moduleCount = 0;
		for (auto pkg : project.packages)
		{
			for (auto classInfo : pkg->classes.values)
			{
				classCount = std::max(classCount, *classInfo->index);
			}
			for (auto moduleInfo : pkg->modules)
			{
				moduleCount = std::max(moduleCount, *moduleInfo->index);
			}
		}
		classCount = classCount + 1;
		moduleCount = moduleCount + 1;

		std::vector<std::shared_ptr<ClassInfo>> classes(classCount);
		std::vector<std::shared_ptr<ModuleInfo>> modules(moduleCount);

		for (auto pkg : project.packages)
		{
			for (auto classInfo : pkg->classes.values)
			{
				classes.at(*classInfo->index) = classInfo;
			}
			for (auto moduleInfo : pkg->modules)
			{
				modules.at(*moduleInfo->index) = moduleInfo;
			}
		}

		ByteCode byteCode;
		CompileMainFunction(modules, byteCode);

		byteCode.AppendUShort(static_cast<int>(classes.size()));
		byteCode.AppendUShort(static_cast<int>(modules.size()));

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
	void Compiler::CompileBinary(std::shared_ptr<BinaryExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		CompileExpression(node->left, constantMap, byteCode);
		CompileExpression(node->right, constantMap, byteCode);
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
	void Compiler::CompileBlock(std::shared_ptr<BlockExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		for (auto exp : node->expressions)
		{
			CompileExpression(exp, constantMap, byteCode);
		}
	}
	void Compiler::CompileExpression(ExpPtr node, const ConstantMap& constantMap, ByteCode& byteCode)
	{
		switch (node->nodeType)
		{
		case ExpressionType::Add:
		case ExpressionType::Subtract:
		case ExpressionType::Multiply:
		case ExpressionType::Divide:
		case ExpressionType::GreaterThan:
		case ExpressionType::LessThan:
		case ExpressionType::GreaterThanOrEqual:
		case ExpressionType::LessThanOrEqual:
		case ExpressionType::Equal:
		case ExpressionType::NotEqual:
			return CompileBinary(std::static_pointer_cast<BinaryExpression>(node), constantMap, byteCode);
		case ExpressionType::Assign:
			return CompileAssign(std::static_pointer_cast<BinaryExpression>(node), constantMap, byteCode);
		case ExpressionType::Constant:
			return CompileConstant(std::static_pointer_cast<ConstantExpression>(node), constantMap, byteCode);
		case ExpressionType::Block:
			return CompileBlock(std::static_pointer_cast<BlockExpression>(node), constantMap, byteCode);
		case ExpressionType::Return:
			return CompileReturn(std::static_pointer_cast<ReturnExpression>(node), constantMap, byteCode);
		case ExpressionType::Parameter:
			return CompileParameter(std::static_pointer_cast<ParameterExpression>(node), byteCode);
		case ExpressionType::Conditional:
			return CompileConditional(
				std::static_pointer_cast<ConditionalExpression>(node), constantMap, byteCode);
		case ExpressionType::Default:
			return CompileDefault(std::static_pointer_cast<DefaultExpression>(node), byteCode);
		case ExpressionType::Invoke:
			return CompileInvocation(
				std::static_pointer_cast<InvocationExpression>(node), constantMap, byteCode);
		case ExpressionType::MemberAccess:
			return CompileMemberAccess(
				std::static_pointer_cast<MemberAccessExpression>(node), constantMap, byteCode);
		case ExpressionType::New:
			return CompileNewExpression(std::static_pointer_cast<NewExpression>(node),
				constantMap, byteCode);
		case ExpressionType::VariableDefinition:
			return CompileVarDefExpression(std::static_pointer_cast<VarDefExpression>(node),
				constantMap, byteCode);
		case ExpressionType::While:
			return CompileWhileLoop(std::static_pointer_cast<WhileExpression>(node), constantMap, byteCode);
		default:
			throw NotImplementedException(
				Format(U"not supported node type: {}", Enum<ExpressionType>::ToString(node->nodeType)));
		}
	}
	void Compiler::CompileConstant(
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
				if (auto moduleInfo = project.GetModule(PackageRoute{ U"Predef" }, U"String"))
				{
					if ((*moduleInfo)->methods.ContainsKey(U"New"))
					{
						auto& methodInfo = (*moduleInfo)->methods.GetValueByKey(U"New");
						// TO DO: call 'New' function

						byteCode.AppendOp(OpCode::PUSH_STRING);
						byteCode.AppendUShort(index);

						byteCode.AppendOp(OpCode::PUSH_FUNCTION);
						byteCode.AppendUShort(*(*moduleInfo)->index);
						byteCode.AppendUShort(*methodInfo.index);

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
					byteCode.AppendUShort(index);
					break;
				}
				case TypeCode::Int64: {
					byteCode.AppendOp(OpCode::PUSH_I64);
					byteCode.AppendUShort(index);
					break;
				}
				case TypeCode::Float32: {
					byteCode.AppendOp(OpCode::PUSH_F32);
					byteCode.AppendUShort(index);
					break;
				}
				case TypeCode::Float64: {
					byteCode.AppendOp(OpCode::PUSH_F64);
					byteCode.AppendUShort(index);
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
		byteCode.AppendUShort(info->fields.Size());
		for (const auto& field : info->fields.values)
		{
			byteCode.AppendString(field.name);
		}

		// constant pool
		CompileConstantPool(info->constantMap, byteCode);

		byteCode.AppendUShort(info->methodDefs.Size());
		for (const auto& method : info->methodDefs.values)
		{
			CompileMethodDef(method, info->constantMap, byteCode);
		}
	}
	void Compiler::CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode& byteCode)
	{
		byteCode.AppendString(info->name);
		byteCode.AppendUShort(info->fields.Size());
		// fields
		for (const auto& field : info->fields.values)
		{
			byteCode.AppendString(field.name);
		}

		// constant pool
		CompileConstantPool(info->constantMap, byteCode);

		byteCode.AppendUShort(info->methods.Size());
		for (const auto& method : info->methods.values)
		{
			CompileMethodDef(method, info->constantMap, byteCode);
		}
	}
	void Compiler::CompileMethodDef(const MethodDef & method, const ConstantMap& constantMap, ByteCode& byteCode)
	{
		std::cout << "method name: " << method.name << std::endl;
		if (method.annotations.ContainsKey(U"LibraryImport"))
		{
			byteCode.Append(1); // native function
			auto annotation = method.annotations.GetValueByKey(U"LibraryImport");
			byteCode.AppendString(method.name);
			byteCode.AppendUShort(method.parameters.size());
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
				byteCode.AppendUShort(method.parameters.size() + 1); // add 'this'
			}
			else
			{
				byteCode.AppendUShort(method.parameters.size());
			}
			byteCode.AppendUShort(static_cast<int>(method.localVariables.size()));
			ByteCode funcCode;
			CompileExpression(method.body, constantMap, funcCode);
			byteCode.AppendUShort(funcCode.Size());
			byteCode.AppendByteCode(funcCode);
		}
	}
	void Compiler::CompileParameter(std::shared_ptr<ParameterExpression> parameter,
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
			byteCode.AppendUShort(std::static_pointer_cast<ParameterLocation>(location)->offset);
			break;
		}
		case LocationType::ClassField: {
			byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
			byteCode.AppendUShort(0); /* this */
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
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
			break;
		}
		case LocationType::ModuleMethod: {
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
			break;
		}
		case LocationType::ClassMethod: {
			byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
			byteCode.AppendUShort(0); /* this */
			byteCode.AppendOp(OpCode::PUSH_METHOD);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
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
	void Compiler::CompileReturn(std::shared_ptr<ReturnExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
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
	void Compiler::CompileConditional(std::shared_ptr<ConditionalExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->condition, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
		int index1 = byteCode.Size();
		byteCode.AppendUShort(0);

		CompileExpression(node->ifTrue, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP);
		int index2 = byteCode.Size();
		byteCode.AppendUShort(0);

		int target1 = byteCode.Size();
		CompileExpression(node->ifFalse, constantMap, byteCode);
		int target2 = byteCode.Size();

		byteCode.WriteUShort(index1, target1);
		byteCode.WriteUShort(index2, target2);
	}
	void Compiler::CompileDefault(std::shared_ptr<DefaultExpression> node,
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
	void Compiler::CompileInvocation(std::shared_ptr<InvocationExpression> node,
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
					CompileExpression(arg.value, constantMap, byteCode);
				}
				CompileExpression(node->expression, constantMap, byteCode);
				byteCode.AppendOp(OpCode::INVOKE);
			}
		}
		else if (node->expression->type->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(node->expression->type);
			if (node->arguments.size() == 1)
			{
				CompileExpression(node->expression, constantMap, byteCode);
				for (auto arg : node->arguments)
				{
					CompileExpression(arg.value, constantMap, byteCode);
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
	void Compiler::CompileMemberAccess(std::shared_ptr<MemberAccessExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->object, constantMap, byteCode);
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
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else if (location->type == LocationType::ModuleMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
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
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else if (location->type == LocationType::ClassMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_METHOD);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
			byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
		}
		else
		{
			throw CompilerException(node->position,
				Format(U"member access: member '{}' parameter location type '{}' is not supported",
					node->field, Enum<LocationType>::ToString(location->type)));
		}
	}
	void Compiler::CompileNewExpression(std::shared_ptr<NewExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		byteCode.AppendOp(OpCode::NEW);
		byteCode.AppendUShort(std::static_pointer_cast<TypeLocation>(node->location)->index);
		std::unordered_set<std::u32string> initializedFields;
		for (auto arg : node->arguments)
		{
			if (arg.name)
			{
				initializedFields.insert(*arg.name);
			}
		}
		auto classType = std::static_pointer_cast<ClassType>(node->type);
		if (auto res = project.GetClass(classType->route, classType->name))
		{
			auto classInfo = *res;
			for (auto& field : classInfo->fields)
			{
				// not found
				if (initializedFields.find(field.name) == initializedFields.end())
				{
					byteCode.AppendOp(OpCode::DUPLICATE);
					CompileExpression(field.value, constantMap, byteCode);
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
						byteCode.AppendUShort(*field.index);
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
				CompileExpression(arg.value, constantMap, byteCode);
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
					byteCode.AppendUShort(*arg.index);
				}
				else
				{
					throw CompilerException(node->position,
						Format(U"field '{}' index not assigned", *arg.name));
				}
			}
		}
	}
	void Compiler::CompileVarDefExpression(std::shared_ptr<VarDefExpression> node,
		const ConstantMap & constantMap, ByteCode & byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
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
		case TypeCode::Class: {
			byteCode.AppendOp(OpCode::POP_LOCAL_OBJECT);
			break;
		}
		default: {
			throw CompilerException(node->position, U"not supported member access type");
		}
		}
		if (node->variable->location->type == LocationType::LocalVariable)
		{
			byteCode.AppendUShort(std::static_pointer_cast<ParameterLocation>(node->variable->location)->offset);
		}
		else
		{
			throw CompilerException(node->position, U"cannot compile non-local variable definition");
		}
	}
	void Compiler::CompileAssign(std::shared_ptr<BinaryExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		CompileExpression(node->right, constantMap, byteCode);

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
				byteCode.AppendUShort(std::static_pointer_cast<ParameterLocation>(parameter->location)->offset);
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
			CompileExpression(memberAccess->object, constantMap, byteCode);
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
				byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->index);
				byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
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
				byteCode.AppendUShort(std::static_pointer_cast<MemberLocation>(location)->offset);
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
	void Compiler::CompileWhileLoop(std::shared_ptr<WhileExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		int index1 = byteCode.Size();
		CompileExpression(node->condition, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
		int index2 = byteCode.Size();
		byteCode.AppendUShort(0);

		CompileExpression(node->body, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP);
		byteCode.AppendUShort(index1);
		int target2 = byteCode.Size();
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
				int index = moduleInfo->methods.GetIndexByKey(U"Main");
				if (found)
				{
					throw CompilerException(method.position, U"multiple definitions of the Main function");
				}
				else
				{
					byteCode.AppendUShort(*moduleInfo->index);
					byteCode.AppendUShort(index);
					found = true;
				}
			}
		}
		if (found == false)
		{
			throw CompilerException(SourcePosition(), U"cannot find Main function");
		}
	}
	void Compiler::CompileConstantPool(const ConstantMap & constantMap, ByteCode& byteCode)
	{
		byteCode.AppendUShort(static_cast<uint32_t>(constantMap.size()));
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
} // namespace cygni