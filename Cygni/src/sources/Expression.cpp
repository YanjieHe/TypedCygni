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

Expression::Expression(SourceLocation location) : location{location} {
	static int currentId = 0;
	this->id			 = currentId;
	currentId++;
}

ConstantExpression::ConstantExpression(SourceLocation location,
									   std::u32string constant)
	: Expression(location), constant{constant} {
}

BinaryExpression::BinaryExpression(SourceLocation location,
								   std::shared_ptr<Expression> left,
								   std::shared_ptr<Expression> right)
	: Expression(location), left{left}, right{right} {
}

UnaryExpression::UnaryExpression(SourceLocation location,
								 std::shared_ptr<Expression> operand)
	: Expression(location), operand{operand} {
}

BlockExpression::BlockExpression(
	SourceLocation location,
	std::vector<std::shared_ptr<Expression>> expressions)
	: Expression(location), expressions{expressions} {
}

ConditionalExpression::ConditionalExpression(
	SourceLocation location, std::shared_ptr<Expression> condition,
	std::shared_ptr<Expression> ifTrue, std::shared_ptr<Expression> ifFalse)
	: Expression(location), condition{condition}, ifTrue{ifTrue}, ifFalse{
																	  ifFalse} {
}

DefaultExpression::DefaultExpression(SourceLocation location)
	: Expression(location) {
}

InvocationExpression::InvocationExpression(
	SourceLocation location, std::shared_ptr<Expression> expression,
	std::vector<std::shared_ptr<Expression>> arguments)
	: Expression(location), expression{expression}, arguments{arguments} {
}

} // namespace cygni