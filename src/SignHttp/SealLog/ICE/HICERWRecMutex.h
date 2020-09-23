// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#pragma once
#include "HICEConfig.h"
#include "HICEMutex.h"
#include "HICECond.h"
#include "HICEThread.h"

namespace HICEUtil
{

template <typename T>
class RLockT
{
public:

    RLockT(const T& mutex) :
	_mutex(mutex)
    {
	_mutex.readlock();
	_acquired = true;
    }

    ~RLockT()
    {
	if (_acquired)
	{
	    _mutex.unlock();
	}
    }

    void acquire() const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_mutex.readlock();
	_acquired = true;
    }

    bool tryAcquire() const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_acquired = _mutex.tryreadlock();
	return _acquired;
    }

    bool timedAcquire(const Time& timeout) const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_acquired = _mutex.timedreadlock(timeout);
	return _acquired;
    }

    

    void release() const
    {
	if (!_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_mutex.unlock();
	_acquired = false;
    }

    bool acquired() const
    {
	return _acquired;
    }

    void
    upgrade() const
    {
	_mutex.upgrade();
    }

    bool
    timedUpgrade(const Time& timeout) const
    {
	return _mutex.timedUpgrade(timeout);
    }

protected:
    
    // TryRLockT's constructors

    RLockT(const T& mutex, bool) :
	_mutex(mutex)
    {
	_acquired = _mutex.tryreadlock();
    }


    RLockT(const T& mutex, const Time& timeout) :
	_mutex(mutex)
    {
	_acquired = _mutex.timedreadlock(timeout);
    }


private:

    // Not implemented; prevents accidental use.
    //
    RLockT(const RLockT&);
    RLockT& operator=(const RLockT&);

    const T& _mutex;
    mutable bool _acquired;
};

template <typename T>
class TryRLockT : public RLockT<T>
{
public:

    TryRLockT(const T& mutex) :
	RLockT<T>(mutex, true)
    {
    }

    TryRLockT(const T& mutex, const Time& timeout) :
	RLockT<T>(mutex, timeout)
    {
    }
};

template <typename T>
class WLockT
{
public:

    WLockT(const T& mutex) :
	_mutex(mutex)
    {
	_mutex.unlock();
	_acquired = true;
    }

    ~WLockT()
    {
	if (_acquired)
	{
	    _mutex.unlock();
	}
    }

    void acquire() const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_mutex.unlock();
	_acquired = true;
    }

    bool tryAcquire() const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_acquired = _mutex.trywritelock();
	return _acquired;
    }

    bool timedAcquire(const Time& timeout) const
    {
	if (_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_acquired = _mutex.timedwritelock(timeout);
	return _acquired;
    }

    void release() const
    {
	if (!_acquired)
	{
	    throw ThreadLockedException(__FILE__, __LINE__);
	}
	_mutex.unlock();
	_acquired = false;
    }

    bool acquired() const
    {
	return _acquired;
    }

protected:

    // TryWLockT's constructor

    WLockT(const T& mutex, bool) :
	_mutex(mutex)
    {
	_acquired = _mutex.trywritelock();
    }

    WLockT(const T& mutex, const Time& timeout) :
	_mutex(mutex)
    {
	_acquired = _mutex.timedwritelock(timeout);
    }

private:

    // Not implemented; prevents accidental use.
    //
    WLockT(const WLockT&);
    WLockT& operator=(const WLockT&);

    const T& _mutex;
    mutable bool _acquired;
};

template <typename T>
class TryWLockT : public WLockT<T>
{
public:

    TryWLockT(const T& mutex) :
	WLockT<T>(mutex, true)
    {
    }

    TryWLockT(const T& mutex, const Time& timeout) :
	WLockT<T>(mutex, timeout)
    {
    }
};

//
// Concurrency primitive that allows many readers & one writer access
// to a data structure. Writers have priority over readers. The
// structure is not strictly fair in that there is no absolute queue
// of waiting writers - that is managed by a condition variable.
//
// Both Reader & Writer mutexes can be recursively locked. Calling
// upgrade() or timedUpgrade() while holding a read lock promotes
// the reader to a writer lock.
//
class  RWRecMutex
{
public:

