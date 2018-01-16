#ifndef DECODER_H
#define DECODER_H

#include "BinaryStreamReader.h"
#include "Function.h"
#include "Value.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::wstring;
using ByteCodeList = vector<ByteCode*>;

class Decoder
{
public:
    BinaryStreamReader reader;
    vector<Value> pool;
	vector<Function> functions;
    ByteCode topCode;
    Decoder(vector<byte>& stream);
	void Decode();

private:
    void LoadConstantPool();
    void LoadFunctions();
    void LoadOneFunction();
    void LoadTopCode();
    void ReadBytes(vector<byte>& vec, i32 size);
};

#endif // DECODER_H
