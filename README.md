# TypedCygni

This is a simple compiler with runtime virtual machine, written in C++11.
The compiler is able to make type check and type inference, then generates the bytecodes for the virtual machine to execute.

# Instruction Set
- PUSH_CONSTANT_INT
- ADD_INT
- SUB_INT
- MUL_INT
- DIV_INT
- MOD_INT

- GT_INT
- LT_INT
- GE_INT
- LE_INT
- EQ_INT
- NE_INT

- JUMP
- JUMP_IF_TRUE
- JUMP_IF_FALSE

- RETURN_INT
- INVOKE