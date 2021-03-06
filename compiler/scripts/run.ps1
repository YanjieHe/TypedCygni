python cpp_expression_generator.py --path=inputs/Expressions_Template.json --header | clang-format.exe | Out-File -Encoding utf8NoBOM outputs/Expression.hpp
python cpp_expression_generator.py --path=inputs/Expressions_Template.json | clang-format.exe | Out-File -Encoding utf8NoBOM outputs/Expression.cpp
python .\cpp_instruction_generator.py --path=inputs/Instructions_Template.json --header | clang-format.exe | Out-File -Encoding utf8NoBOM outputs/Instruction.hpp
# Set-Location enum/
# python enum_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Enum.hpp"
# Set-Location ..

# Set-Location expression/
# python expression_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Expression.hpp"
# Set-Location ..

# Set-Location type/
# python type_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Type.hpp"
# Set-Location ..