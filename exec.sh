yacc -d --verbose pascal.y
lex -i old_lex.l
gcc y.tab.c lex.yy.c -ll -g