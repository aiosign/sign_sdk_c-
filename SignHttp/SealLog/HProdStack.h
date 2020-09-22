#pragma once
/************************************************************************
生产者，消费者栈队列,
要求基类本身自带__next指针成员,且需要实现clear函数
消费时从队列取出,消费完还回队列
目前只支持一次消费一个和一次释放一个
************************************************************************/
#include "ice/HICEMutex.h"


template<class T>
class HProdStack
{
public:
	HProdStack():__head(0){}
	void push_one(T* p)
	{
		if(p == 0) return;

		p->clear();//clear resource

		mu.lock();
		p->__next = __head;
		__head = p;
		mu.unlock();
	}
	//if empty,will return null
	T* pop_one()
	{
		T* out__;

		mu.lock();
		if((out__ = __head) != 0) __head = __head->__next;
		mu.unlock();
		return out__;
	}

public:
	~HProdStack()
	{
		T* temp;
		while (__head != 0)
		{
			temp = __head->__next;
			delete __head;
			__head = temp;
		}
	}
private:
	T* __head;
	HICEUtil::Mutex mu;
};