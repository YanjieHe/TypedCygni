﻿using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
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

                // Console.WriteLine(new JsonViewer().Visit(json));

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

                foreach(var pair in typeMap)
                {
                    Console.WriteLine(pair);
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
