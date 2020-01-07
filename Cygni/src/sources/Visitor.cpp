#include "Visitor.hpp"
#include "Exception.hpp"
#include <algorithm>
namespace cygni {

json AstToJsonSerialization::VisitSourceLocation(SourceLocation location) {
	json obj;
	obj["startLine"] = location.startLine;
	obj["startCol"]  = location.startCol;
	obj["endLine"]   = location.endLine;
	obj["endCol"]	= location.endCol;
	return obj;
}

json AstToJsonSerialization::VisitBinary(
	std::shared_ptr<BinaryExpression> node) {
	json obj;
	AttachNodeInformation(obj, node);
	obj["left"]  = VisitExpression(node->left);
	obj["right"] = VisitExpression(node->right);
	return obj;
}

json AstToJsonSerialization::VisitConstant(
	std::shared_ptr<ConstantExpression> node) {
	json obj;
	AttachNodeInformation(obj, node);
	obj["constant"] = utf32_to_utf8(node->constant);
	return obj;
}

json AstToJsonSerialization::VisitClassInfo(std::shared_ptr<ClassInfo> info) {
	json obj;
	if (info->isModule) {
		obj["nodeType"] = "module";
	} else {
		obj["nodeType"] = "class";
	}
	obj["name"] = utf32_to_utf8(info->name);

	std::vector<json> fieldsJson;
	for (const auto& field : info->fields.values) {
		fieldsJson.push_back(VisitFieldDef(field));
	}
	obj["fields"] = fieldsJson;

	std::vector<json> methodsJson;
	for (const auto& method : info->methods.values) {
		methodsJson.push_back(VisitMethodDef(method));
	}
	obj["methods"] = methodsJson;
	return obj;
}

json AstToJsonSerialization::VisitFieldDef(const FieldDef& field) {
	json obj;
	obj["isStatic"] = field.isStatic;
	obj["location"] = VisitSourceLocation(field.location);
	obj["accessModifier"] =
		utf32_to_utf8(Enum<AccessModifier>::ToString(field.modifier));
	obj["name"]  = utf32_to_utf8(field.name);
	obj["value"] = VisitExpression(field.value);
	return obj;
}

json AstToJsonSerialization::VisitMethodDef(const MethodDef& method) {
	json obj;

	obj["isStatic"] = method.isStatic;
	obj["location"] = VisitSourceLocation(method.location);
	obj["accessModifier"] =
		utf32_to_utf8(Enum<AccessModifier>::ToString(method.modifier));
	obj["name"] = utf32_to_utf8(method.name);

	std::vector<json> parametersJson;
	for (const auto& parameter : method.parameters) {
		parametersJson.push_back(VisitParameter(parameter));
	}
	obj["parameters"] = parametersJson;
	obj["body"]		  = VisitExpression(method.body);

	return obj;
}

json AstToJsonSerialization::VisitParameter(
	std::shared_ptr<ParameterExpression> parameter) {
	json obj;
	obj["name"] = utf32_to_utf8(parameter->name);
	AttachNodeInformation(obj, parameter);
	return obj;
}

json AstToJsonSerialization::VisitBlock(std::shared_ptr<BlockExpression> node) {
	json obj;
	obj["expressions"] = VisitArgumentList(node->expressions);
	AttachNodeInformation(obj, node);
	return obj;
}

json AstToJsonSerialization::VisitReturn(
	std::shared_ptr<ReturnExpression> node) {
	json obj;
	obj["value"] = VisitExpression(node->value);
	AttachNodeInformation(obj, node);
	return obj;
}

json AstToJsonSerialization::VisitConditional(
	std::shared_ptr<ConditionalExpression> node) {
	json obj;
	obj["condition"] = VisitExpression(node->condition);
	obj["ifTrue"]	= VisitExpression(node->ifTrue);
	obj["ifFalse"]   = VisitExpression(node->ifFalse);
	AttachNodeInformation(obj, node);
	return obj;
}

json AstToJsonSerialization::VisitDefault(
	std::shared_ptr<DefaultExpression> node) {
	json obj;
	AttachNodeInformation(obj, node);
	return obj;
}

json AstToJsonSerialization::VisitInvocation(
	std::shared_ptr<InvocationExpression> node) {
	json obj;
	AttachNodeInformation(obj, node);
	obj["expression"] = VisitExpression(node->expression);
	obj["arguments"]  = VisitArgumentList(node->arguments);
	return obj;
}

void AstToJsonSerialization::AttachNodeInformation(json& obj, ExpPtr node) {
	obj["id"]		= node->id;
	obj["location"] = VisitSourceLocation(node->location);
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(node->nodeType));
	obj["type"] = utf32_to_utf8(node->type->ToString());
}

