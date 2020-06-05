#include "Parser.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Unicode.h"
#include "OpCode.h"

char* parse_string(State* state)
{
	uint16_t len;
	char* str;

	len = parse_ushort(state);
	str = vm_alloc(state, sizeof(char) * (len + 1));
	if (fread(str, sizeof(char), len, state->source) == len)
	{
		str[len] = '\0';
		return str;
	}
	else
	{
		fprintf(stderr, "cannot read string\n");
		vm_throw(state, VM_ERROR_READ_STRING);
		return NULL; // make the compiler happy
	}
}

// Big Endian
uint16_t parse_ushort(State* state)
{
	uint16_t value;
	Byte b1;
	Byte b2;
	size_t res1;
	size_t res2;

	res1 = fread(&b1, sizeof(Byte), 1, state->source);
	res2 = fread(&b2, sizeof(Byte), 1, state->source);

	if (res1 == 1 && res2 == 1)
	{
		value = ((uint16_t)b1) * 256 + ((uint16_t)b2);
		return value;
	}
	else
	{
		fprintf(stderr, "cannot read ushort\n");
		vm_throw(state, VM_ERROR_READ_U16);
		return 0; // make the compiler happy
	}
}

Executable* parse_file(State* state, const char* path)
{
	uint16_t class_count;
	uint16_t module_count;
	uint16_t main_func_module_index;
	uint16_t main_func_index;
	int i;
	Executable* exe;

	state->source = fopen(path, "rb");

	if (state->source)
	{
		exe = vm_alloc(state, sizeof(Executable));
		main_func_module_index = parse_ushort(state);
		main_func_index = parse_ushort(state);
		class_count = parse_ushort(state);
		module_count = parse_ushort(state);
		exe->class_count = class_count;
		exe->module_count = module_count;
		exe->classes = (ClassInfo*)vm_alloc(state, sizeof(ClassInfo) * class_count);
		exe->modules = (ModuleInfo*)vm_alloc(state, sizeof(ModuleInfo) * module_count);

		printf("main function module: %d, main function: %d\n",
			main_func_module_index, main_func_index);
		printf("class count = %d, module count = %d\n", class_count, module_count);
		for (i = 0; i < class_count; i++)
		{
			parse_class(state, &(exe->classes[i]));
		}
		for (i = 0; i < module_count; i++)
		{
			parse_module(state, &(exe->modules[i]));
		}

		exe->entry = exe->modules[main_func_module_index].functions[main_func_index];
		fclose(state->source);
		return exe;
	}
	else
	{
		fprintf(stderr, "fail to read file '%s'", path);
		fclose(state->source);
		state->source = NULL;
		vm_throw(state, VM_ERROR_OPEN_FILE);
		return NULL; // make the compiler happy
	}
}

void parse_class(State* state, ClassInfo* class_info)
{
	int i;

	class_info->name = parse_string(state);
	printf("class name: %s\n", class_info->name);
	class_info->n_fields = parse_ushort(state);
	class_info->field_names = vm_alloc(state, sizeof(char*) * class_info->n_fields);

	for (i = 0; i < class_info->n_fields; i++)
	{
		class_info->field_names[i] = parse_string(state);
		printf("field: %s\n", class_info->field_names[i]);
	}
	parse_constant_pool(state, &(class_info->constant_pool));

	class_info->n_methods = parse_ushort(state);
	class_info->methods = vm_alloc(state, sizeof(Function*) * class_info->n_methods);
	for (i = 0; i < class_info->n_methods; i++)
	{
		class_info->methods[i] = parse_function(state);
		if (!(class_info->methods[i]->is_native_function))
		{
			class_info->methods[i]->u.func->n_constants = class_info->constant_pool.n_constants;
			class_info->methods[i]->u.func->constant_pool = class_info->constant_pool.constants;
		}
		printf("method: %s\n", class_info->methods[i]->name);
	}
}

