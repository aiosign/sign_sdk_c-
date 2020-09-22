#pragma  once

// 最基本的包含
#include <string>
#include <vector>
using namespace std;

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif


// 允许动态库连接和代码连接两种形式：

// 如果没有指定代码连接：

#if !defined(HUTIL_SOURCE_LINK) && defined(WIN32)


#ifdef HUTIL_EXPORTS
#define HUTIL_API __declspec(dllexport)
#else
#define HUTIL_API __declspec(dllimport)
#endif

// 必须多线程动态连接
#ifndef HUTIL_EXPORTS
#if !defined(_DLL) || !defined(_MT) || defined(HUTIL_STATIC_LINK)
#undef HUTIL_API
#define HUTIL_API
#   endif


#   endif




#else




#ifdef HUTIL_EXPORTS
#define HUTIL_API __declspec(dllexport)
#else


// 直接代码级连接：
#undef HUTIL_API
#define HUTIL_API


#endif



#endif

// 参数意义，输入还是输出：

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef WIN32
#define HANDLE void*
typedef void* HWND;
#define MAX_PATH 256
#define SOCKET int
#define INVALID_SOCKET (-1)
#define closesocket close
#undef HUTIL_API
#define HUTIL_API
typedef long long __int64;
typedef unsigned int DWORD;
#define SOCKET_ERROR            (-1)
#endif


#ifdef WIN32
#define BSG_SLASH_W		L"\\"
#define BSG_SLASH_C		"\\"
#else
#define BSG_SLASH_W		L"/"
#define BSG_SLASH_C		"/"
#endif



