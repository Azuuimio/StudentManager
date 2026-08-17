//ui.c —— 菜单交互与安全输入

//引入自定义头文件
#include "ui.h"
#include "student.h"

//引入标准库头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <windows.h> 

//定义宏
#define DATA_FILE "students.dat"	//数据文件名
#define INPUT_BUF_LEN 128			//输入缓冲区长度

//函数声明
static char* trim(char* s);
static char* read_line(const char* prompt, char* buf, size_t size);
static int read_int(const char* prompt, int min, int max, int* out);

//函数：去除字符串首尾的空白字符
//返回值：去除首尾空白字符后的字符串
static char *trim (char* s){
	//去除首部空白字符
	while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') {
		s++;
	}
	//定位字符串末尾
	char* end;
	end = s + strlen(s);
	//去除尾部空白字符
	while (end > s && (*(end - 1) == ' ' || *(end - 1) == '\t' || *(end - 1) == '\n' || *(end - 1) == '\r')) {
		end--;
		*end = '\0';	//在末尾添加字符串结束符
	}
	return s;	//返回去除首尾空白字符后的字符串
}
//关于字符串字面量：
/*
字符串字面量是只读的，不能传给会修改内容的函数，否则程序会异常退出，
如果要测试trim函数，必须使用字符数组(写法二)而不是字符串字面量(写法一)
写法一：printf("%s\n", "测试字符串");
写法二：char s[] = "测试字符串";
		printf("%s\n", trim(s));
*/

//函数：安全输入字符串
//返回值：去除首尾空白字符后的字符串，输入失败返回NULL
static char* read_line(const char* prompt, char* buf, size_t size) {
	//确保size不会超过int的最大值，防止fgets函数参数溢出
	size_t safe_size = (size > INT_MAX) ? INT_MAX : size;
	if (prompt != NULL) {
		printf("%s", prompt);
		fflush(stdout);	//手动冲刷，确保提示信息立即输出
	}
	if (fgets(buf, (int)safe_size, stdin) == NULL){
		return NULL;	//输入失败，返回NULL
	}
	//下方if语句用于处理输入超出缓冲区大小的情况
	//strchr函数用于查找字符串中是否包含换行符，如果不包含则说明输入超出缓冲区大小
	//feof函数确认不是因为到了文件末尾才没读到换行符
	//feof函数返回值：读到文件末尾→返回非零值(true)；没读到→返回0(false)
	if (strchr(buf, '\n') == NULL && !feof(stdin)) {
		int ch;
		//清空输入缓冲区，直到遇到换行符或文件末尾
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
	//与fgets配套的操作：strcspn（去除换行符）
	buf[strcspn(buf, "\r\n")] = '\0';
	//去除首尾空白字符后返回
	return trim(buf);	
}

//函数：安全输入整数
//
static int read_int(const char* prompt, int min, int max, int* out){
	char buf[INPUT_BUF_LEN];
	char* line;


	for (;;) {
		if ((*line = read_line) == NULL) {
			return 0;
		}
		
	}
}