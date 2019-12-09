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
	obj["location"] = VisitSourceLocation(node->location);
	obj["id"]		= node->id;
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(node->nodeType));
	obj["left"]  = VisitExpression(node->left);
	obj["right"] = VisitExpression(node->right);
	return obj;
}

json AstToJsonSerialization::VisitConstant(
	std::shared_ptr<ConstantExpression> node) {
	json obj;
	obj["location"] = VisitSourceLocation(node->location);
	obj["id"]		= node->id;
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(node->nodeType));
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
	obj["id"]   = parameter->id;
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(parameter->nodeType));
	return obj;
}

json AstToJsonSerialization::VisitBlock(std::shared_ptr<BlockExpression> node) {
	json obj;
	std::vector<json> expressionsJson;
	for (ExpPtr exp : node->expressions) {
		expressionsJson.push_back(VisitExpression(exp));
	}
	obj["id"] = node->id;
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(node->nodeType));
	obj["expressions"] = expressionsJson;
	obj["location"]	= VisitSourceLocation(node->location);
	return obj;
}

json AstToJsonSerialization::VisitReturn(
	std::shared_ptr<ReturnExpression> node) {
	json obj;
	obj["value"] = VisitExpression(node->value);
	obj["id"]	= node->id;
	obj["nodeType"] =
		utf32_to_utf8(Enum<ExpressionType>::ToString(node->nodeType));
	obj["location"] = VisitSourceLocation(node->location);
	return obj;
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
	default:
		throw NotImplementedException();
	}
}

json AstToJsonSerialization::VisitProgram(const Program& program) {
	json obj;
	obj["path"] = program.path;
	std::vector<json> classesJson;
	for (const auto& info : program.classes.values) {
		classesJson.push_back(VisitClassInfo(info));
	}
	obj["classes"] = classesJson;
	return obj;
}

} // namespace cygni