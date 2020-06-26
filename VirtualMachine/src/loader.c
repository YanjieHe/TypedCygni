#include "loader.h"
#include "opcode.h"
#include "unicode.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_READ(BUFFER, TYPE, SIZE, SOURCE)                                  \
  (fread((BUFFER), sizeof(TYPE), (SIZE), (SOURCE)) == (SIZE))
#define CURRENT_BYTE(STATE) ((STATE)->byte_code.code[(STATE)->byte_code.offset])
#define FORWARD(STATE, STEP) ((STATE)->byte_code.offset += (STEP))

char *read_byte_code_from_file(char *path, int *file_length) {
  char *byte_code;
  FILE *file;

  *file_length = -1;
  file = fopen(path, "rb");

  if (file == NULL) {
    return NULL;
  } else {
    fseek(file, 0L, SEEK_END);
    *file_length = ftell(file);
    fseek(file, 0L, SEEK_SET);

    byte_code = malloc(sizeof(Byte) * (*file_length));

    if (fread(byte_code, sizeof(char), *file_length, file) == *file_length) {
      fclose(file);
      return byte_code;
    } else {
      free(byte_code);
      fclose(file);
      return NULL;
    }
  }
}

char *parse_string(State *state) {
  uint16_t len;
  char *str;

  len = parse_ushort(state);
  str = vm_alloc(state, sizeof(char) * (len + 1));
  if (expect_length(state, len)) {
    memcpy(str, &(CURRENT_BYTE(state)), len);
    str[len] = '\0';
    FORWARD(state, len);
    return str;
  } else {
    fprintf(stderr, "cannot read string\n");
    vm_throw(state, VM_ERROR_READ_STRING);
    return NULL; // make the compiler happy
  }
}

Byte parse_byte(State *state) {
  if (expect_length(state, 1)) {
    Byte b;

    b = CURRENT_BYTE(state);
    FORWARD(state, 1);
    return b;
  } else {
    fprintf(stderr, "cannot read byte\n");
    vm_throw(state, VM_ERROR_READ_U8);
    return 0;
  }
}

// Big Endian
uint16_t parse_ushort(State *state) {
  if (expect_length(state, 2 * sizeof(Byte))) {
    Byte b1;
    Byte b2;

    b1 = CURRENT_BYTE(state);
    FORWARD(state, 1);
    b2 = CURRENT_BYTE(state);
    FORWARD(state, 1);

    return ((uint16_t)b1) * 256 + ((uint16_t)b2);
  } else {
    fprintf(stderr, "cannot read ushort\n");
    vm_throw(state, VM_ERROR_READ_U16);
    return 0; // make the compiler happy
  }
}

