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
			file.read((char*)&b, sizeof(b));
			stream.push_back(b);
		}
		file.close();
	}
	else
	{
		throw "cannot open binary file" + path;
	}
}

Decoder::Decoder(vector<byte> stream)
	:stream{stream}
{
}

void Decoder::Decode()
{
	i32 index = LoadConstantPool(0);
	i32 n = stream.size();
	for (i32 i = index; i < n; i++)
	{
		code.push_back(stream[index]);
	}
}

i32 Decoder::LoadConstantPool(i32 index)
{
	bool done = false;
	while (!done)
	{
		OpCode op = (OpCode)stream[index];
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

i32 Decoder::ReadInt32(i32 offset)
{
	return 
		(stream[offset + 3] << 24)
		+ (stream[offset + 2] << 16)
		+ (stream[offset + 1] << 8)
		+ stream[offset];
}

i64 Decoder::ReadInt64(i32 offset)
{
	i64 n = 0;
	for (i32 i = 0; i < 8; ++i)
	{
		n <<= 8;
		n |= (stream[offset + i]);
	}
	return n;
}

f64 Decoder::ReadFloat64(i32 offset)
{
	union 
	{
		i64 i64_v;
		f64 f64_v;
	} value;
	value.i64_v = ReadInt64(offset);
	return value.f64_v;
}
