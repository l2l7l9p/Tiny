tiny: tiny.tab.o lex.yy.o main.o syntax_tree.o
	g++ main.o syntax_tree.o tiny.tab.o lex.yy.o -o tiny -g
tiny.tab.c: tiny.y
	bison -d tiny.y
lex.yy.c: tiny.l
	flex tiny.l
main.o: main.cpp syntax_tree.h tiny.tab.h
	g++ -c main.cpp -o main.o -g
syntax_tree.o: syntax_tree.h syntax_tree.cpp
	g++ -c syntax_tree.cpp -o syntax_tree.o -g
tiny.tab.o: syntax_tree.h tiny.tab.c
	g++ -c tiny.tab.c -o tiny.tab.o -g
lex.yy.o: tiny.tab.h syntax_tree.h lex.yy.c
	g++ -c lex.yy.c -o lex.yy.o -g