package cygni.util;

import cygni.ast.*;

import java.util.ArrayList;
import java.util.HashMap;

public class AstJson {
    public static String toString(Program program) {
        return JsonUtil.toString(makeList(program.nodes));
    }

    public static Object visit(Node node) {
        if (node instanceof BinaryOp) {
            return visitBinary((BinaryOp) node);
        } else if (node instanceof UnaryOp) {
            return visitUnary((UnaryOp) node);
        } else if (node instanceof Name) {
            return visitName((Name) node);
        } else if (node instanceof Constant) {
            return visitConstant((Constant) node);
        } else if (node instanceof IfThen) {
            return visitIfThen((IfThen) node);
        } else if (node instanceof IfElse) {
            return visitIfElse((IfElse) node);
        } else if (node instanceof Block) {
            return visitBlock((Block) node);
        } else if (node instanceof Assign) {
            return visitAssign((Assign) node);
        } else if (node instanceof Def) {
            return visitDef((Def) node);
        } else if (node instanceof Return) {
            return visitReturn((Return) node);
        } else if (node instanceof Call) {
            return visitCall((Call) node);
        } else if (node instanceof InitArray) {
            return visitInitArray((InitArray) node);
        } else if (node instanceof While) {
            return visitWhile((While) node);
        } else if (node instanceof Var) {
            return visitVar((Var) node);
        } else {
            return "(null: " + node + ")";
        }
    }

    public static HashMap<String, Object> makeMap(String key0, Object value0) {
        HashMap<String, Object> map = new HashMap<String, Object>();
        map.put(key0, value0);
        return map;
    }

    public static HashMap<String, Object> makeMap(String key0, Object value0, String key1, Object value1) {
        HashMap<String, Object> map = makeMap(key0, value0);
        map.put(key1, value1);
        return map;
    }

    public static HashMap<String, Object> makeMap(String key0, Object value0, String key1, Object value1, String key2, Object value2) {
        HashMap<String, Object> map = makeMap(key0, value0, key1, value1);
        map.put(key2, value2);
        return map;
    }

    public static HashMap<String, Object> makeMap(String key0, Object value0, String key1, Object value1,
                                                  String key2, Object value2, String key3, Object value3) {
        HashMap<String, Object> map = makeMap(key0, value0, key1, value1, key2, value2);
        map.put(key3, value3);
        return map;
    }

    public static HashMap<String, Object> makeMap(String key0, Object value0, String key1, Object value1,
                                                  String key2, Object value2, String key3, Object value3,
                                                  String key4, Object value4) {
        HashMap<String, Object> map = makeMap(key0, value0, key1, value1, key2, value2, key3, value3);
        map.put(key4, value4);
        return map;
    }

    public static ArrayList<Object> makeList(ArrayList<Node> nodes) {
        ArrayList<Object> objects = new ArrayList<Object>();
        for (Node node : nodes) {
            objects.add(visit(node));
        }
        return objects;
    }

    public static Object visitBinary(BinaryOp node) {
        Object left = visit(node.left);
        Object right = visit(node.right);
        return makeMap("kind", node.kind,
                "left", left, "right", right);
    }

    public static Object visitUnary(UnaryOp node) {
        Object operand = visit(node.operand);
        return makeMap("kind", node.kind, "operand", operand);
    }

    public static Object visitName(Name node) {
        return makeMap("kind", "Name", "name", node.name);
    }

    public static Object visitConstant(Constant node) {
        return makeMap("kind", "Constant", "value", node.value.toString());
    }

    public static Object visitIfThen(IfThen node) {
        return makeMap("kind", "IfThen", "condition", visit(node.condition),
                "ifTrue", visit(node.ifTrue));
    }

    public static Object visitIfElse(IfElse node) {
        return makeMap("kind", "IfElse", "condition", visit(node.condition),
                "ifTrue", visit(node.ifTrue), "ifFalse", visit(node.ifFalse));
    }

    public static Object visitBlock(Block node) {
        return makeMap("kind", "Block", "nodes", makeList(node.nodes));
    }

    public static Object visitAssign(Assign node) {
        return makeMap("kind", "Assign", "left", visit(node.left), "value", visit(node.value));
    }

    public static Object visitDef(Def node) {
        return makeMap("kind", "Def",
                "name", node.name,
                "type-parameters", node.unknownTypes,
                "parameters", node.parameters,
                "body", visit(node.body));
    }

    public static Object visitReturn(Return node) {
        return makeMap("kind", "Return", "value", visit(node.value));
    }

    public static Object visitCall(Call node) {
        return makeMap("kind", "Call",
                "function", visit(node.function), "argument", makeList(node.arguments));
    }

    public static Object visitInitArray(InitArray node) {
        return makeMap("kind", "InitArray",
                "elements", makeList(node.elements));
    }

    public static Object visitWhile(While node) {
        return makeMap("kind", "While", "condition", visit(node.condition),
                "body", visit(node.body));
    }

    public static Object visitVar(Var node) {
        return makeMap("kind", "Var", "name", node.name, "value", visit(node.value));
    }
}
