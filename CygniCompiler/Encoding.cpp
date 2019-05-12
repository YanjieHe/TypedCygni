#include "Encoding.hpp"

std::basic_string<uint16_t> Encoding::Utf8ToUtf16(const std::basic_string<uint8_t> &utf8)
{
	std::basic_string<uint16_t> result;
	int i = 0;
	int n = utf8.size();
	while (i < n)
	{
		uint8_t code = utf8[i];
		if (((code >> 7) & 0xFF) == 0x0)
		{
			i++;
			result.push_back(code);
		}
		else if (((code >> 5) & 0xFF) == 0x6)
		{
			i++;
			uint8_t code2 = utf8[i];
			i++;
			int32_t byte1 = (code & 0x1F) << 6;
			int32_t byte2 = code2 & 0x3F;
			uint16_t utf16 = byte1 | byte2;
			result.push_back(utf16);
		}
		else if (((code >> 4) & 0xFF) == 0xE)
		{
			i++;
			uint8_t code2 = utf8[i];
			i++;
			uint8_t code3 = utf8[i];
			i++;
			int32_t byte1 = (code & 0x0F) << 12;
			int32_t byte2 = (code2 & 0x3F) << 6;
			int32_t byte3 = code3 & 0x3F;
			uint16_t utf16 = byte1 | byte2 | byte3;
			result.push_back(utf16);
		}
		else if (((code >> 3) & 0xFF) == 0x1E)
		{
			i++;
			uint8_t code2 = utf8[i];
			i++;
			uint8_t code3 = utf8[i];
			i++;
			uint8_t code4 = utf8[i];
			i++;
			int32_t byte1 = (code & 0x07) << 18;
			int32_t byte2 = (code2 & 0x3F) << 12;
			int32_t byte3 = (code3 & 0x3F) << 6;
			int32_t byte4 = code4 & 0x3F;
			uint16_t utf16 = byte1 | byte2 | byte3 | byte4;
			result.push_back(utf16);
		}
		else if (((code >> 2) & 0xFF) == 0x3E)
		{
			i++;
			uint8_t code2 = utf8[i];
			i++;
			uint8_t code3 = utf8[i];
			i++;
			uint8_t code4 = utf8[i];
			i++;
			uint8_t code5 = utf8[i];
			i++;
			int32_t byte1 = (code & 0x03) << 24;
			int32_t byte2 = (code2 & 0x3F) << 18;
			int32_t byte3 = (code3 & 0x3F) << 12;
			int32_t byte4 = (code4 & 0x3F) << 6;
			int32_t byte5 = code5 & 0x3F;
			uint16_t utf16 = byte1 | byte2 | byte3 | byte4 | byte5;
			result.push_back(utf16);
		}
		else if (((code >> 1) & 0xFF) == 0x7E)
		{
			i++;
			uint8_t code2 = utf8[i];
			i++;
			uint8_t code3 = utf8[i];
			i++;
			uint8_t code4 = utf8[i];
			i++;
			uint8_t code5 = utf8[i];
			i++;
			uint8_t code6 = utf8[i];
			i++;
			int32_t byte1 = (code & 0x01) << 30;
			int32_t byte2 = (code2 & 0x3F) << 24;
			int32_t byte3 = (code3 & 0x3F) << 18;
			int32_t byte4 = (code4 & 0x3F) << 12;
			int32_t byte5 = (code5 & 0x3F) << 6;
			int32_t byte6 = code6 & 0x3F;
			uint16_t utf16 = byte1 | byte2 | byte3 | byte4 | byte5 | byte6;
			result.push_back(utf16);
		}
		else
		{
			exit(1);
		}
	}
	return result;
}

