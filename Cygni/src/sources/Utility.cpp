#include "Utility.hpp"
#include "Exception.hpp"

#include <codecvt>
#include <locale>

namespace cygni {

int HexToInt(std::u32string hex) {
	int val = 0;
	for (size_t i = 0; i < hex.size(); i++) {
		val = val * 16;
		if (hex[i] >= U'0' && hex[i] <= U'9') {
			val = val + (hex[i] - U'0');
		} else if (hex[i] >= 'a' && hex[i] <= U'f') {
			val = val + (hex[i] - U'a');
		} else if (hex[i] >= 'A' && hex[i] <= U'F') {
			val = val + (hex[i] - U'F');
		} else {
			throw ArgumentException(U"hex digit character out of range");
		}
	}
	return val;
}

std::string utf32_to_utf8(const std::u32string& utf32) {
	std::string res;
	for (char32_t cp : utf32) {
		if (cp < 0x80) {
			res.push_back(cp & 0x7F);
		} else if (cp < 0x800) {
			res.push_back(((cp >> 6) & 0x1F) | 0xC0),
				res.push_back((cp & 0x3F) | 0x80);
		} else if (cp < 0x10000) {
			res.push_back(((cp >> 12) & 0x0F) | 0xE0),
				res.push_back(((cp >> 6) & 0x3F) | 0x80),
				res.push_back((cp & 0x3F) | 0x80);
		} else {
			res.push_back(((cp >> 18) & 0x07) | 0xF0),
				res.push_back(((cp >> 12) & 0x3F) | 0x80),
				res.push_back(((cp >> 6) & 0x3F) | 0x80),
				res.push_back((cp & 0x3F) | 0x80);
		}
	}
	return res;
}

} // namespace cygni
