/******************************************************************************************
功能:日志类,急速打印日志，极限试过每秒可写30M的日志，内部会启动一个线程专门用来写日志，
写日志请求时，会将组装好的时候，push到队列里面，再有日志线程专门写到硬盘落地
作者:王伟
日期:2017/07/20
最后修改时间:2017/07/25
*******************************************************************************************/
#pragma once

#ifndef WIN32
#if !defined( _GNU_SOURCE )
#define _GNU_SOURCE
#endif
#endif

#include <stdio.h>
#include <string>
using namespace std;

//#define MemoryUseUp			(9527110)
#define G_MAXPATHLEN		1024
#define __Max_Blob_Length_HLogFile_ (2048)//每条日志最大长度2048
//日志打印在哪里
#define DISPLAY_NONE       0
#define DISPLAY_FILE       0x01
#define DISPLAY_SCREEN     0x02
#define DISPLAY_BOTH       (DISPLAY_FILE | DISPLAY_SCREEN)

#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARNING
#undef LOG_ERROR
//日志写级别
#define LOG_DEBUG   0x01  //调试信息
#define LOG_INFO    0x02  //一般正常信息
#define LOG_WARN	0x03  //警告信息
#define LOG_ERROR	0x04   //错误信息

//HLog(HLogError << "asksjhdsjk 21121212123133132"<<10 << 1205.121 << "si pang zi");
//added by wz, at 05-01-04
//HLogWrite1(LOG_ERROR,"ReadIniFile failed,i_ret:%d",i_ret);
#define HLogWrite0( iErrorCode, psErrorMsg ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg )
#define HLogWrite1( iErrorCode, psErrorMsg, arg1 ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg, arg1 )
#define HLogWrite2( iErrorCode, psErrorMsg, arg1, arg2 ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg, arg1, arg2 )
#define HLogWrite3( iErrorCode, psErrorMsg, arg1, arg2, arg3 ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg, arg1, arg2, arg3 )
#define HLogWrite4( iErrorCode, psErrorMsg, arg1, arg2, arg3, arg4 ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg, arg1, arg2, arg3, arg4 )
#define HLogWrite5( iErrorCode, psErrorMsg, arg1, arg2, arg3, arg4, arg5 ) log_write( __FILE__,__LINE__,iErrorCode, psErrorMsg, arg1, arg2, arg3, arg4, arg5 )
//HLog(HLogError << "asksjhdsjk 21121212123133132"<<10 << 1205.121 << "si pang zi");
#define HLog(x) log_end_and_notify(x)
#define HLogDebug log_get_block( __FILE__,__LINE__,LOG_DEBUG)
#define HLogInfo log_get_block( __FILE__,__LINE__,LOG_INFO)
#define HLogWarn log_get_block( __FILE__,__LINE__,LOG_WARN)
#define HLogError log_get_block( __FILE__,__LINE__,LOG_ERROR)
struct WLog;
WLog log_get_block(const char* filename,int linenum, int ierrorlevel);
void log_end_and_notify(WLog& block);

//filename:日志文件名称(short name),不带后缀
//iOutputLevel:日志级别,可以是L0G_DEBUG,LOG_INFO,LOG_WARNING,LOG_ERROR,只输出大于等于自己级别的日志
//iDisplayWhere:显示地方DISPLAY_FILE
void log_init_real(const char* filename = NULL,//日志文件名称
				   bool bdirectlog = true,//日志是否直接写日志(即,不做批量写日志)，如果为true，iminflushnum将被忽略
				   unsigned iminflushnum = 10,//最小的写入日志文件的条目数,(即多少条后批量谢日日志文件)
				   int iOutputLevel = LOG_INFO,//日志级别
				   int iDisplayWhere = DISPLAY_BOTH//日志写到哪里
				   );
const char* log_get_log_path();
void log_write_force();//强制将剩余的log写掉
void log_set_path(const char* psDir, const char* psLogName);
void log_set_outlevel(int iOutputLevel);
int  log_get_outlevel();
void log_set_displaywhere(int iDisplayWhere);
//设置日志文件大小，单位KB
void log_set_filesize(const unsigned& lFileSizeKByte);/*default is 5,000,000 unsigned char (<5M).*/
void log_set_mainarg(int argc, char** argv);/*the argument from main function*/
//int log_get_inner_buff_number();
//获取全局的写日志线程指针
void* log_get_thread_pointer();

// 获得日志打印等级
int get_log_level();
//iErrorCode: 日志级别
void log_write(const char* FILE_NAME, const int LINE_NUM, int iErrorCode, const char *format, ... );

//#define log_write_fl(iErrorCode, psErrorMsg)   log_write(__FILE__, __LINE__, iErrorCode, psErrorMsg)
#define log_write_std(strContext)              log_write(__FILE__, __LINE__, errno, "%s: %s", strContext, strerror(errno))
#define log_write_unimp(strContext)            log_write(__FILE__, __LINE__, errno, "%s: unimplement", strContext)

//@param bPrintModules 是否在调用此函数时打印进程依赖的模块名称及可执行程序的路径，缺省为false
//@param iLogDirType 日志目录类型，0表示采用默认日志目录，即../log/，1表示./log/
void set_ufl_exception_handler( const char* szProcessName, bool bPrintModules = false, int iLogDirType = 0 );

// lock a pid file, 保证进程启动的唯一性
bool lock_file( const char* file_name );
struct stublock;
struct WLog
{
	WLog(stublock* p = NULL);
	stublock* block;//
	//stublock* operator << (const char []);
	WLog& operator << (const char* plog);
	WLog& operator << (const string& strlog);
	WLog& operator << (int ilog);
	WLog& operator << (unsigned int ilog);
	WLog& operator << (long ilog);
	WLog& operator << (unsigned long ilog);
	//WLog& operator << (unsigned int ilog);
	WLog& operator << (short ilog);
	WLog& operator << (unsigned short ilog);
	WLog& operator << (const long long& strlog);
	WLog& operator << (const unsigned long long& i64log);
	WLog& operator << (const double& doulog);
	void Appenlog(const char* plog,int ilen);
};