#ifndef SYNTAX_TREE_CLASS
#define SYNTAX_TREE_CLASS

#include<bits/stdc++.h>
using namespace std;

enum NodeType{
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

enum VarType{
	V_INT,V_REAL,V_CHAR
};

union DataType{
	float d_float;
	int d_int;
	char d_char;
};

enum OperatorType{
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

class node{
	public:
	
	vector<node*> son;
	va_list ap;		// used for passing arguments to node's constructor
	
	node(int sonNum) {
		for(int i=0; i<sonNum; i++) {
			node* p=va_arg(ap,node*);
			if (p!=nullptr) son.push_back(p);
		}
	}
	
	virtual string get_type_string()=0;
	virtual NodeType get_type_int()=0;
	
	void simplify() {
		for(auto x:son) x->simplify();
		NodeType nodeType=this->get_type_int();
		if ((nodeType==METHODDECLS || nodeType==FORMALPARAMS || nodeType==STATEMENTS
			|| nodeType==ACTUALPARAMS) && son.size()>=2)
			{
			node *son1=son[1];	// right-recursion, son[0]: direct son, son[1]: the recursive node
			son.resize(1);
			for(auto x:son1->son) son.push_back(x);
			delete son1;
		}
	}
	
	void draw_syntax_tree(int deep=0) {
		cout << string(4*deep,' ')+(this->get_type_string()) << endl;
		for(auto x:this->son) x->draw_syntax_tree(deep+1);
	}
};

class node_Program : public node{
	public:
	node_Program(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "Program";}
	NodeType get_type_int() {return PROGRAM;}
};

class node_MethodDecls : public node{
	public:
	node_MethodDecls(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "MethodDecls";}
	NodeType get_type_int() {return METHODDECLS;}
};

class node_MethodDecl : public node{
	public:
	bool isMain;
	node_MethodDecl(bool b,int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {
		isMain=b;
	}
	string get_type_string() {return "MethodDecl";}
	NodeType get_type_int() {return METHODDECL;}
};

class node_FormalParams : public node{
	public:
	node_FormalParams(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "FormalParams";}
	NodeType get_type_int() {return FORMALPARAMS;}
};

class node_FormalParam : public node{
	public:
	node_FormalParam(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "FormalParam";}
	NodeType get_type_int() {return FORMALPARAM;}
};

class node_Type : public node{
	public:
	VarType varType;
	node_Type(VarType vt) : node(0) {varType=vt;}
	string get_type_string();
	NodeType get_type_int() {return TYPE;}
};

class node_Statements : public node{
	public:
	node_Statements(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "Statements";}
	NodeType get_type_int() {return STATEMENTS;}
};

class node_Statement : public node{
	public:
	node_Statement(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "Statement";}
	NodeType get_type_int() {return STATEMENT;}
};

class node_LocalVarDecl : public node{
	public:
	node_LocalVarDecl(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "LocalVarDecl";}
	NodeType get_type_int() {return LOCALVARDECL;}
};

class node_AssignStmt : public node{
	public:
	node_AssignStmt(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "AssignStmt";}
	NodeType get_type_int() {return ASSIGNSTMT;}
};

class node_ReturnStmt : public node{
	public:
	node_ReturnStmt(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "ReturnStmt";}
	NodeType get_type_int() {return RETURNSTMT;}
};

class node_IfStmt : public node{
	public:
	node_IfStmt(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "IfStmt";}
	NodeType get_type_int() {return IFSTMT;}
};

class node_ForStmt : public node{
	public:
	node_ForStmt(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "ForStmt";}
	NodeType get_type_int() {return FORSTMT;}
};

class node_WriteStmt : public node{
	public:
	string fileName;
	node_WriteStmt(char *s,int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {
		fileName=string(s+1,strlen(s)-2);
	}
	string get_type_string() {return "WriteStmt";}
	NodeType get_type_int() {return WRITESTMT;}
};

class node_ReadStmt : public node{
	public:
	string fileName;
	node_ReadStmt(char *s,int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {
		fileName=string(s+1,strlen(s)-2);
	}
	string get_type_string() {return "ReadStmt";}
	NodeType get_type_int() {return READSTMT;}
};

class node_Expression : public node{
	public:
	
	VarType varType;
	DataType data;
	OperatorType op;
	
	node_Expression(OperatorType opty,int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {
		op=opty;
	}
	node_Expression(char c,OperatorType opty,DataType d,VarType vt) : node(0) {
		op=opty;
		data=d;
		varType=vt;
	}
	string get_type_string();
	NodeType get_type_int() {return EXPRESSION;}
};

class node_ActualParams : public node{
	public:
	node_ActualParams(int sonNum,...) : node((va_start(ap,sonNum),sonNum)) {}
	string get_type_string() {return "ActualParams";}
	NodeType get_type_int() {return ACTUALPARAMS;}
};

class node_Id : public node{
	public:
	
	string name;
	
	node_Id(string id) : node(0) {name=id;}
	string get_type_string() {return "Id: "+name;}
	NodeType get_type_int() {return ID;}
};

void clear(node *curNode);

#endif