json AstToJsonSerialization::VisitExpression(ExpPtr node) {
	switch (node->nodeType) {
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
		return VisitBinary(std::static_pointer_cast<BinaryExpression>(node));
	case ExpressionType::Constant:
		return VisitConstant(
			std::static_pointer_cast<ConstantExpression>(node));
	case ExpressionType::Block:
		return VisitBlock(std::static_pointer_cast<BlockExpression>(node));
	case ExpressionType::Return:
		return VisitReturn(std::static_pointer_cast<ReturnExpression>(node));
	case ExpressionType::Parameter:
		return VisitParameter(
			std::static_pointer_cast<ParameterExpression>(node));
	case ExpressionType::Conditional:
		return VisitConditional(
			std::static_pointer_cast<ConditionalExpression>(node));
	case ExpressionType::Default:
		return VisitDefault(std::static_pointer_cast<DefaultExpression>(node));
	case ExpressionType::Invoke:
		return VisitInvocation(
			std::static_pointer_cast<InvocationExpression>(node));
	default:
		throw NotImplementedException();
	}
}

json AstToJsonSerialization::VisitProgram(const Program& program) {
	json obj;
	std::vector<json> classesJson;
	for (const auto& info : program.classes.values) {
		classesJson.push_back(VisitClassInfo(info));
	}
	obj["classes"] = classesJson;
	return obj;
}

std::vector<json> AstToJsonSerialization::VisitArgumentList(
	const std::vector<ExpPtr>& arguments) {
	std::vector<json> argumentsJson;
	for (const auto& argument : arguments) {
		argumentsJson.push_back(VisitExpression(argument));
	}
	return argumentsJson;
}

// void LocalVariableCollector::VisitProgram(Program& program) {
// 	for (auto& info : program.classes.values) {
// 		for (auto& method : info->methods.values) {
// 			VisitMethodDef(method);
// 		}
// 	}
// }

// void LocalVariableCollector::VisitMethodDef(MethodDef& method) {
// 	// TO DO
// }

// void LocalVariableCollector::
// VisitBlockExpression(std::shared_ptr<BlockExpression> node) {

// }

TypeChecker::Rule::Rule(std::u32string functionName,
						std::vector<TypePtr> parameters, TypePtr returnType)
	: functionName{functionName}, parameters{parameters}, returnType{
															  returnType} {
}

void TypeChecker::RuleSet::Add(std::u32string functionName,
							   std::vector<TypePtr> parameters,
							   TypePtr returnType) {
	Rule rule{functionName, parameters, returnType};
	if (rules.find(functionName) != rules.end()) {
		rules[functionName].push_back(rule);
	} else {
		rules.insert({functionName, {rule}});
	}
}

