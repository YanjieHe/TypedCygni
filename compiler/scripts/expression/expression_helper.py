import os
import CppHeaderParser
import sys
import json


def get_cpp_header_info(input_file_path):
    try:
        cppHeader = CppHeaderParser.CppHeader(input_file_path)
        return cppHeader
    except CppHeaderParser.CppParseError as e:
        print(e)
        sys.exit(1)


def expression_def(class_name, cppHeader, node_type):
    class_template = """
    class {0}: public Expression {{
    public:
        {1}
        {0}({2}): {3} {{}}
        Position Pos() const override {{ return pos; }}
        ExpressionType NodeType() const override {{ return {4}; }}
    }};
    """
    field_defs = []
    field_args = []
    field_initializers = []
    for field in cppHeader.classes[class_name]["properties"]["public"]:
        field_defs.append("{0} {1};".format(field["type"], field["name"]))
        field_args.append("{0} {1}".format(field["type"], field["name"]))
        field_initializers.append("{0}{{{0}}}".format(field["name"]))
    return class_template.format(
        class_name,
        "\n".join(field_defs),
        ",".join(field_args),
        ",".join(field_initializers),
        node_type
    )


def statement_def(class_name, cppHeader, statement_type):
    class_template = """
    class {0}: public Statement {{
    public:
        {1}
        {0}({2}): {3} {{}}
        Position Pos() const override {{ return pos; }}
        StatementType GetStatementType() const override {{ return {4}; }}
    }};
    """
    field_defs = []
    field_args = []
    field_initializers = []
    for field in cppHeader.classes[class_name]["properties"]["public"]:
        field_defs.append("{0} {1};".format(field["type"], field["name"]))
        field_args.append("{0} {1}".format(field["type"], field["name"]))
        field_initializers.append("{0}{{{0}}}".format(field["name"]))
    return class_template.format(
        class_name,
        "\n".join(field_defs),
        ",".join(field_args),
        ",".join(field_initializers),
        statement_type
    )


def generic_visitor(json_obj):
    visitor_code = """
    template <typename ExpReturnType, typename StatementReturnType,
          typename... ArgTypes>
    class Visitor {{
    public:
    virtual ExpReturnType VisitExpression(Expression* node, ArgTypes... args) {{
        switch (node->NodeType()) {{
            {0}
        default:
        throw Error(node->Pos(), "unsupported node type for visitor");
        }}
    }}
    virtual StatementReturnType VisitStatement(Statement* statement,
                                                ArgTypes... args) {{
        switch (statement->GetStatementType()) {{
            {1}
        default:{{
        throw Error(node->Pos(), "unsupported statement type for visitor");
        }}
        }}
    }}
    {2}
    virtual StatementReturnType VisitExpStatement(Expression *node,
                                                ArgTypes... args) = 0;
    }};"""
    exp_lines = []
    stmt_lines = []
    func_lines = []
    for exp_name in json_obj["Expressions"]:
        for enum_name in json_obj["Expressions"][exp_name]:
            exp_lines.append(
                "case ExpressionType::{0}: {{ return Visit{1}(dynamic_cast<{2}*>(node), args...); }}"
                .format(
                    enum_name, exp_name, exp_name + "Expression"
                ))
        func_lines.append(
            "virtual ExpReturnType Visit{0}({1}* node, ArgTypes... args) = 0;"
            .format(
                exp_name, exp_name + "Expression"
            )
        )
    for stmt_name in json_obj["Statements"]:
        for enum_name in json_obj["Statements"][stmt_name]:
            stmt_lines.append(
                "case StatementType::{0}: {{ return Visit{1}(dynamic_cast<{2}*>(node), args...); }}"
                .format(
                    enum_name, stmt_name, stmt_name + "Statement"
                ))
        func_lines.append(
            "virtual StatementReturnType Visit{0}({1}* node, ArgTypes... args) = 0;"
            .format(
                stmt_name, stmt_name + "Statement"
            )
        )
    return visitor_code.format(
        "\n".join(exp_lines),
        "\n".join(stmt_lines),
        "\n".join(func_lines)
    )


def generate_visitor(json_obj, visitor_name, exp_ret_type, stmt_ret_type, args_type, arg_names):
    visitor_code = """
    template <>
    class {0}<{1}, {2}{3}> {{
    public:
    {4}
    }};
    """
    func_lines = []
    for exp_name in json_obj["Expressions"]:
        func_lines.append(
            "{0} Visit{1}({2}* node{3}) override {{}}"
            .format(
                exp_ret_type, exp_name, exp_name + "Expression", arg_names
            )
        )
    for stmt_name in json_obj["Statements"]:
        func_lines.append(
            "{0} Visit{1}({2}* node{3}) override {{}}"
            .format(
                stmt_ret_type, stmt_name, stmt_name + "Statement", arg_names
            )
        )
    return visitor_code.format(
        visitor_name, exp_ret_type, stmt_ret_type, args_type,
        "\n".join(func_lines)
    )


def main():
    pass


input_file_path = "Expressions_Template.cpp"
cppHeader = get_cpp_header_info(input_file_path)
with open("Expressions_Template.json", "r") as f:
    json_obj = json.load(f)


print("#ifndef EXPRESSION_HPP")
print("#define EXPRESSION_HPP")
for header in cppHeader.includes:
    print("#include {0}".format(header))
print()
print("""
using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;

class Statement {
public:
  virtual Position Pos() const = 0;
  virtual StatementType GetStatementType() const = 0;
};

class Expression : public Statement {
public:
  virtual ExpressionType NodeType() const = 0;
  StatementType GetStatementType() const override {
    return StatementType::EXPRESSION;
  }
};
""")
for key in json_obj["Expressions"]:
    if len(json_obj["Expressions"][key]) > 1:
        print(expression_def(key + "Expression", cppHeader, "nodeType"))
    else:
        print(expression_def(key + "Expression", cppHeader,
                             "ExpressionType::" + json_obj["Expressions"][key][0]))


for key in json_obj["Statements"]:
    if len(json_obj["Statements"][key]) > 1:
        print(statement_def(key + "Statement", cppHeader, "statementType"))
    else:
        print(statement_def(key + "Statement", cppHeader,
                            "StatementType::" + json_obj["Statements"][key][0]))

print(generic_visitor(json_obj))
print(generate_visitor(json_obj, "TypeChecker", "TypePtr", "void", ", ScopePtr", ", ScopePtr scope"))
print(generate_visitor(json_obj, "AstJsonSerializer", "json", "json", "", ""))
print("\n#endif // EXPRESSION_HPP")

main()
