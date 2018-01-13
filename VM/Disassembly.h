#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "BinaryFileReader.h"
#include "PrimitiveType.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Disassembly
{
public:
    BinaryStreamReader reader;
    Disassembly(vector<byte>& stream);

	void ReadConstantPool();
    void ReadAll();

private:
    void ReadCode();
    void ReadFunction();

    i32 ReadUShort(u32 offset);
    i32 ReadInt32(u32 offset);
    i64 ReadInt64(u32 offset);
    f64 ReadFloat64(u32 offset);
};

#endif // DISASSEMBLY_H
