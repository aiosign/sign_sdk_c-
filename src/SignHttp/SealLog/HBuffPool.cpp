#include "HBuffPool.h"
#include <set>
#include "HLogFile.h"
#include "HCommonFuns.h"
#include "HException.h"

HBlob::HBlob(HBuffPool* pool):buff(NULL),ilenmax(0),ilenuse(0),__pool(pool),__next(NULL)
{
}

HBlob::HBlob(unsigned _ilen,HBuffPool* pool):ilenmax(_ilen),ilenuse(0),__pool(pool),__next(NULL)
{
	buff = new char[ilenmax];
}
HBlob::~HBlob()
{
	if(buff != NULL) delete [] buff;
}

int HBlob::BlobCopy(const HBlob* blob)
{
	return BuffCopy(blob->buff,blob->ilenuse);
}

int HBlob::BuffCopy(const char* _buff,unsigned ilen)
{
	if(_buff == NULL) return -10;
	if(ilen > this->ilenmax) return -20;
	this->ilenuse = ilen;
	memcpy(this->buff,_buff,this->ilenuse);
	return 0;
}

int HBlob::BuffMove(unsigned ioffset)
{
	if(ioffset == 0) return 0;
	if(ioffset >= ilenuse) {this->ilenuse = 0; return -10;}
	memmove(this->buff,this->buff + ioffset,this->ilenuse - ioffset);
	this->ilenuse -= ioffset;
	return 0;
}

int HBlob::BuffAppend(const char* _buff,unsigned ilen)
{
	if(_buff == NULL) return -10;
	if(ilen + this->ilenuse > this->ilenmax) return -20;
	//if(ilen == 0) return 0;
	memcpy(this->buff + this->ilenuse,_buff,ilen);
	this->ilenuse += ilen;
	return 0;
}

void HBlob::clear()
{
	ilenuse = 0;
	if(buff != NULL) buff[0] = 0;
}

//void bubling_sort(unsigned* list1,unsigned isize)
//{
//	for (unsigned x = 0 ; x < isize ; ++x)
//	{
//		for (unsigned y = x + 1; y < isize ; ++y)
//		{
//			if(list1[y] < list1[x])
//			{
//				//位运算交换这两个值
//				list1[x]^=list1[y];
//				list1[y]^=list1[x];
//				list1[x]^=list1[y];
//			}
//		}
//	}
//}

HBuffPool::HBuffPool(unsigned __imaxindex):bisclearing(false)
{
	if(__imaxindex > 15) __imaxindex = 15;
	itypetotal = __imaxindex + 1;
	//对于传进来的指针是否需要内部new一个指针
	//plenlist = new unsigned int[ilistsize];
	//memcpy(plenlist,lenlist,ilistsize*sizeof(unsigned));
	//从小到大排序
	//bubling_sort(plenlist,isize);
	//vt_blob = new vector<HBlob*>[ilistsize];
	//lock_list = new HICEUtil::Mutex[ilistsize];
	plenlist = new unsigned[__imaxindex + 1];
	switch(__imaxindex)
	{
	case 15:plenlist[15] = HBuff_Leng15;
	case 14:plenlist[14] = HBuff_Leng14;
	case 13:plenlist[13] = HBuff_Leng13;
	case 12:plenlist[12] = HBuff_Leng12;
	case 11:plenlist[11] = HBuff_Leng11;
	case 10:plenlist[10] = HBuff_Leng10;
	case 9:plenlist[9] = HBuff_Leng9;
	case 8:plenlist[8] = HBuff_Leng8;
	case 7:plenlist[7] = HBuff_Leng7;
	case 6:plenlist[6] = HBuff_Leng6;
	case 5:plenlist[5] = HBuff_Leng5;
	case 4:plenlist[4] = HBuff_Leng4;
	case 3:plenlist[3] = HBuff_Leng3;
	case 2:plenlist[2] = HBuff_Leng2;
	case 1:plenlist[1] = HBuff_Leng1;
	case 0:plenlist[0] = HBuff_Leng0;
	}

	vt_blob = new vector<HBlob*>[itypetotal];

	lock_list = new HICEUtil::Mutex[itypetotal];
	//plenlist[11] = HBuff_Leng11;
	//plenlist[12] = HBuff_Leng12;
	//plenlist[13] = HBuff_Leng13;
	//plenlist[14] = HBuff_Leng14;
	//plenlist[15] = HBuff_Leng15;
	for (unsigned i = 0 ; i < itypetotal ; ++i)
	{
		vt_blob[i].clear();
	}
}
HBuffPool::~HBuffPool()
{
	clear();
}

