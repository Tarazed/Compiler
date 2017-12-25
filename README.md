# Compiler

Compiler for C-like language with C++.

Object Code: 80x86 assembly language

Project created by Visual Studio 2013.

## Abstract

Steps:

1. lex analysis
2. syntax analysis & intermediate code generation without building syntax tree
3. object code generation

Defect: 

&ensp;&ensp;Distribution of registers in object code is not good.

Note:

&ensp;&ensp;The input file "in.txt" should be put in the same directory of the source code.

## Lex

- keyword: int | void | if | else | while | return

- identifier: letter(letter|number)*

- value: number(number)*

- assignment: =

- operator: +|-|*|/|=|==|>|>=|<|<=|!=

- delimiter: ;

- separator: ,

- annotator:  /* */|//

- parenthese: (|)

- brace: {|}

- letter: a|b|...|z|A|B|...|Z

- number: 0|1|2|...|9

- terminator: #

  Note: Every source code should include the terminator in the end. 

## Syntax

//Note: tokens in ' ' mean characters

Program ::= &lt;declaration chain&gt;

&lt;declaration chain&gt; ::= &lt;declaration&gt; { &lt;declaration&gt; }

&lt;declaration&gt; ::= **int** &lt;ID&gt; &lt;declaration type&gt; | **void** &lt;ID&gt; &lt;function declaration&gt;

&lt;declaration type&gt; ::= &lt;variable declaration&gt; | &lt;function declaration&gt;

&lt;variable declaration&gt; ::= '**;**'

&lt;function declaration&gt; ::= '**(**' &lt;formal parameter&gt; '**)**' &lt;statement block&gt;

&lt;formal parameter&gt; ::= &lt;parameter list&gt; | **void**

&lt;parameter list&gt; ::= &lt;parameter&gt; { '**,**' &lt;parameter&gt; }

&lt;parameter&gt; ::= **int** &lt;ID&gt;

&lt;statement block&gt; ::='**{**'  &lt;inner declaration&gt; &lt;statement chain&gt; '**}**'

&lt;inner declaration&gt; ::= &lt;inner variable declaration&gt; { '**;**' &lt;inner variable declaration&gt; } | empty

&lt;inner variable declaration&gt; ::= int &lt;ID&gt; { '**,**' &lt;ID&gt; } '**;**'

&lt;statement chain&gt; ::= &lt;statement&gt; { &lt;statement&gt; }

&lt;statement&gt; ::= &lt;if statement&gt; | &lt;while statement&gt; | &lt;return statement&gt; | &lt;assignment statement&gt;

&lt;if statement&gt; ::= **if** '**(**' &lt;expression&gt; '**)**' &lt;statement block&gt; [ else &lt;statement block&gt;]  //Note: terms in [] indicate they are optional

&lt;while statement&gt; ::= **while** '**(**' &lt;expression&gt; '**)**' &lt;statement block&gt;

&lt;return statement&gt; ::= **return** [ &lt;expression&gt; ]

&lt;assignment statement&gt; ::= &lt;ID&gt; '**=**' &lt;expression&gt;

&lt;expression&gt; ::= &lt;plus expression&gt; { **relop** &lt;plus expression&gt; } //Note: relop-> <|<=|>|>=|==|!=

&lt;plus expression&gt; ::= &lt;term&gt; { '**+**' &lt;term&gt; | '**-**' &lt;term&gt; }

&lt;term&gt; ::= &lt;factor&gt; { '*****' &lt;factor&gt; | '**/**' &lt;factor&gt; }

&lt;factor&gt; ::= num | '**(**' &lt;expression&gt; '**)**' | &lt;ID&gt; &lt;call function&gt; //NOte: num means 0|1|2|...|9

&lt;call function&gt; ::= &lt;call&gt; | empty

&lt;call&gt; ::= '**(**' &lt;actual parameter list&gt; '**)**'

&lt;actual parameter lsit&gt; ::= &lt;actual parameter&gt; | empty

&lt;actual parameter&gt; ::= &lt;expression&gt; { '**,**' &lt;expression&gt; }

&lt;ID&gt; ::= **letter**( **letter** | **number**)*

