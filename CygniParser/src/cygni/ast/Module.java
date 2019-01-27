package cygni.ast;

import java.util.ArrayList;

public class Module extends Node {
    public String path;
    public String moduleName;
    public ArrayList<String> names;
    public ArrayList<Node> nodes;

    public Module(int startLine, int startCol, int endLine, int endCol, String path, String moduleName, ArrayList<String> names, ArrayList<Node> nodes) {
        super(startLine, startCol, endLine, endCol);
        this.path = path;
        this.moduleName = moduleName;
        this.names = names;
        this.nodes = nodes;
    }
}
