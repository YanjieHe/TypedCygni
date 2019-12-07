#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "Decoder.h"
#include "PrimitiveType.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Disassembly
{
public:
    Decoder& decoder;
    Disassembly(Decoder& decoder);

	void ReadConstantPool();
    void ReadAll();

private:
    void ReadCode(ByteCode code);
    void ReadFunctions();
    void ReadOneFunction();
    void ReadOneOperation();

    i32 ReadUShort(u32 offset);
    i32 ReadInt32(u32 offset);
    i64 ReadInt64(u32 offset);
    f64 ReadFloat64(u32 offset);
};

#endif // DISASSEMBLY_H