Executable *parse_file(State *state, const char *path) {
  uint16_t classes_count;
  char *main_function_name;
  int i;
  int j;
  Executable *exe;
  StaticFunctionMap *static_function_map_pair;
  StaticVarMap *static_var_map_pair;
  MethodMap *method_map_pair;
  ClassMap *class_map_pair;

  exe = vm_alloc(state, sizeof(Executable));
  exe->static_function_map = NULL;
  exe->static_var_map = NULL;
  exe->class_map = NULL;
  exe->method_map = NULL;
  classes_count = parse_ushort(state);
  main_function_name = parse_string(state);
  exe->classes_count = classes_count;
  exe->classes = vm_alloc(state, sizeof(ClassInfo) * classes_count);

  fprintf(stdout, "class count = %d\n", classes_count);
  fprintf(stdout, "main function = %s\n", main_function_name);

  for (i = 0; i < classes_count; i++) {
    ClassInfo *class_info;

    class_info = &(exe->classes[i]);
    parse_class(state, class_info);

    class_map_pair = malloc(sizeof(ClassMap));
    class_map_pair->name = class_info->name;
    class_map_pair->class_info = class_info;
    HASH_ADD_KEYPTR(hh, exe->class_map, class_info->name,
                    strlen(class_info->name), class_map_pair);

    for (j = 0; j < exe->classes[i].methods_count; j++) {
      MethodInfo *method = &(exe->classes[i].methods[j]);
      method_map_pair = malloc(sizeof(StaticFunctionMap));
      method_map_pair->name = method->name;
      method_map_pair->method_info = method;
      HASH_ADD_KEYPTR(hh, exe->method_map, method->name, strlen(method->name),
                      method_map_pair);
    }

    for (j = 0; j < exe->classes[i].static_funcs_size; j++) {
      MethodInfo *static_func = &(exe->classes[i].static_funcs[j]);
      static_function_map_pair = malloc(sizeof(StaticFunctionMap));
      static_function_map_pair->name = static_func->name;
      static_function_map_pair->function = static_func;
      HASH_ADD_KEYPTR(hh, exe->static_function_map, static_func->name,
                      strlen(static_func->name), static_function_map_pair);
    }

    for (j = 0; j < exe->classes[i].static_vars_size; j++) {
      StaticVarInfo *static_var = &(exe->classes[i].static_vars[j]);
      static_var_map_pair = malloc(sizeof(StaticFunctionMap));
      static_var_map_pair->name = static_var->name;
      static_var_map_pair->static_var = static_var;
      HASH_ADD_KEYPTR(hh, exe->static_var_map, static_var->name,
                      strlen(static_var->name), static_var_map_pair);
    }
  }

  HASH_FIND_STR(exe->static_function_map, main_function_name,
                static_function_map_pair);
  if (static_function_map_pair) {
    exe->entry = static_function_map_pair->function;
  } else {
    vm_throw(state, VM_ERROR_MISSING_MAIN_FUNCTION);
  }
  return exe;
  // fprintf(stderr, "fail to read file '%s'", path);
  // fclose(state->source);
  // state->source = NULL;
  // vm_throw(state, VM_ERROR_OPEN_FILE);
  // return NULL; // make the compiler happy
}

void parse_class(State *state, ClassInfo *class_info) {
  int i;
  int j;

  class_info->name = parse_string(state);
  class_info->fields_count = parse_ushort(state);
  class_info->methods_count = parse_ushort(state);
  class_info->static_vars_size = parse_ushort(state);
  class_info->static_funcs_size = parse_ushort(state);
  class_info->super_classes_count = parse_ushort(state);
  class_info->virtual_tables_count = parse_ushort(state);
  class_info->constant_pool.constant_pool_size = parse_ushort(state);

  fprintf(stdout, "class name: %s\n", class_info->name);
  fprintf(stdout, "fields count: %d\n", class_info->fields_count);
  fprintf(stdout, "methods count: %d\n", class_info->methods_count);
  fprintf(stdout, "static vars count: %d\n", class_info->static_vars_size);
  fprintf(stdout, "static funcs count: %d\n", class_info->static_funcs_size);
  fprintf(stdout, "super classes count: %d\n", class_info->super_classes_count);
  fprintf(stdout, "virtual table count: %d\n",
          class_info->virtual_tables_count);
  fprintf(stdout, "constant pool size: %d\n",
          class_info->constant_pool.constant_pool_size);

  class_info->fields =
      vm_alloc(state, sizeof(FieldInfo) * class_info->fields_count);
  class_info->methods =
      vm_alloc(state, sizeof(MethodInfo) * class_info->methods_count);
  class_info->static_vars =
      vm_alloc(state, sizeof(StaticVarInfo) * class_info->static_vars_size);
  class_info->static_funcs =
      vm_alloc(state, sizeof(MethodInfo) * class_info->static_funcs_size);
  class_info->super_classes =
      vm_alloc(state, sizeof(ClassRef) * class_info->super_classes_count);
  class_info->virtual_tables =
      vm_alloc(state, sizeof(VirtualTable) * class_info->virtual_tables_count);

  /* parse fields */
  fprintf(stdout, "\n\nfields:\n");
  for (i = 0; i < class_info->fields_count; i++) {
    class_info->fields[i].name = parse_string(state);
    printf("field: %s\n", class_info->fields[i].name);
  }

  // parse methods
  fprintf(stdout, "\n\nmethods:\n");
  for (i = 0; i < class_info->methods_count; i++) {
    parse_method(state, &(class_info->methods[i]), class_info);
  }

  // parse static vars
  fprintf(stdout, "\n\nstatic variables:\n");
  for (i = 0; i < class_info->static_vars_size; i++) {
    class_info->static_vars[i].name = parse_string(state);
    printf("static var: %s\n", class_info->static_vars[i].name);
  }

  // parse static funcs
  fprintf(stdout, "\n\nstatic functions:\n");
  for (i = 0; i < class_info->static_funcs_size; i++) {
    parse_method(state, &(class_info->static_funcs[i]), class_info);
  }

  /* inheritance chain */

  for (i = 0; i < class_info->super_classes_count; i++) {
    class_info->super_classes[i].name = parse_string(state);
    class_info->super_classes[i].class_info = NULL;
  }

  for (i = 0; i < class_info->virtual_tables_count; i++) {
    VirtualTable *vt = &(class_info->virtual_tables[i]);
    vt->class_ref.name = parse_string(state);
    vt->class_ref.class_info = NULL;
    vt->method_count = parse_ushort(state);
    vt->methods = vm_alloc(state, sizeof(MethodInfo) * vt->method_count);
    for (j = 0; j < vt->method_count; j++) {
      vt->methods[j].method_info = NULL;
      vt->methods[j].name = parse_string(state);
    }
  }

  fprintf(stdout, "\n\nconstant pool:\n");
  parse_constant_pool(state, &(class_info->constant_pool));
}

