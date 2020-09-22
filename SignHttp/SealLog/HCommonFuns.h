#pragma once
#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <time.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <stdio.h>
using namespace std;

#ifndef WIN32
//typedef long long __int64;
typedef void* HMODULE;
#define CP_ACP			(0)
//#define sprintf_s snprintf
//#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
//#define SW_HIDE             0
//typedef unsigned int DWORD;
//#define FALSE	(0)
//#define TRUE	(1)
#endif

#ifndef MAX_PATH
#define MAX_PATH (260)
#endif

#ifndef NULL
#define NULL	(0)
#endif
//new出来8的整数倍的内存，最少会多new出来1字节，最多会多new出来8字节(档n为8的整数倍时会多new出来8字节),以减少内存碎片
#define new_charM(__n__) new char [(__n__|7) + 1];
//new出来8的整数倍的内存，最多会多new出来10个字节，最少会多new出来8字节,以减少内存碎片
#define new_wcharM(__n__) new wchar_t [(__n__|3) + 1];
/*************************************************************************
* Function: convert long-second (since 1970-01-01 00:00:00) to datetime
*************************************************************************/
namespace HCommonFun
{
	class MFileNode
	{
	public:
		MFileNode();
		string strFileFullName; // "c:\tem\aaa"
		string strFileLastName; // "aaa"
		time_t t_last_modify;
		time_t t_create;
		__int64 i_size;//文件大小
		bool b_dir;
	};
	//设置使用中文环境
	void set_def_locale();
	//将指针移动到指向'\0'的位置
	char* MoveToEnd(char** p);
	//int转string
	string int2str(int i,int radix = 10);
	//int64转string
	string i64tostr(const __int64& i);
	//double to string
	string doubletostr(const double& dou,int iPrecison = 2);
	//time_t to 2010 01 20 19 30 50
	int sec2time1(const time_t& lSecd,int* pYear,int* pMonth = NULL, int* pDay = NULL,int* pHour = NULL,int* pMin = NULL,int* pSec = NULL, int* piWeekDay = NULL);
	//年月日时分秒转成秒数
	int time2sec(int iYear, int iMonth, int iDay,int iHour, int iMin, int iSecd,time_t& pLSecd);
	//'20151010165013' => time_t
	time_t string2sec(const string& strtime);
	//年月日时分秒转成秒数 time_t => '20151010165013'
	string sec2string(const time_t& lSecd = time(0));
	// time_t => '2015-10-10 16:50:13'
	string sec2strDateTime(const time_t& lSecd = time(0));
	//"2015-10-10 16:50:13'  => time_t
	time_t strDateTime2sec(const string& strtime);
	//获取今天00:00:00的时间格式
	string strDateTimeToday();

	//acsii --> unicode,在里面wstr可能被new了内存，请记得delete
	int char2wchar(const char* str,int ilenin, wchar_t** wstr,int& ilenout,unsigned int ui_code_page = CP_ACP);
	//wchar => char, 当*str != NULL时，ilen需要赋值
	int wchar2char(const wchar_t* wstr,int ilenin, char** str,int& ilenout,unsigned int ui_code_page = CP_ACP);
	//string => wstring
	wstring str2wstring(const string& str,unsigned int ui_code_page = CP_ACP);
	//wstring => string
	string wstr2string(const wstring& wstr,unsigned int ui_code_page = CP_ACP);

	string wstr2ICEstring(const wstring& wstr);
	wstring ICE2wstring(const string& wstr);
	string str2ICEstring(const string& str);
	string ICE2string(const string& str);
	//获取utf8编码字符串的unicode字符个数
	int ICECountCharNum(const string& str);
	//UTF8编码,获取前N个字符数(注意:是字符数不是字节数),对应的字符个数,
	int ICEGetHeadNCharLength0(const string& strICE,int iWish);
	int ICEGetHeadNCharLength1(const char* buff,int iWish,int ibufflen = -1);

