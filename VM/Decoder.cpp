#include "Decoder.h"
#include "Exception.h"
#include "OpCode.h"
#include <sstream>
using namespace std;

Decoder::Decoder(vector<byte>& stream) : reader(stream)
{
}

void Decoder::Decode()
{
    //    LoadConstantPool();
    LoadFunctions();
    LoadTopCode();
}

// void Decoder::LoadConstantPool()
//{
//    while (!reader.IsEof())
//	{
//        OpCode op = static_cast<OpCode>(reader.Read());
//        if (op == OpCode::constant_i32)
//		{
//            pool.push_back(Value::Int(reader.ReadInt()));
//		}
//        else if (op == OpCode::constant_i64)
//        {
//            pool.push_back(Value::Long(reader.ReadLong()));
//        }
//        else if (op == OpCode::constant_f64)
//        {
//            pool.push_back(Value::Double(reader.ReadDouble()));
//        }
//        else
//        {
//            return;
//        }
//    }
//}

void Decoder::LoadFunctions()
{
    while (!reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Peek());
        if (op == OpCode::function_info)
        {
            reader.Read();
            LoadOneFunction();
		}
		else
		{
            return;
		}
    }
}

void Decoder::LoadOneFunction()
{
    wstring name = reader.ReadString();
    i32 parametersSize = reader.ReadUShort();
    i32 frameSize = reader.ReadUShort();
    i32 codeSize = reader.ReadUShort();

	vector<byte> fcode;
    ReadBytes(fcode, codeSize);
    Function f(name, parametersSize, frameSize, fcode);
    functions.push_back(f);
}

void Decoder::LoadTopCode()
{
    vector<byte> code;
    while (!reader.IsEof())
    {
        code.push_back(reader.Read());
    }
    topCode = code;
}

void Decoder::ReadBytes(vector<byte>& vec, i32 size)
{
    for (i32 i = 0; i < size; i++)
    {
        vec.push_back(reader.Read());
    }
}
