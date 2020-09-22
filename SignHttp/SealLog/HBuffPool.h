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
	unsigned ilenmax;//��󳤶�
	unsigned ilenuse;//��д����
	//volatile long ibusy;//0,���У�1:��æ
	HBuffPool* __pool;
	HBlob* __next;//next blob,�ⲿʹ�������γ�blob��������

public:
	HBlob(HBuffPool* __pool = NULL);
	HBlob(unsigned ilenmax,HBuffPool* __pool = NULL);
	~HBlob();
	//�ⲿ��blob�����ݣ��������Լ�
	int BlobCopy(const HBlob* blob);
	int BuffCopy(const char* buff,unsigned ilen);//buff������ȥ
	int BuffMove(unsigned ioffset);//ƫ��λ��
	int BuffAppend(const char* buff,unsigned ilen);//buff׷��
	void clear();
	inline bool empty(){return (ilenuse == 0);}

};

/*******************************
ע�������ڲ�����д��־����־��ʹ���˸��࣬������ʹ����ԭ���������ԣ�����ڱ�����д��־���ܻᵼ������
*******************************/

class HBuffPool
{
public:
	//�����������б�
	//0:max HBuff_Leng0,1:max HBuff_Leng1 ...15:max HBuff_Leng15
	//default 10,max length: HBuff_Leng10
	HBuffPool(unsigned imaxindex = 10);
	~HBuffPool();
	//��������ĳ��ȣ���buffer�ػ�ȡһ��buffer
	HBlob* get_one_buffer(unsigned ineedlen);
	//�黹һ��buff���ڴ��
	static void free_one_buffer(HBlob* blob);
	//append buff to blob
	int append_buff(HBlob*& blob,const char* buff,int ilen);
	//reserve buff
	int reserve_buff(HBlob*& blob,unsigned ineed);
	//������ͬ��󳤶ȵ�buff����ͷţ�ֻ����һ��
	template<class TContainer>
	void free_some_buffers(TContainer& stl_container);
private:
	HBlob* get_one_buff_inner(unsigned index);
	int free_one_buffer_inner(HBlob* blob);//0,�ѹ黹��1δ�黹
	HBlob* alloc_one_buff(unsigned index);
	//���Ǵ��뱾���HBlob����Ϊ���ɱ��෵�ص�buff��
	//bool is_my_buff(HBlob* blob);
	//�������
	void clear();
public:
	bool bisclearing;//�Ƿ���������

	int itypetotal;
	unsigned* plenlist;
	//unsigned ilistsize;
	vector<HBlob*>* vt_blob;
	HICEUtil::Mutex* lock_list;
	//bool bUseLock;
};