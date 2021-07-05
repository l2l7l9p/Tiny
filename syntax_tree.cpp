#include<bits/stdc++.h>
#include"syntax_tree.h"
using namespace std;

string rvOperatorType[]={"+","-","*","/","==","!=","<","<=","-","GET_VALUE","GET_VAR","CALL_FUNC"};
string rvVarType[4]={"BOOL","CHAR","INT","REAL"};

int node::EOCFCnt=0, node::loopCnt=0, node::elseCnt=0, node::varCnt=0;
vector<SymbolTabType> node::symbolTab;
string node::mainFuncLabel="NO";

#define semantic_error(message) {\
	cout << lineno << ": semantic error: " << message << endl;\
	err=1;\
}
#define semantic_warning(message) {\
	cout << lineno << ": warning: " << message << endl;\
}

Symbol find_symbol(string s) {
	for(int i=node::symbolTab.size()-1; i>=0; i--)
		if (node::symbolTab[i].count(s)) return node::symbolTab[i][s];
	return Symbol(SB_NOTFOUND);
}

void add_codes(Codes &a,Codes &b) {
	if (a.empty()) a=b;
		else for(auto code:b) a.push_back(code);
	b.clear();
}

// ****** node ******

void node::simplify() {
	for(auto x:son) x->simplify();
	if ((nodeType==METHODDECLS || nodeType==FORMALPARAMS || nodeType==STATEMENTS || nodeType==ACTUALPARAMS) && son.size()>=2) {
		node *son1=son[1];	// right-recursion, son[0]: direct son, son[1]: the recursive node
		son.resize(1);
		for(auto x:son1->son) son.push_back(x);
		delete son1;
	}
}

void node::draw_syntax_tree(int deep) {
	cout << string(4*deep,' ')+(this->get_type_info()) << endl;
	for(auto x:son) x->draw_syntax_tree(deep+1);
}

// ****** node_Program ******

string node_Program::get_type_info() {return "Program";}

bool node_Program::generate_3addr_code() {
	symbolTab.push_back(SymbolTabType());						// global label
	bool err=son[0]->generate_3addr_code();
	codes.push_back(make_pair("","goto "+mainFuncLabel));	// goto main function
	add_codes(codes,son[0]->codes);
	symbolTab.pop_back();
	return err;
}

// ****** node_MethodDecls ******

string node_MethodDecls::get_type_info() {return "MethodDecls";}

bool node_MethodDecls::generate_3addr_code() {
	bool err=0;
	for(auto xp:son) {
		node_MethodDecl *x=(node_MethodDecl*)xp;
		string &funcName=((node_Id*)(x->son[1]))->name;
		symbolTab.back()[funcName]=Symbol(SB_FUNC,"__"+funcName,((node_Type*)(x->son[0]))->varType,xp);
		if (x->isMain) {
			if (mainFuncLabel!="NO") semantic_error("more than one main function")
				else mainFuncLabel="__"+funcName;
		}
	}
	if (mainFuncLabel=="NO") semantic_error("no main function");
	
	for(auto xp:son) {
		node_MethodDecl *x=(node_MethodDecl*)xp;
		err|=x->generate_3addr_code();		// each method
		add_codes(codes,x->codes);
	}
	return err;
}

// ****** node_MethodDecl ******

string node_MethodDecl::get_type_info() {return "MethodDecl";}

bool node_MethodDecl::generate_3addr_code() {
	bool err=0;
	symbolTab.push_back(SymbolTabType());							// local label
	symbolTab.back()["%EOCF"]=Symbol(SB_LABEL,"EOCF_"+to_string(EOCFCnt++));	// 'End Of Control Flow' label
	symbolTab.back()["%FUNC"]=Symbol(SB_LABEL,"",((node_Type*)son[0])->varType,(node*)this);	// which function we are in
	err|=((node_FormalParams*)son[2])->add_formal_params();		// formal parameters
	if (son.size()>=4) {
		err|=son[3]->generate_3addr_code();						// statements
		add_codes(codes,son[3]->codes);
	}
	// check if the last instruction is return
	if (codes.empty() || codes.back().second!="return") {
		semantic_warning("lack of return at the end of function");
		codes.push_back(make_pair("","return"));
	}
	codes.back().first.insert(0,symbolTab.back()["%EOCF"].label+": ");
	codes[0].first.insert(0,"__"+((node_Id*)son[1])->name+": ");
	symbolTab.pop_back();
	return err;
}

