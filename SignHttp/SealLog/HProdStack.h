#pragma once
/************************************************************************
�����ߣ�������ջ����,
Ҫ����౾���Դ�__nextָ���Ա,����Ҫʵ��clear����
����ʱ�Ӷ���ȡ��,�����껹�ض���
Ŀǰֻ֧��һ������һ����һ���ͷ�һ��
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