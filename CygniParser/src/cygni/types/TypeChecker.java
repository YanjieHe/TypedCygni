package cygni.types;

import cygni.ast.*;
import cygni.Scope;
import cygni.exceptions.TypeException;

import java.util.ArrayList;

public class TypeChecker {

    public Type check(Node node, Scope scope) throws TypeException {
        if (node instanceof Assign) {
            return checkAssign((Assign) node, scope);
        } else if (node instanceof BinaryOp) {
            return checkBinary((BinaryOp) node, scope);
        } else if (node instanceof UnaryOp) {
            return checkUnary((UnaryOp) node, scope);
        } else if (node instanceof Constant) {
            return checkConstant((Constant) node, scope);
        } else if (node instanceof Name) {
            return checkName((Name) node, scope);
        } else if (node instanceof IfElse) {
            return checkIfElse((IfElse) node, scope);
        } else if (node instanceof IfThen) {
            return checkIfThen((IfThen) node, scope);
        } else if (node instanceof Block) {
            return checkBlock((Block) node, scope);
        } else if (node instanceof Var) {
            return checkVar((Var) node, scope);
        } else if (node instanceof Return) {
            return checkReturn((Return) node, scope);
        } else if (node instanceof Call) {
            return checkCall((Call) node, scope);
        } else if (node instanceof Def) {
            return checkDef((Def) node, scope);
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "not supported ast node");
        }
    }

    private Type checkAssign(Assign node, Scope scope) throws TypeException {
        Type value = check(node.value, scope);
        if (node.left instanceof Name) {
            Name name = (Name) node.left;
            Type type = scope.lookUpType(name.name);
            if (type == null) {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, name.name + " not defined");
            } else {
                if (type instanceof UnitType) {
                    return Type.Unit;
                } else if (type.equals(value)) {
                    return Type.Unit;
                } else {
                    throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "assign");
                }
            }
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "cannot assign");
        }
    }

    private Type checkBinary(BinaryOp node, Scope scope) throws TypeException {
        Type left = check(node.left, scope);
        Type right = check(node.right, scope);
        if (node.kind == BinaryOpKind.Add
                || node.kind == BinaryOpKind.Subtract
                || node.kind == BinaryOpKind.Multiply
                || node.kind == BinaryOpKind.Divide
                || node.kind == BinaryOpKind.Modulo) {
            if (left instanceof IntType && right instanceof IntType) {
                return Type.Int;
            } else if (left instanceof DoubleType && right instanceof DoubleType) {
                return Type.Double;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else if (node.kind == BinaryOpKind.GreaterThan
                || node.kind == BinaryOpKind.LessThan
                || node.kind == BinaryOpKind.GreaterThanOrEqual
                || node.kind == BinaryOpKind.LessThanOrEqual) {
            if (left instanceof IntType && right instanceof IntType) {
                return Type.Bool;
            } else if (left instanceof DoubleType && right instanceof DoubleType) {
                return Type.Bool;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else if (node.kind == BinaryOpKind.Equal
                || node.kind == BinaryOpKind.NotEqual) {
            if (left.equals(right)) {
                return Type.Bool;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else if (node.kind == BinaryOpKind.And || node.kind == BinaryOpKind.Or) {
            if (left instanceof BoolType && right instanceof BoolType) {
                return Type.Bool;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
        }
    }

    private Type checkUnary(UnaryOp node, Scope scope) throws TypeException {
        Type operand = check(node.operand, scope);
        if (node.kind == UnaryOpKind.Negate) {
            if (operand instanceof IntType) {
                return Type.Int;
            } else if (operand instanceof DoubleType) {
                return Type.Double;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else if (node.kind == UnaryOpKind.Not) {
            if (operand instanceof BoolType) {
                return Type.Bool;
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
            }
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.kind.toString());
        }
    }

    private Type checkConstant(Constant node, Scope scope) throws TypeException {
        if (node.value instanceof Integer) {
            return Type.Int;
        } else if (node.value instanceof Double) {
            return Type.Double;
        } else if (node.value instanceof String) {
            return Type.String;
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.value.toString());
        }
    }

    private Type checkName(Name node, Scope scope) throws TypeException {
        Type type = scope.lookUpType(node.name);
        if (type == null) {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, node.name + " not defined");
        } else {
            return type;
        }
    }

    private Type checkIfElse(IfElse node, Scope scope) throws TypeException {
        Type condition = check(node.condition, scope);
        if (condition instanceof BoolType) {
            Type ifTrue = check(node.ifTrue, scope);
            Type ifFalse = check(node.ifFalse, scope);
            return Type.union(ifTrue, ifFalse);
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "condition should be bool type");
        }
    }

    private Type checkIfThen(IfThen node, Scope scope) throws TypeException {
        Type condition = check(node.condition, scope);
        if (condition instanceof BoolType) {
            Type ifTrue = check(node.ifTrue, scope);
            return Type.union(ifTrue, Type.Unit);
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "condition should be bool type");
        }
    }

    private Type checkBlock(Block node, Scope scope) throws TypeException {
        Type result = Type.Unit;
        for (Node n : node.nodes) {
            result = check(n, scope);
        }
        return result;
    }

    private Type checkVar(Var node, Scope scope) throws TypeException {
        Type value = check(node.value, scope);
        scope.putType(node.name, value);
        return Type.Unit;
    }

    private Type checkReturn(Return node, Scope scope) throws TypeException {
        Type value = check(node.value, scope);
        return value;
    }

    private Type checkCall(Call node, Scope scope) throws TypeException {
        Type function = check(node.function, scope);
        if (function instanceof FunctionType) {
            Type functionType = (FunctionType) function;
            ArrayList<Type> argumentTypes = new ArrayList<Type>();
            for (Node arg : node.arguments) {
                argumentTypes.add(check(arg, scope));
            }
            if (functionType.parameters.size() == node.arguments.size() + 1) {
                for (int i = 0; i < node.arguments.size(); i++) {
                    if (!functionType.parameters.get(i).equals(argumentTypes.get(i))) {
                        throw new TypeException(
                                node.startLine, node.startCol, node.endLine, node.endCol, "argument " + (i + 1));
                    }
                }
                return functionType.parameters.get(functionType.parameters.size() - 1);
            } else {
                throw new TypeException(
                        node.startLine, node.startCol, node.endLine, node.endCol, "wrong argument number");
            }
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "not callable");
        }
    }

    private Type checkDef(Def node, Scope scope) throws TypeException {
        Scope newScope = new Scope(scope);
        ArrayList<Type> parameterTypes = new ArrayList<Type>();
        for (Parameter p : node.parameters) {
            newScope.putType(p.name, p.type);
            parameterTypes.add(p.type);
        }
        newScope.putType(node.name, new FunctionType(parameterTypes, node.returnType));
        Type bodyReturnType = check(node.body, newScope);
        if (bodyReturnType.equals(node.returnType)) {
            return Type.Unit;
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "return type");
        }
    }
}