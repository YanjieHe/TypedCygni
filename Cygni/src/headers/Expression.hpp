#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "PreDef.hpp"
#include <memory>
#include <string>
namespace cygni {

class SourceDocument {
public:
	std::string filePath;
	std::string fileName;
};

class SourceLocation {
public:
	std::shared_ptr<SourceDocument> document;
	int startLine;
	int startCol;
	int endLine;
	int endCol;

	SourceLocation();
	SourceLocation(std::shared_ptr<SourceDocument> document, int startLine,
				   int startCol, int endLine, int endCol);
};

class Expression {
public:
	int id;
	SourceLocation location;
	explicit Expression(SourceLocation location);
};

class ConstantExpression : public Expression {
public:
	std::u32string constant;
	ConstantExpression(SourceLocation location, std::u32string constant);
};

class BinaryExpression : public Expression {
public:
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;

	BinaryExpression(SourceLocation location, std::shared_ptr<Expression> left,
					 std::shared_ptr<Expression> right);
};

class UnaryExpression : public Expression {
public:
	std::shared_ptr<Expression> operand;

	UnaryExpression(SourceLocation location,
					std::shared_ptr<Expression> operand);
};

class BlockExpression : public Expression {
public:
	std::vector<std::shared_ptr<Expression>> expressions;
	BlockExpression(SourceLocation location,
					std::vector<std::shared_ptr<Expression>> expressions);
};

class ConditionalExpression : public Expression {
public:
	std::shared_ptr<Expression> condition;
	std::shared_ptr<Expression> ifTrue;
	std::shared_ptr<Expression> ifFalse;

	ConditionalExpression(SourceLocation location,
						  std::shared_ptr<Expression> condition,
						  std::shared_ptr<Expression> ifTrue,
						  std::shared_ptr<Expression> ifFalse);
};

class DefaultExpression : public Expression {
public:
	DefaultExpression(SourceLocation location);
};

class InvocationExpression : public Expression {
public:
	std::shared_ptr<Expression> expression;
	std::vector<std::shared_ptr<Expression>> arguments;
	InvocationExpression(SourceLocation location,
						 std::shared_ptr<Expression> expression,
						 std::vector<std::shared_ptr<Expression>> arguments);
};

} // namespace cygni
#endif // CYGNI_EXPRESSION_HPP
