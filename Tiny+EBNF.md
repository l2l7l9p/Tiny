Original definition of Tiny is on https://jlu.myweb.cs.uwindsor.ca/214/language.htm, where you can find definition of some tokens such as `IdString`(`Identifier` on that page), `QString`, `Integer` and `Float`.

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
• PrimExpr ::= Integer | Float | Id | (Id, '(', ActualParams, ')') | ('(', Expression, ')')
• ActualParams ::= [Expression, {',', Expression}]
```
