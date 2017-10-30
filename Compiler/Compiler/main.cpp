#include <iostream>
#include <stdio.h>
#include <list>

#include "base.h"
#include "lexAnalysis.h"
#include "SynAnalysis.h"
#include "asmGen.h"

using namespace std;

int main()
{
	FILE *fin = fopen("in.txt", "r");
	if (!fin){
		printf("打开源码文件失败\n");
		return 0;
	}
	FILE *lexout = fopen("lexAnaResult.txt", "w+");
	if (!lexout){
		printf("打开词法分析结果文件失败\n");
		return 0;
	}
	FILE *synResult = fopen("synResult.txt", "w+");
	if (!synResult){
		printf("打开语法分析结果文件失败\n");
		return 0;
	}
	FILE *interResult = fopen("interCode.txt", "w+");
	if (!interResult){
		printf("打开中间代码结果文件失败\n");
		return 0;
	}
	FILE *asmResult = fopen("asmCode.asm", "w+");
	if (!asmResult){
		printf("打开汇编代码结果文件失败\n");
		return 0;
	}
	printf("词法分析结果文件：lexAnaResult.txt\n语法分析结果文件：synResult.txt\n中间代码文件：interCode.txt\n目标代码文件：asmCode.asm\n\n\n");

	lexAnalysis lexresult = lexAnalysis(fin, lexout);//词法分析
	lexresult.scanwords();

	synAna syn(synResult, interResult, lexresult);//语法分析及中间代码生成
	syn.program();

	ASMCODE asmCode(asmResult, syn.maintable, syn.intercode);//目标代码生成
	asmCode.generate_code();
	getchar();
	if (fin)
		fclose(fin);
	if (lexout)
		fclose(lexout);
	if (synResult)
		fclose(synResult);
	if (interResult)
		fclose(interResult);
	if (asmResult)
		fclose(asmResult);
	return 0;
}