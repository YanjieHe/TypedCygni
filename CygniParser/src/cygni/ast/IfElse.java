package cygni.ast;

public class IfElse extends Node {
    public Node condition;
    public Node ifTrue;
    public Node ifFalse;

    public IfElse(int startLine, int startCol, int endLine, int endCol, Node condition, Node ifTrue, Node ifFalse) {
        super(startLine, startCol, endLine, endCol);
        this.condition = condition;
        this.ifTrue = ifTrue;
        this.ifFalse = ifFalse;
    }
}
