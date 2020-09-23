#pragma  once
#include "HICEThread.h"
#include "HICERecMutex.h"
#include "HICEMonitor.h"
//#include "HICECond.h"
#include <string>
#include <vector>
#include <list>
#include "HICEComonInclude.h"

using namespace std;

//#include "HUtilComonInclude.h"

// �̻߳��࣬�κδӴ���̳е��඼���������̳߳صĹ���

#define HTBRS_NOT_RUN		0
#define HTBRS_RUNNING		1
#define HTBRS_RUN_END		2

namespace HICEUtil
{
class HUTIL_API HICEThreadBase
{
public:

	HICEThreadBase();

	virtual ~HICEThreadBase(){}
	virtual void run() = 0;
	volatile int m_i_run_status; // HTBRS_NOT_RUN...

};

// �ڲ�ʹ�õĴ���̣߳�
class HUTIL_API HICEThreadBaseBig
{

public:

	HICEThreadBaseBig();

	// add ������ָ�룺
	HICEThreadBase* ptbase;

	// �û��߳� ID��ȫ��Ψһ��
	unsigned int uid;

	double d_create_time_mill_sec;	// �û� add ��ʱ��
	double d_start_process_time_mill_sec; // ��ʼ�������ȵ�ʱ��
	double d_end_process_time_mill_sec; // ����û� run ��ʱ��

	int i_timeout_mill_sec; // ���ȳ�ʱʱ�� ����

	bool b_auto_delete; // �Ƿ��Զ�ɾ�� ptbase

	void print(); // ��ӡ��Ϣ

};

class HICEThreadPool;


// �̳߳��ڲ�ʹ�õ��߳�ʵ�壺
class HUTIL_API HICEThreadReal : public HICEUtil::Thread
{
public:

	HICEThreadReal();

	// �����������̳߳أ�
	int set_thread_pool(HICEThreadPool* _ptpool);
	// ����Ҫ�������û��̣߳�
	int set_user_thread(const HICEThreadBaseBig& _tbase_big);
	// ��ȡ���ڹ�����ȴ����û��̣߳����� 0 ��ʾ�ɹ��������ʾû�кϷ���ֵ��
	int get_user_thread(HICEThreadBaseBig& _tbase_big);

	
	virtual void run();
	void waitup();

	// �Ƿ���У�
	bool is_idle();
	// �Ƿ��Ѿ����ˣ�
	bool is_dead();
	
	// �߳� ID
	unsigned int ui_id;

	// ��� idle ʱ�䣺
	int i_max_idle_time_sec;

private:

	friend class HICEThreadPool;

	HICEUtil::Monitor<HICEUtil::RecMutex> monitor;

	// ���ڹ������û��̣߳�
	HICEThreadBaseBig tbase_big;
	// �������̳߳أ�
	HICEThreadPool* ptpool;

