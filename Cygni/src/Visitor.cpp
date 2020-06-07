#include "Visitor.hpp"
#include "Exception.hpp"
#include <algorithm>
#include <stack>

using std::cout;
using std::endl;

namespace cygni
{

	json AstToJsonSerialization::VisitSourceLocation(SourcePosition location)
	{
		json obj;
		obj["startLine"] = location.startLine;
		obj["startCol"] = location.startCol;
		obj["endLine"] = location.endLine;
		obj["endCol"] = location.endCol;
		return obj;
	}

	json AstToJsonSerialization::VisitUnary(std::shared_ptr<UnaryExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["operand"] = VisitExpression(node->operand);
		return obj;
	}

	json AstToJsonSerialization::VisitBinary(std::shared_ptr<BinaryExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["left"] = VisitExpression(node->left);
		obj["right"] = VisitExpression(node->right);
		return obj;
	}

	json AstToJsonSerialization::VisitConstant(std::shared_ptr<ConstantExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["constant"] = UTF32ToUTF8(node->constant);
		obj["type"] = UTF32ToUTF8(node->type->ToString());
		return obj;
	}

	json AstToJsonSerialization::VisitClassInfo(std::shared_ptr<ClassInfo> info)
	{
		json obj;
		obj["nodeType"] = "class";
		obj["name"] = UTF32ToUTF8(info->name);

		json fieldsJson(std::unordered_map<std::string, json>{});
		for (const auto &field : info->fieldDefs.values)
		{
			std::string name = UTF32ToUTF8(field.name);
			fieldsJson[name] = VisitFieldDef(field);
		}
		obj["fields"] = fieldsJson;

		json methodsJson(std::unordered_map<std::string, json>{});
		for (const auto &method : info->methodDefs.values)
		{
			std::string name = UTF32ToUTF8(method.name);
			methodsJson[name] = VisitMethodDef(method);
		}
		obj["methods"] = methodsJson;
		return obj;
	}

	json AstToJsonSerialization::VisitModuleInfo(std::shared_ptr<ModuleInfo> info)
	{
		json obj;
		obj["nodeType"] = "module";
		obj["name"] = UTF32ToUTF8(info->name);

		json fieldsJson(std::unordered_map<std::string, json>{});
		for (const auto &field : info->fields.values)
		{
			std::string name = UTF32ToUTF8(field.name);
			fieldsJson[name] = VisitFieldDef(field);
		}
		obj["fields"] = fieldsJson;

		json methodsJson(std::unordered_map<std::string, json>{});
		for (const auto &method : info->methods.values)
		{
			std::string name = UTF32ToUTF8(method.name);
			methodsJson[name] = VisitMethodDef(method);
		}
		obj["methods"] = methodsJson;
		return obj;
	}

	json AstToJsonSerialization::VisitFieldDef(const FieldDef &field)
	{
		json obj;
		obj["isStatic"] = field.isStatic;
		obj["position"] = VisitSourceLocation(field.position);
		obj["accessModifier"] =
			UTF32ToUTF8(Enum<AccessModifier>::ToString(field.modifier));
		obj["annotations"] = VisitAnnotationList(field.annotations);
		obj["name"] = UTF32ToUTF8(field.name);
		obj["type"] = UTF32ToUTF8(field.type->ToString());
		obj["value"] = VisitExpression(field.value);
		return obj;
	}

	json AstToJsonSerialization::VisitMethodDef(const MethodDef &method)
	{
		json obj;

		obj["isStatic"] = method.isStatic;
		obj["position"] = VisitSourceLocation(method.position);
		obj["accessModifier"] =
			UTF32ToUTF8(Enum<AccessModifier>::ToString(method.modifier));
		obj["annotations"] = VisitAnnotationList(method.annotations);
		obj["name"] = UTF32ToUTF8(method.name);

		std::vector<json> parametersJson;
		for (const auto &parameter : method.parameters)
		{
			parametersJson.push_back(VisitParameter(parameter));
		}
		obj["parameters"] = parametersJson;
		obj["body"] = VisitExpression(method.body);
		obj["type"] = UTF32ToUTF8(method.signature->ToString());

		std::vector<json> localVarsJson;
		for (const auto& localVar : method.localVariables)
		{
			localVarsJson.push_back(VisitVarDefExpression(localVar));
		}
		obj["localVariables"] = localVarsJson;
		return obj;
	}

	json AstToJsonSerialization::VisitParameter(
		std::shared_ptr<ParameterExpression> parameter)
	{
		json obj;
		obj["name"] = UTF32ToUTF8(parameter->name);
		AttachNodeInformation(obj, parameter);
		obj["location"] = VisitLocation(parameter->location);
		return obj;
	}

	json AstToJsonSerialization::VisitBlock(std::shared_ptr<BlockExpression> node)
	{
		json obj;
		obj["expressions"] = VisitExpressionList(node->expressions);
		AttachNodeInformation(obj, node);
		return obj;
	}

	json AstToJsonSerialization::VisitReturn(
		std::shared_ptr<ReturnExpression> node)
	{
		json obj;
		obj["value"] = VisitExpression(node->value);
		AttachNodeInformation(obj, node);
		return obj;
	}

	json AstToJsonSerialization::VisitConditional(
		std::shared_ptr<ConditionalExpression> node)
	{
		json obj;
		obj["condition"] = VisitExpression(node->condition);
		obj["ifTrue"] = VisitExpression(node->ifTrue);
		obj["ifFalse"] = VisitExpression(node->ifFalse);
		AttachNodeInformation(obj, node);
		return obj;
	}

	json AstToJsonSerialization::VisitDefault(
		std::shared_ptr<DefaultExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		return obj;
	}

	json AstToJsonSerialization::VisitInvocation(
		std::shared_ptr<InvocationExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["expression"] = VisitExpression(node->expression);
		obj["arguments"] = VisitArgumentList(node->arguments);
		return obj;
	}

