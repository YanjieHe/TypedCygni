#include "Machine.h"
#include "Exception.h"
#include "OpCode.h"
#include <iostream>
#define begin {
#define end                                                                    \
    break;                                                                     \
    }
using namespace std;

Machine::Machine(i32 staticSize, Decoder& decoder)
    : decoder{decoder}, static_v(staticSize), pool{decoder.pool},
      memory(1000000)
{
}

void Machine::Run(i32 entry)
{
	pc = entry;
    wcout << L"start running" << endl;
    i32 function_id = GetMainFunction();
    sp++;

    Function& f = decoder.functions[static_cast<u32>(function_id)];

    i32 prev_fp = fp;

    fp = sp - f.parametersSize;
    memory[sp].i32_v = pc;
    sp++;
    memory[sp].i32_v = prev_fp;
    sp++;
    memory[sp].i32_v = codePointer;

    pc = 0;
    code = &(f.code);
    codePointer = function_id;
    MainLoop();
}

inline i32 Machine::ReadUShort()
{
    i32 result = ((*code)[pc + 1] << 8) + (*code)[pc];
    pc += sizeof(u16);
    return result;
}

i32 Machine::ReadUShortWithoutMoving()
{
    return ((*code)[pc + 1] << 8) + (*code)[pc];
}

i32 Machine::GetMainFunction()
{
    i32 i = 0;
    for (Function& f : decoder.functions)
    {
        wcout << L"f.name = " << f.name << endl;
        if (f.name == L"main")
        {
            return i;
        }
        i++;
    }
    throw NotImplementedException(L"main function not found");
}

void Machine::MainLoop()
{
    while (pc < code->Length())
    {
        OpCode op = static_cast<OpCode>((*code)[pc]);
        pc++;
		switch (op)
		{
        // push small number
        case OpCode::push_i32_1byte:
        {
            sp++;
            memory[sp].i32_v = (*code)[pc];
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

        case OpCode::push_constant_i32:
        {
            sp++;
            memory[sp].i32_v = pool[ReadUShort()].i32_v;
            break;
        }
        case OpCode::push_constant_f64:
        {
            sp++;
            memory[sp].f64_v = pool[ReadUShort()].f64_v;
            break;
        }
        case OpCode::push_function:
        {
            sp++;
            memory[sp].i32_v = ReadUShort();
            break;
        }

        case OpCode::push_static_i32:
        {
            sp++;
            memory[sp].i32_v = static_v[ReadUShort()].i32_v;
            break;
        }
        case OpCode::push_static_f64:
        {
            sp++;
            memory[sp].f64_v = static_v[ReadUShort()].f64_v;
            break;
        }
        case OpCode::pop_static_i32:
        {
            static_v[ReadUShort()].i32_v = memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::pop_static_f64:
        {
            static_v[ReadUShort()].f64_v = memory[sp].f64_v;
            sp--;
            break;
        }

        case OpCode::push_stack_i32:
        {
            sp++;
            memory[sp].i32_v = memory[fp + ReadUShort()].i32_v;
            break;
        }
        case OpCode::push_stack_f64:
        {
            sp++;
            memory[sp].f64_v = memory[fp + ReadUShort()].f64_v;
            break;
        }
        case OpCode::pop_stack_i32:
        {
            memory[fp + ReadUShort()].i32_v = memory[sp].i32_v;
            sp--;
            break;
        }
        case OpCode::pop_stack_f64:
        {
            memory[fp + ReadUShort()].f64_v = memory[sp].f64_v;
            sp--;
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
        case OpCode::cast_i32_to_f64:
        {
            memory[sp].f64_v = static_cast<f64>(memory[sp].i32_v);
            break;
        }
        case OpCode::cast_f64_to_i32:
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
            pc = ReadUShortWithoutMoving();
            break;
        }
        case OpCode::jump_if_true:
        {
            if (memory[sp].i32_v)
			{
                sp--;
                pc = ReadUShortWithoutMoving();
			}
            else
			{
                sp--;
                pc += sizeof(u16);
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
                pc += sizeof(u16);
            }
            break;
        }

            /*
             * memory:
             * | arg1 | arg2 | function_id |
             * | arg1 | arg2 | local1 | local2 |
             * | arg1 | arg2 | local1 | local2 | pc | prev_fp | codePointer
             */
        case OpCode::invoke:
        {
            i32 function_id = memory[sp].i32_v;

            Function& f = decoder.functions[static_cast<u32>(function_id)];

            i32 prev_fp = fp;

            fp = sp - f.parametersSize;
            memory[sp].i32_v = pc;
            sp++;
            memory[sp].i32_v = prev_fp;
            sp++;
            memory[sp].i32_v = codePointer;

            pc = 0;
            code = &(f.code);
            codePointer = function_id;
            break;
        }

        case OpCode::return_i32:
        {
            i32 result = memory[sp].i32_v;
            wcout << "result = " << result << endl;
            sp--;
            codePointer = memory[sp].i32_v;
            sp--;
            i32 prev_fp = memory[sp].i32_v;
            sp--;

            pc = memory[sp].i32_v;
            sp--;

            sp = fp;
            fp = prev_fp;
            memory[sp].i32_v = result;

            if (codePointer == -1)
            {
                wcout << L"program finished" << endl;
                return;
            }
            else if (codePointer == static_cast<i32>(decoder.functions.size()))
            {
                code = &(decoder.topCode);
            }
            else
            {
                code = &(decoder.functions[static_cast<u32>(codePointer)].code);
            }
            break;
        }
        case OpCode::return_f64:
        {
            f64 result = memory[sp].f64_v;
            wcout << "result = " << result << endl;
            sp--;
            codePointer = memory[sp].i32_v;
            sp--;
            i32 prev_fp = memory[sp].i32_v;
            sp--;

            pc = memory[sp].i32_v;
            sp--;

            sp = fp;
            fp = prev_fp;
            memory[sp].f64_v = result;

            if (codePointer == -1)
            {
                wcout << L"program finished" << endl;
                return;
            }
            else if (codePointer == static_cast<i32>(decoder.functions.size()))
            {
                code = &(decoder.topCode);
            }
            else
            {
                code = &(decoder.functions[static_cast<u32>(codePointer)].code);
            }
            break;
        }
        default:
        {
            wcout << OpCodeToString(op) << endl;
            throw L"error opcode";
        }
		}
	}
}
