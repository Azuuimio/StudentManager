//引入自定义头文件
#include "student.h"
#include "ui.h"

//引入标准库头文件
#include <stdio.h>
#include <windows.h>

int main()
{
	//关于中文乱码问题：
	/*
	本工程的源文件默认以UTF-8无BOM格式保存，而MSVC在中文Windows上默认按GBK解码没有
	BOM的文件，UTF-8中一个汉字占3字节，GBK中一个汉字只占2字节，MSVC按GBK两两分组解读
	UTF-8字节流时，落单的字节会和后面的引号、换行符拼在一起，吃掉字符串的结束引号，
	于是报出一堆位置离谱的语法错误，例如：
	C2001  常量中有换行符
	C2143  语法错误 : 缺少";"(在"..."的前面)
	C2018  未知字符 '0x...'
	报错位置往往集中在含中文的行附近，但代码本身没有任何语法问题。即使侥幸编译通过，
	运行时中文也会全部乱码，解决方法有两种：
	方法一：右键项目→属性→C/C++→命令行→在"附加选项"中输入/utf-8→确定
	方法二：在VS2022 中打开文件→文件→高级保存选项→编码选"Unicode(UTF-8带签名)-代
	码页65001"→保存，需要对每个.c/.h文件单独操作一次
	除此之外，SetConsoleOutputCP/SetConsoleCP也是必不可少的，按UTF-8解码解决的是编译
	时的编码识别问题，SetConsoleOutputCP/SetConsoleCP解决的是运行时控制台的编码解释
	问题，两者缺一不可
	*/
	//设置输入输出编码为UTF-8
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	//启动菜单
	ui_run();

	return 0;
}