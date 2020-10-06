import os
import json


def instructions_definitions(json_data):
    class_def_template = """
    class $class_name: Instruction {
    public:
        $fields

        $class_name($parameters): $initializers { }
        OpCode GetOpCode() const override { return $op_code; }
        int Length() const override { return $length; }
    };
    """
    pass