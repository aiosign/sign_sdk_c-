#include "MThread.h"
#include <time.h>

#ifdef WIN32
#include <process.h>

MThreadControl::MThreadControl()
{
	_handle = NULL;
}

MThreadControl::MThreadControl(const HandleSharePtr& handle,ThreadId id):
_handle(handle),_id(id)
{
}

void MThreadControl::detach()
{
	// No-op: Windows doesn't have the concept of detaching a thread.
}

int MThreadControl::join()
{
	HICEUtil::MutexAuto au(&lock);
	if (_handle.Get() == NULL)
		return -1;

	DWORD dw = 0;
	if (!GetExitCodeThread(_handle.Get(),&dw))
	{
		return 0;
	}

	if (dw != STILL_ACTIVE)
	{
		return 0;
	}

	int rc = WaitForSingleObject(_handle.Get(), INFINITE);
	if(rc != WAIT_OBJECT_0)
	{
		return -2;
	}

	return 0;
}

bool MThreadControl::isAlive()
{
	HICEUtil::MutexAuto au(&lock);
	if (_handle.Get() == NULL)
		return false;

	DWORD dw = 0;
	if (!GetExitCodeThread(_handle.Get(),&dw))
		return false;

	return (dw == STILL_ACTIVE);
}

MThread::MThread():
_started(false),_id(0),_handle(NULL),b_threadIsrunning(false)
{
}


ThreadId MThread::ThreadID()
{
	HICEUtil::MutexAuto au(&mu);
	if(!_started)
	{
		return 0;
	}
	return _id;
}

MThread::~MThread()
{
}

static void* startHook(void* arg)
{
	//注释掉，让线程dump进而采集dump信息
	//__try//防止join之后异常，导致主调线程的代码被跳过执行
	//{
		MThread* rawThread = static_cast<MThread*>(arg);

		{
#ifdef WIN32
			srand((unsigned int)(::GetTickCount() + ::GetCurrentThreadId()));
#else
			struct timespec ts;

			clock_gettime(CLOCK_MONOTONIC, &ts);

			__int64 itick = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
			srand((unsigned int)(itick + pthread_self());
#endif
		}
		rawThread->b_threadIsrunning = true;
		rawThread->run();
		rawThread->b_threadIsrunning = false;
	//}
	//__except(UnhandledExceptionFilter(GetExceptionInformation()))
	//{
	//	;
	//}
	return NULL;
}

MThreadControl* MThread::start(unsigned stackSize)
{
	HICEUtil::MutexAuto au(&mu);
	b_threadIsrunning = false;
	if(_started)
		return new MThreadControl(_handle,_id);

	_handle = (HANDLE)_beginthreadex(
		0, stackSize, (unsigned int (__stdcall*)(void*))startHook, (LPVOID)this, 0, &_id);
	if (_handle.Get() == NULL)
		return new MThreadControl(_handle,0);

	_started = true;
	return new MThreadControl(_handle,_id);
}

#else

MThreadControl::MThreadControl()
{
	_id = 0;
}

MThreadControl::MThreadControl(const ThreadId& id):
_id(id)
{
}

void MThreadControl::detach()
{
	{
		HICEUtil::MutexAuto au(&lock);
		if(_id == 0)
			return;
	}

	int rc = pthread_detach(_id);
	if(rc != 0)
		return ;
}

int MThreadControl::join()
{
	{
		HICEUtil::MutexAuto au(&lock);
		if(_id == 0)
			return -1;
	}

	void* ignore = 0;
	int rc = pthread_join(_id, &ignore);
	_id = 0;
	if(rc != 0)
		return -1;

	return 0;
}

bool MThreadControl::isAlive()
{
	HICEUtil::MutexAuto au(&lock);
	int policy;
	int ret;
	struct sched_param param;

	ret = pthread_getschedparam(_id, &policy, &param);
#ifdef __APPLE__
	if (ret == 0) 
	{
		ret = pthread_setschedparam(_id, policy, &param);
	}
#endif 
	return (ret == 0);
}

MThread::MThread():_started(false),_id(0),b_threadIsrunning(false)
{
}

ThreadId MThread::ThreadID()
{
	HICEUtil::MutexAuto au(&mu);
	if(!_started)
		return 0;
	return _id;
}
MThread::~MThread()
{
}
//extern "C" {
//	
//}
//注释掉，让线程dump进而采集dump信息
static void* startHook(void* arg)
{
	//try
	//{
		MThread* rawThread = static_cast<MThread*>(arg);

		//srand((unsigned int)time(0));

		rawThread->b_threadIsrunning = true;
		rawThread->run();
		rawThread->b_threadIsrunning = false;
	//}
	//catch(...)
	//{
	//}
	return NULL;
}

MThreadControl MThread::start(unsigned stackSize)
{
	HICEUtil::MutexAuto au(&mu);
	if(_started)
		return MThreadControl(_id);
	if(stackSize > 0)
	{
		pthread_attr_t attr;
		int rc = pthread_attr_init(&attr);
		if(rc != 0)
			return MThreadControl(_id);

		rc = pthread_attr_setstacksize(&attr, stackSize);
		if(rc != 0)
			return MThreadControl(_id);

		rc = pthread_create(&_id, &attr, startHook, this);
		if(rc != 0)
			return MThreadControl(_id);
	}
	else
	{
		int rc = pthread_create(&_id, 0, startHook, this);
		if(rc != 0)
			return MThreadControl(_id);
	}

	_started = true;
	return MThreadControl(_id);
}

#endif

MThreadControl* MThread::getThreadControl()
{
	{
		HICEUtil::MutexAuto au(&mu);
		if(!_started)
#ifdef WIN32
			return new MThreadControl(NULL,0);
#else
			return MThreadControl(0);
#endif
	}

#ifdef WIN32
	return new MThreadControl(_handle,_id);
#else
	return MThreadControl(_id);
#endif
}

MThreadControl* MThread::restart(unsigned stackSize)
{
	HICEUtil::MutexAuto au(&mu);
	if(_started)//如果线程还在活动状态，就不重启
	{
		if(getThreadControl()->isAlive())
			return getThreadControl();
	}

	_started = false;
	return start(stackSize);
}