package cygni.ast;

public class While extends Node {
    public Node condition;
    public Node body;

    public While(int startLine, int startCol, int endLine, int endCol, Node condition, Node body) {
        super(startLine, startCol, endLine, endCol);
        this.condition = condition;
        this.body = body;
    }
}
