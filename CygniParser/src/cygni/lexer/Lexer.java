package cygni.lexer;

import cygni.exceptions.LexicalException;

import java.util.ArrayList;
import java.util.HashMap;

public class Lexer {
    public ArrayList<Token> tokens;
    private String code;
    private int line;
    private int col;
    private int offset;
    private StringBuilder builder;
    private static HashMap<String, Tag> operators;

    static {
        operators = new HashMap<String, Tag>();
        operators.put("+", Tag.Add);
        operators.put("-", Tag.Subtract);
        operators.put("*", Tag.Multiply);
        operators.put("/", Tag.Divide);
        operators.put("%", Tag.Modulo);
        operators.put(">", Tag.GreaterThan);
        operators.put("<", Tag.LessThan);
        operators.put(">=", Tag.GreaterThanOrEqual);
        operators.put("<=", Tag.LessThanOrEqual);
        operators.put("==", Tag.Equal);
        operators.put("!=", Tag.NotEqual);
        operators.put("(", Tag.LeftParenthesis);
        operators.put(")", Tag.RightParenthesis);
        operators.put("[", Tag.LeftBracket);
        operators.put("]", Tag.RightBracket);
        operators.put("{", Tag.LeftBrace);
        operators.put("}", Tag.RightBrace);
        operators.put(":", Tag.Colon);
        operators.put(",", Tag.Comma);
        operators.put(".", Tag.Dot);
        operators.put(";", Tag.Semicolon);
        operators.put("=", Tag.Assign);
        operators.put("->", Tag.RightArrow);
    }

    public Lexer(String code) {
        this.code = code;
        this.line = 0;
        this.col = 0;
        this.offset = 0;
        this.builder = new StringBuilder();
    }

    public ArrayList<Token> readAll() throws LexicalException {
        ArrayList<Token> tokens = new ArrayList<Token>();
        skipWhitespaces();
        while (!eof()) {
            if (Character.isDigit(peek())) {
                tokens.add(readInt());
            } else if (peek() == '"' || peek() == '\'') {
                tokens.add(readString());
            } else if (Character.isLetter(peek()) || peek() == '_') {
                tokens.add(readIdentifier());
            } else if ("+-*/%><=!()[]{}:,.;".indexOf(peek()) >= 0) {
                tokens.add(readOperator());
            } else {
                throw new LexicalException(line, col, "unsupported token");
            }
            skipWhitespaces();
        }
        tokens.add(new Token(line, col, Tag.Eof, "<EOF>"));
        return tokens;
    }

    private Token readInt() throws LexicalException {
        reset();
        while (!eof() && Character.isDigit(peek())) {
            consume();
        }
        if (eof()) {
            return new Token(line, col, Tag.Integer, Integer.parseInt(builder.toString()));
        } else {
            if (peek() == '.') {
                consume();
                return readFloat();
            } else {
                return new Token(line, col, Tag.Integer, Integer.parseInt(builder.toString()));
            }
        }
    }

    private Token readFloat() throws LexicalException {
        while (!eof() && Character.isDigit(peek())) {
            consume();
        }
        if (eof()) {
            return new Token(line, col, Tag.Float, Double.parseDouble(builder.toString()));
        } else {
            if (peek() == 'E' || peek() == 'e') {
                consume();
                if (peek() == '+' || peek() == '-') {
                    consume();
                }
                if (eof() || !Character.isDigit(peek())) {
                    throw new LexicalException(line, col, "float literal");
                } else {
                    while (!eof() && Character.isDigit(peek())) {
                        consume();
                    }
                    return new Token(line, col, Tag.Float, Double.parseDouble(builder.toString()));
                }
            } else {
                return new Token(line, col, Tag.Float, Double.parseDouble(builder.toString()));
            }
        }
    }

    private Token readString() throws LexicalException {
        reset();
        forward();
        while (!eof() && peek() != '"') {
            if (peek() == '\\') {
                forward();
                if (eof()) {
                    throw new LexicalException(line, col, "string literal");
                } else {
                    builder.append(unescpaedChar(peek()));
                    forward();
                }
            } else {
                consume();
            }
        }
        if (eof()) {
            throw new LexicalException(line, col, "string literal");
        } else {
            forward();
            return new Token(line, col, Tag.String, builder.toString());
        }
    }

    private char unescpaedChar(char c) throws LexicalException {
        switch (c) {
            case 'b':
                return '\b';
            case 'n':
                return '\n';
            case 't':
                return '\t';
            case 'r':
                return '\r';
            case 'f':
                return '\f';
            case '\"':
                return '\"';
            case '\'':
                return '\'';
            case '\\':
                return '\\';
            default:
                throw new LexicalException(line, col, "unsupported escaped character");
        }
    }

    private Token readIdentifier() {
        reset();
        consume();
        while (!eof() && isIdentifierChar(peek())) {
            consume();
        }
        return new Token(line, col, Tag.Identifier, builder.toString());
    }

    private Token readOperator() throws LexicalException {
        char c1 = peek();
        String s1 = Character.toString(c1);
        forward();
        if (eof()) {
            if (operators.containsKey(s1)) {
                return new Token(line, col, operators.get(s1), s1);
            } else {
                throw new LexicalException(line, col, "operator literal");
            }
        } else {
            char c2 = peek();
            String s12 = Character.toString(c1) + Character.toString(c2);
            if (operators.containsKey(s12)) {
                forward();
                return new Token(line, col, operators.get(s12), s12);
            } else if (operators.containsKey(s1)) {
                return new Token(line, col, operators.get(s1), s1);
            } else {
                throw new LexicalException(line, col, "operator literal");
            }
        }
    }

    private void skipWhitespaces() {
        while (!eof() && Character.isWhitespace(peek())) {
            forward();
        }
    }

    private boolean isIdentifierChar(char c) {
        return Character.isLetterOrDigit(c) || c == '_';
    }

    private boolean eof() {
        return offset >= code.length();
    }

    private void forward() {
        if (code.charAt(offset) == '\n') {
            offset++;
            line++;
            col = 0;
        } else {
            offset++;
            col++;
        }
    }

    private char peek() {
        return code.charAt(offset);
    }

    private void consume() {
        builder.append(peek());
        forward();
    }

    private void reset() {
        builder.setLength(0);
    }
}