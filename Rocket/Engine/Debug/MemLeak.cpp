// https://github.com/YunFei-S/Memory-leak-detector/blob/main/LeakDetector.cpp
// With Smart Pointer Error
//#define NEW_OVERLOAD_IMPLEMENTATION_
#include "Debug/MemLeak.h"
#include "Debug/Log.h"

#if defined(RK_WINDOWS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <new>
#include <cstdio>
#include <iostream>
#include <cstring>

#ifdef RK_DEBUG
// 初始化 LeakDetector类中定义的静态变量
size_t LeakDetector::call_count_ = 0;

// 我们使用带头节点的双向链表来手动管理内存申请与释放, 头节点的_prev指向最后一个结点, _next指向第一个结点
// 双向链表结构
typedef struct MemoryList {
	struct MemoryList* prev = nullptr;
	struct MemoryList* next = nullptr;
	size_t size;				// operator new( )申请的内存大小
	bool   is_array;			// 是否为申请数组(即使用operator new[]( ) 而不是 operator new( ))
	char*  file;				// 如果有, 存储存在内存泄漏文件的文件信息
	size_t line;				// 存储存在内存泄漏位置的行号
} MemoryList;

// 创建一个头结点, 它的前后指针均初始化为指向自己(插入、删除双向链表中结点 和 _LeakDetector( )函数中遍历双向链表时, 这样初始化的作用就体现出来了)。使用静态变量使其只在本文件内有效
// 我们只使用这个头节点的 prev 和 next 成员
static MemoryList memory_list_head = { &memory_list_head, &memory_list_head, 0, false, nullptr, 0 };

// 保存未释放的内存大小
static size_t memory_allocated = 0;

// 对双向链表采用头插法分配内存
static void* AllocateMemory(size_t size, bool array, char const* file, size_t line) {
	//printf("Alllocate Memory\n");
	// 我们需要为我们管理内存分配的 MemoryList结点 也申请内存
	// 计算新的大小
	size_t new_size = size + sizeof(MemoryList);

	// 把接收到的地址强转为 MemoryList*, 以便我们后续操作
	// 由于重载了new, 所以我们使用 malloc 来申请内存
	MemoryList* new_elem = (MemoryList*)malloc(new_size);

	// 更新MemoryList结构成员的值
	new_elem->next = memory_list_head.next;
	new_elem->prev = &memory_list_head;
	new_elem->size = size;						// 注意, 此处为size而不是newSize. 因为我们管理记录的是 new申请的内存, 验证它是否未释放, 存在内存泄漏问题. 申请 newSize的内存(为 MemoryList结点多申请出的内存), 只是为了实现手动管理内存所必须, 这个内存我们一定会释放, 不需关注. 所以保存 时用size而不是newSize
	new_elem->is_array = array;

	// 如果有文件信息, 则保存下来
	if (nullptr != file) {
		new_elem->file = (char*)malloc(strlen(file) + 1);
		strcpy(new_elem->file, file);
	}
	else {
		new_elem->file = nullptr;
	}
	// 保存行号
	new_elem->line = line;
	
	// 更新双向链表结构
	memory_list_head.next->prev = new_elem;
	memory_list_head.next = new_elem;

	// 更新未释放的内存数
	// 我们管理的只是 new申请的内存. 为memory_list_head结点多申请的内存,和为保存文件信息多申请内存无关, 这些内存我们一定会释放, 所以这里只记录size
	memory_allocated += size;

	// 返回new 申请的内存地址
	// 将new_elem强转为char* 类型(保证指针+1时每次加的字节数为1) + memory_list_head所占用字节数( 总共申请的newSize字节数 减去memory_list_head结点占用的字节数, 即为new申请的字节数 )
	return (char*)new_elem + sizeof(memory_list_head);
}

// 对双向链表采用头删法手动管理释放内存
// 注意: delete/delete[]时 我们并不知道它操作的是双向链表中的哪一个结点
static void DeleteMemory(void* ptr, bool array) {
	if(ptr == nullptr) {
		printf("try to free nullptr");
		return;
	}
    //printf("Delete Memory\n");
	// 注意, 堆的空间自底向上增长. 所以此处为减
	MemoryList* cur_elem = (MemoryList*)((char*)ptr - sizeof(MemoryList));

	// 如果 new/new[] 和 delete/delete[] 不匹配使用. 直接返回
	if (cur_elem->is_array != array)
		return;

	// 更新链表结构
    cur_elem->next->prev = cur_elem->prev;
    cur_elem->prev->next = cur_elem->next;

	// 更新memory_allocated值
	memory_allocated -= cur_elem->size;

	// 如果cur_elem->_file不为NULL, 释放保存文件信息时申请的内存
	if (NULL != cur_elem->file)
		free(cur_elem->file);

	// 释放内存
	free(cur_elem);

	ptr = nullptr;
}

// 我们定义的最后一个静态对象析构时调用此函数, 判断是否有内存泄漏, 若有, 则打印出内存泄漏信息
void LeakDetector::LeakDetection() {
	if (0 == memory_allocated) {
		std::cout << "No Memory Leak!" << std::endl;
		return;
	}

	// 存在内存泄漏
	// 记录内存泄漏次数
	size_t count = 0;

	// 若不存在内存泄漏, 则双向链表中应该只剩下一个头节点
	// 若存在内存泄漏, 则双向链表中除头节点之外的结点都已泄露，个数即内存泄漏次数
	MemoryList* ptr = memory_list_head.next;
	while ((NULL != ptr) && (&memory_list_head != ptr)) {
		if (true == ptr->is_array)
			std::cout << "new[] Function Not Release, ";
		else
			std::cout << "new Function Not Release, ";

		std::cout << "Pointer Address: " << ptr << " Size: " << ptr->size;

		if (NULL != ptr->file)
			std::cout << " At " << ptr->file << " : " << ptr->line << " lines";
		else
			std::cout << " (No File Info)";

		std::cout << std::endl;

		ptr = ptr->next;
		++count;
	}

	std::cout << "Exist " << count << " Memory Leaks, Total " << memory_allocated << " bytes." << std::endl;
	return;
}

// 重载new/new[]运算符
void* operator new(size_t size, char const* file, size_t line) {
	return AllocateMemory(size, false, file, line);
}

void* operator new[](size_t size, char const* file, size_t line) {
	return AllocateMemory(size, true, file, line);
}

void* operator new(size_t size) {
	return AllocateMemory(size, false, nullptr, 0);
}

void* operator new[](size_t size) {
	return AllocateMemory(size, true, nullptr, 0);
}

// 重载delete/delete[]运算符
void operator delete(void* ptr) noexcept {
	DeleteMemory(ptr, false);
}

void operator delete[](void* ptr) noexcept {
	DeleteMemory(ptr, true);
}

#endif