// void parse_module(State* state, ModuleInfo* module_info)
//{
//	int i;
//
//	module_info->name = parse_string(state);
//	printf("module name: %s\n", module_info->name);
//	module_info->n_fields = parse_ushort(state);
//	module_info->field_names = vm_alloc(state, sizeof(char*) *
// module_info->n_fields);
//
//	for (i = 0; i < module_info->n_fields; i++)
//	{
//		module_info->field_names[i] = parse_string(state);
//		printf("field: %s\n", module_info->field_names[i]);
//	}
//
//	parse_constant_pool(state, &(module_info->constant_pool));
//	module_info->n_functions = parse_ushort(state);
//	module_info->functions = (MethodInfo**)vm_alloc(state,
// sizeof(MethodInfo*) * module_info->n_functions); 	for (i = 0; i <
// module_info->n_functions; i++)
//	{
//		module_info->functions[i] = parse_function(state);
//		if (!IS_NATIVE_FUNCTION(module_info->functions[i]))
//		{
//			module_info->functions[i]->func.n_constants =
// module_info->constant_pool.n_constants;
//			module_info->functions[i]->func.constant_pool =
// module_info->constant_pool.constants;
//		}
//		printf("function: %s\n",
// FUNCTION_NAME(module_info->functions[i]));
//	}
//}

void parse_method(State *state, MethodInfo *function, ClassInfo *class_info) {

  function->flag = parse_byte(state);
  function->name = parse_string(state);

  fprintf(stdout, "function name: %s\n", function->name);
  fprintf(stdout, "function flag = %d\n", function->flag);
  fprintf(stdout, "is native function? %s\n",
          (function->flag == METHOD_FLAG_NATIVE_FUNCTION ? "true" : "false"));
  function->args_size = parse_ushort(state);
  function->locals = parse_ushort(state);
  function->need_stack_size = parse_ushort(state);
  function->code_length = parse_ushort(state);
  function->class_info = class_info;
  function->constant_pool = &(class_info->constant_pool);

  fprintf(stdout, "args size = %d\n", function->args_size);
  fprintf(stdout, "locals = %d\n", function->locals);
  fprintf(stdout, "need stack size = %d\n", function->need_stack_size);
  fprintf(stdout, "code length = %d\n", function->code_length);

  if (function->flag == METHOD_FLAG_NATIVE_FUNCTION) {
    function->native_method.function_pointer = NULL;
    function->native_method.lib_path = parse_string(state);
    function->native_method.entry_point = parse_string(state);
  } else {
    uint16_t code_length;

    code_length = function->code_length;
    function->code = vm_alloc(state, sizeof(Byte) * code_length);
    if (expect_length(state, code_length)) {
      memcpy(function->code, &(CURRENT_BYTE(state)), code_length);
      FORWARD(state, code_length);
    } else {
      fprintf(stderr, "fail to read byte code\n");
      vm_throw(state, VM_ERROR_READ_FUNCTION_BYTE_CODE);
    }
  }
}

