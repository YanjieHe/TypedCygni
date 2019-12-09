#ifndef CYGNI_EXCEPTION_HPP
#define CYGNI_EXCEPTION_HPP
#include <string>

namespace cygni {

class LexicalException {
public:
	int line;
	int column;
	std::u32string message;

	LexicalException(int line, int column, const std::u32string& message)
		: line{line}, column{column}, message{message} {
	}
};

class ArgumentException {
public:
	std::u32string message;
	explicit ArgumentException(const std::u32string& message)
		: message{message} {
	}
};

class ParserException {
public:
	int line;
	int column;
	std::u32string message;

	ParserException(int line, int column, const std::u32string& message)
		: line{line}, column{column}, message{message} {
	}
};

class NotImplementedException {
public:
	std::u32string message;

	NotImplementedException() = default;
	explicit NotImplementedException(const std::u32string& message)
		: message{message} {
	}
};

} // namespace cygni
#endif // CYGNI_EXCEPTION_HPP
