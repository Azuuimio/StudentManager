//student.h —— 学生数据模型与链表容器

//头文件保护
#ifndef STUDENT_H
#define STUDENT_H

//引入标准库头文件
#include <stddef.h>

//定义宏
#define STUDENTS_ID_LEN	10		//学号长度
#define STUDENTS_NAME_LEN 16	//姓名长度

//定义学生结构体
typedef struct {
	char   id[STUDENTS_ID_LEN];		//学号
	char   name[STUDENTS_NAME_LEN];	//姓名
	int    age;						//年龄
	double score;					//成绩
}Student;

//定义学生链表节点结构体
typedef struct StudentNode StudentNode;
struct StudentNode {
	Student data;					//学生信息
	StudentNode* next;		//下一个节点的地址
};

//定义学生链表容器
typedef struct {
	StudentNode* head;		//第一个节点的地址
	StudentNode* tail;		//最后一个节点的地址
	size_t size;			//节点个数
}StudentList;

//以下为函数声明
/*------------------------------*/
//函数：初始化容器
void list_init(StudentList* list);
//函数：释放容器内存
void list_free(StudentList* list);	
/*------------------------------*/
//函数：添加学生 
//返回值：返回0代表成功，-1代表学号已存在，-2代表内存不足，1代表参数错误
int list_add (StudentList* list, const Student* stu);	
//函数：删除学生
//返回值：返回0代表删除成功， -1代表未找到指定学号的学生，1代表参数错误
int list_remove_by_id(StudentList* list, const char* id);
//函数：根据学号查找
//返回值：找到返回记录地址，找不到返回NULL
//注意：返回的指针在下一次增删或排序后可能失效，不可长期保存
Student* list_find_by_id(const StudentList* list, const char* id);
//函数：根据姓名查找（模糊匹配）
//返回值：找到返回指针数组，参数为空或内存不足时返回NULL，*out_count为匹配数量
//注意：调用方负责free
Student** student_list_find_by_name(const StudentList* list, const char* name, size_t* out_count);
/*------------------------------*/
//函数：按成绩排序
//参数：descending为1表示降序，为0表示升序
void list_sort_by_score(StudentList* list, int descending);
//函数：按学号升序排序
void list_sort_by_id(StudentList* list);
/*------------------------------*/
//函数：打印学生列表表头
void student_print_header(void);
//函数：打印学生列表分割线
void student_print_separator(void);
//函数：打印单个学生信息
void student_print(const Student* stu);
//函数：打印学生列表
/*------------------------------*/

#endif