void parse_constant_pool(State *state, ConstantPool *constant_pool) {
  int i;

  constant_pool->constants = (Constant *)vm_alloc(
      state, sizeof(Constant) * constant_pool->constant_pool_size);
  for (i = 0; i < constant_pool->constant_pool_size; i++) {
    ConstantFlag flag;
    char *str;
    Constant *constant;

    flag = parse_byte(state);
    printf("constant flag = %d\n", flag);
    str = parse_string(state);
    printf("constant: '%s'\n", str);
    constant = &(constant_pool->constants[i]);
    constant->flag = flag;
    if (flag == CONSTANT_FLAG_I32 || flag == CONSTANT_FLAG_BOOLEAN ||
        flag == CONSTANT_FLAG_CHAR) {
      constant->i32_v = atoi(str);
    } else if (flag == CONSTANT_FLAG_I64) {
      constant->i64_v = atol(str);
    } else if (flag == CONSTANT_FLAG_F32) {
      constant->f32_v = (float_t)atof(str);
    } else if (flag == CONSTANT_FLAG_F64) {
      constant->f64_v = (double_t)atof(str);
    } else if (flag == CONSTANT_FLAG_STRING) {
      String *u32_str;

      u32_str = vm_alloc(state, sizeof(String));
      u32_str->length = utf8_to_utf32_len(str);
      u32_str->characters = utf8_to_utf32(str, u32_str->length);
      constant->str_v = u32_str;
    } else if (flag == CONSTANT_FLAG_CLASS) {
      constant->class_ref = vm_alloc(state, sizeof(ClassRef));
      constant->class_ref->class_info = NULL;
      constant->class_ref->name = str;
    } else if (flag == CONSTANT_FLAG_METHOD ||
               flag == CONSTANT_FLAG_STATIC_FUNCTION) {
      constant->method_ref = vm_alloc(state, sizeof(MethodRef));
      constant->method_ref->method_info = NULL;
      constant->method_ref->name = str;
    } else if (flag == CONSTANT_FLAG_STATIC_VAR) {
      constant->static_var_ref = vm_alloc(state, sizeof(StaticVarRef));
      constant->static_var_ref->static_var = NULL;
      constant->static_var_ref->name = str;
    } else {
      fprintf(stderr, "wrong type tag\n");
      vm_throw(state, VM_ERROR_WRONG_TYPE_TAG);
    }
  }
}

void view_exe(Executable *exe) {
  int i;
  int j;

  printf("class count: %d\n", exe->classes_count);
  for (i = 0; i < exe->classes_count; i++) {
    ClassInfo *class_info;
    class_info = &(exe->classes[i]);

    printf("class: %s\n", class_info->name);
    for (j = 0; j < class_info->fields_count; j++) {
      printf("\tfield: %s\n", class_info->fields[j].name);
    }
    printf("\n");
    printf("constant pool:\n");
    for (j = 0; j < class_info->constant_pool.constant_pool_size; j++) {
      Constant *constant = &(class_info->constant_pool.constants[j]);
      if (constant->flag == CONSTANT_FLAG_I32) {
        printf("\t#%d: %d\n", j, constant->i32_v);
      } else if (constant->flag == CONSTANT_FLAG_I64) {
        printf("\t#%d: %d\n", j, constant->i64_v);
      } else if (constant->flag == CONSTANT_FLAG_F32) {
        printf("\t#%d: %f\n", j, constant->f32_v);
      } else if (constant->flag == CONSTANT_FLAG_F64) {
        printf("\t#%d: %f\n", j, constant->f64_v);
      } else if (constant->flag == CONSTANT_FLAG_CLASS) {
        printf("\t#%d: %s\n", j, constant->class_ref->name);
      } else if (constant->flag == CONSTANT_FLAG_STATIC_VAR) {
        printf("\t#%d: %s\n", j, constant->static_var_ref->name);
      } else if (constant->flag == CONSTANT_FLAG_STATIC_FUNCTION) {
        printf("\t#%d: %s\n", j, constant->method_ref->name);
      } else if (constant->flag == CONSTANT_FLAG_METHOD) {
        printf("\t#%d: %s\n", j, constant->method_ref->name);
      }
    }

    printf("\n");
    for (j = 0; j < class_info->static_vars_size; j++) {
      printf("\tstatic variable: %s\n", class_info->static_vars[j].name);
    }
    printf("\n");
    for (j = 0; j < class_info->methods_count; j++) {
      MethodInfo *method;
      method = &(class_info->methods[j]);
      view_function(method, exe);
    }
    printf("\n");
    for (j = 0; j < class_info->static_funcs_size; j++) {
      MethodInfo *function;
      function = &(class_info->static_funcs[j]);
      view_function(function, exe);
    }
    printf("\n");

    for (j = 0; j < class_info->virtual_tables_count; j++) {
      VirtualTable *vt = &(class_info->virtual_tables[j]);
      int k;
      printf("\tsuper class: %s\n", vt->class_ref.name);
      for (k = 0; k < vt->method_count; k++) {
        printf("\t\tvirtual method: %s\n", vt->methods[k].name);
      }
    }
  }
}

