#ifndef DECODER_H
#define DECODER_H

#include "Any.h"
#include "BinaryFileReader.h"
#include "Function.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::wstring;

class Decoder
{
public:
    BinaryStreamReader reader;
    vector<Any> pool;
    vector<byte> code;
	vector<Function> functions;

    Decoder(vector<byte>& stream);
	void Decode();

private:
    void LoadConstantPool();
    void LoadFunctions();
    void LoadOneFunction(std::wstring name, i32 parametersSize, i32 frameSize);
};

#endif // DECODER_H
