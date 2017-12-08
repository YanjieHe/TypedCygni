#ifndef ANY_H
#define ANY_H

#include <cstdint>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;
using byte = unsigned char;

union Any
{
	i32 i32_v;
	i64 i64_v;
	f32 f32_v;
	f64 f64_v;
};

#endif // ANY_H 
