#include<bits/stdc++.h>
#include"syntax_tree.h"
#include"tiny.tab.h"
using namespace std;

void print_help() {
	puts("Usage: ./tiny <file> [options]");
	puts("Options:");
	puts("  -d\t\tPrint syntax tree");
	puts("  -h\t\tPrint help");
	puts("  -o <file>\tPlace the output into <file>");
}
void command_error(const char *message) {
	printf("tiny: command error: %s\n",message);
	exit(0);
}

string outFileName="a.out";
bool drawSyntaxTree;
void analyze_args(int argc,char *argv[]) {
	int inFileId=0;
	for(int i=1; i<argc; i++) {
		if (strcmp(argv[i],"-o")==0) {
			if (++i>=argc) command_error("missing argument to '-o'");
			outFileName=string(argv[i]);
		} else if (strcmp(argv[i],"-h")==0) {
			print_help();
			exit(0);
		} else if (strcmp(argv[i],"-d")==0) {
			drawSyntaxTree=1;
		} else {
			if (inFileId) command_error("more than one file");
			inFileId=i;
		}
	}
	if (!inFileId) command_error("no file");
	extern FILE *yyin;
	yyin=fopen(argv[inFileId],"r");
}

void compile() {
	int res=yyparse();		// res==0 means compiling successfully
	extern int yynerrs;
	res+=yynerrs;
	extern node *root;
	if (root!=nullptr) {
		root->simplify();
		if (drawSyntaxTree) root->draw_syntax_tree();
		res+=root->generate_3addr_code();
		if (res!=0) {
			puts("Complie Error");
		} else {
			FILE *f=fopen(outFileName.c_str(),"w");
			for(auto code:root->codes) fprintf(f,"%s\t\t%s\n",code.first.c_str(),code.second.c_str());
			fclose(f);
		}
		clear(root);
	}
}

int main(int argc,char *argv[]) {
	analyze_args(argc,argv);
	// yydebug=2;
	compile();
}