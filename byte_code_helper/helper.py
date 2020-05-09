import pandas as pd
import numpy as np

df = pd.read_csv("opcode.csv")
df["op_type"].replace(np.nan, "", inplace=True)
df["with_types"].replace(np.nan, "", inplace=True)


def expand(df):
    result = []
    for (_, op_name, op_type, with_types) in df.itertuples():
        if with_types != "":
            types = with_types.split(',')
            for type_name in types:
                result.append((op_name + "_" + type_name, op_type))
        else:
            result.append((op_name, op_type))
    return result


def make_enum(opcode_list):
    lines = []
    for (i, (op_name, op_type)) in enumerate(opcode_list):
        lines.append("{0} = {1}".format(op_name, i))
    return "enum OpCode {{\n{0}\n}};".format(",\n".join(lines))


def make_opcode_info_array(op_list):
    lines = []
    for (op_name, op_type) in op_list:
        lines.append('{{ "{0}", "{1}" }}'.format(op_name, op_type))
    return "static const char* opcode_info [] {{\n{0}\n}};".format(",\n".join(lines))


op_list = expand(df)
print(make_enum(op_list))
print(make_opcode_info_array(op_list))

# opcode_list = add_casts(expand(parse_opcode(read_txt("opcode.txt"))))

# opcode_list = sorted(opcode_list, key=lambda x: x.name)

# print(make_enum(opcode_list))

# print(make_opcode_info_array(opcode_list))