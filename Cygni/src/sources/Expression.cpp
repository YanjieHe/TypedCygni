#include "Expression.hpp"

namespace cygni {

SourceDocument::SourceDocument(std::string filePath, std::string fileName)
	: filePath{filePath}, fileName{fileName} {
}

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
	: location{location}, nodeType{nodeType} {
	static int currentId = 0;
	this->id			 = currentId;
	currentId++;
	static std::shared_ptr<UnknownType> unknownType =
		std::make_shared<UnknownType>();
	this->type = unknownType;
}

ConstantExpression::ConstantExpression(SourceLocation location, TypePtr type,
									   std::u32string constant)
	: Expression(location, ExpressionType::Constant), constant{constant} {
	this->type = type;
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
	: Expression(location, ExpressionType::Parameter), name{name} {
	this->type = type;
}

VariableDefinitionExpression::VariableDefinitionExpression(
	SourceLocation location, std::shared_ptr<ParameterExpression> variable,
	ExpPtr value)
	: Expression(location, ExpressionType::VariableDefinition),
	  variable{variable}, value{value} {
}

FieldDef::FieldDef(SourceLocation location, AccessModifier modifier,
				   bool isStatic, std::u32string name, TypePtr type,
				   ExpPtr value)
	: location{location}, modifier{modifier}, isStatic{isStatic}, name{name},
	  type{type}, value{value} {
}

MethodDef::MethodDef(
	SourceLocation location, AccessModifier modifier, bool isStatic,
	std::u32string name,
	std::vector<std::shared_ptr<ParameterExpression>> parameters,
	TypePtr returnType, ExpPtr body)
	: location{location}, modifier{modifier}, isStatic{isStatic}, name{name},
	  parameters{parameters}, returnType{returnType}, body{body} {
}

ClassInfo::ClassInfo(SourceLocation location, bool isModule,
					 std::u32string name)
	: isModule{isModule}, name{name} {
}

ReturnExpression::ReturnExpression(SourceLocation location, ExpPtr value)
	: Expression(location, ExpressionType::Return), value{value} {
}

BreakExpression::BreakExpression(SourceLocation location)
	: Expression(location, ExpressionType::Break) {
}

WhileExpression::WhileExpression(SourceLocation location, ExpPtr condition,
								 ExpPtr body)
	: Expression(location, ExpressionType::While), condition{condition},
	  body{body} {
}

Program::Program(std::shared_ptr<SourceDocument> document)
	: document{document} {
}

void Program::AddClass(std::shared_ptr<ClassInfo> info) {
	classes.Add(info->name, info);
}

void Program::AddModule(std::shared_ptr<ClassInfo> info) {
	classes.Add(info->name, info);
}
} // namespace cygni