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
		bytes.push_back(value % 256);
		bytes.push_back(value / 256);
	}
	void ByteCode::WriteUShort(int index, int value)
	{
		bytes.at(index) = (value % 256);
		bytes.at(index + 1) = (value / 256);
	}
	void ByteCode::AppendUInt(uint32_t value)
	{
		for (int i = 0; i < 4; i++)
		{
			bytes.push_back(value % 256);
			value = value / 256;
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
		if (node->nodeType == ExpressionType::Assign)
		{
			CompileAssign(node, constantMap, byteCode);
			return;
		}
		else
		{
			CompileExpression(node->left, constantMap, byteCode);
			CompileExpression(node->right, constantMap, byteCode);
			switch (node->nodeType)
			{
			case ExpressionType::Add: {
				byteCode.AppendOp(OpCode::ADD);
				break;
			}
			case ExpressionType::Subtract: {
				byteCode.AppendOp(OpCode::SUB);
				break;
			}
			case ExpressionType::Multiply: {
				byteCode.AppendOp(OpCode::MUL);
				break;
			}
			case ExpressionType::Divide: {
				byteCode.AppendOp(OpCode::DIV);
				break;
			}
			case ExpressionType::GreaterThan: {
				byteCode.AppendOp(OpCode::GT);
				break;
			}
			case ExpressionType::LessThan: {
				byteCode.AppendOp(OpCode::LT);
				break;
			}
			case ExpressionType::GreaterThanOrEqual: {
				byteCode.AppendOp(OpCode::GE);
				break;
			}
			case ExpressionType::LessThanOrEqual: {
				byteCode.AppendOp(OpCode::LE);
				break;
			}
			case ExpressionType::Equal: {
				byteCode.AppendOp(OpCode::EQ);
				break;
			}
			case ExpressionType::NotEqual: {
				byteCode.AppendOp(OpCode::NE);
				break;
			}
			default:
				throw NotImplementedException(
					Format(U"not supported binary operation: {}", Enum<ExpressionType>::ToString(node->nodeType)));
			}
			byteCode.AppendType(node->type);
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
		case ExpressionType::Assign:
			return CompileBinary(std::static_pointer_cast<BinaryExpression>(node), constantMap, byteCode);
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
		ConstantKey key{ node->type->typeCode, node->constant };
		if (constantMap.find(key) != constantMap.end())
		{
			int index = constantMap.at(key);
			byteCode.AppendOp(OpCode::PUSH_CONSTANT);
			byteCode.AppendType(node->type);
			byteCode.AppendUShort(index);
		}
		else
		{
			throw CompilerException(node->location,
				U"the given constant is not in the constant map");
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
		byteCode.AppendUShort(info->methods.Size());
		for (const auto& method : info->methods.values)
		{
			byteCode.AppendString(method.name);
			byteCode.AppendUShort(static_cast<int>(method.parameters.size()));
			byteCode.AppendUShort(static_cast<int>(method.localVariables.size()));
			auto funcCode = CompileMethodDef(method);
			byteCode.AppendUShort(funcCode.Size());
			byteCode.AppendByteCode(funcCode);
		}
	}
	void Compiler::CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode& byteCode)
	{
		byteCode.AppendString(info->name);
		byteCode.AppendUShort(info->fields.Size());
		for (const auto& field : info->fields.values)
		{
			byteCode.AppendString(field.name);
		}
		byteCode.AppendUShort(info->methods.Size());
		for (const auto& method : info->methods.values)
		{
			byteCode.AppendString(method.name);
			byteCode.AppendUShort(static_cast<int>(method.parameters.size()));
			byteCode.AppendUShort(static_cast<int>(method.localVariables.size()));
			auto funcCode = CompileMethodDef(method);
			byteCode.AppendUShort(static_cast<int>(funcCode.Size()));
			byteCode.AppendByteCode(funcCode);
		}
	}
	ByteCode Compiler::CompileMethodDef(const MethodDef & method)
	{
		ByteCode byteCode;
		CompileExpression(method.body, method.constantMap, byteCode);
		return byteCode;
	}
	void Compiler::CompileParameter(std::shared_ptr<ParameterExpression> parameter,
		ByteCode& byteCode)
	{
		auto location = parameter->parameterLocation;
		switch (location.type)
		{
		case ParameterType::LocalVariable: {
			byteCode.AppendOp(OpCode::PUSH_STACK);
			byteCode.AppendType(parameter->type);
			byteCode.AppendUShort(location.offset);
			break;
		}
		case ParameterType::ModuleMethod: {
			byteCode.AppendOp(OpCode::PUSH_MODULE);
			byteCode.AppendUShort(parameter->parameterLocation.index);
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(parameter->parameterLocation.offset);
			break;
		}
		case ParameterType::ModuleName: {
			byteCode.AppendOp(OpCode::PUSH_MODULE);
			byteCode.AppendUShort(parameter->parameterLocation.offset);
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
		byteCode.AppendOp(OpCode::RETURN);
		byteCode.AppendType(node->type);
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
			byteCode.AppendOp(OpCode::PUSH_FIELD);
			byteCode.AppendType(node->type);
			byteCode.AppendUShort(location.offset);
		}
		else if (location.type == ParameterType::ModuleMethod)
		{
			byteCode.AppendOp(OpCode::PUSH_FUNCTION);
			byteCode.AppendUShort(location.offset);
		}
		else if (location.type == ParameterType::ClassField)
		{
			byteCode.AppendOp(OpCode::PUSH_FIELD);
			byteCode.AppendType(node->type);
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
		for (auto arg : node->arguments)
		{
			CompileExpression(arg.value, constantMap, byteCode);
		}
		byteCode.AppendOp(OpCode::NEW);
		byteCode.AppendUShort(node->parameterLocation.offset);
	}
	void Compiler::CompileVarDefExpression(std::shared_ptr<VarDefExpression> node,
		const ConstantMap & constantMap, ByteCode & byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
		byteCode.AppendOp(OpCode::POP_STACK);
		byteCode.AppendType(node->type);
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
				byteCode.AppendOp(OpCode::PUSH_STACK);
				byteCode.AppendType(node->left->type);
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
				byteCode.AppendOp(OpCode::POP_FIELD);
				byteCode.AppendType(memberAccess->type);
				byteCode.AppendUShort(location.offset);
			}
			else if (location.type == ParameterType::ClassField)
			{
				byteCode.AppendOp(OpCode::POP_FIELD);
				byteCode.AppendType(memberAccess->type);
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
} // namespace cygni