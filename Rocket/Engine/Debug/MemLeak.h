// https://github.com/YunFei-S/Memory-leak-detector/blob/main/LeakDetector.h
// With Smart Pointer Error
#pragma once

#ifdef RK_DEBUG
#include <cstdio>
#include <new>

// ���ذ汾: operator new/new[]( ), operator delete/delete[]( ) ������
void* operator new(size_t size);
void* operator new[](size_t size);
void* operator new(size_t size, char* file, size_t line);
void* operator new[](size_t size, char* file, size_t line);
// ע�⵽, �����������صĺ�����, ��һ�������͵�����������������size_t
// ���е�һ������sizeΪ sizeof�ķ���ֵ, ����Ϊsize_t����
// �����������ĺ���Ϊ �к�, ���������� operator new/new[]( )���Լ��ӵĲ���, �˴�Ҳ������unsigned int. ������� size_t. ԭ����size_t�Ŀ���ֲ�Ժ�. ���ɼ���������
void operator delete(void* ptr);
void operator delete[](void* ptr);

#ifndef __NEW_OVERLOAD_IMPLEMENTATION__
#define RK_NEW new( __FILE__, __LINE__ )
//#define new new(__FILE__, __LINE__)
// Ԥ�����: 
// __FILE__(�����»���): ����ǰԴ�����ļ������ַ�������(������������ô����ڴ�й©�ļ����ļ���)
// __LINE__(�����»���): ����ǰԴ�����ļ��е��кŵ���������(������������ô����ڴ�й©�ļ��ڴ�й©���к�)
#endif

class LeakDetector {
public:
	// LeakDetector.cpp�ͱ����Ե�.cpp����� LeakDetector.hͷ�ļ�
	// �������Դ�ļ��лᴴ��������̬LeakDetector����� exitCounter (������̬�����ͬ��, �������ǵ��������Ծ�Ϊ������(ֻ�ڵ�ǰԴ�ļ���Ч), ��˲����ض���), �����ʱ������������, �����������������, ���������ڴ�й©��⺯��. �����ǵ�Ԥ����ֻ����һ���ڴ�й©��⺯��. ������������һ�������������ľ�̬������ʵ�����ǵ�Ŀ��
	static size_t call_count_;

	LeakDetector() { ++call_count_; }
	~LeakDetector() { if (0 == --call_count_) LeakDetection(); }

private:
	void LeakDetection();
};

// ��̬����
static LeakDetector exitCounter;
#else
#define RK_NEW new
#endif
