package cygni.ast;

public class UnaryOp extends Node {
    public UnaryOpKind kind;
    public Node operand;

    public UnaryOp(int startLine, int startCol, int endLine, int endCol, UnaryOpKind kind, Node operand) {
        super(startLine, startCol, endLine, endCol);
        this.kind = kind;
        this.operand = operand;
    }
}
