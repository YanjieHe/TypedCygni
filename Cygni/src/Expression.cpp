#include "Expression.hpp"
#include "Exception.hpp"
#include <algorithm>


namespace cygni
{

	Expression::Expression(SourceLocation location, ExpressionType nodeType)
		: location{ location }, nodeType{ nodeType } {
		static int currentId = 0;
		this->id = currentId;
		currentId++;
		static std::shared_ptr<UnknownType> unknownType =
			std::make_shared<UnknownType>();
		this->type = unknownType;
	}

	ConstantExpression::ConstantExpression(SourceLocation location, TypePtr type,
		std::u32string constant)
		: Expression(location, ExpressionType::Constant), constant{ constant } {
		this->type = type;
	}

	BinaryExpression::BinaryExpression(SourceLocation location,
		ExpressionType nodeType, ExpPtr left,
		ExpPtr right)
		: Expression(location, nodeType), left{ left }, right{ right } {}

	UnaryExpression::UnaryExpression(SourceLocation location,
		ExpressionType nodeType, ExpPtr operand)
		: Expression(location, nodeType), operand{ operand } {}

	BlockExpression::BlockExpression(SourceLocation location, ExpList expressions)
		: Expression(location, ExpressionType::Block), expressions{ expressions } {}

	ConditionalExpression::ConditionalExpression(SourceLocation location,
		ExpPtr condition, ExpPtr ifTrue,
		ExpPtr ifFalse)
		: Expression(location, ExpressionType::Conditional), condition{ condition },
		ifTrue{ ifTrue }, ifFalse{ ifFalse } {}

	DefaultExpression::DefaultExpression(SourceLocation location, TypePtr type)
		: Expression(location, ExpressionType::Default)
	{
		this->type = type;
	}

	InvocationExpression::InvocationExpression(SourceLocation location,
		ExpPtr expression,
		std::vector<Argument> arguments)
		: Expression(location, ExpressionType::Invoke),
		expression{ expression }, arguments{ arguments } {}

	MethodCallExpression::MethodCallExpression(SourceLocation location,
		ExpPtr object,
		std::shared_ptr<MethodDef> method,
		ExpList arguments)
		: Expression(location, ExpressionType::MethodCall), object{ object },
		method{ method }, arguments{ arguments } {}

	NewExpression::NewExpression(SourceLocation location, TypePtr type,
		std::vector<Argument> arguments)
		: Expression(location, ExpressionType::New), arguments{ arguments }
	{
		this->type = type;
	}

	ParameterExpression::ParameterExpression(SourceLocation location,
		std::u32string name, TypePtr type)
		: Expression(location, ExpressionType::Parameter), name{ name } {
		this->type = type;
	}

	VarDefExpression::VarDefExpression(
		SourceLocation location, std::shared_ptr<ParameterExpression> variable,
		TypePtr type, ExpPtr value)
		: Expression(location, ExpressionType::VariableDefinition),
		variable{ variable }, value{ value } {
		this->type = type;
	}

	FieldDef::FieldDef(SourceLocation location, AccessModifier modifier,
		bool isStatic, Table<std::u32string, AnnotationInfo> annotations,
		std::u32string name, TypePtr type, ExpPtr value)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, type{ type }, value{ value } {}