	//获取文件内容，*strCAContent为NULL，内部会new空间，外部要记得delete
	int GetFileContent(const char* strFilePath,char** strContent,int& iContentLen);
	//获取文件内容，内部不会new buff，buff长度如果不够，会直接返回
	int GetFileContent1(const char* strFilePath,char* strContent,int& iContentLen);
	//获取文件大小
	__int64 GetFileLength(const char* strFilePath);
	//文件是否存在
	bool isFileExistA(const char* strpath);
	//文件是否存在，是否是文件夹
	bool isFileExistA(const char* strpath,bool& b_dir);
	bool IsDirEmpty(const char* _strDir);
	bool isFileExistW(const wchar_t* strpath);
	bool isFileExistW(const wchar_t* strpath,bool& b_dir);
	//文件设置为隐藏文件(仅支持Windows平台)
	int SetFileHide(const char* _str_file, bool b_hide);
	//覆盖式写文件
	int WriteFileCovery(const char* strFile,const char* strContent,int iLen);
	//追加式写文件
	int WriteFileAfter(const char* strFile,const char* strContent,int iLen);
	//不关闭文件句柄的追加内容
	int WriteFileAfterNoClose(FILE*& pfile,const char* strFile,const char* strContent,int iLen);
	//关闭文件句柄
	int WriteFileAfterCloseFILE(FILE*& pfile);
	//字符串是否是utf8格式
	bool IsTextUTF8(const char* pstr);
	//utf8转ansi
	void Utf8ToAnsi(const unsigned char * lpsrc,const int srclen, char* lpdst, int& dstlen);
	//将字符串里面的字符串去掉 "abcedf"=> "edf"
	void StrCut(char* strin,int ilen,int& ilenout,const char* strtoCut);
	//str后面追加内部strin会被delete,再new出来,注意strin不能是 char str[xx]这种
	void StrApend(char** strin,int& ilen,const char* strApend);
	//str前面追加,注意strin不能是 char str[xx]这种
	void StrPushFront(char** strin,int& ilen,const char* strHead);
	//去掉字符串里面某个字符
	char* trim(char *str, char chr);
	string& trim(string& str, char chr);
	string& trim(string& str, const string& strlist);
	//去掉末尾的某个字符，如果该字符存在
	char* trim_begin(char *str, const char* strlist);
	string& trim_begin(string& str, const char* strlist);
	//去掉末尾的某个字符，如果该字符存在
	char* trim_end(char *str, const char* strlist);
	string& trim_end(string& str, const char* strlist);

	char* trim_both(char *str,const char* strlist);
	string& trim_both(string& str,const char* strlist);

