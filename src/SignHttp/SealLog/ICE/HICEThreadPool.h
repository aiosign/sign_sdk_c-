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

// 线程基类，任何从此类继承的类都可以纳入线程池的管理：

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

// 内部使用的大基线程：
class HUTIL_API HICEThreadBaseBig
{

public:

	HICEThreadBaseBig();

	// add 进来的指针：
	HICEThreadBase* ptbase;

	// 用户线程 ID，全局唯一：
	unsigned int uid;

	double d_create_time_mill_sec;	// 用户 add 的时间
	double d_start_process_time_mill_sec; // 开始真正调度的时间
	double d_end_process_time_mill_sec; // 完成用户 run 的时间

	int i_timeout_mill_sec; // 调度超时时间 毫秒

	bool b_auto_delete; // 是否自动删除 ptbase

	void print(); // 打印信息

};

class HICEThreadPool;


// 线程池内部使用的线程实体：
class HUTIL_API HICEThreadReal : public HICEUtil::Thread
{
public:

	HICEThreadReal();

	// 设置所属的线程池：
	int set_thread_pool(HICEThreadPool* _ptpool);
	// 设置要工作的用户线程：
	int set_user_thread(const HICEThreadBaseBig& _tbase_big);
	// 获取正在工作或等待的用户线程，返回 0 表示成功，否则表示没有合法的值：
	int get_user_thread(HICEThreadBaseBig& _tbase_big);

	
	virtual void run();
	void waitup();

	// 是否空闲？
	bool is_idle();
	// 是否已经死了？
	bool is_dead();
	
	// 线程 ID
	unsigned int ui_id;

	// 最大 idle 时间：
	int i_max_idle_time_sec;

private:

	friend class HICEThreadPool;

	HICEUtil::Monitor<HICEUtil::RecMutex> monitor;

	// 正在工作的用户线程：
	HICEThreadBaseBig tbase_big;
	// 所属的线程池：
	HICEThreadPool* ptpool;

	// 是否有新的用户线程等待处理（tbase_big）
	volatile bool b_user_thread_fresh;
	// 是否空闲？
	volatile bool b_idle;
	// 是否已经死了？
	volatile bool b_dead;
	// tbase_big 是否合法？即正在run 或等待run，而不是已经run完成
	volatile bool b_user_thread_valid;

	
};

class HUTIL_API HICEThreadRealBig
{
public:
	HICEThreadReal* pThreadReal;
};

// 线程池内部状态：
class HUTIL_API HICEThreadPoolStat
{
public:
	// 最大 idle 时间（秒）：
	int i_max_idle_time_sec;
	// 调用了多少次 add
	int i_total_add;
	// 一共真正创建了多少次线程：
	int i_total_create_thread;
	// 一共有多少次超时：
	int i_total_timeout;
	// 线程池最大数：
	int i_max_pool_size;
	// 线程池自我毁消后剩下的线程的最小数：
	int i_min_pool_size;
	// 线程池当前大小：
	int i_current_pool_size;
	// 当前等待队列长度：
	int i_current_user_queue;
	// 当前空闲线程数：
	int i_current_idle_threads;
	// 一共调用了多少次用户线程的 run
	int i_call_user_run_count;
	void print();
};
// 线程池管理：
class HUTIL_API HICEThreadPool
{
public:
	HICEThreadPool();	
	// 设置线程池最大数，当逐渐达到这个数时，不再增长：
	int set_max_pool_size(int i_max);
	// 设置线程池自我毁消后剩下的线程的最小数，当缩小到这个数时，不再缩小：
	int set_min_pool_size(int i_min);
	// 设置最大的线程空闲间隔，当某个实体线程空闲时间超过这个秒数的时候，该线程自动释放，直到触及池底（set_min_pool_size）：
	int set_max_idle_time(int iSeconds);
	/*
		添加一个用户线程到线程池里, ptb 是 new 出来的，管理器仅保存它的指针，用完后，管理器会自动 delete 它,
		除非 b_auto_delete 设置 为 false。

		i_timeout_mill_sec：当线程池达到最大，又没有空闲的工作线程时，add 会把 ptb push 到队列里，最先空闲下来
		的线程主动调度它，当它在队列中放置的时间超过 i_timeout_mill_sec 时，删除它，不再调度。
		当 i_timeout_mill_sec = 0 表示如果没有空闲线程，就立刻返回 -1，而不需要 push 到队列里，并且不管 b_auto_delete 都不 delete ptb。
		当 i_timeout_mill_sec < 0 表示永不超时
		当 i_timeout_mill_sec > 0 表示超时毫秒数

	*/
	int add(HICEThreadBase* ptb, int i_timeout_mill_sec = -1, bool b_auto_delete = true);
	// 得到指定的用户线程的当前状态，返回 0 表示成功：
	int get_user_thread_stat(HICEThreadBase* ptb, HICEThreadBaseBig& thread_stat);
	// 得到当前线程池的状态：
	int get_pool_stat(HICEThreadPoolStat& pool_stat);
	// 清空等待队列：
	int clear_user_queue();
	friend class HICEThreadReal;

private:

	HICEUtil::RecMutex mutex;
	// 用户线程 ID 产生器：
	unsigned int uid_user_thread_current_index;
	// 实体线程 ID 产生器：
	unsigned int uid_real_thread_current_index;
	unsigned int i_for_rand; // 不断递增
	// 当前线程池状态：
	HICEThreadPoolStat thread_pool_stat;
	// 用户线程等待队列：
	list<HICEThreadBaseBig> list_user_base;
	// 线程池：
	vector<HICEThreadRealBig> vt_pt_real;
	// 以下供 HICEThreadReal 调用：
	// 得到下一个等待队列中的线程基：
	int get_next_user_thread(HICEThreadBaseBig& _tb);
	// 得到当前的某个空闲线程指针：
	HICEThreadReal* get_idle_thread();
	// 删除线程池中的某个线程：
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