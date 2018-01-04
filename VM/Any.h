#ifndef ANY_H
#define ANY_H

#include <cstdint>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;
using byte = unsigned char;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

union Any {
	i32 i32_v;
	i64 i64_v;
	f32 f32_v;
	f64 f64_v;
};

class Object
{
};
#endif // ANY_H
