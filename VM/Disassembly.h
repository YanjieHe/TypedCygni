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
	void ReadCode(i32 offset); 

private:
	i32 ReadUShort(i32 offset);
	i32 ReadInt32(i32 offset);
	i64 ReadInt64(i32 offset);
	f64 ReadFloat64(i32 offset);
};

#endif // DISASSEMBLY_H 
