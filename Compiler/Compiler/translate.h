#ifndef _TRANSLATE_H
#define _TRANSLATE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <string.h>

using namespace std;

#define INT_SIZE 2
#define VOID 1
#define INT 2

struct variable{
	char varname[100];	//变量名
	int is_var;			//是否变量（0表示立即数，1表示变量，2表示跳转语句的跳转地址）
	int is_temp;		//是否是临时变量
};

struct vartable{
	char funcname[100];//函数名
	int offset;		   //需要分配的存储空间
	int param_num;	   //形参个数
	int rettype;	   //返回类型
	vector<variable> param;//形参
	vector<variable> vararr;//变量
	vector<vartable> subtables;//子表
};

struct intermediate_code{
	int codeno;
	char op[10];
	variable arg1;
	variable arg2;
	variable result;
	bool is_label;//是否是跳转地址
};

vartable enterproc(vartable &parenttable, char name[]);//为函数建立表（子表）
void enter(vartable &parenttable, char varname[], int is_var, int is_param, int is_temp=0);//添加变量
void generate_intermediate_code(vector<intermediate_code> &intercode, const char* op, variable arg1, variable arg2, variable result, bool islabel=false);//生成中间代码
variable find_in_table(vartable table,const char* varname);//查找变量是否已定义
vartable find_func_in_table(vartable maintable, const char* funcname);//查找函数是否已定义
void update_table(vartable &maintable,vartable subtable);//更新maintable的子表
void backpatch(vector<intermediate_code> &intercode, list<int> &List, int M);//回填

#endif