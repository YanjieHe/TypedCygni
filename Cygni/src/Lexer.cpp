#include "Lexer.hpp"
#include <iostream>
#include <unordered_set>

namespace cygni
{

	Lexer::Lexer(std::shared_ptr<FileLocation> document, const std::u32string &code)
		: document{ document }, code{ code }, line{ 1 }, column{ 1 }, offset{ 0 } {}

	std::vector<Token> Lexer::ReadAll()
	{
		std::vector<Token> tokens;
		static std::u32string opChars = U"+-*/%><=!()[]{}:,.;@";
		static std::unordered_set<char32_t> opCharSet(opChars.begin(), opChars.end());
		SkipWhitespaces();
		while (!IsEof())
		{
			if (Peek() == U'/')
			{
				SkipComment(tokens);
			}
			else if (IsDigit(Peek()))
			{
				tokens.push_back(ReadInt());
			}
			else if (Peek() == SINGLE_QUOTE)
			{
				tokens.push_back(ReadCharacterLiteral());
			}
			else if (Peek() == DOUBLE_QUOTE)
			{
				tokens.push_back(ReadString());
			}
			else if (opCharSet.find(Peek()) != opCharSet.end())
			{
				tokens.push_back(ReadOperator());
			}
			else if (IsLetter(Peek()) || Peek() == U'_')
			{
				tokens.push_back(ReadIdentifier());
			}
			else if (Peek() == U'@')
			{
				tokens.push_back(ReadAnnotation());
			}
			else
			{
				throw LexicalException(document, line, column, U"unsupported token");
			}
			SkipWhitespaces();
		}
		tokens.emplace_back(line, column, Tag::Eof, U"<EOF>");
		return tokens;
	}

	Token Lexer::ReadInt()
	{
		Reset();
		ReadDecimalDigits();
		if (IsEof())
		{
			return Token(line, column, Tag::Integer, builder);
		}
		else
		{
			if (Peek() == U'.')
			{
				Consume();
				return ReadFloat();
			}
			else
			{
				return Token(line, column, Tag::Integer, builder);
			}
		}
	}

	Token Lexer::ReadFloat()
	{
		ReadDecimalDigits();
		if (IsEof())
		{
			return Token(line, column, Tag::Float, builder);
		}
		else
		{
			if (Peek() == U'E' || Peek() == U'e')
			{
				return ReadExponent();
			}
			else
			{
				return Token(line, column, Tag::Float, builder);
			}
		}
	}

	Token Lexer::ReadExponent()
	{
		Match(U'E', U'e');

		if (Peek() == U'+' || Peek() == U'-')
		{
			Consume();
		}
		if (IsEof() || !IsDigit(Peek()))
		{
			throw LexicalException(document, line, column, U"float literal");
		}
		else
		{
			ReadDecimalDigits();
			return Token(line, column, Tag::Float, builder);
		}
	}

	void Lexer::ReadDecimalDigits()
	{
		while (!IsEof() && IsDigit(Peek()))
		{
			Consume();
		}
	}

	Token Lexer::ReadCharacterLiteral()
	{
		Reset();
		MatchAndSkip(SINGLE_QUOTE);
		ReadCharacter();
		MatchAndSkip(SINGLE_QUOTE);
		return Token(line, column, Tag::Character, builder);
	}

	void Lexer::ReadCharacter()
	{
		if (IsEof())
		{
			throw LexicalException(document, line, column, U"character literal");
		}
		else
		{
			if (Peek() == BACKSLASH)
			{
				MatchAndSkip(BACKSLASH);
				ReadSimpleEscapeSequence();
			}
			else
			{
				Consume();
			}
		}
	}

	void Lexer::ReadSimpleEscapeSequence()
	{
		if (Peek() == U'x')
		{
			ReadHexadecimalEscapeSequence();
		}
		else if (Peek() == U'u' || Peek() == U'U')
		{
			ReadUnicodeEscapeSequence();
		}
		else
		{
			builder.push_back(UnescapedChar(Peek()));
			Forward();
		}
	}

	void Lexer::ReadHexadecimalEscapeSequence()
	{
		MatchAndSkip(U'x');
		std::u32string text;
		if (IsHexDigit())
		{
			text.push_back(Peek());
			Forward();
		}
		else
		{
			throw LexicalException(document, line, column, U"expecting an hex digit");
		}

		for (int i = 0; i < 3 && IsHexDigit(); i++)
		{
			text.push_back(Peek());
			Forward();
		}
		try
		{
			int val = HexToInt(text);
			builder.push_back(static_cast<char32_t>(val));
		}
		catch (ArgumentException &)
		{
			throw LexicalException(document, line, column, U"wrong format for hex digit");
		}
	}

