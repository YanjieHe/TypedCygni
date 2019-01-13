package cygni.exceptions;

public class ParserException extends Exception {
    public int line;
    public int col;
    public String message;

    public ParserException(int line, int col, String message) {
        this.line = line;
        this.col = col;
        this.message = message;
    }

    @Override
    public String getMessage() {
        return "(" + (line + 1) + ", " + (col + 1) + "): " + message;
    }
}
