package cygni.ast;

public class IfThen extends Node {
    public Node condition;
    public Node ifTrue;

    public IfThen(int startLine, int startCol, int endLine, int endCol, Node condition, Node ifTrue) {
        super(startLine, startCol, endLine, endCol);
        this.condition = condition;
        this.ifTrue = ifTrue;
    }
}
