import os


class OpCode:
    def __init__(self, name, has_type, op_type):
        self.name = name
        self.has_type = has_type
        self.op_type = op_type
    

    def __repr__(self):
        text = ""
        if self.has_type:
            text = self.name + "[T]"
        else:
            text = self.name
        if self.op_type == "":
            return text
        else:
            return text + ":" + self.op_type


type_names = ["I32", "I64", "F32", "F64", "STRING", "OBJECT"]

def read_txt(path):
    lines = []
    with open(path, "r") as f:
        for line in f:
            if len(line.strip()) != 0:
                lines.append(line.strip())
    return lines


def parse_opcode(lines):
    result = []
    for line in lines:
        if line.find(':') >= 0:
            items = line.split(':')
            op_name = items[0]
            op_type = items[1]
        else:
            op_name = line
            op_type = ""
        N = len("[T]")
        if len(line) > N and op_name[-N:] == "[T]":
            result.append(OpCode(op_name[:-N], True, op_type))
        else:
            result.append(OpCode(op_name, False, op_type))
    return result


def expand(opcode_list):
    result = []
    for opcode in opcode_list:
        if opcode.has_type:
            for type_name in type_names:
                result.append(opcode.name + "_" + type_name)
        else:
            result.append(opcode.name)
    return result


def add_casts(opcode_list):
    result = opcode_list.copy()
    basic_types = ["I32", "I64", "F32", "F64"]
    for i in range(len(basic_types)):
        for j in range(i + 1, len(basic_types)):
            type_1 = basic_types[i]
            type_2 = basic_types[j]
            result.append("CAST_{0}_TO_{1}".format(type_1, type_2))
            result.append("CAST_{0}_TO_{1}".format(type_2, type_1))
    return result


def make_enum(opcode_list):
    lines = []
    for (i, opcode) in enumerate(opcode_list):
        lines.append("{0} = {1}".format(opcode, i))
    return "enum OpCode {{\n{0}\n}};".format(",\n".join(lines))


def make_opcode_info_array(opcode_info_list):
    lines = []
    for opcode in opcode_info_list:
        lines.append('{{ "{0}", {1} }}'.format(opcode.name, opcode))

opcode_info_list = parse_opcode(read_txt("opcode.txt"))

opcode_list = add_casts(expand(opcode_info_list))

opcode_list.sort()

for opcode in opcode_list:
    print(opcode)

print(make_enum(opcode_list))