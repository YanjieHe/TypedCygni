using System;
using System.Collections.Generic;
using Op = Compiler.Op;
using System.Text;
using System.Linq;

namespace VirtualMachine
{
    public class Union
    {
        public int i32_v;
        public long i64_v;
        public float f32_v;
        public double f64_v;
        public Object pointer;
    }
    public class Value
    {
        public bool isPointer;
        public Union u;
        public Value()
        {
            this.u = new Union();
        }
    }
    public enum TypeTag
    {
        TYPE_I32,
        TYPE_I64,
        TYPE_F32,
        TYPE_F64,
        TYPE_BOOL,
        TYPE_CHAR,
        TYPE_STRING,
        TYPE_ARRAY
    }
    public class TaggedValue
    {
        public TypeTag tag;
        public Union u;
        public TaggedValue()
        {
            this.u = new Union();
        }
    }
    public class Function
    {
        public byte[] name;
        public ushort locals;
        public ushort stack;
        public ushort args_size;
        public byte[] code;
        public Env env;
    }
    public class Env
    {
        public TaggedValue[] constantPool;
        public Value[] variables;
        public Function[] functions;
    }
    public interface IRecord
    {
         Env GetEnv();
    }
    public class Module: IRecord
    {
        public byte[] name;
        public Env env;
        public Module()
        {
            this.env = new Env();
        }

