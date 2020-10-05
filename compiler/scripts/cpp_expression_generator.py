import argparse
import json
import os
import sys
from string import Template
from cpp_generator_utils import *

# import CppHeaderParser


# def get_cpp_header_info(input_file_path):
#     try:
#         cppHeader = CppHeaderParser.CppHeader(input_file_path)
#         return cppHeader
#     except CppHeaderParser.CppParseError as e:
#         print(e)
#         sys.exit(1)


# def expression_def(class_name, cppHeader, node_type):
#     class_template = """
#     class $class_name: public Expression {
#     public:
#         $fields
#         $class_name($parameters): $initializers { }
#         Position Pos() const override { return pos; }
#         ExpressionType NodeType() const override { return $node_type; }
#     }};
#     """
#     field_defs = []
#     field_args = []
#     field_initializers = []
#     for field in cppHeader.classes[class_name]["properties"]["public"]:
#         field_defs.append("{0} {1};".format(field["type"], field["name"]))
#         field_args.append("{0} {1}".format(field["type"], field["name"]))
#         field_initializers.append("{0}{{{0}}}".format(field["name"]))
#     return class_template.format(
#         class_name,
#         "\n".join(field_defs),
#         ",".join(field_args),
#         ",".join(field_initializers),
#         node_type
#     )


# def statement_def(class_name, cppHeader, statement_type):
#     class_template = """
#     class {0}: public Statement {{
#     public:
#         {1}
#         {0}({2}): {3} {{}}
#         Position Pos() const override {{ return pos; }}
#         StatementType GetStatementType() const override {{ return {4}; }}
#     }};
#     """
#     field_defs = []
#     field_args = []
#     field_initializers = []
#     for field in cppHeader.classes[class_name]["properties"]["public"]:
#         field_defs.append("{0} {1};".format(field["type"], field["name"]))
#         field_args.append("{0} {1}".format(field["type"], field["name"]))
#         field_initializers.append("{0}{{{0}}}".format(field["name"]))
#     return class_template.format(
#         class_name,
#         "\n".join(field_defs),
#         ",".join(field_args),
#         ",".join(field_initializers),
#         statement_type
#     )


# def generic_visitor(json_obj):
#     visitor_code = """
#     template <typename ExpReturnType, typename StatementReturnType,
#           typename... ArgTypes>
#     class Visitor {{
#     public:
#     virtual ExpReturnType VisitExpression(Expression* node, ArgTypes... args) {{
#         switch (node->NodeType()) {{
#             {0}
#         default:
#         throw Error(node->Pos(), "unsupported node type for visitor");
#         }}
#     }}
#     virtual StatementReturnType VisitStatement(Statement* statement,
#                                                 ArgTypes... args) {{
#         switch (statement->GetStatementType()) {{
#             {1}
#         default:{{
#         throw Error(node->Pos(), "unsupported statement type for visitor");
#         }}
#         }}
#     }}
#     {2}
#     virtual StatementReturnType VisitExpStatement(Expression *node,
#                                                 ArgTypes... args) = 0;
#     }};"""
#     exp_lines = []
#     stmt_lines = []
#     func_lines = []
#     for exp_name in json_obj["Expressions"]:
#         for enum_name in json_obj["Expressions"][exp_name]:
#             exp_lines.append(
#                 "case ExpressionType::{0}: {{ return Visit{1}(dynamic_cast<{2}*>(node), args...); }}"
#                 .format(
#                     enum_name, exp_name, exp_name + "Expression"
#                 ))
#         func_lines.append(
#             "virtual ExpReturnType Visit{0}({1}* node, ArgTypes... args) = 0;"
#             .format(
#                 exp_name, exp_name + "Expression"
#             )
#         )
#     for stmt_name in json_obj["Statements"]:
#         for enum_name in json_obj["Statements"][stmt_name]:
#             stmt_lines.append(
#                 "case StatementType::{0}: {{ return Visit{1}(dynamic_cast<{2}*>(node), args...); }}"
#                 .format(
#                     enum_name, stmt_name, stmt_name + "Statement"
#                 ))
#         func_lines.append(
#             "virtual StatementReturnType Visit{0}({1}* node, ArgTypes... args) = 0;"
#             .format(
#                 stmt_name, stmt_name + "Statement"
#             )
#         )
#     return visitor_code.format(
#         "\n".join(exp_lines),
#         "\n".join(stmt_lines),
#         "\n".join(func_lines)
#     )


# def generate_visitor(json_obj, visitor_name, exp_ret_type, stmt_ret_type, args_type, arg_names):
#     visitor_code = """
#     template <>
#     class {0}<{1}, {2}{3}> {{
#     public:
#     {4}
#     }};
#     """
#     func_lines = []
#     for exp_name in json_obj["Expressions"]:
#         func_lines.append(
#             "{0} Visit{1}({2}* node{3}) override {{}}"
#             .format(
#                 exp_ret_type, exp_name, exp_name + "Expression", arg_names
#             )
#         )
#     for stmt_name in json_obj["Statements"]:
#         func_lines.append(
#             "{0} Visit{1}({2}* node{3}) override {{}}"
#             .format(
#                 stmt_ret_type, stmt_name, stmt_name + "Statement", arg_names
#             )
#         )
#     return visitor_code.format(
#         visitor_name, exp_ret_type, stmt_ret_type, args_type,
#         "\n".join(func_lines)
#     )


