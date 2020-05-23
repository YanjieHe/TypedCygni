#ifndef VM_UNICODE_H
#define VM_UNICODE_H
#include <inttypes.h>

int utf8_to_utf32_len(const char* utf8);

uint32_t* utf8_to_utf32(const char* utf8, int utf32_len);

int utf32_to_utf8_len(const uint32_t* utf32, int utf32_len);

char* utf32_to_utf8(const uint32_t* utf32, int utf32_len, int utf8_len);
#endif // VM_UNICODE_H