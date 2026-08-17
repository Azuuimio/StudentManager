//student.h —— 学生数据模型与动态数组容器（接口声明）

//头文件保护
#ifndef STUDENT_H
#define STUDENT_H

//引入标准库头文件
#include <stddef.h>

//定义宏
#define STUDENTS_ID_LEN	16		//学号长度
#define STUDENTS_NAME_LEN 32	//姓名长度

//定义学生结构体
typedef struct {
	char   id[STUDENTS_ID_LEN];		//学号
	char   name[STUDENTS_NAME_LEN];	//姓名
	int    age;						//年龄
	double score;					//成绩
}Student;

//定义学生列表结构体（容器封装）
typedef struct {
	Student* data;		//学生数组指针
	size_t   size;		//当前学生数量
	size_t   capacity;	//最大学生数量
}StudentList;

/*--------以下为函数声明--------*/

//初始化容器
void list_init(StudentList* list);

//释放容器内存
void list_free(StudentList* list);	

//添加学生 (返回0代表成功，-1代表业务失败，-2代表系统失败)
int  list_add (StudentList* list, const Student* stu);

//根据学号查找 (找到返回记录地址，找不到返回“NULL”)
Student* list_find_by_id(const StudentList* list, const char* id);	

//删除学生（返回0代表未找到指定学号的学生，1代表删除成功）
int list_remove_by_id(StudentList* list, const char* id);

//按成绩排序（参数descending为1表示降序，为0表示升序）
void list_sort_by_score(StudentList* list, int descending);

//打印单个学生信息
void student_print(const Student* stu);

//打印学生列表
void list_print(const StudentList* list);

#endif
