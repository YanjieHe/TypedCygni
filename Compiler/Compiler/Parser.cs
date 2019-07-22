using System;
using System.Collections.Generic;
namespace Compiler
{
    public class ParserException : Exception
    {
        public int line;
        public int column;
        public ParserException(int line, int column, String message) : base(message)
        {
            this.line = line;
            this.column = column;
        }
    }
    public class Parser
    {
        private List<Token> tokens;
        private String path;
        private int offset;
        public Parser(String path, List<Token> tokens)
        {
            this.path = path;
            this.tokens = tokens;
        }

        bool IsEof()
        {
            return Look().tag == Tag.Eof;
        }

        Token Look()
        {
            return tokens[offset];
        }

        void Move()
        {
            offset++;
        }

        Token Match(Tag tag)
        {
            if (tag == Look().tag)
            {
                Token t = Look();
                Move();
                return t;
            }
            else
            {
                String message = String.Format("expecting '{0}', got '{1}'", tag, Look().tag);
                throw new ParserException(Look().line, Look().column, message);
            }
        }

        Position GetPos(Token token)
        {
            return new Position(token.line, token.column, Look().line, Look().column);
        }

        public Program ParseProgram()
        {
            List<DefClass> classes = new List<DefClass>();
            List<DefModule> modules = new List<DefModule>();
            while (!IsEof())
            {
                if (Look().tag == Tag.Class)
                {
                    classes.Add(ParseDefClass());
                }
                else
                {
                    modules.Add(ParseDefModule());
                }
            }
            return new Program(path, classes, modules);
        }
        Ast Statement()
        {
            switch (Look().tag)
            {
                case Tag.If:
                    return IfStatement();
                case Tag.Var:
                    return ParseVar();
                case Tag.Def:
                    return ParseDef();
                case Tag.Return:
                    return ParseReturn();
                case Tag.While:
                    return ParseWhile();
                default:
                    return ParseAssign();
            }
        }

        Ast ParseAssign()
        {
            Token start = Look();
            Ast x = ParseOr();
            if (Look().tag == Tag.Assign)
            {
                Token t = Match(Tag.Assign);
                Ast y = ParseOr();
                return new Binary(GetPos(start), Kind.Assign, x, y);
            }
            else
            {
                return x;
            }
        }

        Ast ParseOr()
        {
            Token start = Look();
            Ast x = ParseAnd();
            while (Look().tag == Tag.Or)
            {
                Token t = Match(Tag.Or);
                Ast y = ParseAnd();
                x = new Binary(GetPos(start), Kind.Or, x, y);
            }
            return x;
        }

        Ast ParseAnd()
        {
            Token start = Look();
            Ast x = ParseEquality();
            while (Look().tag == Tag.And)
            {
                Token t = Match(Tag.And);
                Ast y = ParseEquality();
                x = new Binary(GetPos(start), Kind.And, x, y);
            }
            return x;
        }

        Ast ParseEquality()
        {
            Token start = Look();
            Ast x = ParseRelation();
            while (Look().tag == Tag.Equal || Look().tag == Tag.NotEqual)
            {
                Token t = Look();
                Move();
                Ast y = ParseRelation();
                if (t.tag == Tag.Equal)
                {
                    x = new Binary(GetPos(start), Kind.Equal, x, y);
                }
                else
                {
                    x = new Binary(GetPos(start), Kind.NotEqual, x, y);
                }
            }
            return x;
        }

        Ast ParseRelation()
        {
            Token start = Look();
            Ast x = ParseExpr();
            if (Look().tag == Tag.GreaterThan || Look().tag == Tag.LessThan ||
                Look().tag == Tag.GreaterThanOrEqual ||
                Look().tag == Tag.LessThanOrEqual)
            {
                Token t = Look();
                Move();
                Ast y = ParseExpr();
                if (t.tag == Tag.GreaterThan)
                {
                    return new Binary(GetPos(start), Kind.GreaterThan, x, y);
                }
                else if (t.tag == Tag.LessThan)
                {
                    return new Binary(GetPos(start), Kind.LessThan, x, y);
                }
                else if (t.tag == Tag.GreaterThanOrEqual)
                {
                    return new Binary(GetPos(start), Kind.GreaterThanOrEqual, x, y);
                }
                else
                {
                    return new Binary(GetPos(start), Kind.LessThanOrEqual, x, y);
                }
            }
            else
            {
                return x;
            }
        }

        Ast ParseExpr()
        {
            Token start = Look();
            Ast x = ParseTerm();
            while (Look().tag == Tag.Add || Look().tag == Tag.Subtract)
            {
                Token t = Look();
                Move();
                Ast y = ParseTerm();
                if (t.tag == Tag.Add)
                {
                    x = new Binary(GetPos(start), Kind.Add, x, y);
                }
                else
                {
                    x = new Binary(GetPos(start), Kind.Subtract, x, y);
                }
            }
            return x;
        }

