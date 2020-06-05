#ifndef CYGNI_EXPRESSION_HPP
#define CYGNI_EXPRESSION_HPP
#include "Enum.hpp"
#include "Scope.hpp"
#include "SourcePosition.hpp"
#include "Type.hpp"
#include <iostream>
#include "Location.hpp"

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
		SourcePosition position;
		ExpressionType nodeType;
		TypePtr type;

		Expression(SourcePosition position, ExpressionType nodeType);
	};

	class ConstantExpression : public Expression
	{
	public:
		std::u32string constant;
		ConstantExpression(SourcePosition position, TypePtr type,
			std::u32string constant);
	};

	class BinaryExpression : public Expression
	{
	public:
		ExpPtr left;
		ExpPtr right;

		BinaryExpression(SourcePosition position, ExpressionType nodeType,
			ExpPtr left, ExpPtr right);
	};

	class UnaryExpression : public Expression
	{
	public:
		ExpPtr operand;

		UnaryExpression(SourcePosition position, ExpressionType nodeType,
			ExpPtr operand);
	};

	class BlockExpression : public Expression
	{
	public:
		ExpList expressions;
		BlockExpression(SourcePosition position, ExpList expressions);
	};

	class ConditionalExpression : public Expression
	{
	public:
		ExpPtr condition;
		ExpPtr ifTrue;
		ExpPtr ifFalse;

		ConditionalExpression(SourcePosition position, ExpPtr condition,
			ExpPtr ifTrue, ExpPtr ifFalse);
	};

	class DefaultExpression : public Expression
	{
	public:
		DefaultExpression(SourcePosition position, TypePtr type);
	};

	class Argument
	{
	public:
		std::optional<std::u32string> name;
		ExpPtr value;
		std::optional<int> index; // parameter index

		Argument() = default;
		explicit Argument(ExpPtr value);
		Argument(std::optional<std::u32string> name, ExpPtr value);
	};

	class InvocationExpression : public Expression
	{
	public:
		ExpPtr expression;
		std::vector<Argument> arguments;
		InvocationExpression(SourcePosition position, ExpPtr expression,
			std::vector<Argument> arguments);
	};

	class ParameterExpression : public Expression
	{
	public:
		std::u32string name;
		LocationPtr location;
		ParameterExpression(SourcePosition position, std::u32string name,
			TypePtr type);
	};

	class VarDefExpression : public Expression
	{
	public:
		std::shared_ptr<ParameterExpression> variable;
		ExpPtr value;

		VarDefExpression(SourcePosition position,
			std::shared_ptr<ParameterExpression> variable, TypePtr type,
			ExpPtr value);
	};

	class AnnotationInfo
	{
	public:
		SourcePosition position;
		std::u32string name;
		std::vector<Argument> arguments;

		AnnotationInfo(SourcePosition position, std::u32string name,
			std::vector<Argument> arguments);
	};

	class FieldDef
	{
	public:
		SourcePosition position;
		AccessModifier modifier;
		bool isStatic;
		Table<std::u32string, AnnotationInfo> annotations;
		std::u32string name;
		TypePtr type;
		ExpPtr value;
		std::optional<int> index;
		FieldDef() = default;
		FieldDef(SourcePosition position, AccessModifier modifier, bool isStatic,
			Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
			TypePtr type, ExpPtr value);
	};

	class MethodDef
	{
	public:
		SourcePosition position;
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
		std::optional<int> index;

		MethodDef() = default;
		MethodDef(SourcePosition position, AccessModifier modifier, bool isStatic, TypePtr selfType,
			Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
			std::vector<std::shared_ptr<ParameterExpression>> parameters,
			TypePtr returnType, ExpPtr body);
	};

	class MemberAccessExpression : public Expression
	{
	public:
		ExpPtr object;
		std::u32string field;
		LocationPtr location;
		MemberAccessExpression(SourcePosition position, ExpPtr object,
			std::u32string field);
	};

	//class MethodCallExpression : public Expression
	//{
	//public:
	//	ExpPtr object;
	//	std::u32string method;
	//	std::vector<Argument> arguments;
	//	LocationPtr location;
	//	MethodCallExpression(SourcePosition position, ExpPtr object,
	//		std::u32string method, std::vector<Argument> arguments);
	//};

	class ReturnExpression : public Expression
	{
	public:
		ExpPtr value;
		ReturnExpression(SourcePosition position, ExpPtr value);
	};

	class BreakExpression : public Expression
	{
	public:
		BreakExpression(SourcePosition position);
	};

	class WhileExpression : public Expression
	{
	public:
		ExpPtr condition;
		ExpPtr body;
		WhileExpression(SourcePosition position, ExpPtr condition, ExpPtr body);
	};

	class NewExpression : public Expression
	{
	public:
		std::vector<Argument> arguments;
		LocationPtr location;
		NewExpression(SourcePosition position, TypePtr type,
			std::vector<Argument> arguments);
	};

	class ClassInfo
	{
	public:
		SourcePosition position;
		PackageRoute route;
		std::u32string name;

		/* definitions */
		Table<std::u32string, FieldDef> fieldDefs;
		Table<std::u32string, MethodDef> methodDefs;

		/* all information */
		Table<std::u32string, FieldDef> fields;
		Table<std::u32string, MethodDef> methods;

		/* virtual function table */
		std::unordered_map<int, std::vector<MethodDef>> virtualMethodTable;

		std::unordered_map<ConstantKey, int> constantMap;
		std::vector<TypePtr> superClasses;
		std::optional<int> index;

		ClassInfo() = default;
		ClassInfo(SourcePosition position, PackageRoute route, std::u32string name);
	};

	class ModuleInfo
	{
	public:
		SourcePosition position;
		PackageRoute route;
		std::u32string name;
		Table<std::u32string, FieldDef> fields;
		Table<std::u32string, MethodDef> methods;
		std::optional<int> index;
		std::unordered_map<ConstantKey, int> constantMap;
		ModuleInfo() = default;
		ModuleInfo(SourcePosition position, PackageRoute route, std::u32string name);
	};

	class InterfaceInfo
	{
	public:
		SourcePosition position;
		std::u32string name;
		Table<std::u32string, MethodDef> methods;
		Table<std::u32string, MethodDef> methodMap;
		std::vector<TypePtr> superInterfaces;
		InterfaceInfo() = default;
		InterfaceInfo(SourcePosition position, std::u32string name);
	};

	class TypeAlias
	{
	public:
		SourcePosition position;
		PackageRoute route;
		std::u32string typeName;
		std::u32string alias;

		TypeAlias() = default;
		TypeAlias(SourcePosition position, PackageRoute route, std::u32string typeName, std::u32string alias);
	};

	class PackageRouteStatement
	{
	public:
		SourcePosition position;
		PackageRoute route;
		PackageRouteStatement() = default;
		PackageRouteStatement(SourcePosition position, PackageRoute route);
	};

	class ImportStatement
	{
	public:
		SourcePosition position;
		PackageRoute route;

		ImportStatement();
		ImportStatement(SourcePosition position, PackageRoute route);
	};

	class Package
	{
	public:
		PackageRoute route;
		std::vector<ImportStatement> importedPackages;
		Table<std::u32string, TypeAlias> typeAliases;

		/* Definitions */
		Table<std::u32string, std::shared_ptr<ClassInfo>> classDefs;
		Table<std::u32string, std::shared_ptr<ModuleInfo>> moduleDefs;
		Table<std::u32string, std::shared_ptr<InterfaceInfo>> interfaceDefs;

		/* All Information */
		Table<std::u32string, std::shared_ptr<ClassInfo>> classes;
		Table<std::u32string, std::shared_ptr<ModuleInfo>> modules;
		Table<std::u32string, std::shared_ptr<InterfaceInfo>> interfaces;

		explicit Package(PackageRoute route);
	};

	class SourceDocument
	{
	public:
		std::shared_ptr<FileLocation> fileLocation;
		PackageRouteStatement packageRoute;
		std::vector<ImportStatement> importedPackages;
		Table<std::u32string, TypeAlias> typeAliases;
		Table<std::u32string, std::shared_ptr<ClassInfo>> classDefs;
		Table<std::u32string, std::shared_ptr<ModuleInfo>> moduleDefs;
		Table<std::u32string, std::shared_ptr<InterfaceInfo>> interfaceDefs;

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
