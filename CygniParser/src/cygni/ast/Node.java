package cygni.ast;

public class Node {
    public int startLine;
    public int startCol;
    public int endLine;
    public int endCol;

    public Node(int startLine, int startCol, int endLine, int endCol) {
        this.startLine = startLine;
        this.startCol = startCol;
        this.endLine = endLine;
        this.endCol = endCol;
    }
}
