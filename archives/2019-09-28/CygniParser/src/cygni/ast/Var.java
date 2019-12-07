package cygni.ast;

public class Var extends Node {
    public String name;
    public Node value;

    public Var(int startLine, int startCol, int endLine, int endCol, String name, Node value) {
        super(startLine, startCol, endLine, endCol);
        this.name = name;
        this.value = value;
    }
}
