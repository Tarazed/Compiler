#include "base.h"

int charkind(char c)//判断字符类型
{
	if (is_letter(c) == 1)
		return LETTER;
	else if (is_number(c) == 1)
		return NUMBER;
	else
		return PUNCTUATION;
}

int is_letter(char c)//是否是字母
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
		return 1;
	return 0;
}

int is_number(char c)//是否是数字
{
	if (c >= '0'&&c <= '9')
		return 1;
	return 0;
}

int is_keyword(char str[])//是否是关键字
{
	int size = sizeof(keywords) / 7;
	for (int i = 0; i < size; i++){
		if (strcmp(str, keywords[i]) == 0){
			return 1;
		}
	}
	return 0;
}

int is_signword(char str[])//是否是标识符
{
	if (is_letter(str[0]) == 1){
		for (int i = 1; i < strlen(str); i++){
			if (!is_letter(str[i]) && !is_number(str[i]))
				return 0;
		}
		return 1;
	}
	return 0;
}

int is_numberstr(char str[])//是否纯数字
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!is_number(str[i]))
			return 0;
	}
	return 1;
}