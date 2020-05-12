#ifndef DATA_H
#define DATA_H
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

typedef uint8_t Byte;

typedef struct
{
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		void* obj;
	}u;
	uint8_t is_pointer;
} Value;
#endif // DATA_H