def expressions_definition(json_data):
    class_def_template = """
    class $class_name: public Expression {
    public:
        $fields

        $class_name($parameters): $initializers { }
        Position Pos() const override { return pos; }
        ExpressionType NodeType() const override { return $node_type; }
    };
    """
    class_defs = []
    for exp_name in json_data["Expressions"]:
        exp_info = json_data["Expressions"][exp_name]
        if len(exp_info["Node Type"]) > 1:
            node_type = "nodeType"
        elif len(exp_info["Node Type"]) == 1:
            node_type = "ExpressionType::" + exp_info["Node Type"][0]
        else:
            raise Exception("missing node type")
        class_name = exp_name + "Expression"
        (fields, parameters, initializers) = generate_fields_and_constructor(
            exp_info["Fields"])
        class_defs.append(Template(class_def_template).substitute(
            {"class_name": class_name,
             "fields": "\n".join(fields),
             "parameters": ", ".join(parameters),
             "initializers": ", ".join(initializers),
             "node_type": node_type}
        ))
    return "\n".join(class_defs)


def statements_definition(json_data):
    class_def_template = """
    class $class_name: public Statement {
    public:
        $fields

        $class_name($parameters): $initializers { }
        Position Pos() const override { return pos; }
        StatementType GetStatementType() const override { return $statement_type; }
    };
    """
    class_defs = []
    for statement_name in json_data["Statements"]:
        statement_info = json_data["Statements"][statement_name]
        if len(statement_info["Statement Type"]) > 1:
            node_type = "statementType"
        elif len(statement_info["Statement Type"]) == 1:
            node_type = "StatementType::" + statement_info["Statement Type"][0]
        else:
            raise Exception("missing node type")
        class_name = statement_name + "Statement"
        (fields, parameters, initializers) = generate_fields_and_constructor(
            statement_info["Fields"])
        class_defs.append(Template(class_def_template).substitute(
            {"class_name": class_name,
             "fields": "\n".join(fields),
             "parameters": ", ".join(parameters),
             "initializers": ", ".join(initializers),
             "statement_type": node_type}
        ))
    return "\n".join(class_defs)


def expression_static_methods(json_data):
    def upper_case_to_camel(name):
        items = name.split('_')
        result = []
        for item in items:
            result.append(item[0] + item[1:].lower())
        return "".join(result)

    func_def = """static $return_type $func_name($parameters);
    """
    func_list = []
    for exp_name in json_data["Expressions"]:
        exp_info = json_data["Expressions"][exp_name]
        class_name = exp_name + "Expression"
        parameters = []
        for field_type, field_name in exp_info["Fields"]:
            parameters.append(field_type + " " + field_name)
        func_list.append(Template(func_def).substitute(
            {
                "return_type": "shared_ptr<" + class_name + ">",
                "func_name": exp_name,
                "parameters": ", ".join(parameters)
            }
        ))
    return "\n".join(func_list)


def statements_static_methods(json_data):
    func_def = """static shared_ptr<$class_name> $func_name($parameters);
    """
    func_list = []
    for exp_name in json_data["Statements"]:
        exp_info = json_data["Statements"][exp_name]
        class_name = exp_name + "Statement"
        parameters = []
        for field_type, field_name in exp_info["Fields"]:
            parameters.append(field_type + " " + field_name)
        func_list.append(Template(func_def).substitute(
            {
                "class_name": class_name,
                "func_name": exp_name,
                "parameters": ", ".join(parameters)
            }
        ))
    return "\n".join(func_list)


def expressions_implementations(json_data):
    func_def = """
    static shared_ptr<$class_name> $func_name($parameters) {
        return make_shared<$class_name>($arguments);
    }"""
    func_list = []
    for exp_name in json_data["Expressions"]:
        exp_info = json_data["Expressions"][exp_name]
        class_name = exp_name + "Expression"
        parameters = []
        arguments = []
        for field_type, field_name in exp_info["Fields"]:
            parameters.append(field_type + " " + field_name)
            arguments.append(field_name)
        func_list.append(Template(func_def).substitute(
            {
                "class_name": class_name,
                "func_name": exp_name,
                "parameters": ", ".join(parameters),
                "arguments": ", ".join(arguments)
            }
        ))
    return "\n".join(func_list)


def statements_implementations(json_data):
    func_def = """
    static shared_ptr<$class_name> $func_name($parameters) {
        return make_shared<$class_name>($arguments);
    }"""
    func_list = []
    for statement_name in json_data["Statements"]:
        statement_info = json_data["Statements"][statement_name]
        class_name = statement_name + "Statement"
        parameters = []
        arguments = []
        for field_type, field_name in statement_info["Fields"]:
            parameters.append(field_type + " " + field_name)
            arguments.append(field_name)
        func_list.append(Template(func_def).substitute(
            {
                "class_name": class_name,
                "func_name": statement_name,
                "parameters": ", ".join(parameters),
                "arguments": ", ".join(arguments)
            }
        ))
    return "\n".join(func_list)


def main():
    # parse command line arguments
    my_parser = argparse.ArgumentParser()
    my_parser.add_argument(
        "--path", type=str, help="the input json formatted template for expressions.")
    my_parser.add_argument(
        "--header", type=bool, help="if adding this flag, then print header file. Otherwise, print implementation.")
    args = my_parser.parse_args()

    with open(args.path, "r") as f:
        json_obj = json.load(f)

    if args.header:
        with open(os.path.join("templates", "Expression_hpp.txt"), "r") as f:
            expression_header = f.read()
        print(Template(expression_header).substitute(
            {
                "statement_static_methods": statements_static_methods(json_obj),
                "expression_static_methods": expression_static_methods(json_obj),
                "expression_definitions": expressions_definition(json_obj),
                "statement_definitions": statements_definition(json_obj)
            }
        ))
    else:
        with open(os.path.join("templates", "Expression_cpp.txt"), "r") as f:
            expression_source = f.read()
        print(Template(expression_source).substitute(
            {
                "expression_static_methods_impls": expressions_implementations(json_obj),
                "statement_static_methods_impls": statements_implementations(json_obj)
            }
        ))


if __name__ == "__main__":
    main()
