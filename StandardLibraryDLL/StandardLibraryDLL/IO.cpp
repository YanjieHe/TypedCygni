#include "stdafx.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "IO.h"
#include "Unicode.h"
#include <stdio.h>
#include <malloc.h>

int put_int(Value * env)
{
	printf("%d", env[0].u.i32_v);
	return 0;
}

int put_str(Value * env)
{
	Array* array;
	int32_t utf8_len;
	char* utf8;

	array = GET_CHAR_ARRAY(env[0]);
	utf8_len = utf32_to_utf8_len((uint32_t*)array->u.i32_array, array->length);
	utf8 = utf32_to_utf8((uint32_t*)array->u.i32_array, array->length, utf8_len);
	printf(utf8);
	free(utf8);
	return 0;
}

int put_double(Value * env)
{
	printf("%f", env[0].u.f64_v);
	return 0;
}
