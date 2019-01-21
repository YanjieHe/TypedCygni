package cygni.ast;

import java.util.HashMap;

import cygni.Scope;

public class Module {
    public HashMap<String, Module> subModules;
    public HashMap<String, Program> programs;
    public HashMap<String, Scope> symbolTables;

    public Module() {
        this.subModules = new HashMap<String, Module>();
        this.programs = new HashMap<String, Program>();
        this.symbolTables = new HashMap<String, Scope>();
    }

    public void addProgram(String name, Program program) {
        this.programs.put(name, program);
    }

    public void addModule(String name, Module module) {
        this.subModules.put(name, module);
    }
}
