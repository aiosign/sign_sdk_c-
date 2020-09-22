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
//new����8�����������ڴ棬���ٻ��new����1�ֽڣ������new����8�ֽ�(��nΪ8��������ʱ���new����8�ֽ�),�Լ����ڴ���Ƭ
#define new_charM(__n__) new char [(__n__|7) + 1];
//new����8�����������ڴ棬�����new����10���ֽڣ����ٻ��new����8�ֽ�,�Լ����ڴ���Ƭ
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
		__int64 i_size;//�ļ���С
		bool b_dir;
	};
	//����ʹ�����Ļ���
	void set_def_locale();
	//��ָ���ƶ���ָ��'\0'��λ��
	char* MoveToEnd(char** p);
	//intתstring
	string int2str(int i,int radix = 10);
	//int64תstring
	string i64tostr(const __int64& i);
	//double to string
	string doubletostr(const double& dou,int iPrecison = 2);
	//time_t to 2010 01 20 19 30 50
	int sec2time1(const time_t& lSecd,int* pYear,int* pMonth = NULL, int* pDay = NULL,int* pHour = NULL,int* pMin = NULL,int* pSec = NULL, int* piWeekDay = NULL);
	//������ʱ����ת������
	int time2sec(int iYear, int iMonth, int iDay,int iHour, int iMin, int iSecd,time_t& pLSecd);
	//'20151010165013' => time_t
	time_t string2sec(const string& strtime);
	//������ʱ����ת������ time_t => '20151010165013'
	string sec2string(const time_t& lSecd = time(0));
	// time_t => '2015-10-10 16:50:13'
	string sec2strDateTime(const time_t& lSecd = time(0));
	//"2015-10-10 16:50:13'  => time_t
	time_t strDateTime2sec(const string& strtime);
	//��ȡ����00:00:00��ʱ���ʽ
	string strDateTimeToday();

	//acsii --> unicode,������wstr���ܱ�new���ڴ棬��ǵ�delete
	int char2wchar(const char* str,int ilenin, wchar_t** wstr,int& ilenout,unsigned int ui_code_page = CP_ACP);
	//wchar => char, ��*str != NULLʱ��ilen��Ҫ��ֵ
	int wchar2char(const wchar_t* wstr,int ilenin, char** str,int& ilenout,unsigned int ui_code_page = CP_ACP);
	//string => wstring
	wstring str2wstring(const string& str,unsigned int ui_code_page = CP_ACP);
	//wstring => string
	string wstr2string(const wstring& wstr,unsigned int ui_code_page = CP_ACP);

	string wstr2ICEstring(const wstring& wstr);
	wstring ICE2wstring(const string& wstr);
	string str2ICEstring(const string& str);
	string ICE2string(const string& str);
	//��ȡutf8�����ַ�����unicode�ַ�����
	int ICECountCharNum(const string& str);
	//UTF8����,��ȡǰN���ַ���(ע��:���ַ��������ֽ���),��Ӧ���ַ�����,
	int ICEGetHeadNCharLength0(const string& strICE,int iWish);
	int ICEGetHeadNCharLength1(const char* buff,int iWish,int ibufflen = -1);

	//��ȡ�ļ����ݣ�*strCAContentΪNULL���ڲ���new�ռ䣬�ⲿҪ�ǵ�delete
	int GetFileContent(const char* strFilePath,char** strContent,int& iContentLen);
	//��ȡ�ļ����ݣ��ڲ�����new buff��buff���������������ֱ�ӷ���
	int GetFileContent1(const char* strFilePath,char* strContent,int& iContentLen);
	//��ȡ�ļ���С
	__int64 GetFileLength(const char* strFilePath);
	//�ļ��Ƿ����
	bool isFileExistA(const char* strpath);
	//�ļ��Ƿ���ڣ��Ƿ����ļ���
	bool isFileExistA(const char* strpath,bool& b_dir);
	bool IsDirEmpty(const char* _strDir);
	bool isFileExistW(const wchar_t* strpath);
	bool isFileExistW(const wchar_t* strpath,bool& b_dir);
	//�ļ�����Ϊ�����ļ�(��֧��Windowsƽ̨)
	int SetFileHide(const char* _str_file, bool b_hide);
	//����ʽд�ļ�
	int WriteFileCovery(const char* strFile,const char* strContent,int iLen);
	//׷��ʽд�ļ�
	int WriteFileAfter(const char* strFile,const char* strContent,int iLen);
	//���ر��ļ������׷������
	int WriteFileAfterNoClose(FILE*& pfile,const char* strFile,const char* strContent,int iLen);
	//�ر��ļ����
	int WriteFileAfterCloseFILE(FILE*& pfile);
	//�ַ����Ƿ���utf8��ʽ
	bool IsTextUTF8(const char* pstr);
	//utf8תansi
	void Utf8ToAnsi(const unsigned char * lpsrc,const int srclen, char* lpdst, int& dstlen);
	//���ַ���������ַ���ȥ�� "abcedf"=> "edf"
	void StrCut(char* strin,int ilen,int& ilenout,const char* strtoCut);
	//str����׷���ڲ�strin�ᱻdelete,��new����,ע��strin������ char str[xx]����
	void StrApend(char** strin,int& ilen,const char* strApend);
	//strǰ��׷��,ע��strin������ char str[xx]����
	void StrPushFront(char** strin,int& ilen,const char* strHead);
	//ȥ���ַ�������ĳ���ַ�
	char* trim(char *str, char chr);
	string& trim(string& str, char chr);
	string& trim(string& str, const string& strlist);
	//ȥ��ĩβ��ĳ���ַ���������ַ�����
	char* trim_begin(char *str, const char* strlist);
	string& trim_begin(string& str, const char* strlist);
	//ȥ��ĩβ��ĳ���ַ���������ַ�����
	char* trim_end(char *str, const char* strlist);
	string& trim_end(string& str, const char* strlist);

	char* trim_both(char *str,const char* strlist);
	string& trim_both(string& str,const char* strlist);

	//�����ļ���
	int MakeDir(const string& strDir);
	int MakeDir1(const char* strDir,int idirlen = 0);
	//�Ƿ���������ipv4
	bool is_ipv4_format(const string& str);
	//��ȡ��ǰ���̵�ȫ·��,linux��hmode������Ч
	string GetMyExeFullPath(const HMODULE& hmode = NULL);
	wstring GetMyExeFullPathW(const HMODULE& hmode = NULL);
	/*��ȡ�ļ��ļ���·�� c:\abc\112.txt => c:\abc\ */
	string GetParentPath(const string& strFile,bool bHaveSlash = true/*�Ƿ�Ҫб��*/);
	//c:\abc\123.txt => 123.txt
	string GetShortName(const string& strFile,bool bHaveSuffix = true/*�Ƿ�Ҫ��׺*/);
	//��·�����Ϊ·����shortname
	bool Spilt_to_Path_and_Short(const string& strFile,string& strPath,string& strShort,bool bHaveSlash = true);
	//��ȡ�ļ�·����׺��
	string GetFileSuffix(const string& strFile);
	//��ȡ�ļ�·����׺��
	wstring GetFileSuffixW(const wstring& strFile);
	//ȥ��·����׺��"C:/share/abc.exe" => "C:/share/abc"
	string GetNoSuffix(const string& strFile);
	wstring GetNoSuffixW(const wstring& strFile);
	//�滻�ַ���
	string& replace_ex(string& strin,const string& str_old, const string& str_new,bool b_case_sense = false);
	// ����Ŀ¼ ��һ��Ŀ¼����� b_have_slash �棬 "asdf/sadf/" or "asdf/sadf" => "asdf/sadf/"  ; ��� b_have_slash �٣� "asdf/sadf/" or "asdf/sadf" => "asdf/sadf"
	void DirFit(string& strdir,bool bHaveslash);
	// if win: / => \\, if lin: \\ => /
	void FixPathSlash(string& strPath);
	//\\ => /
	void FixPathLinuxSlash(string& strPath);
	//���ַ������ַ�c�ָ���ŵ�vector��
	int Get_vt_Str_By_Sep(const string& strin,vector<string>& vt_str,const char& c);
	//��unicode�ַ������ַ�wc�ָ�,���ŵ�vector��
	int Get_vt_WStr_By_Sep(const wstring& strin,vector<wstring>& vt_str,const wchar_t& wc);
	//���ַ������ַ�c�ָ���ŵ�set��
	int Get_set_Str_By_Sep(const string& strin,set<string>& set_str,const char& c);
	//ret <0 if s1<s2;ret =0 if s1=s2,  ret >0 if s1>s2
	bool is_equal_str(const char* s1,const char* s2,bool bCaseSense = false);
	bool is_equal_str(const string& str1,const string& str2,bool bCaseSense = false);
	bool is_equal_str(const char* s1,int ilen1,const char* s2,int ilen2,bool bCaseSense = false);
	//�Ƿ�ǰn���ַ�һ��
	bool is_begin_by(const char* s1,const char* s2,int isize,bool bCaseSense = false);
	bool is_begin_by(const string& strin,const string& strbegin,bool bCaseCense = false);
	bool is_end_by(const string& strin,const string& strend,bool bCaseCense = false);
	//��pinָ�뿪ʼ������pin�����Ƿ����ĳ���ַ�����ilenpinΪpin�ĳ��ȣ���������룬���ҵ�*pin = '\0'Ϊֹ
	const char* find(const char* pin,const char* pstrfind,int ilenpin = -1,bool bcasecense = false);
	//��pinָ�뿪ʼ��ǰ�����ַ��������Ƿ����ĳ���ַ�����ilenpinΪpin��ǰ���ҵ���󳤶ȣ���������
	const char* rfind(const char* pin,const char* pstrfind,int ilenpin = -1,bool bcasecense = false);
	//psDest ���*psDest == NULL�ǵ��ͷ��ڴ�
	void String2Upper(const char* psSrc, char*& psDest);
	//������תΪ��д
	char* str2Upper(char* psString);
	string& str2Upper(string& psString);
	//������תΪСд
	char* str2Lower(char* buff);
	string& str2Lower(string& psString);
	//���⣬����ʹϵͳ����ֻ����һ������ʵ��
	bool TakeOwner(const string& str_name);
	//ö���ļ�������ļ�,bRecurFind�Ƿ�ݹ�ö��
	int DirSnap(const string& strDir,vector<MFileNode>& vtout,bool bRecurFind = true);
	//��ȡ�ļ�����
	int GetFileAttribute(const string& strFilename,MFileNode& nd);
	//��ȡ�ļ�����
	int SetFileTimeM(const string& strFilename,const time_t& t_modify,const time_t& t_create = 0,const time_t& t_access = 0);
	//���strTo�Ѿ����ڣ���ɾ������move
	bool movefile(const string& strFrom,const string& strTo,bool bFailifExist = false);
	//�������ļ�
	int renamefile(const char* psFileOld, const char* psFileNew);
	//���ļ������ļ��п�����һ��Ŀ¼��,Ŀǰ��֧��windowsƽ̨
	//��fromΪ�ļ���ʱ,�����from�ļ����������ļ���������todirĿ¼��
	//��fromΪ�ļ�ʱ,�����from�ļ�������todirĿ¼��
	int CopyFileToDir(const string& _strFrom, const string& _strToDir);
	//ɾ���ļ�
	bool DeleteFileH(const string& _str_file);
	bool DeleteFileH1(const char* _str_file);
	//�ݹ�ɾ��Ŀ¼,����0����ʾ�ɹ�������������ʾɾ��ʧ�ܵĸ���
	int DeleteDirRecur(const string& strDir);
	//����һ��ָ����С���ļ�
	int CreateFileWithNoContent(const string& _str_file, __int64 i_len);
	//��ȡ�ļ��������ڲ����رվ����iReadByte������Ҫ��ȡ���ֽ��������ҷ��ض�ȡ���ֽ���
	//���Զ�close hfile��pbuff�ڴ�
