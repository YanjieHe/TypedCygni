import argparse
import json
import os
import sys
from string import Template
from cpp_generator_utils import *


def json_serializer_static_methods(json_data):
    json_serializer_def = """
    static json ${type}ToJson(const shared_ptr<${type}>& node); """
    json_serializer_def_list = []
    for exp_name in json_data["Expressions"]:
        exp_info = json_data["Expressions"][exp_name]
        class_name = exp_name + "Expression"
        json_serializer_def_list.append(
            Template(json_serializer_def).substitute({"type": class_name}))
    return "\n".join(json_serializer_def_list)


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

    if args.header:
        print(json_serializer_static_methods(json_data))
        with open(os.path.join("templates", "JsonSerializer_hpp.txt"), "r") as f:
            json_serializer_header = f.read()
        # print(Template(expression_header).substitute(
        #     {
        #         "statement_static_methods": statements_static_methods(json_data),
        #         "expression_static_methods": expression_static_methods(json_data),
        #         "expression_definitions": expressions_definition(json_data),
        #         "statement_definitions": statements_definition(json_data)
        #     }
        # ))
    else:
        with open(os.path.join("templates", "Expression_cpp.txt"), "r") as f:
            expression_source = f.read()
        pass
        # print(Template(expression_source).substitute(
        #     {
        #         "expression_static_methods_impls": expressions_implementations(json_data),
        #         "statement_static_methods_impls": statements_implementations(json_data)
        #     }
        # ))


if __name__ == "__main__":
    main()
