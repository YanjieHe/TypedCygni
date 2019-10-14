#ifndef CYGNI_UTILITY_HPP
#define CYGNI_UTILITY_HPP

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

} // namespace cygni

#endif // CYGNI_UTILITY_HPP
