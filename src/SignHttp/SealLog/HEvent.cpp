#include "HEvent.h"

#ifdef WIN32
HEvent::HEvent(void)
{
	_hEvent = NULL;
}

HEvent::~HEvent(void)
{
}


int HEvent::init()
{
	_hEvent = CreateEventA(0, TRUE, FALSE, 0);

	if (_hEvent == 0)
		return -1;

	return 0;
}

int HEvent::wait(int i_timeou_mill_sec)
{
	DWORD dwRet;

	if (i_timeou_mill_sec <= 0)
	{
		dwRet = WaitForSingleObject(_hEvent, INFINITE);
	}
	else
	{
		dwRet = WaitForSingleObject(_hEvent, i_timeou_mill_sec);
		if (dwRet == WAIT_TIMEOUT)
		{
			return -2; // timeout
		}
	}

	ResetEvent(_hEvent);
	return 0;
}

int HEvent::notify()
{
	if (_hEvent == 0)return -1;

	SetEvent(_hEvent);
	return 0;
}

int HEvent::destroy()
{
	if (_hEvent)
	{
		CloseHandle(_hEvent);
		_hEvent = 0;
	}

	return 0;
}
#else
#include<time.h>
HEvent::HEvent(void)
{
}

HEvent::~HEvent(void)
{
}

int HEvent::init()
{
	return sem_init(&_m_sem_t,0,0);
}

int HEvent::wait(int i_timeou_mill_sec)
{
	if(i_timeou_mill_sec<=0)
	{
		return sem_wait(&_m_sem_t);
	}
	else
	{
		timespec ts;
		ts.tv_sec=time(0);
		ts.tv_nsec=0;
		ts.tv_sec += i_timeou_mill_sec/1000;

		return sem_timedwait(&_m_sem_t,&ts);
	}
}

int HEvent::notify()
{
	sem_post(&_m_sem_t);

	return 0;
}

int HEvent::destroy()
{
	sem_destroy(&_m_sem_t);
	return 0;
}
#endif