	// �Ƿ����µ��û��̵߳ȴ�����tbase_big��
	volatile bool b_user_thread_fresh;
	// �Ƿ���У�
	volatile bool b_idle;
	// �Ƿ��Ѿ����ˣ�
	volatile bool b_dead;
	// tbase_big �Ƿ�Ϸ���������run ��ȴ�run���������Ѿ�run���
	volatile bool b_user_thread_valid;

	
};

class HUTIL_API HICEThreadRealBig
{
public:
	HICEThreadReal* pThreadReal;
};

// �̳߳��ڲ�״̬��
class HUTIL_API HICEThreadPoolStat
{
public:
	// ��� idle ʱ�䣨�룩��
	int i_max_idle_time_sec;
	// �����˶��ٴ� add
	int i_total_add;
	// һ�����������˶��ٴ��̣߳�
	int i_total_create_thread;
	// һ���ж��ٴγ�ʱ��
	int i_total_timeout;
	// �̳߳��������
	int i_max_pool_size;
	// �̳߳����һ�����ʣ�µ��̵߳���С����
	int i_min_pool_size;
	// �̳߳ص�ǰ��С��
	int i_current_pool_size;
	// ��ǰ�ȴ����г��ȣ�
	int i_current_user_queue;
	// ��ǰ�����߳�����
	int i_current_idle_threads;
	// һ�������˶��ٴ��û��̵߳� run
	int i_call_user_run_count;
	void print();
};
// �̳߳ع���
class HUTIL_API HICEThreadPool
{
public:
	HICEThreadPool();	
	// �����̳߳�����������𽥴ﵽ�����ʱ������������
	int set_max_pool_size(int i_max);
	// �����̳߳����һ�����ʣ�µ��̵߳���С��������С�������ʱ��������С��
	int set_min_pool_size(int i_min);
	// ���������߳̿��м������ĳ��ʵ���߳̿���ʱ�䳬�����������ʱ�򣬸��߳��Զ��ͷţ�ֱ�������صף�set_min_pool_size����
	int set_max_idle_time(int iSeconds);
	/*
		���һ���û��̵߳��̳߳���, ptb �� new �����ģ�����������������ָ�룬����󣬹��������Զ� delete ��,
		���� b_auto_delete ���� Ϊ false��

		i_timeout_mill_sec�����̳߳شﵽ�����û�п��еĹ����߳�ʱ��add ��� ptb push ����������ȿ�������
		���߳������������������ڶ����з��õ�ʱ�䳬�� i_timeout_mill_sec ʱ��ɾ���������ٵ��ȡ�
		�� i_timeout_mill_sec = 0 ��ʾ���û�п����̣߳������̷��� -1��������Ҫ push ����������Ҳ��� b_auto_delete ���� delete ptb��
		�� i_timeout_mill_sec < 0 ��ʾ������ʱ
		�� i_timeout_mill_sec > 0 ��ʾ��ʱ������

	*/
	int add(HICEThreadBase* ptb, int i_timeout_mill_sec = -1, bool b_auto_delete = true);
	// �õ�ָ�����û��̵߳ĵ�ǰ״̬������ 0 ��ʾ�ɹ���
	int get_user_thread_stat(HICEThreadBase* ptb, HICEThreadBaseBig& thread_stat);
	// �õ���ǰ�̳߳ص�״̬��
	int get_pool_stat(HICEThreadPoolStat& pool_stat);
	// ��յȴ����У�
	int clear_user_queue();
	friend class HICEThreadReal;

private:

	HICEUtil::RecMutex mutex;
	// �û��߳� ID ��������
	unsigned int uid_user_thread_current_index;
	// ʵ���߳� ID ��������
	unsigned int uid_real_thread_current_index;
	unsigned int i_for_rand; // ���ϵ���
	// ��ǰ�̳߳�״̬��
	HICEThreadPoolStat thread_pool_stat;
	// �û��̵߳ȴ����У�
	list<HICEThreadBaseBig> list_user_base;
	// �̳߳أ�
	vector<HICEThreadRealBig> vt_pt_real;
	// ���¹� HICEThreadReal ���ã�
	// �õ���һ���ȴ������е��̻߳���
	int get_next_user_thread(HICEThreadBaseBig& _tb);
	// �õ���ǰ��ĳ�������߳�ָ�룺
	HICEThreadReal* get_idle_thread();
	// ɾ���̳߳��е�ĳ���̣߳�
	int del_thread_real(unsigned int ui_id, bool b_foce);
};
}


////////////////////////////// Example ///////////////////////////////////////
/*

	class MyThread : public HICEThreadBase
	{
	public:

	int id;
	int loop;

		virtual void run()
		{
			for(int i = 0; i < loop; i++)
			{
			printf("MyThread(id:%d, loop:%d, i:%d)\n", id, loop, i);
			::Sleep(1000);
			}
		}

		int set(int _id, int _loop)
		{
			id  = _id;
			loop = _loop;

			return 0;
		}
	};


	HICEThreadPool tp;
	tp.set_max_pool_size(200);
	tp.set_min_pool_size(2);
	tp.set_max_idle_time(20);

	MyThread* p;

	p = new MyThread();
	p->set(id++, 5);
	tp.add(p);

	p = new MyThread();
	p->set(id++, 5);
	tp.add(p);


	p = new MyThread();
	p->set(id++, 5);
	tp.add(p);


*/
////////////////////////////// Example ///////////////////////////////////////