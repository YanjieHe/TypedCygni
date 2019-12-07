package tests;

import cygni.ast.Module;
import cygni.exceptions.ParserException;
import cygni.lexer.*;
import cygni.parser.*;
import cygni.util.*;
import org.junit.Test;

import java.util.ArrayList;

import static java.lang.System.out;
import static org.junit.Assert.*;
import static tests.Util.parseCode;

public class ParserTest {
    @Test
    public void test1() {
        String code = "a + b * 3.2";
        ArrayList<Token> tokens = parseCode(code);
        Parser parser = new Parser(code, tokens);
        try {
            Module module = parser.module();
            assertEquals("{\"kind\": \"Add\", \"left\": {\"kind\": \"Name\", \"name\": \"a\"}, \"right\": {\"kind\": \"Multiply\", \"left\": {\"kind\": \"Name\", \"name\": \"b\"}, \"right\": {\"kind\": \"Constant\", \"value\": \"3.2\"}}}",
                    AstJson.visit(module.nodes.get(0)));
        } catch (ParserException ex) {
            out.println(ex);
            assertTrue(ex.message, false);
        }
    }

    @Test
    public void testIfElse() {
        String code = "if a == 0 { b = 1 } else { b = 2 }";
        ArrayList<Token> tokens = parseCode("if a == 0 { b = 1 } else { b = 2 }");
        Parser parser = new Parser(code, tokens);
        try {
            Module module = parser.module();
            assertEquals("{\"kind\": \"IfElse\", \"condition\": {\"kind\": \"Equal\", \"left\": {\"kind\": \"Name\", \"name\": \"a\"}, \"right\": {\"kind\": \"Constant\", \"value\": \"0\"}}, \"ifTrue\": {\"kind\": \"block\", \"nodes\": [{\"kind\": \"Assign\", \"left\": {\"kind\": \"Name\", \"name\": \"b\"}, \"value\": {\"kind\": \"Constant\", \"value\": \"1\"}}]}, \"ifFalse\": {\"kind\": \"block\", \"nodes\": [{\"kind\": \"Assign\", \"left\": {\"kind\": \"Name\", \"name\": \"b\"}, \"value\": {\"kind\": \"Constant\", \"value\": \"2\"}}]}}",
                    AstJson.visit(module.nodes.get(0)));
        } catch (ParserException ex) {
            out.println("(" + ex.line + ", " + ex.col + "): " + ex.message);
            assertTrue(ex.message, false);
        }
    }

    @Test
    public void testDef() {
        String code = "def factorial(n: Int): Int { if n == 0 { return 1 } else { return n * factorial(n - 1) } }";
        ArrayList<Token> tokens = parseCode(code);
        Parser parser = new Parser(code, tokens);
        try {
            Module module = parser.module();
            out.println(AstJson.visit(module.nodes.get(0)));
        } catch (ParserException ex) {
            out.println("(" + ex.line + ", " + ex.col + "): " + ex.message);
            assertTrue(ex.message, false);
        }
    }
}
