#include "Expression.hpp"

namespace cygni {
SourceLocation::SourceLocation()
	: document(), startLine{0}, startCol{0}, endLine{0}, endCol{0} {
}

SourceLocation::SourceLocation::SourceLocation(
	std::shared_ptr<SourceDocument> document, int startLine, int startCol,
	int endLine, int endCol)
	: document{document}, startLine{startLine}, startCol{startCol},
	  endLine{endLine}, endCol{endCol} {
}

Expression::Expression(SourceLocation location, ExpressionType nodeType)
	: location{location} {
	static int currentId = 0;
	this->id			 = currentId;
	currentId++;
}

ConstantExpression::ConstantExpression(SourceLocation location,
                                       TypePtr type,
									   std::u32string constant)
	: Expression(location, ExpressionType::Constant),type{type}, constant{constant} {
}

BinaryExpression::BinaryExpression(SourceLocation location,
								   ExpressionType nodeType, ExpPtr left,
								   ExpPtr right)
	: Expression(location, nodeType), left{left}, right{right} {
}

UnaryExpression::UnaryExpression(SourceLocation location,
								 ExpressionType nodeType, ExpPtr operand)
	: Expression(location, nodeType), operand{operand} {
}

BlockExpression::BlockExpression(SourceLocation location, ExpList expressions)
	: Expression(location, ExpressionType::Block), expressions{expressions} {
}

ConditionalExpression::ConditionalExpression(SourceLocation location,
											 ExpPtr condition, ExpPtr ifTrue,
											 ExpPtr ifFalse)
	: Expression(location, ExpressionType::Conditional), condition{condition},
	  ifTrue{ifTrue}, ifFalse{ifFalse} {
}

DefaultExpression::DefaultExpression(SourceLocation location)
	: Expression(location, ExpressionType::Default) {
}

InvocationExpression::InvocationExpression(SourceLocation location,
										   ExpPtr expression,
										   std::vector<ExpPtr> arguments)
	: Expression(location, ExpressionType::Invoke),
	  expression{expression}, arguments{arguments} {
}

MethodCallExpression::MethodCallExpression(SourceLocation location,
										   ExpPtr object,
										   std::shared_ptr<MethodDef> method,
										   ExpList arguments)
	: Expression(location, ExpressionType::MethodCall), object{object},
	  method{method}, arguments{arguments} {};

NewExpression::NewExpression(SourceLocation location,
							 ConstructorInfo constructorInfo, ExpList arguments)
	: Expression(location, ExpressionType::New),
	  constructorInfo{constructorInfo}, arguments{arguments} {
}

ParameterExpression::ParameterExpression(SourceLocation location,
										 std::u32string name, TypePtr type)
	: Expression(location, ExpressionType::Parameter), name{name}, type{type} {
}

FunctionDef::FunctionDef(std::u32string name,
						 std::vector<ParameterExpression> parameters,
						 TypePtr returnType, ExpPtr body)
	: name{name}, parameters{parameters}, returnType{returnType}, body{body} {
}

FieldDef::FieldDef(std::u32string name, TypePtr type)
	: name{name}, type{type} {
}

MethodDef::MethodDef(std::u32string name, std::vector<ParameterExpression> parameters,
						   TypePtr returnType)
	: name{name}, parameters{parameters}, returnType{returnType} {
}

ClassInfo::ClassInfo(std::u32string name) : name{name} {
}

} // namespace cygni