#ifndef CYGNI_PARSER_HPP
#define CYGNI_PARSER_HPP
#include "Exception.hpp"
#include "Expression.hpp"
#include "Token.hpp"

namespace cygni
{
	class Parser
	{
	public:
		std::vector<Token> tokens;
		std::shared_ptr<SourceDocument> document;
		int offset;
		PackageRoute route;

		Parser(std::vector<Token> tokens, std::shared_ptr<SourceDocument> document);

		inline bool IsEof() const { return Look().tag == Tag::Eof; }

		inline const Token &Look() const { return tokens[offset]; }

		inline void Advance() { offset++; }

		inline void Back() { offset--; }

		const Token &Match(Tag tag)
		{
			if (tag == Look().tag)
			{
				const Token &t = Look();
				Advance();
				return t;
			}
			else
			{
				throw ParserException(Look().line, Look().column,
					Format(U"expecting '{}', got '{}'",
						Enum<Tag>::ToString(tag),
						Enum<Tag>::ToString(Look().tag)));
			}
		}

		inline SourceLocation GetLoc(const Token &token) const
		{
			return SourceLocation{ document, token.line, token.column, Look().line,
								  Look().column };
		}

		Program ParseProgram();

		PackageRouteStatement ParsePackageRouteStatement();

		PackageRoute ParsePackageRoute();

		ExpPtr Statement();

		ExpPtr ParseAssign();

		ExpPtr ParseOr();

		ExpPtr ParseAnd();

		ExpPtr ParseEquality();

		ExpPtr ParseRelation();

		ExpPtr ParseExpr();

		ExpPtr ParseTerm();

		ExpPtr ParseUnary();

		ExpPtr ParsePostfix();

		ExpPtr ParseFactor();

		ExpPtr ParseBlock();

		ExpPtr IfStatement();

		ExpPtr ParseVar();

		FieldDef ParseFieldDefinition(AccessModifier modifier,
			Table<std::u32string, AnnotationInfo> annotations, bool isStatic);

		MethodDef ParseMethodDefinition(AccessModifier modifier,
			Table<std::u32string, AnnotationInfo> annotations, bool isStatic, TypePtr selfType);

		std::shared_ptr<ParameterExpression> ParseParameter();

		std::shared_ptr<Type> ParseType();

		std::shared_ptr<ArrayType> ParseArrayType();

		ExpPtr ParseReturn();

		std::vector<TypePtr> ParseTypeArguments();

		ExpPtr ParseWhile();

		std::shared_ptr<ClassInfo> ParseDefClass();

		std::shared_ptr<ModuleInfo> ParseDefModule();

		std::shared_ptr<InterfaceInfo> ParseDefInterface();

		AccessModifier ParseAccess();

		AnnotationInfo ParseAnnotation();

		Table<std::u32string, AnnotationInfo> ParseAnnotationList();

		std::vector<Argument> ParseArguments();

		std::shared_ptr<NewExpression> ParseNewExpression();

		Argument ParseArgument();

		std::vector<PackageRoute> ParseImportedPackages();

		Table<std::u32string, TypeAlias> ParseTypeAliases();

	};
} // namespace cygni
#endif // CYGNI_PARSER_HPP
