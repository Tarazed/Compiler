#ifndef _BASE_H
#define _BASE_H

#include <stdio.h>
#include <iostream>
#include <string.h>

const static char keywords[][7] = { "int", "void", "if", "else", "while", "return" };

#define LETTER 1
#define NUMBER 2
#define PUNCTUATION 3


int charkind(char c);//判断字符类型
int is_letter(char c);//是否是字母
int is_number(char c);//是否是数字
int is_keyword(char str[]);//是否是关键字
int is_signword(char str[]);//是否是标识符
int is_numberstr(char str[]);//是否纯数字


#endif