#ifdef WIN32
	//���ifrom <= 0,���ʾ���ļ�ͷ����ʼ��ȡ�����iwishByte = -1 ��ʾһֱ�����ļ�β��
	//��ȡ���ļ�β��ʱ,���Զ��ر��ļ����
	int ReadFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& ifrom,int iwishByte,char*& pbuff,int& iRealByte);
	//д�ļ��������ڲ����رվ����iReadByte������Ҫ��ȡ���ֽ��������ҷ��ض�ȡ���ֽ���
	//���Լ��ֹ�close hfile
	int WriteFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& ifrom,int iwishByte,const char* pbuff,int& iRealByte);
#endif
	//��ƽ̨��,��ָ��λ�ö�ȡ�ļ��������ȡʧ�ܣ�������ر�fp�ļ����
	int ReadFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,char* pbuff,int& iRealByte);
	//��ظ�fp��ָ����ļ����λ��
	int GetFileLength1(FILE* fp,__int64& ilen);
	//��ƽ̨��,��ָ��λ�ÿ�ʼд�ļ������дʧ�ܣ�����ر�fp�ļ����
	int WriteFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,const char* pbuff,int& iRealByte);
	//��������
	int NewProcess(const string& strCmd,int iShowMode,int& iResultCode,bool bWait = true);
	//��ȡ�ļ�����޸�ʱ��
	int GetFileModTime(const string& _str_file,time_t& t_modify);
	//��ȡ��ǰ�Ѵ��ڵĽ����б�
	int GetProcessList(set<string>& setprocess);
	//exe�Ƿ�������
	bool IsProcessRunning(const string& strExePath);
	//��ȡexe��pid�б�
	int GetProcessPID(const string& strExePath,vector<int>& vtid);
	//��ȡ"C:" <==> "\Device\HarddiskVolume1"��Ӧ��ϵ
	int GetDosDevicePair(map<string,string>& maptemp);
	//C:\abc.exe ==> \Device\HarddiskVolume1\abc.exe
	string CommonPathToDosPath(const string& strExe);
	//\Device\HarddiskVolume1\abc.exe ==> C:\abc.exe
	string DosPathToCommonPath(const string& strDospath);
	//����Pid��ȡ������ͨ·�� eg:C:\abc.exe
	string GetProcessPathCommonByPID(unsigned ipid);
	//����Pid��ȡ����Dos·���� eg:\Device\HarddiskVolume1\abc.exe
	string GetProcessPathDosByPID(unsigned ipid);
	//��ȡexe��pid�б������ڴ治�������,szImagePathֻ����Ϊ��ֹ�ڴ���������������ʵ������������
	int GetProcessPIDSaving(const string& strExePath,char* szImagePath,int szlen,vector<int>& vtid);
	//��ȡ���̵�ǰʹ�õ��ڴ��С
	__int64 GetProcessCurrentMemory(int pid);
	//ɱ������
	int KillProcessByPID(int pid);
	//���ݽ���·��ɱ������
	int KillProcessByPath(const string& strExePath);

	string MakeUUID();
	int MakeUUID(string& struuid);
	//�����ڱ���Ψһ��15λ���ȵ�id,���ݵ�ǰ����,��ӵ������,ÿ������������2�ڸ����ظ���id
	//ע:�������޸�ϵͳʱ��,���򽫻ᵼ�����ɵ�id�ظ�
	void make_exclusive_id_rand();//�����ʼ���ڲ��ĵ���index
	int make_exclusive_id_by_time(char* buff);
	void make_exclusive_id_by_time(string& strid);

	bool IsWow64();
	//��������
	int StartServiceM(const string& strServiceName,int nArg,const char** pArg);
	//ֹͣ����
	int StopServiceM(const string& strServiceName);
	//��ȡCPU��Ŀ
	int GetCPUNumbers();
	//ֻ֧��windows
	int WSAStartupWin32();//WSAStartup
	//www.baidu.com ==> 192.168.1.213,��������(ֻ֧��windows����ϵͳ)
	int NetToIP(const char* strNewAddr,char* strip,int ilen);
	//��ȡ����ip�б�
	int GetIPListLocal(vector<string>& vt);
	//����ip�Ͷ˿ں��Ƿ����bind,�Բ��Է������Ƿ����ʹ�øö˿���Ϊ�����˿�
	int TestIPAndPortBindable(const char* strip,unsigned short port);
	//��ȡ��ǰ�߳�id
	__int64 GetCurrentPId();
	//��ȡ��ǰ�߳�id
	__int64 GetCurrentTId();
	//�߳�sleep΢��
	void Sleep(unsigned int imillSec);
	//��ȡsystem32dir��linux��/etcĿ¼
	string GetSystem32Dir(bool bHaveSlash = true);

	__int64 GetTickCountM();
	//����ֵ����������֮ǰ��ԭֵֵ
	long InterlockedExchangeM(volatile long* i,int iexchange);
	long long InterlockedExchange64M(volatile long long* i,long long iexchange);
	//ԭ������,�ȷ���ԭʼֵ�����Լ�
	long InterlockedIncrementM(volatile long* i,int iadd = 1);
	//ԭ������,�ȷ���ԭʼֵ�����Լ�
	long long InterlockedIncrement64M(volatile long long* i,long long iadd = 1);
	//������ֵ,������֮ǰ����ֵ
	long InterlockedComparedExchangeM(volatile long* i,long iexchange,long lcompvalue);
	//��ȡԭ��ѭ��������id,�̰߳�ȫ
	long GetIncreasementID();
};