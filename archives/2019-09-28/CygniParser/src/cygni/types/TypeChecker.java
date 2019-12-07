package cygni.types;

import cygni.ast.*;
import cygni.Scope;
import cygni.ast.Module;
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
        } else if (node instanceof Specialize) {
            return checkSpecialize((Specialize) node, scope);
        } else if (node instanceof While) {
            return checkWhile((While) node, scope);
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "not supported ast node");
        }
    }

    public Type checkProgram(Module module, Scope scope) throws TypeException {
        for (Node node : module.nodes) {
            if (node instanceof Def) {
                Def def = (Def) node;
                registerDef(def, scope);
            }
        }
        for (Node node : module.nodes) {
            check(node, scope);
        }
        return Type.Unit;
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
        } else if (node.left instanceof Call) {
            Call call = (Call) node.left;
            Type function = check(call.function, scope);
            ArrayList<Type> arguments = checkArguments(call.arguments, scope);
            if (function instanceof ArrayType && arguments.size() == 1 && arguments.get(0) instanceof IntType) {
                ArrayType arrayType = (ArrayType) function;
                if (arrayType.types.get(0).equals(value)) {
                    return Type.Unit;
                } else {
                    throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "array element type does not match");
                }
            } else {
                throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "cannot assign to array");
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
        } else if (node.value instanceof Long) {
            return Type.Long;
        } else if (node.value instanceof Float) {
            return Type.Float;
        } else if (node.value instanceof Double) {
            return Type.Double;
        } else if (node.value instanceof Character) {
            return Type.Char;
        } else if (node.value instanceof String) {
            return Type.String;
        } else if (node.value instanceof Boolean) {
            return Type.Bool;
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
        scope = new Scope(scope);
        for (Node n : node.nodes) {
            if (n instanceof Def) {
                Def def = (Def) n;
                registerDef(def, scope);
            }
        }
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
        Def def = (Def) scope.lookUpValue("*SELF_FUNCTION*");
        def.addExit(value);
        return value;
    }

    private Type checkCall(Call node, Scope scope) throws TypeException {
        Type function = check(node.function, scope);
        if (function instanceof FunctionType) {
            FunctionType functionType = (FunctionType) function;
            ArrayList<Type> argumentTypes = new ArrayList<Type>();
            for (Node arg : node.arguments) {
                argumentTypes.add(check(arg, scope));
            }
            if (functionType.matchArguments(argumentTypes)) {
                return functionType.getReturnType();
            } else {
                throw new TypeException(
                        node.startLine, node.startCol, node.endLine, node.endCol, "wrong argument type for calling");
            }
        } else if (function instanceof ArrayType) {
            ArrayType arrayType = (ArrayType) function;
            if (node.arguments.size() == 1) {
                Type index = check(node.arguments.get(0), scope);
                if (index instanceof IntType) {
                    return arrayType.types.get(0);
                } else {
                    throw new TypeException(
                            node.startLine, node.startCol, node.endLine, node.endCol, "index type must be 'Int' or 'Long'");
                }
            } else {
                throw new TypeException(
                        node.startLine, node.startCol, node.endLine, node.endCol, "wrong index number");
            }
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "not callable");
        }
    }

    private void registerDef(Def node, Scope scope) {
        ArrayList<Type> parameterTypes = new ArrayList<Type>();
        for (Parameter p : node.parameters) {
            parameterTypes.add(p.type);
        }
        if (node.unknownTypes.size() == 0) {
            scope.putType(node.name, FunctionType.create(parameterTypes, node.returnType));
        } else {
            scope.putType(node.name, FunctionType.create(parameterTypes, node.returnType, node.unknownTypes));
        }
    }

    private Type checkDef(Def node, Scope scope) throws TypeException {
        Scope newScope = new Scope(scope);
        for (Parameter p : node.parameters) {
            newScope.putType(p.name, p.type);
        }
        newScope.putValue("*SELF_FUNCTION*", node);
        Type bodyReturnType = check(node.body, newScope);
        if (node.returnType.equals(Type.Unit)) {
            return Type.Unit;
        } else {
            for (Type exit : node.exits) {
                if (!exit.equals(node.returnType)) {
                    throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "return type");
                }
            }
            return Type.Unit;
        }
    }

    private Type checkSpecialize(Specialize node, Scope scope) throws TypeException {
        Type type = check(node.expression, scope);
        if (type instanceof FunctionType) {
            FunctionType functionType = (FunctionType) type;
            scope = new Scope(scope);
            for (int i = 0; i < node.arguments.size(); i++) {
                scope.putType(functionType.unknownTypes.get(i).name, node.arguments.get(i));
            }
            FunctionType result = (FunctionType) functionType.substitute(scope);
            System.out.println("Function: " + result.toString());
            return result;
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "requires generic type");
        }
    }

//    private Type checkInitArray(InitArray node, Scope scope) throws TypeException {
//        HashSet<Type> types = new HashSet<Type>();
//        Type t = null;
//        for (Node element : node.elements) {
//            t = check(element, scope);
//            types.add(t);
//        }
//        if (types.size() == 0) {
//            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "cannot init 0 length array");
//        } else if (types.size() == 1) {
//            return new ArrayType(t);
//        } else {
//            return new ArrayType(Type.);
//        }
//    }

    private ArrayList<Type> checkArguments(ArrayList<Node> arguments, Scope scope) throws TypeException {
        ArrayList<Type> types = new ArrayList<Type>();
        for (int i = 0; i < arguments.size(); i++) {
            types.add(check(arguments.get(i), scope));
        }
        return types;
    }

    private Type checkWhile(While node, Scope scope) throws TypeException {
        Type condition = check(node.condition, scope);
        if (condition instanceof BoolType) {
            check(node.body, scope);
            return Type.Unit;
        } else {
            throw new TypeException(node.startLine, node.startCol, node.endLine, node.endCol, "the type of condition in while statement must be boolean");
        }
    }
}