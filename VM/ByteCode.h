#ifndef BYTECODE_H
#define BYTECODE_H
#include <stdint.h>
#include <vector>
using byte = unsigned char;
using i32 = int32_t;
using u32 = uint32_t;
using std::vector;

class ByteCode
{
public:
    ByteCode();
    ByteCode(vector<byte> vec);
    ByteCode(int length);
    ByteCode(const ByteCode& other);
    ByteCode& operator=(const ByteCode& other);
    ByteCode(ByteCode&& other);
    ByteCode& operator=(ByteCode&& other);
    byte& operator[](i32 index);
    ~ByteCode();
    i32 Length();

private:
    int length;
    byte* code;
};

#endif // BYTECODE_H
