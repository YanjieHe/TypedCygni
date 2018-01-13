#include "Machine.h"
#include "Exception.h"
#include "OpCode.h"
#include <iostream>
using namespace std;

Machine::Machine(Memory pool, i32 staticSize, ByteCode topCode,
                 vector<Function> functions)
    : code{topCode}, topCode{topCode}, codeStack(1000),
      codePointer{-1}, pc{0}, sp{-1}, fp{0}, ret{-1}, running{true},
      static_v(staticSize), pool{pool}, memory(1000000), functions{functions}
{
}

void Machine::Run(i32 entry)
{
	pc = entry;
    wcout << L"start running" << endl;
    Function& f = GetMainFunction();

    i32 prev_fp = fp;
    sp++;
    fp = sp - f.parametersSize;
    memory[fp + f.frameSize].i32_v = pc;
    memory[fp + f.frameSize + 1].i32_v = prev_fp;
    sp = fp + f.frameSize + (2 - 1);

    pc = 0;
    code = f.code;
    codePointer++;
    codeStack[codePointer] = &code;

    MainLoop();
}

inline i32 Machine::ReadUShort()
{
    return (code[pc + 1] << 8) + code[pc];
}

Function& Machine::GetMainFunction()
{
    for (Function& f : functions)
    {
        wcout << L"f.name = " << f.name << endl;
        if (f.name == L"main")
        {
            return f;
        }
    }
    throw NotImplementedException(L"main function not found");
}

