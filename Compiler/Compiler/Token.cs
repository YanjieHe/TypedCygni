using System;
using System.Collections.Generic;
namespace Compiler
{
    public enum Tag
    {
        Identifier,
        Integer,
        Float,
        String,

        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,

        Equal,
        NotEqual,
        GreaterThan,
        GreaterThanOrEqual,
        LessThan,
        LessThanOrEqual,

        And,
        Or,
        Not,

        Var,
        Def,
        Assign,

        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        LeftParenthesis,
        RightParenthesis,

        Dot,
        Comma,
        Semicolon,
        Colon,
        RightArrow,

        True,
        False,

        If,
        Else,
        While,

        For,
        Break,
        Return,

        Import,
        Uses,

        LowerBound,
        UpperBound,

        Class,
        Private,
        Protected,
        Module,
        Eof
    };

    public class Token
    {
        public Tag tag;
        public int line;
        public int column;
        public String text;

        public Token(int line, int column, Tag tag, String text)
        {
            this.line = line;
            this.column = column;
            this.tag = tag;
            this.text = text;

            if (tag == Tag.Identifier && keywords.ContainsKey(text))
            {
                this.tag = keywords[text];
            }
        }

        private static readonly Dictionary<String, Tag> keywords = new Dictionary<string, Tag>()
        {
            {"if", Tag.If},
                {"else", Tag.Else},
        {"var",       Tag.Var},
        {"def",       Tag.Def},
        {"return",    Tag.Return},
        {"true",      Tag.True},
        {"false",     Tag.False},
        {"while",     Tag.While},
        {"class",     Tag.Class},
        {"private",   Tag.Private},
        {"module",    Tag.Module},
        {"protected", Tag.Protected}
        };

        public override string ToString()
        {
            return String.Format("({0}, {1}, {2})    {3}", line, column, tag, text);
        }
    }
}
