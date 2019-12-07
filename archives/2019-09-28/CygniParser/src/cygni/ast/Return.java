package cygni.ast;

public class Return extends Node {
    public Node value;

    public Return(int startLine, int startCol, int endLine, int endCol, Node value) {
        super(startLine, startCol, endLine, endCol);
        this.value = value;
    }
}
