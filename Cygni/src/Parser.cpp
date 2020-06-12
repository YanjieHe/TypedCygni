#include "Parser.hpp"
using std::unordered_map;
using std::vector;

namespace cygni
{
	Parser::Parser(std::vector<Token> tokens, std::shared_ptr<FileLocation> document)
		: tokens{ tokens }, document{ document }, offset{ 0 } {}

	SourceDocument Parser::ParseProgram()
	{
		SourceDocument program(document);
		program.packageRoute = ParsePackageRouteStatement();
		program.importedPackages = ParseImportedPackages();
		program.typeAliases = ParseTypeAliases();
		route = program.packageRoute.route;
		while (!IsEof())
		{
			if (Look().tag == Tag::Class)
			{
				auto classInfo = ParseDefClass();
				program.classDefs.Add(classInfo->name, classInfo);
			}
			else if (Look().tag == Tag::Module)
			{
				auto moduleInfo = ParseDefModule();
				program.moduleDefs.Add(moduleInfo->name, moduleInfo);
			}
			else if (Look().tag == Tag::Interface)
			{
				auto interfaceInfo = ParseDefInterface();
				program.interfaceDefs.Add(interfaceInfo->name, interfaceInfo);
			}
			else
			{
				throw ParserException(document, Look().line, Look().column, U"expecting 'class', 'module' or 'interface'");
			}
		}
		return program;
	}

	PackageRouteStatement Parser::ParsePackageRouteStatement()
	{
		auto location = Pos(Look());
		Match(Tag::Package);
		auto route = ParsePackageRoute();
		return { location, route };
	}

	PackageRoute Parser::ParsePackageRoute()
	{
		PackageRoute route;
		route.push_back(Match(Tag::Identifier).text);
		while (Look().tag == Tag::Dot)
		{
			Match(Tag::Dot);
			route.push_back(Match(Tag::Identifier).text);
		}
		return route;
	}

	ExpPtr Parser::Statement()
	{
		switch (Look().tag)
		{
		case Tag::If:
			return IfStatement();
		case Tag::Var:
			return ParseVar();
		case Tag::While:
			return ParseWhile();
		case Tag::Return:
			return ParseReturn();
		default:
			return ParseAssign();
		}
	}

	ExpPtr Parser::ParseAssign()
	{
		const Token &start = Look();
		auto x = ParseOr();
		if (Look().tag == Tag::Assign)
		{
			Match(Tag::Assign);
			auto y = ParseOr();
			return std::make_shared<BinaryExpression>(Pos(start),
				ExpressionType::Assign, x, y);
		}
		else
		{
			return x;
		}
	}

	ExpPtr Parser::ParseOr()
	{
		const Token &start = Look();
		auto x = ParseAnd();
		while (Look().tag == Tag::Or)
		{
			Match(Tag::Or);
			auto y = ParseAnd();
			x = std::make_shared<BinaryExpression>(Pos(start),
				ExpressionType::Or, x, y);
		}
		return x;
	}

	ExpPtr Parser::ParseAnd()
	{
		const Token &start = Look();
		auto x = ParseEquality();
		while (Look().tag == Tag::And)
		{
			Match(Tag::And);
			auto y = ParseEquality();
			x = std::make_shared<BinaryExpression>(Pos(start),
				ExpressionType::And, x, y);
		}
		return x;
	}

