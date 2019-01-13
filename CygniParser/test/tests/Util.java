package tests;
import cygni.exceptions.LexicalException;
import cygni.lexer.Lexer;
import cygni.lexer.Token;

import java.util.ArrayList;

import static java.lang.System.out;
import static org.junit.Assert.assertTrue;

public class Util {
    public static ArrayList<Token> parseCode(String code) {
        Lexer lexer = new Lexer(code);
        try {
            return lexer.readAll();
        } catch (LexicalException ex) {
            out.println(ex.message);
            assertTrue(ex.message, false);
            return null;
        }
    }
    public static Token[] listToArray(ArrayList<Token> tokens) {
        Token[] items = new Token[tokens.size()];
        for (int i = 0; i < tokens.size(); i++) {
            items[i] = tokens.get(i);
        }
        return items;
    }
}
