#ifndef CYGNI_PARSER_HPP
#define CYGNI_PARSER_HPP
#include "Exception.hpp"
#include "Expression.hpp"
#include "Token.hpp"
namespace cygni {
class Parser {
public:
	std::vector<Token> tokens;
	std::shared_ptr<SourceDocument> document;
	int offset;

	Parser(std::vector<Token> tokens, std::shared_ptr<SourceDocument> document);

	inline bool IsEof() const {
		return Look().tag == Tag::Eof;
	}

	inline const Token& Look() const {
		return tokens[offset];
	}

	inline void Advance() {
		offset++;
	}

	Token Match(Tag tag) {
		if (tag == Look().tag) {
			Token t = Look();
			Advance();
			return t;
		} else {
			std::u32string message = U"expecting '" + Enum<Tag>::ToString(tag) +
									 U"', got '" +
									 Enum<Tag>::ToString(Look().tag) + U"'";
			throw ParserException(Look().line, Look().column, message);
		}
	}

	inline SourceLocation GetLoc(const Token& token) const {
		return SourceLocation{document, token.line, token.column, Look().line,
							  Look().column};
	}

	Program ParseProgram();

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

	std::shared_ptr<VariableDefinitionExpression> ParseVarDeclaration();

	FieldDef ParseFieldDefinition(AccessModifier modifier, bool isStatic);

	MethodDef ParseMethodDefinition(AccessModifier modifier, bool isStatic);

	std::shared_ptr<ParameterExpression> ParseParameter();

	std::shared_ptr<Type> ParseType();

	ExpPtr ParseReturn();

	std::vector<std::shared_ptr<Type>> ParseTypeArguments();

	ExpPtr ParseWhile();

	std::shared_ptr<ClassInfo> ParseDefClass();

	std::shared_ptr<ClassInfo> ParseDefModule();

	AccessModifier ParseAccess();
};
} // namespace cygni
#endif // CYGNI_PARSER_HPP