	MethodDef::MethodDef(
		SourceLocation location, AccessModifier modifier, bool isStatic, TypePtr selfType,
		Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		selfType{ selfType }, annotations{ annotations }, name{ name }, parameters{ parameters },
		returnType{ returnType }, body{ body }
	{
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(selfType, parameterTypes, returnType);
	}

	ClassInfo::ClassInfo(SourceLocation location, PackageRoute route, std::u32string name)
		: location{ location }, route{ route }, name{ name } {}

	ModuleInfo::ModuleInfo(SourceLocation location, PackageRoute route, std::u32string name)
		: location{ location }, route{ route }, name{ name } {}

	ReturnExpression::ReturnExpression(SourceLocation location, ExpPtr value)
		: Expression(location, ExpressionType::Return), value{ value } {}

	BreakExpression::BreakExpression(SourceLocation location)
		: Expression(location, ExpressionType::Break)
	{
	}

	WhileExpression::WhileExpression(SourceLocation location, ExpPtr condition,
		ExpPtr body)
		: Expression(location, ExpressionType::While), condition{ condition },
		body{ body } {}

	AnnotationInfo::AnnotationInfo(SourceLocation location, std::u32string name,
		std::vector<Argument> arguments)
		: location{ location }, name{ name }, arguments{ arguments } {}

	MemberAccessExpression::MemberAccessExpression(SourceLocation location,
		ExpPtr object,
		std::u32string field)
		: Expression(location, ExpressionType::MemberAccess), object{ object },
		field{ field } {}

	//ConstructorInfo::ConstructorInfo(
	//	SourceLocation location, AccessModifier modifier,
	//	Table<std::u32string, AnnotationInfo> annotations, std::u32string name,
	//	std::vector<std::shared_ptr<ParameterExpression>> parameters,
	//	TypePtr returnType, ExpPtr body)
	//	: location{ location }, modifier{ modifier }, annotations{ annotations },
	//	name{ name }, parameters{ parameters }, returnType{ returnType }, body{ body } {
	//	std::vector<TypePtr> parameterTypes(parameters.size());
	//	std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
	//		[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
	//	{
	//		return p->type;
	//	});
	//	this->signature = std::make_shared<FunctionType>(parameterTypes, returnType);
	//}

	Argument::Argument(ExpPtr value) : name(), value{ value } {}

	Argument::Argument(std::optional<std::u32string> name, ExpPtr value)
		: name{ name }, value{ value } {}


	ParameterLocation::ParameterLocation() : type{ ParameterType::Unknown }, offset{ -1 }, index{ -1 }
	{
	}

	ParameterLocation::ParameterLocation(ParameterType type, int offset) : type{ type }, offset{ offset }, index{ -1 }
	{
	}

	ParameterLocation::ParameterLocation(ParameterType type, int offset, int index) : type{ type }, offset{ offset }, index{ index }
	{
	}

	Package::Package(PackageRoute route) : route{ route }
	{
	}


	SourceDocument::SourceDocument(std::shared_ptr<FileLocation> document) : fileLocation{ document }
	{
	}

	TypeAlias::TypeAlias(SourceLocation location, PackageRoute route, std::u32string typeName, std::u32string alias)
		: location{ location }, route{ route }, typeName{ typeName }, alias{ alias }
	{
	}


	PackageRouteStatement::PackageRouteStatement(SourceLocation location, PackageRoute route)
		: location{ location }, route{ route }
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

	std::optional<std::shared_ptr<ModuleInfo>> Project::GetModule(PackageRoute route, std::u32string name)
	{
		if (packages.ContainsKey(route))
		{
			auto pkg = packages.GetValueByKey(route);
			if (pkg->modules.ContainsKey(name))
			{
				return { pkg->modules.GetValueByKey(name) };
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

	std::optional<std::shared_ptr<ClassInfo>> Project::GetClass(PackageRoute route, std::u32string name)
	{
		if (packages.ContainsKey(route))
		{
			auto pkg = packages.GetValueByKey(route);
			if (pkg->classes.ContainsKey(name))
			{
				return { pkg->classes.GetValueByKey(name) };
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

	std::optional<std::shared_ptr<InterfaceInfo>> Project::GetInterface(PackageRoute route, std::u32string name)
	{
		if (packages.ContainsKey(route))
		{
			auto pkg = packages.GetValueByKey(route);
			if (pkg->interfaces.ContainsKey(name))
			{
				return { pkg->interfaces.GetValueByKey(name) };
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

	InterfaceInfo::InterfaceInfo(SourceLocation location, std::u32string name)
		: location{ location }, name{ name }
	{
	}

	ImportStatement::ImportStatement() : location{}, route{}
	{
	}

	ImportStatement::ImportStatement(SourceLocation location, PackageRoute route) : location{ location }, route{ route }
	{
	}

} // namespace cygni
