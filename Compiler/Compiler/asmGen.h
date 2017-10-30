#ifndef _ASMGEN_H
#define _ASMGEN_H

#include <iostream>
#include <stdio.h>
#include <stack>

#include "synAnalysis.h"

using namespace std;

struct reginfo{//寄存器信息
	char regname[7];	//寄存器名
	char varname[100];	//存储的变量名
	int offset;			//变量在栈中的偏移量
	int is_temp;		//是否是临时变量
};

struct reglist{
	list<reginfo> emptylist;
	list<reginfo> usedlist;
	int jaddr;
};

class ASMCODE{//汇编代码
private://寄存器
	const char* EAX = "AX";
	const char* EBX = "BX";
	const char* ECX = "CX";
	const char* EDX = "DX";
	const char* ESP = "SP";
	const char* EBP = "BP";
	const char* EDI = "DI";
	const char* ESI = "SI";
	const char* EIP = "IP";
	const char* EFLAGS = "FLAGS";
private://指令
	const char* MOV = "MOV";
	const char* ADD = "ADD";
	const char* SUB = "SUB";
	const char* NEG = "NEG";
	const char* PUSH = "PUSH";
	const char* POP = "POP";
	const char* PUSHA = "PUSHA";
	const char* POPA = "POPA";
	const char* LEA = "LEA";
	const char* INC = "INC";
	const char* DEC = "DEC";
	const char* IMUL = "IMUL";
	const char* IDIV = "IDIV";
	const char* JMP = "JMP";
	const char* CMP = "CMP";
	const char* JE = "JE";
	const char* JNE = "JNE";
	const char* JG = "JG";
	const char* JGE = "JGE";
	const char* JL = "JL";
	const char* JLE = "JLE";
	const char* CALL = "CALL";
	const char* RET = "RET";
	const char* Int = "INT";
private:
	FILE *asmresult;
	vartable maintable;//总符号表
	vartable curtable;//当前符号表
	vector<intermediate_code> intercode;
	list<reginfo> empty_reg_list;//空闲寄存器
	list<reginfo> used_reg_list;//被占用寄存器
	vector<reglist> list_stack;
	int should_pop;
public:
	ASMCODE();
	ASMCODE(FILE *fout, vartable table, vector<intermediate_code> code);
	void generate_code();//开始生成目标代码
private:
	void print_asmcode(const char* code);//输出汇编代码
	void generate_datasegment();//生成数据段
	void generate_codesegment_head();//代码段头
	void generate_codesegment_stop();//终止程序
	void generate_codesegment_tail();//代码段结束语句
	void generate_instruction(const char* op, const char* dst, const char* src);//双操作数指令
	void generate_instruction(const char* op, const char* dst, int src);//双操作数指令
	void generate_instruction(const char* op, const char* src);//单操作数指令
	void init_empty_reg_list();//初始化空闲寄存器
	void clear_used_reg_list();//清空使用的寄存器
	reginfo is_in_reg(const char* varname);//变量是否在寄存器中
	reginfo get_empty_reg(variable arg, int willchange=0);//分配空的寄存器
	reginfo get_spec_reg(const char* regname);//返回指定的寄存器
	void write_to_mem(reginfo reg);//寄存器内容写回内存
	void get_addr(int offset, char *addr);//得到变量地址
	int pos_of_var(const char* varname);//返回指定参数在栈中的偏移地址
};

#endif