#if 0
#define DirectNewBlob
#endif

HBlob* HBuffPool::get_one_buffer(unsigned ineedlen)
{
#ifdef DirectNewBlob
	int _ineedlen = (ineedlen/4096) * 4096;
	if(ineedlen%4096 != 0) _ineedlen += 4096;
	if(_ineedlen > HBuff_LengMax)
	{
		printf("%s HBuffPool::get_one_buffer ineedlen:%u\n",HCommonFun::sec2strDateTime(time(0)).c_str(),_ineedlen);
		return NULL;
	}
	HBlob* blob = new HBlob(_ineedlen);
	return blob;
#endif
	if(bisclearing)
		return NULL;

	for (unsigned i = 0 ; i <= itypetotal ; ++i)
	{
		if(ineedlen <= plenlist[i])
			return get_one_buff_inner(i);
	}
	//HLogWrite2(LOG_ERROR,"CBuffPool::get_one_buffer needed length too long,imaxallow:%u ineedlen:%u...",plenlist[ilistsize - 1],ineedlen);
	return NULL;
}

HBlob* HBuffPool::get_one_buff_inner(unsigned index)
{
	if(index >= itypetotal)
		return 0;

	HBlob* blob(0);
	lock_list[index].lock();
	//rbegin has bug,will cause a blob was owned by to threads and cause deep problem
	//if(!vt_blob[index].empty()){blob = *vt_blob[index].rbegin();vt_blob[index].pop_back();}
	if(!vt_blob[index].empty()){blob = vt_blob[index][(vt_blob[index]).size() - 1];vt_blob[index].pop_back();}
	lock_list[index].unlock();

	if(blob != 0){return blob;}
	
	blob = alloc_one_buff(index);

	//HBlob* b1 = alloc_one_buff(index);
	//log_write_test(__FILE__,__LINE__,LOG_INFO,b1,"HBuffPool::alloc_one_buff,vt_blob[%u].size()=%u",index,vt_blob->size());

	return blob;
}

HBlob* HBuffPool::alloc_one_buff(unsigned index)
{
	try
	{
		HBlob* blob = new HBlob(plenlist[index],this);
		//blob->buff = new char[plenlist[index]];
		//return new HBlob(plenlist[index],this);
		return blob;
	}
	catch (bad_alloc& e)
	{
		//exit(e->what());
		HLogWrite4(LOG_ERROR,"HBuffPool::alloc_one_buff memery use up...file:%s,line:%d,lenth:%d,mess:%s\n",__FILE__,__LINE__,plenlist[index],e.what());
		return NULL;
	}
}

void HBuffPool::free_one_buffer(HBlob* blob)
{
#ifdef DirectNewBlob
	if(blob != NULL) delete blob;
	return;
#endif
	if(blob == NULL) return;
	if(blob->__next != NULL) blob->__next = NULL;
	if(blob->__pool == NULL){delete blob; return;}//野blob，直接delete
	//不加锁置为空闲状态，最多本轮无法使用该指针，但是可以稍微加快速度
	blob->clear();
	//HCommonFun::InterlockedExchangeM(&blob->ifree,1);
	if(blob->__pool->free_one_buffer_inner(blob) == 0) return;
	delete blob;
}

