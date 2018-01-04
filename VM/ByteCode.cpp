#include "ByteCode.h"

ByteCode::ByteCode() : length{0}, code{nullptr}
{
}

ByteCode::ByteCode(vector<byte> vec)
    : length{static_cast<i32>(vec.size())}, code{new byte[vec.size()]}
{
    for (u32 i = 0; i < vec.size(); i++)
    {
        code[i] = vec[i];
    }
}

ByteCode::ByteCode(int length) : length{length}, code{new byte[length]}
{
}

ByteCode::ByteCode(const ByteCode& other)
{
    length = other.length;
    code = new byte[length];
    for (i32 i = 0; i < length; i++)
    {
        code[i] = other.code[i];
    }
}

ByteCode& ByteCode::operator=(const ByteCode& other)
{
    length = other.length;
    code = new byte[length];
    for (i32 i = 0; i < length; i++)
    {
        code[i] = other.code[i];
    }
    return *this;
}
ByteCode::ByteCode(ByteCode&& other)
{
    length = other.length;
    code = other.code;
    other.code = nullptr;
}

ByteCode& ByteCode::operator=(ByteCode&& other)
{
    length = other.length;
    code = other.code;
    other.code = nullptr;
    return *this;
}

byte& ByteCode::operator[](i32 index)
{
    return code[index];
}

ByteCode::~ByteCode()
{
    delete[] code;
}

i32 ByteCode::Length()
{
    return length;
}
