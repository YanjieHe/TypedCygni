package cygni.ast;

import cygni.types.Type;

import java.util.ArrayList;

public class Def extends Node {
    public String name;
    public ArrayList<Parameter> parameters;
    public Type returnType;
    public Node body;

    public Def(int startLine, int startCol, int endLine, int endCol, String name, ArrayList<Parameter> parameters, Type returnType, Node body) {
        super(startLine, startCol, endLine, endCol);
        this.name = name;
        this.parameters = parameters;
        this.returnType = returnType;
        this.body = body;
    }
}
