#ifndef CYGNI_TOKEN_HPP
#define CYGNI_TOKEN_HPP

#include "Enum.hpp"
#include <string>
#include <unordered_map>

namespace cygni
{

	class Token
	{
	public:
		int line;
		int column;
		Tag tag;
		std::u32string text;

		Token();

		Token(int line, int column, Tag tag, const std::u32string& text);

		static Tag IdentifyKeyword(Tag tag, const std::u32string& text);
		static std::unordered_map<std::u32string, Tag> keywords;
		static std::unordered_map<std::u32string, Tag> operators;
	};

} // namespace cygni
#endif // CYGNI_TOKEN_HPP
