#include "Type.hpp"

Ptr<Type> Type::VOID = New<VoidType>();
Ptr<Type> Type::INT = New<IntType>();
Ptr<Type> Type::FLOAT = New<FloatType>();
Ptr<Type> Type::LONG = New<LongType>();
Ptr<Type> Type::DOUBLE = New<DoubleType>();
Ptr<Type> Type::CHAR = New<CharType>();
Ptr<Type> Type::STRING = New<StringType>();
Ptr<Type> Type::BOOL = New<BoolType>();