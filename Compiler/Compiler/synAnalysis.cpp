#include "synAnalysis.h"

synAna::synAna(FILE *synResult, FILE *interResult, lexAnalysis lex)
{
	synresult = synResult;
	interresult = interResult;
	tokenList = lex.tokenList;
	it = tokenList.begin();
	maintable.offset = 0;
	strcpy(maintable.funcname,"_mainproc");
	maintable.vararr.reserve(10);
	maintable.subtables.reserve(10);
	curvarno = 0;
	intercode.reserve(100);
}

void synAna::program()//程序启动
{
	printf("开始语法分析及中间代码生成...\n\n");
	while (it != tokenList.end()){
		declaration_chain();
	}
	vartable functable;
	functable = find_func_in_table(maintable, "main");
	if (functable.param_num == -1){
		ERROR(-1, "找不到main函数");
	}
	char string[500];
	for (int i = 0; i < intercode.size(); i++){
		sprintf(string, "%5d : ( %s, %s, %s, %s )\n", intercode[i].codeno, intercode[i].op, intercode[i].arg1.varname, intercode[i].arg2.varname, intercode[i].result.varname);
		fprintf(interresult, string);
	}
	fflush(interresult);
	for (int i = 0; i < label_arr.size(); i++){
		intercode[label_arr[i]].is_label = true;
	}
	printf("语法分析及中间代码生成完成...\n\n");
	fprintf(synresult, "语法分析完毕，无语法错误");
	fflush(synresult);
}