void parse_module(State* state, ModuleInfo* module_info)
{
	int i;

	module_info->name = parse_string(state);
	printf("module name: %s\n", module_info->name);
	module_info->n_fields = parse_ushort(state);
	module_info->field_names = vm_alloc(state, sizeof(char*) * module_info->n_fields);

	for (i = 0; i < module_info->n_fields; i++)
	{
		module_info->field_names[i] = parse_string(state);
		printf("field: %s\n", module_info->field_names[i]);
	}

	parse_constant_pool(state, &(module_info->constant_pool));
	module_info->n_functions = parse_ushort(state);
	module_info->functions = (Function**)vm_alloc(state, sizeof(Function*) * module_info->n_functions);
	for (i = 0; i < module_info->n_functions; i++)
	{
		module_info->functions[i] = parse_function(state);
		if (!(module_info->functions[i]->is_native_function))
		{
			module_info->functions[i]->u.func->n_constants = module_info->constant_pool.n_constants;
			module_info->functions[i]->u.func->constant_pool = module_info->constant_pool.constants;
		}
		printf("function: %s\n", module_info->functions[i]->name);
	}
}

Function * parse_function(State* state)
{
	Function* function;
	Byte is_native_function;
	uint16_t code_len;
	NativeFunction* native_function;

	function = (Function*)vm_alloc(state, sizeof(Function));
	if (fread(&is_native_function, sizeof(Byte), 1, state->source) == 1)
	{
		if (is_native_function)
		{
			function->is_native_function = true;
			function->name = parse_string(state);
			native_function = (NativeFunction*)vm_alloc(state, sizeof(NativeFunction));
			native_function->is_loaded = false;
			native_function->function_pointer = NULL;
			native_function->args_size = parse_ushort(state);
			native_function->lib_path = parse_string(state);
			native_function->func_name = parse_string(state);
			function->u.nv = native_function;
			return function;
		}
		else
		{
			function->name = parse_string(state);
			function->is_native_function = false;
			function->u.func = (FunctionInfo*)vm_alloc(state, sizeof(FunctionInfo));
			function->u.func->args_size = parse_ushort(state);
			function->u.func->locals = parse_ushort(state);
			//printf("args_size = %d, locals = %d\n", function->u.func->args_size, function->u.func->locals);
			code_len = parse_ushort(state);
			function->u.func->code_len = code_len;
			//printf("function code len=%d\n", code_len);
			function->u.func->code = (Byte*)vm_alloc(state, sizeof(Byte) * code_len);
			if (fread(function->u.func->code, sizeof(Byte), code_len, state->source) == code_len)
			{
				return function;
			}
			else
			{
				fprintf(stderr, "fail to read byte code\n");
				vm_throw(state, VM_ERROR_READ_FUNCTION_BYTE_CODE);
				return NULL; // make the compiler happy
			}
		}
	}
	else
	{
		fprintf(stderr, "fail to load function");
		vm_throw(state, -1);
		return NULL; // make the compiler happy
	}
}

void parse_constant_pool(State* state, ConstantPool* constant_pool)
{
	Byte type_tag;
	int i;
	char* str;
	String* u32_str;

	constant_pool->n_constants = parse_ushort(state);
	printf("# of constants: %d\n", constant_pool->n_constants);
	constant_pool->constants = (Constant*)vm_alloc(state, sizeof(Constant) * constant_pool->n_constants);
	for (i = 0; i < constant_pool->n_constants; i++)
	{
		if (fread(&type_tag, sizeof(Byte), 1, state->source) == 1)
		{
			str = parse_string(state);
			printf("constant: '%s'\n", str);
			constant_pool->constants[i].tag = type_tag;
			if (type_tag == TYPE_I32)
			{
				constant_pool->constants[i].u.i32_v = atoi(str);
			}
			else if (type_tag == TYPE_I64)
			{
				constant_pool->constants[i].u.i64_v = atol(str);
			}
			else if (type_tag == TYPE_F32)
			{
				constant_pool->constants[i].u.f32_v = (float_t)atof(str);
			}
			else if (type_tag == TYPE_F64)
			{
				constant_pool->constants[i].u.f64_v = (double_t)atof(str);
			}
			else if (type_tag == TYPE_STRING)
			{
				u32_str = vm_alloc(state, sizeof(String));
				u32_str->length = utf8_to_utf32_len(str);
				u32_str->characters = utf8_to_utf32(str, u32_str->length);
				constant_pool->constants[i].u.str_v = u32_str;
			}
			else
			{
				fprintf(stderr, "wrong type tag\n");
				vm_throw(state, VM_ERROR_WRONG_TYPE_TAG);
			}
		}
		else
		{
			fprintf(stderr, "cannot read type tag\n");
			vm_throw(state, VM_ERROR_READ_TYPE_TAG);
		}
	}
}

