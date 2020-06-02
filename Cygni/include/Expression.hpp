#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "Enum.hpp"
#include "Scope.hpp"
#include "SourceLocation.hpp"
#include "Type.hpp"
#include <iostream>

namespace cygni
{
	class ConstantKey
	{
	public:
		TypeCode typeCode;
		std::u32string constant;
	};
}

namespace std
{
	template <>
	struct hash<cygni::ConstantKey>
	{
		std::hash<int> h1;
		std::hash<std::u32string> h2;
		std::size_t operator()(const cygni::ConstantKey& key) const
		{
			return h1(static_cast<int>(key.typeCode)) ^ (h2(key.constant) << 1);
		}
	};

	template <>
	struct equal_to<cygni::ConstantKey>
	{
		bool operator()(const cygni::ConstantKey& lhs, const cygni::ConstantKey& rhs) const
		{
			return lhs.typeCode == rhs.typeCode && lhs.constant == rhs.constant;
		}
	};
}
namespace cygni
{

	class Expression;
	using ExpPtr = std::shared_ptr<Expression>;
	using ExpList = std::vector<std::shared_ptr<Expression>>;

	class Expression
	{
	public:
		int id;
		SourceLocation location;
		ExpressionType nodeType;
		TypePtr type;

		Expression(SourceLocation location, ExpressionType nodeType);
	};

	class ConstantExpression : public Expression
	{
	public:
		std::u32string constant;
		ConstantExpression(SourceLocation location, TypePtr type,
			std::u32string constant);
	};

	class BinaryExpression : public Expression
	{
	public:
		ExpPtr left;
		ExpPtr right;

		BinaryExpression(SourceLocation location, ExpressionType nodeType,
			ExpPtr left, ExpPtr right);
	};

	class UnaryExpression : public Expression
	{
	public:
		ExpPtr operand;

		UnaryExpression(SourceLocation location, ExpressionType nodeType,
			ExpPtr operand);
	};

	class BlockExpression : public Expression
	{
	public:
		ExpList expressions;
		std::shared_ptr<Scope> scope;
		BlockExpression(SourceLocation location, ExpList expressions);
	};

	class ConditionalExpression : public Expression
	{
	public:
		ExpPtr condition;
		ExpPtr ifTrue;
		ExpPtr ifFalse;

		ConditionalExpression(SourceLocation location, ExpPtr condition,
			ExpPtr ifTrue, ExpPtr ifFalse);
	};

	class DefaultExpression : public Expression
	{
	public:
		DefaultExpression(SourceLocation location, TypePtr type);
	};

	class Argument
	{
	public:
		std::optional<std::u32string> name;
		ExpPtr value;
		int index = -1; // parameter index

		Argument() = default;
		explicit Argument(ExpPtr value);
		Argument(std::optional<std::u32string> name, ExpPtr value);
	};

	class InvocationExpression : public Expression
	{
	public:
		ExpPtr expression;
		std::vector<Argument> arguments;
		InvocationExpression(SourceLocation location, ExpPtr expression,
			std::vector<Argument> arguments);
	};

	class ParameterLocation
	{
	public:
		ParameterType type;
		int offset;
		// module of class index
		int index;
		ParameterLocation();
		ParameterLocation(ParameterType type, int offset);
		ParameterLocation(ParameterType type, int offset, int index);
	};

	class ParameterExpression : public Expression
	{
	public:
		std::u32string name;
		ParameterLocation parameterLocation;
		ParameterExpression(SourceLocation location, std::u32string name,
			TypePtr type);
	};

	class VarDefExpression : public Expression
	{
	public:
		std::shared_ptr<ParameterExpression> variable;
		ExpPtr value;

		VarDefExpression(SourceLocation location,
			std::shared_ptr<ParameterExpression> variable, TypePtr type,
			ExpPtr value);
	};

	class AnnotationInfo
	{
	public:
		SourceLocation location;
		std::u32string name;
		std::vector<Argument> arguments;

		AnnotationInfo(SourceLocation location, std::u32string name,
			std::vector<Argument> arguments);
	};

	class FieldDef
	{
	public:
		SourceLocation location;
		AccessModifier modifier;
		bool isStatic;
		Table<std::u32string, AnnotationInfo> annotations;
		std::u32string name;
		TypePtr type;
		ExpPtr value;
		int index = -1;
		FieldDef() = default;
		FieldDef(SourceLocation location, AccessModifier modifier, bool isStatic,
			Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
			TypePtr type, ExpPtr value);
	};

	class MethodDef
	{
	public:
		SourceLocation location;
		AccessModifier modifier;
		bool isStatic;
		TypePtr selfType;
		Table<std::u32string, AnnotationInfo> annotations;
		std::u32string name;
		std::vector<std::shared_ptr<ParameterExpression>> parameters;
		TypePtr returnType;
		ExpPtr body;
		TypePtr signature;
		std::vector<std::shared_ptr<VarDefExpression>> localVariables;
		int index = -1;

		MethodDef() = default;
		MethodDef(SourceLocation location, AccessModifier modifier, bool isStatic, TypePtr selfType,
			Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
			std::vector<std::shared_ptr<ParameterExpression>> parameters,
			TypePtr returnType, ExpPtr body);
	};

