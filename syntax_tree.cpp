#include<bits/stdc++.h>
#include"syntax_tree.h"
using namespace std;

string rvOperatorType[11]={"+","-","*","/","==","!=","<","<=","GET_VALUE","GET_VAR","CALL_FUNC"};
string rvVarType[3]={"INT","REAL","CHAR"};

string node_Type::get_type_string() {
	return "Type: "+rvVarType[varType];
}

string node_Expression::get_type_string() {
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

void clear(node *curNode) {
	for(auto p:curNode->son) if (p!=nullptr) clear(p);
	delete curNode;
}