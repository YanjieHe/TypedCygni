package cygni.ast;

public class Name extends Node {
    public String name;

    public Name(int startLine, int startCol, int endLine, int endCol, String name) {
        super(startLine, startCol, endLine, endCol);
        this.name = name;
    }
}
