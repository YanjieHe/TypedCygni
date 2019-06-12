#include "Type.hpp"

Ptr<Value> Value::VOID = New<VoidValue>();
Ptr<Value> Value::INT = New<IntValue>();
Ptr<Value> Value::FLOAT = New<FloatValue>();
Ptr<Value> Value::LONG = New<LongValue>();
Ptr<Value> Value::DOUBLE = New<DoubleValue>();
Ptr<Value> Value::CHAR = New<CharValue>();
Ptr<Value> Value::STRING = New<StringValue>();
Ptr<Value> Value::BOOL = New<BoolValue>();