        public Env GetEnv()
        {
            return env;
        }
    }
    public class Class:IRecord
    {
        public byte[] name;
        Env env;
        public Class()
        {
            this.env = new Env();
        }
        public Env GetEnv()
        {
            return env;
        }
    }
    public class Program
    {
        public byte[] path;
        public Module[] modules;
        public Class[] classes;
    }
    public class VM
    {
        public Program program;
        public int sp;
        public int pc;
        public int fp;
        public Value[] stack;
        public VM(Program program, int stackSize)
        {
            this.program = program;
            this.stack = new Value[stackSize];
            for (int i = 0; i < this.stack.Length; i++)
            {
                stack[i] = new Value();
            }
            this.sp = -1;
            this.pc = 0;
            this.fp = 0;
        }
        public static ushort USHORT(byte[] bytes, int index)
        {
            return (ushort)((bytes[index]) + (bytes[index + 1] << 8));
        }
        public static void ViewStack(VM vm)
        {
            Console.Write("[");
            for (int i = 0; i < vm.stack.Length; i++)
            {
                if (i != 0)
                {
                    Console.Write(", ");
                }
                if (i == vm.sp)
                {
                    ConsoleColor color = Console.ForegroundColor;
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write(vm.stack[i].u.i32_v);
                    Console.ForegroundColor = color;
                }
                else if (i == vm.fp)
                {
                    ConsoleColor color = Console.ForegroundColor;
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.Write(vm.stack[i].u.i32_v);
                    Console.ForegroundColor = color;
                }
                else
                {
                    Console.Write(vm.stack[i].u.i32_v);
                }
            }
            Console.WriteLine("]");
        }
        public static void Run(VM vm, Function entry)
        {
            Function current = entry;
            InitializeEntryFunction(vm, entry);
            while (vm.pc < current.code.Length)
            {
                byte op = current.code[vm.pc];
                vm.pc++;
                switch ((Op)op)
                {
                    case Op.PUSH_CONST_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v =
                               current.env.constantPool[index].u.i32_v;
                            vm.pc += 2;
                            break;
                        }
                    case Op.PUSH_LOCAL_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v =
                                vm.stack[vm.fp + index].u.i32_v;
                            vm.pc += 2;
                            break;
                        }
                    case Op.POP_LOCAL_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.stack[vm.fp + index].u.i32_v =
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            vm.pc += 2;
                            break;
                        }
                    case Op.ADD_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v +
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.SUB_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v -
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.MUL_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v *
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.DIV_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v /
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.MOD_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v %
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.GT_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v >
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.LT_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v <
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.GE_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v >=
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.LE_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v <=
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.EQ_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v ==
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.NE_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v !=
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.CALL:
                        {
                            Function function = (Function)(vm.stack[vm.sp].u.pointer);
                            Console.WriteLine("successfully load function: {0}", Encoding.UTF8.GetString(function.name));
                            int previous_fp = vm.fp;
                            vm.fp = vm.sp - function.args_size;
                            int _base = vm.fp + function.args_size + function.locals;
                            vm.stack[_base].u.i32_v = vm.pc;
                            vm.stack[_base + 1].u.i32_v = previous_fp;
                            vm.stack[_base + 2].u.pointer = current;
                            vm.sp = vm.fp + function.args_size + function.locals + 2;
                            current = function;
                            vm.pc = 0;
                            break;
                        }
                    case Op.JUMP_IF_TRUE:
                        {
                            bool condition = Convert.ToBoolean(vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            if (condition)
                            {
                                vm.pc = USHORT(current.code, vm.pc);
                            }
                            else
                            {
                                vm.pc += 2;
                            }
                            break;
                        }
                    case Op.JUMP_IF_FALSE:
                        {
                            bool condition = Convert.ToBoolean(vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            if (!condition)
                            {
                                vm.pc = USHORT(current.code, vm.pc);
                            }
                            else
                            {
                                vm.pc += 2;
                            }
                            break;
                        }
                    case Op.JUMP:
                        {
                            vm.pc = USHORT(current.code, vm.pc);
                            break;
                        }
                    case Op.PUSH_FUNCTION:
                        {
                            //ViewStack(vm);
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.pointer =
                                current.env.functions[index];

                            vm.pc += 2;
                            break;
                        }
                    case Op.RETURN_I32:
                        {
                            int result = vm.stack[vm.sp].u.i32_v;
                            int _base = vm.fp + current.args_size + current.locals;
                            vm.sp = vm.fp;
                            vm.pc = vm.stack[_base].u.i32_v;
                            vm.fp = vm.stack[_base + 1].u.i32_v;

                            vm.stack[vm.sp].u.i32_v = result;
                            Console.WriteLine("result = {0}\n", result);
                            //ViewStack(vm);
                            Function previous = (Function)(vm.stack[_base + 2].u.pointer);
                            if (vm.fp == -1)
                            {
                                Console.WriteLine("program result: {0}", result);
                                return;
                            }
                            current = previous;
                            break;
                        }
                    case Op.PUSH_MODULE:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.pointer = vm.program.modules[index];
                            vm.pc += 2;
                            break;
                        }
                    case Op.PUSH_MEMBER_FUNCTION:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            IRecord record = (IRecord)vm.stack[vm.sp].u.pointer;
                            vm.sp--;
                            vm.sp++;
                            vm.stack[vm.sp].u.pointer = record.GetEnv().functions[index];
                            vm.pc += 2;
                            break;
                        }
                    case Op.PUSH_FIELD_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            IRecord record = (IRecord)vm.stack[vm.sp].u.pointer;
                            vm.sp--;
                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v = record.GetEnv().variables[index].u.i32_v;
                            vm.pc += 2;
                            break;
                        }
                    case Op.POP_FIELD_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            int value = vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            IRecord record = (IRecord)vm.stack[vm.sp].u.pointer;
                            vm.sp--;
                            record.GetEnv().variables[index].u.i32_v = value;
                            vm.pc += 2;
                            break;
                        }
                    case Op.NEW:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            throw new UriFormatException();
                        }
                    default:
                        {
                            Console.Write("not supported operation code");
                            throw new NotSupportedException();
                        }
                }
            }
        }

        public static void InitializeEntryFunction(VM vm, Function entry)
        {
            int _base = vm.fp + entry.args_size + entry.locals;
            vm.stack[_base].u.i32_v = vm.pc;
            vm.stack[_base + 1].u.i32_v = -1;
            vm.stack[_base + 2].u.pointer = null;
            vm.sp = vm.fp + entry.args_size + entry.locals + 2;
        }
    }
    public class Parser
    {
        public byte[] bytes;
        public int offset;
        public Parser(byte[] bytecode)
        {
            this.bytes = bytecode;
            this.offset = 0;
        }
        public Program Parse()
        {
            Program program = new Program();
            program.path = ReadString();
            int moduleCount = ReadU16();
            int classCount = ReadU16();
            program.modules = ParseModules(moduleCount);
            return program;
        }

        Module[] ParseModules(int moduleCount)
        {
            Module[] modules = new Module[moduleCount];
            for (int i = 0; i < moduleCount; i++)
            {
                Module module = new Module();

                module.name = ReadString();
                int constantPoolCount = ReadU16();
                int variableCount = ReadU16();
                int functionCount = ReadU16();
                module.env.constantPool = ParseConstantPool(constantPoolCount);
                module.env.variables = new Value[variableCount];
                InitializeVariables(module.env.variables);
                module.env.functions = ParseFunctions(functionCount, module.env);
                modules[i] = module;
            }
            return modules;
        }

        void InitializeVariables(Value[] variables)
        {
            for (int i = 0; i < variables.Length; i++)
            {
                variables[i] = new Value();
            }
        }
        Function[] ParseFunctions(int functionCount, Env env)
        {
            Function[] functions = new Function[functionCount];
            for (int i = 0; i < functionCount; i++)
            {
                Function function = new Function();
                function.name = ReadString();
                function.locals = ReadU16();
                function.stack = ReadU16();
                function.args_size = ReadU16();
                int codeSize = ReadU16();
                function.code = ReadBytes(codeSize);
                function.env = env;
                functions[i] = function;
            }
            return functions;
        }

        TaggedValue[] ParseConstantPool(int constantCount)
        {
            TaggedValue[] constantPool = new TaggedValue[constantCount];
            for (int i = 0; i < constantCount; i++)
            {
                constantPool[i] = new TaggedValue();
                TypeTag tag = (TypeTag)bytes[offset];
                offset++;
                switch (tag)
                {
                    case TypeTag.TYPE_I32:
                        constantPool[i].u.i32_v = BitConverter.ToInt32(ReadBytes(4), 0);
                        break;
                    case TypeTag.TYPE_I64:
                        constantPool[i].u.i64_v = BitConverter.ToInt64(ReadBytes(8), 0);
                        break;
                    case TypeTag.TYPE_F32:
                        constantPool[i].u.f32_v = BitConverter.ToSingle(ReadBytes(4), 0);
                        break;
                    case TypeTag.TYPE_F64:
                        constantPool[i].u.f64_v = BitConverter.ToDouble(ReadBytes(8), 0);
                        break;
                    case TypeTag.TYPE_STRING:
                        constantPool[i].u.pointer = ReadString();
                        break;
                    default:
                        throw new NotSupportedException();
                }
            }
            return constantPool;
        }

        ushort ReadU16()
        {
            ushort value = (ushort)(bytes[offset] + (bytes[offset + 1] << 8));
            offset = offset + 2;
            return value;
        }

        byte[] ReadString()
        {
            ushort length = ReadU16();
            byte[] characters = ReadBytes(length);
            return characters;
        }

        byte[] ReadBytes(int length)
        {
            byte[] array = new byte[length];
            for (int i = 0; i < length; i++)
            {
                array[i] = bytes[offset + i];
            }
            offset = offset + length;
            return array;
        }
    }
}
