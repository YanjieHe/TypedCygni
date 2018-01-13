#ifndef ANY_H
#define ANY_H

#include "PrimitiveType.h"

class Any
{
public:
    i32 i32_v = 0;
    i64 i64_v = 0;
    f32 f32_v = 0;
    f64 f64_v = 0;
    void* refObject;
    static Any Int(i32 value);
    static Any Float(f32 value);
    static Any Long(i64 value);
    static Any Double(f64 value);
};

class Object
{
};
class String
{
};
#endif // ANY_H
