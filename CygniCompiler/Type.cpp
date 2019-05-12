#include "Type.hpp"

Ptr<Value> Value::IntValue = New<ValueLeaf>("Int");
Ptr<Value> Value::FloatValue = New<ValueLeaf>("Float");
Ptr<Value> Value::LongValue = New<ValueLeaf>("Long");
Ptr<Value> Value::DoubleValue = New<ValueLeaf>("Double");
Ptr<Value> Value::BoolValue = New<ValueLeaf>("Bool");
Ptr<Value> Value::CharValue = New<ValueLeaf>("Char");
Ptr<Value> Value::StringValue = New<ValueLeaf>("String");
Ptr<Value> Value::UnitValue = New<ValueLeaf>("Unit");