void view_function(MethodInfo *function, Executable *exe) {
  int i;
  Byte op_code;
  const char *op_name;
  const char *op_type;
  uint32_t u32_v;
  uint32_t module_index;

  printf("\tmethod: %s\n", function->name);
  if (function->flag == METHOD_FLAG_NATIVE_FUNCTION) {
    printf("\targs_size=%d\n", function->args_size);
    printf("\tlibrary name: %s\n\tfunction name: %s\n\n",
           function->native_method.lib_path, function->name);
  } else {
    printf("\targs_size=%d, locals=%d\n", function->args_size,
           function->locals);

    printf("\tcode:\n");
    i = 0;
    while (i < function->code_length) {
      op_code = function->code[i];
      op_name = opcode_info[op_code][0];
      //   if (strcmp(op_name, "INVOKE_FUNCTION") == 0) {
      //     printf("\t\t%d: %s", i, op_name);
      //     i++;
      //     module_index = function->code[i];
      //     module_index = module_index * 256 + ((uint16_t)func->code[i + 1]);
      //     printf(" %s", exe->modules[module_index].name);
      //     i = i + 2;

      //     u32_v = func->code[i];
      //     u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
      //     printf(".%s",
      //            FUNCTION_NAME(exe->modules[module_index].functions[u32_v]));
      //     i = i + 2;
      //   } else if (strcmp(op_name, "NEW") == 0) {
      //     printf("\t\t%d: %s", i, op_name);
      //     i++;
      //     u32_v = func->code[i];
      //     u32_v = u32_v * 256 + ((uint16_t)func->code[i + 1]);
      //     printf(" %s", exe->classes[u32_v].name);
      //     i = i + 2;
      //   } else {
      printf("\t\t%d: %s", i, op_name);
      i++;
      op_type = opcode_info[op_code][1];
      if (strcmp(op_type, "b") == 0) {
        printf(" %d", (int)function->code[i]);
        i++;
      } else if (strcmp(op_type, "u") == 0 || strcmp(op_type, "p") == 0) {
        u32_v = function->code[i];
        u32_v = u32_v * 256 + ((uint16_t)function->code[i + 1]);
        printf(" %d", u32_v);
        i = i + 2;
      } else if (strcmp(op_type, "uu") == 0) {
        u32_v = function->code[i];
        u32_v = u32_v * 256 + ((uint16_t)function->code[i + 1]);
        printf(" %d", u32_v);
        i = i + 2;

        u32_v = function->code[i];
        u32_v = u32_v * 256 + ((uint16_t)function->code[i + 1]);
        printf(" %d", u32_v);
        i = i + 2;
      }
      printf("\n");
    }
    // }

    printf("\n");
  }
}

void view_type_tag(State *state, Byte tag) {
  if (tag == TYPE_I32) {
    printf(" [I32]");
  } else if (tag == TYPE_I64) {
    printf(" [I64]");
  } else if (tag == TYPE_F32) {
    printf(" [F32]");
  } else if (tag == TYPE_F64) {
    printf(" [F64]");
  } else if (tag == TYPE_STRING) {
    printf(" [STRING]");
  } else if (tag == TYPE_OBJECT) {
    printf(" [OBJECT]");
  } else {
    fprintf(stderr, "error type tag\n");
    vm_throw(state, -1);
  }
}