int HBuffPool::free_one_buffer_inner(HBlob* blob)
{
	//if(!bUseLock) {HCommonFun::InterlockedExchangeM(&blob->ibusy,0);return 0;}

	for (unsigned i = 0 ; i < itypetotal ; ++i)
	{
		if(blob->ilenmax == plenlist[i])
		{
			lock_list[i].lock();
			vt_blob[i].push_back(blob);
			lock_list[i].unlock();
			return 0;
		}
	}

	return -100;
}

int HBuffPool::append_buff(HBlob*& blob,const char* buff,int ilen)
{
	if(blob == NULL)
	{
		if((blob = this->get_one_buffer(ilen)) == NULL)
		{
			HLog(HLogError << "HBuffPool::append_buff get_one_buffer failed,needed:"<<ilen);
			return -100;
		}
	}
	//blob isn't null,but needed length is bigger than the current max length
	else if (ilen + blob->ilenuse > blob->ilenmax)
	{
		//get a new blob
		HBlob* blobtemp = this->get_one_buffer(ilen + blob->ilenuse);
		if(blobtemp == NULL)
		{
			HLog(HLogError << "HBuffPool::append_buff get_one_buffer blobtemp failed,needed:"<<ilen + blob->ilenuse);
			return -200;
		}
		//and copy the old content to the new blob
		blobtemp->BlobCopy(blob);
		//this mustn't lose
		blobtemp->__next = blob->__next;
		//free the old blob
		HBuffPool::free_one_buffer(blob);
		blob = blobtemp;
	}
	blob->BuffAppend(buff,ilen);
	return 0;
}

int HBuffPool::reserve_buff(HBlob*& blob,unsigned ineed)
{
	if(blob == NULL)
	{
		if((blob = this->get_one_buffer(ineed)) == NULL)
		{
			HLog(HLogError << "HBuffPool::reserve_buff get_one_buffer failed,needed:"<<ineed);
			return -100;
		}
		return 0;
	}

	if (blob->ilenmax >= ineed) return 0;

	//if blob->ilenmax < ineed blob isn't null,but needed length is bigger than the current max length
	//get a new blob
	HBlob* blobtemp = this->get_one_buffer(ineed);
	if(blobtemp == NULL)
	{
		HLog(HLogError << "HBuffPool::reserve_buff get_one_buffer blobtemp failed,needed:"<<ineed);
		return -200;
	}
	//and copy the old content to the new blob
	blobtemp->BlobCopy(blob);
	//this mustn't lose
	blobtemp->__next = blob->__next;
	//free the old blob
	HBuffPool::free_one_buffer(blob);
	blob = blobtemp;
	return 0;
}

template<class TContainer>
void HBuffPool::free_some_buffers(TContainer& stl_container)
{
	typename TContainer::iterator ite;
	if(bisclearing)
	{
		for (ite = stl_container.begin() ; ite != stl_container.end() ;ite++)
			delete *ite;
		stl_container.clear();
		return;
	}

	if(stl_container.empty())
		return;
	for (ite = stl_container.begin() ; ite != stl_container.end() ;ite++)
		HBuffPool::free_one_buffer(*ite);
	stl_container.clear();
}

void HBuffPool::clear()
{
	bisclearing = true;
	HBlob* blob;
	for (unsigned i = 0 ; i < itypetotal ; ++i)
	{
		lock_list[i].lock();
		for (unsigned index = 0 ; index < vt_blob[i].size() ; ++index)
		{
			blob = vt_blob[i][index];
			delete blob;
		}
		vt_blob[i].clear();
		lock_list[i].unlock();
	}
}

void testHBuffPool()
{
	//unsigned ilenlist[6];
	//ilenlist[0] = 8192;ilenlist[1] = 32*1024;ilenlist[2] = 64*1024;ilenlist[3] = 512*1024;ilenlist[4] = 2*1024*1024;ilenlist[5] = 4*1024*1024;
	HBuffPool pool;
	list<HBlob*>li;
	pool.free_some_buffers<list<HBlob*>>(li);
	vector<HBlob*>vt;
	pool.free_some_buffers<vector<HBlob*>>(vt);
	set<HBlob*>set1;
	pool.free_some_buffers<set<HBlob*>>(set1);
}