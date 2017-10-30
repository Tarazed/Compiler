#include "lexAnalysis.h"
#include "base.h"

#define MAXLINE 4096
lexAnalysis::lexAnalysis()
{

}
lexAnalysis::lexAnalysis(FILE *fin, FILE *fout)
{
	this->fin = fin;
	this->fout = fout;
	this->curline = 0;
}

void lexAnalysis::scanwords()//扫描输入文件，按行处理
{
	char ch;
	int count = 0; //每一行字符个数
	int chgline = 0;//行数
	char bufferscan[MAXLINE];
	memset(bufferscan, 0, sizeof(bufferscan));
	int finish_note = 2;
	printf("开始词法分析...\n\n");
	while (1)
	{
		ch = fgetc(fin);
		if (ch == '#'||ch == EOF){//读到文件尾
			fflush(fout);
			break;
		}
		if (ch == '\n'){//换行，开始处理
			curline++;
			fprintf(fout, "%d: %s\n", curline,bufferscan);
			finish_note=clearnotes(bufferscan,finish_note);
			if (finish_note==2){
				getwords(bufferscan);
			}
			else if (finish_note == 1){
				finish_note = 2;
			}
			memset(bufferscan, 0, sizeof(bufferscan));
			count = 0;
			continue;
		}
		else{
			bufferscan[count++] = ch;
		}
	}
	printf("词法分析完毕...\n\n");
}

int lexAnalysis::clearnotes(char buffer[],int finish_note)//清除注释
{
	int i, j;
	int flag = 0;//0表示非字符串状态，1表示字符串状态
	char note[MAXLINE];
	memset(note, 0, sizeof(note));
	int count = 0;
	int finish = 0;
	for (i = 0; buffer[i] != '\0'; i++){
		if (finish_note >0){
			if (buffer[i] == '"'){
				flag = 1 - flag;
				continue;
			}
			if (buffer[i] == '/'&&flag == 0){
				if (buffer[i + 1] == '/'){//单行注释
					for (j = i; buffer[j] != '\0'; j++){
						note[count++] = buffer[j];
					}
					note[count] = '\0';
					count = 0;
					buffer[i] = '\0';
					fprintf(fout, " %s \t\t\t注释\n", note);
					finish = 2;
					return finish;
				}
				else if (buffer[i + 1] == '*'){//多行注释
					note[count++] = '/';
					note[count++] = '*';
					for (j = i + 2; buffer[j] != '\0'; j++){
						note[count++] = buffer[j];
						if (buffer[j] == '*'&&buffer[j + 1] == '/'){
							note[count++] = '/';
							note[count] = '\0';
							count = 0;
							j += 2;
							finish = 1;
							break;
						}
					}
					if (strlen(note)>0)
						fprintf(fout, " %s \t\t\t注释\n", note);
					for (; buffer[j] != '\0'; j++)
						buffer[i] = buffer[j];
					buffer[i] = '\0';
					return finish;
				}
			}
		}//end if
		else{
			note[count++] = buffer[i];
			if (buffer[i] == '*'&&buffer[i + 1] == '/'){//结束多行注释
				finish = 1;
				note[count++] = buffer[i + 1];
				note[count] = '\0';
				count = 0;
				i += 1;
			}
		}
	}
	if (strlen(note)>0)
		fprintf(fout, " %s \t\t\t注释\n", note);
	if (finish_note == 2 && finish == 0)
		finish = 2;
	return finish;
}

void lexAnalysis::getwords(char buffer[])//处理出单词
{
	char word[100];
	memset(word, 0, sizeof(word));
	int i, j=0;
	for (i = 0; buffer[i] != '\0'; i++){
		int chkind = charkind(buffer[i]);
		switch (chkind)
		{
		case LETTER:case NUMBER:
			while (chkind==LETTER||chkind==NUMBER){
				word[j++] = buffer[i++];
				chkind = charkind(buffer[i]);
			}
			i--;
			word[j] = '\0';
			wordresult(word);
			j = 0;
			break;
		case PUNCTUATION:
			word[0] = buffer[i];
			if ((buffer[i] == '=' || buffer[i] == '!' || buffer[i] == '<' || buffer[i] == '>') && buffer[i + 1] == '=')
			{
				word[1] = '=';
				word[2] = '\0';
				i++;
			}
			else
				word[1] = '\0';
			
			wordresult(word);
			j = 0;
			break;
		}
	}
}

void lexAnalysis::wordresult(char word[])//输出单词分析结果
{
	Token token;
	strcpy(token.word, word);
	token.linenum = curline;
	if (is_keyword(word)){
		fprintf(fout, " %s \t\t\t关键字\n", word);
		token.type = token_type::keyword;
	}
	else if (is_signword(word)){
		fprintf(fout, " %s \t\t\t标识符\n", word);
		token.type = token_type::ID;
	}
	else if (is_numberstr(word)){
		fprintf(fout, " %s \t\t\t数\n", word);
		token.type = token_type::num;
	}
	else if (strlen(word) == 1){
		switch (word[0])
		{
		case '=':
			fprintf(fout, " %s \t\t\t 赋值/运算符\n", word);
			token.type = token_type::assignment;
			break;
		case '+':case '-':
			fprintf(fout, " %s \t\t\t 运算符\n", word);
			token.type = token_type::plus;
			break;
		case '*':case '/':
			fprintf(fout, " %s \t\t\t 运算符\n", word);
			token.type = token_type::multiple;
			break;
		case '>':case '<':
			fprintf(fout, " %s \t\t\t 运算符\n", word);
			token.type = token_type::relop;
			break;
			break;
		case ';':
			fprintf(fout, " %s \t\t\t 界符\n", word);
			token.type = token_type::semicolon;
			break;
		case ',':
			fprintf(fout, " %s \t\t\t 分隔符\n", word);
			token.type = token_type::comma;
			break;
		case '(':
			fprintf(fout, " %s \t\t\t 左括号\n", word);
			token.type = token_type::lbracket;
			break;
		case ')':
			fprintf(fout, " %s \t\t\t 右括号\n", word);
			token.type = token_type::rbracket;
			break;
		case '{':
			fprintf(fout, " %s \t\t\t 左大括号\n", word);
			token.type = token_type::lbrace;
			break;
		case '}':
			fprintf(fout, " %s \t\t\t 右大括号\n", word);
			token.type = token_type::rbrace;
			break;
		}
	}
	else if (!strcmp(word, "==") || !strcmp(word, "!=") || !strcmp(word, ">=") || !strcmp(word, "<=")){
		fprintf(fout, " %s \t\t\t 运算符\n", word);
		token.type = token_type::relop;
	}
	else{
		fprintf(fout, " %s \t\t\t 非法串\n", word);
		printf("词法错误：第%d行， %s : 非法串\n", curline, word);
		getchar();
		exit(0);
	}
	if (word[0] != ' '&&word[0]!='\t')
		this->tokenList.push_back(token);
}