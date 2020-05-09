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

	NewExpression::NewExpression(SourceLocation location, std::u32string name,
		std::vector<Argument> arguments)
		: Expression(location, ExpressionType::New), name{ name }, arguments{
																	 arguments } {}

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
		bool isStatic, std::vector<AnnotationInfo> annotations,
		std::u32string name, TypePtr type, ExpPtr value)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, type{ type }, value{ value } {}

	MethodDef::MethodDef(
		SourceLocation location, AccessModifier modifier, bool isStatic,
		std::vector<AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, parameters{ parameters },
		returnType{ returnType }, body{ body } {
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(parameterTypes, returnType);
	}

	ClassInfo::ClassInfo(SourceLocation location, std::u32string name)
		: location{ location }, name{ name } {}

	ModuleInfo::ModuleInfo(SourceLocation location, std::u32string name)
		: location{ location }, name{ name } {}

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

	//Program::Program(std::shared_ptr<SourceDocument> document)
	//	: document{ document }
	//{
	//}

	//void Program::AddClass(std::shared_ptr<ClassInfo> info)
	//{
	//	classes.Add(info->name, info);
	//}

	//void Program::AddModule(std::shared_ptr<ModuleInfo> info)
	//{
	//	modules.Add(info->name, info);
	//}

	AnnotationInfo::AnnotationInfo(SourceLocation location, std::u32string name,
		std::vector<Argument> arguments)
		: location{ location }, name{ name }, arguments{ arguments } {}

	MemberAccessExpression::MemberAccessExpression(SourceLocation location,
		ExpPtr object,
		std::u32string field)
		: Expression(location, ExpressionType::MemberAccess), object{ object },
		field{ field } {}

	ConstructorInfo::ConstructorInfo(
		SourceLocation location, AccessModifier modifier,
		std::vector<AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: location{ location }, modifier{ modifier }, annotations{ annotations },
		name{ name }, parameters{ parameters }, returnType{ returnType }, body{ body } {
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(parameterTypes, returnType);
	}

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


	Program::Program(std::shared_ptr<SourceDocument> document) : document{ document }
	{
	}

	TypeAlias::TypeAlias(PackageRoute route, std::u32string typeName, std::u32string alias)
		: route{ route }, typeName{ typeName }, alias{ alias }
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
			if (packages.find(route) != packages.end())
			{
				// found package
				package = packages.at(route);
			}
			else
			{
				// package not found. create a new package
				package = std::make_shared<Package>(route);
			}
			for (auto _class : program.classes.values)
			{
				package->classes.Add(_class->name, _class);
			}
			for (auto module : program.modules.values)
			{
				package->modules.Add(module->name, module);
			}
			for (auto pair : program.typeAliases)
			{
				package->typeAliases.insert(pair);
			}
			for (auto route : program.importedPackages)
			{
				package->importedPackages.push_back(route);
			}
			this->packages.insert({ route, package });
		}
	}

	std::optional<std::shared_ptr<ModuleInfo>> Project::GetModule(PackageRoute route, std::u32string name)
	{
		if (packages.find(route) != packages.end())
		{
			if (packages.at(route)->modules.ContainsKey(name))
			{
				return { packages.at(route)->modules.GetValueByKey(name) };
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
		if (packages.find(route) != packages.end())
		{
			if (packages.at(route)->classes.ContainsKey(name))
			{
				return { packages.at(route)->classes.GetValueByKey(name) };
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

} // namespace cygni
