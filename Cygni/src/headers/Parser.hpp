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

	Parser();

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
};
} // namespace cygni
#endif // CYGNI_PARSER_HPP
