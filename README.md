# C语言练习项目：学生信息管理系统

基于**动态数组**或**链表**（尚未完成）实现的控制台学生信息管理系统

## 运行示例

<img width="1738" height="933" alt="屏幕截图" src="https://github.com/user-attachments/assets/1cdedb45-1ea5-49e3-8a80-64eaae20b524" />

## 常见问题（重要）：C4996 报错

MSVC 认为部分标准 C 函数（如fopen、scanf、strcpy等）“不安全”，默认会给出 C4996 警告。如果项目开启了将警告视为错误（/WX），警告就会升级为错误，导致编译失败。

### 解决方法

在项目属性中添加预处理器定义 `_CRT_SECURE_NO_WARNINGS`：

1. 右键项目 → **属性**

2. **C/C++** → **预处理器** → **预处理器定义**

3. 下拉框选择 **“编辑…”**

4. 新增一行：

   ```
   _CRT_SECURE_NO_WARNINGS
   ```

5. 确定保存后重新编译即可。

## 环境

| 项目 | 版本 |
| --- | --- |
| 操作系统 | Windows 11 |
| IDE | Visual Studio Community 2022 |
| 编译器 | MSVC v143 工具集 |

