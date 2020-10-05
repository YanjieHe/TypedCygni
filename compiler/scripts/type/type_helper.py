import json
from string import Template

type_list = ["Int", "Float", "Long", "Double",
             "Char", "String", "Boolean", "Function", "Array", "Object"]


def generate_type_def(type_name):
    if type_name == "Array":
        return """
        class ArrayType: public Type {
            public:
            TypePtr element;
            explicit ArrayType(TypePtr element): element{element}
            TypeCode GetTypeCode() override { return TypeCode::ARRAY; }
        };
        """
    elif type_name == "Object":
        return """
        class ObjectType: public Type {
            public:
            unordered_map<string, TypePtr> fields;
            explicit ObjectType(unordered_map<string, TypePtr> fields): fields{fields}
            TypeCode GetTypeCode() override { return TypeCode::OBJECT; }
        };
        """
    elif type_name == "Function":
        return """
        class FunctionType: public Type {
            public:
            vector<TypePtr> args;
            TypePtr ret;
            explicit FunctionType(unordered_map<string, TypePtr> fields): fields{fields}
            TypeCode GetTypeCode() override { return TypeCode::OBJECT; }
        };
        """


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

# print(create_binary())


def generate_fields_and_constructor(field_json):
    fields = []
    parameters = []
    initializers = []
    for field_type, field_name in field_json:
        fields.append(field_type + " " + field_name + ";")
        parameters.append(field_type + " " + field_name)
        initializers.append(field_name + "{" + field_name + "}")
    return (fields, parameters, initializers)


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
    with open("Types_Template.json", "r") as f:
        json_obj = json.load(f)
    for line in json_obj["Code"]["Top"]:
        print(line)
    print(types_definition(json_obj))
    print(values_definition(json_obj))
    for line in json_obj["Code"]["Bottom"]:
        print(line)


# python .\type_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM Type.hpp
main()
