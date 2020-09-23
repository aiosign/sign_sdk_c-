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

//ʹ��ʱ��ע�⣬�ײ⣬linux��һ��notifyֻ�ܻ���һ��wait
class HEvent
{
public:

	HEvent(void);
	~HEvent(void);

	int init();
	/*
	  i_timeou_mill_sec �ǳ�ʱʱ�䣬i_timeou_mill_sec <= 0 ��ʾ����ʱ��
	  ��λ����
    */
	int wait(int i_timeou_mill_sec = 0);

	// ֪ͨ����һ���̣߳���������wait ���أ�
	int notify();

	// �ͷţ�
	int destroy();

public:
#ifdef WIN32
	HANDLE _hEvent;
#else
	sem_t _m_sem_t;
#endif
};
