#ifndef _SYNANALYSIS_H
#define _SYNANALYSIS_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include "lexAnalysis.h"
#include "translate.h"

using namespace std;

class synAna{
private:
	FILE *synresult;//语法分析输出文件指针
	FILE *interresult;//中间代码输出文件指针
	list<Token>::iterator it;
	list<Token> tokenList;
	vartable curtable;//当前符号表
	bool has_return;  //函数是否return
	int curvarno;//当前中间变量编号
	list<int> truelist;//真出口链表
	list<int> falselist;//假出口链表
	list<int> nextlist;//下一语句
	int M1, M2, N;//用于回填
	bool is_boolexp;//是否是布尔表达式
	vector<int> label_arr;//跳转地址数组
public:
	vartable maintable;//总符号表
	vector<intermediate_code> intercode;
public:
	synAna(FILE *synResult,FILE *interResult,lexAnalysis lex);
	void program();//程序启动
private:
	void declaration_chain();	//声明串
	void declaration();			//声明，包括全局变量和函数声明
	void function();			//函数
	void type(char word[]);				//声明类型：变量或者函数
	void fparameter();			//形参
	void parameter_list();		//形参列表
	void param();				//参数
	void statement_block();		//语句块
	void innerdec();			//内部声明
	void statement_chain();		//语句串
	void innervardec();			//内部变量声明
	void statement();			//语句
	void if_statement();		//if语句
	void while_statement();		//while语句
	void return_statement();	//return语句
	void assignment_statement();//赋值语句
	variable expression();			//表达式
	variable plus_expression();		//加法表达式
	variable term();				//项
	variable factor();				//因子
	variable call(char word[]);				//call
	int argument_list(const char* funcname);		//实参列表
	int argument(const char* funcname);			//实参
	void ERROR(int linenum,char errinfo[]);				//出错
};

#endif