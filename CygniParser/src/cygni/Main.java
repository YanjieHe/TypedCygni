package cygni;

import cygni.ast.Program;
import cygni.exceptions.LexicalException;
import cygni.lexer.*;
import cygni.interpreter.*;
import cygni.parser.Parser;
import cygni.ast.*;
import cygni.util.AstJson;

import java.io.*;
import java.util.ArrayList;

import static java.lang.System.out;

public class Main {

    public static String readFile(String fileName) {
        try {
            File file = new File(fileName);
            FileReader fr = new FileReader(file);
            BufferedReader br = new BufferedReader(fr);
            StringBuilder builder = new StringBuilder();
            String line;
            while ((line = br.readLine()) != null) {
                builder.append(line);
            }
            return builder.toString();
        } catch (IOException ex) {
            ex.printStackTrace();
            return "";
        }
    }

    public static void main(String[] args) {
        try {
            String fileName = "C:\\Users\\HeYan\\Documents\\MyCode\\Cygni\\test.txt";
            Lexer lexer = new Lexer(readFile(fileName));
            ArrayList<Token> tokens = lexer.readAll();
            Parser parser = new Parser(fileName, tokens);
            Program program = parser.program();
            Interpreter interpreter = new Interpreter();
            Object result = null;
            Scope scope = new Scope();
            for (Node node : program.nodes) {
//                out.println(AstJson.visit(node));
                result = interpreter.eval(node, scope);
            }
            out.println(result);
        } catch (Exception ex) {
            out.println(ex.getMessage());
        }
    }
}