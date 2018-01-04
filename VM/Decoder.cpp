#include "Decoder.h"
#include "OpCode.h"
#include <fstream>
#include <iostream>

using namespace std;

Decoder::Decoder(string path)
{
	ifstream file(path);
	if (file)
	{
		byte b;
		while (!file.eof())
		{
            file.read(reinterpret_cast<char*>(&b), sizeof(b));
			stream.push_back(b);
		}
		file.close();
		stream.pop_back();
	}
	else
	{
		wcout << "cannot open binary file" << endl;
		throw "cannot open binary file" + path;
	}
}

Decoder::Decoder(vector<byte> stream) : stream{stream}
{
}

void Decoder::Decode()
{
    wcout << "start decoding" << endl;
    u32 index = LoadConstantPool(0);
	index = LoadFunctions(index);
    u64 n = stream.size();
    for (u32 i = index; i < n; i++)
	{
		code.push_back(stream[i]);
	}
}

u32 Decoder::LoadConstantPool(u32 index)
{
	bool done = false;
    u32 n = static_cast<u32>(stream.size());
	while (!done && index < n)
	{
        OpCode op = static_cast<OpCode>(stream[index]);
		switch (op)
		{
        case OpCode::constant_i32:
        {
            index++;

            Any value;
            value.i32_v = ReadInt32(index);
            pool.push_back(value);

            index += 4;
            break;
        }
        case OpCode::constant_i64:
        {
            index++;

            Any value;
            value.i64_v = ReadInt64(index);
            pool.push_back(value);

            index += 8;
            break;
        }
        case OpCode::constant_f64:
        {
            index++;

            Any value;
            value.f64_v = ReadFloat64(index);
            pool.push_back(value);

            index += 8;
            break;
		}
        default:
        {
            done = true;
            break;
        }
        }
    }
	return index;
}

u32 Decoder::LoadFunctions(u32 index)
{
	bool done = false;
    u32 n = static_cast<u32>(stream.size());
	while (!done && index < n)
	{
        OpCode op = static_cast<OpCode>(stream[index]);
		if (op == OpCode::function_begin)
		{
			index++;
            i32 parametersSize = ReadUShort(index);
			index += 2;
            i32 frameSize = ReadUShort(index);
			index += 2;
			index = LoadOneFunction(index, parametersSize, frameSize);
		}
		else
		{
			done = true;
		}
	}
	return index;
}

u32 Decoder::LoadOneFunction(u32 index, i32 parametersSize, i32 frameSize)
{
	bool done = false;
	vector<byte> fcode;
	while (!done)
	{
        OpCode op = static_cast<OpCode>(stream[index]);
		if (op == OpCode::function_end)
		{
			index++;
			done = true;
		}
		else
		{
			fcode.push_back(stream[index]);
			index++;
            u32 size = OperandSize(op);
            for (u32 i = 0; i < size; i++)
			{
				fcode.push_back(stream[index + i]);
			}
			index += size;
		}
	}
	functions.push_back(Function(parametersSize, frameSize, fcode));
	return index;
}

i32 Decoder::ReadUShort(u32 offset)
{
    return (stream[offset + 1] << 8) + stream[offset];
}

i32 Decoder::ReadInt32(u32 offset)
{
    return (stream[offset + 3] << 24) + (stream[offset + 2] << 16) +
           (stream[offset + 1] << 8) + stream[offset];
}

i64 Decoder::ReadInt64(u32 offset)
{
	i64 n = 0;
    for (u32 i = 0; i < 8; ++i)
	{
		n <<= 8;
		n |= (stream[offset + i]);
	}
	return n;
}

f64 Decoder::ReadFloat64(u32 offset)
{
    union {
		i64 i64_v;
		f64 f64_v;
	} value;
	value.i64_v = ReadInt64(offset);
	return value.f64_v;
}
