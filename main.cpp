#include<bits/stdc++.h>
#include"syntax_tree.h"
#include"tiny.tab.h"
using namespace std;

int main(int argc,char *argv[]) {
	if (argc<2) {
		puts("no file found!\ncommand format: tiny <file>");
		return 0;
	}
	extern FILE *yyin;
	yyin=fopen(argv[1],"r");
	
	// yydebug=2;
	
	extern int yynerrs;
	if (yyparse()==0 && yynerrs==0) {
		extern node *root;
		assert(root!=nullptr);
		puts("Successfully");
		root->simplify();
		root->draw_syntax_tree();
		clear(root);
	} else {
		puts("Complie Error");
	}
}