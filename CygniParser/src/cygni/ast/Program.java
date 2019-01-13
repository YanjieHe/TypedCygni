package cygni.ast;

import java.util.ArrayList;

public class Program extends Node {
    public String path;
    public ArrayList<Node> nodes;

    public Program(int startLine, int startCol, int endLine, int endCol, String path, ArrayList<Node> nodes) {
        super(startLine, startCol, endLine, endCol);
        this.nodes = nodes;
        this.path = path;
    }
}
