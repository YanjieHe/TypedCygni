package cygni.ast;

import java.util.ArrayList;

public class Block extends Node {
    public ArrayList<Node> nodes;

    public Block(int startLine, int startCol, int endLine, int endCol, ArrayList<Node> nodes) {
        super(startLine, startCol, endLine, endCol);
        this.nodes = nodes;
    }
}
