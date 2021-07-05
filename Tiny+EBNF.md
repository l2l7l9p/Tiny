Original definition of Tiny is in https://jlu.myweb.cs.uwindsor.ca/214/language.htm, where you can find some definition of tokens such as `Id` and `QString`.

```
• Program ::= {MethodDecl}
• MethodDecl ::= Type, Id, ['MAIN'], '(', FormalParams, ')', Block
• FormalParams ::= [FormalParam, {',', FormalParam}]
• FormalParam ::= Type, Id
• Type ::= ‘BOOL’ | 'INT' | 'REAL' | 'CHAR'
• Id ::= IdString
• Block ::= 'BEGIN', {Statement}, 'END'
• Statement ::= SimpleStmt | Block | IfStmt | ForStmt
• SimpleStmt ::= LocalVarDecl | AssignStmt | ReturnStmt | WriteStmt | ReadStmt | ';'
• LocalVarDecl ::= Type, Id, ';'
• AssignStmt ::= Id, ':=', Expression, ';'
• ReturnStmt ::= 'RETURN', Expression, ';'
• WriteStmt ::= 'WRITE', '(', Expression, ',', QString, ')', ';'
• ReadStmt ::= 'READ', '(', Expression, ',', QString, ')', ';'
• IfStmt ::= 'IF', '(', Expression, ')', Statement, ['ELSE', Statement]
• ForStmt ::= 'FOR', '(', SimpleStmt, Expression, ';', SimpleStmt, ')', Statement
• Expression ::= PrimExpr | (Expression, ('+' | '-' | '*' | '/' | '==' | '!=' | '<' | '<='), Expression) | ('-', Expression)
• PrimExpr ::= Number Integer | Number Float | Id | (Id, '(', ActualParams, ')') | ('(', Expression, ')')
• ActualParams ::= [Expression, {',', Expression}]
```