	//创建文件夹
	int MakeDir(const string& strDir);
	int MakeDir1(const char* strDir,int idirlen = 0);
	//是否是正常的ipv4
	bool is_ipv4_format(const string& str);
	//获取当前进程的全路径,linux下hmode参数无效
	string GetMyExeFullPath(const HMODULE& hmode = NULL);
	wstring GetMyExeFullPathW(const HMODULE& hmode = NULL);
	/*获取文件文件夹路径 c:\abc\112.txt => c:\abc\ */
	string GetParentPath(const string& strFile,bool bHaveSlash = true/*是否要斜杠*/);
	//c:\abc\123.txt => 123.txt
	string GetShortName(const string& strFile,bool bHaveSuffix = true/*是否要后缀*/);
	//把路径拆分为路径和shortname
	bool Spilt_to_Path_and_Short(const string& strFile,string& strPath,string& strShort,bool bHaveSlash = true);
	//获取文件路径后缀名
	string GetFileSuffix(const string& strFile);
	//获取文件路径后缀名
	wstring GetFileSuffixW(const wstring& strFile);
	//去掉路径后缀名"C:/share/abc.exe" => "C:/share/abc"
	string GetNoSuffix(const string& strFile);
	wstring GetNoSuffixW(const wstring& strFile);
	//替换字符串
	string& replace_ex(string& strin,const string& str_old, const string& str_new,bool b_case_sense = false);
	// 适配目录 给一个目录，如果 b_have_slash 真， "asdf/sadf/" or "asdf/sadf" => "asdf/sadf/"  ; 如果 b_have_slash 假， "asdf/sadf/" or "asdf/sadf" => "asdf/sadf"
	void DirFit(string& strdir,bool bHaveslash);
	// if win: / => \\, if lin: \\ => /
	void FixPathSlash(string& strPath);
	//\\ => /
	void FixPathLinuxSlash(string& strPath);
	//将字符串以字符c分割，并放到vector中
	int Get_vt_Str_By_Sep(const string& strin,vector<string>& vt_str,const char& c);
	//将unicode字符串以字符wc分割,并放到vector中
	int Get_vt_WStr_By_Sep(const wstring& strin,vector<wstring>& vt_str,const wchar_t& wc);
	//将字符串以字符c分割，并放到set中
	int Get_set_Str_By_Sep(const string& strin,set<string>& set_str,const char& c);
	//ret <0 if s1<s2;ret =0 if s1=s2,  ret >0 if s1>s2
	bool is_equal_str(const char* s1,const char* s2,bool bCaseSense = false);
	bool is_equal_str(const string& str1,const string& str2,bool bCaseSense = false);
	bool is_equal_str(const char* s1,int ilen1,const char* s2,int ilen2,bool bCaseSense = false);
	//是否前n个字符一样
	bool is_begin_by(const char* s1,const char* s2,int isize,bool bCaseSense = false);
	bool is_begin_by(const string& strin,const string& strbegin,bool bCaseCense = false);
	bool is_end_by(const string& strin,const string& strend,bool bCaseCense = false);
	//从pin指针开始向后查找pin里面是否包含某个字符串，ilenpin为pin的长度，如果不输入，则找到*pin = '\0'为止
	const char* find(const char* pin,const char* pstrfind,int ilenpin = -1,bool bcasecense = false);
	//从pin指针开始向前查找字符串里面是否包含某个字符串，ilenpin为pin向前查找的最大长度，必须输入
	const char* rfind(const char* pin,const char* pstrfind,int ilenpin = -1,bool bcasecense = false);
	//psDest 如果*psDest == NULL记得释放内存
	void String2Upper(const char* psSrc, char*& psDest);
	//将自身转为大写
	char* str2Upper(char* psString);
	string& str2Upper(string& psString);
	//将自身转为小写
	char* str2Lower(char* buff);
	string& str2Lower(string& psString);
	//互斥，可以使系统里面只存在一个进程实例
	bool TakeOwner(const string& str_name);
	//枚举文件夹里的文件,bRecurFind是否递归枚举
	int DirSnap(const string& strDir,vector<MFileNode>& vtout,bool bRecurFind = true);
	//获取文件属性
	int GetFileAttribute(const string& strFilename,MFileNode& nd);
	//获取文件属性
	int SetFileTimeM(const string& strFilename,const time_t& t_modify,const time_t& t_create = 0,const time_t& t_access = 0);
	//如果strTo已经存在，就删除了再move
	bool movefile(const string& strFrom,const string& strTo,bool bFailifExist = false);
	//重命名文件
	int renamefile(const char* psFileOld, const char* psFileNew);
	//把文件或者文件夹拷贝到一个目录下,目前仅支持windows平台
	//当from为文件夹时,将会把from文件夹下所有文件都拷贝到todir目录下
	//当from为文件时,将会把from文件拷贝到todir目录下
	int CopyFileToDir(const string& _strFrom, const string& _strToDir);
	//删除文件
	bool DeleteFileH(const string& _str_file);
	bool DeleteFileH1(const char* _str_file);
	//递归删除目录,返回0，表示成功，返回其他表示删除失败的个数
	int DeleteDirRecur(const string& strDir);
	//创建一个指定大小的文件
	int CreateFileWithNoContent(const string& _str_file, __int64 i_len);
	//读取文件，并且内部不关闭句柄，iReadByte传入想要读取的字节数，并且返回读取的字节数
	//请自动close hfile和pbuff内存
#ifdef WIN32
	//如果ifrom <= 0,则表示从文件头部开始读取，如果iwishByte = -1 表示一直读到文件尾部
	//读取到文件尾部时,会自动关闭文件句柄
	int ReadFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& ifrom,int iwishByte,char*& pbuff,int& iRealByte);
	//写文件，并且内部不关闭句柄，iReadByte传入想要读取的字节数，并且返回读取的字节数
	//请自己手工close hfile
	int WriteFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& ifrom,int iwishByte,const char* pbuff,int& iRealByte);