	//class ConstructorInfo
	//{
	//public:
	//	SourceLocation location;
	//	AccessModifier modifier;
	//	Table<std::u32string, AnnotationInfo> annotations;
	//	std::u32string name;
	//	std::vector<std::shared_ptr<ParameterExpression>> parameters;
	//	TypePtr returnType;
	//	ExpPtr body;
	//	TypePtr signature;
	//	ConstructorInfo() = default;
	//	ConstructorInfo(SourceLocation location, AccessModifier modifier,
	//		Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
	//		std::vector<std::shared_ptr<ParameterExpression>> parameters,
	//		TypePtr returnType, ExpPtr body);
	//};

	class MemberAccessExpression : public Expression
	{
	public:
		ExpPtr object;
		std::u32string field;
		ParameterLocation parameterLocation;
		MemberAccessExpression(SourceLocation location, ExpPtr object,
			std::u32string field);
	};

	class MethodCallExpression : public Expression
	{
	public:
		ExpPtr object;
		std::shared_ptr<MethodDef> method;
		ExpList arguments;
		MethodCallExpression(SourceLocation location, ExpPtr object,
			std::shared_ptr<MethodDef> method, ExpList arguments);
	};

	class ReturnExpression : public Expression
	{
	public:
		ExpPtr value;
		ReturnExpression(SourceLocation location, ExpPtr value);
	};

	class BreakExpression : public Expression
	{
	public:
		BreakExpression(SourceLocation location);
	};

	class WhileExpression : public Expression
	{
	public:
		ExpPtr condition;
		ExpPtr body;
		WhileExpression(SourceLocation location, ExpPtr condition, ExpPtr body);
	};

	class NewExpression : public Expression
	{
	public:
		std::vector<Argument> arguments;
		ParameterLocation parameterLocation;
		NewExpression(SourceLocation location, TypePtr type,
			std::vector<Argument> arguments);
	};

	class ClassInfo
	{
	public:
		SourceLocation location;
		std::u32string name;
		Table<std::u32string, FieldDef> fields;
		Table<std::u32string, MethodDef> methods;
		Table<std::u32string, FieldDef> allFields;
		int index = -1;
		std::unordered_map<ConstantKey, int> constantMap;
		std::vector<TypePtr> superClasses;
		ClassInfo() = default;
		ClassInfo(SourceLocation location, std::u32string name);
	};

	class ModuleInfo
	{
	public:
		SourceLocation location;
		std::u32string name;
		Table<std::u32string, FieldDef> fields;
		Table<std::u32string, MethodDef> methods;
		int index = -1;
		std::unordered_map<ConstantKey, int> constantMap;
		ModuleInfo() = default;
		ModuleInfo(SourceLocation location, std::u32string name);
	};

	class InterfaceInfo
	{
	public:
		SourceLocation location;
		std::u32string name;
		Table<std::u32string, MethodDef> methods;
		Table<std::u32string, MethodDef> methodMap;
		std::vector<TypePtr> superInterfaces;
		InterfaceInfo() = default;
		InterfaceInfo(SourceLocation location, std::u32string name);
	};

	class TypeAlias
	{
	public:
		SourceLocation location;
		PackageRoute route;
		std::u32string typeName;
		std::u32string alias;

		TypeAlias() = default;
		TypeAlias(SourceLocation location, PackageRoute route, std::u32string typeName, std::u32string alias);
	};

	class PackageRouteStatement
	{
	public:
		SourceLocation location;
		PackageRoute route;
		PackageRouteStatement() = default;
		PackageRouteStatement(SourceLocation location, PackageRoute route);
	};

	class ImportStatement
	{
	public:
		SourceLocation location;
		PackageRoute route;

		ImportStatement();
		ImportStatement(SourceLocation location, PackageRoute route);
	};

	class Package
	{
	public:
		PackageRoute route;
		std::vector<ImportStatement> importedPackages;
		Table<std::u32string, TypeAlias> typeAliases;

		Table<std::u32string, std::shared_ptr<ClassInfo>> classes;
		Table<std::u32string, std::shared_ptr<ModuleInfo>> modules;
		Table<std::u32string, std::shared_ptr<InterfaceInfo>> interfaces;

		//std::unordered_map<std::u32string, std::shared_ptr<ModuleInfo>> moduleMap;
		//std::unordered_map<std::u32string, std::shared_ptr<ClassInfo>> classMap;
		//std::unordered_map<std::u32string, std::shared_ptr<InterfaceInfo>> interfaceMap;

		explicit Package(PackageRoute route);
	};

	class SourceDocument
	{
	public:
		std::shared_ptr<FileLocation> fileLocation;
		PackageRouteStatement packageRoute;
		std::vector<ImportStatement> importedPackages;
		Table<std::u32string, TypeAlias> typeAliases;
		Table<std::u32string, std::shared_ptr<ClassInfo>> classes;
		Table<std::u32string, std::shared_ptr<ModuleInfo>> modules;
		Table<std::u32string, std::shared_ptr<InterfaceInfo>> interfaces;

		explicit SourceDocument(std::shared_ptr<FileLocation> document);
	};

	class Project
	{
	public:
		Table<std::string, SourceDocument> programs;
		Table<PackageRoute, std::shared_ptr<Package>> packages;

		void MergeAllPrograms();
		std::optional<std::shared_ptr<ModuleInfo>> GetModule(PackageRoute route, std::u32string name);
		std::optional<std::shared_ptr<ClassInfo>> GetClass(PackageRoute route, std::u32string name);
		std::optional<std::shared_ptr<InterfaceInfo>> GetInterface(PackageRoute route, std::u32string name);
	};

} // namespace cygni

#endif // CYGNI_EXPRESSION_HPP
