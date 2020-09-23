#pragma once
#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#else
#include<semaphore.h>
#endif

//#ifdef _WIN32
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//#include <winsock2.h>
//#include <windows.h>
//#undef WIN32_LEAN_AND_MEAN
//#endif

//使用时请注意，亲测，linux下一次notify只能唤醒一个wait
class HEvent
{
public:

	HEvent(void);
	~HEvent(void);

	int init();
	/*
	  i_timeou_mill_sec 是超时时间，i_timeou_mill_sec <= 0 表示不超时。
	  单位毫秒
    */
	int wait(int i_timeou_mill_sec = 0);

	// 通知另外一个线程，导致它的wait 返回：
	int notify();

	// 释放：
	int destroy();

public:
#ifdef WIN32
	HANDLE _hEvent;
#else
	sem_t _m_sem_t;
#endif
};
