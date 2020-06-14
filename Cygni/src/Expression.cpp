#include "Expression.hpp"
#include "Exception.hpp"
#include <algorithm>


namespace cygni
{

	Expression::Expression(SourcePosition position, ExpressionType nodeType)
		: position{ position }, nodeType{ nodeType } {
		static int currentId = 0;
		this->id = currentId;
		currentId++;
		static std::shared_ptr<UnknownType> unknownType =
			std::make_shared<UnknownType>();
		this->type = unknownType;
	}

	ConstantExpression::ConstantExpression(SourcePosition position, TypePtr type,
		std::u32string constant)
		: Expression(position, ExpressionType::Constant), constant{ constant } {
		this->type = type;
	}

	BinaryExpression::BinaryExpression(SourcePosition position,
		ExpressionType nodeType, ExpPtr left,
		ExpPtr right)
		: Expression(position, nodeType), left{ left }, right{ right } {}

	UnaryExpression::UnaryExpression(SourcePosition position,
		ExpressionType nodeType, ExpPtr operand)
		: Expression(position, nodeType), operand{ operand } {}

	BlockExpression::BlockExpression(SourcePosition position, ExpList expressions)
		: Expression(position, ExpressionType::Block), expressions{ expressions } {}

	ConditionalExpression::ConditionalExpression(SourcePosition position,
		ExpPtr condition, ExpPtr ifTrue,
		ExpPtr ifFalse)
		: Expression(position, ExpressionType::Conditional), condition{ condition },
		ifTrue{ ifTrue }, ifFalse{ ifFalse } {}

	DefaultExpression::DefaultExpression(SourcePosition position, TypePtr type)
		: Expression(position, ExpressionType::Default)
	{
		this->type = type;
	}

	InvocationExpression::InvocationExpression(SourcePosition position,
		ExpPtr expression,
		std::vector<Argument> arguments)
		: Expression(position, ExpressionType::Invoke),
		expression{ expression }, arguments{ arguments } {}

	//MethodCallExpression::MethodCallExpression(SourcePosition position,
	//	ExpPtr object,
	//	std::u32string method,
	//	std::vector<Argument> arguments)
	//	: Expression(position, ExpressionType::MethodCall), object{ object },
	//	method{ method }, arguments{ arguments } {}

	NewExpression::NewExpression(SourcePosition position, TypePtr type,
		std::vector<Argument> arguments)
		: Expression(position, ExpressionType::New), arguments{ arguments }
	{
		this->type = type;
	}

	ParameterExpression::ParameterExpression(SourcePosition position,
		std::u32string name, TypePtr type)
		: Expression(position, ExpressionType::Parameter), name{ name } {
		this->type = type;
	}

	VarDefExpression::VarDefExpression(
		SourcePosition position, std::shared_ptr<ParameterExpression> variable,
		TypePtr type, ExpPtr value)
		: Expression(position, ExpressionType::VariableDefinition),
		variable{ variable }, value{ value } {
		this->type = type;
	}

	FieldDef::FieldDef(SourcePosition position, AccessModifier modifier,
		bool isStatic, Table<std::u32string, AnnotationInfo> annotations,
		std::u32string name, TypePtr type, ExpPtr value)
		: position{ position }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, type{ type }, value{ value } {}

