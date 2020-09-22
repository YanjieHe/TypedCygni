cd '.\lex & yacc\'
win_flex.exe lexer.l
win_bison -d parser.y
cd ..
Copy-Item -Path '.\lex & yacc\lex.yy.c' -Destination 'src\Lexer.cpp'
Copy-Item -Path '.\lex & yacc\parser.tab.h' -Destination 'include\Parser.hpp'
Copy-Item -Path '.\lex & yacc\parser.tab.c' -Destination 'src\Parser.cpp'
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
cd ..