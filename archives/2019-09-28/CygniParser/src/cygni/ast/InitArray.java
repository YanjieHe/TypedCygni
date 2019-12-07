package cygni.ast;

import java.util.ArrayList;

public class InitArray extends Node {
    public ArrayList<Node> elements;

    public InitArray(int startLine, int startCol, int endLine, int endCol, ArrayList<Node> elements) {
        super(startLine, startCol, endLine, endCol);
        this.elements = elements;
    }
}