// ****** node_FormalParams ******

string node_FormalParams::get_type_info() {return "FormalParams";}

bool node_FormalParams::add_formal_params() {
	bool err=0;
	for(auto x:son) {
		string &paramName=((node_Id*)(x->son[1]))->name;
		if (symbolTab.back().count(paramName)) semantic_error("redefinition of '"+paramName+"'")
			else symbolTab.back()[paramName]=Symbol(SB_VAR,paramName+"_"+to_string(varCnt),((node_Type*)(x->son[0]))->varType,x);
	}
	varCnt++;
	return err;
}

// ****** node_FormalParam ******

string node_FormalParam::get_type_info() {return "FormalParam";}

// ****** node_Type ******

string node_Type::get_type_info() {
	return "Type: "+rvVarType[varType];
}

// ****** node_Statements ******

string node_Statements::get_type_info() {return "Statements";}

bool isControlFlow(node_Statement *x) {
	return (!x->son.empty() && (x->son[0]->nodeType==IFSTMT || x->son[0]->nodeType==FORSTMT));
}

bool node_Statements::generate_3addr_code() {
	bool err=0;
	symbolTab.push_back(SymbolTabType());		// local label
	string lastEOCF="";
	// stack<node*> S;
	for(int i=0; i<son.size(); i++) {
		node_Statement *x=(node_Statement*)son[i];
		bool setNewEOCF=(isControlFlow(x) && i+1<son.size());
		if (setNewEOCF) symbolTab.back()["%EOCF"]=Symbol(SB_LABEL,"EOCF_"+to_string(EOCFCnt++));
		err|=x->generate_3addr_code();		// each statement
		if (!x->codes.empty()) {
			if (lastEOCF!="") x->codes[0].first.insert(0,lastEOCF+": ");
			if (setNewEOCF) lastEOCF=symbolTab.back()["%EOCF"].label; else {
				lastEOCF="";
				symbolTab.back().erase("%EOCF");
			}
		}
		add_codes(codes,x->codes);
	}
	symbolTab.pop_back();
	return err;
}

// ****** node_Statement ******

string node_Statement::get_type_info() {return "Statement";}

bool node_Statement::generate_3addr_code() {
	if (son.empty()) return 0;
	bool err=son[0]->generate_3addr_code();
	add_codes(codes,son[0]->codes);
	return err;
}

// ****** node_LocalVarDecl ******

string node_LocalVarDecl::get_type_info() {return "LocalVarDecl";}

bool node_LocalVarDecl::generate_3addr_code() {
	bool err=0;
	string &varName=((node_Id*)son[1])->name;
	if (symbolTab.back().count(varName)) semantic_error("redeclaration of '"+varName+"'")
		else symbolTab.back()[varName]=Symbol(SB_VAR,varName+"_"+to_string(varCnt++),((node_Type*)son[0])->varType,this);
	return err;
}

// ****** node_AssignStmt ******

string node_AssignStmt::get_type_info() {return "AssignStmt";}

bool node_AssignStmt::generate_3addr_code() {
	bool err=0;
	string &leftName=((node_Id*)son[0])->name;
	Symbol left=find_symbol(leftName);
	if (left.type==SB_NOTFOUND) semantic_error("undeclared identifier '"+leftName+"'")
		else if (left.type!=SB_VAR) semantic_error("identifier '"+leftName+"' is not a variable");
	err|=son[1]->generate_3addr_code();
	add_codes(codes,son[1]->codes);
	codes.push_back(make_pair("",left.label+" = "+((node_Expression*)son[1])->resultLabel));
	return err;
}

// ****** node_ReturnStmt ******

string node_ReturnStmt::get_type_info() {return "ReturnStmt";}

bool node_ReturnStmt::generate_3addr_code() {
	bool err=0;
	node_Expression *s=(node_Expression*)son[0];
	err|=s->generate_3addr_code();
	add_codes(codes,s->codes);
	Var_t funcType=find_symbol("%FUNC").varType;
	codes.push_back(make_pair("","%t_return = "+((s->varType!=funcType) ?"" :"("+rvVarType[funcType]+")")+s->resultLabel));
	codes.push_back(make_pair("","return"));
	return err;
}

// ****** node_IfStmt ******

string node_IfStmt::get_type_info() {return "IfStmt";}

