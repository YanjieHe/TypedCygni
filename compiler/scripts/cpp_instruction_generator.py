import os
import json
import argparse
from string import Template
from cpp_generator_utils import *


def instructions_definitions(json_data):
    class_def_template = """
    class $class_name: public Instruction {
    public:
        $fields

        $class_name($parameters): $initializers { }
        OpCode Operation() const override { return $op_code; }
        int Length() const override { return $length; }
    };
    """
    class_defs = []
    for instruction_name in json_data["Instructions"]:
        instruction_info = json_data["Instructions"][instruction_name]
        if len(instruction_info["OpCodes"]) > 1:
            op_code = "operation"
        elif len(instruction_info["OpCodes"]) == 1:
            if len(instruction_info["OpCodes"][0]["Type"]) > 1:
                op_code = "operation"
            else:
                op_code = "OpCode::" + \
                    instruction_info["OpCodes"][0]["Operation"]
        else:
            raise Exception("missing operation code")
        class_name = instruction_name + "Instruction"

        (fields, parameters, initializers) = generate_fields_and_constructor(
            instruction_info["Fields"])
        class_defs.append(Template(class_def_template).substitute(
            {
                "class_name": class_name,
                "fields": "\n".join(fields),
                "parameters": ", ".join(parameters),
                "initializers": ", ".join(initializers),
                "op_code": op_code,
                "length": instruction_info["Length"]
            }
        ))
    return "\n".join(class_defs)


def get_all_opcodes(json_data):
    opcode_list = []
    for instruction_name in json_data["Instructions"].keys():
        instruction_info = json_data["Instructions"][instruction_name]
        for opcode in instruction_info["OpCodes"]:
            if len(opcode["Type"]) > 0:
                for type in opcode["Type"]:
                    opcode_list.append(opcode["Operation"] + "_" + type)
            else:
                opcode_list.append(opcode["Operation"])
    full_opcode_list = []
    for opcode_info in json_data["OpCodes"]:
        if len(opcode_info["Type"]) > 0:
            for type in opcode_info["Type"]:
                full_opcode_list.append(opcode_info["Operation"] + "_" + type)
        else:
            full_opcode_list.append(opcode_info["Operation"])
    opcode_set = set(opcode_list)
    for opcode in full_opcode_list:
        if not (opcode in opcode_set):
            print(opcode)
    print(len(full_opcode_list))


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

    get_all_opcodes(json_data)
    return

    if args.header:
        with open(os.path.join("templates", "Instruction_hpp.txt"), "r") as f:
            expression_header = f.read()
        print(Template(expression_header).substitute(
            {
                "instructions_definitions": instructions_definitions(json_data),
                "emit_methods": ""
            }
        ))


if __name__ == "__main__":
    main()