	MethodDef::MethodDef(
		SourcePosition position, AccessModifier modifier, bool isStatic, TypePtr selfType,
		Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: position{ position }, modifier{ modifier }, isStatic{ isStatic },
		selfType{ selfType }, annotations{ annotations }, name{ name }, parameters{ parameters },
		returnType{ returnType }, body{ body }
	{
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(selfType, name, parameterTypes, returnType);
	}

	ClassInfo::ClassInfo(SourcePosition position, PackageRoute route, std::u32string name)
		: position{ position }, route{ route }, name{ name } {}

	ModuleInfo::ModuleInfo(SourcePosition position, PackageRoute route, std::u32string name)
		: position{ position }, route{ route }, name{ name } {}

	ReturnExpression::ReturnExpression(SourcePosition position, ExpPtr value)
		: Expression(position, ExpressionType::Return), value{ value } {}

	BreakExpression::BreakExpression(SourcePosition position)
		: Expression(position, ExpressionType::Break)
	{
	}

	WhileExpression::WhileExpression(SourcePosition position, ExpPtr condition,
		ExpPtr body)
		: Expression(position, ExpressionType::While), condition{ condition },
		body{ body } {}

	AnnotationInfo::AnnotationInfo(SourcePosition position, std::u32string name,
		std::vector<Argument> arguments)
		: position{ position }, name{ name }, arguments{ arguments } {}

	MemberAccessExpression::MemberAccessExpression(SourcePosition position,
		ExpPtr object,
		std::u32string field)
		: Expression(position, ExpressionType::MemberAccess), object{ object },
		field{ field } {}

	Argument::Argument(ExpPtr value) : name(), value{ value } {}

	Argument::Argument(std::u32string name, ExpPtr value)
		: name{ name }, value{ value } {}


	Package::Package(PackageRoute route) : route{ route }
	{
	}


	SourceDocument::SourceDocument(std::shared_ptr<FileLocation> document) : fileLocation{ document }
	{
	}

	TypeAlias::TypeAlias(SourcePosition position, PackageRoute route, std::u32string typeName, std::u32string alias)
		: position{ position }, route{ route }, typeName{ typeName }, alias{ alias }
	{
	}


	PackageRouteStatement::PackageRouteStatement(SourcePosition position, PackageRoute route)
		: position{ position }, route{ route }
	{
	}

	void Project::MergeAllPrograms()
	{
		for (auto program : programs.values)
		{
			auto route = program.packageRoute.route;
			std::shared_ptr<Package> package;
			if (packages.ContainsKey(route))
			{
				// found package
				package = packages.GetValueByKey(route);
			}
			else
			{
				// package not found. create a new package
				package = std::make_shared<Package>(route);
			}
			for (auto classInfo : program.classDefs.values)
			{
				package->classDefs.Add(classInfo->name, classInfo);
			}
			for (auto moduleInfo : program.moduleDefs.values)
			{
				package->moduleDefs.Add(moduleInfo->name, moduleInfo);
			}
			for (auto interfaceInfo : program.interfaceDefs.values)
			{
				package->interfaceDefs.Add(interfaceInfo->name, interfaceInfo);
			}
			for (auto typeAlias : program.typeAliases)
			{
				package->typeAliases.Add(typeAlias.alias, typeAlias);
			}
			for (auto importStatement : program.importedPackages)
			{
				package->importedPackages.push_back(importStatement);
			}
			this->packages.Add(route, package);
		}
	}

	std::optional<std::shared_ptr<ModuleInfo>> Project::GetModule(std::shared_ptr<ModuleType> moduleType)
	{
		if (packages.ContainsKey(moduleType->route))
		{
			auto pkg = packages.GetValueByKey(moduleType->route);
			if (pkg->modules.ContainsKey(moduleType->name))
			{
				return { pkg->modules.GetValueByKey(moduleType->name) };
			}
			else
			{
				return {};
			}
		}
		else
		{
			return {};
		}
	}

	std::optional<std::shared_ptr<ClassInfo>> Project::GetClass(std::shared_ptr<ClassType> classType)
	{
		if (packages.ContainsKey(classType->route))
		{
			auto pkg = packages.GetValueByKey(classType->route);
			if (pkg->classes.ContainsKey(classType->name))
			{
				return { pkg->classes.GetValueByKey(classType->name) };
			}
			else
			{
				return {};
			}
		}
		else
		{
			return {};
		}
	}

	std::optional<std::shared_ptr<InterfaceInfo>> Project::GetInterface(std::shared_ptr<InterfaceType> interfaceType)
	{
		if (packages.ContainsKey(interfaceType->route))
		{
			auto pkg = packages.GetValueByKey(interfaceType->route);
			if (pkg->interfaces.ContainsKey(interfaceType->name))
			{
				return { pkg->interfaces.GetValueByKey(interfaceType->name) };
			}
			else
			{
				return {};
			}
		}
		else
		{
			return {};
		}
	}

	std::optional<TypePtr> Project::ResolveType(std::shared_ptr<UnresolvedType> unresolvedType)
	{
		auto route = unresolvedType->route;
		auto name = unresolvedType->name;
		if (packages.ContainsKey(route))
		{
			auto pkg = packages.GetValueByKey(route);
			if (pkg->classes.ContainsKey(name))
			{
				auto classInfo = pkg->classes.GetValueByKey(name);
				return { std::make_shared<ClassType>(classInfo->route, classInfo->name) };
			}
			else if (pkg->interfaces.ContainsKey(name))
			{
				auto interfaceInfo = pkg->interfaces.GetValueByKey(name);
				return { std::make_shared<InterfaceType>(interfaceInfo->route, interfaceInfo->name) };
			}
			else
			{
				return {};
			}
		}
		else
		{
			return {};
		}
	}

	InterfaceInfo::InterfaceInfo(SourcePosition position, PackageRoute route, std::u32string name)
		: position{ position }, route{ route }, name{ name }
	{
	}

	ImportStatement::ImportStatement() : position{}, route{}
	{
	}

	ImportStatement::ImportStatement(SourcePosition position, PackageRoute route) : position{ position }, route{ route }
	{
	}

} // namespace cygni
