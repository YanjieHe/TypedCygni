#ifndef CYGNI_TOKEN_HPP
#define CYGNI_TOKEN_HPP

#include "Enum.hpp"
#include <string>

namespace cygni {

	class Token {
	public:
		int line;
		int column;
		Tag tag;
		std::u32string text;

		Token();

		Token(int line, int column, Tag tag, const std::u32string& text);

	private:
		static Tag IdentifyKeyword(Tag tag, const std::u32string& text);
	};

} // namespace cygni
#endif // CYGNI_TOKEN_HPP