bool node_IfStmt::generate_3addr_code() {
	node_Expression *condition=(node_Expression*)son[0];
	bool err=condition->generate_3addr_code();
	add_codes(codes,condition->codes);
	string t_condition;
	if (condition->varType==V_BOOL) t_condition=condition->resultLabel; else {
		t_condition="%t_"+to_string(varCnt++);
		codes.push_back(make_pair("",t_condition+" = "+condition->resultLabel+" != 0"));
	}
	Symbol eocf=find_symbol("%EOCF");
	symbolTab.push_back(SymbolTabType());		// local label
	err|=son[1]->generate_3addr_code();
	symbolTab.pop_back();
	if (son.size()>2) {
		symbolTab.push_back(SymbolTabType());	// local label
		err|=son[2]->generate_3addr_code();
		symbolTab.pop_back();
	}
	if (son.size()<3 || son[2]->codes.empty()) {	// no else or else is empty
		codes.push_back(make_pair("","ifFalse "+t_condition+" goto "+eocf.label));
		add_codes(codes,son[1]->codes);
	} else {				// with else
		string elseLabel="else_"+to_string(elseCnt++);
		codes.push_back(make_pair("","ifFalse "+t_condition+" goto "+elseLabel));
		add_codes(codes,son[1]->codes);
		codes.push_back(make_pair("","goto "+eocf.label));
		son[2]->codes[0].first.insert(0,elseLabel+": ");
		add_codes(codes,son[2]->codes);
	}
	return err;
}

// ****** node_ForStmt ******

string node_ForStmt::get_type_info() {return "ForStmt";}

bool node_ForStmt::generate_3addr_code() {
	bool err=0;
	
	symbolTab.push_back(SymbolTabType());
	
	node_Statement *init=(node_Statement*)son[0];			// init
	err|=init->generate_3addr_code();
	add_codes(codes,init->codes);
	
	node_Expression *condition=(node_Expression*)son[1];	// condition
	err|=condition->generate_3addr_code();
	Codes forCodes;
	add_codes(forCodes,condition->codes);
	string t_condition;
	if (condition->varType==V_BOOL) t_condition=condition->resultLabel; else {
		t_condition="%t_"+to_string(varCnt++);
		forCodes.push_back(make_pair("",t_condition+" = "+condition->resultLabel+" != 0"));
	}
	Symbol eocf=find_symbol("%EOCF");
	forCodes.push_back(make_pair("","ifFalse "+t_condition+" goto "+eocf.label));
	
	symbolTab.push_back(SymbolTabType());		// loop
	err|=son[3]->generate_3addr_code();
	symbolTab.pop_back();
	add_codes(forCodes,son[3]->codes);
	
	err|=son[2]->generate_3addr_code();		// after loop
	add_codes(forCodes,son[2]->codes);
	string loopLabel="loop_"+to_string(loopCnt++);
	forCodes.push_back(make_pair("","goto "+loopLabel));
	
	forCodes[0].first.insert(0,loopLabel+": ");
	add_codes(codes,forCodes);
	
	symbolTab.pop_back();
	
	return err;
}

// ****** node_WriteStmt ******

string node_WriteStmt::get_type_info() {return "WriteStmt";}

bool node_WriteStmt::generate_3addr_code() {
	bool err=son[0]->generate_3addr_code();
	codes=son[0]->codes;
	son[0]->codes.clear();
	codes.push_back(make_pair("","param "+((node_Expression*)son[0])->resultLabel));
	codes.push_back(make_pair("","param \""+fileName+"\""));
	codes.push_back(make_pair("","call WRITE"));
	return err;
}

// ****** node_ReadStmt ******

string node_ReadStmt::get_type_info() {return "ReadStmt";}

bool node_ReadStmt::generate_3addr_code() {
	bool err=0;
	string &varName=((node_Id*)son[0])->name;
	Symbol var=find_symbol(varName);
	if (var.type==SB_NOTFOUND) semantic_error("undeclared identifier '"+varName+"'")
		else if (var.type!=SB_VAR) semantic_error("identifier '"+varName+"' is not a variable");
	codes.push_back(make_pair("","param "+var.label));
	codes.push_back(make_pair("","param \""+fileName+"\""));
	codes.push_back(make_pair("","call READ"));
	return err;
}

// ****** node_Expression ******

