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
static int read_double(const char* prompt, double min, double max, double* out);

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
//返回值：1表示成功获取合法整数（会将解析后的整数写入out）；0表示遇到EOF（如windows用户按下Ctrl+Z）
static int read_int(const char* prompt, int min, int max, int* out){
	char buf[INPUT_BUF_LEN];
	char* line;
	char* end;
	long value;
	//无限循环直到读到合法输入或遇到EOF
	for (;;) {
		line = read_line(prompt, buf, INPUT_BUF_LEN);
		if (line == NULL) {
			return 0;	//读取失败
		}
		//strtol转换前必须将errno清零
		errno = 0;
		//将输入值转化为十进制（&end为二级指针，strtol通过这个二级指针修改原始的end）
		value = strtol(line, &end, 10);	
		//四重检验
		if (errno == 0 &&	//检验未发生溢出
			end != line &&	//检验至少成功解析了一个字符（排除纯空白输入）
			*end == '\0' &&	//检验数字后没有垃圾字符（排除类似“123abc”的输入）
			value >= min && value <= max){	//检验数值范围
			*out = (int)value;
			return 1;	//读取成功
		}
		printf("输入无效，请输入%d-%d之间的整数。\n", min, max);
	}
}

//函数：安全输入浮点数
//返回值：1表示成功获取合法浮点数（会将解析后的浮点数写入out）；0表示遇到EOF（如windows用户按下Ctrl+Z）
static int read_double(const char* prompt, double min, double max, double* out) {
	char buf[INPUT_BUF_LEN];
	char* line;
	char* end;
	double value;
	//无限循环直到读到合法输入或遇到EOF
	for (;;) {
		line = read_line(prompt, buf, INPUT_BUF_LEN);
		if (line == NULL) {
			return 0;	//读取失败
		}
		//strtod转换前必须将errno清零
		errno = 0;
		//将输入值转化为double类型（&end为二级指针，strtod通过这个二级指针修改原始的end）
		value = strtod(line, &end);
		//四重检验
		if (errno == 0 &&	//检验未发生溢出
			end != line &&	//检验至少成功解析了一个字符
			*end == '\0' &&	//检验数字后没有垃圾字符
			value >= min && value <= max) {	//检验数值范围
			*out = value;
			return 1;	//读取成功
		}
		printf("输入无效，请输入%.0f-%.0f之间的数字。\n", min, max);
	}
}

