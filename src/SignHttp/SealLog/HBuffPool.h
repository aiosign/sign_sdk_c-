#pragma once
#include <vector>
#include <list>
#include "ICE/HICEMutex.h"
using namespace std;

#define HBuff_Leng0		(4*1024)//4KB
#define HBuff_Leng1		(8*1024)//8KB
#define HBuff_Leng2		(32*1024)//32KB
#define HBuff_Leng3		(64*1024)//64KB
#define HBuff_Leng4		(128*1024)//128KB
#define HBuff_Leng5		(512*1024)//512KB
#define HBuff_Leng6		(1*1024*1024)//1M
#define HBuff_Leng7		(2*1024*1024)//2M
#define HBuff_Leng8		(4*1024*1024)//4M
#define HBuff_Leng9		(8*1024*1024)//8M
#define HBuff_Leng10	(16*1024*1024)//16M
#define HBuff_Leng11	(32*1024*1024)//32M
#define HBuff_Leng12	(64*1024*1024)//64M
#define HBuff_Leng13	(128*1024*1024)//128M
#define HBuff_Leng14	(256*1024*1024)//256M
#define HBuff_Leng15	(512*1024*1024)//512M

class HBuffPool;
class HBlob
{
public:
	char* buff;
	unsigned ilenmax;//最大长度
	unsigned ilenuse;//已写长度
	//volatile long ibusy;//0,空闲，1:繁忙
	HBuffPool* __pool;
	HBlob* __next;//next blob,外部使用用以形成blob单向链表

public:
	HBlob(HBuffPool* __pool = NULL);
	HBlob(unsigned ilenmax,HBuffPool* __pool = NULL);
	~HBlob();
	//外部的blob的内容，拷贝给自己
	int BlobCopy(const HBlob* blob);
	int BuffCopy(const char* buff,unsigned ilen);//buff拷贝进去
	int BuffMove(unsigned ioffset);//偏移位置
	int BuffAppend(const char* buff,unsigned ilen);//buff追加
	void clear();
	inline bool empty(){return (ilenuse == 0);}

};

/*******************************
注：该类内部请勿写日志，日志类使用了该类，该类中使用了原子锁，所以，如果在本类中写日志可能会导致死锁
*******************************/

class HBuffPool
{
public:
	//不定长长度列表
	//0:max HBuff_Leng0,1:max HBuff_Leng1 ...15:max HBuff_Leng15
	//default 10,max length: HBuff_Leng10
	HBuffPool(unsigned imaxindex = 10);
	~HBuffPool();
	//根据所需的长度，从buffer池获取一个buffer
	HBlob* get_one_buffer(unsigned ineedlen);
	//归还一个buff到内存池
	static void free_one_buffer(HBlob* blob);
	//append buff to blob
	int append_buff(HBlob*& blob,const char* buff,int ilen);
	//reserve buff
	int reserve_buff(HBlob*& blob,unsigned ineed);
	//对于相同最大长度的buff组的释放，只用锁一次
	template<class TContainer>
	void free_some_buffers(TContainer& stl_container);
private:
	HBlob* get_one_buff_inner(unsigned index);
	int free_one_buffer_inner(HBlob* blob);//0,已归还，1未归还
	HBlob* alloc_one_buff(unsigned index);
	//凡是传入本类的HBlob都认为是由本类返回的buff，
	//bool is_my_buff(HBlob* blob);
	//清理缓存池
	void clear();
public:
	bool bisclearing;//是否正在清理

	int itypetotal;
	unsigned* plenlist;
	//unsigned ilistsize;
	vector<HBlob*>* vt_blob;
	HICEUtil::Mutex* lock_list;
	//bool bUseLock;
};