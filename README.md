# DXLexicalAnalyzer
编译原理作业

能够分析的词法
1. 标识符 关键字
2. 数字常量  包括 1e+3, 1.2e3, 23 的形式
3. 注释 /* */ 和 //
4. < > = <= >= = == +

#类似C语言文法
```
Program → Type main () {StatementBlock}
Type → int | void | float|char
StatementBlock → { StatementSequence }
StatementSequence → DeclareSequence ExecuteSequence
DeclareSequence → Type IdSequence ; DeclareSequence | ϵ
IdSequence → OneId IdSequence1 
IdSequence1 → , OneId IdSequence1 | ϵ
OneId → id Assign
Assign → = Express | ϵ
Express      →   Term  Expre1
Express1     →   + Term Expre1
Express1     →   - Term Expre1
Express1     →   ϵ
Term       →   Factor Term1
Term1      →   * Factor  Term1
Term1      →   / Factor  Term1
Term1      →   ϵ
Factor → id | constInterger | constFloat | (Express) 

ExecuteSequence → Execute ExecuteSequence | ϵ
Execute → id Assign ;  //→ OneId ;
Execute → WhileLoop
Execute → ForLoop
Execute → Branch
WhileLoop → while ( Condition ) { ExecuteSequence }
ForLoop → for(A;B;A){ ExecuteSequence }
Branch → if (Condition) { ExecuteSequence } Branch1
Branch1 → else { ExecuteSequence } | ϵ
Condition → Factor Cmp
Cmp → op Factor | ϵ
A → id Assign | ϵ //→ OneId
B → Condition | ϵ 

```
