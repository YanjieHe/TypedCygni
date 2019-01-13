package cygni.ast;

public class BinaryOp extends Node {
    public BinaryOpKind kind;
    public Node left;
    public Node right;

    public BinaryOp(int startLine, int startCol, int endLine, int endCol, BinaryOpKind kind, Node left, Node right) {
        super(startLine, startCol, endLine, endCol);
        this.kind = kind;
        this.left = left;
        this.right = right;
    }
}
