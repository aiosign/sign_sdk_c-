#include "HLogFile.h"
#include "HCommonFuns.h"
#include "HEvent.h"
#include "ice/HICERecMutex.h"
//#include "frame_memcheck.h"
#ifdef WIN32
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
#endif

#ifndef WIN32
#include <unistd.h>
#endif
//#include "ice/HICEThread.h"
#include "MThread.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

char g_szProcessName[256];
int g_iLogDirType;

struct stublock
{
	volatile long istatus;//0,free,1,write完毕,2,正在写
	unsigned ilenuse;//已写长度
	char* buff;//
};

WLog::WLog(stublock* p):block(p)
{
}

void WLog::Appenlog(const char* plog,int ilen)
{
	//if not enough,trunked the log,if write full,return directly...
	if((block->ilenuse + ilen> __Max_Blob_Length_HLogFile_) && (ilen = __Max_Blob_Length_HLogFile_ - block->ilenuse) == 0) return;
	memcpy(block->buff + block->ilenuse,plog,ilen);
	block->ilenuse += ilen;
}

//stublock* stublock::operator << (const char plog[])
//{
//	int ilen = strlen(plog);
//	this->Appenlog(plog,ilen);
//	return this;
//}

WLog& WLog::operator << (const char* plog)
{
	if(plog == NULL) return *this;
	if(block == NULL) return * this;
	int ilen = strlen(plog);
	this->Appenlog(plog,ilen);
	return *this;
}
WLog& WLog::operator << (const string& strlog)
{
	if(block == NULL) return * this;
	this->Appenlog(strlog.c_str(),strlog.length());
	return *this;
}
WLog& WLog::operator << (int ilog)
{
	if(block == NULL) return * this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%d",ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (unsigned int ilog)
{
	if(block == NULL) return * this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%u",ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (long ilog)
{
	if(block == NULL) return * this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%d",ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (unsigned long ilog)
{
	if(block == NULL) return * this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%u",ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (short ilog)
{
	if(block == NULL) return *this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%d",(int)ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (unsigned short ilog)
{
	if(block == NULL) return *this;
	char plog[16];
	int ilen = sprintf_s(plog,16,"%d",(int)ilog);
	this->Appenlog(plog,ilen);
	return *this;
}

WLog& WLog::operator << (const long long& i64log)
{
	if(block == NULL) return *this;
	char plog[24];
	int ilen = sprintf_s(plog,24,"%lld",i64log);
	this->Appenlog(plog,ilen);
	return *this;
}
WLog& WLog::operator << (const unsigned long long& i64log)
{
	if(block == NULL) return * this;
	char plog[24];
	int ilen = sprintf_s(plog,24,"%llu",i64log);
	this->Appenlog(plog,ilen);
	return *this;
}
WLog& WLog::operator << (const double& doulog)
{
	if(block == NULL) return * this;
	char plog[24];
	int ilen = sprintf_s(plog,24,"%f",doulog);
	this->Appenlog(plog,ilen);
	return *this;
}

// lock a pid file, 保证进程启动的唯一性
// file_name, 一般采用进程名，建议放在/var/lock目录下，所以一般格式化为"/var/lock/proc_name.lock"
bool lock_file( const char* file_name)
{
#ifdef WIN32
	HANDLE handle = CreateFileA(file_name,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(handle == INVALID_HANDLE_VALUE) return false;
	return true;
#else
	int fd = open( file_name, O_CREAT | O_WRONLY );
	if ( fd > 0 ) {
		if ( flock( fd, LOCK_EX | LOCK_NB ) < 0 ) {
			close( fd );
			return false;
		}

		char buf[32];

		sprintf_s( buf,32, "%d", getpid() );
		write( fd, buf, strlen( buf ) );

		//		close( fd );

		return true;
	}
	else
		return false;
#endif
}

/********************************************************
* Function: get current date time.
* param:   plSecd means the second from 1970-1-1 00:00:00
********************************************************/
int get_current_time1(int* pYear, int* pMonth, int* pDay,
			int *pHour, int* pMin, int* pSec)
{
    time_t l_secd;
    tm tm_pointer;
    time(&l_secd);
    if(localtime_s(&tm_pointer,&l_secd) != 0) return -1;

    if(pYear)  *pYear = tm_pointer.tm_year+1900;
    if(pMonth) *pMonth = tm_pointer.tm_mon+1;
    if(pDay)   *pDay = tm_pointer.tm_mday;
    if(pHour)  *pHour = tm_pointer.tm_hour;
    if(pMin)   *pMin = tm_pointer.tm_min;
    if(pSec)   *pSec = tm_pointer.tm_sec;
    return 0;
}

class HLogFile:public MThread
{
public:
	HLogFile(void);
	//virtual ~HLogFile(void);
	
	HEvent ev;
	int m_iOutputLevel;
	HICEUtil::Mutex m_lock;
	bool bdirectlog;//是否直接写日志
	bool m_bDisplayFile;
	bool m_bDisplayScreen;
	FILE* m_pfile;
	string m_strLogName;
	string m_strDir;
	string m_strlogFilenow;
	string m_strlogFileold;
	time_t m_lsecdLast;
	time_t m_lSecdCurr;
	long m_lMaxFileSizeByte;
	//volatile long m_ilognumnow;//当前日志数
	//unsigned m_iminflushnum;//最少要多少条日志，才会写日志
	long lPos;

	unsigned m_ilenmax;//2个buff的最大长度
	//unsigned m_ilenused_write;//外部已写的日志长度
	char* m_bufflog;//外部进来时要写的buff
	//unsigned m_ilenused_flush;//将要flush的日志长度
	//char* m_bufflog_flush;
	stublock* m_blocks;
	unsigned m_iblocknum;
	long mi_read_index;
	volatile long mi_write_index;

	void write_logs_inner();
	//将2个buff交换
	bool swap_log_buff();
	void write_log_direct(stublock* block);

	virtual void run();
};
static HLogFile* g_Log = NULL;
HLogFile::HLogFile():m_iOutputLevel(LOG_INFO),bdirectlog(true),m_bDisplayFile(false),m_bDisplayScreen(false),
m_pfile(NULL),m_lsecdLast(0),m_lMaxFileSizeByte(50*1024*1024),lPos(0),
m_lSecdCurr(0),m_ilenmax(16*1024*1024),m_bufflog(NULL),m_blocks(NULL),m_iblocknum(0),mi_read_index(0),mi_write_index(0)
{
	ev.init();
	//m_lock.unlock();
}

void HLogFile::write_logs_inner()
{
	if(m_bDisplayFile)
	{
		bool bfinish(false);
		for (; mi_read_index < m_iblocknum; ++mi_read_index)
		{
			if(HCommonFun::InterlockedComparedExchangeM(&m_blocks[mi_read_index].istatus,0,1) != 1){bfinish = true;break;}
			write_log_direct(&m_blocks[mi_read_index]);
		}

		if(!bfinish)
		{
			for (mi_read_index = 0; mi_read_index < m_iblocknum; ++mi_read_index)
			{
				if(HCommonFun::InterlockedComparedExchangeM(&m_blocks[mi_read_index].istatus,0,1) != 1){break;}
				write_log_direct(&m_blocks[mi_read_index]);
			}
		}

		if(m_pfile != NULL) fflush(m_pfile);
	}
}

void HLogFile::write_log_direct(stublock* block)
{
	if(block == 0) return;
	if(block->ilenuse == 0){block->istatus = 0;return;}

	if(m_bDisplayFile)
	{
		if(bdirectlog) m_lock.lock();
		do 
		{
			m_lSecdCurr = time(0);
			if((m_lSecdCurr - m_lsecdLast > 15) && (m_pfile != NULL)) if(m_pfile != NULL){fclose(m_pfile);m_pfile = NULL;}

			if(lPos > m_lMaxFileSizeByte)
			{
				HCommonFun::DeleteFileH(m_strlogFileold);
				if(m_pfile != NULL){fclose(m_pfile); m_pfile = NULL;}
				HCommonFun::renamefile(m_strlogFilenow.c_str(),m_strlogFileold.c_str());
			}

			if(m_pfile == NULL)
			{
				lPos = 0;
				m_pfile = fopen(m_strlogFilenow.c_str(), "ab+");
				if(m_pfile == NULL)
				{
					printf("open log file failed,filename:%s",m_strlogFilenow.c_str());
					break;
				}

				fseek(m_pfile,SEEK_SET, SEEK_END);//将文件指针移动文件结尾
				//fgetpos(m_pfile,&lPos);
				lPos = ftell(m_pfile);
			}
			//写到文件
			fwrite(block->buff,block->ilenuse,1,m_pfile);
			lPos += block->ilenuse;
			m_lsecdLast = time(0);
			if(bdirectlog && m_pfile != NULL) fflush(m_pfile);
		} while (false);
		if(bdirectlog){block->istatus = 0; m_lock.unlock();}
		
	}

	if(m_bDisplayScreen)
	{
		block->buff[block->ilenuse] = 0;
		//printf(block->buff);
		cout<<block->buff;
	}
	//重置
	block->ilenuse = 0;
}

void HLogFile::run()
{
	stublock *block(NULL);
	int iret;
	while(true)
	{
		iret = ev.wait(10000);
		//if(iret == -2) continue;
		write_logs_inner();
	}

	printf("Thread HLogFile::run end...\n");
	return;
}

void log_write(const char* filename, const int linenum, int iErrorCode, const char *format, ... )
{
	if(g_Log == NULL)
	{
		perror("You should call log_init() first\n");
		log_init_real();
		if(g_Log == NULL)
		{
			perror("log_write call log_init_real() failed...\n");
			return;
		}
	}

	if(filename == NULL)
	{
		perror("You should input filename param first...\n");
		return;
	}

	if(iErrorCode < g_Log->m_iOutputLevel) return;

	if((!g_Log->m_bDisplayFile) && (!g_Log->m_bDisplayScreen)) return ;

    //int iYear, iMonth, iDay, iHour, iMin, iSec;
    //int i(0);
	stublock* block = &g_Log->m_blocks[(((unsigned long)HCommonFun::InterlockedIncrementM(&g_Log->mi_write_index))%g_Log->m_iblocknum)];
	HCommonFun::InterlockedExchangeM(&block->istatus,2);
	//t = 0;
	time_t t_now;
	time(&t_now);
	block->ilenuse = strftime(block->buff,2048,"<%Y-%m-%d %H:%M:%S,",localtime(&t_now));
	int t = (int)strlen(filename);
	for (int i = t - 1; i >= 0 ;--i)
	{
		if(filename[i] == '/' || filename[i] == '\\')
		{
			filename = filename + i + 1;
			t -= (i + 1);
			break;
		}
	}
	
	if(t > 16) t = 16;
	else
	{
		memset(block->buff + block->ilenuse,' ',16 - t);
		block->ilenuse += (16 - t);
	}

	memcpy(block->buff + block->ilenuse,filename,t);
	block->ilenuse += t;
	block->ilenuse += sprintf_s(block->buff + block->ilenuse,500,",%.4d>",linenum);

	static const string strDebug = "Debug: ";
	static const string strInfo = "Info: ";
	static const string strWarn = "Warning: ";
	static const string strError = "Error: ";
	switch(iErrorCode)
	{
	case LOG_DEBUG:
		memcpy(block->buff + block->ilenuse,strDebug.c_str(),strDebug.length());
		block->ilenuse += strDebug.length();
		break;
	case LOG_INFO:
		memcpy(block->buff + block->ilenuse,strInfo.c_str(),strInfo.length());
		block->ilenuse += strInfo.length();
		break;
	case LOG_WARN:
		memcpy(block->buff + block->ilenuse,strWarn.c_str(),strWarn.length());
		block->ilenuse += strWarn.length();
		break;
	case LOG_ERROR:
		memcpy(block->buff + block->ilenuse,strError.c_str(),strError.length());
		block->ilenuse += strError.length();
		break;
	default:
		memcpy(block->buff + block->ilenuse,strError.c_str(),strError.length());
		block->ilenuse += strError.length();
		break;
	}

    if(format)
    {
		va_list ap;
		va_start(ap, format);
		if((t = vsnprintf(block->buff + block->ilenuse,1920 - block->ilenuse,format,ap)) < 0)
			block->ilenuse = 1920;
		else
			block->ilenuse += t;

		va_end(ap);
    }

	block->buff[block->ilenuse++] = '\n';
	//g_Log->m_bufflog_write[g_Log->m_ilenused_write++] = 0;
	if(g_Log->bdirectlog)//如果是直接写日志，就直接push到日志或者screen
	{
		g_Log->write_log_direct(block);
		return;
	}
	else
	{
		HCommonFun::InterlockedExchangeM(&block->istatus,1);
		g_Log->ev.notify();
	}
    return;
}

void log_init_real_inner(const char* psDir, const char* psLogFname,bool bdirectlog,unsigned iminflushnum, int iOutputLevel, int iDisplayWhere);
void log_init_real(const char* filedir,const char* filename,bool bdirectlog,unsigned iminflushnum, int iOutputLevel, int iDisplayWhere)
{
	if(g_Log != NULL) return;
	//int i = 0;
	if(iminflushnum > 100) iminflushnum = 100;//最大每次缓存100条日志
	//char szDir[G_MAXPATHLEN];
	string strdir,strshort,strfilename;
	if (filedir == NULL)
	{
		strdir = HCommonFun::GetMyExeFullPath();
		HCommonFun::Spilt_to_Path_and_Short(strfilename,strdir,strshort);
	}
	else
	{
		strdir = filedir;
	}
	strdir.append("log\\");
	if(filename != NULL) strshort = filename;//没有传递日志文件名称,内部自动生成日志路径,放在exe的当前路径的log目录下
	log_init_real_inner(strdir.c_str(), strshort.c_str(),bdirectlog,iminflushnum, iOutputLevel, iDisplayWhere);
}

const char* log_get_log_path()
{
	const static string strdefault = "g_log is null ,should call log_init_real first...";
	if(g_Log != NULL) return g_Log->m_strlogFilenow.c_str();
	return strdefault.c_str();
}

void log_write_force()
{
	if(g_Log == NULL) return;
	if(g_Log->bdirectlog) return;
	g_Log->ev.notify();
	time_t t_begin(time(0));
	bool bneedwait = false;

	do 
	{
		for (unsigned i = 0; i < g_Log->m_iblocknum; ++i)
		{
			if(g_Log->m_blocks[i].ilenuse > 0){bneedwait = true;break;}
		}

		if(!bneedwait) break;
		if(time(0) - t_begin >= 5) break;
		HCommonFun::Sleep(300);
	} while (false);
}

void log_init_real_inner(const char* psDir, const char* psLogFname,bool bdirectlog,unsigned iminflushnum, int iOutputLevel, int iDisplayWhere)
{
	if(g_Log != NULL) return;

	g_Log = new HLogFile();

	g_Log->bdirectlog = bdirectlog;
	//g_Log->m_iminflushnum = iminflushnum;
	//只有在不直接写日志的情况下才启动线程
	if(bdirectlog)
	{
		g_Log->m_ilenmax = 204800;
		g_Log->m_bufflog = new char[g_Log->m_ilenmax];
		g_Log->m_iblocknum = g_Log->m_ilenmax/2048;
		g_Log->m_blocks = new stublock[g_Log->m_iblocknum];
		ZeroMemory(g_Log->m_blocks,g_Log->m_iblocknum * sizeof(stublock));
		for(unsigned i = 0 ; i < g_Log->m_iblocknum; ++i)
		{
			g_Log->m_blocks[i].istatus = 0;
			g_Log->m_blocks[i].buff = g_Log->m_bufflog + (i * 2048);
		}
		//g_Log->start().detach();
	}
	else
	{
		g_Log->m_bufflog = new char[g_Log->m_ilenmax];
		//new 出来交换的buff
		g_Log->m_iblocknum = g_Log->m_ilenmax/2048;
		g_Log->m_blocks = new stublock[g_Log->m_iblocknum];
		ZeroMemory(g_Log->m_blocks,g_Log->m_iblocknum * sizeof(stublock));
		for(unsigned i = 0 ; i < g_Log->m_iblocknum; ++i)
		{
			g_Log->m_blocks[i].istatus = 0;
			g_Log->m_blocks[i].buff = g_Log->m_bufflog + (i * 2048);
		}
		g_Log->start()->detach();
	}

	int i = 0;
	int iLen = 0;

	g_Log->m_iOutputLevel = iOutputLevel;
	switch(iDisplayWhere)
	{
	case DISPLAY_NONE:
		g_Log->m_bDisplayFile = false;
		g_Log->m_bDisplayScreen = false;
		break;
	case DISPLAY_FILE:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = false;
		break;
	case DISPLAY_SCREEN:
		g_Log->m_bDisplayFile = false;
		g_Log->m_bDisplayScreen = true;
		break;
	case DISPLAY_BOTH:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = true;
		break;
	default:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = true;
	}

	g_Log->m_lsecdLast = 0;
	g_Log->m_pfile = NULL;
	//g_Log->m_lMaxFileSizeByte = 50*1024*1024*20;

	if(psDir)
	{
		g_Log->m_strDir = psDir;
		HCommonFun::DirFit(g_Log->m_strDir,true);
		HCommonFun::FixPathLinuxSlash(g_Log->m_strDir);
		HCommonFun::MakeDir(g_Log->m_strDir);
	}
	else
		g_Log->m_strDir = "";

	if(psLogFname && (strlen(psLogFname) > 0)) 
		g_Log->m_strLogName = psLogFname;
	else 
		g_Log->m_strLogName = "app";

	g_Log->m_strlogFilenow = g_Log->m_strDir + g_Log->m_strLogName + "_01.log";
	g_Log->m_strlogFileold = g_Log->m_strDir + g_Log->m_strLogName + "_02.log";
}

void log_set_outlevel(int iOutputLevel)
{
	if(g_Log == NULL) return;
	if(iOutputLevel > LOG_ERROR) iOutputLevel = LOG_ERROR;
	else if(iOutputLevel < LOG_DEBUG) iOutputLevel = LOG_DEBUG;
	g_Log->m_iOutputLevel = iOutputLevel;
	return ;
}

int log_get_outlevel()
{
	if(g_Log == NULL) return -1;
	return g_Log->m_iOutputLevel;
}

void log_set_displaywhere(int iDisplayWhere)
{
	if(g_Log == NULL) return;

	switch(iDisplayWhere)
	{
	case DISPLAY_NONE:
		g_Log->m_bDisplayFile = false;
		g_Log->m_bDisplayScreen = false;
		break;
	case DISPLAY_FILE:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = false;
		break;
	case DISPLAY_SCREEN:
		g_Log->m_bDisplayFile = false;
		g_Log->m_bDisplayScreen = true;
		break;
	case DISPLAY_BOTH:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = true;
		break;
	default:
		g_Log->m_bDisplayFile = true;
		g_Log->m_bDisplayScreen = true;
	}
}

void log_set_filesize(const unsigned& lFileSizeKByte)  /*default is 5000KByte (5M).*/
{
	if(g_Log == NULL) return;
	if(lFileSizeKByte < 5)
		g_Log->m_lMaxFileSizeByte = 5 * 1024;
	else
		g_Log->m_lMaxFileSizeByte = lFileSizeKByte * 1024;
}

void log_set_path(const char* psDir, const char* psLogName)
{
	if(g_Log == NULL) return;
	int iTemp = 0;
	if(psDir && psDir[0] != '\0')
	{
		g_Log->m_strDir = psDir;
		if(!g_Log->m_strDir.empty())
		{
			HCommonFun::DirFit(g_Log->m_strDir,true);
			HCommonFun::MakeDir(psDir);
		}
	}
	if(psLogName && psLogName[0] != '\0')
	{
		g_Log->m_strLogName = HCommonFun::GetShortName(psLogName);

		if(HCommonFun::is_end_by(g_Log->m_strLogName,".exe") || HCommonFun::is_end_by(g_Log->m_strLogName,".log"))
		{
			g_Log->m_strLogName.resize(g_Log->m_strLogName.length() - 4);
			g_Log->m_strLogName.push_back('_');
		}
	}
}

void log_set_mainarg(int argc, char** argv)  /*the argument from main function*/
{
	if(g_Log == NULL) return;
	int i = 0;
	if(argc<1) return;

	g_Log->m_strDir.clear();
	g_Log->m_strLogName = HCommonFun::Spilt_to_Path_and_Short(argv[0],g_Log->m_strDir,g_Log->m_strLogName);
	g_Log->m_strLogName = HCommonFun::GetNoSuffix(g_Log->m_strLogName) + "_";
}

// int log_get_inner_buff_number()
// {
// 	if(g_Log == NULL) return 0;
// 	return (int)(g_Log->m_ilognumnow);
// }

void* log_get_thread_pointer()
{
	return g_Log;
}

// 获得日志打印等级
int get_log_level(int &iOutLevel, const int iDebugLevel)
{
	if(g_Log == NULL) return LOG_DEBUG;
	return g_Log->m_iOutputLevel;
}

WLog log_get_block(const char* filename,int linenum, int level)
{
	if(g_Log == NULL)
	{
		perror("You should call log_init() first\n");
		log_init_real(NULL, NULL,true,10,LOG_INFO,DISPLAY_FILE);
		if(g_Log == NULL)
		{
			throw std::exception("log_get_block log_init_real failed...");
		}
	}

	if(filename == NULL)
	{
		//perror("You should input filename param first...\n");
		//return;
		filename = "w3unkowncpp";
	}

	if(level < g_Log->m_iOutputLevel) return NULL;

	if((!g_Log->m_bDisplayFile) && (!g_Log->m_bDisplayScreen)) return NULL;

	//int iYear, iMonth, iDay, iHour, iMin, iSec;
	//int i(0);
	stublock* block = &g_Log->m_blocks[(((unsigned long)HCommonFun::InterlockedIncrementM(&g_Log->mi_write_index))%g_Log->m_iblocknum)];
	HCommonFun::InterlockedExchangeM(&block->istatus,2);
	//t = 0;
	time_t t_now;
	time(&t_now);
	block->ilenuse = strftime(block->buff,2048,"<%Y-%m-%d %H:%M:%S,",localtime(&t_now));
	int t = (int)strlen(filename);
	for (int i = t - 1; i >= 0 ;--i)
	{
		if(filename[i] == '/' || filename[i] == '\\')
		{
			filename = filename + i + 1;
			t -= (i + 1);
			break;
		}
	}

	if(t > 16) t = 16;
	else
	{
		memset(block->buff + block->ilenuse,' ',16 - t);
		block->ilenuse += (16 - t);
	}

	memcpy(block->buff + block->ilenuse,filename,t);
	block->ilenuse += t;
	block->ilenuse += sprintf_s(block->buff + block->ilenuse,500,",%.4d>",linenum);

	switch(level)
	{
	case LOG_DEBUG:
		memcpy(block->buff + block->ilenuse,"Debug: ",7);
		block->ilenuse += 7;
		break;
	case LOG_INFO:
		memcpy(block->buff + block->ilenuse,"Info: ",6);
		block->ilenuse += 6;
		break;
	case LOG_WARN:
		memcpy(block->buff + block->ilenuse,"Warning: ",9);
		block->ilenuse += 9;
		break;
	case LOG_ERROR:
		memcpy(block->buff + block->ilenuse,"Error: ",7);
		block->ilenuse += 7;
		break;
	default:
		memcpy(block->buff + block->ilenuse,"Error: ",7);
		block->ilenuse += 7;
		break;
	}

	return block;
}

void log_end_and_notify(WLog& log)
{
	if(log.block == NULL) return;
	if(log.block->ilenuse + 2 >= __Max_Blob_Length_HLogFile_) log.block->ilenuse = __Max_Blob_Length_HLogFile_ - 2;
	log.block->buff[log.block->ilenuse++] = '\n';
	//log.block->buff[log.block->ilenuse++] = 0;
	HCommonFun::InterlockedExchangeM(&log.block->istatus,1);
	if(g_Log->bdirectlog){g_Log->write_log_direct(log.block);return;}
	g_Log->ev.notify();
}

#if defined( WIN32 ) && !defined( _USE_BUGSLAYER )
LONG __stdcall MyExceptionHandler( _EXCEPTION_POINTERS* ExceptInfo )
{
	__try
	{
		//printf( "MyExceptionHandler enter ...\n");
		char szFileName[512];

		sprintf_s(szFileName,512,".\\%s_dump.dmp", g_szProcessName );

		HANDLE hFile = CreateFileA( szFileName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL );

		if ( hFile == INVALID_HANDLE_VALUE ) 
			printf( "CreateFile failed, %d\n", GetLastError() );
		else
		{
			//printf( "MyExceptionHandler here ...num:%d\n",__LINE__);
			MINIDUMP_EXCEPTION_INFORMATION miniExcept;

			miniExcept.ExceptionPointers = ExceptInfo;
			miniExcept.ClientPointers = TRUE;
			miniExcept.ThreadId = GetCurrentThreadId();

			if(MiniDumpWriteDump( GetCurrentProcess(),
				GetCurrentProcessId(),
				hFile,
				MiniDumpWithFullMemory,
				&miniExcept,
				NULL,
				NULL ))
			//	printf( "MyExceptionHandler here 1...num:%d,dw:%u\n",__LINE__,GetLastError());
			//else
			//	printf( "MyExceptionHandler here 2...num:%d,dw:%u\n",__LINE__,GetLastError());
			CloseHandle( hFile );
		}
	}
	__except ( MyExceptionHandler( GetExceptionInformation() ) )
	{
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif

#if defined(WIN32) && defined(_USE_BUGSLAYER)
static long __stdcall MyExceptionHandler( EXCEPTION_POINTERS* pExPtrs )
{
	DWORD dwOpts;
	HANDLE hFile;
	DWORD dwFileLenWritten;
	char szPrintBuffer[4096];
	char szDateTimeString[256];

	log_uninit();

	get_current_time2( szDateTimeString, 256, "YYYY-MM-DD hh:mm:ss" );

	if ( g_iLogDirType == 0 ) {
		make_dir( "..\\log" );
		sprintf_s( szPrintBuffer,4096,"..\\log\\%s_dump.log", g_szProcessName );
	}
	else {
		make_dir( ".\\log" );
		sprintf_s( szPrintBuffer,4096, ".\\log\\%s_dump.log", g_szProcessName );
	}

	printf( "Use log file %s\n", szPrintBuffer );

	hFile = CreateFile( szPrintBuffer,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL );

	if ( hFile == INVALID_HANDLE_VALUE ) 
		printf( "CreateFile failed, %d\n", GetLastError() );

	SetFilePointer( hFile, 0, 0, FILE_END );

	sprintf_s( szPrintBuffer,4096,"<%s>Error: An exception occured at address 0x%08x, code:[0x%08x]\r\n",
		szDateTimeString,
		pExPtrs->ExceptionRecord->ExceptionAddress,
		pExPtrs->ExceptionRecord->ExceptionCode );

	if ( hFile != INVALID_HANDLE_VALUE && !WriteFile( hFile, szPrintBuffer, (DWORD)strlen( szPrintBuffer ), &dwFileLenWritten, NULL ) )
		printf( "WriteFile failed, %d\n", GetLastError() );

	printf( "%s", szPrintBuffer );
	fflush( stdout );

	dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
	
    const char * szBuff = GetFirstStackTraceString ( dwOpts  ,
                                                         pExPtrs  ) ;
    do
    {
		sprintf_s( szPrintBuffer,4096,
			"<%s>Stack walking: [%s]\r\n",
			szDateTimeString,
			szBuff );
		if ( hFile != INVALID_HANDLE_VALUE && !WriteFile( hFile, szPrintBuffer, (DWORD)strlen( szPrintBuffer ), &dwFileLenWritten, NULL ) )
			printf( "WriteFile failed, %d\n", GetLastError() );

		printf( "%s", szPrintBuffer );
		fflush( stdout );

        szBuff = GetNextStackTraceString ( dwOpts , pExPtrs ) ;
    }
    while ( NULL != szBuff ) ;

	if ( hFile != INVALID_HANDLE_VALUE )
		CloseHandle( hFile );

	return EXCEPTION_CONTINUE_SEARCH;
}
#endif

#ifdef WIN32
#ifdef _USE_BUGSLAYER

void set_ufl_exception_handler( const char* szProcessName, bool bPrintModules, int iLogDirType )
{
    HMODULE ahMods[ 250 ] ;
    DWORD   dwNumMods ;
    BOOL    bRet ;

	strcpy( g_szProcessName, szProcessName );

	g_iLogDirType = iLogDirType;

	SetUnhandledExceptionFilter( MyExceptionHandler );

	if ( bPrintModules ) {
		// Get the loaded modules for this process.
		memset ( &ahMods , NULL , sizeof ( ahMods ) ) ;

		bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
								250                     ,
								(HMODULE*)&ahMods       ,
								&dwNumMods               ) ;
		if ( ( FALSE == bRet ) || ( 0 == dwNumMods ) )
		{
			log_write_fl0( LOG_ERROR,
				"Failed getting loaded modules for this process!" ) ;
			return ;
		}

		log_write_fl0( LOG_INFO,
			"Modules for this process -" ) ;

		char szBuff[ MAX_PATH ] ;

		for ( UINT i = 0 ; i < dwNumMods ; i++ )
		{
			GetModuleFileName ( ahMods[ i ] , szBuff , sizeof ( szBuff ) ) ;
			log_write_fl2( LOG_INFO,
				"   0x%08X - %s" , ahMods[ i ] , szBuff ) ;
		}
	}
}

#else
void set_ufl_exception_handler( const char* szProcessName, bool bPrintModules, int iLogDirType)
{
	if(szProcessName == NULL)
	{
		string strshort = HCommonFun::GetNoSuffix(HCommonFun::GetShortName(HCommonFun::GetMyExeFullPath()));
		memcpy(g_szProcessName,strshort.c_str(),strshort.length());
		g_szProcessName[strshort.length()] = 0;
	}
	else
	{
		int ilen = (int)strlen(szProcessName);
		if(ilen >= 256) ilen = 255;
		memcpy(g_szProcessName, szProcessName,ilen);
		g_szProcessName[ilen] = 0;
	}
	SetUnhandledExceptionFilter( MyExceptionHandler );
}

#endif
#endif