void Machine::MainLoop()
{
    while (pc < code.Length())
    {
        OpCode op = static_cast<OpCode>(code[pc]);
        pc++;
		switch (op)
		{
        // push small number
        case OpCode::push_i32_1byte:
        {
            sp++;
            memory[sp].i32_v = code[pc];
            pc++;
            break;
        }
        case OpCode::push_i32_2byte:
        {
            sp++;
            memory[sp].i32_v = ReadUShort();
            pc += 2;
            break;
        }
        case OpCode::push_f64_0:
        {
            sp++;
            memory[sp].f64_v = 0.0;
            break;
        }
        case OpCode::push_f64_1:
        {
            sp++;
            memory[sp].f64_v = 1.0;
            break;
        }

        case OpCode::push_i32:
        {
            sp++;
            memory[sp].i32_v = pool[ReadUShort()].i32_v;
            pc += 2;
            break;
        }
        case OpCode::push_f64:
        {
            sp++;
            memory[sp].f64_v = pool[ReadUShort()].f64_v;
            pc += 2;
            break;
        }
        case OpCode::push_function:
        {
            sp++;
            memory[sp].i32_v = ReadUShort();
            pc += 2;
            break;
        }

        case OpCode::push_static_i32:
        {
            sp++;
            memory[sp].i32_v = static_v[ReadUShort()].i32_v;
            pc += 2;
            break;
        }
        case OpCode::push_static_f64:
        {
            sp++;
            memory[sp].f64_v = static_v[ReadUShort()].f64_v;
            pc += 2;
            break;
        }
        case OpCode::pop_static_i32:
        {
            static_v[ReadUShort()].i32_v = memory[sp].i32_v;
            sp--;
            pc += 2;
            break;
        }
        case OpCode::pop_static_f64:
        {
            static_v[ReadUShort()].f64_v = memory[sp].f64_v;
            sp--;
            pc += 2;
            break;
        }

        case OpCode::push_stack_i32:
        {
            sp++;
            memory[sp].i32_v = memory[fp + ReadUShort()].i32_v;
            pc += 2;
            break;
        }
        case OpCode::push_stack_f64:
        {
            sp++;
            memory[sp].f64_v = memory[fp + ReadUShort()].f64_v;
            pc += 2;
            break;
        }
        case OpCode::pop_stack_i32:
        {
            memory[fp + ReadUShort()].i32_v = memory[sp].i32_v;
            sp--;
            pc += 2;
            break;
        }
        case OpCode::pop_stack_f64:
        {
            memory[fp + ReadUShort()].f64_v = memory[sp].f64_v;
            sp--;
            pc += 2;
            break;
        }

        // i32 arithmetic operation
        case OpCode::add_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v + memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::sub_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v - memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::mul_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v * memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::div_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v / memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::mod_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v % memory[sp].i32_v;
            sp--;
            break;
        }

        // f64 arithmetic operation
        case OpCode::add_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v + memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::sub_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v - memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::mul_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v * memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::div_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v / memory[sp].f64_v;
            sp--;
            break;
        }
        /*case OpCode::mod_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v % memory[sp].f64_v;
            sp--;
            break;
        }*/

        // i32 comparision
        case OpCode::gt_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v > memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::lt_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v < memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::ge_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v >= memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::le_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v <= memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::eq_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v == memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::ne_i32:
        {
            memory[sp - 1].i32_v = memory[sp - 1].i32_v != memory[sp].i32_v;
            sp--;
            break;
        }

        // f64 comparision
        case OpCode::gt_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v > memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::lt_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v < memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::ge_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v >= memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::le_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v <= memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::eq_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v == memory[sp].f64_v;
            sp--;
            break;
        }
        case OpCode::ne_f64:
        {
            memory[sp - 1].f64_v = memory[sp - 1].f64_v != memory[sp].f64_v;
            sp--;
            break;
        }

        // minus operation
        case OpCode::minus_i32:
        {
            memory[sp].i32_v = -memory[sp].i32_v;
            break;
        }
        case OpCode::minus_f64:
        {
            memory[sp].f64_v = -memory[sp].f64_v;
            break;
        }

        // cast
        case OpCode::cast_int32_to_float64:
        {
            memory[sp].f64_v = static_cast<f64>(memory[sp].i32_v);
            break;
        }
        case OpCode::cast_float64_to_int32:
        {
            memory[sp].i32_v = static_cast<i32>(memory[sp].f64_v);
            break;
        }

			/*
			case OpCode::logical_and:
			{
			}

			case OpCode::logical_or:
			{
			}
			*/

        case OpCode::logical_not:
        {
            memory[sp].i32_v = !(memory[sp].i32_v);
            break;
        }

        // jump
        case OpCode::jump:
        {
            pc = ReadUShort();
            break;
        }
        case OpCode::jump_if_true:
        {
            if (memory[sp].i32_v)
			{
                sp--;
                pc = ReadUShort();
			}
            else
			{
                sp--;
                pc += 2;
			}
            break;
        }
        case OpCode::jump_if_false:
        {
            if (!memory[sp].i32_v)
			{
                sp--;
                pc = ReadUShort();
			}
            else
			{
                sp--;
				pc += 2;
            }
            break;
        }

        case OpCode::invoke:
        {
            u32 function_id = static_cast<u32>(memory[sp].i32_v);

            Function& f = functions[function_id];

            i32 prev_fp = fp;

            fp = sp - f.parametersSize;
            memory[fp + f.frameSize].i32_v = pc;
            memory[fp + f.frameSize + 1].i32_v = prev_fp;
            sp = fp + f.frameSize + (2 - 1);

            pc = 0;
            code = f.code;
            codePointer++;
            codeStack[codePointer] = &code;
            break;
        }

        case OpCode::return_i32:
        {
            i32 result = memory[sp].i32_v;
            wcout << "result = " << result << endl;
            sp--;

            i32 prev_fp = memory[sp].i32_v;
            sp--;

            pc = memory[sp].i32_v;
            sp--;

            sp = fp;
            fp = prev_fp;
            memory[sp].i32_v = result;

            code = *(codeStack[codePointer]);
            codePointer--;
            if (codePointer == -1)
            {
                return;
            }
            break;
        }
        case OpCode::return_f64:
        {
            f64 result = memory[sp].f64_v;
            sp--;

            i32 prev_fp = memory[sp].i32_v;
            sp--;

            pc = memory[sp].i32_v;
            sp--;

            sp = fp;
            fp = prev_fp;
            memory[sp].f64_v = result;

            code = *(codeStack[codePointer]);
            codePointer--;
            break;
        }
        default:
        {
            wcout << opcode_to_wstring(op) << endl;
            throw L"error opcode";
        }
		}
	}
}