string node_Expression::get_type_info() {
	string bas="Expression: "+rvOperatorType[op];
	if (op==OP_GET_VALUE) {
		switch (varType) {
			case V_INT : return bas+to_string(data.d_int);
			case V_REAL : return bas+to_string(data.d_float);
		}
	} else {
		return bas;
	}
}

bool node_Expression::generate_3addr_code() {
	bool err=0;
	if (op<=OP_UMINUS) {			// arithmetic or logic operation
		for(auto x:son) {
			err|=x->generate_3addr_code();
			add_codes(codes,x->codes);
		}
		if (op<=OP_LE) {				// binary operation
			node_Expression *left=(node_Expression*)son[0], *right=(node_Expression*)son[1];
			varType=max(left->varType,right->varType);		// type check
			if (left->varType<varType) codes.push_back(make_pair("",left->resultLabel+" = ("+rvVarType[varType]+") "+left->resultLabel));
			if (right->varType<varType) codes.push_back(make_pair("",right->resultLabel+" = ("+rvVarType[varType]+") "+right->resultLabel));
			resultLabel="%t_"+to_string(varCnt++);
			codes.push_back(make_pair("",resultLabel+" = "+left->resultLabel+" "+rvOperatorType[op]+" "+right->resultLabel));
			if (op>=OP_EQUAL) varType=V_BOOL;	// logic result
		} else if (op==OP_UMINUS) {		// unary operation
			node_Expression *s=(node_Expression*)son[0];
			codes.push_back(make_pair("",s->resultLabel+" = "+rvOperatorType[op]+" "+s->resultLabel));
			resultLabel=s->resultLabel;
			varType=s->varType;
		}
	} else if (op==OP_GET_VALUE) {
		resultLabel="%t_"+to_string(varCnt++);
		switch (varType) {
			case V_INT :
				codes.push_back(make_pair("",resultLabel+" = "+to_string(data.d_int)));
				break;
			case V_REAL :
				codes.push_back(make_pair("",resultLabel+" = "+to_string(data.d_float)));
				break;
		}
	} else if (op==OP_GET_VAR) {
		string &varName=((node_Id*)son[0])->name;
		Symbol var=find_symbol(varName);
		if (var.type==SB_NOTFOUND) semantic_error("undeclared identifier '"+varName+"'")
			else if (var.type!=SB_VAR) semantic_error("identifier '"+varName+"' is not a variable");
		resultLabel=var.label;
		varType=var.varType;
	} else if (op==OP_CALL_FUNC) {
		string &funcName=((node_Id*)son[0])->name;
		Symbol func=find_symbol(funcName);
		if (func.type==SB_NOTFOUND) semantic_error("undeclared identifier '"+funcName+"'")
			else if (func.type!=SB_FUNC) semantic_error("identifier '"+funcName+"' is not a function");
		// parameters
		node_ActualParams *aps=(node_ActualParams*)son[1];
		err|=aps->generate_3addr_code(func);
		add_codes(codes,aps->codes);
		// call function and get return value
		codes.push_back(make_pair("","call "+func.label));
		resultLabel="%t_"+to_string(varCnt++);
		codes.push_back(make_pair("",resultLabel+" = %t_return"));
		varType=func.varType;
	}
	return err;
}

// ****** node_ActualParams ******

string node_ActualParams::get_type_info() {return "ActualParams";}

bool node_ActualParams::generate_3addr_code(Symbol func) {
	bool err=0;
	vector<node_Expression*> params;
	for(auto xp:son) {
		node_Expression *x=(node_Expression*)xp;
		err|=x->generate_3addr_code();
		add_codes(codes,x->codes);
		params.push_back(x);
	}
	
	node_FormalParams *formalParams=(node_FormalParams*)((node_MethodDecl*)func.ref)->son[2];
	for(int i=0; i<params.size(); i++)
		if (i>=formalParams->son.size()) semantic_error("unexpected actual parameter") else {
			Var_t formalParamType=((node_Type*)((formalParams->son[i])->son[0]))->varType;
			if (params[i]->varType!=formalParamType)
				codes.push_back(make_pair("",params[i]->resultLabel+" = ("+rvVarType[formalParamType]+") "+params[i]->resultLabel));
			codes.push_back(make_pair("","param "+params[i]->resultLabel));
		}
	return err;
}

// ****** node_Id ******

string node_Id::get_type_info() {return "Id: "+name;}

// ****** others ******

void clear(node *curNode) {
	for(auto p:curNode->son) if (p!=nullptr) clear(p);
	delete curNode;
}