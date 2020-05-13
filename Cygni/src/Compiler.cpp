#include "Compiler.hpp"
#include "Exception.hpp"

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
	void ByteCode::AppendUShort(int value)
	{
		bytes.push_back(value / 256);
		bytes.push_back(value % 256);
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
			throw NotImplementedException(Format(U"not supported type code: {}", Enum<TypeCode>::ToString(type->typeCode)));
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
		for (auto pair : project.packages)
		{
			auto pkg = pair.second;
			classCount = classCount + static_cast<int>(pkg->classes.values.size());
			moduleCount = moduleCount + static_cast<int>(pkg->modules.values.size());
		}

		std::vector<std::shared_ptr<ClassInfo>> classes(classCount);
		std::vector<std::shared_ptr<ModuleInfo>> modules(moduleCount);

		for (auto pair : project.packages)
		{
			auto pkg = pair.second;
			for (auto _class : pkg->classes.values)
			{
				classes.at(_class->index) = _class;
			}
			for (auto module : pkg->modules.values)
			{
				modules.at(module->index) = module;
			}
		}

		ByteCode byteCode;

		CompileMainFunction(modules, byteCode);

		byteCode.AppendUShort(static_cast<int>(classes.size()));
		byteCode.AppendUShort(static_cast<int>(modules.size()));

		for (auto _class : classes)
		{
			CompileClassInfo(_class, byteCode);
		}

		for (auto module : modules)
		{
			CompileModuleInfo(module, byteCode);
		}

		return byteCode;
	}
	void Compiler::CompileBinary(std::shared_ptr<BinaryExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		CompileExpression(node->left, constantMap, byteCode);
		CompileExpression(node->right, constantMap, byteCode);
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
				throw CompilerException(node->location, U"not supported type for addition");
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
				throw CompilerException(node->location, U"not supported type for subtraction");
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
				throw CompilerException(node->location, U"not supported type for multiplication");
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
				throw CompilerException(node->location, U"not supported type for division");
			}
			}
			break;
		}
		case ExpressionType::GreaterThan: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::GT_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::GT_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::GT_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::GT_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '>'");
			}
			}
			break;
		}
		case ExpressionType::LessThan: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::LT_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::LT_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::LT_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::LT_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '<'");
			}
			}
			break;
		}
		case ExpressionType::GreaterThanOrEqual: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::GE_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::GE_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::GE_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::GE_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '>='");
			}
			}
			break;
		}
		case ExpressionType::LessThanOrEqual: {
			switch (node->type->typeCode)
			{
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::LE_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::LE_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::LE_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::LE_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '<='");
			}
			}
			break;
		}
		case ExpressionType::Equal: {
			switch (node->type->typeCode)
			{
			case TypeCode::Boolean:
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::EQ_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::EQ_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::EQ_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::EQ_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '=='");
			}
			}
			break;
		}
		case ExpressionType::NotEqual: {
			switch (node->type->typeCode)
			{
			case TypeCode::Boolean:
			case TypeCode::Int32: {
				byteCode.AppendOp(OpCode::NE_I32);
				break;
			}
			case TypeCode::Int64: {
				byteCode.AppendOp(OpCode::NE_I64);
				break;
			}
			case TypeCode::Float32: {
				byteCode.AppendOp(OpCode::NE_F32);
				break;
			}
			case TypeCode::Float64: {
				byteCode.AppendOp(OpCode::NE_F64);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported type for '!='");
			}
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
			//return VisitWhileLoop(std::static_pointer_cast<WhileExpression>(node), constantMap, byteCode);
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
				throw CompilerException(node->location,
					U"illegal format of boolean value. should be 'true' or 'false'");
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
				case TypeCode::String: {
					byteCode.AppendOp(OpCode::PUSH_STRING);
					byteCode.AppendUShort(index);
					break;
				}
				default: {
					throw CompilerException(node->location, U"constant type not supported");
				}
				}
			}
			else
			{
				throw CompilerException(node->location,
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

		byteCode.AppendUShort(info->methods.Size());
		for (const auto& method : info->methods.values)
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
			byteCode.AppendUShort(static_cast<int>(method.parameters.size()));
			auto libName = std::static_pointer_cast<ConstantExpression>(annotation.arguments.at(0).value)->constant;
			auto funcName = std::static_pointer_cast<ConstantExpression>(annotation.arguments.at(1).value)->constant;
			byteCode.AppendString(libName);
			byteCode.AppendString(funcName);
		}
		else
		{
			byteCode.Append(0); // user-defined function
			byteCode.AppendString(method.name);
			byteCode.AppendUShort(static_cast<int>(method.parameters.size()));
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
		auto location = parameter->parameterLocation;
		switch (location.type)
		{
		case ParameterType::LocalVariable: {
			switch (parameter->type->typeCode)
			{
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
			case TypeCode::Class: {
				byteCode.AppendOp(OpCode::PUSH_LOCAL_OBJECT);
				break;
			}
			default: {
				throw CompilerException(parameter->location, U"not supported parameter type");
			}
			}
			byteCode.AppendUShort(location.offset);
			break;
		}
		case ParameterType::ModuleMethod: {
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(parameter->parameterLocation.index);
			byteCode.AppendUShort(parameter->parameterLocation.offset);
			break;
		}
		case ParameterType::ModuleName: {
			std::cout << "module name: " << parameter->name << std::endl;
			break;
		}
		default:
			throw NotImplementedException(
				Format(U"parameter: {}", Enum<ParameterType>::ToString(location.type)));
		}
	}
	void Compiler::CompileReturn(std::shared_ptr<ReturnExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
		switch (node->type->typeCode)
		{
		case TypeCode::Boolean:
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
			throw CompilerException(node->location, U"not supported return type");
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
		case TypeCode::Void: {
			break;
		}
		default: {
			throw CompilerException(node->location,
				U"not supported type of default expression");
		}
		}
	}
	void Compiler::CompileInvocation(std::shared_ptr<InvocationExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		for (auto arg : node->arguments)
		{
			CompileExpression(arg.value, constantMap, byteCode);
		}
		CompileExpression(node->expression, constantMap, byteCode);
		byteCode.AppendOp(OpCode::INVOKE);
	}
	void Compiler::CompileMemberAccess(std::shared_ptr<MemberAccessExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->object, constantMap, byteCode);
		auto location = node->parameterLocation;
		if (location.type == ParameterType::ModuleField)
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
				throw CompilerException(node->location, U"not supported member access type");
			}
			}
			byteCode.AppendUShort(location.index);
			byteCode.AppendUShort(location.offset);
		}
		else if (location.type == ParameterType::ModuleMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(location.index);
			byteCode.AppendUShort(location.offset);
		}
		else if (location.type == ParameterType::ClassField)
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
			case TypeCode::Class: {
				byteCode.AppendOp(OpCode::PUSH_FIELD_OBJECT);
				break;
			}
			default: {
				throw CompilerException(node->location, U"not supported member access type");
			}
			}
			byteCode.AppendUShort(location.offset);
		}
		else if (location.type == ParameterType::ClassMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_METHOD);
			byteCode.AppendUShort(location.offset);
		}
		else
		{
			throw CompilerException(node->location, U"member access: parameter location type is not supported");
		}
	}
	void Compiler::CompileNewExpression(std::shared_ptr<NewExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		byteCode.AppendOp(OpCode::NEW);
		byteCode.AppendUShort(node->parameterLocation.offset);
		for (auto arg : node->arguments)
		{
			CompileExpression(arg.value, constantMap, byteCode);
			byteCode.AppendOp(OpCode::DUPLICATE_OFFSET);
			byteCode.AppendUShort(1); // get the initialized class object
			switch (node->type->typeCode)
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
				throw CompilerException(node->location, U"not supported field initializer type");
			}
			}
			byteCode.AppendUShort(arg.index);
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
			throw CompilerException(node->location, U"not supported member access type");
		}
		}
		if (node->variable->parameterLocation.type == ParameterType::LocalVariable)
		{
			byteCode.AppendUShort(node->variable->parameterLocation.offset);
		}
		else
		{
			throw CompilerException(node->location, U"cannot compile non-local variable definition");
		}
	}
	void Compiler::CompileAssign(std::shared_ptr<BinaryExpression> node, const ConstantMap & constantMap, ByteCode & byteCode)
	{
		CompileExpression(node->right, constantMap, byteCode);

		if (node->left->nodeType == ExpressionType::Parameter)
		{
			auto parameter = std::static_pointer_cast<ParameterExpression>(node->left);
			if (parameter->parameterLocation.type == ParameterType::LocalVariable)
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
					throw CompilerException(node->location, U"not supported assignment type");
				}
				}
				byteCode.AppendUShort(parameter->parameterLocation.offset);
			}
			else
			{
				throw CompilerException(node->location, U"cannot compile non-local variable definition");
			}
		}
		else if (node->left->nodeType == ExpressionType::MemberAccess)
		{
			auto memberAccess = std::static_pointer_cast<MemberAccessExpression>(node->left);
			auto location = memberAccess->parameterLocation;
			CompileExpression(memberAccess->object, constantMap, byteCode);
			if (location.type == ParameterType::ModuleField)
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
					throw CompilerException(node->location, U"not supported module field type");
				}
				}
				byteCode.AppendUShort(location.index);
				byteCode.AppendUShort(location.offset);
			}
			else if (location.type == ParameterType::ClassField)
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
					throw CompilerException(node->location, U"not supported class field type");
				}
				}
				byteCode.AppendUShort(location.offset);
			}
			else
			{
				throw CompilerException(memberAccess->location, U"member access: parameter location type is not supported");
			}
		}
		else
		{
			throw CompilerException(node->location, U"wrong assignment");
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
		for (auto module : modules)
		{
			if (module->methods.ContainsKey(U"Main"))
			{
				auto method = module->methods.GetValueByKey(U"Main");
				if (found)
				{
					throw CompilerException(method.location, U"multiple definitions of the Main function");
				}
				else
				{
					byteCode.AppendUShort(module->index);
					byteCode.AppendUShort(method.index);
					found = true;
				}
			}
		}
		if (found == false)
		{
			throw CompilerException(SourceLocation(), U"cannot find Main function");
		}
	}
	void Compiler::CompileConstantPool(const ConstantMap & constantMap, ByteCode& byteCode)
	{
		byteCode.AppendUShort(constantMap.size());
		std::vector<ConstantKey> constants(constantMap.size());
		for (auto pair : constantMap)
		{
			constants[pair.second] = pair.first;
		}
		for (ConstantKey constant : constants)
		{
			byteCode.AppendTypeCode(constant.typeCode);
			byteCode.AppendString(constant.constant);
		}
	}
} // namespace cygni