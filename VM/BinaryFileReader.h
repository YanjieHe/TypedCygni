#ifndef BINARYFILEREADER_H
#define BINARYFILEREADER_H

#include "PrimitiveType.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::wstring;
class BinaryFileReader
{
public:
    string path;
    BinaryFileReader(string path);
    vector<byte> ReadAll();
};

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
#endif // BINARYFILEREADER_H
