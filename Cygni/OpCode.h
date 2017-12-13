#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include <cstdint>

using std::wstring;
using i32 = int32_t;

enum class OpCode
{
	push_i32_1byte,
	push_i32_2byte,

    push_f64_0,
    push_f64_1,

	push_i32,
	push_f64,
	push_string,
	push_null,

    push_static_i32,
    push_static_f64,
    push_static_string,

    push_stack_i32,
    push_stack_f64,
    push_stack_string,

    pop_static_i32,
    pop_static_f64,
    pop_static_string,

    pop_stack_i32,
    pop_stack_f64,
    pop_stack_string,

    push_constant_i32,
    push_constant_f64,
    push_constant_string,

    store_constant_i32,
    store_constant_f64,
    store_constant_string,

    function_begin,
    function_end,

    add_i32,
    sub_i32,
    mul_i32,
    div_i32,
    mod_i32,

	add_f64,
    sub_f64,
    mul_f64,
    div_f64,
    mod_f64,

    gt_i32,
    lt_i32,
    ge_i32,
    le_i32,
    eq_i32,
    ne_i32,

    gt_f64,
    lt_f64,
    ge_f64,
    le_f64,
    eq_f64,
    ne_f64,

    logical_and,
    logical_or,
    logical_not,

    minus_i32,

    minus_f64,

    cast_int32_to_float64,
    cast_float64_to_int32,

    jump,
    jump_if_true,
    jump_if_false,

    return_i32,
    return_f64,

    push_function,
    invoke,

    push_array_i32,
    push_array_f64,
	push_array_object,

    pop_array_i32,
    pop_array_f64,
    pop_array_object,

    new_array,
    new_array_literal_i32,
    new_array_literal_f64,
    new_array_literal_object,

	constant_i32,
	constant_i64,
	constant_f64,
	constant_string
};

wstring opcode_to_wstring(OpCode code);

i32 OperandSize(OpCode op);

#endif // OPCODE_H 
