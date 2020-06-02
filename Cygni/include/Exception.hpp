#ifndef CYGNI_EXCEPTION_HPP
#define CYGNI_EXCEPTION_HPP
#include "SourceLocation.hpp"
#include "Utility.hpp"
#include <string>

namespace cygni
{
	class LexicalException
	{
	public:
		int line;
		int column;
		std::u32string message;

		LexicalException(int line, int column, const std::u32string &message)
			: line{ line }, column{ column }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualLine = std::to_string(line + 1);
			auto actualColumn = std::to_string(column + 1);
			return Format(U"Syntax Error: ({}, {}) {} ", actualLine, actualColumn, message);
		}
	};

	class ArgumentException
	{
	public:
		std::u32string message;
		explicit ArgumentException(const std::u32string &message)
			: message{ message }
		{
		}
	};

	class ParserException
	{
	public:
		int line;
		int column;
		std::u32string message;

		ParserException(int line, int column, const std::u32string &message)
			: line{ line }, column{ column }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualLine = std::to_string(line + 1);
			auto actualColumn = std::to_string(column + 1);
			return Format(U"Syntax Error: ({}, {}) {} ", actualLine, actualColumn, message);
		}
	};

	class NotImplementedException
	{
	public:
		std::u32string message;

		NotImplementedException() : message{ U"Not Implemented Exception" } {}

		explicit NotImplementedException(const std::u32string &message)
			: message{ message }
		{
		}
	};

	class TypeException
	{
	public:
		SourceLocation location;
		std::u32string message;

		TypeException() = default;
		TypeException(SourceLocation location, const std::u32string &message)
			: location{ location }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualStartLine = std::to_string(location.startLine + 1);
			auto actualStartColumn = std::to_string(location.startCol + 1);
			auto actualEndLine = std::to_string(location.endLine + 1);
			auto actualEndColumn = std::to_string(location.endCol + 1);
			return Format(U"File: {} Type Error: ({}, {}) - ({}, {}) {}",
				location.document->fileName, actualStartLine, actualStartColumn, actualEndLine, actualEndColumn, message);
		}
	};

	class SyntaxException
	{
	public:
		SourceLocation location;
		std::u32string message;

		SyntaxException() = default;
		SyntaxException(SourceLocation location, const std::u32string &message)
			: location{ location }, message{ message } {}
	};

	class CompilerException
	{
	public:
		SourceLocation location;
		std::u32string message;
		CompilerException() = default;
		CompilerException(SourceLocation location, const std::u32string& message)
			:location{ location }, message{ message }
		{

		}
		std::u32string FormattedErrorMessage() const
		{
			auto actualStartLine = std::to_string(location.startLine + 1);
			auto actualStartColumn = std::to_string(location.startCol + 1);
			auto actualEndLine = std::to_string(location.endLine + 1);
			auto actualEndColumn = std::to_string(location.endCol + 1);
			return Format(U"File: {} Compiler Error: ({}, {}) - ({}, {}) {}",
				location.document->fileName, actualStartLine, actualStartColumn, actualEndLine, actualEndColumn, message);
		}
	};

	class FileNotFoundException
	{
	public:
		std::u32string message;
		explicit FileNotFoundException(std::u32string fileName) :message{ U"cannot find file '" + fileName + U"'." }
		{

		}
	};

} // namespace cygni
#endif // CYGNI_EXCEPTION_HPP
