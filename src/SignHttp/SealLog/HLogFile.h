/******************************************************************************************
����:��־��,���ٴ�ӡ��־�������Թ�ÿ���д30M����־���ڲ�������һ���߳�ר������д��־��
д��־����ʱ���Ὣ��װ�õ�ʱ��push���������棬������־�߳�ר��д��Ӳ�����
����:��ΰ
����:2017/07/20
����޸�ʱ��:2017/07/25
*******************************************************************************************/
#pragma once

#ifndef _WIN32
#if !defined( _GNU_SOURCE )
#define _GNU_SOURCE
#endif
#endif

#include <stdio.h>
#include <string>
using namespace std;

//#define MemoryUseUp			(9527110)
#define G_MAXPATHLEN		1024
#define __Max_Blob_Length_HLogFile_ (2048)//ÿ����־��󳤶�2048
//��־��ӡ������
#define DISPLAY_NONE       0
#define DISPLAY_FILE       0x01
#define DISPLAY_SCREEN     0x02
#define DISPLAY_BOTH       (DISPLAY_FILE | DISPLAY_SCREEN)

#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARNING
#undef LOG_ERROR
//��־д����
#define LOG_DEBUG   0x01  //������Ϣ
#define LOG_INFO    0x02  //һ��������Ϣ
#define LOG_WARN	0x03  //������Ϣ
#define LOG_ERROR	0x04   //������Ϣ

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

//filename:��־�ļ�����(short name),������׺
//iOutputLevel:��־����,������L0G_DEBUG,LOG_INFO,LOG_WARNING,LOG_ERROR,ֻ������ڵ����Լ��������־
//iDisplayWhere:��ʾ�ط�DISPLAY_FILE
void log_init_real(const char* filedir = NULL,//��־�ļ�����
				   const char* filename = NULL,//��־�ļ�����
				   bool bdirectlog = true,//��־�Ƿ�ֱ��д��־(��,��������д��־)�����Ϊtrue��iminflushnum��������
				   unsigned iminflushnum = 10,//��С��д����־�ļ�����Ŀ��,(��������������л����־�ļ�)
				   int iOutputLevel = LOG_INFO,//��־����
				   int iDisplayWhere = DISPLAY_BOTH//��־д������
				   );
const char* log_get_log_path();
void log_write_force();//ǿ�ƽ�ʣ���logд��
void log_set_path(const char* psDir, const char* psLogName);
void log_set_outlevel(int iOutputLevel);
int  log_get_outlevel();
void log_set_displaywhere(int iDisplayWhere);
//������־�ļ���С����λKB
void log_set_filesize(const unsigned& lFileSizeKByte);/*default is 5,000,000 unsigned char (<5M).*/
void log_set_mainarg(int argc, char** argv);/*the argument from main function*/
//int log_get_inner_buff_number();
//��ȡȫ�ֵ�д��־�߳�ָ��
void* log_get_thread_pointer();

// �����־��ӡ�ȼ�
int get_log_level();
//iErrorCode: ��־����
void log_write(const char* FILE_NAME, const int LINE_NUM, int iErrorCode, const char *format, ... );

//#define log_write_fl(iErrorCode, psErrorMsg)   log_write(__FILE__, __LINE__, iErrorCode, psErrorMsg)
#define log_write_std(strContext)              log_write(__FILE__, __LINE__, errno, "%s: %s", strContext, strerror(errno))
#define log_write_unimp(strContext)            log_write(__FILE__, __LINE__, errno, "%s: unimplement", strContext)

//@param bPrintModules �Ƿ��ڵ��ô˺���ʱ��ӡ����������ģ�����Ƽ���ִ�г����·����ȱʡΪfalse
//@param iLogDirType ��־Ŀ¼���ͣ�0��ʾ����Ĭ����־Ŀ¼����../log/��1��ʾ./log/
void set_ufl_exception_handler( const char* szProcessName, bool bPrintModules = false, int iLogDirType = 0 );

// lock a pid file, ��֤����������Ψһ��
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