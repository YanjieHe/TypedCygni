package tests;

import cygni.ast.Module;
import cygni.exceptions.ParserException;
import cygni.exceptions.TypeException;
import cygni.lexer.*;
import cygni.parser.*;
import cygni.types.TypeChecker;
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
            Module module = parser.module();
            TypeChecker typeChecker = new TypeChecker();
            Scope scope = new Scope();
            try {
                typeChecker.check(module.nodes.get(0), scope);
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
