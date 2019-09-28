#include "Value.h"

Value Value::Int(i32 value)
{
    Value any;
    any.i32_v = value;
    return any;
}

Value Value::Float(f32 value)
{
    Value any;
    any.f32_v = value;
    return any;
}

Value Value::Long(i64 value)
{
    Value any;
    any.i64_v = value;
    return any;
}

Value Value::Double(f64 value)
{
    Value any;
    any.f64_v = value;
    return any;
}