void view_exe(Executable* exe)
{
	int i;
	int j;
	Function* function;

	printf("class count: %d, module count: %d\n\n", exe->class_count, exe->module_count);
	for (i = 0; i < exe->class_count; i++)
	{
		printf("class: %s\n", exe->classes[i].name);
		for (j = 0; j < exe->classes[i].n_fields; j++)
		{
			printf("\tfield: %s\n", exe->classes[i].field_names[j]);
		}
		printf("\n");
		for (j = 0; j < exe->classes[i].n_methods; j++)
		{
			function = exe->classes[i].methods[j];
			view_function(function, exe);
		}
		printf("\n");
	}
	for (i = 0; i < exe->module_count; i++)
	{
		printf("module: %s\n", exe->modules[i].name);
		for (j = 0; j < exe->modules[i].n_fields; j++)
		{
			printf("\tfield: %s\n", exe->modules[i].field_names[j]);
		}
		printf("\n");
		for (j = 0; j < exe->modules[i].n_functions; j++)
		{
			function = exe->modules[i].functions[j];
			view_function(function, exe);
		}
		printf("\n");
	}
}

void view_function(Function * function, Executable* exe)
{
	int i;
	Byte op_code;
	const char* op_name;
	const char* op_type;
	uint32_t u32_v;
	uint32_t module_index;

	if (function->is_native_function)
	{
		printf("\tmethod: %s\n", function->name);
		printf("\targs_size=%d\n", function->u.nv->args_size);
		printf("\tlibrary name: %s\n\tfunction name: %s\n\n", function->u.nv->lib_path, function->u.nv->func_name);
	}
	else
	{
		printf("\tmethod: %s\n", function->name);
		printf("\targs_size=%d, locals=%d\n", function->u.func->args_size, function->u.func->locals);

		printf("\tcode:\n");
		i = 0;
		while (i < function->u.func->code_len)
		{
			op_code = function->u.func->code[i];
			op_name = opcode_info[op_code][0];
			if (strcmp(op_name, "INVOKE_FUNCTION") == 0)
			{
				printf("\t\t%d: %s", i, op_name);
				i++;
				module_index = function->u.func->code[i];
				module_index = module_index * 256 + ((uint16_t)function->u.func->code[i + 1]);
				printf(" %s", exe->modules[module_index].name);
				i = i + 2;

				u32_v = function->u.func->code[i];
				u32_v = u32_v * 256 + ((uint16_t)function->u.func->code[i + 1]);
				printf(".%s", exe->modules[module_index].functions[u32_v]->name);
				i = i + 2;
			}
			else if (strcmp(op_name, "NEW") == 0)
			{
				printf("\t\t%d: %s", i, op_name);
				i++;
				u32_v = function->u.func->code[i];
				u32_v = u32_v * 256 + ((uint16_t)function->u.func->code[i + 1]);
				printf(" %s", exe->classes[u32_v].name);
				i = i + 2;
			}
			else
			{
				printf("\t\t%d: %s", i, op_name);
				i++;
				op_type = opcode_info[op_code][1];
				if (strcmp(op_type, "b") == 0)
				{
					printf(" %d", (int)function->u.func->code[i]);
					i++;
				}
				else if (strcmp(op_type, "u") == 0 || strcmp(op_type, "p") == 0)
				{
					u32_v = function->u.func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)function->u.func->code[i + 1]);
					printf(" %d", u32_v);
					i = i + 2;
				}
				else if (strcmp(op_type, "uu") == 0)
				{
					u32_v = function->u.func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)function->u.func->code[i + 1]);
					printf(" %d", u32_v);
					i = i + 2;

					u32_v = function->u.func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)function->u.func->code[i + 1]);
					printf(" %d", u32_v);
					i = i + 2;
				}
			}
			printf("\n");
		}

		printf("\n");
	}
}

void view_type_tag(State* state, Byte tag)
{
	if (tag == TYPE_I32)
	{
		printf(" [I32]");
	}
	else if (tag == TYPE_I64)
	{
		printf(" [I64]");
	}
	else if (tag == TYPE_F32)
	{
		printf(" [F32]");
	}
	else if (tag == TYPE_F64)
	{
		printf(" [F64]");
	}
	else if (tag == TYPE_STRING)
	{
		printf(" [STRING]");
	}
	else if (tag == TYPE_OBJECT)
	{
		printf(" [OBJECT]");
	}
	else
	{
		fprintf(stderr, "error type tag\n");
		vm_throw(state, -1);
	}
}