std::basic_string<uint8_t> Encoding::Utf16ToUtf8(const std::basic_string<uint16_t> &utf16)
{
	std::basic_string<uint8_t> result;
	for (uint32_t code : utf16)
	{
		if (code > 0x0000 && code <= 0x007F)
		{
			result.push_back(code);
		}
		else if (code >= 0x0080 && code <= 0x07FF)
		{
			uint8_t byte1 = 0xC0 | ((code >> 6) & 0x1F);
			uint8_t byte2 = 0x80 | (code & 0x3F);
			result.push_back(byte1);
			result.push_back(byte2);
		}
		else if (code >= 0x0800 && code <= 0xFFFF)
		{
			uint8_t byte1 = 0xE0 | ((code >> 12) & 0x0F);
			uint8_t byte2 = 0x80 | ((code >> 6) & 0x3F);
			uint8_t byte3 = 0x80 | (code & 0x3F);
			result.push_back(byte1);
			result.push_back(byte2);
			result.push_back(byte3);
		}
		else if (code >= 0x00010000 && code <= 0x001FFFFF)
		{
			uint8_t byte1 = 0xF0 | ((code >> 18) & 0x07);
			uint8_t byte2 = 0x80 | ((code >> 12) & 0x3F);
			uint8_t byte3 = 0x80 | ((code >> 6) & 0x3F);
			uint8_t byte4 = 0x80 | (code & 0x3F);
			result.push_back(byte1);
			result.push_back(byte2);
			result.push_back(byte3);
			result.push_back(byte4);
		}
		else if (code >= 0x00200000 && code <= 0x03FFFFFF)
		{
			uint8_t byte1 = 0xF8 | ((code >> 24) & 0x03);
			uint8_t byte2 = 0x80 | ((code >> 18) & 0x3F);
			uint8_t byte3 = 0x80 | ((code >> 12) & 0x3F);
			uint8_t byte4 = 0x80 | ((code >> 6) & 0x3F);
			uint8_t byte5 = 0x80 | (code & 0x3F);
			result.push_back(byte1);
			result.push_back(byte2);
			result.push_back(byte3);
			result.push_back(byte4);
			result.push_back(byte5);
		}
		else if (code >= 0x04000000 && code <= 0x7FFFFFFF)
		{
			uint8_t byte1 = 0xFC | ((code >> 30) & 0x01);
			uint8_t byte2 = 0x80 | ((code >> 24) & 0x3F);
			uint8_t byte3 = 0x80 | ((code >> 18) & 0x3F);
			uint8_t byte4 = 0x80 | ((code >> 12) & 0x3F);
			uint8_t byte5 = 0x80 | ((code >> 6) & 0x3F);
			uint8_t byte6 = 0x80 | (code & 0x3F);
			result.push_back(byte1);
			result.push_back(byte2);
			result.push_back(byte3);
			result.push_back(byte4);
			result.push_back(byte5);
			result.push_back(byte6);
		}
	}
	return result;
}

std::basic_string<uint32_t> Encoding::Utf16ToUnicode(const std::basic_string<uint16_t> &input)
{
	std::basic_string<uint32_t> result;
	int idx = 0;
	int n = input.size();
	while (idx < n)
	{
		uint16_t c1 = input[idx];
		if (c1 >= 0xD800 && c1 <= 0xDFFF)
		{
			idx++;
			if (idx < n)
			{
				uint16_t c2 = input[idx];
				if (c2 >= 0xDC00 && c2 <= 0xDFFF)
				{
					result.push_back((c1 - 0xD800) * 0x400 + c2 - 0xDC00 + 0x10000);
				}
			}
		}
		else
		{
			idx++;
			result.push_back(c1);
		}
	}
	if (idx < n)
	{
		result.push_back(input[idx]);
	}
	return result;
}

std::basic_string<uint8_t> Encoding::UnicodeToUtf8(const std::basic_string<uint32_t> &input)
{
	std::basic_string<uint8_t> result;
	int i = 0;
	int n = input.size();
	for (uint32_t cp : input)
	{
		if (cp < 0x80)
		{
			result.push_back(cp & 0x7F);
		}
		else if (cp < 0x800)
		{
			result.push_back(((cp >> 6) & 0x1F) | 0xC0);
			result.push_back((cp & 0x3F) | 0x80);
		}
		else if (cp < 0x10000)
		{
			result.push_back(((cp >> 12) & 0x0F) | 0xE0);
			result.push_back(((cp >> 6) & 0x3F) | 0x80);
			result.push_back((cp & 0x3F) | 0x80);
		}
		else
		{
			result.push_back(((cp >> 18) & 0x07) | 0xF0);
			result.push_back(((cp >> 12) & 0x3F) | 0x80);
			result.push_back(((cp >> 6) & 0x3F) | 0x80);
			result.push_back((cp & 0x3F) | 0x80);
		}
	}
	return result;
}
std::basic_string<uint8_t> utf8(const char *str)
{
	std::basic_string<uint8_t> utf8Str;
	int length = strlen(str);
	utf8Str.reserve(length);
	for (int i = 0; i < length; i++)
	{
		utf8Str.push_back((uint8_t)(str[i] & 0xff));
	}
	return utf8Str;
}

std::basic_string<uint16_t> utf16(const char *str)
{
	return Encoding::Utf8ToUtf16(utf8(str));
}
