#ifndef BINARYSTREAMREADER_H
#define BINARYSTREAMREADER_H
#include "PrimitiveType.h"
#include <string>
#include <vector>
using std::vector;
using std::wstring;
class BinaryStreamReader
{
public:
    u32 offset;
    vector<byte>& stream;
    BinaryStreamReader();
    BinaryStreamReader(vector<byte>& stream);
    byte Peek();
    byte Read();
    i32 ReadInt();
    i32 ReadUShort();
    i64 ReadLong();
    f64 ReadDouble();
    wstring ReadString();
    bool IsEof();
};

#endif // BINARYSTREAMREADER_H
