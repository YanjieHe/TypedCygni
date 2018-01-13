#include "Any.h"

Any Any::Int(i32 value)
{
    Any any;
    any.i32_v = value;
    return any;
}

Any Any::Float(f32 value)
{
    Any any;
    any.f32_v = value;
    return any;
}

Any Any::Long(i64 value)
{
    Any any;
    any.i64_v = value;
    return any;
}

Any Any::Double(f64 value)
{
    Any any;
    any.f64_v = value;
    return any;
}