std::optional<TypePtr>
	TypeChecker::RuleSet::Match(std::u32string functionName,
								std::vector<TypePtr> parameters) {
	auto matchOneRule = [&parameters](const Rule& rule) -> bool {
		if (rule.parameters.size() == parameters.size()) {
			int n = rule.parameters.size();
			for (int i = 0; i < n; i++) {
				if (!(rule.parameters[i]->Equals(parameters[i]))) {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	};
	if (rules.find(functionName) != rules.end()) {
		for (const auto& rule : rules[functionName]) {
			if (matchOneRule(rule)) {
				return rule.returnType;
			}
		}
		return std::optional<TypePtr>();
	} else {
		return std::optional<TypePtr>();
	}
}

TypeChecker::TypeChecker() {
	ruleSet.Add(U"+", {Type::Int32(), Type::Int32()}, Type::Int32());
	ruleSet.Add(U"+", {Type::Int64(), Type::Int64()}, Type::Int64());

	ruleSet.Add(U"-", {Type::Int32(), Type::Int32()}, Type::Int32());
	ruleSet.Add(U"-", {Type::Int64(), Type::Int64()}, Type::Int64());

	ruleSet.Add(U"*", {Type::Int32(), Type::Int32()}, Type::Int32());
	ruleSet.Add(U"*", {Type::Int64(), Type::Int64()}, Type::Int64());

	ruleSet.Add(U"/", {Type::Int32(), Type::Int32()}, Type::Int32());
	ruleSet.Add(U"/", {Type::Int64(), Type::Int64()}, Type::Int64());

	ruleSet.Add(U"==", {Type::Int32(), Type::Int32()}, Type::Boolean());
	ruleSet.Add(U"==", {Type::Int64(), Type::Int64()}, Type::Boolean());
}

TypePtr TypeChecker::VisitBinary(std::shared_ptr<BinaryExpression> node,
								 ScopePtr scope) {
	auto left  = VisitExpression(node->left, scope);
	auto right = VisitExpression(node->right, scope);
	if (node->nodeType == ExpressionType::Add) {
		if (auto res = ruleSet.Match(U"+", {left, right})) {
			return Attach(node, *res);
		} else {
			throw TypeException(node->location, U"type mismatch: +");
		}
	} else if (node->nodeType == ExpressionType::Subtract) {
		if (auto res = ruleSet.Match(U"-", {left, right})) {
			return Attach(node, *res);
		} else {
			throw TypeException(node->location, U"type mismatch: -");
		}
	} else if (node->nodeType == ExpressionType::Multiply) {
		if (auto res = ruleSet.Match(U"*", {left, right})) {
			return Attach(node, *res);
		} else {
			throw TypeException(node->location, U"type mismatch: *");
		}
	} else if (node->nodeType == ExpressionType::Divide) {
		if (auto res = ruleSet.Match(U"+", {left, right})) {
			return Attach(node, *res);
		} else {
			throw TypeException(node->location, U"type mismatch: /");
		}
	} else if (node->nodeType == ExpressionType::Equal) {
		if (auto res = ruleSet.Match(U"==", {left, right})) {
			return Attach(node, *res);
		} else {
			throw TypeException(node->location, U"type mismatch: ==");
		}
	} else {
		throw NotImplementedException();
	}
}

TypePtr TypeChecker::VisitBlock(std::shared_ptr<BlockExpression> node,
								ScopePtr scope) {
	TypePtr result = Type::Void();
	for (const auto& exp : node->expressions) {
		result = VisitExpression(exp, scope);
	}
	Attach(node, result);
	return result;
}

TypePtr TypeChecker::Attach(ExpPtr node, TypePtr type) {
	node->type = type;
	return type;
}

TypePtr TypeChecker::VisitExpression(ExpPtr node, ScopePtr scope) {
	std::cout << "msg: "
			  << cygni::utf32_to_utf8(
					 Enum<ExpressionType>::ToString(node->nodeType))
			  << std::endl;
	switch (node->nodeType) {
	case ExpressionType::Add:
	case ExpressionType::Subtract:
	case ExpressionType::Multiply:
	case ExpressionType::Divide:
	case ExpressionType::Equal:
		return VisitBinary(std::static_pointer_cast<BinaryExpression>(node),
						   scope);
	case ExpressionType::Block:
		return VisitBlock(std::static_pointer_cast<BlockExpression>(node),
						  scope);
	case ExpressionType::Constant:
		return VisitConstant(
			std::static_pointer_cast<ConstantExpression>(node));
	case ExpressionType::Parameter:
		return VisitParameter(
			std::static_pointer_cast<ParameterExpression>(node), scope);
	case ExpressionType::Conditional:
		return VisitConditional(
			std::static_pointer_cast<ConditionalExpression>(node), scope);
	case ExpressionType::Default:
		return VisitDefault(std::static_pointer_cast<DefaultExpression>(node));
	case ExpressionType::Invoke:
		return VisitInvocation(
			std::static_pointer_cast<InvocationExpression>(node), scope);
	default:
		throw NotImplementedException();
	}
}

TypePtr TypeChecker::VisitConstant(std::shared_ptr<ConstantExpression> node) {
	return node->type;
}

TypePtr TypeChecker::VisitClassInfo(std::shared_ptr<ClassInfo> info,
									ScopePtr outerScope) {
	ScopePtr scope{outerScope};

	for (const auto& field : info->fields.values) {
		VisitFieldDef(field, scope);
	}
	for (const auto& method : info->methods.values) {
		scope->Put(method.name, method.signature);
	}
	for (const auto& method : info->methods.values) {
		VisitMethodDef(method, scope);
	}
	return Type::Void();
}

TypePtr TypeChecker::VisitFieldDef(const FieldDef& field, ScopePtr scope) {
	auto type = VisitExpression(field.value, scope);
	if (!field.type->Equals(type)) {
		throw TypeException(field.location, U"field type mismatch");
	} else {
		scope->Put(field.name, field.type);
		return field.type;
	}
}

TypePtr TypeChecker::VisitMethodDef(const MethodDef& method,
									ScopePtr outerScope) {
	ScopePtr scope{outerScope};
	for (const auto& parameter : method.parameters) {
		scope->Put(parameter->name, parameter->type);
	}
	return VisitExpression(method.body, scope);
}

TypePtr	TypeChecker::VisitParameter(std::shared_ptr<ParameterExpression> parameter,
								ScopePtr scope) {
	auto result = scope->Get(parameter->name);
	if (result) {
		auto type = std::any_cast<TypePtr>(*result);
		Attach(parameter, type);
		return type;
	} else {
		throw TypeException(parameter->location, U"parameter name not found");
	}
}

TypePtr TypeChecker::VisitReturn(std::shared_ptr<ReturnExpression> node,
								 ScopePtr scope) {
	TypePtr returnType = VisitExpression(node->value, scope);
	return returnType;
}

TypePtr
	TypeChecker::VisitConditional(std::shared_ptr<ConditionalExpression> node,
								  ScopePtr scope) {
	auto condition = VisitExpression(node->condition, scope);
	auto ifTrue	= VisitExpression(node->ifTrue, scope);
	auto ifFalse   = VisitExpression(node->ifFalse, scope);
	if (condition->Equals(Type::Boolean())) {
		if (ifTrue->Equals(ifFalse)) {
			Attach(node, ifTrue);
			return ifTrue;
		} else {
			auto resultType =
				Type::Unify(std::vector<TypePtr>{ifTrue, ifFalse});
			Attach(node, resultType);
			return resultType;
		}
	} else {
		throw TypeException(node->condition->location,
							U"condition type must be boolean");
	}
}

TypePtr TypeChecker::VisitDefault(std::shared_ptr<DefaultExpression> node) {
	return node->type;
}

TypePtr TypeChecker::VisitInvocation(std::shared_ptr<InvocationExpression> node,
									 ScopePtr scope) {
	auto exp = VisitExpression(node->expression, scope);
	std::vector<TypePtr> args(node->arguments.size());
	std::transform(node->arguments.begin(), node->arguments.end(), args.begin(),
				   [this, &scope](const ExpPtr& arg) -> TypePtr {
					   return VisitExpression(arg, scope);
				   });
	if (exp->typeCode == TypeCode::Function) {
		auto functionType = std::static_pointer_cast<FunctionType>(exp);
		if (functionType->Match(args)) {
			Attach(node, functionType->returnType);
			return functionType->returnType;
		} else {
			throw TypeException(node->location,
								U"function call arguments type do not match");
		}
	} else {
		throw TypeException(node->location, U"expression is not a function");
	}
}
void TypeChecker::VisitProgram(const Program& program, ScopePtr scope) {
	for (const auto& _class : program.classes.values) {
		VisitClassInfo(_class, scope);
	}
}
} // namespace cygni