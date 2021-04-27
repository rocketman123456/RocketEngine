// https://github.com/YunFei-S/Memory-leak-detector/blob/main/LeakDetector.cpp
// With Smart Pointer Error
//#define NEW_OVERLOAD_IMPLEMENTATION_
//#define _CRT_SECURE_NO_WARNINGS
#include "Debug/MemLeak.h"

#include <iostream>
#include <cstring>

#ifdef RK_DEBUG
// ��ʼ�� LeakDetector���ж���ľ�̬����
size_t LeakDetector::call_count_ = 0;

// ����ʹ�ô�ͷ�ڵ��˫���������ֶ������ڴ��������ͷ�, ͷ�ڵ��_prevָ�����һ�����, _nextָ���һ�����
// ˫������ṹ
typedef struct MemoryList {
	struct MemoryList* prev = nullptr;
	struct MemoryList* next = nullptr;
	size_t size;				// operator new( )������ڴ��С
	bool   is_array;			// �Ƿ�Ϊ��������(��ʹ��operator new[]( ) ������ operator new( ))
	char*  file;				// �����, �洢�����ڴ�й©�ļ����ļ���Ϣ
	size_t line;				// �洢�����ڴ�й©λ�õ��к�
} MemoryList;

// ����һ��ͷ���, ����ǰ��ָ�����ʼ��Ϊָ���Լ�(���롢ɾ��˫�������н�� �� _LeakDetector( )�����б���˫������ʱ, ������ʼ�������þ����ֳ�����)��ʹ�þ�̬����ʹ��ֻ�ڱ��ļ�����Ч
// ����ֻʹ�����ͷ�ڵ�� prev �� next ��Ա
static MemoryList memory_list_head = { &memory_list_head, &memory_list_head, 0, false, nullptr, 0 };

// ����δ�ͷŵ��ڴ��С
static size_t memory_allocated = 0;

// ��˫���������ͷ�巨�����ڴ�
static void* AllocateMemory(size_t size, bool array, char* file, size_t line) {
    printf("Alllocate Memory\n");
	// ������ҪΪ���ǹ����ڴ����� MemoryList��� Ҳ�����ڴ�
	// �����µĴ�С
	size_t new_size = size + sizeof(MemoryList);

	// �ѽ��յ��ĵ�ַǿתΪ MemoryList*, �Ա����Ǻ�������
	// ����������new, ��������ʹ�� malloc �������ڴ�
	MemoryList* new_elem = (MemoryList*)malloc(new_size);

	// ����MemoryList�ṹ��Ա��ֵ
	new_elem->next = memory_list_head.next;
	new_elem->prev = &memory_list_head;
	new_elem->size = size;						// ע��, �˴�Ϊsize������newSize. ��Ϊ���ǹ����¼���� new������ڴ�, ��֤���Ƿ�δ�ͷ�, �����ڴ�й©����. ���� newSize���ڴ�(Ϊ MemoryList������������ڴ�), ֻ��Ϊ��ʵ���ֶ������ڴ�������, ����ڴ�����һ�����ͷ�, �����ע. ���Ա��� ʱ��size������newSize
	new_elem->is_array = array;

	// ������ļ���Ϣ, �򱣴�����
	if (nullptr != file) {
		new_elem->file = (char*)malloc(strlen(file) + 1);
		strcpy(new_elem->file, file);
	}
	else {
		new_elem->file = nullptr;
	}
	// �����к�
	new_elem->line = line;
	
	// ����˫������ṹ
	memory_list_head.next->prev = new_elem;
	memory_list_head.next = new_elem;

	// ����δ�ͷŵ��ڴ���
	// ���ǹ����ֻ�� new������ڴ�. Ϊmemory_list_head����������ڴ�,��Ϊ�����ļ���Ϣ�������ڴ��޹�, ��Щ�ڴ�����һ�����ͷ�, ��������ֻ��¼size
	memory_allocated += size;

	// ����new ������ڴ��ַ
	// ��new_elemǿתΪchar* ����(��ָ֤��+1ʱÿ�μӵ��ֽ���Ϊ1) + memory_list_head��ռ���ֽ���( �ܹ������newSize�ֽ��� ��ȥmemory_list_head���ռ�õ��ֽ���, ��Ϊnew������ֽ��� )
	return (char*)new_elem + sizeof(memory_list_head);
}

// ��˫���������ͷɾ���ֶ������ͷ��ڴ�
// ע��: delete/delete[]ʱ ���ǲ���֪������������˫�������е���һ�����
static void  DeleteMemory(void* ptr, bool array) {
    printf("Delete Memory\n");
	// ע��, �ѵĿռ��Ե���������. ���Դ˴�Ϊ��
	MemoryList* cur_elem = (MemoryList*)((char*)ptr - sizeof(MemoryList));

	// ��� new/new[] �� delete/delete[] ��ƥ��ʹ��. ֱ�ӷ���
	if (cur_elem->is_array != array)
		return;

	// ��������ṹ
    cur_elem->next->prev = cur_elem->prev;
    cur_elem->prev->next = cur_elem->next;

	// ����memory_allocatedֵ
	memory_allocated -= cur_elem->size;

	// ���cur_elem->_file��ΪNULL, �ͷű����ļ���Ϣʱ������ڴ�
	if (NULL != cur_elem->file)
		free(cur_elem->file);

	// �ͷ��ڴ�
	free(cur_elem);
}

// ���Ƕ�������һ����̬��������ʱ���ô˺���, �ж��Ƿ����ڴ�й©, ����, ���ӡ���ڴ�й©��Ϣ
void LeakDetector::LeakDetection() {
	if (0 == memory_allocated) {
		std::cout << "���벻�����ڴ�й©!" << std::endl;
		return;
	}

	// �����ڴ�й©
	// ��¼�ڴ�й©����
	size_t count = 0;

	// ���������ڴ�й©, ��˫��������Ӧ��ֻʣ��һ��ͷ�ڵ�
	// �������ڴ�й©, ��˫�������г�ͷ�ڵ�֮��Ľ�㶼��й¶���������ڴ�й©����
	MemoryList* ptr = memory_list_head.next;
	while ((NULL != ptr) && (&memory_list_head != ptr)) {
		if (true == ptr->is_array)
			std::cout << "new[] �ռ�δ�ͷ�, ";
		else
			std::cout << "new �ռ�δ�ͷ�, ";

		std::cout << "ָ��: " << ptr << " ��С: " << ptr->size;

		if (NULL != ptr->file)
			std::cout << " λ�� " << ptr->file << " �� " << ptr->line << " ��";
		else
			std::cout << " (���ļ���Ϣ)";

		std::cout << std::endl;

		ptr = ptr->next;
		++count;
	}

	std::cout << "����" << count << "���ڴ�й¶, ������ " << memory_allocated << " byte." << std::endl;
	return;
}

// ����new/new[]�����
void* operator new(size_t size, char* file, size_t line) {
	return AllocateMemory(size, false, file, line);
}

void* operator new[](size_t size, char* file, size_t line) {
	return AllocateMemory(size, true, file, line);
}

void* operator new(size_t size) {
	return AllocateMemory(size, false, nullptr, 0);
}

void* operator new[](size_t size) {
	return AllocateMemory(size, true, nullptr, 0);
}

// ����delete/delete[]�����
void operator delete(void* ptr) {
	DeleteMemory(ptr, false);
}

void operator delete[](void* ptr) {
	DeleteMemory(ptr, true);
}

#endif
