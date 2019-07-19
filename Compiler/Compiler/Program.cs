using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using VM = VirtualMachine.VM;
namespace Compiler
{
    class MainClass
    {
        public static void Main(string[] args)
        {
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
                    // Console.WriteLine(token);
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
                Scope scope = Scope.BuiltIn();
                TypeRegister register = new TypeRegister(scopeMap);
                register.Register(program, scope);

                foreach (var pair in scopeMap)
                {
                    Console.WriteLine("{0}: ", pair.Key);
                    Console.WriteLine(pair.Value);
                    Console.WriteLine();
                }

                /* pass 5: type checking */
                Dictionary<int, Type> typeMap = new Dictionary<int, Type>();
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

                Locator locator = new Locator(scopeMap2, locationMap, constantPoolMap);
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

                VM vm = new VM(100);
                VirtualMachine.Parser bytecodeParser = new VirtualMachine.Parser(bytecode.ToArray());
                var compiledProgram = bytecodeParser.Parse();
                Func<byte[], String> str = bytes => Encoding.UTF8.GetString(bytes);
                Console.WriteLine(str(compiledProgram.path));
                VirtualMachine.Function main = null;
                foreach (var module in compiledProgram.modules)
                {
                    Console.WriteLine(str(module.name));
                    foreach (var function in module.env.functions)
                    {
                        if (str(function.name) == "Main")
                        {
                            main = function;
                        }
                        Console.WriteLine(str(function.name));
                    }
                }
                if (main != null)
                {
                    VM.Run(vm, main);
                }
                else
                {
                    Console.WriteLine("cannot find 'main' function");
                }
            }
            catch (LexerException ex)
            {
                Console.WriteLine("line: {0}, col: {1}, message: {2}", ex.line, ex.column, ex.Message);
            }
            catch (ParserException ex)
            {
                Console.WriteLine("line: {0}, col: {1}, message: {2}", ex.line, ex.column, ex.Message);
            }
        }
    }
}
