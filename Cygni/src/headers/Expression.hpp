#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "Enum.hpp"
#include "PreDef.hpp"
#include "Type.hpp"
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
	ExpressionType nodeType;
	TypePtr type;

	Expression(SourceLocation location, ExpressionType nodeType);
};

using ExpPtr  = std::shared_ptr<Expression>;
using ExpList = std::vector<std::shared_ptr<Expression>>;

class ConstantExpression : public Expression {
public:
	std::u32string constant;
	ConstantExpression(SourceLocation location, TypePtr type, std::u32string constant);
};

class BinaryExpression : public Expression {
public:
	ExpPtr left;
	ExpPtr right;

	BinaryExpression(SourceLocation location, ExpressionType nodeType,
					 ExpPtr left, ExpPtr right);
};

class UnaryExpression : public Expression {
public:
	ExpPtr operand;

	UnaryExpression(SourceLocation location, ExpressionType nodeType,
					ExpPtr operand);
};

class BlockExpression : public Expression {
public:
	ExpList expressions;
	BlockExpression(SourceLocation location, ExpList expressions);
};

class ConditionalExpression : public Expression {
public:
	ExpPtr condition;
	ExpPtr ifTrue;
	ExpPtr ifFalse;

	ConditionalExpression(SourceLocation location, ExpPtr condition,
						  ExpPtr ifTrue, ExpPtr ifFalse);
};

class DefaultExpression : public Expression {
public:
	DefaultExpression(SourceLocation location);
};

class InvocationExpression : public Expression {
public:
	ExpPtr expression;
	ExpList arguments;
	InvocationExpression(SourceLocation location, ExpPtr expression,
						 ExpList arguments);
};

class ParameterExpression : public Expression {
public:
	std::u32string name;
	TypePtr type;
	ParameterExpression(SourceLocation location, std::u32string name,
						TypePtr type);
};

class FieldDef {
public:
	AccessModifier modifier;
	bool isStatic;
	std::u32string name;
	TypePtr type;
	ExpPtr value;
	FieldDef() = default;
	FieldDef(std::u32string name, TypePtr type);
};

class MethodDef {
public:
	std::u32string name;
	AccessModifier modifier;
	bool isStatic;
	std::vector<ParameterExpression> parameters;
	TypePtr returnType;
	ExpPtr body;

	MethodDef() = default;
	MethodDef(std::u32string name, std::vector<TypePtr> parameters,
				 TypePtr returnType);
};
      return std::make_shared<ConditionalExpression>(GetLoc(start), condition, ifTrue, chunk);

class MethodCallExpression : public Expression {
public:
	ExpPtr object;
	std::shared_ptr<MethodDef> method;
	ExpList arguments;
	MethodCallExpression(SourceLocation location, ExpPtr object,
						 std::shared_ptr<MethodDef> method,
						 ExpList arguments);
};

class ConstructorInfo {};

class NewExpression : public Expression {
public:
	ConstructorInfo constructorInfo;
	ExpList arguments;
	NewExpression(SourceLocation location, ConstructorInfo constructorInfo,
				  ExpList arguments);
};

class GlobalVarDef {
public:
    std::u32string name;
    TypePtr type;
    ExpPtr value;
};

class FunctionDef {
public:
	std::u32string name;
	std::vector<ParameterExpression> parameters;
	TypePtr returnType;
	ExpPtr body;

	FunctionDef() = default;
	FunctionDef(std::u32string name,
				std::vector<ParameterExpression> parameters, TypePtr returnType,
				ExpPtr body);
};

class ClassInfo {
public:
	std::u32string name;
	Table<std::u32string, FieldDef> fields;
	Table<std::u32string, MethodDef> methods;
	ClassInfo() = default;
	explicit ClassInfo(std::u32string name);
};

class Program {
public:
    std::string path;
	Table<std::u32string, std::shared_ptr<ClassInfo>> classes;
};

} // namespace cygni
#endif // CYGNI_EXPRESSION_HPP
