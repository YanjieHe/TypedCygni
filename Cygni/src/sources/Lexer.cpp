#include "Lexer.hpp"
#include <iostream>
#include <unordered_set>

namespace cygni {

Lexer::Lexer(const std::u32string& code)
	: code{code}, line{1}, column{1}, offset{0} {
}

std::vector<Token> Lexer::ReadAll() {
	std::vector<Token> tokens;
	static std::u32string opChars = U"+-*/%><=!()[]{}:,.;@";
	static std::unordered_set<char32_t> opCharSet(opChars.begin(),
												  opChars.end());
	SkipWhitespaces();
	while (!IsEof()) {
		if (Peek() == U'/') {
			SkipComment();
		} else if (IsDigit(Peek())) {
			tokens.push_back(ReadInt());
		} else if (Peek() == SINGLE_QUOTE) {
			tokens.push_back(ReadCharacterLiteral());
		} else if (Peek() == DOUBLE_QUOTE) {
			tokens.push_back(ReadString());
		} else if (opCharSet.find(Peek()) != opCharSet.end()) {
			tokens.push_back(ReadOperator());
		} else if (IsLetter(Peek()) || Peek() == U'_') {
			tokens.push_back(ReadIdentifier());
		} else {
			throw LexicalException(line, column, U"unsupported token");
		}
		SkipWhitespaces();
	}
	tokens.emplace_back(line, column, Tag::Eof, U"<EOF>");
	return tokens;
}

Token Lexer::ReadInt() {
	Reset();
	ReadDecimalDigits();
	if (IsEof()) {
		return Token(line, column, Tag::Integer, builder);
	} else {
		if (Peek() == U'.') {
			Consume();
			return ReadFloat();
		} else {
			return Token(line, column, Tag::Integer, builder);
		}
	}
}

Token Lexer::ReadFloat() {
	ReadDecimalDigits();
	if (IsEof()) {
		return Token(line, column, Tag::Float, builder);
	} else {
		if (Peek() == U'E' || Peek() == U'e') {
			return ReadExponent();
		} else {
			return Token(line, column, Tag::Float, builder);
		}
	}
}

Token Lexer::ReadExponent() {
	Match(U'E', U'e');

	if (Peek() == U'+' || Peek() == U'-') {
		Consume();
	}
	if (IsEof() || !IsDigit(Peek())) {
		throw LexicalException(line, column, U"float literal");
	} else {
		ReadDecimalDigits();
		return Token(line, column, Tag::Float, builder);
	}
}

void Lexer::ReadDecimalDigits() {
	while (!IsEof() && IsDigit(Peek())) {
		Consume();
	}
}

Token Lexer::ReadCharacterLiteral() {
	Reset();
	MatchAndSkip(SINGLE_QUOTE);
	ReadCharacter();
	MatchAndSkip(SINGLE_QUOTE);
	return Token(line, column, Tag::Character, builder);
}

void Lexer::ReadCharacter() {
	if (IsEof()) {
		throw LexicalException(line, column, U"character literal");
	} else {
		if (Peek() == BACKSLASH) {
			MatchAndSkip(BACKSLASH);
			ReadSimpleEscapeSequence();
		} else {
			Consume();
		}
	}
}

void Lexer::ReadSimpleEscapeSequence() {
	if (Peek() == U'x') {
		ReadHexadecimalEscapeSequence();
	} else if (Peek() == U'u' || Peek() == U'U') {
		ReadUnicodeEscapeSequence();
	} else {
		builder.push_back(UnescapedChar(Peek()));
		Forward();
	}
}

void Lexer::ReadHexadecimalEscapeSequence() {
	MatchAndSkip(U'x');
	std::u32string text;
	if (IsHexDigit()) {
		text.push_back(Peek());
		Forward();
	} else {
		throw LexicalException(line, column, U"expecting an hex digit");
	}

	for (int i = 0; i < 3 && IsHexDigit(); i++) {
		text.push_back(Peek());
		Forward();
	}
	try {
		int val = HexToInt(text);
		builder.push_back(static_cast<char32_t>(val));
	} catch (ArgumentException& ex) {
		throw LexicalException(line, column, U"wrong format for hex digit");
	}
}

void Lexer::ReadUnicodeEscapeSequence() {
	std::u32string text;
	if (Peek() == U'u') {
		Match(U'u');
		for (int i = 0; i < 4; i++) {
			if ((!IsEof()) && IsHexDigit()) {
				text.push_back(Peek());
				Forward();
			} else {
				throw LexicalException(line, column, U"expecting an hex digit");
			}
		}
	} else if (Peek() == U'U') {
		Match(U'U');
		for (int i = 0; i < 8; i++) {
			if ((!IsEof()) && IsHexDigit()) {
				text.push_back(Peek());
				Forward();
			} else {
				throw LexicalException(line, column, U"expecting an hex digit");
			}
		}
	} else {
		throw LexicalException(line, column, U"expecting 'u' or 'U'");
	}
	try {
		int val = HexToInt(text);
		builder.push_back(static_cast<char32_t>(val));
	} catch (ArgumentException& ex) {
		throw LexicalException(line, column, U"wrong format for hex digit");
	}
}

bool Lexer::IsHexDigit() {
	static std::u32string digits = U"0123456789abcdefABCDEF";
	static std::unordered_set<char32_t> digitSet(digits.begin(), digits.end());
	if (!IsEof()) {
		return digitSet.find(Peek()) != digitSet.end();
	} else {
		return false;
	}
}

Token Lexer::ReadString() {
	Reset();
	Forward();
	while (!IsEof() && Peek() != DOUBLE_QUOTE) {
		if (Peek() == U'\\') {
			Forward();
			if (IsEof()) {
				throw LexicalException(line, column, U"string literal");
			} else {
				builder.push_back(UnescapedChar(Peek()));
				Forward();
			}
		} else {
			Consume();
		}
	}
	if (IsEof()) {
		throw LexicalException(line, column, U"string literal");
	} else {
		Forward();
		return Token(line, column, Tag::String, builder);
	}
}

char32_t Lexer::UnescapedChar(char32_t c) {
	switch (c) {
	case U'b':
		return U'\b';
	case U'n':
		return U'\n';
	case U't':
		return U'\t';
	case U'r':
		return U'\r';
	case U'f':
		return U'\f';
	case U'\"':
		return U'\"';
	case U'\'':
		return U'\'';
	case U'\\':
		return U'\\';
	default:
		throw LexicalException(line, column, U"unsupported escaped character");
	}
}

Token Lexer::ReadIdentifier() {
	Reset();
	Consume();
	while (!IsEof() && IsIdentifierChar(Peek())) {
		Consume();
	}
	return Token(line, column, Tag::Identifier, builder);
}

Token Lexer::ReadOperator() {
	static std::unordered_map<std::u32string, Tag> operators = {
		{U"+", Tag::Add},
		{U"-", Tag::Subtract},
		{U"*", Tag::Multiply},
		{U"/", Tag::Divide},
		{U"%", Tag::Modulo},
		{U">", Tag::GreaterThan},
		{U"<", Tag::LessThan},
		{U">=", Tag::GreaterThanOrEqual},
		{U"<=", Tag::LessThanOrEqual},
		{U"==", Tag::Equal},
		{U"!=", Tag::NotEqual},
		{U"(", Tag::LeftParenthesis},
		{U")", Tag::RightParenthesis},
		{U"[", Tag::LeftBracket},
		{U"]", Tag::RightBracket},
		{U"{", Tag::LeftBrace},
		{U"}", Tag::RightBrace},
		{U":", Tag::Colon},
		{U",", Tag::Comma},
		{U".", Tag::Dot},
		{U";", Tag::Semicolon},
		{U"=", Tag::Assign},
		{U"->", Tag::RightArrow},
		{U"@", Tag::At},
		{U"<:", Tag::UpperBound},
		{U":>", Tag::LowerBound}};

	char32_t c1 = Peek();
	std::u32string s1;
	s1.push_back(c1);
	Forward();
	if (IsEof()) {
		if (operators.find(s1) != operators.end()) {
			return Token(line, column, operators[s1], s1);
		} else {
			throw LexicalException(line, column, U"operator literal");
		}
	} else {
		char32_t c2 = Peek();
		std::u32string s12;
		s12.push_back(c1);
		s12.push_back(c2);
		if (operators.find(s12) != operators.end()) {
			Forward();
			return Token(line, column, operators[s12], s12);
		} else if (operators.find(s1) != operators.end()) {
			return Token(line, column, operators[s1], s1);
		} else {
			throw LexicalException(line, column, U"operator literal");
		}
	}
}

void Lexer::SkipWhitespaces() {
	while (!IsEof() && IsWhiteSpace(Peek())) {
		Forward();
	}
}

void Lexer::SkipComment() {
	MatchAndSkip(U'/');
	if (Peek() == U'/') {
		SkipSingleLineComment();
	} else {
		throw LexicalException(line, column, U"expecting '/' for comment");
	}
}

void Lexer::SkipSingleLineComment() {
	MatchAndSkip(U'/');
	while ((!IsEof()) && Peek() != END_LINE) {
		Forward();
	}
}

} // namespace cygni