	ExpPtr Parser::ParseEquality()
	{
		const Token &start = Look();
		auto x = ParseRelation();
		while (Look().tag == Tag::Equal || Look().tag == Tag::NotEqual)
		{
			Token t = Look();
			Advance();
			auto y = ParseRelation();
			if (t.tag == Tag::Equal)
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::Equal, x, y);
			}
			else
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::NotEqual, x, y);
			}
		}
		return x;
	}

	ExpPtr Parser::ParseRelation()
	{
		const Token &start = Look();
		auto x = ParseExpr();
		if (Look().tag == Tag::GreaterThan || Look().tag == Tag::LessThan ||
			Look().tag == Tag::GreaterThanOrEqual ||
			Look().tag == Tag::LessThanOrEqual)
		{
			Token t = Look();
			Advance();
			auto y = ParseExpr();
			if (t.tag == Tag::GreaterThan)
			{
				return std::make_shared<BinaryExpression>(
					Pos(start), ExpressionType::GreaterThan, x, y);
			}
			else if (t.tag == Tag::LessThan)
			{
				return std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::LessThan, x, y);
			}
			else if (t.tag == Tag::GreaterThanOrEqual)
			{
				return std::make_shared<BinaryExpression>(
					Pos(start), ExpressionType::GreaterThanOrEqual, x, y);
			}
			else
			{
				return std::make_shared<BinaryExpression>(
					Pos(start), ExpressionType::LessThanOrEqual, x, y);
			}
		}
		else
		{
			return x;
		}
	}

	ExpPtr Parser::ParseExpr()
	{
		const Token &start = Look();
		auto x = ParseTerm();
		while (Look().tag == Tag::Add || Look().tag == Tag::Subtract)
		{
			Token t = Look();
			Advance();
			auto y = ParseTerm();
			if (t.tag == Tag::Add)
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::Add, x, y);
			}
			else
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::Subtract, x, y);
			}
		}
		return x;
	}

	ExpPtr Parser::ParseTerm()
	{
		const Token &start = Look();
		auto x = ParseUnary();
		while (Look().tag == Tag::Multiply || Look().tag == Tag::Divide)
		{
			Token t = Look();
			Advance();
			auto y = ParseUnary();
			if (t.tag == Tag::Multiply)
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::Multiply, x, y);
			}
			else
			{
				x = std::make_shared<BinaryExpression>(Pos(start),
					ExpressionType::Divide, x, y);
			}
		}
		return x;
	}

	ExpPtr Parser::ParseUnary()
	{
		const Token &start = Look();
		if (Look().tag == Tag::Add)
		{
			Advance();
			auto x = ParseUnary();
			return std::make_shared<UnaryExpression>(Pos(start),
				ExpressionType::UnaryPlus, x);
		}
		else if (Look().tag == Tag::Subtract)
		{
			Advance();
			auto x = ParseUnary();
			return std::make_shared<UnaryExpression>(Pos(start),
				ExpressionType::UnaryMinus, x);
		}
		else if (Look().tag == Tag::Not)
		{
			Advance();
			auto x = ParseUnary();
			return std::make_shared<UnaryExpression>(Pos(start), ExpressionType::Not,
				x);
		}
		else
		{
			return ParsePostfix();
		}
	}

	ExpPtr Parser::ParsePostfix()
	{
		auto x = ParseFactor();
		while (Look().tag == Tag::LeftParenthesis || Look().tag == Tag::LeftBracket ||
			Look().tag == Tag::Dot)
		{
			const Token &start = Look();

			if (Look().tag == Tag::LeftParenthesis)
			{
				auto arguments = ParseArguments();
				x = std::make_shared<InvocationExpression>(Pos(start), x, arguments);
			}
			else if (Look().tag == Tag::LeftBracket)
			{
				// TO DO
				throw NotImplementedException();
			}
			else // if (Look().tag == Tag::Dot)
			{
				Match(Tag::Dot);
				auto name = Match(Tag::Identifier).text;
				x = std::make_shared<MemberAccessExpression>(Pos(start), x, name);
			}
		}
		return x;
	}

	ExpPtr Parser::ParseFactor()
	{
		if (Look().tag == Tag::LeftParenthesis)
		{
			Advance();
			ExpPtr x = ParseOr();
			Match(Tag::RightParenthesis);
			return x;
		}
		else if (Look().tag == Tag::Integer)
		{
			std::u32string v = Look().text;
			const Token &start = Look();
			Advance();
			return std::make_shared<ConstantExpression>(
				Pos(start), std::make_shared<Int32Type>(), v);
		}
		else if (Look().tag == Tag::Float)
		{
			std::u32string v = Look().text;
			const Token &start = Look();
			Advance();
			return std::make_shared<ConstantExpression>(
				Pos(start), std::make_shared<Float64Type>(), v);
		}
		else if (Look().tag == Tag::String)
		{
			std::u32string v = Look().text;
			const Token &start = Look();
			Advance();
			return std::make_shared<ConstantExpression>(
				Pos(start), std::make_shared<StringType>(), v);
		}
		else if (Look().tag == Tag::True)
		{
			const Token &start = Look();
			Advance();
			return std::make_shared<ConstantExpression>(
				Pos(start), std::make_shared<BooleanType>(), U"true");
		}
		else if (Look().tag == Tag::False)
		{
			const Token &start = Look();
			Advance();
			return std::make_shared<ConstantExpression>(
				Pos(start), std::make_shared<BooleanType>(), U"false");
		}
		else if (Look().tag == Tag::Identifier)
		{
			std::u32string name = Look().text;
			const Token &start = Look();
			Advance();
			return std::make_shared<ParameterExpression>(
				Pos(start), name, Type::Unknown());
		}
		else if (Look().tag == Tag::New)
		{
			return ParseNewExpression();
		}
		else
		{
			throw ParserException(document, Look().line, Look().column,
				Format(U"unexpected token type: {}", Enum<Tag>::ToString(Look().tag)));
		}
	}

	ExpPtr Parser::ParseBlock()
	{
		const Token &start = Look();
		Match(Tag::LeftBrace);
		vector<ExpPtr> expressions;
		while (!IsEof() && Look().tag != Tag::RightBrace)
		{
			expressions.push_back(Statement());
		}
		Match(Tag::RightBrace);
		return std::make_shared<BlockExpression>(Pos(start), expressions);
	}

	ExpPtr Parser::IfStatement()
	{
		const Token &start = Look();
		Match(Tag::If);
		ExpPtr condition = ParseOr();
		ExpPtr ifTrue = ParseBlock();
		if (Look().tag == Tag::Else)
		{
			Match(Tag::Else);
			if (Look().tag == Tag::If)
			{
				auto chunk = IfStatement();
				return std::make_shared<ConditionalExpression>(Pos(start), condition,
					ifTrue, chunk);
			}
			else
			{
				auto chunk = ParseBlock();
				return std::make_shared<ConditionalExpression>(Pos(start), condition,
					ifTrue, chunk);
			}
		}
		else
		{
			auto empty =
				std::make_shared<DefaultExpression>(Pos(Look()), Type::Void());
			return std::make_shared<ConditionalExpression>(Pos(start), condition,
				ifTrue, empty);
		}
	}

	ExpPtr Parser::ParseVar()
	{
		const Token &start = Look();
		Match(Tag::Var);
		Token t = Match(Tag::Identifier);
		auto name = t.text;
		if (Look().tag == Tag::Colon)
		{
			Match(Tag::Colon);
			auto type = ParseType();
			if (Look().tag == Tag::Assign)
			{ /* var x: Int = 46 */
				Match(Tag::Assign);
				auto value = ParseOr();
				auto variable =
					std::make_shared<ParameterExpression>(Pos(Look()), name, type);
				return std::make_shared<VarDefExpression>(Pos(start), variable, type,
					value);
			}
			else
			{ /* var x: Int */
				auto value = std::make_shared<DefaultExpression>(Pos(Look()), type);
				auto variable =
					std::make_shared<ParameterExpression>(Pos(Look()), name, type);
				return std::make_shared<VarDefExpression>(Pos(start), variable, type,
					value);
			}
		}
		else
		{ /* var x = 46 */
			Match(Tag::Assign);
			auto value = ParseOr();
			auto variable =
				std::make_shared<ParameterExpression>(Pos(Look()), name, Type::Unknown());
			return std::make_shared<VarDefExpression>(Pos(start), variable, Type::Unknown(),
				value);
		}
	}

	FieldDef Parser::ParseFieldDefinition(AccessModifier modifier,
		Table<std::u32string, AnnotationInfo> annotations, bool isStatic)
	{
		const Token &start = Look();
		Match(Tag::Var);
		auto name = Match(Tag::Identifier).text;
		Match(Tag::Colon);
		auto type = ParseType();
		if (Look().tag == Tag::Assign)
		{
			Match(Tag::Assign);
			auto value = ParseOr();
			return FieldDef(Pos(start),
				modifier, isStatic, annotations, name, type, value);
		}
		else
		{
			auto value = std::make_shared<DefaultExpression>(Pos(Look()), type);
			return FieldDef(Pos(start),
				modifier, isStatic, annotations, name, type, value);
		}
	}

	MethodDef Parser::ParseMethodDefinition(AccessModifier modifier,
		Table<std::u32string, AnnotationInfo> annotations, bool isStatic, TypePtr selfType)
	{
		const Token &start = Look();
		Match(Tag::Def);
		auto name = Match(Tag::Identifier).text;

		Match(Tag::LeftParenthesis);
		std::vector<std::shared_ptr<ParameterExpression>> parameters;
		if (Look().tag != Tag::RightParenthesis)
		{
			parameters.push_back(ParseParameter());
			while (!IsEof() && Look().tag != Tag::RightParenthesis)
			{
				Match(Tag::Comma);
				parameters.push_back(ParseParameter());
			}
		}
		Match(Tag::RightParenthesis);
		Match(Tag::Colon);
		auto returnType = ParseType();
		if (Look().tag == Tag::LeftBrace)
		{
			auto body = ParseBlock();
			return MethodDef(Pos(start), modifier, isStatic, selfType,
				annotations, name, parameters, returnType, body);
		}
		else
		{
			auto empty =
				std::make_shared<DefaultExpression>(Pos(Look()), returnType);
			return MethodDef(Pos(start), modifier, isStatic, selfType,
				annotations, name, parameters, returnType, empty);
		}
	}

	std::shared_ptr<ParameterExpression> Parser::ParseParameter()
	{
		const Token &start = Look();
		auto name = Match(Tag::Identifier).text;
		Match(Tag::Colon);
		auto type = ParseType();
		auto parameter =
			std::make_shared<ParameterExpression>(Pos(start), name, type);
		return parameter;
	}

	std::shared_ptr<Type> Parser::ParseType()
	{
		auto name = Match(Tag::Identifier).text;
		static std::unordered_map<std::u32string, std::shared_ptr<Type>> basicTypes =
		{ {U"Int", Type::Int32()},
		 {U"Long",Type::Int64()},
		 {U"Bool", Type::Boolean()},
		 {U"Float", Type::Float32()},
		 {U"Double", Type::Float64()},
		 {U"Char", Type::Char()},
		 {U"String", Type::String()},
		 {U"Any", Type::Any()},
		 {U"Void", Type::Void()} };
		if (basicTypes.find(name) != basicTypes.end())
		{
			return basicTypes[name];
		}
		else if (name == U"Array")
		{
			return ParseArrayType();
		}
		else
		{
			return std::make_shared<UnresolvedType>(route, name);
		}
	}

	std::shared_ptr<ArrayType> Parser::ParseArrayType()
	{
		auto args = ParseTypeArguments();
		if (args.size() == 1)
		{
			return std::make_shared<ArrayType>(args.at(0));
		}
		else
		{
			throw ParserException(document, Look().line, Look().column,
				Format(U"wrong number of type arguments for the 'Array' type. should be 1 instead of {}.", static_cast<int>(args.size())));
		}
	}

	ExpPtr Parser::ParseReturn()
	{
		const Token &start = Look();
		Match(Tag::Return);
		auto value = ParseOr();
		return std::make_shared<ReturnExpression>(Pos(start), value);
	}

	std::vector<TypePtr> Parser::ParseTypeArguments()
	{
		Match(Tag::LeftBracket);
		std::vector<TypePtr> types;
		types.push_back(ParseType());
		while (!IsEof() && Look().tag != Tag::RightBracket)
		{
			Match(Tag::Comma);
			types.push_back(ParseType());
		}
		Match(Tag::RightBracket);
		return types;
	}

	ExpPtr Parser::ParseWhile()
	{
		const Token &start = Look();
		Match(Tag::While);
		auto condition = ParseOr();
		auto body = ParseBlock();
		return std::make_shared<WhileExpression>(Pos(start), condition, body);
	}

	std::shared_ptr<ClassInfo> Parser::ParseDefClass()
	{
		const Token &start = Look();
		Match(Tag::Class);
		auto name = Match(Tag::Identifier).text;
		auto info = std::make_shared<ClassInfo>(Pos(start), route, name);

		if (Look().tag == Tag::UpperBound)
		{
			Match(Tag::UpperBound);
			info->superTypes.push_back(ParseType());
			while (!IsEof() && Look().tag != Tag::LeftBrace)
			{
				Match(Tag::Comma);
				info->superTypes.push_back(ParseType());
			}
		}
		Match(Tag::LeftBrace);
		while (!IsEof() && Look().tag != Tag::RightBrace)
		{
			auto annotations = ParseAnnotationList();
			auto access = ParseAccess();

			if (Look().tag == Tag::Var)
			{
				// ParseVar field: Type
				auto field = ParseFieldDefinition(access, annotations, false);
				info->fieldDefs.Add(field.name, field);
			}
			else if (Look().tag == Tag::Def)
			{
				// def method(args..) { }
				auto selfType = std::make_shared<ClassType>(route, name);
				auto method = ParseMethodDefinition(access, annotations, false, selfType);
				info->methodDefs.Add(method.name, method);
			}
			else
			{
				throw ParserException(document, Look().line, Look().column,
					Format(U"unexpected token type '{}', expecting 'var' or 'def'", Enum<Tag>::ToString(Look().tag)));
			}
		}
		Match(Tag::RightBrace);
		return info;
	}

	std::shared_ptr<ModuleInfo> Parser::ParseDefModule()
	{
		const Token &start = Look();
		Match(Tag::Module);
		auto name = Match(Tag::Identifier).text;
		auto info = std::make_shared<ModuleInfo>(Pos(start), route, name);
		Match(Tag::LeftBrace);
		while (!IsEof() && Look().tag != Tag::RightBrace)
		{
			auto annotations = ParseAnnotationList();
			auto access = ParseAccess();

			if (Look().tag == Tag::Var)
			{
				// ParseVar field: Type
				auto field = ParseFieldDefinition(access, annotations, true);
				info->fields.Add(field.name, field);
			}
			else if (Look().tag == Tag::Def)
			{
				// def method(args..) { }
				auto selfType = std::make_shared<ModuleType>(route, name);
				auto method = ParseMethodDefinition(access, annotations, true, selfType);
				info->methods.Add(method.name, method);
			}
			else
			{
				throw ParserException(document, Look().line, Look().column,
					Format(U"unexpected token type '{}', expecting 'var' or 'def'", Enum<Tag>::ToString(Look().tag)));
			}
		}
		Match(Tag::RightBrace);
		return info;
	}

	std::shared_ptr<InterfaceInfo> Parser::ParseDefInterface()
	{
		const Token &start = Look();
		Match(Tag::Interface);
		auto name = Match(Tag::Identifier).text;
		auto info = std::make_shared<InterfaceInfo>(Pos(start), route, name);
		if (Look().tag == Tag::UpperBound)
		{
			Match(Tag::UpperBound);
			info->superInterfaces.push_back(ParseType());
			while (!IsEof() && Look().tag != Tag::LeftBrace)
			{
				Match(Tag::Comma);
				info->superInterfaces.push_back(ParseType());
			}
		}
		Match(Tag::LeftBrace);
		while (!IsEof() && Look().tag != Tag::RightBrace)
		{
			Table<std::u32string, AnnotationInfo> annotations = ParseAnnotationList();
			if (Look().tag == Tag::Def)
			{
				// def method(args..) { }
				auto selfType = std::make_shared<InterfaceType>(route, name);
				auto method = ParseMethodDefinition(AccessModifier::Public, annotations, false, selfType);
				info->methodDefs.Add(method.name, method); // only definition, no implementation
			}
			else
			{
				throw ParserException(document, Look().line, Look().column,
					Format(U"unexpected token type '{}', expecting 'def'", Enum<Tag>::ToString(Look().tag)));
			}
		}
		Match(Tag::RightBrace);
		return info;
	}

	AccessModifier Parser::ParseAccess()
	{
		if (Look().tag == Tag::Private)
		{
			Match(Tag::Private);
			return AccessModifier::Private;
		}
		else if (Look().tag == Tag::Protected)
		{
			Match(Tag::Protected);
			return AccessModifier::Protected;
		}
		else
		{
			return AccessModifier::Public;
		}
	}

	AnnotationInfo Parser::ParseAnnotation()
	{
		const Token &start = Look();
		Match(Tag::At);
		auto name = Match(Tag::Identifier).text;
		auto arguments = ParseArguments();
		return AnnotationInfo(Pos(start), name, arguments);
	}

	Table<std::u32string, AnnotationInfo> Parser::ParseAnnotationList()
	{
		Table<std::u32string, AnnotationInfo> annotations;
		while (Look().tag == Tag::At)
		{
			auto annotation = ParseAnnotation();
			annotations.Add(annotation.name, annotation);
		}
		return annotations;
	}

	std::vector<Argument> Parser::ParseArguments()
	{
		vector<Argument> arguments;
		Match(Tag::LeftParenthesis);
		if (Look().tag == Tag::RightParenthesis)
		{
			Match(Tag::RightParenthesis);
		}
		else
		{
			arguments.push_back(ParseArgument());
			while (!IsEof() && Look().tag != Tag::RightParenthesis)
			{
				Match(Tag::Comma);
				arguments.push_back(ParseArgument());
			}
			Match(Tag::RightParenthesis);
		}
		return arguments;
	}

	std::shared_ptr<NewExpression> Parser::ParseNewExpression()
	{
		const auto &start = Look();
		Match(Tag::New);
		auto name = Match(Tag::Identifier).text;
		if (Look().tag != Tag::LeftBrace)
		{
			return std::make_shared<NewExpression>(Pos(start),
				std::make_shared<ClassType>(route, name),
				std::vector<Argument>{});
		}
		else
		{
			Match(Tag::LeftBrace);
			std::vector<Argument> arguments;
			arguments.push_back(ParseArgument());
			while (!IsEof() && Look().tag != Tag::RightBrace)
			{
				Match(Tag::Comma);
				arguments.push_back(ParseArgument());
			}
			Match(Tag::RightBrace);
			return std::make_shared<NewExpression>(Pos(start),
				std::make_shared<UnresolvedType>(route, name),
				arguments);
		}
	}

	Argument Parser::ParseArgument()
	{
		if (Look().tag == Tag::Identifier)
		{
			auto name = Match(Tag::Identifier).text;
			if (Look().tag == Tag::Assign)
			{
				Match(Tag::Assign);
				ExpPtr value = ParseOr();
				return Argument(name, value);
			}
			else
			{
				Back();
				ExpPtr value = ParseOr();
				return Argument(name, value);
			}
		}
		else
		{
			ExpPtr value = ParseOr();
			return Argument(value);
		}
	}

	std::vector<ImportStatement> Parser::ParseImportedPackages()
	{
		std::vector<ImportStatement> importedPackages;
		while (Look().tag == Tag::Import)
		{
			Match(Tag::Import);
			importedPackages.emplace_back(Pos(Look()), ParsePackageRoute());
		}
		return importedPackages;
	}

	Table<std::u32string, TypeAlias> Parser::ParseTypeAliases()
	{
		Table<std::u32string, TypeAlias> typeAliases;
		while (Look().tag == Tag::Rename)
		{
			auto token = Match(Tag::Rename);
			auto route = ParsePackageRoute();
			Match(Tag::To);
			auto alias = Match(Tag::Identifier).text;
			auto originalName = route.back();
			route.pop_back();
			typeAliases.Add(alias, TypeAlias(Pos(token), route, originalName, alias));
		}
		return typeAliases;
	}
} // namespace cygni
