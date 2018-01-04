#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "Any.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Disassembly
{
public:
	vector<byte> stream;
    Disassembly(string path);

	void ReadConstantPool();
	void ReadCode();

private:
    u32 ReadCode(u32 offset);
    u32 ReadFunction(u32 offset);

    i32 ReadUShort(u32 offset);
    i32 ReadInt32(u32 offset);
    i64 ReadInt64(u32 offset);
    f64 ReadFloat64(u32 offset);
};

#endif // DISASSEMBLY_H
