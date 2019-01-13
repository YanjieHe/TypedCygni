package cygni.ast;

import java.util.ArrayList;

public class Call extends Node {
    public Node function;
    public ArrayList<Node> arguments;

    public Call(int startLine, int startCol, int endLine, int endCol, Node function, ArrayList<Node> arguments) {
        super(startLine, startCol, endLine, endCol);
        this.function = function;
        this.arguments = arguments;
    }
}
