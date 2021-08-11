%{
#include<bits/stdc++.h>
#include"syntax_tree.h"
using namespace std;

extern int yylex();
extern int yylineno, yynerrs;
void yyerror(const char* s);

node *root;
int lastyynerrs=0;
%}

%define api.value.type union
%define parse.trace
%error-verbose

%token<int> Number_Integer
%token<double> Number_Float
%token<char*> IdString QString
%token IF ELSE FOR WRITE READ RETURN tkBEGIN END MAIN INT REAL CHAR Assign ":=" Equal "==" NEqual "!=" LE "<="

%type<node_Program*> Program
%type<node_MethodDecls*> MethodDecls
%type<node_MethodDecl*> MethodDecl
%type<node_FormalParams*> FormalParams NonEmptyFormalParams
%type<node_FormalParam*> FormalParam
%type<node_Type*> Type
%type<node_Statements*> Statements Block
%type<node_Statement*> Statement ElseStmt SimpleStmt
%type<node_LocalVarDecl*> LocalVarDecl
%type<node_AssignStmt*> AssignStmt
%type<node_ReturnStmt*> ReturnStmt
%type<node_IfStmt*> IfStmt
%type<node_ForStmt*> ForStmt
%type<node_WriteStmt*> WriteStmt
%type<node_ReadStmt*> ReadStmt
%type<node_Expression*> Expression PrimExpr
%type<node_ActualParams*> ActualParams NonEmptyActualParams
%type<node_Id*> Id

%nonassoc "==" "!=" '<' "<="
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%destructor{} <int> <double> <char*>
%destructor{
	if (yynerrs!=lastyynerrs) clear($$);
	lastyynerrs=yynerrs;
} <*>

%%

Program:
	MethodDecls {$$ = new node_Program(1,$1); root=$$;}
	;
MethodDecls:
	  {$$ = new node_MethodDecls(0);}
	| MethodDecl MethodDecls {$$ = new node_MethodDecls(2,$1,$2);}
	;
MethodDecl:
	  Type Id '(' FormalParams ')' Block {$$ = new node_MethodDecl(0,4,$1,$2,$4,$6);}
	| Type MAIN Id '(' FormalParams ')' Block {$$ = new node_MethodDecl(1,4,$1,$3,$5,$7);}
	;
FormalParams:
	  {$$ = new node_FormalParams(0);}
	| NonEmptyFormalParams {$$ = $1;}
	;
NonEmptyFormalParams:
	  FormalParam {$$ = new node_FormalParams(1,$1);}
	| FormalParam ',' NonEmptyFormalParams {$$ = new node_FormalParams(2,$1,$3);}
	;
FormalParam:
	Type Id {$$ = new node_FormalParam(2,$1,$2);}
	;
Type:
	  INT {$$ = new node_Type(V_INT);}
	| REAL {$$ = new node_Type(V_REAL);}
	| CHAR {$$ = new node_Type(V_CHAR);}
	;
Statements: {$$ = new node_Statements(0);}
	| Statement Statements {$$ = new node_Statements(2,$1,$2);}
	;
Block:
	tkBEGIN Statements END {$$ = $2;}
	;
Statement:
	  SimpleStmt {$$ = $1;}
	| Block {$$ = new node_Statement(1,$1);}
	| IfStmt {$$ = new node_Statement(1,$1);}
	| ForStmt {$$ = new node_Statement(1,$1);}
	| error {$$ = nullptr;}
	;
SimpleStmt:
	  LocalVarDecl {$$ = new node_Statement(1,$1);}
	| AssignStmt {$$ = new node_Statement(1,$1);}
	| ReturnStmt {$$ = new node_Statement(1,$1);}
	| WriteStmt {$$ = new node_Statement(1,$1);}
	| ReadStmt {$$ = new node_Statement(1,$1);}
	| ';' {$$ = new node_Statement(0);}
	| error ';' {$$ = nullptr;}
	;
LocalVarDecl:
	Type Id ';' {$$ = new node_LocalVarDecl(2,$1,$2);}
	;
AssignStmt:
	Id Assign Expression ';' {$$ = new node_AssignStmt(2,$1,$3);}
	;
ReturnStmt:
	RETURN Expression ';' {$$ = new node_ReturnStmt(1,$2);}
	;
IfStmt:
	IF '(' Expression ')' Statement ElseStmt {$$ = new node_IfStmt(3,$3,$5,$6);}
	;
ElseStmt: {$$ = nullptr;}
	| ELSE Statement {$$ = $2;}
	;
ForStmt:
	FOR '(' SimpleStmt Expression ';' SimpleStmt ')' Statement {$$ = new node_ForStmt(4,$3,$4,$6,$8);}
	;
WriteStmt:
	WRITE '(' Expression ',' QString ')' ';' {$$ = new node_WriteStmt($5,1,$3);}
	;
ReadStmt:
	READ '(' Id ',' QString ')' ';' {$$ = new node_ReadStmt($5,1,$3);}
	;
Expression:
	  PrimExpr {$$ = $1;}
	| '(' Expression ')' {$$ = $2;}
	| Expression '+' Expression {$$ = new node_Expression(OP_PLUS,2,$1,$3);}
	| Expression '-' Expression {$$ = new node_Expression(OP_MINUS,2,$1,$3);}
	| Expression '*' Expression {$$ = new node_Expression(OP_MUL,2,$1,$3);}
	| Expression '/' Expression {$$ = new node_Expression(OP_DIV,2,$1,$3);}
	| Expression '<' Expression {$$ = new node_Expression(OP_LESS,2,$1,$3);}
	| Expression Equal Expression {$$ = new node_Expression(OP_EQUAL,2,$1,$3);}
	| Expression NEqual Expression {$$ = new node_Expression(OP_NEQUAL,2,$1,$3);}
	| Expression LE Expression {$$ = new node_Expression(OP_LE,2,$1,$3);}
	| '-' Expression %prec UMINUS {$$ = new node_Expression(OP_UMINUS,1,$2);}
	;
PrimExpr:
	  Number_Integer {
		Data_t data;
		data.d_int=$1;
		$$ = new node_Expression('N',OP_GET_VALUE,data,V_INT);
		}
	| Number_Float {
		Data_t data;
		data.d_float=$1;
		$$ = new node_Expression('N',OP_GET_VALUE,data,V_REAL);
		}
	| Id {$$ = new node_Expression(OP_GET_VAR,1,$1);}
	| Id '(' ActualParams ')' {$$ = new node_Expression(OP_CALL_FUNC,2,$1,$3);}
	;
ActualParams:
	  {$$ = new node_ActualParams(0);}
	| NonEmptyActualParams {$$ = $1;}
	;
NonEmptyActualParams:
	  Expression {$$ = new node_ActualParams(1,$1);}
	| Expression ',' NonEmptyActualParams {$$ = new node_ActualParams(2,$1,$3);}
	;
Id:
	IdString {$$ = new node_Id(string($1));}

%%

void yyerror(const char *msg) {
	printf("%d: %s\n",yylineno,msg);
}