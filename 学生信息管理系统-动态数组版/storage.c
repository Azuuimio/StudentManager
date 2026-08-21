//storage.c —— 文件读写

//引入自定义头文件
#include "storage.h"

//引入标准库头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//定义宏（u后缀表示unsigned）
#define FILE_MAGIC     "STU1"		//魔数
#define FILE_VERSION   1u			//文件格式版本号
#define MAX_RECORDS    1000000u		//记录数上限

//定义文件头结构体
typedef struct {
	char magic[4];		//魔数
	uint32_t version;	//文件格式版本号
	uint32_t count;		//记录数
}FileHeader;

//函数声明
int storage_save(const char* filename, const StudentList* list);
int storage_load(const char* filename, StudentList* list);

//函数：将容器中的全部记录保存到文件
//返回值：0代表成功，-1代表参数非法，-2代表文件无法打开或写入
int storage_save(const char* filename, const StudentList* list) {
	FILE* fp;
	FileHeader header;
	if (filename == NULL || list == NULL) {
		return -1;
	}
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		return -2;
	}
	memcpy(header.magic, FILE_MAGIC, 4);
	header.version = FILE_VERSION;
	header.count = (uint32_t)list->size;
	//写入文件头
	if (fwrite(&header, sizeof(header), 1, fp) != 1) {
		return -2;
	}
	//写入学生数据
	if (list->size > 0) {
		size_t written = fwrite(list->data, sizeof(Student), list->size, fp);
		if (written != list->size) {
			fclose(fp);
			return -2;
		}
	}
	if (fclose(fp) != 0) {
		return -2;
	}
	return 0;
}

//函数：从文件加载记录并替换容器现有内容。
//返回值：0代表成功，-1代表参数非法，-2代表文件无法读取，-3代表文件格式不符
int storage_load(const char* filename, StudentList* list) {
	FILE* fp;
	FileHeader header;
	StudentList tmp;
	if (filename == NULL || list == NULL) {
		return -1;
	}
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return -2;
	}
	//检验文件头
	if (fread(&header, sizeof(header), 1, fp) != 1 ||	//读取文件头
		memcmp(header.magic, FILE_MAGIC, 4) != 0 ||		//检验魔数
		header.version != FILE_VERSION ||				//检验文件格式版本号
		header.count > MAX_RECORDS) {					//检验记录数不超限
		fclose(fp);
		return 3;
	}
	//读取学生数据：先用临时容器接收数据，确认无误后将临时容器整体赋值给目标容
	//器，这样做可以保证加载失败时原数据完好无损
	list_init(&tmp);
	if (header.count > 0) {
		tmp.data = (Student*)malloc((size_t)header.count * sizeof(Student));
		if (tmp.data == NULL) {
			fclose(fp);
			return -2;
		}
		tmp.size = header.count;
		tmp.capacity = header.count;
		if (fread(tmp.data, sizeof(Student), header.count, fp) != header.count) {
			list_free(&tmp);
			fclose(fp);
			return -3;
		}
	}
	fclose(fp);
	list_free(list);
	*list = tmp;
	return 0;
} 