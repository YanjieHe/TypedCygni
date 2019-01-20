package cygni;

import cygni.ast.Program;
import cygni.exceptions.LexicalException;
import cygni.exceptions.ParserException;
import cygni.exceptions.TypeException;
import cygni.lexer.*;
import cygni.interpreter.*;
import cygni.parser.Parser;
import cygni.ast.*;
import cygni.types.*;
import cygni.util.AstJson;
import cygni.util.JsonUtil;

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

    public static Type makeGenericArrayType() {
        return new ArrayType(new UnknownType("A"));
    }

    public static String objectToString(Object object) {
        if (object instanceof Object[]) {
            Object[] objects = (Object[]) object;
            String[] items = new String[objects.length];
            for (int i = 0; i < items.length; i++) {
                items[i] = objectToString(objects[i]);
            }
            return "[" + String.join(", ", items) + "]";
        } else {
            return object.toString();
        }
    }

    public static void main(String[] args) {
        try {
            String fileName = "C:\\Users\\HeYan\\Documents\\MyCode\\Cygni\\factorial.txt";
            Lexer lexer = new Lexer(readFile(fileName));
            ArrayList<Token> tokens = lexer.readAll();
            Parser parser = new Parser(fileName, tokens);
            Program program = parser.program();
            TypeChecker checker = new TypeChecker();

            Scope scope = new Scope();
            scope.putType("Array", makeGenericArrayType());
            scope.putValue("Array", makeGenericArrayType());

            checker.checkProgram(program, scope);
            Interpreter interpreter = new Interpreter();
            Object result = null;
            out.println(AstJson.toString(program));
            for (Node node : program.nodes) {
                result = interpreter.eval(node, scope);
            }
            out.println(objectToString(result));
        } catch (LexicalException ex) {
            out.print("lexical error: ");
            out.println(ex.getMessage());
        } catch (ParserException ex) {
            out.print("syntax error: ");
            out.println(ex.getMessage());
        } catch (TypeException ex) {
            out.print("type error: ");
            out.println(ex.getMessage());
        } catch (Exception ex) {
            out.println(ex.getMessage());
        }
    }
}