	void Lexer::ReadUnicodeEscapeSequence()
	{
		std::u32string text;
		if (Peek() == U'u')
		{
			Match(U'u');
			for (int i = 0; i < 4; i++)
			{
				if ((!IsEof()) && IsHexDigit())
				{
					text.push_back(Peek());
					Forward();
				}
				else
				{
					throw LexicalException(document, line, column, U"expecting an hex digit");
				}
			}
		}
		else if (Peek() == U'U')
		{
			Match(U'U');
			for (int i = 0; i < 8; i++)
			{
				if ((!IsEof()) && IsHexDigit())
				{
					text.push_back(Peek());
					Forward();
				}
				else
				{
					throw LexicalException(document, line, column, U"expecting an hex digit");
				}
			}
		}
		else
		{
			throw LexicalException(document, line, column, U"expecting 'u' or 'U'");
		}
		try
		{
			int val = HexToInt(text);
			builder.push_back(static_cast<char32_t>(val));
		}
		catch (ArgumentException &)
		{
			throw LexicalException(document, line, column, U"wrong format for hex digit");
		}
	}

	Token Lexer::ReadAnnotation()
	{
		Match(U'@');
		return Token(line, column, Tag::At, U"@");
	}

	bool Lexer::IsHexDigit()
	{
		static std::u32string digits = U"0123456789abcdefABCDEF";
		static std::unordered_set<char32_t> digitSet(digits.begin(), digits.end());
		if (!IsEof())
		{
			return digitSet.find(Peek()) != digitSet.end();
		}
		else
		{
			return false;
		}
	}

	Token Lexer::ReadString()
	{
		Reset();
		Forward();
		while (!IsEof() && Peek() != DOUBLE_QUOTE)
		{
			if (Peek() == U'\\')
			{
				Forward();
				if (IsEof())
				{
					throw LexicalException(document, line, column, U"string literal");
				}
				else
				{
					builder.push_back(UnescapedChar(Peek()));
					Forward();
				}
			}
			else
			{
				Consume();
			}
		}
		if (IsEof())
		{
			throw LexicalException(document, line, column, U"string literal");
		}
		else
		{
			Forward();
			return Token(line, column, Tag::String, builder);
		}
	}

	char32_t Lexer::UnescapedChar(char32_t c)
	{
		switch (c)
		{
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
			throw LexicalException(document, line, column, U"unsupported escaped character");
		}
	}

	Token Lexer::ReadIdentifier()
	{
		Reset();
		Consume();
		while (!IsEof() && IsIdentifierChar(Peek()))
		{
			Consume();
		}
		return Token(line, column, Tag::Identifier, builder);
	}

	Token Lexer::ReadOperator()
	{
		char32_t c1 = Peek();
		std::u32string s1;
		s1.push_back(c1);
		Forward();
		if (IsEof())
		{
			if (Token::operators.find(s1) != Token::operators.end())
			{
				return Token(line, column, Token::operators[s1], s1);
			}
			else
			{
				throw LexicalException(document, line, column, U"operator literal");
			}
		}
		else
		{
			char32_t c2 = Peek();
			std::u32string s12;
			s12.push_back(c1);
			s12.push_back(c2);
			if (Token::operators.find(s12) != Token::operators.end())
			{
				Forward();
				return Token(line, column, Token::operators[s12], s12);
			}
			else if (Token::operators.find(s1) != Token::operators.end())
			{
				return Token(line, column, Token::operators[s1], s1);
			}
			else
			{
				throw LexicalException(document, line, column, U"operator literal");
			}
		}
	}

	void Lexer::SkipWhitespaces()
	{
		while (!IsEof() && IsWhiteSpace(Peek()))
		{
			Forward();
		}
	}


	void Lexer::SkipComment(std::vector<Token>& tokens)
	{
		MatchAndSkip(U'/');
		if (Peek() == U'/')
		{
			SkipSingleLineComment();
		}
		else
		{
			tokens.push_back(Token(line, column, Tag::Divide, U"/"));
			//throw LexicalException(document, line, column, U"expecting '/' for comment");
		}
	}

	void Lexer::SkipSingleLineComment()
	{
		MatchAndSkip(U'/');
		while ((!IsEof()) && Peek() != END_LINE)
		{
			Forward();
		}
	}

} // namespace cygni