        Ast ParseTerm()
        {
            Token start = Look();
            Ast x = ParseUnary();
            while (Look().tag == Tag.Multiply || Look().tag == Tag.Divide)
            {
                Token t = Look();
                Move();
                Ast y = ParseUnary();
                if (t.tag == Tag.Multiply)
                {
                    x = new Binary(GetPos(start), Kind.Multiply, x, y);
                }
                else
                {
                    x = new Binary(GetPos(start), Kind.Divide, x, y);
                }
            }
            return x;
        }

        Ast ParseUnary()
        {
            Token start = Look();
            if (Look().tag == Tag.Add)
            {
                Move();
                Ast x = ParseUnary();
                return new Unary(GetPos(start), Kind.UnaryPlus, x);
            }
            else if (Look().tag == Tag.Subtract)
            {
                Move();
                Ast x = ParseUnary();
                return new Unary(GetPos(start), Kind.UnaryMinus, x);
            }
            else if (Look().tag == Tag.Not)
            {
                Move();
                Ast x = ParseUnary();
                return new Unary(GetPos(start), Kind.Not, x);
            }
            else
            {
                return ParsePostfix();
            }
        }

        Ast ParsePostfix()
        {
            Ast x = ParseFactor();
            while (Look().tag == Tag.LeftParenthesis || Look().tag == Tag.LeftBracket || Look().tag == Tag.Dot)
            {
                Token start = Look();

                if (Look().tag == Tag.LeftParenthesis)
                {
                    x = ParseCall(x);
                }
                else if (Look().tag == Tag.LeftBracket)
                {
                    // TO DO
                    throw new PlatformNotSupportedException();
                }
                else if (Look().tag == Tag.Dot)
                {
                    x = ParseDot(x);
                }
            }
            return x;
        }

        Ast ParseFactor()
        {
            if (Look().tag == Tag.LeftParenthesis)
            {
                Move();
                Ast x = ParseOr();
                Match(Tag.RightParenthesis);
                return x;
            }
            else if (Look().tag == Tag.Integer)
            {
                String v = Look().text;
                Token start = Look();
                Move();
                return new Constant(GetPos(start), int.Parse(v));
            }
            else if (Look().tag == Tag.Float)
            {
                String v = Look().text;
                Token start = Look();
                Move();
                return new Constant(GetPos(start), double.Parse(v));
            }
            else if (Look().tag == Tag.String)
            {
                String v = Look().text;
                Token start = Look();
                Move();
                return new Constant(GetPos(start), v);
            }
            else if (Look().tag == Tag.True)
            {
                Token start = Look();
                Move();
                Ast x = new Constant(GetPos(start),
                                          true);
                return x;
            }
            else if (Look().tag == Tag.False)
            {
                Token start = Look();
                Move();
                Ast x = new Constant(GetPos(start),
                                         false);
                return x;
            }
            else if (Look().tag == Tag.Identifier)
            {
                String name = Look().text;
                Token start = Look();
                Move();
                Ast x = new Name(GetPos(start), name);
                return x;
            }
            else
            {
                throw new ParserException(Look().line, Look().column, "factor");
            }
        }

        Ast ParseBlock()
        {
            Token start = Look();
            Match(Tag.LeftBrace);
            List<Ast> expressions = new List<Ast>();
            while (!IsEof() && Look().tag != Tag.RightBrace)
            {
                expressions.Add(Statement());
            }
            Match(Tag.RightBrace);
            return new Block(GetPos(start), expressions);
        }

        Ast IfStatement()
        {
            Token start = Look();
            Match(Tag.If);
            Ast condition = ParseOr();
            Ast ifTrue = ParseBlock();
            if (Look().tag == Tag.Else)
            {
                Match(Tag.Else);
                if (Look().tag == Tag.If)
                {
                    Ast chunk = IfStatement();
                    return new IfElse(GetPos(start), condition, ifTrue, chunk);
                }
                else
                {
                    Ast chunk = ParseBlock();
                    return new IfElse(GetPos(start), condition, ifTrue, chunk);
                }
            }
            else
            {
                return new IfThen(GetPos(start), condition, ifTrue);
            }
        }

        Var ParseVar()
        {
            Token start = Look();
            Match(Tag.Var);
            Token t = Match(Tag.Identifier);
            if (Look().tag == Tag.Colon)
            {
                Match(Tag.Colon);
                var type = ParseType();
                if (Look().tag == Tag.Assign)
                {
                    Match(Tag.Assign);
                    var value = ParseOr();
                    return new Var(GetPos(start), t.text, type, value);
                }
                else
                {
                    return new Var(GetPos(start), t.text, type, null);
                }
            }
            else
            {
                Match(Tag.Assign);
                var value = ParseOr();
                return new Var(GetPos(start), t.text, null, value);
            }
        }

        Var ParseVarDeclaration()
        {
            Token start = Look();
            Match(Tag.Var);
            Token t = Match(Tag.Identifier);
            Match(Tag.Colon);
            var type = ParseType();
            return new Var(GetPos(start), t.text, type, null);
        }

