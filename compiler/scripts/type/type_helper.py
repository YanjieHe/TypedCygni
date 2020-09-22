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
    lines.append("else { throw Error(node->Pos(), \"binary exp: error type\"); }")
    return "\n".join(lines)

print(create_binary())