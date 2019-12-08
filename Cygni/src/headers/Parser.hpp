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

    Ptr<Var> ParseVarDeclaration();

    Ptr<Def> ParseDef();

    Parameter ParseParameter();

    Ptr<TypeExpression> ParseType();

    ExpPtr ParseReturn();

    Vector<Ptr<TypeExpression>> ParseTypeArguments();

    Ptr<While> ParseWhile();

    Ptr<DefClass> ParseDefClass();

    Ptr<DefModule> ParseDefModule();

    Access ParseAccess();
};
} // namespace cygni
#endif // CYGNI_PARSER_HPP