    //
    // RLock (reader) & WLock (writer) typedefs.
    //
    typedef RLockT<RWRecMutex> RLock;
    typedef TryRLockT<RWRecMutex> TryRLock;
    typedef WLockT<RWRecMutex> WLock;
    typedef TryWLockT<RWRecMutex> TryWLock;

    RWRecMutex();
    ~RWRecMutex();

    //
    // Note that readLock/writeLock & unlock in general should not be
    // used directly. Instead use RLock & WLock.
    //

    //
    // Acquire a read lock.
    //
    void readlock() const;

    //
    // Try to acquire a read lock.
    //
    bool tryreadlock() const;

    //
    // Try to acquire a read lock for upto the given timeout.
    //
    bool timedreadlock(const Time&) const;

    //
    // Acquire a write lock.
    //
    void writelock() const;

    //
    // Acquire a write lock.
    //
    bool trywritelock() const;

    //
    // Acquire a write lock for up to the given timeout.
    //
    bool timedwritelock(const Time&) const;

    //
    // Unlock the reader/writer lock.
    //
    void unlock() const;

    //
    // Upgrade the read lock to a writer lock. Note that this method
    // can only be called if the reader lock is not held recursively.
    //
    void upgrade() const;

    //
    // Upgrade the read lock to a writer lock for up to the given
    // timeout Note that this method can only be called if the reader
    // lock is not held recursively.
    //
    bool timedUpgrade(const Time&) const;

public:

    // noncopyable
    RWRecMutex(const RWRecMutex&);
    void operator=(const RWRecMutex&);

    //
    // Number of readers holding the lock. A positive number indicates
    // readers are active. A negative number means that a writer is
    // active.
    //
    mutable int _count;

    //
    // If there is an active writer this is the ID of the writer thread.
    //
    mutable ThreadId _writerId;

    //
    // Number of waiting writers.
    //
    mutable unsigned int _waitingWriters;

    //
    // Internal mutex.
    //
    Mutex _mutex;

    //
    // Two condition variables for waiting readers & writers.
    //
    mutable Cond _readers;
    mutable Cond _writers;
};

} // End namespace IceUtil




#ifdef HICERW_TEST_CODE


¶ÁÐ´Ëø²âÊÔ´úÂë£º


#include <HICEThread.h>
#include <HICERWRecMutex.h>

ÉùÃ÷±äÁ¿£º

HICEUtil::RWRecMutex g_RWMutexDevInfoMap;     


class RRRRRRRRR : public HICEUtil::Thread
{
public:


	int mi_index;

	virtual void run()
	{

		while(1)
		{
			HEnvironment::Sleep(100);

			¶ÁËø£º

			HICEUtil::RWRecMutex::RLock autoLock(g_RWMutexDevInfoMap);

			hout << L"R lock ok:" << mi_index << hend;

			HEnvironment::Sleep(10000);

		}


	}



};


class WWWWWWW : public HICEUtil::Thread
{
public:


	int mi_index;

	int mi_loop;

	virtual void run()
	{
		for(int i = 0; i < mi_loop; i++)
		{
			HEnvironment::Sleep(500);

			Ð´Ëø£º

			HICEUtil::RWRecMutex::WLock autoLock(g_RWMutexDevInfoMap);

			hout << L"W lock ok:" << mi_index << hend;

			HEnvironment::Sleep(5000);

		}


	}



};


void test_rwl()
{

	for (int i = 0; i < 5; i++)
	{
		RRRRRRRRR* pt = new RRRRRRRRR;
		pt->mi_index = i;
		pt->start().detach();
	}

	WWWWWWW* ptw = new WWWWWWW;
	ptw->mi_index = 0;
	ptw->mi_loop = 5;
	ptw->start().detach();

	WWWWWWW* ptw1 = new WWWWWWW;
	ptw1->mi_index = 1;
	ptw1->mi_loop = 10;
	ptw1->start().detach();



	while(1)
	{

		HEnvironment::Sleep(1000);
	}


}



#endif
