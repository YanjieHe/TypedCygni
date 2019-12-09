#include "Visitor.hpp"
#include "Exception.hpp"
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
	std::vector<json> expressionsJson;
	for (ExpPtr exp : node->expressions) {
		expressionsJson.push_back(VisitExpression(exp));
	}
	obj["expressions"] = expressionsJson;
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
	std::vector<json> argumentsJson;
	for (const auto& argument : node->arguments) {
		argumentsJson.push_back(VisitExpression(argument));
	}
	obj["arguments"] = argumentsJson;
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



} // namespace cygni