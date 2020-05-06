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
				std::u32string message = U"expecting '" + Enum<Tag>::ToString(tag) +
					U"', got '" + Enum<Tag>::ToString(Look().tag) +
					U"'";
				throw ParserException(Look().line, Look().column, message);
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
			std::vector<AnnotationInfo> annotations, bool isStatic);

		MethodDef ParseMethodDefinition(AccessModifier modifier, 
			std::vector<AnnotationInfo> annotations, bool isStatic);

		std::shared_ptr<ParameterExpression> ParseParameter();

		std::shared_ptr<Type> ParseType();

		ExpPtr ParseReturn();

		std::vector<std::shared_ptr<Type>> ParseTypeArguments();

		ExpPtr ParseWhile();

		std::shared_ptr<ClassInfo> ParseDefClass();

		std::shared_ptr<ModuleInfo> ParseDefModule();

		AccessModifier ParseAccess();

		AnnotationInfo ParseAnnotation();

		std::vector<AnnotationInfo> ParseAnnotationList();

		std::vector<Argument> ParseArguments();

		std::shared_ptr<NewExpression> ParseNewExpression();

		Argument ParseArgument();

		std::vector<PackageRoute> ParseImportedPackages();
		std::unordered_map<std::u32string, TypeAlias> ParseTypeAliases();
	};
} // namespace cygni
#endif // CYGNI_PARSER_HPP
