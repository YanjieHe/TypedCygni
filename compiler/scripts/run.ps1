Set-Location enum/
python enum_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Enum.hpp"
Set-Location ..

Set-Location expression/
python expression_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Expression.hpp"
Set-Location ..

Set-Location type/
python type_helper.py | clang-format.exe | Out-File -Encoding utf8NoBOM "../outputs/Type.hpp"
Set-Location ..