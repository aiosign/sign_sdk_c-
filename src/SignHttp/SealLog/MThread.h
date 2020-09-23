#pragma once
#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include "HandleSharePtr.h"
#else
#include <pthread.h>
#endif

//#ifdef _WIN32
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//#include <winsock2.h>
//#include <windows.h>
//#undef WIN32_LEAN_AND_MEAN
//#endif
//
//#include "HandleSharePtr.h"

#include "ICE/HICEMutex.h"

#ifdef _WIN32
typedef unsigned int ThreadId;
#else
typedef pthread_t ThreadId;
#endif

class MThreadControl
{
public:
	MThreadControl();

#ifdef WIN32
	MThreadControl(const HandleSharePtr& handle,ThreadId id);
#else
	MThreadControl(const ThreadId& id);
#endif
	void detach();

	int join();

	bool isAlive();

private:
#ifdef _WIN32
	HandleSharePtr _handle;
#endif
	ThreadId _id;
	HICEUtil::Mutex lock;
};

class MThread
{
public:

	MThread();
	virtual ~MThread();

	ThreadId ThreadID();

	virtual void run() = 0;

	MThreadControl* start(unsigned stackSize = 0);

	MThreadControl* getThreadControl();
	//线程重启
	MThreadControl* restart(unsigned stackSize = 0);

private:
	bool _started;
#ifdef WIN32
	HandleSharePtr _handle;
#endif
	ThreadId _id;

	HICEUtil::Mutex mu;
public:
	//线程是否正在执行
	bool b_threadIsrunning;
};