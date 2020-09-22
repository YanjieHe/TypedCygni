cd "./lex & yacc/"
flex lexer.l
bison -d parser.y
cd ..
cp "./lex & yacc/lex.yy.c" "src/Lexer.cpp"
cp "./lex & yacc/parser.tab.h" "include/Parser.hpp"
cp "./lex & yacc/parser.tab.c" "src/Parser.cpp"
cd build
cmake ..
make
cd ..