package cygni.parser;

import cygni.lexer.Tag;
import cygni.lexer.Token;
import cygni.exceptions.ParserException;
import cygni.ast.*;
import cygni.types.*;

import java.util.ArrayList;
import java.util.HashMap;

public class Parser {
    private ArrayList<Token> tokens;
    private int offset;
    private String path;

    public Parser(String path, ArrayList<Token> tokens) {
        this.tokens = tokens;
        this.offset = 0;
        this.path = path;
    }

    public Program program() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        ArrayList<Node> nodes = new ArrayList<Node>();
        while (!eof()) {
            nodes.add(statement());
        }
        return new Program(startLine, startCol, look().line, look().col, path, nodes);
    }

    private boolean eof() {
        return look().tag == Tag.Eof;
    }

    private Token look() {
        return tokens.get(offset);
    }

    private void move() {
        this.offset++;
    }

    private Token match(Tag tag) throws ParserException {
        if (tag == look().tag) {
            Token t = look();
            move();
            return t;
        } else {
            throw new ParserException(look().line, look().col,
                    "expecting " + tag + ", got " + look().tag);
        }
    }

    private Node statement() throws ParserException {
        if (look().tag == Tag.If) {
            return ifStatement();
        } else if (look().tag == Tag.Var) {
            return var();
        } else if (look().tag == Tag.Def) {
            return def();
        } else if (look().tag == Tag.Return) {
            return parseReturn();
        } else {
            return assign();
        }
    }

    private Node assign() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = or();
        if (x instanceof Name && look().tag == Tag.Assign) {
            Token t = match(Tag.Assign);
            Node y = or();
            return new Assign(startLine, startCol, look().line, look().col, x, y);
        } else {
            return x;
        }
    }

    private Node or() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = and();
        while (look().tag == Tag.Or) {
            Token t = match(Tag.Or);
            Node y = and();
            x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Or, x, y);
        }
        return x;
    }

    private Node and() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = equality();
        while (look().tag == Tag.And) {
            Token t = match(Tag.And);
            Node y = equality();
            x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.And, x, y);
        }
        return x;
    }

    private Node equality() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = relation();
        while (look().tag == Tag.Equal || look().tag == Tag.NotEqual) {
            Token t = look();
            move();
            Node y = relation();
            if (t.tag == Tag.Equal) {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Equal, x, y);
            } else {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.NotEqual, x, y);
            }
        }
        return x;
    }

    private Node relation() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = expr();
        if (look().tag == Tag.GreaterThan
                || look().tag == Tag.LessThan
                || look().tag == Tag.GreaterThanOrEqual
                || look().tag == Tag.LessThanOrEqual) {
            Token t = look();
            move();
            Node y = expr();
            if (t.tag == Tag.GreaterThan) {
                return new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.GreaterThan, x, y);
            } else if (t.tag == Tag.LessThan) {
                return new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.LessThan, x, y);
            } else if (t.tag == Tag.GreaterThanOrEqual) {
                return new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.GreaterThanOrEqual, x, y);
            } else {
                return new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.LessThanOrEqual, x, y);
            }
        } else {
            return x;
        }
    }

    private Node expr() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = term();
        while (look().tag == Tag.Add || look().tag == Tag.Subtract) {
            Token t = look();
            move();
            Node y = term();
            if (t.tag == Tag.Add) {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Add, x, y);
            } else {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Subtract, x, y);
            }
        }
        return x;
    }

    private Node term() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = unary();
        while (look().tag == Tag.Multiply || look().tag == Tag.Divide) {
            Token t = look();
            move();
            Node y = unary();
            if (t.tag == Tag.Multiply) {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Multiply, x, y);
            } else {
                x = new BinaryOp(startLine, startCol, look().line, look().col, BinaryOpKind.Divide, x, y);
            }
        }
        return x;
    }

    private Node unary() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        if (look().tag == Tag.Subtract) {
            move();
            Node x = unary();
            return new UnaryOp(startLine, startCol, look().line, look().col, UnaryOpKind.Negate, x);
        } else if (look().tag == Tag.Not) {
            move();
            Node x = unary();
            return new UnaryOp(startLine, startCol, look().line, look().col, UnaryOpKind.Not, x);
        } else {
            return postfix();
        }
    }

    private Node postfix() throws ParserException {
        Node x = factor();
        while (look().tag == Tag.LeftParenthesis) {
            int startLine = look().line;
            int startCol = look().col;
            ArrayList<Node> arguments = new ArrayList<Node>();
            match(Tag.LeftParenthesis);
            if (look().tag == Tag.RightParenthesis) {
                match(Tag.RightParenthesis);
                x = new Call(startLine, startCol, look().line, look().col, x, arguments);
            } else {
                arguments.add(or());
                while (!eof() && look().tag != Tag.RightParenthesis) {
                    match(Tag.Comma);
                    arguments.add(or());
                }
                match(Tag.RightParenthesis);
                x = new Call(startLine, startCol, look().line, look().col, x, arguments);
            }
        }
        return x;
    }

    private Node factor() throws ParserException {
        if (look().tag == Tag.LeftParenthesis) {
            move();
            Node x = or();
            match(Tag.RightParenthesis);
            return x;
        } else if (look().tag == Tag.Integer || look().tag == Tag.Float) {
            Object v = look().value;
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Constant(startLine, startCol, look().line, look().col, v);
            return x;
        } else if (look().tag == Tag.Identifier) {
            String name = (String) look().value;
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Name(startLine, startCol, look().line, look().col, name);
            return x;
        } else {
            throw new ParserException(look().line, look().col, "factor " + look().tag);
        }
    }

    private Node block() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.LeftBrace);
        ArrayList<Node> nodes = new ArrayList<Node>();
        while (!eof() && look().tag != Tag.RightBrace) {
            nodes.add(statement());
        }
        match(Tag.RightBrace);
        return new Block(startLine, startCol, look().line, look().col, nodes);
    }

    private Node ifStatement() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.If);
        Node condition = or();
        Node ifTrue = block();
        if (look().tag == Tag.Else) {
            match(Tag.Else);
            if (look().tag == Tag.If) {
                Node chunk = ifStatement();
                return new IfElse(startLine, startCol, look().line, look().col, condition, ifTrue, chunk);
            } else {
                Node chunk = block();
                return new IfElse(startLine, startCol, look().line, look().col, condition, ifTrue, chunk);
            }
        } else {
            return new IfThen(startLine, startCol, look().line, look().col, condition, ifTrue);
        }
    }

    private Node var() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.Var);
        Token t = match(Tag.Identifier);
        match(Tag.Assign);
        Node value = or();
        return new Var(startLine, startCol, look().line, look().col, (String) t.value, value);
    }

    private Node def() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.Def);
        String name = (String) match(Tag.Identifier).value;
        match(Tag.LeftParenthesis);
        ArrayList<Parameter> parameters = new ArrayList<Parameter>();
        if (look().tag != Tag.RightParenthesis) {
            parameters.add(parameter());
            while (!eof() && look().tag != Tag.RightParenthesis) {
                match(Tag.Comma);
                parameters.add(parameter());
            }
        }
        match(Tag.RightParenthesis);
        match(Tag.Colon);
        Type returnType = parseType();
        Node body = block();
        return new Def(startLine, startCol, look().line, look().col, name, parameters, returnType, body);
    }

    private Parameter parameter() throws ParserException {
        String name = (String) match(Tag.Identifier).value;
        match(Tag.Colon);
        Type type = parseType();
        return new Parameter(name, type);
    }

    private static HashMap<String, Type> basicTypes;

    static {
        basicTypes = new HashMap<String, Type>();
        basicTypes.put("Int", Type.Int);
        basicTypes.put("Float", Type.Float);
        basicTypes.put("Long", Type.Long);
        basicTypes.put("Double", Type.Double);
        basicTypes.put("Bool", Type.Bool);
        basicTypes.put("Char", Type.Char);
        basicTypes.put("String", Type.String);
        basicTypes.put("Unit", Type.Unit);
    }

    private Type parseType() throws ParserException {
        String name = (String) match(Tag.Identifier).value;
        if (look().tag == Tag.LessThan) {
            match(Tag.LessThan);
            ArrayList<Type> types = new ArrayList<Type>();
            while (!eof() && look().tag != Tag.LessThan) {
                types.add(parseType());
            }
            if (types.size() == 0 || eof()) {
                throw new ParserException(look().line, look().col, "type");
            } else {
                match(Tag.GreaterThan);
                return new Type(name, types);
            }
        } else {
            if (basicTypes.containsKey(name)) {
                return basicTypes.get(name);
            } else {
                return new Type(name);
            }
        }
    }

    private Node parseReturn() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.Return);
        Node value = or();
        return new Return(startLine, startCol, look().line, look().col, value);
    }
}