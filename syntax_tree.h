#include<bits/stdc++.h>
using namespace std;

extern int yylineno;

#ifndef SYNTAX_TREE
#define SYNTAX_TREE

enum Node_t{
	PROGRAM,
	METHODDECLS,
	METHODDECL,
	FORMALPARAMS,
	FORMALPARAM,
	TYPE,
	STATEMENTS,
	STATEMENT,
	LOCALVARDECL,
	ASSIGNSTMT,
	RETURNSTMT,
	IFSTMT,
	FORSTMT,
	WRITESTMT,
	READSTMT,
	EXPRESSION,
	ACTUALPARAMS,
	ID
};

enum Var_t{
	V_BOOL, V_CHAR, V_INT, V_REAL
};

union Data_t{
	float d_float;
	int d_int;
	char d_char;
};

enum Operator_t{
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	OP_EQUAL,
	OP_NEQUAL,
	OP_LESS,
	OP_LE,
	OP_UMINUS,
	OP_GET_VALUE,
	OP_GET_VAR,
	OP_CALL_FUNC
};

enum Symbol_t{
	SB_VAR,
	SB_FUNC,
	SB_LABEL,
	SB_NOTFOUND
};

class node;
struct Symbol {
	Symbol_t type;
	string label;
	Var_t varType;
	node *ref;		// reference to that symbol
	
	Symbol(Symbol_t ty,string lb="",Var_t vt=V_BOOL,node *rf=nullptr) {
		type=ty, varType=vt, label=lb, ref=rf;
	}
	Symbol() {}
};

typedef vector<pair<string,string>> Codes;	// format: pair<label,instruction>
typedef unordered_map<string,Symbol> SymbolTabType;

class node{
	public:
	
	Node_t nodeType;
	int lineno;
	vector<node*> son;
	va_list ap;							// used for passing arguments to node's constructor
	Codes codes;						// three address codes. 
	
	static vector<SymbolTabType> symbolTab;	// symbol table. It is a stack
	static int EOCFCnt, elseCnt, loopCnt, varCnt;			// label count
	static string mainFuncLabel;
	
	node(Node_t nt,int sonNum) {
		nodeType=nt;
		lineno=yylineno;
		for(int i=0; i<sonNum; i++) {
			node* p=va_arg(ap,node*);
			if (p!=nullptr) son.push_back(p);
		}
	}
	
	virtual string get_type_info()=0;
	virtual bool generate_3addr_code() {}		// return 0 if compiled successfully
	
	void simplify();
	void draw_syntax_tree(int deep=0);
};

class node_Program : public node{
	public:
	node_Program(int sonNum,...) : node(PROGRAM,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_MethodDecls : public node{
	public:
	node_MethodDecls(int sonNum,...) : node(METHODDECLS,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_MethodDecl : public node{
	public:
	bool isMain;
	node_MethodDecl(bool b,int sonNum,...) : node(METHODDECL,(va_start(ap,sonNum),sonNum)) {
		isMain=b;
	}
	string get_type_info();
	bool generate_3addr_code();
};

class node_FormalParams : public node{
	public:
	node_FormalParams(int sonNum,...) : node(FORMALPARAMS,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool add_formal_params();
};

class node_FormalParam : public node{
	public:
	node_FormalParam(int sonNum,...) : node(FORMALPARAM,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
};

class node_Type : public node{
	public:
	Var_t varType;
	node_Type(Var_t vt) : node(TYPE,0) {varType=vt;}
	string get_type_info();
};

class node_Statements : public node{
	public:
	node_Statements(int sonNum,...) : node(STATEMENTS,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_Statement : public node{
	public:
	node_Statement(int sonNum,...) : node(STATEMENT,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_LocalVarDecl : public node{
	public:
	node_LocalVarDecl(int sonNum,...) : node(LOCALVARDECL,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_AssignStmt : public node{
	public:
	node_AssignStmt(int sonNum,...) : node(ASSIGNSTMT,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_ReturnStmt : public node{
	public:
	node_ReturnStmt(int sonNum,...) : node(RETURNSTMT,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_IfStmt : public node{
	public:
	node_IfStmt(int sonNum,...) : node(IFSTMT,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_ForStmt : public node{
	public:
	node_ForStmt(int sonNum,...) : node(FORSTMT,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code();
};

class node_WriteStmt : public node{
	public:
	string fileName;
	node_WriteStmt(char *s,int sonNum,...) : node(WRITESTMT,(va_start(ap,sonNum),sonNum)) {
		int len=0;
		for(int i=1; s[i]!='"'; i++) len++;
		fileName=string(s+1,len);
	}
	string get_type_info();
	bool generate_3addr_code();
};

class node_ReadStmt : public node{
	public:
	string fileName;
	node_ReadStmt(char *s,int sonNum,...) : node(READSTMT,(va_start(ap,sonNum),sonNum)) {
		int len=0;
		for(int i=1; s[i]!='"'; i++) len++;
		fileName=string(s+1,len);
	}
	string get_type_info();
	bool generate_3addr_code();
};

class node_Expression : public node{
	public:
	
	Var_t varType;
	Data_t data;
	Operator_t op;
	string resultLabel;
	
	node_Expression(Operator_t opty,int sonNum,...) : node(EXPRESSION,(va_start(ap,sonNum),sonNum)) {
		op=opty;
	}
	node_Expression(char c,Operator_t opty,Data_t d,Var_t vt) : node(EXPRESSION,0) {
		op=opty;
		data=d;
		varType=vt;
	}
	string get_type_info();
	bool generate_3addr_code();
};

class node_ActualParams : public node{
	public:
	node_ActualParams(int sonNum,...) : node(ACTUALPARAMS,(va_start(ap,sonNum),sonNum)) {}
	string get_type_info();
	bool generate_3addr_code(Symbol);
};

class node_Id : public node{
	public:
	string name;
	node_Id(string id) : node(ID,0) {name=id;}
	string get_type_info();
};

void clear(node *curNode);

#endif