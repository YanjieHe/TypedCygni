package cygni.ast;

public class Constant extends Node {
    public Object value;

    public Constant(int startLine, int startCol, int endLine, int endCol, Object value) {
        super(startLine, startCol, endLine, endCol);
        this.value = value;
    }
}
