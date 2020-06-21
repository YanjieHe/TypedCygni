#include "loader.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unicode.h"
#include "opcode.h"

#define FILE_READ(BUFFER, TYPE, SIZE, SOURCE) (fread((BUFFER), sizeof(TYPE), (SIZE), (SOURCE)) == (SIZE))
#define CURRENT_BYTE(STATE) ((STATE)->byte_code.code[(STATE)->byte_code.offset])
#define FORWARD(STATE, STEP) ((STATE)->byte_code.offset += (STEP))

char* read_byte_code_from_file(char* path, int* file_length)
{
	char *byte_code;
	FILE *file;

	*file_length = -1;
	file = fopen(path, "rb");

	if (file == NULL)
	{
		return NULL;
	}
	else
	{
		fseek(file, 0L, SEEK_END);
		*file_length = ftell(fd);
		fseek(file, 0L, SEEK_SET);

		byte_code = malloc(sizeof(Byte) * (*file_length));

		if (fread(byte_code, sizeof(char), *file_length, file) == *file_length)
		{
			fclose(file);
			return byte_code;
		}
		else
		{
			free(byte_code);
			fclose(file);
			return NULL;
		}
	}
}

char* parse_string(State* state)
{
	uint16_t len;
	char* str;

	len = parse_ushort(state);
	str = vm_alloc(state, sizeof(char) * (len + 1));
	if (expect_length(state, len))
	{
		memcpy(str, &(CURRENT_BYTE(state)), len);
		str[len] = '\0';
		FORWARD(state, len);
		return str;
	}
	else
	{
		fprintf(stderr, "cannot read string\n");
		vm_throw(state, VM_ERROR_READ_STRING);
		return NULL; // make the compiler happy
	}
}

Byte parse_byte(State * state)
{
	if (expect_length(state, 1))
	{
		Byte b;

		b = CURRENT_BYTE(state);
		FORWARD(state, 1);
		return b;
	}
	else
	{
		fprintf(stderr, "cannot read byte\n");
		vm_throw(state, VM_ERROR_READ_U8);
		return 0;
	}
}

