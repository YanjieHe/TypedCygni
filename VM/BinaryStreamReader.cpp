#include "BinaryStreamReader.h"
#include <sstream>
using namespace std;
BinaryStreamReader::BinaryStreamReader(vector<byte>& stream)
    : offset{0}, stream{stream}
{
}

byte BinaryStreamReader::Peek()
{
    return stream.at(offset);
}

byte BinaryStreamReader::Read()
{
    byte result = stream.at(offset);
    offset++;
    return result;
}

i32 BinaryStreamReader::ReadInt()
{
    i32 value = (stream[offset + 3] << 24) + (stream[offset + 2] << 16) +
                (stream[offset + 1] << 8) + stream[offset];
    offset += sizeof(i32);
    return value;
}

i32 BinaryStreamReader::ReadUShort()
{
    i32 value = (stream[offset + 1] << 8) + stream[offset];
    offset += sizeof(u16);
    return value;
}

i64 BinaryStreamReader::ReadLong()
{
    i64 n = 0;
    for (u32 i = 0; i < sizeof(i64); ++i)
    {
        n <<= 8;
        n |= (stream[offset + i]);
    }
    offset += sizeof(i64);
    return n;
}

f64 BinaryStreamReader::ReadDouble()
{
    union {
        i64 i64_v;
        f64 f64_v;
    } value;
    value.i64_v = ReadLong();
    return value.f64_v;
}

wstring BinaryStreamReader::ReadString()
{
    i32 strSize = ReadInt();
    wostringstream wos;
    for (i32 i = 0; i < strSize; i++)
    {
        wos << static_cast<wchar_t>(ReadInt());
    }
    return wos.str();
}

bool BinaryStreamReader::IsEof()
{
    return offset >= stream.size();
}
