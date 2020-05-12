#include "Parser.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* parse_string(FILE* file)
{
	uint16_t len;
	char* str;

	len = parse_ushort(file);
	str = (char*)malloc(sizeof(char) * (len + 1));
	if (fread(str, sizeof(char), len, file) == len)
	{
		str[len] = '\0';
		return str;
	}
	else
	{
		fprintf(stderr, "cannot read string\n");
		free(str);
		exit(-1);
	}
}

// Big Endian
uint16_t parse_ushort(FILE* file)
{
	uint16_t value;
	uint8_t b1;
	uint8_t b2;
	size_t res1;
	size_t res2;

	res1 = fread(&b1, sizeof(uint8_t), 1, file);
	res2 = fread(&b2, sizeof(uint8_t), 1, file);

	if (res1 == 1 && res2 == 1)
	{
		value = ((uint16_t)b1) * 256 + ((uint16_t)b2);
		return value;
	}
	else
	{
		fprintf(stderr, "cannot read ushort\n");
		exit(-1);
	}
}

Executable* parse_file(const char* path)
{
	FILE* file;
	uint16_t class_count;
	uint16_t module_count;
	uint16_t main_func_module_index;
	uint16_t main_func_index;
	int i;
	Executable* exe;

	file = fopen(path, "rb");

	if (file)
	{
		exe = (Executable*)malloc(sizeof(Executable));
		main_func_module_index = parse_ushort(file);
		main_func_index = parse_ushort(file);
		class_count = parse_ushort(file);
		module_count = parse_ushort(file);
		exe->class_count = class_count;
		exe->module_count = module_count;
		exe->classes = (ClassInfo*)malloc(sizeof(ClassInfo) * class_count);
		exe->modules = (ModuleInfo*)malloc(sizeof(ModuleInfo) * module_count);

		printf("main function module: %d, main function: %d\n",
			main_func_module_index, main_func_index);
		printf("class count = %d, module count = %d\n", class_count, module_count);
		for (i = 0; i < class_count; i++)
		{
			parse_class(file, &(exe->classes[i]));
		}
		for (i = 0; i < module_count; i++)
		{
			parse_module(file, &(exe->modules[i]));
		}

		exe->entry = exe->modules[main_func_module_index].functions[main_func_index];
		return exe;
	}
	else
	{
		fprintf(stderr, "fail to read file '%s'", path);
		exit(-1);
	}
}

void parse_class(FILE* file, ClassInfo* class_info)
{
	char* class_name;
	uint16_t field_count;
	uint16_t method_count;
	char** field_names;
	Function** methods;
	int i;

	class_name = parse_string(file);
	printf("class name: %s\n", class_name);
	field_count = parse_ushort(file);
	field_names = (char**)malloc(sizeof(char*)*field_count);

	for (i = 0; i < field_count; i++)
	{
		field_names[i] = parse_string(file);
		printf("field: %s\n", field_names[i]);
	}

	method_count = parse_ushort(file);
	methods = (Function**)malloc(sizeof(Function*)*method_count);
	for (i = 0; i < method_count; i++)
	{
		methods[i] = parse_function(file);
		printf("method: %s\n", methods[i]->name);
	}

	class_info->name = class_name;
	class_info->n_fields = field_count;
	class_info->field_names = field_names;
	class_info->n_methods = method_count;
	class_info->methods = methods;
}

void parse_module(FILE* file, ModuleInfo* module_info)
{
	char* module_name;
	uint16_t field_count;
	uint16_t function_count;
	char** field_names;
	Function** functions;
	int i;

	module_name = parse_string(file);
	printf("module name: %s\n", module_name);
	field_count = parse_ushort(file);
	field_names = (char**)malloc(sizeof(char*)*field_count);

	for (i = 0; i < field_count; i++)
	{
		field_names[i] = parse_string(file);
		printf("field: %s\n", field_names[i]);
	}

	function_count = parse_ushort(file);
	functions = (Function**)malloc(sizeof(Function*)*function_count);
	for (i = 0; i < function_count; i++)
	{
		functions[i] = parse_function(file);
		printf("function: %s\n", functions[i]->name);
	}

	module_info->name = module_name;
	module_info->n_fields = field_count;
	module_info->field_names = field_names;
	module_info->n_functions = function_count;
	module_info->functions = functions;

}

Function * parse_function(FILE * file)
{
	Function* function;
	uint16_t code_len;

	function = (Function*)malloc(sizeof(Function));
	function->name = parse_string(file);
	function->u.func_info = (FunctionInfo*)malloc(sizeof(FunctionInfo));
	function->u.func_info->n_parameters = parse_ushort(file);
	function->u.func_info->locals = parse_ushort(file);
	code_len = parse_ushort(file);
	function->u.func_info->code_len = code_len;
	printf("function code len=%d\n", code_len);
	function->u.func_info->code = (uint8_t*)malloc(sizeof(uint8_t) * code_len);
	if (fread(function->u.func_info->code, sizeof(uint8_t), code_len, file) == code_len)
	{
		return function;
	}
	else
	{
		fprintf(stderr, "fail to read byte code\n");
		free(function->name);
		free(function->u.func_info->code);
		free(function);
		exit(-1);
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
			view_function(function);
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
			view_function(function);
		}
		printf("\n");
	}
}

void view_function(Function * function)
{
	int i;
	uint8_t op_code;
	const char* op_name;
	const char* op_type;
	uint32_t u32_v;

	printf("\tmethod: %s\n", function->name);
	printf("\targs_size=%d, locals=%d\n", function->u.func_info->n_parameters, function->u.func_info->locals);

	printf("\tcode:\n");
	i = 0;
	while (i < function->u.func_info->code_len)
	{
		op_code = function->u.func_info->code[i];
		op_name = opcode_info[op_code][0];
		printf("\t\t%d: %s", i, op_name);
		i++;
		op_type = opcode_info[op_code][1];
		if (strcmp(op_type, "b") == 0)
		{
			printf(" %d", (int)function->u.func_info->code[i]);
			i++;
		}
		else if (strcmp(op_type, "u") == 0 || strcmp(op_type, "p") == 0)
		{
			u32_v = function->u.func_info->code[i];
			u32_v = u32_v * 256 + ((uint16_t)function->u.func_info->code[i + 1]);
			printf(" %d", u32_v);
			i = i + 2;
		}
		else if (strcmp(op_type, "uu") == 0)
		{
			u32_v = function->u.func_info->code[i];
			u32_v = u32_v * 256 + ((uint16_t)function->u.func_info->code[i + 1]);
			printf(" %d", u32_v);
			i = i + 2;

			u32_v = function->u.func_info->code[i];
			u32_v = u32_v * 256 + ((uint16_t)function->u.func_info->code[i + 1]);
			printf(" %d", u32_v);
			i = i + 2;
		}
		printf("\n");
	}

	printf("\n");
}

void view_type_tag(uint8_t tag)
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
		exit(-1);
	}
}
