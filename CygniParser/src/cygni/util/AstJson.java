package cygni.util;

import cygni.ast.*;

public class AstJson {
    public static String visit(Node node) {
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
        } else {
            return "(null: " + node + ")";
        }
    }

    public static String visitBinary(BinaryOp node) {
        String left = visit(node.left);
        String right = visit(node.right);
        return "{\"kind\": \"" + node.kind + "\", \"left\": " + left + ", \"right\": " + right + "}";
    }

    public static String visitUnary(UnaryOp node) {
        String operand = visit(node.operand);
        return "{\"kind\": \"" + node.kind + "\", \"operand\": " + operand + "}";
    }

    public static String visitName(Name node) {
        return "{\"kind\": \"Name\", \"name\": \"" + node.name + "\"}";
    }

    public static String visitConstant(Constant node) {
        return "{\"kind\": \"Constant\", \"value\": \"" + node.value.toString() + "\"}";
    }

    public static String visitIfThen(IfThen node) {
        return "{\"kind\": \"IfThen\", " + "\"condition\": " + visit(node.condition) + ", \"ifTrue\": "
                + visit(node.ifTrue) + "}";
    }

    public static String visitIfElse(IfElse node) {
        return "{\"kind\": \"IfElse\", " + "\"condition\": " + visit(node.condition) + ", \"ifTrue\": " + visit(node.ifTrue)
                + ", \"ifFalse\": " + visit(node.ifFalse) + "}";
    }

    public static String visitBlock(Block node) {
        StringBuilder builder = new StringBuilder();
        builder.append("{\"kind\": \"block\", \"nodes\": [");
        boolean first = true;
        for (Node n : node.nodes) {
            if (first) {
                first = false;
            } else {
                builder.append(",");
            }
            builder.append(visit(n));
        }
        builder.append("]}");
        return builder.toString();
    }

    public static String visitAssign(Assign node) {
        return "{\"kind\": \"Assign\", " + "\"left\": " + visit(node.left) + ", \"value\": " + visit(node.value) + "}";
    }

    public static String visitDef(Def node) {
        return "{\"kind\": \"Def\", \"name\": \"" + node.name + "\", \"body\": " + visit(node.body) + "}";
    }

    public static String visitReturn(Return node) {
        return "{\"kind\": \"Return\", \"value\": " + visit(node.value) + "}";
    }

    public static String visitCall(Call node) {
        String[] items = new String[node.arguments.size()];
        for (int i = 0; i < items.length; i++) {
            items[i] = visit(node.arguments.get(i));
        }
        return "{\"kind\": \"Call\", " + "\"function\": " + visit(node.function) +
                ", \"argument\": [" + String.join(", ", items) + "]}";
    }

    public static String visitInitArray(InitArray node) {
        String[] items = new String[node.elements.size()];
        for (int i = 0; i < items.length; i++) {
            items[i] = visit(node.elements.get(i));
        }
        return "{\"kind\": \"InitArray\", \"elements\": [" + String.join(", ", items) + "]}";
    }
}
