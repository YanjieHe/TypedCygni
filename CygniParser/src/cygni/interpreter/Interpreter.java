package cygni.interpreter;

import cygni.ast.*;
import cygni.Scope;
import cygni.exceptions.TypeException;
import cygni.types.TypeConstructor;


public class Interpreter {
    public Object eval(Node node, Scope scope) {
        if (node instanceof BinaryOp) {
            return evalBinary((BinaryOp) node, scope);
        } else if (node instanceof UnaryOp) {
            return evalUnary((UnaryOp) node, scope);
        } else if (node instanceof Block) {
            return evalBlock((Block) node, scope);
        } else if (node instanceof Call) {
            return evalCall((Call) node, scope);
        } else if (node instanceof Constant) {
            return evalConstant((Constant) node, scope);
        } else if (node instanceof Def) {
            return evalDef((Def) node, scope);
        } else if (node instanceof IfThen) {
            return evalIfThen((IfThen) node, scope);
        } else if (node instanceof IfElse) {
            return evalIfElse((IfElse) node, scope);
        } else if (node instanceof Name) {
            return evalName((Name) node, scope);
        } else if (node instanceof Return) {
            return evalReturn((Return) node, scope);
        } else if (node instanceof Var) {
            return evalVar((Var) node, scope);
        } else if (node instanceof InitArray) {
            return evalInitArray((InitArray) node, scope);
        } else if (node instanceof Assign) {
            return evalAssign((Assign) node, scope);
        } else if (node instanceof Specialize) {
            return evalSpecialize((Specialize) node, scope);
        } else {
            return null;
        }
    }

    public Object evalBinary(BinaryOp node, Scope scope) {
        Object lvalue = eval(node.left, scope);
        Object rvalue = eval(node.right, scope);
        if (node.kind == BinaryOpKind.Add) {
            return BuiltinFunctions.add(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.Subtract) {
            return BuiltinFunctions.subtract(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.Multiply) {
            return BuiltinFunctions.multiply(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.Divide) {
            return BuiltinFunctions.divide(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.GreaterThan) {
            return BuiltinFunctions.greaterThan(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.LessThan) {
            return BuiltinFunctions.lessThan(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.GreaterThanOrEqual) {
            return BuiltinFunctions.greaterThanOrEqual(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.LessThanOrEqual) {
            return BuiltinFunctions.lessThanOrEqual(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.Equal) {
            return BuiltinFunctions.equal(lvalue, rvalue);
        } else if (node.kind == BinaryOpKind.NotEqual) {
            return BuiltinFunctions.notEqual(lvalue, rvalue);
        } else {
            return null;
        }
    }

    public Object evalUnary(UnaryOp node, Scope scope) {
        Object value = eval(node, scope);
        if (node.kind == UnaryOpKind.Not) {
            return !((Boolean) value);
        } else {
            return null;
        }
    }

    public Object evalBlock(Block node, Scope scope) {
        Object result = null;
        for (Node n : node.nodes) {
            result = eval(n, scope);
        }
        return result;
    }

    public Object evalCall(Call node, Scope scope) {
        Callable function = (Callable) eval(node.function, scope);
        Object[] arguments = new Object[node.arguments.size()];
        for (int i = 0; i < node.arguments.size(); i++) {
            arguments[i] = eval(node.arguments.get(i), scope);
        }
        return function.invoke(arguments);
    }

    public Object evalConstant(Constant node, Scope scope) {
        return node.value;
    }

    public Object evalDef(Def node, Scope scope) {
        Function function = new Function(node.name, node.parameters, scope, node.body);
        scope.putValue(node.name, function);
        return null;
    }

    public Object evalIfThen(IfThen node, Scope scope) {
        Object condition = eval(node.condition, scope);
        if (condition instanceof Boolean) {
            if (((Boolean) condition)) {
                return eval(node.ifTrue, scope);
            } else {
                return null;
            }
        } else {
            return null;
        }
    }

    public Object evalIfElse(IfElse node, Scope scope) {
        Object condition = eval(node.condition, scope);
        if (condition instanceof Boolean) {
            if (((Boolean) condition)) {
                return eval(node.ifTrue, scope);
            } else {
                return eval(node.ifFalse, scope);
            }
        } else {
            return null;
        }
    }

    public Object evalName(Name node, Scope scope) {
        return scope.lookUpValue(node.name);
    }

    public Object evalReturn(Return node, Scope scope) {
        return eval(node.value, scope);
    }

    public Object evalVar(Var node, Scope scope) {
        scope.putValue(node.name, eval(node.value, scope));
        return null;
    }

    public Object evalInitArray(InitArray node, Scope scope) {
        Object[] objects = new Object[node.elements.size()];
        for (int i = 0; i < objects.length; i++) {
            objects[i] = eval(node.elements.get(i), scope);
        }
        return new Array(objects);
    }

    public Object evalAssign(Assign node, Scope scope) {
        if (node.left instanceof Name) {
            String name = ((Name) node.left).name;
            scope.putValue(name, eval(node.value, scope));
            return null;
        } else if (node.left instanceof Call) {
            Call call = (Call) node.left;
            Array array = (Array) eval(call.function, scope);
            Integer index = (Integer) eval(call.arguments.get(0), scope);
            array.write(index, eval(node.value, scope));
            return null;
        } else {
            return null;
        }
    }

    public Object evalSpecialize(Specialize node, Scope scope) {
        TypeConstructor typeConstructor = (TypeConstructor) eval(node.expression, scope);
        try {
            typeConstructor = typeConstructor.substitute(node.startLine, node.startCol, node.endLine, node.endCol, node.arguments);
            if (typeConstructor.name.equals("Array")) {
                return new BuiltinFunctions.InitGenericArray(node.arguments.get(0));
            } else {
                return null;
            }
        } catch (TypeException ex) {
            System.out.println("type exception");
            return null;
        }
    }
}
