package cygni.interpreter;

import cygni.ast.*;
import cygni.Scope;

import java.util.ArrayList;

public class Function {
    public String name;
    public ArrayList<Parameter> parameters;
    public Scope parent;
    public Node body;

    public Function(String name, ArrayList<Parameter> parameters, Scope parent, Node body) {
        this.name = name;
        this.parameters = parameters;
        this.parent = parent;
        this.body = body;
    }

    public Object invoke(Object[] arguments) {
        Scope newScope = new Scope(parent);
        for (int i = 0; i < parameters.size(); i++) {
            newScope.putValue(parameters.get(i).name, arguments[i]);
        }
        Interpreter interpreter = new Interpreter();
        return interpreter.eval(body, newScope);
    }
}
