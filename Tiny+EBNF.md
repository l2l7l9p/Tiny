```
• Program ::= fMethodDeclg, MethodDeclMain, fMethodDeclg
• MethodDecl ::= Type, Id, '(', FormalParams, ')', Block
• MethodDeclMain ::= Type, Id, 'MAIN', '(', FormalParams, ')', Block
• FormalParams ::= [FormalParam, f',', FormalParamg]
• FormalParam ::= Type, Id
• Type ::= 'INT' | 'REAL' | 'CHAR'
• Id ::= IdString
• Block ::= 'BEGIN', fStatementg, 'END'
• Statement ::= SimpleStmt | Block | IfStmt | ForStmt
• SimpleStmt ::= LocalVarDecl | AssignStmt | ReturnStmt | WriteStmt | ReadStmt | ';'
• LocalVarDecl ::= Type, Id, ';'
• （省略AssignStmt, ReturnStmt, WriteStmt, ReadStmt 的描述，与Tiny 相同）
• IfStmt ::= 'IF', '(', Expression, ')', Statement, ['ELSE', Statement]
• ForStmt ::= 'FOR', '(', Statement, Expression, ';', Statement, ')', Statement
• Expression ::= PrimExpr | (Expression, ('+' | '-' | '*' | '/' | '==' | '!=' | '<' | '<='), Expression)
• PrimExpr ::= Number Integer | Number Float | Id | (Id, '(', ActualParams, ')') | ('(', Expression, ')')
• ActualParams ::= [Expression, f',', Expressiong]
```