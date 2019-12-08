#ifndef CYGNI_UTILITY_HPP
#define CYGNI_UTILITY_HPP
#include <string>

namespace cygni {

inline static bool IsDigit(char32_t c) {
	return (c >= U'0' && c <= U'9');
}

inline static bool IsLetter(char32_t c) {
	return (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z');
}

inline static bool IsWhiteSpace(char32_t c) {
	return c == U' ' || c == U'\t' || c == U'\v' || c == U'\r' || c == U'\n';
}

int HexToInt(std::u32string hex);

std::string utf32_to_utf8(const std::u32string& utf32);

} // namespace cygni

#endif // CYGNI_UTILITY_HPP
