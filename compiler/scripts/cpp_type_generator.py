import os
import argparse
import json
from string import Template
from cpp_generator_utils import *


def create_binary():
    lines = []
    first = True
    for op in ["ADD", "SUBTRACT", "MULTIPLY", "DIVIDE"]:
        for basic_type in ["Int", "Float", "Long", "Double"]:
            if first:
                else_text = ""
                first = False
            else:
                else_text = "else "
            lines.append("""{0}if (node->NodeType() == ExpressionType::{1} &&
            left->Equals(Type::{2}()) && right->Equals(Type::{2}())) {{
                return Type::{3}();
            }}""".format(else_text, op, basic_type, basic_type))
    for op in ["GT", "LT", "GE", "LE", "EQ", "NE"]:
        for basic_type in ["INT", "FLOAT", "LONG", "DOUBLE", "CHAR", "BOOLEAN"]:
            if first:
                else_text = ""
                first = False
            else:
                else_text = "else "
            lines.append("""{0}if (node->NodeType() == ExpressionType::{1} &&
                left->Equals(Type::{2}()) && right->Equals(Type::{2}())) {{
                    return Type::Boolean();
                }}""".format(else_text, op, basic_type))
    lines.append(
        "else { throw Error(node->Pos(), \"binary exp: error type\"); }")
    return "\n".join(lines)


def types_definition(json_data):
    class_def_template = """
    class $class_name: Type {
    public:
        $fields

        $class_name($parameters): $initializers { }
        TypeCode GetTypeCode() const override { return $type_code; }
    };
    """
    class_defs = []
    for type_name in json_data["Types"]:
        type_info = json_data["Types"][type_name]
        if len(type_info["Type Code"]) > 1:
            type_code = "typeCode"
        elif len(type_info["Type Code"]) == 1:
            type_code = "TypeCode::" + type_info["Type Code"][0]
        else:
            raise Exception("missing type code")
        class_name = type_name + "Type"
        (fields, parameters, initializers) = generate_fields_and_constructor(
            type_info["Fields"])
        class_defs.append(Template(class_def_template).substitute(
            {"class_name": class_name,
             "fields": "\n".join(fields),
             "parameters": ", ".join(parameters),
             "initializers": ", ".join(initializers),
             "type_code": type_code}
        ))
    return "\n".join(class_defs)


def values_definition(json_data):
    class_def_template = """
    class $class_name: Value {
    public:
        $fields

        $class_name($parameters): $initializers { }
        TypeCode GetTypeCode() const override { return $type_code; }
    };
    """
    class_defs = []
    for type_name in json_data["Values"]:
        type_info = json_data["Values"][type_name]
        if len(type_info["Type Code"]) > 1:
            type_code = "typeCode"
        elif len(type_info["Type Code"]) == 1:
            type_code = "TypeCode::" + type_info["Type Code"][0]
        else:
            raise Exception("missing type code")
        class_name = type_name + "Value"
        (fields, parameters, initializers) = generate_fields_and_constructor(
            type_info["Fields"])
        class_defs.append(Template(class_def_template).substitute(
            {"class_name": class_name,
             "fields": "\n".join(fields),
             "parameters": ", ".join(parameters),
             "initializers": ", ".join(initializers),
             "type_code": type_code}
        ))
    return "\n".join(class_defs)


def main():

    # parse command line arguments
    my_parser = argparse.ArgumentParser()
    my_parser.add_argument(
        "--path", type=str, help="the input json formatted template for expressions.")
    my_parser.add_argument(
        "--header", action="store_true", help="if adding this flag, then print header file. Otherwise, print implementation.")
    args = my_parser.parse_args()

    with open(args.path, "r") as f:
        json_data = json.load(f)
    with open(os.path.join("templates", "Type_hpp.txt"), "r") as f:
        type_header = f.read()
        print(Template(type_header).substitute(
            {
                "types_definitions": types_definition(json_data),
                "values_definitions": values_definition(json_data)
            }
        ))


# python .\type_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM Type.hpp
main()