void synAna::declaration_chain()	//声明串
{
	declaration();
}
void synAna::declaration()			//声明，包括全局变量和函数声明
{
	if (it->type == token_type::keyword){
		if (strcmp(it->word, "int") == 0){
			it++;
			if (it->type == token_type::ID){
				Token token(*it);
				it++;
				type(token.word);
				return;
			}
			ERROR(it->linenum,"声明语句出错");
		}
		else if (strcmp(it->word, "void") == 0){
			it++;
			if (it->type == token_type::ID){
				vartable functable;
				functable = find_func_in_table(maintable, it->word);
				if (functable.param_num != -1)
					ERROR(it->linenum, "函数重复定义");
				curtable = enterproc(maintable, it->word);
				curtable.rettype = VOID;
				variable arg1 = { "", 1 }, arg2 = { "", 1 }, result;
				result.is_var = 1;
				strcpy(result.varname, it->word);
				generate_intermediate_code(intercode, "define", arg1, arg2,result);
				it++;
				function();
				return;
			}
			ERROR(it->linenum, "函数声明出错");
		}
	}
	ERROR(it->linenum, "声明语句出错");
}
void synAna::type(char word[])				//声明类型：变量或者函数
{
	if (it->type == token_type::semicolon){
		it++;
		enter(maintable, word, 1, 0);
		return;
	}
	else{
		vartable functable;
		functable = find_func_in_table(maintable, word);
		if (functable.param_num != -1)
			ERROR(it->linenum, "函数重复定义");
		curtable = enterproc(maintable, word);
		curtable.rettype = INT;
		variable arg1 = { "", 1 }, arg2 = { "", 1 }, result;
		result.is_var = 1;
		strcpy(result.varname, word);
		generate_intermediate_code(intercode, "define", arg1, arg2, result);
		function();
	}
}
void synAna::function()			//函数
{
	has_return = false;
	if (it->type == token_type::lbracket){
		it++;
		curtable.param_num = 0;
		fparameter();
		if (it->type != token_type::rbracket){
			ERROR(it->linenum, "缺少右括号");
		}
		it++;
		statement_block();
		if (has_return == false && curtable.rettype==INT){
			ERROR(it->linenum, "函数缺少return");
		}
		update_table(maintable,curtable);
		curtable = maintable;
		return;
	}
	ERROR(it->linenum, "缺少左括号");
}
void synAna::fparameter()			//形参
{
	if (it->type == token_type::rbracket)
		return;
	else if (strcmp(it->word, "void") == 0){
		it++;
		return;
	}
	parameter_list();
}
void synAna::parameter_list()		//形参列表
{
	param();
	while (it->type == token_type::comma){
		it++;
		param();
	}
}
void synAna::param()				//参数
{
	if (strcmp(it->word, "int") == 0){
		it++;
		if (it->type == token_type::ID){
			variable v = find_in_table(curtable, it->word);
			if (v.is_var!=-1)
				ERROR(it->linenum, "变量重复定义");
			enter(curtable, it->word, 1, 1);
			it++;
			curtable.param_num++;
			return;
		}
		ERROR(it->linenum, "声明语句出错");
	}
	ERROR(it->linenum, "声明语句出错");
}
void synAna::statement_block()		//语句块
{
	if (it->type == token_type::lbrace){
		it++;
		innerdec();
		statement_chain();
		if (it->type == token_type::rbrace){
			it++;
			return;
		}
		ERROR(it->linenum, "缺少右大括号");
	}
	ERROR(it->linenum, "语句块缺少左大括号");
}
void synAna::innerdec()			//内部声明
{
	innervardec();
	while (it->type == token_type::semicolon){
		it++;
		innervardec();
	}
}
void synAna::statement_chain()		//语句串
{
	statement();
	while (it->type != token_type::rbrace){
		statement();
	}
}
void synAna::innervardec()			//内部变量声明
{
	if (strcmp(it->word, "int") == 0){
		it++;
		if (it->type == token_type::ID){
			variable v = find_in_table(curtable, it->word);
			if (v.is_var != -1)
				ERROR(it->linenum, "变量重复定义");
			enter(curtable, it->word, 1, 0);
			it++;
			while (it->type == token_type::comma){
				it++;
				if (it->type == token_type::ID){
					variable v = find_in_table(curtable, it->word);
					if (v.is_var != -1)
						ERROR(it->linenum, "变量重复定义");
					enter(curtable, it->word, 1, 0);
					it++;
					continue;
				}
				else{
					ERROR(it->linenum, "声明语句出错");
				}
			}
			if (it->type == token_type::semicolon){
				return;
			}
			ERROR(it->linenum, "缺少分号");
		}
		ERROR(it->linenum, "声明语句出错");
	}
}
void synAna::statement()			//语句
{
	if (strcmp(it->word,"if") == 0){
		is_boolexp = true;
		it++;
		if_statement();
		return;
	}
	else if (strcmp(it->word, "while") == 0){
		is_boolexp = true;
		it++;
		while_statement();
		return;
	}
	else if (strcmp(it->word, "return") == 0){
		is_boolexp = false;
		this->has_return = true;
		it++;
		return_statement();
		return;
	}
	is_boolexp = false;
	assignment_statement();
}
void synAna::if_statement()		//if语句
{
	if (it->type == token_type::lbracket){
		it++;
		expression();
		if (it->type == token_type::rbracket){
			it++;
			M1 = this->intercode[intercode.size() - 1].codeno + 1;
			backpatch(intercode, truelist, M1);
			label_arr.push_back(M1);
			statement_block();
			M2 = this->intercode[intercode.size() - 1].codeno + 1;
			if (strcmp(it->word, "else") == 0){
				variable arg = { "", 2 };
				generate_intermediate_code(intercode, "j", arg, arg, arg);
				nextlist.push_back(this->intercode[intercode.size() - 1].codeno);
				M2++;
				backpatch(intercode, falselist, M2);
				label_arr.push_back(M2);
				it++;
				statement_block();
				N = this->intercode[intercode.size() - 1].codeno + 1;
				backpatch(intercode, nextlist, N);
				label_arr.push_back(N);
			}
			else{
				backpatch(intercode, falselist, M2);
				label_arr.push_back(M2);
			}
			return;
		}
		ERROR(it->linenum, "缺少右括号");
	}
	ERROR(it->linenum, "if缺少左括号");
}
void synAna::while_statement()		//while语句
{
	if (it->type == token_type::lbracket){
		it++;
		M1 = this->intercode[intercode.size() - 1].codeno + 1;
		expression();
		if (it->type == token_type::rbracket){
			it++;
			M2 = this->intercode[intercode.size() - 1].codeno + 1;
			backpatch(intercode, truelist, M2);
			label_arr.push_back(M2);
			statement_block();
			variable arg = { "", 2 }, result = { "", 2};
			sprintf(result.varname, "%d", M1);
			generate_intermediate_code(intercode, "j", arg, arg, result);
			label_arr.push_back(M1);
			N = this->intercode[intercode.size() - 1].codeno + 1;
			backpatch(intercode, falselist, N);
			label_arr.push_back(N);
			return;
		}
		ERROR(it->linenum, "缺少右括号");
	}
	ERROR(it->linenum, "while缺少左括号");
}
void synAna::return_statement()	//return语句
{
	variable v = expression();
	if (curtable.rettype == INT && strcmp(v.varname, "") == 0){
		ERROR(it->linenum, "int型函数不可返回空");
	}
	else if (curtable.rettype == VOID && strcmp(v.varname, "") != 0 && strcmp(v.varname, "void") == 0){
		ERROR(it->linenum, "void型函数不可返回int");
	}
	if (it->type == token_type::semicolon){
		it++;
		variable arg = { "", 1 };
		generate_intermediate_code(intercode, "RET", v, arg, arg);
		return;
	}
	ERROR(it->linenum, "缺少分号");
}
void synAna::assignment_statement()//赋值语句
{
	if (it->type == token_type::ID){
		Token token(*it);
		variable v1 = find_in_table(curtable, it->word), v2 = find_in_table(maintable, it->word);
		if (v1.is_var==-1 && v2.is_var==-1)
			ERROR(it->linenum, "变量未定义");
		it++;
		if (it->type == token_type::assignment){
			it++;
			variable v = expression();
			variable arg2 = { "", 1 }, result;
			result.is_var = 1;
			strcpy(result.varname, token.word);
			generate_intermediate_code(intercode, "=", v, arg2, result);
			if (it->type == token_type::semicolon){
				it++;
				return;
			}
			ERROR(it->linenum, "缺少分号");
		}
		ERROR(it->linenum, "赋值语句出错"); 
	}
}
variable synAna::expression()			//表达式
{
	variable v = plus_expression();
	variable arg = { "", 2 };
	variable v2;
	if(it->type == token_type::relop){
		is_boolexp = false;
		Token relop(*it);
		it++;
		v2 = plus_expression();
		char op[10]="j";
		strcat(op, relop.word);
		generate_intermediate_code(intercode, op, v, v2, arg);
		this->truelist.push_back(this->intercode[intercode.size() - 1].codeno);//真出口链表
		generate_intermediate_code(intercode, "j", arg, arg, arg);//无条件转移
		this->falselist.push_back(this->intercode[intercode.size() - 1].codeno);//假出口链表
	}
	else{
		if (is_boolexp){
			generate_intermediate_code(intercode, "jnz", v, arg, arg);
			this->truelist.push_back(this->intercode[intercode.size() - 1].codeno);//真出口链表
			generate_intermediate_code(intercode, "j", arg, arg, arg);//无条件转移
			this->falselist.push_back(this->intercode[intercode.size() - 1].codeno);//假出口链表
		}
	}
	return v;
}
variable synAna::plus_expression()		//加法表达式
{
	variable v = term();
	variable v2;
	while (it->type == token_type::plus){
		Token plus(*it);
		it++;
		v2 = term();
		char newname[100];
		sprintf(newname, "T%d", curvarno);
		enter(curtable, newname, 1, 0, 1);
		curvarno++;
		variable newvar;
		strcpy(newvar.varname, newname);
		newvar.is_var = 1;
		generate_intermediate_code(intercode, plus.word, v, v2, newvar);
		v = newvar;
	}
	return v;
}
variable synAna::term()				//项
{
	variable v = factor();
	variable v2;
	while (it->type == token_type::multiple){
		Token multiple(*it);
		it++;
		v2 = factor();
		char newname[100];
		sprintf(newname, "T%d", curvarno);
		enter(curtable, newname, 1, 0, 1);
		curvarno++;
		variable newvar;
		strcpy(newvar.varname, newname);
		newvar.is_var = 1;
		generate_intermediate_code(intercode, multiple.word, v, v2, newvar);
		v = newvar;
	}
	return v;
}
variable synAna::factor()				//因子
{
	variable v = {"",1};
	if (it->type == token_type::num){
		strcpy(v.varname, it->word);
		v.is_var = 0;
		it++;
		return v;
	}
	else if (it->type == token_type::lbracket){
		it++;
		v = expression();
		if (it->type == token_type::rbracket){
			it++;
			return v;
		}
		ERROR(it->linenum, "缺少右括号");
	}
	else if (it->type == token_type::ID){
		Token token(*it);
		it++;
		v = call(token.word);
		return v;
	}
	return v;
}
variable synAna::call(char word[])				//call
{
	variable v;
	if (it->type == token_type::lbracket){
		vartable functable;
		functable = find_func_in_table(maintable, word);
		if (functable.param_num == -1)
			ERROR(it->linenum, "函数未定义");
		else if (functable.rettype==VOID)
			ERROR(it->linenum, "void型函数不可作为右值");
		char newname[100];
		sprintf(newname, "T%d", curvarno);
		enter(curtable, newname, 1, 0, 1);
		curvarno++;
		strcpy(v.varname, newname);
		v.is_var = 1;
		variable arg1 = { "", 1}, arg2 = { "", 1 }, result;
		strcpy(arg1.varname, word);
		result.is_var = 1;
		strcpy(result.varname, word);
		it++;
		if(functable.param_num != argument_list(word))
			ERROR(it->linenum, "参数个数不匹配");
		if (it->type == token_type::rbracket){
			it++;
			generate_intermediate_code(intercode, "call", arg1, arg2, v);
			return v;
		}
		ERROR(it->linenum, "缺少右括号");
	}
	variable v1 = find_in_table(curtable, word), v2 = find_in_table(maintable, word);
	if (v1.is_var == -1 && v2.is_var == -1)
		ERROR(it->linenum, "变量未定义");
	strcpy(v.varname, word);
	v.is_var = 1;
	v.is_temp = 0;
	return v;
}
int synAna::argument_list(const char* funcname)		//实参列表
{
	return argument(funcname);
}
int synAna::argument(const char* funcname)			//实参
{
	int paramnum = 0;
	variable retvar = expression();
	if (strcmp(retvar.varname, "") != 0){
		paramnum++;
		variable arg1 = { "", 1 }, result = { "", 1};
		strcpy(arg1.varname, funcname);
		generate_intermediate_code(intercode, "push", arg1, retvar, result);
	}
	while (it->type == token_type::comma){
		if (strcmp(retvar.varname, "") == 0){
			ERROR(it->linenum, "多个实参不可为空");
		}
		it++;
		retvar = expression();
		if (strcmp(retvar.varname, "") != 0){
			paramnum++;
			variable arg1 = { "", 1 }, result = { "", 1 };
			strcpy(arg1.varname, funcname);
			generate_intermediate_code(intercode, "push", arg1, retvar, result);
		}
	}
	return paramnum;
}
void synAna::ERROR(int linenum, char errinfo[])				//出错
{
	char errstr[200];
	if (linenum>=0)
		sprintf(errstr, "第%d行\t: %s\n", linenum, errinfo);
	else
		sprintf(errstr, "%s\n", errinfo);
	fprintf(synresult, errstr);
	fflush(synresult);
	printf("程序出错，信息如下：\n%s", errstr);
	getchar();
	exit(0);
}