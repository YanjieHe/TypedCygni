#ifndef CYGNI_EXCEPTION_HPP
#define CYGNI_EXCEPTION_HPP
#include "SourcePosition.hpp"
#include "Utility.hpp"
#include <string>

namespace cygni
{
	class LexicalException
	{
	public:
		std::shared_ptr<FileLocation> document;
		int line;
		int column;
		std::u32string message;

		LexicalException(std::shared_ptr<FileLocation> document, int line, int column, const std::u32string &message)
			: document{ document }, line{ line }, column{ column }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualLine = std::to_string(line + 1);
			auto actualColumn = std::to_string(column + 1);
			return Format(U"File: {} Syntax Error: ({}, {}) {} ", document->fileName, actualLine, actualColumn, message);
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
		std::shared_ptr<FileLocation> document;
		int line;
		int column;
		std::u32string message;

		ParserException(std::shared_ptr<FileLocation> document, int line, int column, const std::u32string &message)
			: document{ document }, line{ line }, column{ column }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualLine = std::to_string(line + 1);
			auto actualColumn = std::to_string(column + 1);
			return Format(U"File: {} Syntax Error: ({}, {}) {} ", document->fileName, actualLine, actualColumn, message);
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
		SourcePosition position;
		std::u32string message;

		TypeException() = default;
		TypeException(SourcePosition position, const std::u32string &message)
			: position{ position }, message{ message } {}

		std::u32string FormattedErrorMessage() const
		{
			auto actualStartLine = std::to_string(position.startLine + 1);
			auto actualStartColumn = std::to_string(position.startCol + 1);
			auto actualEndLine = std::to_string(position.endLine + 1);
			auto actualEndColumn = std::to_string(position.endCol + 1);
			return Format(U"File: {} Type Error: ({}, {}) - ({}, {}) {}",
				position.document->fileName, actualStartLine, actualStartColumn, actualEndLine, actualEndColumn, message);
		}
	};

	class SyntaxException
	{
	public:
		SourcePosition position;
		std::u32string message;

		SyntaxException() = default;
		SyntaxException(SourcePosition position, const std::u32string &message)
			: position{ position }, message{ message } {}
	};

	class CompilerException
	{
	public:
		SourcePosition position;
		std::u32string message;
		CompilerException() = default;
		CompilerException(SourcePosition position, const std::u32string& message)
			:position{ position }, message{ message }
		{

		}
		std::u32string FormattedErrorMessage() const
		{
			auto actualStartLine = std::to_string(position.startLine + 1);
			auto actualStartColumn = std::to_string(position.startCol + 1);
			auto actualEndLine = std::to_string(position.endLine + 1);
			auto actualEndColumn = std::to_string(position.endCol + 1);
			return Format(U"File: {} Compiler Error: ({}, {}) - ({}, {}) {}",
				position.document->fileName, actualStartLine, actualStartColumn, actualEndLine, actualEndColumn, message);
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