#endif
	//跨平台版,从指定位置读取文件，如果读取失败，则无需关闭fp文件句柄
	int ReadFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,char* pbuff,int& iRealByte);
	//会回复fp所指向的文件句柄位置
	int GetFileLength1(FILE* fp,__int64& ilen);
	//跨平台版,从指定位置开始写文件，如果写失败，无需关闭fp文件句柄
	int WriteFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,const char* pbuff,int& iRealByte);
	//创建进程
	int NewProcess(const string& strCmd,int iShowMode,int& iResultCode,bool bWait = true);
	//获取文件最后修改时间
	int GetFileModTime(const string& _str_file,time_t& t_modify);
	//获取当前已存在的进程列表
	int GetProcessList(set<string>& setprocess);
	//exe是否在运行
	bool IsProcessRunning(const string& strExePath);
	//获取exe的pid列表
	int GetProcessPID(const string& strExePath,vector<int>& vtid);
	//获取"C:" <==> "\Device\HarddiskVolume1"对应关系
	int GetDosDevicePair(map<string,string>& maptemp);
	//C:\abc.exe ==> \Device\HarddiskVolume1\abc.exe
	string CommonPathToDosPath(const string& strExe);
	//\Device\HarddiskVolume1\abc.exe ==> C:\abc.exe
	string DosPathToCommonPath(const string& strDospath);
	//根据Pid获取进程普通路径 eg:C:\abc.exe
	string GetProcessPathCommonByPID(unsigned ipid);
	//根据Pid获取进程Dos路径个 eg:\Device\HarddiskVolume1\abc.exe
	string GetProcessPathDosByPID(unsigned ipid);
	//获取exe的pid列表，避免内存不断生灭版,szImagePath只是作为防止内存生灭的输入参数，实际内容无意义
	int GetProcessPIDSaving(const string& strExePath,char* szImagePath,int szlen,vector<int>& vtid);
	//获取进程当前使用的内存大小
	__int64 GetProcessCurrentMemory(int pid);
	//杀掉进程
	int KillProcessByPID(int pid);
	//根据进程路径杀掉进程
	int KillProcessByPath(const string& strExePath);

	string MakeUUID();
	int MakeUUID(string& struuid);
	//生成在本机唯一的15位长度的id,根据当前秒数,外加递增序号,每秒最大可以生成2亿个不重复的id
	//注:不允许修改系统时间,否则将会导致生成的id重复
	void make_exclusive_id_rand();//随机初始化内部的递增index
	int make_exclusive_id_by_time(char* buff);
	void make_exclusive_id_by_time(string& strid);

	bool IsWow64();
	//启动服务
	int StartServiceM(const string& strServiceName,int nArg,const char** pArg);
	//停止服务
	int StopServiceM(const string& strServiceName);
	//获取CPU数目
	int GetCPUNumbers();
	//只支持windows
	int WSAStartupWin32();//WSAStartup
	//www.baidu.com ==> 192.168.1.213,域名解析(只支持windows操作系统)
	int NetToIP(const char* strNewAddr,char* strip,int ilen);
	//获取本机ip列表
	int GetIPListLocal(vector<string>& vt);
	//测试ip和端口号是否可以bind,以测试服务器是否可以使用该端口作为监听端口
	int TestIPAndPortBindable(const char* strip,unsigned short port);
	//获取当前线程id
	__int64 GetCurrentPId();
	//获取当前线程id
	__int64 GetCurrentTId();
	//线程sleep微秒
	void Sleep(unsigned int imillSec);
	//获取system32dir，linux是/etc目录
	string GetSystem32Dir(bool bHaveSlash = true);

	__int64 GetTickCountM();
	//交换值，并返交换之前的原值值
	long InterlockedExchangeM(volatile long* i,int iexchange);
	long long InterlockedExchange64M(volatile long long* i,long long iexchange);
	//原子自增,先返回原始值，再自加
	long InterlockedIncrementM(volatile long* i,int iadd = 1);
	//原子自增,先返回原始值，再自加
	long long InterlockedIncrement64M(volatile long long* i,long long iadd = 1);
	//交换数值,并返回之前的数值
	long InterlockedComparedExchangeM(volatile long* i,long iexchange,long lcompvalue);
	//获取原子循环增量的id,线程安全
	long GetIncreasementID();
};