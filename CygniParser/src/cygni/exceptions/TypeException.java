package cygni.exceptions;

import cygni.interpreter.Interpreter;

public class TypeException extends Exception {
    public int startLine;
    public int startCol;
    public int endLine;
    public int endCol;
    public String message;

    public TypeException(int startLine, int startCol, int endLine, int endCol, String message) {
        this.startLine = startLine;
        this.startCol = startCol;
        this.endLine = endLine;
        this.endCol = endCol;
        this.message = message;
    }

    @Override
    public String getMessage() {
        String[] items = new String[4];
        items[0] = Integer.toString(startLine);
        items[1] = Integer.toString(startCol);
        items[2] = Integer.toString(endLine);
        items[3] = Integer.toString(endCol);
        return "(" + String.join(", ", items) + "): " + message;
    }
}