	json AstToJsonSerialization::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["object"] = VisitExpression(node->object);
		obj["fieldName"] = UTF32ToUTF8(node->field);
		return obj;
	}

	json AstToJsonSerialization::VisitNewExpression(std::shared_ptr<NewExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["arguments"] = VisitArgumentList(node->arguments);
		obj["parameterLocation"] = VisitLocation(node->location);
		return obj;
	}

	json AstToJsonSerialization::VisitVarDefExpression(
		std::shared_ptr<VarDefExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["variable"] = VisitExpression(node->variable);
		obj["value"] = VisitExpression(node->value);
		return obj;
	}

	json AstToJsonSerialization::VisitWhileLoop(
		std::shared_ptr<WhileExpression> node)
	{
		json obj;
		AttachNodeInformation(obj, node);
		obj["condition"] = VisitExpression(node->condition);
		obj["body"] = VisitExpression(node->body);
		return obj;
	}

	void AstToJsonSerialization::AttachNodeInformation(json &obj, ExpPtr node)
	{
		obj["id"] = node->id;
		obj["position"] = VisitSourceLocation(node->position);
		obj["nodeType"] = UTF32ToUTF8(Enum<ExpressionType>::ToString(node->nodeType));
		obj["type"] = UTF32ToUTF8(node->type->ToString());
	}

	json AstToJsonSerialization::VisitExpression(ExpPtr node)
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
			return VisitBinary(std::static_pointer_cast<BinaryExpression>(node));
		case ExpressionType::UnaryPlus:
		case ExpressionType::UnaryMinus:
		case ExpressionType::Convert:
			return VisitUnary(std::static_pointer_cast<UnaryExpression>(node));
		case ExpressionType::Constant:
			return VisitConstant(std::static_pointer_cast<ConstantExpression>(node));
		case ExpressionType::Block:
			return VisitBlock(std::static_pointer_cast<BlockExpression>(node));
		case ExpressionType::Return:
			return VisitReturn(std::static_pointer_cast<ReturnExpression>(node));
		case ExpressionType::Parameter:
			return VisitParameter(std::static_pointer_cast<ParameterExpression>(node));
		case ExpressionType::Conditional:
			return VisitConditional(
				std::static_pointer_cast<ConditionalExpression>(node));
		case ExpressionType::Default:
			return VisitDefault(std::static_pointer_cast<DefaultExpression>(node));
		case ExpressionType::Invoke:
			return VisitInvocation(
				std::static_pointer_cast<InvocationExpression>(node));
		case ExpressionType::MemberAccess:
			return VisitMemberAccess(
				std::static_pointer_cast<MemberAccessExpression>(node));
		case ExpressionType::New:
			return VisitNewExpression(std::static_pointer_cast<NewExpression>(node));
		case ExpressionType::VariableDefinition:
			return VisitVarDefExpression(
				std::static_pointer_cast<VarDefExpression>(node));
		case ExpressionType::While:
			return VisitWhileLoop(std::static_pointer_cast<WhileExpression>(node));
		default:
			throw NotImplementedException(U"not supported expression type for JSON converter");
		}
	}

	json AstToJsonSerialization::VisitPackage(std::shared_ptr<Package> package)
	{
		json obj;
		json classesJson(std::unordered_map<std::string, json>{});
		for (const auto &info : package->classDefs)
		{
			std::string name = UTF32ToUTF8(info->name);
			classesJson[name] = VisitClassInfo(info);
		}
		json moduleJson(std::unordered_map<std::string, json>{});
		for (const auto &info : package->moduleDefs)
		{
			std::string name = UTF32ToUTF8(info->name);
			moduleJson[name] = VisitModuleInfo(info);
		}
		obj["route"] = UTF32ToUTF8(PackageRouteToString(package->route));
		obj["classes"] = classesJson;
		obj["modules"] = moduleJson;
		return obj;
	}

	json AstToJsonSerialization::VisitProject(Project & project)
	{
		json obj;
		for (auto pkg : project.packages)
		{
			obj[UTF32ToUTF8(PackageRouteToString(pkg->route))] = VisitPackage(pkg);
		}
		return obj;
	}

	std::vector<json> AstToJsonSerialization::VisitArgumentList(
		const std::vector<Argument> &arguments)
	{
		std::vector<json> argumentsJson;
		for (const auto &argument : arguments)
		{
			json arg;
			if (argument.name)
			{
				arg["name"] = UTF32ToUTF8(*(argument.name));
				arg["value"] = VisitExpression(argument.value);
			}
			else
			{
				arg["name"] = "";
				arg["value"] = VisitExpression(argument.value);
			}
			argumentsJson.push_back(arg);
		}
		return argumentsJson;
	}

	json AstToJsonSerialization::VisitAnnotationList(
		const Table<std::u32string, AnnotationInfo> &annotations)
	{
		json annotationList(std::unordered_map<std::string, json>{});
		for (const auto &annotation : annotations.values)
		{
			json obj;
			std::string name = UTF32ToUTF8(annotation.name);
			obj["position"] = VisitSourceLocation(annotation.position);
			obj["name"] = name;
			obj["arguments"] = VisitArgumentList(annotation.arguments);
			annotationList[name] = obj;
		}
		return annotationList;
	}

	std::vector<json> AstToJsonSerialization::VisitExpressionList(
		const std::vector<ExpPtr> &expressions)
	{
		std::vector<json> expressionList;
		for (const auto &exp : expressions)
		{
			expressionList.push_back(VisitExpression(exp));
		}
		return expressionList;
	}

	json AstToJsonSerialization::VisitLocation(LocationPtr location)
	{
		json obj;
		obj["type"] = UTF32ToUTF8(Enum<LocationType>::ToString(location->type));
		return obj;
	}

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
			if (rule.parameters.size() == parameters.size())
			{
				int n = static_cast<int>(rule.parameters.size());
				for (int i = 0; i < n; i++)
				{
					if (!(rule.parameters[i]->Equals(parameters[i])))
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		};
		if (rules.find(functionName) != rules.end())
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

	TypeChecker::TypeChecker(Project &project) : project{ project }
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

	TypePtr TypeChecker::CheckUnary(std::shared_ptr<UnaryExpression> node, Scope<TypePtr>* scope)
	{
		auto operand = CheckExpression(node->operand, scope);
		if (node->nodeType == ExpressionType::UnaryPlus)
		{
			if (auto res = ruleSet.Match(U"+", { operand }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: +, operand: {}", operand->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::UnaryMinus)
		{
			if (auto res = ruleSet.Match(U"-", { operand }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: -, operand: {}", operand->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::Convert)
		{
			TypePtr from = CheckExpression(node->operand, scope);
			TypePtr to = node->type;
			if (typeGraph.IsSubTypeof(from, to))
			{
				node->upCasting = true;
				return to;
			}
			else if (typeGraph.IsSuperTypeof(from, to))
			{
				node->upCasting = false;
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

	TypePtr TypeChecker::CheckBinary(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope)
	{
		auto left = CheckExpression(node->left, scope);
		auto right = CheckExpression(node->right, scope);
		if (node->nodeType == ExpressionType::Add)
		{
			if (auto res = ruleSet.Match(U"+", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: +, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::Subtract)
		{
			if (auto res = ruleSet.Match(U"-", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: -, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::Multiply)
		{
			if (auto res = ruleSet.Match(U"*", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: *, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::Divide)
		{
			if (auto res = ruleSet.Match(U"/", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: /, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::GreaterThan)
		{
			if (auto res = ruleSet.Match(U">", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: >, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::LessThan)
		{
			if (auto res = ruleSet.Match(U"<", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: <, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::GreaterThanOrEqual)
		{
			if (auto res = ruleSet.Match(U">=", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: >=, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::LessThanOrEqual)
		{
			if (auto res = ruleSet.Match(U"<=", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: <=, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::Equal)
		{
			if (auto res = ruleSet.Match(U"==", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: ==, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else if (node->nodeType == ExpressionType::NotEqual)
		{
			if (auto res = ruleSet.Match(U"!=", { left, right }))
			{
				return Attach(node, *res);
			}
			else
			{
				throw TypeException(node->position,
					Format(U"type mismatch: !=, left: {}, right: {}", left->ToString(), right->ToString()));
			}
		}
		else
		{
			throw NotImplementedException(U"not implemented binary operation");
		}
	}

	TypePtr TypeChecker::CheckBlock(std::shared_ptr<BlockExpression> node, Scope<TypePtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		for (const auto &exp : node->expressions)
		{
			CheckExpression(exp, scope);
		}
		Attach(node, Type::Void());
		return Type::Void();
	}

	TypePtr TypeChecker::Attach(ExpPtr node, TypePtr type)
	{
		node->type = type;
		return type;
	}

	TypePtr TypeChecker::CheckAssign(std::shared_ptr<BinaryExpression> node, Scope<TypePtr>* scope)
	{
		if (node->left->nodeType == ExpressionType::Parameter)
		{
			auto left = CheckExpression(node->left, scope);
			auto right = CheckExpression(node->right, scope);
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
			auto left = CheckExpression(node->left, scope);
			auto right = CheckExpression(node->right, scope);
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

	TypePtr TypeChecker::CheckWhile(std::shared_ptr<WhileExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr condition = CheckExpression(node->condition, scope);
		if (condition->typeCode == TypeCode::Boolean)
		{
			CheckExpression(node->body, scope);
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
		if (type->typeCode == TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<ClassType>(type);
			if (auto classInfo = project.GetClass(classType))
			{
				return std::make_shared<ClassType>((*classInfo)->route, (*classInfo)->name);
			}
			else if (auto moduleInfo = project.GetModule(std::make_shared<ModuleType>(classType->route, classType->name)))
			{
				return std::make_shared<ModuleType>((*moduleInfo)->route, (*moduleInfo)->name);
			}
			else if (auto interfaceInfo = project.GetInterface(std::make_shared<InterfaceType>(classType->route, classType->name)))
			{
				return std::make_shared<InterfaceType>((*interfaceInfo)->route, (*interfaceInfo)->name);
			}
			else
			{
				throw TypeException(position,
					Format(U"type '{}' not defined", classType->ToString()));
			}
		}
		else if (type->typeCode == TypeCode::Module)
		{
			auto moduleType = std::static_pointer_cast<ModuleType>(type);
			if (auto moduleInfo = project.GetModule(moduleType))
			{
				return std::make_shared<ModuleType>((*moduleInfo)->route, (*moduleInfo)->name);
			}
			else
			{
				throw TypeException(position,
					Format(U"type '{}' not defined", moduleType->ToString()));
			}
		}
		else if (type->typeCode == TypeCode::Interface)
		{
			auto interfaceType = std::static_pointer_cast<InterfaceType>(type);
			if (auto interfaceInfo = project.GetInterface(interfaceType))
			{
				return std::make_shared<InterfaceType>((*interfaceInfo)->route, (*interfaceInfo)->name);
			}
			else
			{
				throw TypeException(position,
					Format(U"type '{}' not defined", interfaceType->ToString()));
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
				[this, position](TypePtr t) -> TypePtr
			{
				return CheckType(position, t);
			}
			);
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

	TypePtr TypeChecker::CheckExpression(ExpPtr node, Scope<TypePtr>* scope)
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
			return CheckBinary(std::static_pointer_cast<BinaryExpression>(node), scope);
		case ExpressionType::UnaryPlus:
		case ExpressionType::UnaryMinus:
		case ExpressionType::Convert:
			return CheckUnary(std::static_pointer_cast<UnaryExpression>(node), scope);
		case ExpressionType::Assign:
			return CheckAssign(std::static_pointer_cast<BinaryExpression>(node), scope);
		case ExpressionType::Block:
			return CheckBlock(std::static_pointer_cast<BlockExpression>(node), scope);
		case ExpressionType::Constant:
			return CheckConstant(std::static_pointer_cast<ConstantExpression>(node));
		case ExpressionType::Parameter:
			return CheckParameter(std::static_pointer_cast<ParameterExpression>(node), scope);
		case ExpressionType::Conditional:
			return CheckConditional(std::static_pointer_cast<ConditionalExpression>(node), scope);
		case ExpressionType::Default:
			return CheckDefault(std::static_pointer_cast<DefaultExpression>(node));
		case ExpressionType::Invoke:
			return CheckInvocation(std::static_pointer_cast<InvocationExpression>(node), scope);
		case ExpressionType::Return:
			return CheckReturn(std::static_pointer_cast<ReturnExpression>(node), scope);
		case ExpressionType::MemberAccess:
			return CheckMemberAccess(std::static_pointer_cast<MemberAccessExpression>(node), scope);
		case ExpressionType::New:
			return CheckNewExpression(std::static_pointer_cast<NewExpression>(node), scope);
		case ExpressionType::VariableDefinition:
			return CheckVarDefExpression(std::static_pointer_cast<VarDefExpression>(node), scope);
		case ExpressionType::While:
			return CheckWhile(std::static_pointer_cast<WhileExpression>(node), scope);
		default:
			throw NotImplementedException(
				Format(U"not supported type checker node type '{}'", Enum<ExpressionType>::ToString(node->nodeType)));
		}
	}

	TypePtr TypeChecker::CheckConstant(std::shared_ptr<ConstantExpression> node)
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

	TypePtr TypeChecker::CheckFieldDef(FieldDef &field, Scope<TypePtr>* scope)
	{
		field.type = CheckType(field.position, field.type);
		if (field.value->nodeType == ExpressionType::Default)
		{
			scope->Put(field.name, field.type);
			return field.type;
		}
		else
		{
			auto type = CheckExpression(field.value, scope);
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

	TypePtr TypeChecker::CheckMethodDef(MethodDef &method, Scope<TypePtr>* outerScope)
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
		CheckExpression(method.body, scope);
		return method.returnType;
	}

	TypePtr TypeChecker::CheckParameter(std::shared_ptr<ParameterExpression> parameter, Scope<TypePtr>* scope)
	{
		auto result = scope->Get(parameter->name);
		if (result)
		{
			auto type = std::any_cast<TypePtr>(*result);
			Attach(parameter, type);
			return type;
		}
		else
		{
			throw TypeException(parameter->position, Format(U"parameter '{}' not defined", parameter->name));
		}
	}

	TypePtr TypeChecker::CheckReturn(std::shared_ptr<ReturnExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr returnType = CheckExpression(node->value, scope);
		Attach(node, returnType);
		return returnType;
	}

	TypePtr TypeChecker::CheckConditional(std::shared_ptr<ConditionalExpression> node, Scope<TypePtr>* scope)
	{
		auto condition = CheckExpression(node->condition, scope);
		auto ifTrue = CheckExpression(node->ifTrue, scope);
		auto ifFalse = CheckExpression(node->ifFalse, scope);
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

	TypePtr TypeChecker::CheckDefault(std::shared_ptr<DefaultExpression> node)
	{
		return node->type;
	}

	TypePtr TypeChecker::CheckInvocation(std::shared_ptr<InvocationExpression> node, Scope<TypePtr>* scope)
	{
		auto exp = CheckExpression(node->expression, scope);
		// TO DO: named arguments
		std::vector<TypePtr> args(node->arguments.size());
		std::transform(node->arguments.begin(), node->arguments.end(), args.begin(),
			[this, &scope](const Argument &arg) -> TypePtr
		{
			return CheckExpression(arg.value, scope);
		});
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

	TypePtr TypeChecker::CheckMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr object = CheckExpression(node->object, scope);
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
			if (node->field == U"Size")
			{
				auto ft = std::make_shared<FunctionType>(arrayType, U"Size", std::vector<TypePtr>{}, Type::Int32());
				return Attach(node, ft);
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

	TypePtr TypeChecker::CheckNewExpression(std::shared_ptr<NewExpression> node, Scope<TypePtr>* scope)
	{
		node->type = CheckType(node->position, std::static_pointer_cast<ClassType>(node->type));
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
							auto value = CheckExpression(argument.value, scope);
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

	TypePtr TypeChecker::CheckVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<TypePtr>* scope)
	{
		TypePtr value = CheckExpression(node->value, scope);
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
			auto convertExp = std::make_shared<UnaryExpression>(node->position, ExpressionType::Convert, node->value);
			convertExp->upCasting = true;
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
			std::sort(
				classInfo->interfaceList.begin(),
				classInfo->interfaceList.end(),
				[this, classInfo](std::shared_ptr<InterfaceType>&x, std::shared_ptr<InterfaceType>& y) -> bool
			{
				auto ix = project.GetInterface(x);
				auto iy = project.GetInterface(y);
				if (!ix)
				{
					throw CompilerException(classInfo->position,
						Format(U"missing interface {} for class {}", x->ToString(), classInfo->name));
				}
				else if (!iy)
				{
					throw CompilerException(classInfo->position,
						Format(U"missing interface {} for class {}", y->ToString(), classInfo->name));
				}
				else
				{
					if (!(*ix)->index)
					{
						throw CompilerException(classInfo->position,
							Format(U"missing index of interface {} for class {}", x->ToString(), classInfo->name));
					}
					else if (!(*iy)->index)
					{
						throw CompilerException(classInfo->position,
							Format(U"missing index of interface {} for class {}", y->ToString(), classInfo->name));
					}
					else
					{
						return (*ix)->index < (*iy)->index;
					}
				}
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

	TreeTraverser::TreeTraverser(std::function<bool(ExpPtr)> filter) :filter{ filter }
	{
	}

	void TreeTraverser::VisitExpression(ExpPtr node, std::vector<ExpPtr>& nodeList)
	{
		if (filter(node))
		{
			nodeList.push_back(node);
		}
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
			VisitBinary(std::static_pointer_cast<BinaryExpression>(node), nodeList);
			return;
		case ExpressionType::UnaryPlus:
		case ExpressionType::UnaryMinus:
		case ExpressionType::Convert:
			VisitUnary(std::static_pointer_cast<UnaryExpression>(node), nodeList);
			return;
		case ExpressionType::Constant:
			return;
		case ExpressionType::Block:
			VisitBlock(std::static_pointer_cast<BlockExpression>(node), nodeList);
			return;
		case ExpressionType::Return:
			return VisitReturn(std::static_pointer_cast<ReturnExpression>(node), nodeList);
		case ExpressionType::Parameter:
			return;
		case ExpressionType::Conditional:
			VisitConditional(std::static_pointer_cast<ConditionalExpression>(node), nodeList);
			return;
		case ExpressionType::Default:
			return;
		case ExpressionType::Invoke:
			VisitInvocation(std::static_pointer_cast<InvocationExpression>(node), nodeList);
			return;
		case ExpressionType::MemberAccess:
			VisitMemberAccess(std::static_pointer_cast<MemberAccessExpression>(node), nodeList);
			return;
		case ExpressionType::New:
			VisitNewExpression(std::static_pointer_cast<NewExpression>(node), nodeList);
			return;
		case ExpressionType::VariableDefinition:
			VisitVarDefExpression(std::static_pointer_cast<VarDefExpression>(node), nodeList);
			return;
		case ExpressionType::While:
			VisitWhileExpression(std::static_pointer_cast<WhileExpression>(node), nodeList);
			return;
		default:
			throw NotImplementedException(U"not implemented node type for tree traverser");
		}
	}
	void TreeTraverser::VisitUnary(std::shared_ptr<UnaryExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->operand, nodeList);
	}
	void TreeTraverser::VisitBinary(std::shared_ptr<BinaryExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->left, nodeList);
		VisitExpression(node->right, nodeList);
	}
	void TreeTraverser::VisitBlock(std::shared_ptr<BlockExpression> node, std::vector<ExpPtr>& nodeList)
	{
		for (auto exp : node->expressions)
		{
			VisitExpression(exp, nodeList);
		}
	}
	void TreeTraverser::VisitReturn(std::shared_ptr<ReturnExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->value, nodeList);
	}
	void TreeTraverser::VisitConditional(std::shared_ptr<ConditionalExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->condition, nodeList);
		VisitExpression(node->ifTrue, nodeList);
		VisitExpression(node->ifFalse, nodeList);
	}
	void TreeTraverser::VisitInvocation(std::shared_ptr<InvocationExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->expression, nodeList);
		for (auto arg : node->arguments)
		{
			VisitExpression(arg.value, nodeList);
		}
	}
	void TreeTraverser::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->object, nodeList);
	}
	void TreeTraverser::VisitNewExpression(std::shared_ptr<NewExpression> node, std::vector<ExpPtr>& nodeList)
	{
		for (auto arg : node->arguments)
		{
			VisitExpression(arg.value, nodeList);
		}
	}
	void TreeTraverser::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->value, nodeList);
	}
	void TreeTraverser::VisitWhileExpression(std::shared_ptr<WhileExpression> node, std::vector<ExpPtr>& nodeList)
	{
		VisitExpression(node->condition, nodeList);
		VisitExpression(node->body, nodeList);
	}

	void LocalVariableCollector::VisitMethodDef(MethodDef & method)
	{
		std::function<bool(ExpPtr)> filter = [](ExpPtr node)
		{
			return node->nodeType == ExpressionType::VariableDefinition;
		};
		TreeTraverser traverser(filter);
		std::vector<ExpPtr> nodeList;
		traverser.VisitExpression(method.body, nodeList);
		int offset = 0;
		if (method.selfType->typeCode == TypeCode::Class)
		{
			offset++;
		}
		for (auto parameter : method.parameters)
		{
			parameter->location = std::make_shared<ParameterLocation>(offset);
			offset++;
		}
		for (auto node : nodeList)
		{
			auto varDef = std::static_pointer_cast<VarDefExpression>(node);
			varDef->variable->location = std::make_shared<ParameterLocation>(offset);
			method.localVariables.push_back(varDef);
			offset++;
		}
	}
	void LocalVariableCollector::VisitPackage(std::shared_ptr<Package> package)
	{
		for (auto& _class : package->classes.values)
		{
			for (auto& method : _class->methodDefs.values)
			{
				VisitMethodDef(method);
			}
		}
		for (auto& module : package->modules.values)
		{
			for (auto& method : module->methods.values)
			{
				VisitMethodDef(method);
			}
		}
	}
	void LocalVariableCollector::VisitProject(Project & project)
	{
		for (auto package : project.packages)
		{
			VisitPackage(package);
		}
	}
	VariableLocator::VariableLocator(Project & project) : project{ project }
	{

	}
	void VariableLocator::VisitExpression(ExpPtr node, Scope<LocationPtr>* scope)
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
			VisitBinary(std::static_pointer_cast<BinaryExpression>(node), scope);
			return;
		case ExpressionType::UnaryPlus:
		case ExpressionType::UnaryMinus:
		case ExpressionType::Convert:
			VisitUnary(std::static_pointer_cast<UnaryExpression>(node), scope);
			return;
		case ExpressionType::Block:
			VisitBlock(std::static_pointer_cast<BlockExpression>(node), scope);
			return;
		case ExpressionType::Parameter:
			VisitParameter(std::static_pointer_cast<ParameterExpression>(node), scope);
			return;
		case ExpressionType::Return:
			VisitReturn(std::static_pointer_cast<ReturnExpression>(node), scope);
			return;
		case ExpressionType::Conditional:
			VisitConditional(std::static_pointer_cast<ConditionalExpression>(node), scope);
			return;
		case ExpressionType::Invoke:
			VisitInvocation(std::static_pointer_cast<InvocationExpression>(node), scope);
			return;
		case ExpressionType::MemberAccess:
			VisitMemberAccess(std::static_pointer_cast<MemberAccessExpression>(node), scope);
			return;
		case ExpressionType::New:
			VisitNewExpression(std::static_pointer_cast<NewExpression>(node), scope);
			return;
		case ExpressionType::VariableDefinition:
			VisitVarDefExpression(std::static_pointer_cast<VarDefExpression>(node), scope);
			return;
		case ExpressionType::While:
			VisitWhileExpression(std::static_pointer_cast<WhileExpression>(node), scope);
			return;
		case ExpressionType::Constant:
		case ExpressionType::Default:
			return;
		default:
			throw NotImplementedException(
				Format(U"not implemented node type '{}' for variable locator", Enum<ExpressionType>::ToString(node->nodeType)));
		}
	}
	void VariableLocator::VisitBlock(std::shared_ptr<BlockExpression> node, Scope<LocationPtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		for (auto exp : node->expressions)
		{
			VisitExpression(exp, scope);
		}
	}
	void VariableLocator::VisitUnary(std::shared_ptr<UnaryExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->operand, scope);
	}
	void VariableLocator::VisitBinary(std::shared_ptr<BinaryExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->left, scope);
		VisitExpression(node->right, scope);
	}
	void VariableLocator::VisitClassInfo(std::shared_ptr<ClassInfo> info, Scope<LocationPtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		int offset = 0;
		for (auto& field : info->fields)
		{
			scope->Put(field.name, std::make_shared<MemberLocation>(LocationType::ClassField, *info->index, offset));
			field.index = offset;
			offset++;
		}
		for (auto& method : info->methods)
		{
			scope->Put(method.name, std::make_shared<MemberLocation>(LocationType::ClassMethod, *info->index, offset));
		}
		for (auto& method : info->methodDefs)
		{
			VisitMethodDef(method, scope);
		}
	}
	void VariableLocator::VisitModuleInfo(std::shared_ptr<ModuleInfo> info, Scope<LocationPtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		int offset = 0;
		for (auto& field : info->fields.values)
		{
			scope->Put(field.name, std::make_shared<MemberLocation>(LocationType::ModuleField, *info->index, *field.index));
			offset++;
		}
		for (auto& method : info->methods.values)
		{
			scope->Put(method.name, std::make_shared<MemberLocation>(LocationType::ModuleMethod, *info->index, *method.index));
		}
		for (const auto& method : info->methods.values)
		{
			VisitMethodDef(method, scope);
		}
	}
	void VariableLocator::VisitMethodDef(const MethodDef & method, Scope<LocationPtr>* outerScope)
	{
		auto scope = scopeFactory->New(outerScope);
		if (method.selfType->typeCode == TypeCode::Class)
		{
			scope->Put(U"this", std::make_shared<ParameterLocation>(0));
		}
		for (auto parameter : method.parameters)
		{
			auto location = parameter->location;
			scope->Put(parameter->name, location);
		}
		VisitExpression(method.body, scope);
	}
	void VariableLocator::VisitParameter(std::shared_ptr<ParameterExpression> parameter, Scope<LocationPtr>* scope)
	{
		if (auto value = scope->Get(parameter->name))
		{
			parameter->location = *value;
		}
		else
		{
			throw TypeException(parameter->position,
				Format(U"parameter '{}' is not defined", parameter->name));
		}
	}
	void VariableLocator::VisitReturn(std::shared_ptr<ReturnExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->value, scope);
	}
	void VariableLocator::VisitConditional(std::shared_ptr<ConditionalExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->condition, scope);
		VisitExpression(node->ifTrue, scope);
		VisitExpression(node->ifFalse, scope);
	}
	void VariableLocator::VisitInvocation(std::shared_ptr<InvocationExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->expression, scope);
		for (auto arg : node->arguments)
		{
			VisitExpression(arg.value, scope);
		}
	}
	void VariableLocator::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->object, scope);
		TypePtr object = node->object->type;
		if (object->typeCode == TypeCode::Module)
		{
			auto moduleType = std::static_pointer_cast<ModuleType>(object);

			if (auto res = project.GetModule(moduleType))
			{
				const auto &moduleInfo = *res;
				if (moduleInfo->fields.ContainsKey(node->field))
				{
					auto& field = moduleInfo->fields.GetValueByKey(node->field);
					node->location = std::make_shared<MemberLocation>(LocationType::ModuleField, *moduleInfo->index, *field.index);
				}
				else if (moduleInfo->methods.ContainsKey(node->field))
				{
					auto& method = moduleInfo->methods.GetValueByKey(node->field);
					node->location = std::make_shared<MemberLocation>(LocationType::ModuleMethod, *moduleInfo->index, *method.index);
				}
				else
				{
					throw TypeException(node->position,
						Format(U"undefined field '{}' in module '{}'", node->field, moduleType->name));
				}
			}
			else
			{
				throw TypeException(node->position, Format(U"undefined module '{}'", moduleType->ToString()));
			}
		}
		else if (object->typeCode == TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<ClassType>(object);
			if (auto res = project.GetClass(classType))
			{
				const auto &classInfo = *res;
				if (classInfo->fields.ContainsKey(node->field))
				{
					auto& field = classInfo->fields.GetValueByKey(node->field);
					node->location = std::make_shared<MemberLocation>(LocationType::ClassField, *classInfo->index, *field.index);
				}
				else if (classInfo->methods.ContainsKey(node->field))
				{
					auto& method = classInfo->methods.GetValueByKey(node->field);
					node->location = std::make_shared<MemberLocation>(LocationType::ClassMethod, *classInfo->index, *method.index);
				}
				else
				{
					throw TypeException(node->position,
						Format(U"undefined field '{}' in class '{}'", node->field, classType->name));
				}
			}
			else
			{
				throw TypeException(node->position,
					Format(U"undefined class '{}'", classType->name));
			}
		}
		else if (object->typeCode == TypeCode::Interface)
		{
			auto interfaceType = std::static_pointer_cast<InterfaceType>(object);
			if (auto res = project.GetInterface(interfaceType))
			{
				const auto &interfaceInfo = *res;
				if (interfaceInfo->methodDefs.ContainsKey(node->field))
				{
					auto& method = interfaceInfo->methodDefs.GetValueByKey(node->field);
					node->location = std::make_shared<MemberLocation>(LocationType::InterfaceMethod, *interfaceInfo->index, *method.index);
				}
				else
				{
					throw TypeException(node->position,
						Format(U"undefined field '{}' in class '{}'", node->field, interfaceInfo->name));
				}
			}
			else
			{
				throw TypeException(node->position,
					Format(U"undefined interface '{}'", interfaceType->name));
			}
		}
		else if (object->typeCode == TypeCode::Array)
		{
			// pass
		}
		else
		{
			throw TypeException(node->position,
				Format(U"object '{}' does not have any field", object->ToString()));
		}
	}
	void VariableLocator::VisitNewExpression(std::shared_ptr<NewExpression> node, Scope<LocationPtr>* scope)
	{
		if (node->type->typeCode == TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<ClassType>(node->type);
			if (auto res = project.GetClass(classType))
			{
				const auto &classInfo = *res;
				for (auto& arg : node->arguments)
				{
					if (arg.name)
					{
						if (classInfo->fields.ContainsKey(*arg.name))
						{
							int index = classInfo->fields.GetIndexByKey(*arg.name);
							arg.index = index;
							VisitExpression(arg.value, scope);
						}
						else
						{
							throw TypeException(node->position,
								Format(U"missing field name '{}' in the new statement", *arg.name));
						}
					}
					else
					{
						throw TypeException(node->position,
							Format(U"missing field name '{}' in the new statement", *arg.name));
					}
				}
			}
			else
			{
				throw TypeException(node->position,
					Format(U"undefined class '{}'", classType->ToString()));
			}
		}
		else
		{
			throw TypeException(node->position, U"wrong new statement: type is not a class");
		}
	}
	void VariableLocator::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node, Scope<LocationPtr>* scope)
	{
		scope->Put(node->variable->name, node->variable->location);
		VisitExpression(node->value, scope);
	}
	void VariableLocator::VisitWhileExpression(std::shared_ptr<WhileExpression> node, Scope<LocationPtr>* scope)
	{
		VisitExpression(node->condition, scope);
		VisitExpression(node->body, scope);
	}

	void VariableLocator::VisitPackage(std::shared_ptr<Package> package, Scope<LocationPtr>* globalScope)
	{
		auto scope = scopeFactory->New(globalScope);
		for (auto moduleInfo : package->modules)
		{
			scope->Put(moduleInfo->name, std::make_shared<TypeLocation>(LocationType::ModuleName, *moduleInfo->index));
		}
		for (auto classInfo : package->classes.values)
		{
			VisitClassInfo(classInfo, scope);
		}
		for (auto moduleInfo : package->modules.values)
		{
			VisitModuleInfo(moduleInfo, scope);
		}
	}
	void VariableLocator::VisitProject()
	{
		scopeFactory = ScopeFactory<LocationPtr>::Create();
		auto scope = scopeFactory->New();
		for (auto package : project.packages)
		{
			VisitPackage(package, scope);
		}
	}
	void ConstantCollector::VisitMethodDef(MethodDef & method, std::unordered_set<ConstantKey>& constantSet)
	{
		std::function<bool(ExpPtr)> filter = [](ExpPtr node)
		{
			return node->nodeType == ExpressionType::Constant;
		};
		TreeTraverser traverser(filter);
		std::vector<ExpPtr> nodeList;
		traverser.VisitExpression(method.body, nodeList);
		for (auto exp : nodeList)
		{
			auto node = std::static_pointer_cast<ConstantExpression>(exp);
			if (node->type->typeCode == TypeCode::Boolean)
			{
				// pass
			}
			else
			{
				// TO DO: small integers, 0.0 and 1.0
				ConstantKey key{ node->type->typeCode, node->constant };
				constantSet.insert(key);
			}
		}
	}
	void ConstantCollector::VisitPackage(std::shared_ptr<Package> package)
	{
		for (auto& classInfo : package->classes)
		{
			std::unordered_set<ConstantKey> constantSet;
			for (auto& method : classInfo->methodDefs.values)
			{
				VisitMethodDef(method, constantSet);
			}
			int index = 0;
			for (auto key : constantSet)
			{
				classInfo->constantMap.insert({ key, index });
				index++;
			}
		}
		for (auto& moduleInfo : package->modules)
		{
			std::unordered_set<ConstantKey> constantSet;
			for (auto& method : moduleInfo->methods.values)
			{
				VisitMethodDef(method, constantSet);
			}
			int index = 0;
			for (auto key : constantSet)
			{
				moduleInfo->constantMap.insert({ key, index });
				index++;
			}
		}
	}
	void ConstantCollector::VisitProject(Project & project)
	{
		for (auto package : project.packages)
		{
			VisitPackage(package);
		}
	}

	void TypeRenamer::RenameAll(Project & project)
	{
		for (auto pkg : project.packages)
		{
			auto& typeAliases = pkg->typeAliases;
			for (auto classInfo : pkg->classes)
			{
				//std::cout << "rename class: " << classInfo->name << std::endl;
				for (auto& superType : classInfo->superTypes)
				{
					superType = RenameType(superType, typeAliases);
				}
				for (auto& field : classInfo->fieldDefs.values)
				{
					RenameField(field, typeAliases);
				}
				for (auto& method : classInfo->methodDefs.values)
				{
					RenameMethod(method, typeAliases);
				}
			}
			for (auto moduleInfo : pkg->modules)
			{
				//std::cout << "rename module: " << moduleInfo->name << std::endl;
				for (auto& field : moduleInfo->fields.values)
				{
					RenameField(field, typeAliases);
				}
				for (auto& method : moduleInfo->methods.values)
				{
					RenameMethod(method, typeAliases);
				}
			}
		}
	}
	void TypeRenamer::RenameMethod(MethodDef& method, Table<std::u32string, TypeAlias>& typeAliases)
	{
		std::function<bool(ExpPtr)> filter = [](ExpPtr node)
		{
			return node->nodeType == ExpressionType::VariableDefinition || node->nodeType == ExpressionType::New;
		};
		TreeTraverser traverser(filter);
		std::vector<ExpPtr> nodeList;
		traverser.VisitExpression(method.body, nodeList);

		for (auto node : nodeList)
		{
			if (node->nodeType == ExpressionType::VariableDefinition)
			{
				auto varDef = std::static_pointer_cast<VarDefExpression>(node);
				varDef->variable->type = RenameType(varDef->variable->type, typeAliases);
			}
			else
			{
				// New
				auto newExp = std::static_pointer_cast<NewExpression>(node);
				newExp->type = RenameType(newExp->type, typeAliases);
			}
		}

		method.selfType = RenameType(method.selfType, typeAliases);
		method.returnType = RenameType(method.returnType, typeAliases);
		method.signature = RenameType(method.signature, typeAliases);
		for (auto parameter : method.parameters)
		{
			parameter->type = RenameType(parameter->type, typeAliases);
		}
	}
	void TypeRenamer::RenameField(FieldDef & field, Table<std::u32string, TypeAlias>& typeAliases)
	{
		field.type = RenameType(field.type, typeAliases);
	}
	TypePtr TypeRenamer::RenameType(TypePtr type, Table<std::u32string, TypeAlias>& typeAliases)
	{
		if (type->typeCode == TypeCode::Class)
		{
			auto classType = std::static_pointer_cast<ClassType>(type);
			if (typeAliases.ContainsKey(classType->name))
			{
				auto typeAlias = typeAliases.GetValueByKey(classType->name);
				return std::make_shared<ClassType>(typeAlias.route, typeAlias.typeName);
			}
			else
			{
				return type;
			}
		}
		else if (type->typeCode == TypeCode::Module)
		{
			auto moduleType = std::static_pointer_cast<ModuleType>(type);
			if (typeAliases.ContainsKey(moduleType->name))
			{
				auto typeAlias = typeAliases.GetValueByKey(moduleType->name);
				return std::make_shared<ModuleType>(typeAlias.route, typeAlias.typeName);
			}
			else
			{
				return type;
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
				[&](TypePtr t) -> TypePtr { return RenameType(t, typeAliases); });
			return std::make_shared<FunctionType>(
				RenameType(functionType->selfType, typeAliases),
				functionType->name,
				parameters,
				RenameType(functionType->returnType, typeAliases));
		}
		else if (type->typeCode == TypeCode::Array)
		{
			auto arrayType = std::static_pointer_cast<ArrayType>(type);
			return std::make_shared<ArrayType>(
				RenameType(arrayType->elementType, typeAliases));
		}
		else
		{
			// TO DO: GENERIC TYPE
			return type;
		}
	}
	void InheritanceProcessor::VisitProject(Project & project)
	{
		for (auto& program : project.programs.values)
		{
			for (auto& classInfo : program.classDefs.values)
			{
				int n = static_cast<int>(classInfo->superTypes.size());
				for (int i = 0; i < n; i++)
				{
					auto type = classInfo->superTypes.at(i);
					auto classType = std::static_pointer_cast<ClassType>(type);
					if (auto res = project.GetClass(classType))
					{
						auto classInfo = *res;
						if (i != 0)
						{
							throw TypeException(
								classInfo->position,
								U"the super class must be at the first of the extends list"
							);
						}
					}
					else if (auto res = project.GetInterface(
						std::make_shared<InterfaceType>(classType->route, classType->name)))
					{
						classInfo->superTypes.at(i) = std::make_shared<InterfaceType>(classType->route, classType->name);
					}
					else
					{
						throw TypeException(
							classInfo->position,
							Format(U"missing super class '{}", classType->ToString())
						);
					}
				}

			}
		}
		for (auto& program : project.programs.values)
		{
			for (auto& classInfo : program.classDefs.values)
			{
				VisitClass(project, classInfo);
			}
		}
	}
	void InheritanceProcessor::VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo)
	{
		std::stack<FieldDef> fieldStack;
		std::stack<MethodDef> methodStack;
		std::unordered_set<TypePtr> typeSet;
		std::unordered_set<std::u32string> fieldNames;
		std::unordered_set<std::u32string> methodNames;

		auto originalClass = classInfo;
		bool done = false;
		while (!done)
		{
			if (typeSet.find(
				std::make_shared<ClassType>(classInfo->route, classInfo->name)) != typeSet.end())
			{
				throw TypeException(originalClass->position,
					U"cycle detected in the inheritance chain");
			}
			else
			{
				typeSet.insert(
					std::make_shared<ClassType>(classInfo->route, classInfo->name));
			}
			int nFields = static_cast<int>(classInfo->fieldDefs.Size());
			for (int i = nFields - 1; i >= 0; i--)
			{
				auto& field = classInfo->fieldDefs.values[i];
				if (fieldNames.find(field.name) != fieldNames.end())
				{
					throw TypeException(originalClass->position,
						Format(
							U"duplicate field definition '{}' detected in the inheritance chain", field.name));
				}
				else
				{
					fieldNames.insert(field.name);
					fieldStack.push(field);
				}
			}
			int nMethods = static_cast<int>(classInfo->methodDefs.Size());
			for (int i = nMethods - 1; i >= 0; i--)
			{
				auto& method = classInfo->methodDefs.values[i];
				if (methodNames.find(method.name) != methodNames.end())
				{
					// override support
					throw TypeException(originalClass->position,
						Format(
							U"duplicate method definition '{}' detected in the inheritance chain", method.name));
				}
				else
				{
					methodNames.insert(method.name);
					methodStack.push(method);
				}
			}
			if (classInfo->superTypes.size() >= 1)
			{
				auto type = classInfo->superTypes.front();
				if (type->typeCode == TypeCode::Class)
				{
					auto classType = std::static_pointer_cast<ClassType>(type);
					if (auto res = project.GetClass(classType))
					{
						classInfo = *res;
					}
					else
					{
						throw TypeException(
							classInfo->position,
							Format(U"error in extending the class '{}'", classInfo->name)
						);
					}
				}
				else
				{
					done = true;
				}
			}
			else
			{
				done = true;
			}
		}
		while (!(fieldStack.empty()))
		{
			auto field = fieldStack.top();
			originalClass->fields.Add(field.name, field);
			fieldStack.pop();
		}
		while (!(methodStack.empty()))
		{
			auto method = methodStack.top();
			originalClass->methods.Add(method.name, method);
			methodStack.pop();
		}
	}
	void PackageImporter::ImportPackages(Project & project)
	{
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ClassInfo>>> classMap;
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ModuleInfo>>> moduleMap;
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<InterfaceInfo>>> interfaceMap;
		for (auto pkg : project.packages)
		{
			std::unordered_set<PackageRoute> visited;
			CollectInfo(project, classMap, moduleMap, interfaceMap, pkg->route, pkg, visited);
		}
		for (auto pkg : project.packages)
		{
			if (classMap.find(pkg->route) != classMap.end())
			{
				for (auto classInfo : classMap.at(pkg->route))
				{
					pkg->classes.Add(classInfo->name, classInfo);
				}
			}
			if (moduleMap.find(pkg->route) != moduleMap.end())
			{
				for (auto moduleInfo : moduleMap.at(pkg->route))
				{
					pkg->modules.Add(moduleInfo->name, moduleInfo);
				}
			}
			if (interfaceMap.find(pkg->route) != interfaceMap.end())
			{
				for (auto interfaceInfo : interfaceMap.at(pkg->route))
				{
					pkg->interfaces.Add(interfaceInfo->name, interfaceInfo);
				}
			}
		}
	}
	void PackageImporter::CollectInfo(Project& project,
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ClassInfo>>>& classMap,
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<ModuleInfo>>>& moduleMap,
		std::unordered_map<PackageRoute, std::vector<std::shared_ptr<InterfaceInfo>>>& interfaceMap,
		PackageRoute currentRoute,
		std::shared_ptr<Package> pkg, std::unordered_set<PackageRoute>& visited)
	{
		if (visited.find(pkg->route) != visited.end())
		{
			// already visited the package, pass
		}
		else
		{
			visited.insert(pkg->route);

			for (const auto& classInfo : pkg->classDefs)
			{
				classMap[currentRoute].push_back(classInfo);
			}
			for (const auto& moduleInfo : pkg->moduleDefs)
			{
				moduleMap[currentRoute].push_back(moduleInfo);
			}
			for (const auto& interfaceInfo : pkg->interfaceDefs)
			{
				interfaceMap[currentRoute].push_back(interfaceInfo);
			}

			for (auto importedPkg : pkg->importedPackages)
			{
				if (project.packages.ContainsKey(importedPkg.route))
				{
					std::shared_ptr<Package> next = project.packages.GetValueByKey(importedPkg.route);
					CollectInfo(project, classMap, moduleMap, interfaceMap, currentRoute, next, visited);
				}
				else
				{
					throw TypeException(importedPkg.position,
						Format(U"missing pakcage: {}", PackageRouteToString(importedPkg.route)));
				}
			}
		}
	}
	void AssignIndex(Project & project)
	{
		int classIndex = 0;
		int moduleIndex = 0;
		int interfaceIndex = 0;
		for (auto pkg : project.packages)
		{
			for (auto classInfo : pkg->classDefs)
			{
				classInfo->index = classIndex;
				classIndex++;
				int methodIndex = 0;
				for (auto& method : classInfo->methodDefs)
				{
					method.index = methodIndex;
					methodIndex++;
				}
			}
			for (auto moduleInfo : pkg->moduleDefs)
			{
				moduleInfo->index = moduleIndex;
				moduleIndex++;
				int methodIndex = 0;
				for (auto& method : moduleInfo->methods)
				{
					method.index = methodIndex;
					methodIndex++;
				}
			}
			for (auto interfaceInfo : pkg->interfaceDefs)
			{
				interfaceInfo->index = interfaceIndex;
				interfaceIndex++;
				int methodIndex = 0;
				for (auto& method : interfaceInfo->methodDefs)
				{
					method.index = methodIndex;
					methodIndex++;
				}
			}
		}
	}
} // namespace cygni
