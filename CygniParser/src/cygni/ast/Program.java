package cygni.ast;

import java.util.ArrayList;
import java.util.HashMap;

public class Program {
    public HashMap<String, Module> modules;
    public ArrayList<Exception> exceptions;

    public Program(HashMap<String, Module> modules, ArrayList<Exception> exceptions) {
        this.modules = modules;
        this.exceptions = exceptions;
    }
}
