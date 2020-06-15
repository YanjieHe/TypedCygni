#include "TypeChecker.hpp"
#include "Exception.hpp"
#include <algorithm>

namespace cygni
{
	TypeChecker::Rule::Rule(std::u32string functionName,
		std::vector<TypePtr> parameters, TypePtr returnType)
		: functionName{ functionName }, parameters{ parameters }, returnType{ returnType }
	{
	}

	void TypeChecker::RuleSet::Add(std::u32string functionName, std::vector<TypePtr> parameters, TypePtr returnType)
	{
		Rule rule{ functionName, parameters, returnType };
		if (rules.find(functionName) != rules.end())
		{
			rules[functionName].push_back(rule);
		}
		else
		{
			rules.insert({ functionName, {rule} });
		}
	}

	std::optional<TypePtr> TypeChecker::RuleSet::Match(std::u32string functionName,
		std::vector<TypePtr> parameters)
	{
		auto matchOneRule = [&parameters](const Rule &rule) -> bool
		{
			return (rule.parameters.size() == parameters.size()) && (
				std::equal(
					rule.parameters.begin(),
					rule.parameters.end(),
					parameters.begin(),
					[](TypePtr x, TypePtr y)->bool { return x->Equals(y); })
				);
		};
		if (HasKey(rules, functionName))
		{
			for (const auto &rule : rules[functionName])
			{
				if (matchOneRule(rule))
				{
					return rule.returnType;
				}
			}
			return std::optional<TypePtr>();
		}
		else
		{
			return std::optional<TypePtr>();
		}
	}

	TypeChecker::TypeChecker(Project &project) : project{ project }, typeGraph{project.typeGraph}
	{

		ruleSet.Add(U"+", { Type::Int32(), Type::Int32() }, Type::Int32());
		ruleSet.Add(U"+", { Type::Int64(), Type::Int64() }, Type::Int64());
		ruleSet.Add(U"+", { Type::Float32(), Type::Float32() }, Type::Float32());
		ruleSet.Add(U"+", { Type::Float64(), Type::Float64() }, Type::Float64());

		ruleSet.Add(U"-", { Type::Int32(), Type::Int32() }, Type::Int32());
		ruleSet.Add(U"-", { Type::Int64(), Type::Int64() }, Type::Int64());
		ruleSet.Add(U"-", { Type::Float32(), Type::Float32() }, Type::Float32());
		ruleSet.Add(U"-", { Type::Float64(), Type::Float64() }, Type::Float64());

		ruleSet.Add(U"*", { Type::Int32(), Type::Int32() }, Type::Int32());
		ruleSet.Add(U"*", { Type::Int64(), Type::Int64() }, Type::Int64());
		ruleSet.Add(U"*", { Type::Float32(), Type::Float32() }, Type::Float32());
		ruleSet.Add(U"*", { Type::Float64(), Type::Float64() }, Type::Float64());

		ruleSet.Add(U"/", { Type::Int32(), Type::Int32() }, Type::Int32());
		ruleSet.Add(U"/", { Type::Int64(), Type::Int64() }, Type::Int64());
		ruleSet.Add(U"/", { Type::Float32(), Type::Float32() }, Type::Float32());
		ruleSet.Add(U"/", { Type::Float64(), Type::Float64() }, Type::Float64());

		ruleSet.Add(U">", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U">", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U">", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U">", { Type::Float64(), Type::Float64() }, Type::Boolean());

		ruleSet.Add(U"<", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U"<", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U"<", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U"<", { Type::Float64(), Type::Float64() }, Type::Boolean());

		ruleSet.Add(U">=", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U">=", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U">=", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U">=", { Type::Float64(), Type::Float64() }, Type::Boolean());

		ruleSet.Add(U"<=", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U"<=", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U"<=", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U"<=", { Type::Float64(), Type::Float64() }, Type::Boolean());

		ruleSet.Add(U"==", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U"==", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U"==", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U"==", { Type::Float64(), Type::Float64() }, Type::Boolean());
		ruleSet.Add(U"==", { Type::Char(), Type::Char() }, Type::Boolean());

		ruleSet.Add(U"!=", { Type::Int32(), Type::Int32() }, Type::Boolean());
		ruleSet.Add(U"!=", { Type::Int64(), Type::Int64() }, Type::Boolean());
		ruleSet.Add(U"!=", { Type::Float32(), Type::Float32() }, Type::Boolean());
		ruleSet.Add(U"!=", { Type::Float64(), Type::Float64() }, Type::Boolean());
		ruleSet.Add(U"!=", { Type::Char(), Type::Char() }, Type::Boolean());


		ruleSet.Add(U"+", { Type::Int32() }, Type::Int32());
		ruleSet.Add(U"+", { Type::Int64() }, Type::Int64());
		ruleSet.Add(U"+", { Type::Float32() }, Type::Float32());
		ruleSet.Add(U"+", { Type::Float64() }, Type::Float64());

		ruleSet.Add(U"-", { Type::Int32() }, Type::Int32());
		ruleSet.Add(U"-", { Type::Int64() }, Type::Int64());
		ruleSet.Add(U"-", { Type::Float32() }, Type::Float32());
		ruleSet.Add(U"-", { Type::Float64() }, Type::Float64());

		for (auto pkg : project.packages)
		{
			for (auto classInfo : pkg->classDefs)
			{
				for (auto& superClass : classInfo->superTypes)
				{
					superClass = CheckType(classInfo->position, superClass);
					auto classType = std::make_shared<ClassType>(classInfo->route, classInfo->name);
					typeGraph.AddEdge(classType, superClass);
				}
			}
			for (auto interfaceInfo : pkg->interfaceDefs)
			{
				for (auto& superInterface : interfaceInfo->superInterfaces)
				{
					auto interfaceType = std::make_shared<InterfaceType>(interfaceInfo->route, interfaceInfo->name);
					superInterface = CheckType(interfaceInfo->position, superInterface);
					typeGraph.AddEdge(interfaceType, superInterface);
				}
			}
		}
	}

