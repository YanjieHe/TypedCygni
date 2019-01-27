package cygni.parser;

import cygni.ast.Module;
import cygni.lexer.Tag;
import cygni.lexer.Token;
import cygni.exceptions.ParserException;
import cygni.ast.*;
import cygni.types.*;

import java.util.ArrayList;

public class Parser {
    private ArrayList<Token> tokens;
    private int offset;
    private String path;

    public Parser(String path, ArrayList<Token> tokens) {
        this.tokens = tokens;
        this.offset = 0;
        this.path = path;
    }

    public Module module() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        String moduleName;
        ArrayList<String> names = new ArrayList<>();
        if (look().tag == Tag.Module) {
            match(Tag.Module);
            moduleName = (String) match(Tag.Identifier).value;
            match(Tag.LeftParenthesis);
            while (!eof() && look().tag != Tag.RightParenthesis) {
                names.add((String) match(Tag.Identifier).value);
            }
            match(Tag.RightParenthesis);
        } else {
            moduleName = "(default)";
        }
        ArrayList<Node> nodes = new ArrayList<Node>();
        while (!eof()) {
            nodes.add(statement());
        }
        return new Module(startLine, startCol, look().line, look().col, path, moduleName, names, nodes);
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
        } else if (look().tag == Tag.While) {
            return parseWhile();
        } else {
            return assign();
        }
    }

    private Node assign() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        Node x = or();
        if (look().tag == Tag.Assign) {
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
        while (look().tag == Tag.LeftParenthesis || look().tag == Tag.LeftBracket) {
            int startLine = look().line;
            int startCol = look().col;
            if (look().tag == Tag.LeftParenthesis) {
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
            } else if (look().tag == Tag.LeftBracket) {
                ArrayList<Type> types = parseTypeArguments();
                x = new Specialize(startLine, startCol, look().line, look().col, x, types);
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
        } else if (look().tag == Tag.Integer || look().tag == Tag.Float || look().tag == Tag.String) {
            Object v = look().value;
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Constant(startLine, startCol, look().line, look().col, v);
            return x;
        } else if (look().tag == Tag.True) {
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Constant(startLine, startCol, look().line, look().col, true);
            return x;
        } else if (look().tag == Tag.False) {
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Constant(startLine, startCol, look().line, look().col, false);
            return x;
        } else if (look().tag == Tag.Identifier) {
            String name = (String) look().value;
            int startLine = look().line;
            int startCol = look().col;
            move();
            Node x = new Name(startLine, startCol, look().line, look().col, name);
            return x;
        } else if (look().tag == Tag.LeftBracket) {
            return initArray();
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
        ArrayList<UnknownType> unknownTypes = new ArrayList<UnknownType>();
        if (look().tag == Tag.LeftBracket) {
            unknownTypes = parseGenericTypesDef();
        }
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
        return new Def(startLine, startCol, look().line, look().col, name, parameters, returnType, body, unknownTypes);
    }

    private Parameter parameter() throws ParserException {
        String name = (String) match(Tag.Identifier).value;
        match(Tag.Colon);
        Type type = parseType();
        return new Parameter(name, type);
    }


    private Type parseType() throws ParserException {
        String name = (String) match(Tag.Identifier).value;
        if (look().tag == Tag.LeftBracket) {
            ArrayList<Type> types = parseTypeArguments();
            Type result = Type.makeType(name, types);
            if (result == null) {
                throw new ParserException(look().line, look().col, "type error");
            } else {
                return result;
            }
        } else {
            return Type.makePrimitiveType(name);
        }
    }

    private Node parseReturn() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.Return);
        Node value = or();
        return new Return(startLine, startCol, look().line, look().col, value);
    }

    private ArrayList<Type> parseTypeArguments() throws ParserException {
        match(Tag.LeftBracket);
        ArrayList<Type> types = new ArrayList<Type>();
        types.add(parseType());
        while (!eof() && look().tag != Tag.RightBracket) {
            match(Tag.Comma);
            types.add(parseType());
        }
        match(Tag.RightBracket);
        return types;
    }

    private Node initArray() throws ParserException {
        int startLine = look().line;
        int startCol = look().col;
        match(Tag.LeftBracket);
        ArrayList<Node> elements = new ArrayList<Node>();
        if (look().tag == Tag.RightBracket) {
            return new InitArray(startLine, startCol, look().line, look().col, elements);
        } else {
            elements.add(or());
            while (!eof() && look().tag != Tag.RightBracket) {
                match(Tag.Comma);
                elements.add(or());
            }
            match(Tag.RightBracket);
            return new InitArray(startLine, startCol, look().line, look().col, elements);
        }
    }

    private ArrayList<UnknownType> parseGenericTypesDef() throws ParserException {
        match(Tag.LeftBracket);
        ArrayList<UnknownType> unknownTypes = new ArrayList<UnknownType>();
        unknownTypes.add(parseOneGenericTypeDef());
        while (!eof() && look().tag != Tag.RightBracket) {
            match(Tag.Comma);
            unknownTypes.add(parseOneGenericTypeDef());
        }
        match(Tag.RightBracket);
        return unknownTypes;
    }

    private UnknownType parseOneGenericTypeDef() throws ParserException {
        String name = (String) match(Tag.Identifier).value;
        return new UnknownType(name);
    }

    private While parseWhile() throws ParserException {
        Token token = match(Tag.While);
        Node condition = or();
        Node body = block();
        return new While(token.line, token.col, look().line, look().col, condition, body);
    }

    private DefClass defClass() throws ParserException {
        match(Tag.Class);
        String name = (String) match(Tag.Identifier).value;
        ArrayList<UnknownType> unknownTypes;
        if (look().tag == Tag.LeftBracket) {
            unknownTypes = parseGenericTypesDef();
        } else {
            unknownTypes = new ArrayList<>();
        }
        ArrayList<DefClass.Field> fields = new ArrayList<>();
        ArrayList<DefClass.Method> methods = new ArrayList<>();
        match(Tag.LeftBrace);
        while (!eof() && look().tag != Tag.RightBrace) {
            DefClass.Access access;
            if (look().tag == Tag.Private) {
                access = DefClass.Access.Private;
            } else if (look().tag == Tag.Protected) {
                access = DefClass.Access.Protected;
            } else {
                access = DefClass.Access.Public;
            }

            if (look().tag == Tag.Var) {
                match(Tag.Var);
                String varName = (String) match(Tag.Identifier).value;
                match(Tag.Colon);
                Type
            }
        }
    }
}