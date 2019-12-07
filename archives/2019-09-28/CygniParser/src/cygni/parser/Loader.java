package cygni.parser;

import cygni.ast.Program;
import cygni.exceptions.ParserException;
import cygni.lexer.Token;
import cygni.ast.Module;
import cygni.exceptions.LexicalException;
import cygni.lexer.Lexer;

import java.util.ArrayList;
import java.util.HashMap;

public class Loader {
    public static Program load(ArrayList<String> files) {
        HashMap<String, Module> modules = new HashMap<>();
        ArrayList<Exception> exceptions = new ArrayList<>();
        for (String file : files) {
            try {
                Lexer lexer = new Lexer(file);
                ArrayList<Token> tokens = lexer.readAll();
                Parser parser = new Parser(file, tokens);
                Module module = parser.module();
                modules.put(file, module);
            } catch (LexicalException ex) {
                exceptions.add(ex);
            } catch (ParserException ex) {
                exceptions.add(ex);
            }
        }
        return new Program(modules, exceptions);
    }
}