        Def ParseDef()
        {
            Token start = Look();
            Match(Tag.Def);
            String name = Match(Tag.Identifier).text;

            Match(Tag.LeftParenthesis);
            List<Parameter> parameters = new List<Parameter>();
            if (Look().tag != Tag.RightParenthesis)
            {
                parameters.Add(ParseParameter());
                while (!IsEof() && Look().tag != Tag.RightParenthesis)
                {
                    Match(Tag.Comma);
                    parameters.Add(ParseParameter());
                }
            }
            Match(Tag.RightParenthesis);
            Match(Tag.Colon);
            var returnType = ParseType();
            Ast body = ParseBlock();
            return new Def(GetPos(start), name, parameters, Def.MakeFunctionType(GetPos(start), parameters, returnType), body);
        }

        Parameter ParseParameter()
        {
            String name = Match(Tag.Identifier).text;
            Match(Tag.Colon);
            var type = ParseType();
            return new Parameter(name, type);
        }

        TypeSpecifier ParseType()
        {
            String name = Match(Tag.Identifier).text;
            if (Look().tag == Tag.LeftBracket)
            {
                Match(Tag.LeftBracket);
                Token token = Look();
                List<TypeSpecifier> types = ParseTypeArguments();
                var result = new TypeSpecifier(GetPos(token), name, types);
                if (types.Count == 0)
                {
                    throw new ParserException(Look().line, Look().column, "type error");
                }
                else
                {
                    return result;
                }
            }
            else
            {
                return new TypeSpecifier(GetPos(Look()), name);
            }
        }

        Ast ParseReturn()
        {
            Token start = Look();
            Match(Tag.Return);
            Ast value = ParseOr();
            return new Return(GetPos(start), value);
        }

        List<TypeSpecifier> ParseTypeArguments()
        {
            Match(Tag.LeftBracket);
            List<TypeSpecifier> types = new List<TypeSpecifier>();
            types.Add(ParseType());
            while (!IsEof() && Look().tag != Tag.RightBracket)
            {
                Match(Tag.Comma);
                types.Add(ParseType());
            }
            Match(Tag.RightBracket);
            return types;
        }

        While ParseWhile()
        {
            Token start = Look();
            Token token = Match(Tag.While);
            Ast condition = ParseOr();
            Ast body = ParseBlock();
            return new While(GetPos(start), condition, body);
        }

        DefClass ParseDefClass()
        {
            Token start = Look();
            Match(Tag.Class);
            String name = Match(Tag.Identifier).text;
            List<Var> fields = new List<Var>();
            List<Def> methods = new List<Def>();
            Match(Tag.LeftBrace);
            while (!IsEof() && Look().tag != Tag.RightBrace)
            {
                Access access = ParseAccess();

                if (Look().tag == Tag.Var)
                {
                    // ParseVar field: Type
                    var field = ParseVarDeclaration();
                    field.access = access;
                    fields.Add(field);
                }
                else if (Look().tag == Tag.Def)
                {
                    // def method(args..) { }
                    var method = ParseDef();
                    method.access = access;
                    methods.Add(method);
                }
                else
                {
                    throw new ParserException(Look().line, Look().column, "unexpected token");
                }
            }
            Match(Tag.RightBrace);
            return new DefClass(GetPos(start), name, fields, methods);
        }

        DefModule ParseDefModule()
        {
            Token start = Look();
            Match(Tag.Module);
            String name = Match(Tag.Identifier).text;
            List<Var> fields = new List<Var>();
            List<Def> methods = new List<Def>();
            Match(Tag.LeftBrace);
            while (!IsEof() && Look().tag != Tag.RightBrace)
            {
                Access access = ParseAccess();
                if (Look().tag == Tag.Var)
                {
                    // ParseVar field: Type
                    var field = ParseVarDeclaration();
                    field.access = access;
                    fields.Add(field);
                }
                else if (Look().tag == Tag.Def)
                {
                    // def method(args..) { }
                    var method = ParseDef();
                    method.access = access;
                    methods.Add(method);
                }
                else
                {
                    throw new ParserException(Look().line, Look().column, "unexpected token");
                }
            }
            Match(Tag.RightBrace);
            return new DefModule(GetPos(start), name, fields, methods);
        }

        Access ParseAccess()
        {
            if (Look().tag == Tag.Private)
            {
                Match(Tag.Private);
                return Access.Private;
            }
            else if (Look().tag == Tag.Protected)
            {
                Match(Tag.Protected);
                return Access.Protected;
            }
            else
            {
                return Access.Public;
            }
        }

        Ast ParseCall(Ast function)
        {
            Token start = Look();
            Match(Tag.LeftParenthesis);
            List<Ast> arguments = new List<Ast>();
            if (Look().tag == Tag.RightParenthesis)
            {
                Match(Tag.RightParenthesis);
                return new Call(GetPos(start), function, arguments);
            }
            else
            {
                arguments.Add(ParseOr());
                while (!IsEof() && Look().tag != Tag.RightParenthesis)
                {
                    Match(Tag.Comma);
                    arguments.Add(ParseOr());
                }
                Match(Tag.RightParenthesis);
                return new Call(GetPos(start), function, arguments);
            }
        }

        Ast ParseDot(Ast expression)
        {
            Token start = Look();
            Match(Tag.Dot);
            String field = Match(Tag.Identifier).text;
            var member = new MemberInfo(field);
            return new MemberAccess(GetPos(start), expression, member);
        }
    }
}
