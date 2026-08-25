//student.c —— 学生数据模型与链表容器

//引入自定义头文件
#include "student.h"

//引入标准库头文件
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//定义宏
#define COL_ID 12			//学号列宽
#define COL_NAME 12			//姓名列宽
#define COL_AGE 12			//年龄列宽
#define SEPARATOR_WIDTH 41	//分隔线宽

//函数声明
static int compare_score_desc(const void* a, const void* b);
static int compare_score_asc(const void* a, const void* b);
static int compare_id_asc(const void* a, const void* b);
static int display_width(const char* s);
static void print_padded(const char* s, int width);

//函数：初始化容器
void list_init(StudentList* list) {
	//检测list是否为NULL，不假设调用方永远正确
	if (list == NULL) {		
		return;
	}
	list->head = NULL;		
	list->tail = NULL;		
	list->size = 0;
}
 
//函数：释放容器内存
void list_free(StudentList* list) {
	//检测list是否为NULL，不假设调用方永远正确
	if (list == NULL) {		
		return;
	}
	StudentNode* cur = NULL;
	StudentNode* next = NULL;
	for (cur = list->head; cur != NULL; cur = next) {
		next = cur->next;
		free(cur);
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

//函数：根据学号查找
//返回值：找到返回记录地址，找不到返回NULL
//注意：返回的指针在下一次增删或排序后可能失效，不可长期保存
Student* list_find_by_id(const StudentList* list, const char* id) {
	if (list == NULL || id == NULL) {
		return NULL;	//参数错误
	}
	StudentNode* cur;
	for (cur = list->head; cur != NULL; cur = cur->next){
		if (strcmp(cur->data.id, id) == 0){
			return &cur->data;	//找到，返回记录地址
		}
	}
	return NULL;	//找不到			
}

//函数：根据姓名查找（模糊匹配）
//返回值：找到返回指针数组，参数为空或内存不足时返回NULL，*out_count为匹配数量
//注意：调用方负责free
Student** student_list_find_by_name(const StudentList* list, const char* name, size_t* out_count) {
	if (list == NULL || name == NULL || name[0] == '\0' || out_count == NULL) {
		return NULL;	//参数错误
	}
	*out_count = 0;
	size_t cap = (list->size > 0) ? list->size : 1;		 //避免malloc(0)
	Student** results = malloc(cap * sizeof(*results));
	if (results == NULL) {
		return NULL;	//内存不足
	}
	size_t count = 0;
	StudentNode* cur;
	for (cur = list->head; cur != NULL; cur = cur->next) {
		if (strstr(cur->data.name, name) != NULL) {
			results[count] = &cur->data;
			count++;
		}
	}
	*out_count = count;
	return results;
}

//函数：添加学生 
//返回值：返回0代表成功，-1代表学号已存在，-2代表内存不足，1代表参数错误
int list_add(StudentList* list, const Student* stu) {
	if (list == NULL || stu == NULL) {
		return 1;	//参数错误
	}
	if (list_find_by_id(list, stu->id) != NULL) {
		return -1;	//学号已存在
	}
	StudentNode* node = malloc(sizeof(StudentNode));
	if (node == NULL) {
		return -2;	//内存不足
	}
	node->data = *stu;
	node->next = NULL;
	if (list->tail == NULL) {	//若是空链表：新节点既是头也是尾
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
	return 0;	//成功
}

//函数：删除学生
//返回值：返回0代表删除成功，-1代表未找到指定学号的学生，1代表参数错误
int list_remove_by_id(StudentList* list, const char* id) {
	if (list == NULL || id == NULL) {
		return 1;	//参数错误
	}
	StudentNode* prev = NULL;
	StudentNode* cur;
	for (cur = list->head; cur != NULL; prev = cur, cur = cur->next) {
		if (strcmp(cur->data.id, id) == 0) {
			if (prev == NULL) {			//删的是头节点
				list->head = cur->next;
			} else {
				prev->next = cur->next;
			}
			if (cur == list->tail) {
				list->tail = prev;
			}
			free(cur);
			list->size--;
			return 0;

		}
	}
	return -1;	//未找到指定学号的学生
}

//函数：按成绩降序排序的比较函数（传入qsort）
static int compare_score_desc(const void* a, const void* b) {
	//void可以指向任何类型，但正因为它不知道指向什么类型，所以不能解引用、不能访
	//问成员、不能做指针算术,所以要做强制类型转换
	const Student* sa = (const Student*)a;
	const Student* sb = (const Student*)b;
	//浮点数在内存里是近似存储的，直接相减再取符号，遇到极小差值时符号可能出错，
	//而“<”和“>”是浮点数的原生比较，没有减法，没有精度损失，所以采用三分支写法
	if (sa->score < sb->score) return 1;
	if (sa->score > sb->score) return -1;
	return 0;
}
//关于qsort比较函数的返回值规则：
/*
返回负数：	a排在b前
返回零：	a与b相等
返回正数：	a排在b后
*/

//函数：按成绩升序排序的比较函数（传入qsort）
static int compare_score_asc(const void* a, const void* b) {
	return compare_score_desc(b, a);	//升序排序就是降序排序的相反
}

//函数：按成绩排序
//参数：descending为1表示降序，为0表示升序
void list_sort_by_score(StudentList* list, int descending) {
	if (list == NULL || list->size < 2) {
		return;	//参数错误或无需排序
	}
	//使用临时数组进行排序
	Student* tmp = malloc(list->size * sizeof(Student));
	if (tmp == NULL) return;
	StudentNode* cur;
	size_t i;
	for (cur = list->head, i = 0; cur != NULL; cur = cur->next, i++) {
		tmp[i] = cur->data;
	}
	if (descending) {
		qsort(tmp, list->size, sizeof(Student), compare_score_desc);
	} else {
		qsort(tmp, list->size, sizeof(Student), compare_score_asc);
	}
	//把临时数组写回链表
	for (cur = list->head, i = 0; cur != NULL; cur = cur->next, i++) {
		cur->data = tmp[i];
	}
	free(tmp);
}

//函数：按学号排序的比较函数（传入qsort）
static int compare_id_asc(const void* a, const void* b) {
	const Student* sa = (const Student*)a;
	const Student* sb = (const Student*)b;
	//strcmp的返回值语义与qsort比较函数完全一致，直接返回即可
	return strcmp(sa->id, sb->id);
}

//函数：按学号升序排序
void list_sort_by_id(StudentList* list) {
	if (list == NULL || list->size < 2) {
		return;	//参数错误或无需排序
	}
	//使用临时数组进行排序
	Student* tmp = malloc(list->size * sizeof(Student));
	if (tmp == NULL) return;
	StudentNode* cur;
	size_t i;
	for (cur = list->head, i = 0; cur != NULL; cur = cur->next, i++) {
		tmp[i] = cur->data;
	}
	qsort(tmp, list->size, sizeof(Student), compare_id_asc);
	//把临时数组写回链表
	for (cur = list->head, i = 0; cur != NULL; cur = cur->next, i++) {
		cur->data = tmp[i];
	}
	free(tmp);
}

//关于中文显示宽度：
/*
打印表格最自然的写法是printf("%-16s%-24s", id, name)，这在纯英文下工作良好，但一
旦姓名是中文就会错位，问题的根源在于中文显示宽度和字节数的差异：printf的%-24s按
字节数补齐，它数字符串占多少字节，不足24就补空格，而UTF-8编码下一个汉字占3个字
节，在控制台上却只显示2个字符宽度
解决办法是放弃%-Ns的自动填充，自己计算宽度并补空格，具体计算方法如下：
ASCII字符（<0x80）			占 1 列
UTF-8首字节（非10xx xxxx）	视为全角字符，占 2 列
UTF-8后续字节（10xx xxxx）	不占列（它属于前一个字符）
*/
//函数：计算字符串在终端显示的列宽
//返回值：字符串在终端显示的列宽
static int display_width(const char* s) {
	int width = 0;
	while (*s != '\0') {
		//以无符号方式安全地读取当前字节存入 c，同时将指针推进到下一个字节
		//（任何涉及字节值比较、位运算的场景，都必须用unsigned char，否则高位为1的字节会被当作负数）
		unsigned char c = (unsigned char)*s++;
		if (c < 0x80) {
			width += 1;	//ASCII字符占1列
		}
		//通过取余把c的低6位全部清零，只提取最高2位，用于判断是UTF-8首字节还是后续字节
		else if ((c & 0xC0) != 0x80) {
			width += 2;	//UTF-8首字节占2列
		} 
		//UTF-8后续字节不占列
	}
	return width;
}

//函数：打印指定字符串并补齐空格
static void print_padded(const char* s, int width) {
	if (s == NULL || width <= 0) {
		return;
	}
	int padding = width - display_width(s);
	printf("%s", s);
	for (int i = 0; i < padding; i++) {
		putchar(' ');
	}
}

//函数：打印学生列表表头
void student_print_header(void){
	print_padded("学号", COL_ID);
	print_padded("姓名", COL_NAME);
	print_padded("年龄", COL_AGE);
	printf("成绩\n");
}

//函数：打印学生列表分隔线
void student_print_separator(void) {
	for (int i = 0; i < SEPARATOR_WIDTH; i++) {
		putchar('-');
	}
	putchar('\n');
}

//函数：打印单个学生信息
void student_print(const Student* stu) {
	if (stu == NULL) {
		return;
	}
	//age是int类型，要先转换为字符串
	char age_str[32];
	snprintf(age_str, sizeof(age_str), "%d", stu->age);
	print_padded(stu->id, COL_ID);	
	print_padded(stu->name, COL_NAME);
	print_padded(age_str, COL_AGE);
	printf("%.1f\n", stu->score);
}

//函数：打印学生列表
void list_print(const StudentList* list) {
	if (list == NULL || list->size == 0) {
		printf("暂无学生记录\n");
		return;
	}
	StudentNode* cur;
	student_print_separator();
	student_print_header();
	for (cur = list->head; cur != NULL; cur = cur->next) {
		student_print(&cur->data);
	}
	student_print_separator();
	printf("共 %llu 条记录\n", (unsigned long long)list->size);
}




