#ifndef _LEXANALYSIS_H
#define _LEXANALYSIS_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>

using namespace std;

enum token_type{
	keyword, ID, num, assignment, plus, multiple, relop, semicolon,
	comma, lbracket, rbracket, lbrace, rbrace
};

typedef struct Token{
	char word[100];		//串
	token_type type;	//类型
	int linenum;		//行数
}Token;

class lexAnalysis{
public:
	list<Token> tokenList;	//词法分析结果表
private:
	FILE *fin;//输入文件
	FILE *fout;//词法结果输出文件
	int curline;//当前行数
public:
	lexAnalysis();
	lexAnalysis(FILE *fin, FILE *fout);
	void scanwords();//扫描输入文件，按行处理
	int clearnotes(char buffer[], int finish_note);//清除注释
	void getwords(char buffer[]);//处理出单词
	void wordresult(char word[]);//输出单词分析结果
};

#endif