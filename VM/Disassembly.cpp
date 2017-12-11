#include "Disassembly.h"
#include "OpCode.h"
#include <iostream>
#include <fstream>

using namespace std;

Disassembly::Disassembly(string path)
{
	ifstream file(path, ifstream::binary);
	if (file)
	{
		byte b;
		while (!file.eof())
		{
			file.read((char*)&b, sizeof(b));
			stream.push_back(b);
		}
		file.close();
		stream.pop_back();
		wcout << "stream size: " << stream.size() << endl;
	}
	else
	{
		throw "cannot open binary file" + path;
	}
}

void Disassembly::ReadCode(i32 offset)
{
	i32 i = offset;
	while (i < stream.size())
	{
		wcout << i << ": ";
		OpCode op = (OpCode) stream.at(i);
		if (OperandSize(op) == 0)
		{
			wcout << opcode_to_wstring(op) << endl;
			i++;
		}
		else if (OperandSize(op) == 1)
		{
			wcout << opcode_to_wstring(op) << L"  ";
			i++;
			wcout << stream.at(i) << endl;
			i++;
		}
		else if (OperandSize(op) == 2)
		{
			wcout << opcode_to_wstring(op) << L"  ";
			i++;
			wcout << ReadUShort(i) << endl;
			i += 2;
		}
		else
		{
			wcout << L"not implemented opcode: " + opcode_to_wstring(op) << endl;
			throw L"not implemented opcode: " + opcode_to_wstring(op);
		}
	}
}

i32 Disassembly::ReadUShort(i32 offset)
{
	return (stream[offset + 1] << 8) + stream[offset];
}

i32 Disassembly::ReadInt32(i32 offset)
{
	return 
		(stream[offset + 3] << 24)
		+ (stream[offset + 2] << 16)
		+ (stream[offset + 1] << 8)
		+ stream[offset];
}

i64 Disassembly::ReadInt64(i32 offset)
{
	i64 n = 0;
	for (i32 i = 0; i < sizeof(i64); ++i)
	{
		n <<= 8;
		n |= (stream[offset + i]);
	}
	return n;
}

f64 Disassembly::ReadFloat64(i32 offset)
{
	union 
	{
		i64 i64_v;
		f64 f64_v;
	} value;
	value.i64_v = ReadInt64(offset);
	return value.f64_v;
}
