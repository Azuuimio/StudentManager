/*
学生信息包含：学号、姓名、年龄、成绩
要实现的功能及其界面如下↓

========== 学生信息管理系统 ==========
1. 添加学生
2. 删除学生
3. 修改学生信息
4. 按学号查询
5. 按姓名查询
6. 显示全部学生
7. 排序显示
8. 保存到文件
9. 从文件加载
0. 退出
======================================
请选择功能：
*/

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

//引入自定义头文件
#include "student.h"
#include "ui.h"

//引入标准库头文件
#include <stdio.h>
#include <windows.h>

int main()
{
	//设置输入输出编码为UTF-8
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	ui_run();

	//测试5（三种输入）
	/*char name[128];
	char* trimmed_name;
	int age;
	double score;
	if (trimmed_name = read_line("请输入姓名：", name, 128))
		printf("姓名：%s\n", trimmed_name);
	if (read_int("请输入年龄（1-150）：", 1, 150, &age))
		printf("年龄：%d\n", age);
	if (read_double("请输入成绩（0-100）：", 0.0, 100.0, &score))
		printf("成绩：%.1f\n", score);*/

	//测试4（学生列表打印）
	/*StudentList list;
	list_init(&list);
	Student s[3] = {
		{"2023001", "张三", 20, 90.5},
		{"2023002", "李四", 21, 85.0},
		{"2023003", "王五", 19, 92.0}
	};
	list_add(&list, &s[0]);
	list_add(&list, &s[1]);
	list_add(&list, &s[2]);
	list_print(&list);*/

	//测试3（添加、查找、删除学生）
	/*StudentList list;
	list_init(&list);
	Student s[3] = {
		{"2023001", "张三", 20, 90.5},
		{"2023002", "李四", 21, 85.0},
		{"2023003", "王五", 19, 92.0}
	};
	list_add(&list, &s[0]);
	printf("添加学生：%s\n", s[0].name);
	list_add(&list, &s[1]);
	printf("添加学生：%s\n", s[1].name);
	printf("\n");
	printf("查找张三：%s\n", list_find_by_id(&list, "2023001") == NULL ? "未找到" : "找到");
	printf("查找李四：%s\n", list_find_by_id(&list, "2023002") == NULL ? "未找到" : "找到");
	printf("查找王五：%s\n", list_find_by_id(&list, "2023003") == NULL ? "未找到" : "找到");
	printf("\n");
	list_remove_by_id(&list, "2023001");
	printf("删除学生：%s\n", s[0].name);
	printf("\n");
	printf("查找张三：%s\n", list_find_by_id(&list, "2023001") == NULL ? "未找到" : "找到");
	printf("查找李四：%s\n", list_find_by_id(&list, "2023002") == NULL ? "未找到" : "找到");
	printf("查找王五：%s\n", list_find_by_id(&list, "2023003") == NULL ? "未找到" : "找到");*/

	//测试2（初始化、扩容）
	/*StudentList list;
	Student stu = { "2023001", "张三", 18, 90.0 };
	list_init(&list);
	int return_value;
	printf("capacity = %zu\n", list.capacity);
	printf("size = %zu\n", list.size);
	printf("------------------\n");
	return_value = list_grow(&list);
	printf("return_value = %d\n", return_value);
	printf("capacity = %zu\n", list.capacity);
	printf("size = %zu\n", list.size);
	printf("------------------\n");
	for (int i = 1; i < 10; i++) {
		return_value = list_add(&list, &stu);
		printf("return_value = %d\n", return_value);
		printf("capacity = %zu\n", list.capacity);
		printf("size = %zu\n", list.size);
		printf("------------------\n");
		stu.id[7] = '0' + i;
	}*/

	//测试1（学生结构体和封装）
	/*printf("你好，学生信息管理系统！\n");
	Student s[3] = {
		{"2023001", "张三", 20, 90.5},
		{"2023002", "李四", 21, 85.0},
		{"2023003", "王五", 19, 92.0}
	};
	StudentList list;
	list.data = s;
	printf("sizeof(Student) = %zu\n", sizeof(Student));
	printf("sizeof(StudentList) = %zu\n", sizeof(StudentList));
	printf("学号：\t%s\n", list.data->id);
	printf("姓名：\t%s\n", list.data->name);
	printf("年龄：\t%d\n", list.data->age);
	printf("成绩：\t%.2f\n", list.data->score);*/

	return 0;
}