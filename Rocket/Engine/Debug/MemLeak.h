﻿// https://github.com/YunFei-S/Memory-leak-detector/blob/main/LeakDetector.h
// With Smart Pointer Error
#pragma once

#include <stdio.h>
#include <new>

#ifdef RK_MEM_CHECK
// Add File Info
void* operator new(size_t size, char const* file, size_t line);
void* operator new[](size_t size, char const* file, size_t line);

//#define new new(__FILE__, __LINE__)
// 预定义宏: 
// __FILE__(两个下划线): 代表当前源代码文件名的字符串文字(我们用这个宏获得存在内存泄漏文件的文件名)
// __LINE__(两个下划线): 代表当前源代码文件中的行号的整数常量(我们用这个宏获得存在内存泄漏文件内存泄漏的行号)
#define RK_NEW new( __FILE__, __LINE__ )
#define RK_DELETE delete

class LeakDetector {
public:
	// LeakDetector.cpp和被测试的.cpp都会包 LeakDetector.h头文件
	// 因此两个源文件中会创建两个静态LeakDetector类对象 exitCounter 
	// (两个静态类对象同名, 但是它们的链接属性均为内链接(只在当前源文件有效), 因此不会重定义), 
	// 如果此时两个对象析构, 会调用两次析构函数, 调用两次内存泄漏检测函数. 
	// 而我们的预期是只调用一次内存泄漏检测函数. 所以我们声明一个所有类对象共享的静态变量来实现我们的目的
	static size_t call_count_;

	LeakDetector() { ++call_count_; }
	~LeakDetector() { if (0 == --call_count_) LeakDetection(); }

private:
	void LeakDetection();
};

// 静态对象
static LeakDetector exitCounter;

// 重载版本: operator new/new[]( ), operator delete/delete[]( ) 的声明
void* operator new(size_t size) noexcept(false);
void* operator new[](size_t size) noexcept(false);
void* operator new  (std::size_t n, const std::nothrow_t& tag) noexcept;
void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept;
// 注意到, 上面我们重载的函数中, 第一个参数和第三个参数的类型是size_t
// 其中第一个参数size为 sizeof的返回值, 所以为size_t类型
// 第三个参数的含义为 行号, 是我们重载 operator new/new[]( )后自己加的参数, 此处也可以用unsigned int. 但最好用 size_t. 原因是size_t的可移植性好. 理由见上面链接
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
#else
#define RK_NEW new
#define RK_DELETE delete
#endif
