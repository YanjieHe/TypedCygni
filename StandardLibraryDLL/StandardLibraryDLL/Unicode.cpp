#include "stdafx.h"
#include "Unicode.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int utf8_to_utf32_len(const char * utf8)
{
	int utf8_len;
	int i;
	int utf32_len;
	char c;

	i = 0;
	utf8_len = (int)strlen(utf8);
	utf32_len = 0;
	while (i < utf8_len)
	{
		c = utf8[i];
		if ((c & 0x80) == 0)
		{
			utf32_len++;
			i = i + 1;
		}
		else if ((c & 0xE0) == 0xC0)
		{
			if (i + 1 < utf8_len)
			{
				utf32_len++;
				i = i + 2;
			}
			else
			{
				return -1;
			}
		}
		else if ((c & 0xF0) == 0xE0)
		{
			if (i + 2 < utf8_len)
			{
				utf32_len++;
				i = i + 3;
			}
			else
			{
				return -1;
			}
		}
		else if ((c & 0xF8) == 0xF0)
		{
			if (i + 3 < utf8_len)
			{
				utf32_len++;
				i = i + 4;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	return utf32_len;
}

uint32_t* utf8_to_utf32(const char * utf8, int utf32_len)
{
	int utf8_len;
	int i;
	int utf32_index;
	uint32_t* utf32;

	i = 0;
	utf8_len = (int)strlen(utf8);
	utf32_index = 0;
	utf32 = (uint32_t*)malloc(sizeof(uint32_t) * utf32_len);

	while (i < utf8_len)
	{
		if ((utf8[i] & 0x80) == 0)
		{
			utf32[utf32_index] = (utf8[i]);
			utf32_index++;
			i = i + 1;
		}
		else if ((utf8[i] & 0xE0) == 0xC0)
		{
			if (i + 1 < utf8_len)
			{
				utf32[utf32_index] = (((utf8[i] & 0x1F) << 6) | (utf8[i + 1] & 0x3F));
				utf32_index++;
				i = i + 2;
			}
			else
			{
				fprintf(stderr, "The sequence is truncated.\n");
				exit(-1);
			}
		}
		else if ((utf8[i] & 0xF0) == 0xE0)
		{
			if (i + 2 < utf8_len)
			{
				utf32[utf32_index] = (((utf8[i] & 0x0F) << 12) | ((utf8[i + 1] & 0x3F) << 6) |
					(utf8[i + 2] & 0x3F));
				utf32_index++;
				i = i + 3;
			}
			else
			{
				fprintf(stderr, "The sequence is truncated.\n");
				exit(-1);
			}
		}
		else if ((utf8[i] & 0xF8) == 0xF0)
		{
			if (i + 3 < utf8_len)
			{
				utf32[utf32_index] = (((utf8[i] & 0x07) << 18) | ((utf8[i + 1] & 0x3F) << 12) |
					((utf8[i + 2] & 0x3F) << 6) | (utf8[i + 3] & 0x3F));
				utf32_index++;
				i = i + 4;
			}
			else
			{
				fprintf(stderr, "The sequence is truncated.\n");
				exit(-1);
			}
		}
		else
		{
			fprintf(stderr, "Illegal starting byte\n");
			exit(-1);
		}
	}
	return utf32;
}

int utf32_to_utf8_len(const uint32_t * utf32, int utf32_len)
{
	int utf8_len;
	int i;
	// code point
	uint32_t cp;

	utf8_len = 0;

	for (i = 0; i < utf32_len; i++)
	{
		cp = utf32[i];
		if (cp < 0x80)
		{
			utf8_len++;
		}
		else if (cp < 0x800)
		{
			utf8_len += 2;
		}
		else if (cp < 0x10000)
		{
			utf8_len += 3;
		}
		else
		{
			utf8_len += 4;
		}
	}
	return utf8_len;
}

char * utf32_to_utf8(const uint32_t * utf32, int utf32_len, int utf8_len)
{
	char* utf8;
	int i;
	int utf8_index;
	uint32_t cp;

	utf8 = (char*)malloc(sizeof(char) * utf8_len + 1);
	utf8_index = 0;
	for (i = 0; i < utf32_len; i++)
	{
		cp = utf32[i];
		if (cp < 0x80)
		{
			utf8[utf8_index] = (cp & 0x7F);
			utf8_index++;
		}
		else if (cp < 0x800)
		{
			utf8[utf8_index] = (((cp >> 6) & 0x1F) | 0xC0);
			utf8[utf8_index + 1] = ((cp & 0x3F) | 0x80);
			utf8_index += 2;
		}
		else if (cp < 0x10000)
		{
			utf8[utf8_index] = (((cp >> 12) & 0x0F) | 0xE0);
			utf8[utf8_index + 1] = (((cp >> 6) & 0x3F) | 0x80);
			utf8[utf8_index + 2] = ((cp & 0x3F) | 0x80);
			utf8_index += 3;
		}
		else
		{
			utf8[utf8_index] = (((cp >> 18) & 0x07) | 0xF0);
			utf8[utf8_index + 1] = (((cp >> 12) & 0x3F) | 0x80);
			utf8[utf8_index + 2] = (((cp >> 6) & 0x3F) | 0x80);
			utf8[utf8_index + 3] = ((cp & 0x3F) | 0x80);
			utf8_index += 4;
		}
	}
	utf8[utf8_index] = '\0';
	return utf8;
}

