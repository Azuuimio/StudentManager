//storage.h —— 文件读写

//头文件保护
#ifndef STORAGE_H
#define STORAGE_H

//引入自定义头文件
#include "student.h"

//函数：将容器中的全部记录保存到文件
//返回值：0代表成功，-1代表参数非法，-2代表文件无法打开或写入
int storage_save(const char* filename, const StudentList* list);

//函数：从文件加载记录并替换容器现有内容。
//返回值：0代表成功，-1代表参数非法，-2代表文件无法读取，-3代表文件格式不符
int storage_load(const char* filename, StudentList* list);

#endif

