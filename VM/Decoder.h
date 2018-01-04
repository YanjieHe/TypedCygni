#ifndef DECODER_H
#define DECODER_H

#include "Any.h"
#include "Function.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Decoder
{
public:
	vector<byte> stream;
    vector<Any> pool;
    vector<byte> code;
	vector<Function> functions;
    Decoder(string path);
	Decoder(vector<byte> stream);
	void Decode();

private:
    u32 LoadConstantPool(u32 index);
    u32 LoadFunctions(u32 index);
    u32 LoadOneFunction(u32 index, i32 parametersSize, i32 frameSize);
    i32 ReadUShort(u32 offset);
    i32 ReadInt32(u32 offset);
    i64 ReadInt64(u32 offset);
    f64 ReadFloat64(u32 offset);
};

#endif // DECODER_H