// Big Endian
uint16_t parse_ushort(State* state)
{
	if (expect_length(2 * sizeof(Byte)))
	{
		Byte b1;
		Byte b2;

		b1 = CURRENT_BYTE(state);
		FORWARD(state, 1);
		b2 = CURRENT_BYTE(state);
		FORWARD(state, 1);

		return ((uint16_t)b1) * 256 + ((uint16_t)b2);
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
	int j;

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

	/* inheritance chain */

	class_info->n_super_class = parse_ushort(state);
	class_info->super_classes = vm_alloc(state, sizeof(uint16_t) * class_info->n_super_class);
	for (i = 0; i < class_info->n_super_class; i++)
	{
		class_info->super_classes[i] = parse_ushort(state);
	}

	//class_info->n_interface = parse_ushort(state);
	//class_info->interface_index_list = vm_alloc(state, sizeof(uint16_t) * class_info->n_interface);
	//class_info->interface_tables = vm_alloc(state, sizeof(VirtualTable) * class_info->n_interface);
	//for (i = 0; i < class_info->n_interface; i++)
	//{
	//	class_info->interface_index_list[i] = parse_ushort(state);
	//	class_info->interface_tables[i].n_methods = parse_ushort(state);
	//	class_info->interface_tables[i].methods = vm_alloc(state,
	//		sizeof(Function*) * class_info->interface_tables[i].n_methods);		
	//	class_info->interface_tables[i].class_index_list = vm_alloc(state,
	//			sizeof(uint16_t) * class_info->interface_tables[i].n_methods);
	//	class_info->interface_tables[i].method_index_list = vm_alloc(state,
	//		sizeof(uint16_t) * class_info->interface_tables[i].n_methods);
	//	for (j = 0; j < class_info->interface_tables[i].n_methods; j++)
	//	{
	//		class_info->interface_tables[i].class_index_list[j] = parse_ushort(state);
	//		class_info->interface_tables[i].method_index_list[j] = parse_ushort(state);
	//	}
	//}


	class_info->v_table.n_methods = parse_ushort(state);
	class_info->v_table.methods = vm_alloc(state, sizeof(MethodInfo*) * class_info->v_table.n_methods);
	for (i = 0; i < class_info->v_table.n_methods; i++)
	{
		class_info->v_table.methods[i] = parse_method(state);
		if (!(class_info->v_table.methods[i]->is_native_function))
		{
			class_info->v_table.methods[i]->u.func->n_constants = class_info->constant_pool.n_constants;
			class_info->v_table.methods[i]->u.func->constant_pool = class_info->constant_pool.constants;
		}
		printf("method: %s\n", class_info->v_table.methods[i]->name);
	}
}

//void parse_module(State* state, ModuleInfo* module_info)
//{
//	int i;
//
//	module_info->name = parse_string(state);
//	printf("module name: %s\n", module_info->name);
//	module_info->n_fields = parse_ushort(state);
//	module_info->field_names = vm_alloc(state, sizeof(char*) * module_info->n_fields);
//
//	for (i = 0; i < module_info->n_fields; i++)
//	{
//		module_info->field_names[i] = parse_string(state);
//		printf("field: %s\n", module_info->field_names[i]);
//	}
//
//	parse_constant_pool(state, &(module_info->constant_pool));
//	module_info->n_functions = parse_ushort(state);
//	module_info->functions = (MethodInfo**)vm_alloc(state, sizeof(MethodInfo*) * module_info->n_functions);
//	for (i = 0; i < module_info->n_functions; i++)
//	{
//		module_info->functions[i] = parse_function(state);
//		if (!IS_NATIVE_FUNCTION(module_info->functions[i]))
//		{
//			module_info->functions[i]->func.n_constants = module_info->constant_pool.n_constants;
//			module_info->functions[i]->func.constant_pool = module_info->constant_pool.constants;
//		}
//		printf("function: %s\n", FUNCTION_NAME(module_info->functions[i]));
//	}
//}

MethodInfo * parse_method(State* state)
{
	MethodInfo* function;

	function = vm_alloc(state, sizeof(MethodInfo));

	function->flag = parse_byte(state);
	function->name = parse_string(state);
	if (function->flag = METHOD_FLAG_NATIVE_FUNCTION)
	{
		function->native_method.is_loaded = false;
		function->native_method.function_pointer = NULL;
		function->args_size = parse_ushort(state);
		function->native_method.lib_path = parse_string(state);
		function->native_method.entry_point = parse_string(state);
		return function;
	}
	else
	{
		uint16_t code_length;

		function->args_size = parse_ushort(state);
		function->locals = parse_ushort(state);
		//printf("args_size = %d, locals = %d\n", function->u.func->args_size, function->u.func->locals);
		code_length = parse_ushort(state);
		function->code_length = code_length;
		//printf("function code len=%d\n", code_len);
		function->code = vm_alloc(state, sizeof(Byte) * code_length);
		if (expect(state, code_length))
		{
			memcpy(function->code, &(CURRENT_BYTE(state)), code_length);
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

void parse_constant_pool(State* state, ConstantPool* constant_pool)
{
	int i;

	constant_pool->n_constants = parse_ushort(state);
	printf("# of constants: %d\n", constant_pool->n_constants);
	constant_pool->constants = (Constant*)vm_alloc(state, sizeof(Constant) * constant_pool->n_constants);
	for (i = 0; i < constant_pool->n_constants; i++)
	{
		Byte type_tag;

		if (fread(&type_tag, sizeof(Byte), 1, state->source) == 1)
		{
			char* str;

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
				String* u32_str;

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
	MethodInfo* function;

	printf("class count: %d, module count: %d\n\n", exe->class_count, exe->module_count);
	for (i = 0; i < exe->class_count; i++)
	{
		printf("class: %s\n", exe->classes[i].name);
		for (j = 0; j < exe->classes[i].n_fields; j++)
		{
			printf("\tfield: %s\n", exe->classes[i].field_names[j]);
		}
		printf("\n");
		for (j = 0; j < exe->classes[i].v_table.n_methods; j++)
		{
			function = exe->classes[i].v_table.methods[j];
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

void view_function(MethodInfo * function, Executable* exe)
{
	int i;
	Byte op_code;
	const char* op_name;
	const char* op_type;
	uint32_t u32_v;
	uint32_t module_index;

	printf("\tmethod: %s\n", FUNCTION_NAME(function));
	if (IS_NATIVE_FUNCTION(function))
	{
		printf("\targs_size=%d\n", function->nf.args_size);
		printf("\tlibrary name: %s\n\tfunction name: %s\n\n", function->nf.lib_path, function->nf.func_name);
	}
	else
	{
		FunctionInfo* func = &(function->func);

		printf("\targs_size=%d, locals=%d\n", func->args_size, func->locals);

		printf("\tcode:\n");
		i = 0;
		while (i < func->code_len)
		{
			op_code = func->code[i];
			op_name = opcode_info[op_code][0];
			if (strcmp(op_name, "INVOKE_FUNCTION") == 0)
			{
				printf("\t\t%d: %s", i, op_name);
				i++;
				module_index = func->code[i];
				module_index = module_index * 256 + ((uint16_t)func->code[i + 1]);
				printf(" %s", exe->modules[module_index].name);
				i = i + 2;

				u32_v = func->code[i];
				u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
				printf(".%s", FUNCTION_NAME(exe->modules[module_index].functions[u32_v]));
				i = i + 2;
			}
			else if (strcmp(op_name, "NEW") == 0)
			{
				printf("\t\t%d: %s", i, op_name);
				i++;
				u32_v = func->code[i];
				u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
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
					printf(" %d", (int)func->code[i]);
					i++;
				}
				else if (strcmp(op_type, "u") == 0 || strcmp(op_type, "p") == 0)
				{
					u32_v = func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
					printf(" %d", u32_v);
					i = i + 2;
				}
				else if (strcmp(op_type, "uu") == 0)
				{
					u32_v = func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
					printf(" %d", u32_v);
					i = i + 2;

					u32_v = func->code[i];
					u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
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
