package cygni.ast;

import cygni.types.Type;
import cygni.types.UnknownType;

import java.util.ArrayList;

public class DefClass extends Node {
    public static enum Access {
        Public,
        Private,
        Protected
    }

    public static class Field {
        public Access access;
        public String name;
        public Type type;
        public Node value;

        public Field(Access access, String name, Type type, Node value) {
            this.access = access;
            this.name = name;
            this.type = type;
            this.value = value;
        }
    }

    public static class Method {
        public Access access;
        public DefClass self;
        public Def def;

        public Method(Access access, DefClass self, Def def) {
            this.access = access;
            this.self = self;
            this.def = def;
        }
    }

    public String name;
    public ArrayList<Field> fields;
    public ArrayList<Method> methods;
    public ArrayList<UnknownType> unknownTypes;

    public DefClass(int startLine, int startCol, int endLine, int endCol,
                    ArrayList<Field> fields, ArrayList<Method> methods, ArrayList<UnknownType> unknownTypes) {
        super(startLine, startCol, endLine, endCol);
        this.fields = fields;
        this.methods = methods;
        this.unknownTypes = unknownTypes;
    }
}
