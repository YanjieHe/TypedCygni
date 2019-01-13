package tests;

import cygni.exceptions.LexicalException;
import cygni.lexer.*;
import org.junit.Test;

import java.util.ArrayList;

import static java.lang.System.out;
import static org.junit.Assert.*;
import static tests.Util.*;

public class LexerTest {

    private Token t(Tag kind, Object value) {
        return new Token(-1, -1, kind, value);
    }

    @Test
    public void test1() {
        ArrayList<Token> tokens = parseCode("a + b * 3.2");
        assertArrayEquals(new Token[]{
                t(Tag.Identifier, "a"),
                t(Tag.Add, "+"),
                t(Tag.Identifier, "b"),
                t(Tag.Multiply, "*"),
                t(Tag.Float, 3.2),
                t(Tag.Eof, "<EOF>")
        }, listToArray(tokens));
    }

    @Test
    public void testString() {
        ArrayList<Token> tokens = parseCode("a = \"Some text\\n\"");
        assertArrayEquals(new Token[]{
                t(Tag.Identifier, "a"),
                t(Tag.Assign, "="),
                t(Tag.String, "Some text\n"),
                t(Tag.Eof, "<EOF>")
        }, listToArray(tokens));
    }

    @Test
    public void testIfThen() {
        ArrayList<Token> tokens = parseCode("if a == 0 { b = 1 }");
        assertArrayEquals(new Token[]{
                t(Tag.Identifier, "if"),
                t(Tag.Identifier, "a"),
                t(Tag.Equal, "=="),
                t(Tag.Integer, 0),
                t(Tag.LeftBrace, "{"),
                t(Tag.Identifier, "b"),
                t(Tag.Assign, "="),
                t(Tag.Integer, 1),
                t(Tag.RightBrace, "}"),
                t(Tag.Eof, "<EOF>")
        }, listToArray(tokens));
    }
    @Test
    public void testIfElse() {
        ArrayList<Token> tokens = parseCode("if a == 0 { b = 1 } else { b = 2 }");
        assertArrayEquals(new Token[]{
                t(Tag.Identifier, "if"),
                t(Tag.Identifier, "a"),
                t(Tag.Equal, "=="),
                t(Tag.Integer, 0),
                t(Tag.LeftBrace, "{"),
                t(Tag.Identifier, "b"),
                t(Tag.Assign, "="),
                t(Tag.Integer, 1),
                t(Tag.RightBrace, "}"),
                t(Tag.Identifier, "else"),
                t(Tag.LeftBrace, "{"),
                t(Tag.Identifier, "b"),
                t(Tag.Assign, "="),
                t(Tag.Integer, 2),
                t(Tag.RightBrace, "}"),
                t(Tag.Eof, "<EOF>")
        }, listToArray(tokens));
    }
}
