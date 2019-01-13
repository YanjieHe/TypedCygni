package tests;

import cygni.exceptions.ParserException;
import cygni.exceptions.TypeException;
import cygni.lexer.*;
import cygni.parser.*;
import cygni.ast.*;
import cygni.types.TypeChecker;
import cygni.util.*;
import cygni.Scope;
import org.junit.Test;

import java.util.ArrayList;

import static java.lang.System.out;
import static org.junit.Assert.*;
import static tests.Util.parseCode;

public class TypeCheckerTest {

    @Test
    public void testFactorial() {
        String code = "def factorial(n: Int): Int { if n == 0 { 1 } else { n * factorial(n - 1) } }";
        ArrayList<Token> tokens = parseCode(code);
        Parser parser = new Parser(code, tokens);
        try {
            Program program = parser.program();
            TypeChecker typeChecker = new TypeChecker();
            Scope scope = new Scope();
            try {
                typeChecker.check(program.nodes.get(0), scope);
            } catch (TypeException ex) {
                out.println("(" + ex.startLine + ", " + ex.startCol + ", "
                        + ex.endLine + ", " + ex.endCol + "): " + ex.message);
                assertTrue(ex.message, false);
            }

        } catch (ParserException ex) {
            out.println("(" + ex.line + ", " + ex.col + "): " + ex.message);
            assertTrue(ex.message, false);
        }
    }
}
