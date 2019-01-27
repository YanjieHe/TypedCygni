package cygni.interpreter;

import cygni.ast.*;
import cygni.Scope;
import cygni.exceptions.RuntimeException;


public class Interpreter {
    public CygObject eval(Node node, Scope scope) throws RuntimeException {
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
        } else if (node instanceof While) {
            return evalWhile((While) node, scope);
        } else {
            throw new RuntimeException("unsupported node");
        }
    }

    public CygObject evalBinary(BinaryOp node, Scope scope) throws RuntimeException {
        CygObject lvalue = eval(node.left, scope);
        CygObject rvalue = eval(node.right, scope);
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
            throw new RuntimeException("unsupported binary operation");
        }
    }

    public CygObject evalUnary(UnaryOp node, Scope scope) throws RuntimeException {
        CygObject value = eval(node.operand, scope);
        if (node.kind == UnaryOpKind.UnaryPlus) {
            return value;
        } else if (node.kind == UnaryOpKind.Negate) {
            if (value instanceof CygInt) {
                return new CygInt(-((CygInt) value).intValue());
            } else {
                return new CygDouble(-((CygDouble) value).doubleValue());
            }
        } else if (node.kind == UnaryOpKind.Not) {
            return new CygBool(!((CygBool) value).booleanValue());
        } else {
            throw new RuntimeException("unsupported unary operation");
        }
    }

    public CygObject evalBlock(Block node, Scope scope) throws RuntimeException {
        CygObject result = CygObject.Unit;
        for (Node n : node.nodes) {
            result = eval(n, scope);
        }
        return result;
    }

    public CygObject evalCall(Call node, Scope scope) throws RuntimeException {
        Callable function = (Callable) eval(node.function, scope);
        CygObject[] arguments = new CygObject[node.arguments.size()];
        for (int i = 0; i < node.arguments.size(); i++) {
            arguments[i] = eval(node.arguments.get(i), scope);
        }
        return function.invoke(arguments);
    }

    public CygObject evalConstant(Constant node, Scope scope) throws RuntimeException {
        if (node.value instanceof Integer) {
            return new CygInt(((Integer) node.value).intValue());
        } else if (node.value instanceof Double) {
            return new CygDouble(((Double) node.value).doubleValue());
        } else if (node.value instanceof Boolean) {
            return new CygBool(((Boolean) node.value).booleanValue());
        } else {
            throw new RuntimeException("not supported constant type");
        }
    }

    public CygObject evalDef(Def node, Scope scope) throws RuntimeException {
        Function function = new Function(node.name, node.parameters, scope, node.body);
        scope.putValue(node.name, function);
        return CygObject.Unit;
    }

    public CygObject evalIfThen(IfThen node, Scope scope) throws RuntimeException {
        CygObject condition = eval(node.condition, scope);
        if (condition instanceof CygBool) {
            if (((CygBool) condition).booleanValue()) {
                return eval(node.ifTrue, scope);
            } else {
                return CygObject.Unit;
            }
        } else {
            return CygObject.Unit;
        }
    }

    public CygObject evalIfElse(IfElse node, Scope scope) throws RuntimeException {
        CygObject condition = eval(node.condition, scope);
        if (condition instanceof CygBool) {
            if (((CygBool) condition).booleanValue()) {
                return eval(node.ifTrue, scope);
            } else {
                return eval(node.ifFalse, scope);
            }
        } else {
            return CygObject.Unit;
        }
    }

    public CygObject evalName(Name node, Scope scope) {
        return (CygObject) scope.lookUpValue(node.name);
    }

    public CygObject evalReturn(Return node, Scope scope) throws RuntimeException {
        return eval(node.value, scope);
    }

    public CygObject evalVar(Var node, Scope scope) throws RuntimeException {
        scope.putValue(node.name, eval(node.value, scope));
        return CygObject.Unit;
    }

    public CygObject evalInitArray(InitArray node, Scope scope) throws RuntimeException {
        CygObject[] objects = new CygObject[node.elements.size()];
        for (int i = 0; i < objects.length; i++) {
            objects[i] = eval(node.elements.get(i), scope);
        }
        return new CygArray(objects);
    }

    public CygObject evalAssign(Assign node, Scope scope) throws RuntimeException {
        if (node.left instanceof Name) {
            String name = ((Name) node.left).name;
            CygObject value = eval(node.value, scope);
            // TO DO: check whether the variable is defined.
            if (scope.putValueIfDefined(name, value)) {
                return CygObject.Unit;
            } else {
                throw new RuntimeException("'" + name + "' is not defined");
            }
        } else if (node.left instanceof Call) {
            Call call = (Call) node.left;
            CygArray array = (CygArray) eval(call.function, scope);
            CygInt index = (CygInt) eval(call.arguments.get(0), scope);
            array.write(index.intValue(), eval(node.value, scope));
            return CygObject.Unit;
        } else {
            return CygObject.Unit;
        }
    }

    public CygObject evalSpecialize(Specialize node, Scope scope) throws RuntimeException {
        return eval(node.expression, scope);
    }

    public CygObject evalWhile(While node, Scope scope) throws RuntimeException {
        while (((CygBool) eval(node.condition, scope)).booleanValue()) {
            eval(node.body, scope);
        }
        return CygObject.Unit;
    }
}
