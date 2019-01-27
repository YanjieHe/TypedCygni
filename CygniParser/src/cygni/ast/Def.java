package cygni.ast;

import cygni.types.Type;
import cygni.types.UnknownType;

import java.util.ArrayList;

public class Def extends Node {
    public String name;
    public ArrayList<Parameter> parameters;
    public Type returnType;
    public Node body;
    public ArrayList<UnknownType> unknownTypes;
    public ArrayList<Type> exits;

    public Def(int startLine, int startCol, int endLine, int endCol,
               String name, ArrayList<Parameter> parameters, Type returnType, Node body, ArrayList<UnknownType> unknownTypes) {
        super(startLine, startCol, endLine, endCol);
        this.name = name;
        this.parameters = parameters;
        this.returnType = returnType;
        this.body = body;
        this.unknownTypes = unknownTypes;
        this.exits = new ArrayList<>();
    }

    public void addExit(Type exit) {
        this.exits.add(exit);
    }
}
