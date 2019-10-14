#ifndef CYGNI_EXCEPTION_HPP
#define CYGNI_EXCEPTION_HPP

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

} // namespace cygni
#endif // CYGNI_EXCEPTION_HPP
