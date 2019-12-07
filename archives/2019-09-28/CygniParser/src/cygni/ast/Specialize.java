package cygni.ast;

import cygni.types.Type;

import java.util.ArrayList;

public class Specialize extends Node {
    public Node expression;
    public ArrayList<Type> arguments;

    public Specialize(int startLine, int startCol, int endLine, int endCol, Node expression, ArrayList<Type> arguments) {
        super(startLine, startCol, endLine, endCol);
        this.expression = expression;
        this.arguments = arguments;
    }
}