	TypePtr TypeChecker::VisitUnary(std::shared_ptr<UnaryExpression> node, Scope<TypePtr>* scope)
	{
		auto operand = VisitExpression(node->operand, scope);
		auto checkUnaryOp = [this, &node, &operand](std::u32string opName) -> TypePtr
		{
			if (auto res = ruleSet.Match(opName, { operand }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: {}, operand: {}", opName, operand->ToString()));
			}
		};
		if (node->nodeType == ExpressionType::UnaryPlus)
		{
			return checkUnaryOp(U"+");
		}
		else if (node->nodeType == ExpressionType::UnaryMinus)
		{
			return checkUnaryOp(U"-");
		}
		else if (node->nodeType == ExpressionType::Convert)
		{
			TypePtr from = VisitExpression(node->operand, scope);
			TypePtr to = node->type;
			if (typeGraph.IsSubTypeof(from, to))
			{
				node->nodeType = ExpressionType::UpCast;
				return to;
			}
			else if (typeGraph.IsSuperTypeof(from, to))
			{
				node->nodeType = ExpressionType::DownCast;
				return to;
			}
			else
			{
				throw TypeException(node->position,
					Format(U"cannot convert the object from type '{}' to '{}'", from->ToString(), to->ToString()));
			}
		}
		else
		{
			throw NotImplementedException(U"not implemented unary operation");
		}
	}

	TypePtr TypeChecker::VisitBinary(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope)
	{
		auto left = VisitExpression(node->left, scope);
		auto right = VisitExpression(node->right, scope);
		auto checkBinaryOp = [this, &node, &left, &right](std::u32string opName) -> TypePtr
		{
			if (auto res = ruleSet.Match(opName, { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: {}, left: {}, right: {}", opName, left->ToString(), right->ToString()));
			}
		};
		if (node->nodeType == ExpressionType::Add)
		{
			return checkBinaryOp(U"+");
		}
		else if (node->nodeType == ExpressionType::Subtract)
		{
			return checkBinaryOp(U"-");
		}
		else if (node->nodeType == ExpressionType::Multiply)
		{
			return checkBinaryOp(U"*");
		}
		else if (node->nodeType == ExpressionType::Divide)
		{
			return checkBinaryOp(U"/");
		}
		else if (node->nodeType == ExpressionType::GreaterThan)
		{
			return checkBinaryOp(U">");
		}
		else if (node->nodeType == ExpressionType::LessThan)
		{
			return checkBinaryOp(U"<");
		}
		else if (node->nodeType == ExpressionType::GreaterThanOrEqual)
		{
			return checkBinaryOp(U">=");
		}
		else if (node->nodeType == ExpressionType::LessThanOrEqual)
		{
			return checkBinaryOp(U"<=");
		}
		else if (node->nodeType == ExpressionType::Equal)
		{
			return checkBinaryOp(U"==");
		}
		else if (node->nodeType == ExpressionType::NotEqual)
		{
			return checkBinaryOp(U"!=");
		}
		else
		{
			throw NotImplementedException(U"not implemented binary operation");
		}
	}

	TypePtr TypeChecker::VisitBlock(std::shared_ptr<BlockExpression> node, Scope<TypePtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		for (const auto &exp : node->expressions)
		{
			VisitExpression(exp, scope);
		}
		Attach(node, Type::Void());
		return Type::Void();
	}

	TypePtr TypeChecker::Attach(ExpPtr node, TypePtr type)
	{
		node->type = type;
		return type;
	}

	TypePtr TypeChecker::VisitAssign(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope)
	{
		if (node->left->nodeType == ExpressionType::Parameter)
		{
			auto left = VisitExpression(node->left, scope);
			auto right = VisitExpression(node->right, scope);
			if (left->Equals(right))
			{
				node->type = Type::Void();
				return Type::Void();
			}
			else
			{
				throw TypeException(node->position, U"type mismtach: =");
			}
		}
		else if (node->left->nodeType == ExpressionType::MemberAccess)
		{
			auto left = VisitExpression(node->left, scope);
			auto right = VisitExpression(node->right, scope);
			if (left->Equals(right))
			{
				node->type = Type::Void();
				return Type::Void();
			}
			else
			{
				throw TypeException(node->position,
					U"type mismtach: assignment to field");
			}
		}
		else
		{
			throw TypeException(node->position,
				U"cannot assign to the left expression");
		}
	}

	TypePtr TypeChecker::VisitWhile(std::shared_ptr<WhileExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr condition = VisitExpression(node->condition, scope);
		if (condition->typeCode == TypeCode::Boolean)
		{
			VisitExpression(node->body, scope);
			return Type::Void();
		}
		else
		{
			throw TypeException(node->position,
				U"while condition must be boolean type");
		}
	}

	TypePtr TypeChecker::CheckType(SourcePosition position, TypePtr type)
	{
		if (type->typeCode == TypeCode::Unresolved)
		{
			auto unresolvedType = std::static_pointer_cast<UnresolvedType>(type);
			if (auto resultType = project.ResolveType(unresolvedType))
			{
				return *resultType;
			}
			else
			{
				throw TypeException(position,
					Format(U"type '{}' not defined", unresolvedType->ToString()));
			}
		}
		else if (type->typeCode == TypeCode::Function)
		{
			auto functionType = std::static_pointer_cast<FunctionType>(type);
			std::vector<TypePtr> parameters;
			std::transform(
				functionType->parameters.begin(),
				functionType->parameters.end(),
				std::back_inserter(parameters),
				[this, position](TypePtr t) -> TypePtr { return CheckType(position, t); });
			return std::make_shared<FunctionType>(
				CheckType(position, functionType->selfType),
				functionType->name,
				parameters,
				CheckType(position, functionType->returnType));
		}
		else if (type->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(type);
			return std::make_shared<ArrayType>(
				CheckType(position, arrayType->elementType));
		}
		else
		{
			return type;
		}
	}

	void TypeChecker::CheckInterfaceConstraint(std::shared_ptr<ClassInfo> classInfo, std::shared_ptr<InterfaceInfo> interfaceInfo)
	{
		/*for (auto superClass : classInfo->superClasses)
		{
			if (superClass->typeCode == TypeCode::Interface)
			{
				auto interfaceType = std::static_pointer_cast<InterfaceType>(superClass);
				auto interfaceInfo = project.GetInterface(interfaceType);
				if (interfaceInfo)
				{
					for (const auto& method : (*interfaceInfo)->methodMap.values)
					{
						if (classInfo->methods.ContainsKey(method.name))
						{
							classInfo->virtualMethodTable[(*interfaceInfo)->name].push_back(method);
						}
						else
						{
							throw TypeException(method.position,
								Format(U"interface constraint function '{}' not implemented", method.name));
						}
					}
				}
				else
				{
					throw TypeException(classInfo->position,
						Format(U"undefined interface '{}'", interfaceType->ToString()));
				}
			}
		}*/

	}

	TypePtr TypeChecker::VisitConstant(std::shared_ptr<ConstantExpression> node, Scope<TypePtr>* outerScope)
	{
		return node->type;
	}

	TypePtr TypeChecker::CheckClassInfo(std::shared_ptr<ClassInfo> info, Scope<TypePtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		for (const auto& field : info->fields.values)
		{
			std::cout << "visit class " << info->name << " field name: " << field.name << std::endl;
			scope->Put(field.name, field.type);
		}
		for (const auto &method : info->methods.values)
		{
			scope->Put(method.name, method.signature);
		}
		for (auto &field : info->fieldDefs.values)
		{
			CheckFieldDef(field, scope);
		}
		for (auto &method : info->methodDefs.values)
		{
			CheckMethodDef(method, scope);
		}
		return Type::Void();
	}

	TypePtr TypeChecker::CheckModuleInfo(std::shared_ptr<ModuleInfo> info, Scope<TypePtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		for (const auto& field : info->fields.values)
		{
			scope->Put(field.name, field.type);
		}
		for (const auto &method : info->methods.values)
		{
			scope->Put(method.name, method.signature);
		}
		for (auto &field : info->fields.values)
		{
			CheckFieldDef(field, scope);
		}
		for (auto &method : info->methods.values)
		{
			CheckMethodDef(method, scope);
		}
		return Type::Void();
	}

	TypePtr TypeChecker::CheckFieldDef(FieldInfo &field, Scope<TypePtr>* scope)
	{
		field.type = CheckType(field.position, field.type);
		if (field.value->nodeType == ExpressionType::Default)
		{
			scope->Put(field.name, field.type);
			return field.type;
		}
		else
		{
			auto type = VisitExpression(field.value, scope);
			if (!field.type->Equals(type))
			{
				throw TypeException(field.position, U"field type mismatch");
			}
			else
			{
				scope->Put(field.name, field.type);
				return field.type;
			}
		}
	}

	TypePtr TypeChecker::CheckMethodDef(MethodInfo &method, Scope<TypePtr>* outerScope)
	{
		method.selfType = CheckType(method.position, method.selfType);
		auto scope = scopeFactory->New(outerScope);
		if (method.selfType->typeCode == TypeCode::Class)
		{
			scope->Put(U"this", method.selfType);
		}
		for (const auto &parameter : method.parameters)
		{
			scope->Put(parameter->name, parameter->type);
		}
		VisitExpression(method.body, scope);
		return method.returnType;
	}

	TypePtr TypeChecker::VisitParameter(std::shared_ptr<ParameterExpression> parameter, Scope<TypePtr>* scope)
	{
		if (auto type = scope->Get(parameter->name))
		{
			return Attach(parameter, *type);
		}
		else
		{
			throw TypeException(parameter->position, Format(U"parameter '{}' not defined", parameter->name));
		}
	}

	TypePtr TypeChecker::VisitReturn(std::shared_ptr<ReturnExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr returnType = VisitExpression(node->value, scope);
		Attach(node, returnType);
		return returnType;
	}

	TypePtr TypeChecker::VisitConditional(std::shared_ptr<ConditionalExpression> node, Scope<TypePtr>* scope)
	{
		auto condition = VisitExpression(node->condition, scope);
		auto ifTrue = VisitExpression(node->ifTrue, scope);
		auto ifFalse = VisitExpression(node->ifFalse, scope);
		if (condition->Equals(Type::Boolean()))
		{
			Attach(node, Type::Void());
			return Type::Void();
		}
		else
		{
			throw TypeException(node->condition->position, U"condition type must be boolean");
		}
	}

	TypePtr TypeChecker::VisitDefault(std::shared_ptr<DefaultExpression> node, Scope<TypePtr>* outerScope)
	{
		return node->type;
	}

	TypePtr TypeChecker::VisitInvocation(std::shared_ptr<InvocationExpression> node, Scope<TypePtr>* scope)
	{
		auto exp = VisitExpression(node->expression, scope);
		// TO DO: named arguments
		std::vector<TypePtr> args(node->arguments.size());
		std::transform(node->arguments.begin(), node->arguments.end(), args.begin(),
			[this, &scope](const Argument &arg) -> TypePtr { return VisitExpression(arg.value, scope); });
		if (exp->typeCode == TypeCode::Function)
		{
			auto functionType = std::static_pointer_cast<FunctionType>(exp);
			if (functionType->Match(args))
			{
				return Attach(node, functionType->returnType);
			}
			else
			{
				throw TypeException(node->position, U"function call argument(s)' type(s) do not match");
			}
		}
		else if (exp->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(exp);
			if (args.size() == 1)
			{
				if (args.at(0)->typeCode == TypeCode::Int32)
				{
					return Attach(node, arrayType->elementType);
				}
				else
				{
					throw TypeException(node->position, U"array index should be 'Int' type");
				}
			}
			else
			{
				throw TypeException(node->position, U"array should be accessed by one index");
			}
		}
		else
		{
			throw TypeException(node->position, U"expression is not a function");
		}
	}

	TypePtr TypeChecker::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr object = VisitExpression(node->object, scope);
		if (object->typeCode == TypeCode::Module)
		{
			auto moduleType = std::static_pointer_cast<ModuleType>(object);
			if (auto moduleInfo = project.GetModule(moduleType))
			{
				if ((*moduleInfo)->fields.ContainsKey(node->field))
				{
					auto& field = (*moduleInfo)->fields.GetValueByKey(node->field);
					return Attach(node, field.type);
				}
				else if ((*moduleInfo)->methods.ContainsKey(node->field))
				{
					auto& method = (*moduleInfo)->methods.GetValueByKey(node->field);
					return Attach(node, method.signature);
				}
				else
				{
					throw TypeException(node->position,
						Format(U"undefined field '{}' in module '{}'", node->field, moduleType->name));
				}
			}
			else
			{

				throw TypeException(node->position,
					Format(U"undefined module '{}'", object->ToString()));
			}
		}
		else if (object->typeCode == TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<ClassType>(object);
			if (auto classInfo = project.GetClass(classType))
			{
				if ((*classInfo)->fields.ContainsKey(node->field))
				{
					auto& field = (*classInfo)->fields.GetValueByKey(node->field);
					return Attach(node, field.type);
				}
				else if ((*classInfo)->methods.ContainsKey(node->field))
				{
					auto& method = (*classInfo)->methods.GetValueByKey(node->field);
					return Attach(node, method.signature);
				}
				else
				{
					throw TypeException(node->position, Format(U"undefined field '{}'", node->field));
				}
			}
			else
			{
				throw TypeException(node->position,
					Format(U"undefined class '{}'", object->ToString()));
			}
		}
		else if (object->typeCode == TypeCode::Interface)
		{
			auto interfaceType = std::static_pointer_cast<InterfaceType>(object);
			if (auto interfaceInfo = project.GetInterface(interfaceType))
			{
				if ((*interfaceInfo)->methodDefs.ContainsKey(node->field))
				{
					auto& method = (*interfaceInfo)->methodDefs.GetValueByKey(node->field);
					return Attach(node, method.signature);
				}
				else
				{
					throw TypeException(node->position, Format(U"undefined field '{}'", node->field));
				}
			}
			else
			{
				throw TypeException(node->position,
					Format(U"undefined interface '{}'", object->ToString()));
			}
		}
		else if (object->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(object);
			if (node->field == U"Size" || node->field == U"Length")
			{
				return Attach(node, Type::Int32());
			}
			else
			{
				throw TypeException(node->position, Format(U"undefined field '{}' of array", node->field));
			}
		}
		else
		{
			throw TypeException(node->position,
				Format(U"object '{}' does not have any field", object->ToString()));
		}
	}

	TypePtr TypeChecker::VisitNewExpression(std::shared_ptr<NewExpression> node, Scope<TypePtr>* scope)
	{
		node->type = CheckType(node->position, node->type);
		if (node->type->typeCode == TypeCode::Class)
		{
			auto newExpType = std::static_pointer_cast<ClassType>(node->type);
			if (auto res = project.GetClass(newExpType))
			{
				auto classInfo = *res;
				for (const auto& argument : node->arguments)
				{
					if (argument.name)
					{
						if (classInfo->fields.ContainsKey(*argument.name))
						{
							auto& field = classInfo->fields.GetValueByKey(*(argument.name));
							auto value = VisitExpression(argument.value, scope);
							if (field.type->Equals(value))
							{
								// pass
							}
							else
							{
								throw TypeException(node->position,
									Format(U"field '{}' initialization type does not match", *(argument.name)));
							}
						}
						else
						{
							throw TypeException(node->position,
								Format(U"field initialization name '{}' not found", *(argument.name)));
						}
					}
					else
					{
						throw TypeException(node->position, U"field initialization name not specified");
					}
				}
				node->location = std::make_shared<TypeLocation>(LocationType::ClassName, *classInfo->index);
				return node->type;
			}
			else
			{
				throw TypeException(node->position,
					Format(U"error new expression: undefined class '{}'", newExpType->ToString()));
			}
		}
		else
		{
			throw TypeException(node->position,
				Format(U"new expression expects a class type, not '{}'", node->type->ToString()));
		}
	}

	TypePtr TypeChecker::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr value = VisitExpression(node->value, scope);
		node->variable->type = CheckType(node->position, node->variable->type);
		if (node->variable->type->typeCode == TypeCode::Unknown)
		{
			node->variable->type = value;
			scope->Put(node->variable->name, node->variable->type);
			return  Attach(node, value);
		}
		else if (node->variable->type->Equals(value))
		{
			scope->Put(node->variable->name, node->variable->type);
			return Attach(node, node->variable->type);
		}
		else if (typeGraph.IsSubTypeof(value, node->variable->type))
		{
			// add type conversion
			auto convertExp = std::make_shared<UnaryExpression>(node->position, ExpressionType::UpCast, node->value);
			node->value = convertExp;
			node->value->type = node->variable->type;
			scope->Put(node->variable->name, node->value->type);
			return Attach(node, node->variable->type);
		}
		else
		{
			throw TypeException(node->position,
				Format(U"variable initialization type mismatch, variable: '{}', initialization: '{}'",
					node->variable->type->ToString(), node->value->type->ToString()));
		}
	}
	void TypeChecker::CheckPackage(Scope<TypePtr>* globalScope)
	{
		std::unordered_set<PackageRoute> visited;
		auto scope = scopeFactory->New(globalScope);
		for (const auto &moduleInfo : package->modules)
		{
			TypePtr moduleType = std::make_shared<ModuleType>(package->route, moduleInfo->name);
			scope->Put(moduleInfo->name, moduleType);
		}
		for (const auto &classInfo : package->classes)
		{
			CheckClassInfo(classInfo, scope);
		}
		for (const auto &moduleInfo : package->modules)
		{
			CheckModuleInfo(moduleInfo, scope);
		}
		for (const auto &classInfo : package->classDefs)
		{
			auto classType = std::make_shared<ClassType>(classInfo->route, classInfo->name);
			classInfo->inheritanceChain = typeGraph.InheritanceChain(classType);
			classInfo->interfaceList = typeGraph.GetAllInterfaces(classType);
			auto getIndex = [this, classInfo](std::shared_ptr<InterfaceType> interfaceType) -> int
			{
				if (auto interfaceInfo = project.GetInterface(interfaceType))
				{
					return *(*interfaceInfo)->index;
				}
				else
				{
					throw CompilerException(classInfo->position,
						Format(U"missing interface {} for class {}", interfaceType->ToString(), classInfo->name));
				}
			};
			std::sort(
				classInfo->interfaceList.begin(),
				classInfo->interfaceList.end(),
				[this, &getIndex, classInfo](std::shared_ptr<InterfaceType>&x, std::shared_ptr<InterfaceType>& y) -> bool
			{
				int ix = getIndex(x);
				int iy = getIndex(y);
				return ix < iy;
			});
		}
		for (auto& interfaceInfo : package->interfaceDefs)
		{
			auto interfaceType = std::make_shared<InterfaceType>(interfaceInfo->route, interfaceInfo->name);
			interfaceInfo->allSuperInterfaces = typeGraph.GetAllSuperInterfaces(interfaceType);
			for (auto superInterfaceType : interfaceInfo->allSuperInterfaces)
			{
				if (auto superInterfaceInfo = project.GetInterface(superInterfaceType))
				{
					for (auto method : (*superInterfaceInfo)->methodDefs)
					{
						interfaceInfo->allMethods.push_back(method);
					}
				}
				else
				{
					throw CompilerException(interfaceInfo->position,
						Format(U"missing interface {} for interface {}", superInterfaceType->ToString(), interfaceInfo->name));
				}
			}
		}
	}

	void TypeChecker::CheckProject(Scope<TypePtr>* globalScope)
	{
		auto scope = scopeFactory->New(globalScope);
		for (auto pkg : project.packages)
		{
			this->package = pkg;
			CheckPackage(scope);
		}
	}
} // namespace cygni