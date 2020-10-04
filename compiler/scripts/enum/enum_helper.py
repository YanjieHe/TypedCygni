import os
import CppHeaderParser
import sys
from CppHeaderParser import CppHeader
from typing import List, Dict
from string import Template


def get_cpp_header_info(input_file_path: str) -> CppHeader:
    return CppHeaderParser.CppHeader(input_file_path)


def fill(text: str, key_value_pairs: Dict[str, str]) -> str:
    return Template(text).substitute(key_value_pairs)


def enum_definitions(cppHeader: CppHeader, is_enum_class: bool) -> List[str]:
    def enum_name(name: str) -> str:
        if is_enum_class:
            return fill("enum class $name {",
                        {"name": name})
        else:
            return fill("enum $name {",
                        {"name": name})
    lines = []
    for enum in cppHeader.enums:
        lines.append(enum_name(enum["name"]))
        n = len(enum["values"])
        for index, value in enumerate(enum["values"]):
            if index == n - 1:
                lines.append(value["name"] + "=" + str(value["value"]))
            else:
                lines.append(value["name"] + "=" + str(value["value"]) + ",")
        lines.append("};\n")
    return lines


def enum_to_string(cppHeader, is_enum_class):
    func_def = """
    template <> class Enum<$enum_type> {
        public:
        static std::string ToString($enum_type value) {
            switch (value) {"""
    lines = []
    for enum in cppHeader.enums:
        lines.append(fill(func_def, {"enum_type": enum["name"]}))
        for value in enum["values"]:
            if is_enum_class:
                lines.append("case {0}::{1}: return \"{0}::{1}\";".format(
                    enum["name"], value["name"]
                ))
            else:
                lines.append("case {0}: return \"{0}\";".format(value["name"]))
        lines.append(
            "default: return \"ENUMERATION VALUE OUT OF BOUND\"; }}};")
    return lines


def enum_generic_to_string():
    return """
    template <typename TEnum> class Enum {
public:
  static std::string ToString(TEnum) {
    return "ENUMERATION VALUE OUT OF BOUND";
  }
};"""


def main():
    is_enum_class = True
    input_file_path = "Enum_Template.cpp"
    cppHeader = get_cpp_header_info(input_file_path)
    print("#ifndef ENUM_HPP")
    print("#define ENUM_HPP")
    for header in cppHeader.includes:
        print("#include {0}".format(header))
    print()
    lines = enum_definitions(cppHeader, is_enum_class)
    print("\n".join(lines))
    print(enum_generic_to_string())
    lines = enum_to_string(cppHeader, is_enum_class)
    print("\n".join(lines))
    print("\n#endif // ENUM_HPP")


# python enum_helper.py | clang-format.exe > Enum.hpp
# python .\enum_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "Enum.hpp"
main()
