using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
namespace Compiler
{
    public class LexerException : Exception
    {
        public int line;
        public int column;
        public LexerException(int line, int column, String message) : base(message)
        {
            this.line = line;
            this.column = column;
        }
    }
    public class Lexer
    {
        String code;
        int line;
        int column;
        int offset;
        static Dictionary<String, Tag> operatorTable = new Dictionary<string, Tag>()
        {
             {"+",  Tag.Add},
        {"-",  Tag.Subtract},
        {"*",  Tag.Multiply},
        {"/",  Tag.Divide},
        {"%",  Tag.Modulo},
        {">",  Tag.GreaterThan},
        {"<",  Tag.LessThan},
        {">=", Tag.GreaterThanOrEqual},
        {"<=", Tag.LessThanOrEqual},
        {"==", Tag.Equal},
        {"!=", Tag.NotEqual},
        {"(",  Tag.LeftParenthesis},
        {")",   Tag.RightParenthesis},
        {"[",  Tag.LeftBracket},
        {"]",  Tag.RightBracket},
        {"{",  Tag.LeftBrace},
        {"}",  Tag.RightBrace},
        {":",  Tag.Colon},
        {",",  Tag.Comma},
        {".",  Tag.Dot},
        {";",  Tag.Semicolon},
        {"=",  Tag.Assign},
        {"->", Tag.RightArrow},
        {"<:", Tag.UpperBound},
        {":>", Tag.LowerBound}

        };

        public Lexer(String code)
        {
            this.code = code;
        }

        public List<Token> ReadAll()
        {
            Regex real = new Regex("[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
            String opChars = "+-*/%><=!()[]{}:,.;";
            Regex operators = new Regex("\\+|-|\\*|/|%|>=|<=|>|<|==|!=|=|\\(|\\)|\\[|\\]|\\{|\\}|:|,|\\.|;");
            Regex name = new Regex("[a-zA-Z_][a-zA-Z0-9_]*");
            List<Token> tokens = new List<Token>();
            SkipWhiteSpaces();
            while (offset < code.Length)
            {
                char c = code[offset];
                if (Char.IsDigit(c))
                {
                    Match match = real.Match(code, offset);
                    if (match.Success)
                    {
                        if (int.TryParse(match.Value, out int intResult))
                        {
                            tokens.Add(new Token(line, column, Tag.Integer, match.Value));
                            Move(match.Value);
                        }
                        else
                        {
                            tokens.Add(new Token(line, column, Tag.Float, match.Value));
                            Move(match.Value);
                        }
                    }
                    else
                    {
                        throw new LexerException(line, column, "number");
                    }
                }
                else if (c == '\"')
                {
                    throw new NotImplementedException("read string");
                }
                else if (opChars.IndexOf(c) >= 0)
                {
                    Match match = operators.Match(code, offset);
                    if (match.Success)
                    {
                        tokens.Add(new Token(line, column, operatorTable[match.Value], match.Value));
                        Move(match.Value);
                    }
                    else
                    {
                        throw new LexerException(line, column, "operator");
                    }
                }
                else if (Char.IsLetter(c) || c == '_')
                {
                    Match match = name.Match(code, offset);
                    if (match.Success)
                    {
                        tokens.Add(new Token(line, column, Tag.Identifier, match.Value));
                        Move(match.Value);
                    }
                    else
                    {
                        throw new LexerException(line, column, "identifier");
                    }
                }
                else
                {
                    throw new LexerException(line, column, "unrecognizable character");
                }
                SkipWhiteSpaces();
            }
            tokens.Add(new Token(line, column, Tag.Eof, "<EOF>"));
            return tokens;
        }

        void Move(String text)
        {
            foreach (char c in text)
            {
                if (c == '\n')
                {
                    line++;
                    column = 1;
                }
                else
                {
                    column++;
                }
                offset++;
            }
        }

        void Move()
        {
            char c = code[offset];
            if (c == '\n')
            {
                line++;
                column = 1;
            }
            else
            {
                column++;
            }
            offset++;
        }

        void SkipWhiteSpaces()
        {
            while (offset < code.Length && char.IsWhiteSpace(code[offset]))
            {
                Move();
            }
        }

    };

}
