# Compiler

Compiler for C-like language with C++.

Object Code: 80x86 assembly language

Project created by Visual Studio 2013.

## Abstract

Steps:

1. lex analysis
2. syntax analysis & intermediate code generation with no syntax tree
3. object code generation

Defect: 

&ensp;&ensp;distribution of registers in object code is not good.

## lex

* keyword: int | void | if | else | while | return

* identifier: letter(letter|number)*

* value: number(number)*

* assignment: =

* operator: +|-|*|/|=|==|>|>=|<|<=|!=

* delimiter: ;

* separator: ,

* annotator:  /* */|//

* parenthese: (|)

* brace: {|}

* letter: a|b|...|z|A|B|...|Z

* number: 0|1|2|...|9

* terminator: #

  Note: Every source code should include the terminator in the end.

##Syntax

//Note: tokens in ' ' mean characters

Program ::= <declaration chain>

<declaration chain> ::= <declaration> { <declaration> }

<declaration> ::= int <ID> <declaration type> | void <ID> <function declaration>

<declaration type> ::= <variable declaration> | <function declaration>

<variable declaration> ::= ;

<function declaration> ::= '(' <formal parameter> ')' <statement block>

<formal parameter> ::= <parameter list> | void

<parameter list> ::= <parameter> { ',' <parameter> }

<parameter> ::= int <ID>

<statement block> ::='{'  <inner declaration> <statement chain> '}'

<inner declaration> ::= <inner variable declaration> { ';' <inner variable declaration> } | empty

<inner variable declaration> ::= int <ID> { ',' <ID> } ';'

<statement chain> ::= <statement> { <statement> }

<statement> ::= <if statement> | <while statement> | <return statement> | <assignment statement>

<if statement> ::= if '(' <expression> ')' <statement block> [ else <statement block>]  //Note: terms in [] indicate they are optional

<while statement> ::= while '(' <expression> ')' <statement block>

<return statement> ::= return [ <expression> ]

<assignment statement> ::= <ID> '=' <expression>

<expression> ::= <plus expression> { relop <plus expression> } //Note: relop-> <|<=|>|>=|==|!=

<plus expression> ::= <term> { '+' <term> | '-' <term> }

<term> ::= <factor> { '*' <factor> | '/' <factor> }

<factor> ::= num | '(' <expression> ')' | <ID> <call function> //NOte: num means 0|1|2|...|9

<call function> ::= <call> | empty

<call> ::= '(' <actual parameter list> ')'

<actual parameter lsit> ::= <actual parameter> | empty

<actual parameter> ::= <expression> { ',' <expression> }

<ID> ::= letter( letter | number)*

