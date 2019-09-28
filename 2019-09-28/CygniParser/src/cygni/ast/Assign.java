package cygni.ast;

public class Assign extends Node {
    public Node left;
    public Node value;

    public Assign(int startLine, int startCol, int endLine, int endCol, Node left, Node value) {
        super(startLine, startCol, endLine, endCol);
        this.left = left;
        this.value = value;
    }
}
