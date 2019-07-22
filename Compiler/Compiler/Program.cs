using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using VM = VirtualMachine.VM;
using Disassembler;
namespace Compiler
{
    class MainClass
    {
        public static string TaggedValueToString(VirtualMachine.TaggedValue value)
        {
            var tag = value.tag;
            switch (tag)
            {
                case VirtualMachine.TypeTag.TYPE_I32:
                    return value.u.i32_v.ToString();
                case VirtualMachine.TypeTag.TYPE_I64:
                    return value.u.i64_v.ToString();
                case VirtualMachine.TypeTag.TYPE_F32:
                    return value.u.f32_v.ToString();
                case VirtualMachine.TypeTag.TYPE_F64:
                    return value.u.f64_v.ToString();
                default:
                    return value.u.pointer.ToString();
            }
        }
        public static void Main(string[] args)
        {
            Console.WriteLine("Press any key to continue...");
            Console.ReadKey();
            String path = "test_code.txt";
            String code = File.ReadAllText(path);
            Lexer lexer = new Lexer(code);
            try
            {
                /* pass 1: lexical analysis */
                var tokens = lexer.ReadAll();
                foreach (var token in tokens)
                {
                    Console.WriteLine(token);
                }

                /* pass 2: syntax analysis */
                var parser = new Parser(path, tokens);
                Program program = parser.ParseProgram();

                /* pass 3: abstract syntax tree to json */
                AstJson astJson = new AstJson();
                JsonObject json = astJson.Visit(program);

                File.WriteAllText("ast.json", new JsonViewer().Visit(json));

                /* pass 4: register types */
                Dictionary<int, Scope> scopeMap = new Dictionary<int, Scope>();
                Dictionary<int, Type> typeMap = new Dictionary<int, Type>();
                Scope scope = Scope.BuiltIn();
                TypeRegister register = new TypeRegister(scopeMap, typeMap);
                register.Register(program, scope);

                foreach (var pair in scopeMap)
                {
                    Console.WriteLine("{0}: ", pair.Key);
                    Console.WriteLine(pair.Value);
                    Console.WriteLine();
                }
                Console.WriteLine(scope);
                /* pass 5: type checking */
                TypeChecker checker = new TypeChecker(scopeMap, typeMap);
                checker.Check(program, scope);

                // foreach (var pair in typeMap)
                // {
                //     Console.WriteLine(pair);
                // }

                /* pass 6: register symbols */
                Dictionary<int, Scope> scopeMap2 = new Dictionary<int, Scope>();
                Scope scope2 = Scope.BuiltIn();
                LocationRegister locationRegister = new LocationRegister(scopeMap2);
                locationRegister.Register(program, scope2);

                /* pass 7: locate symbols and constants */
                Dictionary<int, Location> locationMap = new Dictionary<int, Location>();
                Dictionary<int, List<Object>> constantPoolMap = new Dictionary<int, List<object>>();

                Locator locator = new Locator(scopeMap2, locationMap, constantPoolMap, typeMap);
                locator.Locate(program, scope2);


                // foreach (var pair in locationMap)
                // {
                //     Console.WriteLine(pair);
                // }

                /* pass 8: compile */
                Compiler compiler = new Compiler(typeMap, locationMap, constantPoolMap);
                var bytecode = compiler.Compile(program);
                //foreach(var b in bytecode)
                //{
                //    Console.WriteLine(b);
                //}

                VirtualMachine.Parser bytecodeParser = new VirtualMachine.Parser(bytecode.ToArray());
                var compiledProgram = bytecodeParser.Parse();
                Func<byte[], String> str = Encoding.UTF8.GetString;
                Console.WriteLine(str(compiledProgram.path));
                VirtualMachine.Function main = null;
                foreach (var module in compiledProgram.modules)
                {
                    Console.WriteLine(str(module.name));
                    for (int i = 0; i < module.env.constantPool.Length; i++)
                    {
                        var item = module.env.constantPool[i];
                        Console.WriteLine("    #{0} = {1} {2}", i, item.tag.ToString().Substring(5), TaggedValueToString(item));
                    }
                    Console.WriteLine();
                    foreach (var function in module.env.functions)
                    {
                        if (str(function.name) == "Main")
                        {
                            main = function;
                        }
                        Console.WriteLine(str(function.name));
                        var disassembler = new Disassembler.Disassembler();
                        var instructions = disassembler.CodeToInstructions(function.code.ToList());
                        disassembler.ShowInstructions(instructions, 4);
                        Console.WriteLine();
                    }
                }
                Console.WriteLine("Press any key to run the program...");
                Console.ReadKey();
                if (main != null)
                {
                    VM vm = new VM(compiledProgram, 100);
                    VM.Run(vm, main);
                }
                else
                {
                    Console.WriteLine("cannot find 'main' function");
                }
            }
            catch (LexerException ex)
            {
                Console.WriteLine("line: {0}, col: {1}, message: {2}", ex.line + 1, ex.column + 1, ex.Message);
            }
            catch (ParserException ex)
            {
                Console.WriteLine("line: {0}, col: {1}, message: {2}", ex.line + 1, ex.column + 1, ex.Message);
            }
            Console.ReadKey();
        }
    }
}