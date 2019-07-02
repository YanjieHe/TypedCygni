#ifndef ENCODING_HPP
#define ENCODING_HPP
#include <string>
#include <cstdint>

class Encoding
{
public:
	static std::basic_string<uint16_t> Utf8ToUtf16(const std::basic_string<uint8_t> &utf8);

	static std::basic_string<uint8_t> Utf16ToUtf8(const std::basic_string<uint16_t> &utf16);

	static std::basic_string<uint32_t> Utf16ToUnicode(const std::basic_string<uint16_t> &input);

	static std::basic_string<uint8_t> UnicodeToUtf8(const std::basic_string<uint32_t> &input);
};

std::basic_string<uint8_t> utf8(const char *str);

std::basic_string<uint16_t> utf16(const char *str);

#endif // ENCODING_HPP