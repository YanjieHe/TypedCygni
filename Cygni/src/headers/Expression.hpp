#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "Enum.hpp"
#include "PreDef.hpp"
#include "Scope.hpp"
#include "Type.hpp"
#include <memory>
#include <string>

namespace cygni {

class Expression;
using ExpPtr  = std::shared_ptr<Expression>;
using ExpList = std::vector<std::shared_ptr<Expression>>;

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

class ConstantExpression : public Expression {
public:
	std::u32string constant;
	ConstantExpression(SourceLocation location, TypePtr type,
					   std::u32string constant);
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
	std::shared_ptr<Scope> scope;
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
	ParameterExpression(SourceLocation location, std::u32string name,
						TypePtr type);
};

class VariableDefinitionExpression : public Expression {
public:
	ParameterExpression variable;
	ExpPtr value;
	VariableDefinitionExpression(SourceLocation location,
								 ParameterExpression variable, ExpPtr value);
};

class FieldDef {
public:
	SourceLocation location;
	AccessModifier modifier;
	bool isStatic;
	std::u32string name;
	TypePtr type;
	ExpPtr value;
	FieldDef() = default;
	FieldDef(SourceLocation location, AccessModifier modifier, bool isStatic,
			 std::u32string name, TypePtr type, ExpPtr value);
};

class MethodDef {
public:
	SourceLocation location;
	AccessModifier modifier;
	bool isStatic;
	std::u32string name;
	std::vector<ParameterExpression> parameters;
	TypePtr returnType;
	ExpPtr body;

	MethodDef() = default;
	MethodDef(SourceLocation location, AccessModifier modifier, bool isStatic,
			  std::u32string name, std::vector<ParameterExpression> parameters,
			  TypePtr returnType, ExpPtr body);
};

class MethodCallExpression : public Expression {
public:
	ExpPtr object;
	std::shared_ptr<MethodDef> method;
	ExpList arguments;
	MethodCallExpression(SourceLocation location, ExpPtr object,
						 std::shared_ptr<MethodDef> method, ExpList arguments);
};

class ConstructorInfo {};

class NewExpression : public Expression {
public:
	ConstructorInfo constructorInfo;
	ExpList arguments;
	NewExpression(SourceLocation location, ConstructorInfo constructorInfo,
				  ExpList arguments);
};

class ReturnExpression : public Expression {
public:
	ExpPtr value;
	ReturnExpression(SourceLocation location, ExpPtr value);
};

class WhileExpression : public Expression {
public:
	ExpPtr condition;
	ExpPtr body;
	WhileExpression(SourceLocation location, ExpPtr condition, ExpPtr body);
};

class ClassInfo {
public:
	bool isModule;
	std::u32string name;
	Table<std::u32string, FieldDef> fields;
	Table<std::u32string, MethodDef> methods;
	ClassInfo() = default;
	ClassInfo(SourceLocation location, bool isModule, std::u32string name);
};

class Program {
public:
	std::string path;
	Table<std::u32string, std::shared_ptr<ClassInfo>> classes;
	explicit Program(std::string path);

	void AddClass(std::shared_ptr<ClassInfo> info);

	void AddModule(std::shared_ptr<ClassInfo> info);
};

} // namespace cygni
#endif // CYGNI_EXPRESSION_HPP
