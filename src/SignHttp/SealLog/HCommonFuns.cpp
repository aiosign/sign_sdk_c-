#include "HCommonFuns.h"
#include <WinSock2.h>
#ifdef WIN32
#include <Psapi.h>
#include <TlHelp32.h>
#include <WinSvc.h>
#include <direct.h>
#include <IPHlpApi.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#endif
#include <stdio.h>
#include <errno.h>

namespace HCommonFun
{
	MFileNode::MFileNode():t_last_modify(0),t_create(0),i_size(0),b_dir(false)
	{
	}

	void set_def_locale()
	{
		static bool bset = false;
		if(bset)
			return;
		bset = true;
		//给所有的后续的rand一个随机种子
		srand((unsigned int)time(0));
#ifdef WIN32
		std::locale::global(std::locale(""));
#else
		//setlocale(LC_CTYPE, "zh_CN.GB2312");//centos 6.5目前需要设置为gb2312
		setlocale(LC_CTYPE, "zh_CN.UTF-8");//有些操作系统需要用utf-8，
#endif
		
	}

	char* MoveToEnd(char** p)
	{
		char* ptemp = *p;
		if (*p == NULL)
			return NULL;

		while(*ptemp != '\0')
		{
			++ptemp;
		}

		*p = ptemp;
		return ptemp;
	}
	/*************************************************************************
	* Function: convert long-second (since 1970-01-01 00:00:00) to datetime
	*************************************************************************/
	int sec2time1(const time_t& lSecd, int* pYear, int* pMonth, int* pDay,
		int *pHour, int* pMin, int* pSec, int *piWeekDay)
	{
		//"3001-01-01 15:59:59",时间上限点,win32下的最大时间
		if(lSecd > 32535244799)
		{
			if(pYear)  *pYear = 1970;
			if(pMonth) *pMonth = 1;
			if(pDay)   *pDay = 1;
			if(pHour)  *pHour = 0;
			if(pMin)   *pMin = 0;
			if(pSec)   *pSec = 0;
			if(piWeekDay) *piWeekDay = 0;
			return -1;
		}

		struct tm *tm_pointer = localtime(&lSecd);
		if(tm_pointer == NULL)
		{
			if(pYear)  *pYear = 1970;
			if(pMonth) *pMonth = 1;
			if(pDay)   *pDay = 1;
			if(pHour)  *pHour = 0;
			if(pMin)   *pMin = 0;
			if(pSec)   *pSec = 0;
			if(piWeekDay) *piWeekDay = 0;
			return -1;
		}

		if(pYear)  *pYear = tm_pointer->tm_year+1900;
		if(pMonth) *pMonth = tm_pointer->tm_mon+1;
		if(pDay)   *pDay = tm_pointer->tm_mday;
		if(pHour)  *pHour = tm_pointer->tm_hour;
		if(pMin)   *pMin = tm_pointer->tm_min;
		if(pSec)   *pSec = tm_pointer->tm_sec;
		if(piWeekDay)
		{
			*piWeekDay = tm_pointer->tm_wday;
			if(tm_pointer->tm_wday==0) *piWeekDay = 7;
		}

		return 0;
	}

	string int2str(int i,int radix)
	{
		char pstr[16];
		_itoa(i,pstr,radix);
		return pstr;
	}

	string i64tostr(const __int64& i)
	{
		char pstr[24] = {0};
#ifdef WIN32
		_i64toa_s(i,pstr,24,10);
#else
		sprintf_s(pstr,24, "%lld", i);
#endif
		return pstr;
	}

	string doubletostr(const double& dou,int iPrecison)
	{
		char pstr[24];
		switch(iPrecison)
		{
		case 0:sprintf_s(pstr,24,"%.0f",dou);break;
		case 1:sprintf_s(pstr,24,"%.1f",dou);break;
		case 2:sprintf_s(pstr,24,"%.2f",dou);break;
		case 3:sprintf_s(pstr,24,"%.3f",dou);break;
		case 4:sprintf_s(pstr,24,"%.4f",dou);break;
		case 5:sprintf_s(pstr,24,"%.5f",dou);break;
		case 6:sprintf_s(pstr,24,"%.6f",dou);break;
		case 7:sprintf_s(pstr,24,"%.7f",dou);break;
		case 8:sprintf_s(pstr,24,"%.8f",dou);break;
		case 9:sprintf_s(pstr,24,"%.9f",dou);break;
		default:sprintf_s(pstr,24,"%f",dou);break;
		}
		
		return pstr;
	}

	int time2sec(int iYear, int iMonth, int iDay,int iHour, int iMin, int iSecd,time_t& pLSecd)
	{
		if(iYear < 1900)
		{
			pLSecd = 0;
			return -100;
		}
		struct tm struTm;
		struTm.tm_isdst= 0;
		struTm.tm_year = iYear-1900;
		struTm.tm_mon  = iMonth-1;
		struTm.tm_mday = iDay;
		struTm.tm_hour = iHour;
		struTm.tm_min  = iMin;
		struTm.tm_sec  = iSecd;

		struTm.tm_yday = -1;

		pLSecd = mktime(&struTm);
		return 0;
	}

	time_t string2sec(const string& strtime)
	{
		if (strtime.length() != 14)
			return 0;

		char ptemp[8];
		struct tm stuTm;
		stuTm.tm_isdst= 0;stuTm.tm_yday = -1;
		
		memcpy(ptemp,strtime.c_str(),4);ptemp[4] = 0;stuTm.tm_year = atoi(ptemp) - 1900;
		memcpy(ptemp,strtime.c_str() + 4,2);ptemp[2] = 0;stuTm.tm_mon = atoi(ptemp) - 1;
		memcpy(ptemp,strtime.c_str() + 6,2);stuTm.tm_mday = atoi(ptemp);
		memcpy(ptemp,strtime.c_str() + 8,2);stuTm.tm_hour = atoi(ptemp);
		memcpy(ptemp,strtime.c_str() + 10,2);stuTm.tm_min = atoi(ptemp);
		memcpy(ptemp,strtime.c_str() + 12,2);stuTm.tm_sec = atoi(ptemp);

		time_t lSecd(mktime(&stuTm));
		return lSecd;
	}

	string sec2string(const time_t& _lSecd)
	{
		time_t lSecd(_lSecd);
		if (lSecd == 0)
			lSecd = time(0);

		char ptemp[16];
		strftime(ptemp,16,"%Y%m%d%H%M%S",localtime(&lSecd));
		//int iYear,iMonth,iDay,iHour,iMin,iSec;
		//secd2time1(lSecd,&iYear,&iMonth,&iDay,&iHour,&iMin,&iSec,NULL);
		//sprintf_s(strtemp,16, "%d%.2d%.2d%.2d%.2d%.2d", iYear, iMonth, iDay, iHour, iMin, iSec);
		return ptemp;
	}

	// time_t => '2015-10-10 16:50:13'
	string sec2strDateTime(const time_t& _lSecd)
	{
		time_t lSecd(_lSecd);
		if (lSecd == 0)
			lSecd = time(0);

		char ptemp[24];
		strftime(ptemp,24,"%Y-%m-%d %H:%M:%S",localtime(&lSecd));
		//int iYear,iMonth,iDay,iHour,iMin,iSec;
		//secd2time1(lSecd,&iYear,&iMonth,&iDay,&iHour,&iMin,&iSec,NULL);
		//sprintf(strtemp,"%d-%.2d-%.2d %.2d:%.2d:%.2d", iYear, iMonth, iDay, iHour, iMin, iSec);
		return ptemp;
	}
	//"2015-10-10 16:50:13'  => time_t
	time_t strDateTime2sec(const string& strtime)
	{
		if (strtime.length() != 19)
			return 0;

		const char* pstr = strtime.c_str();

		struct tm stutime;
		stutime.tm_isdst= 0;
		stutime.tm_year = atoi(pstr) - 1900;
		stutime.tm_mon  = atoi(pstr + 5) - 1;
		stutime.tm_mday = atoi(pstr + 8);
		stutime.tm_hour = atoi(pstr + 11);
		stutime.tm_min  = atoi(pstr + 14);
		stutime.tm_sec  = atoi(pstr + 17);

		time_t lSecd(mktime(&stutime));
		return lSecd;
	}

	string strDateTimeToday()
	{
		time_t t_now;
		time(&t_now);
		int year,month,day;
		struct tm *tm_pointer = localtime(&t_now);
		if(tm_pointer == NULL)
		{
			year = 1970;
			month = 1;
			day = 1;
			return "";
		}
		tm_pointer->tm_hour = 0;
		tm_pointer->tm_min = 0;
		tm_pointer->tm_sec = 0;
		t_now = mktime(tm_pointer);

		char ptemp[24];
		strftime(ptemp,24,"%Y-%m-%d %H:%M:%S",localtime(&t_now));
		//int iYear,iMonth,iDay,iHour,iMin,iSec;
		//secd2time1(lSecd,&iYear,&iMonth,&iDay,&iHour,&iMin,&iSec,NULL);
		//sprintf(strtemp,"%d-%.2d-%.2d %.2d:%.2d:%.2d", iYear, iMonth, iDay, iHour, iMin, iSec);
		return ptemp;
	}

	int char2wchar(const char* str,int ilenin, wchar_t** wstr,int& ilenout,unsigned int ui_code_page)
	{
#ifdef WIN32
		int i_len = MultiByteToWideChar(ui_code_page, 0,  str, -1, 0, 0);

		if (i_len <= 0) 
		{
			return -1;
		}

		bool bNeedNewMem = false;
		if (*wstr == NULL)
		{
			bNeedNewMem = true;
		}
		//缓冲区不够写
		else if (i_len > ilenout + 1)
		{
			return -3;
		}

		if (bNeedNewMem)
		{
			*wstr = new wchar_t[i_len + 1];
		}

		if (*wstr == NULL) 
		{
			return -2;
		}

		ilenout = 0;

		//memset(*wstr,0,1);
		(*wstr)[0] = L'\0';

		int ret = MultiByteToWideChar(ui_code_page, 0,  str, -1, *wstr, i_len);

		if (ret <= 0) 
		{
			delete [] *wstr;
			*wstr = NULL;
			return -3;
		}
		ilenout = i_len;
		(*wstr)[i_len] = L'\0';

		return 0;
#else
		ilenout = 0;
		if (ilenin == 0) 
		{
			*wstr = new wchar_t[1];
			(*wstr)[0] = L'\0';
			return 0;
		}

		int n = mbstowcs(NULL, str, ilenin);

		if(n == -1)
			return -1;

		n += 2;
		wchar_t *dest = new wchar_t[n];
		dest[0] = 0;
		if ( (n = mbstowcs(dest, str, ilenin)) == -1)
		{
			delete dest;
			return -3;
		}
		ilenout = n;
		dest[ilenout] = L'\0';
		*wstr = dest;
		return 0;
#endif
		
	}

	int GetFileContent(const char* strFilePath,char** strContent,int& iContentLen)
	{
		*strContent = NULL;
		iContentLen = 0;
		FILE *fp = fopen(strFilePath, "rb");

		if (fp == NULL)
			return -1;

		fseek(fp,0,SEEK_SET);
		fseek(fp,0,SEEK_END);
		iContentLen = ftell(fp);
		*strContent = new char[iContentLen + 2];
		if (*strContent == NULL)
		{
			fclose(fp);
			return -2;
		}
		char* pbuf = *strContent;
		memset(pbuf,0,iContentLen + 2);
		fseek(fp,0,SEEK_SET);
		//将整个证书的内容读出来
		fread(*strContent,iContentLen,1,fp);
		fclose(fp);
		return 0;
	}

	int GetFileContent1(const char* strFilePath,char* strContent,int& iContentLen)
	{
		FILE *fp = fopen(strFilePath, "rb");

		if (fp == NULL)
		{
			iContentLen = 0;
			return -1;
		}

		fseek(fp,0,SEEK_SET);
		fseek(fp,0,SEEK_END);
		int ilen = ftell(fp);
		if ((strContent == NULL) || (iContentLen == 0))
		{
			iContentLen = 0;
			fclose(fp);
			return -2;
		}

		if (iContentLen < ilen)
		{
			iContentLen = 0;
			fclose(fp);
			return -3;
		}

		fseek(fp,0,SEEK_SET);
		fread(strContent,ilen,1,fp);
		//也可以替换为该函数需要include<io.h>
		//_read(_fileno(fp),strContent,ilen)
		if(iContentLen > ilen) strContent[ilen] = 0;
		iContentLen = ilen;

		fclose(fp);

		return 0;
	}

	int SetFileHide(const char* _str_file, bool b_hide)
	{

#ifdef WIN32
		//文件为空,直接返回
		if(_str_file == NULL) return -10;
		//如果文件不存在就直接返回
		if (!isFileExistA(_str_file)) return -20;

		DWORD dwAttrs = GetFileAttributesA(_str_file); 
		//获取文件属性失败,直接返回
		if (INVALID_FILE_ATTRIBUTES == dwAttrs) return -30;

		if (b_hide) 
		{
			if (dwAttrs & FILE_ATTRIBUTE_HIDDEN) return 0;
			if (SetFileAttributesA(_str_file,dwAttrs | FILE_ATTRIBUTE_HIDDEN))
				return 0;
			return -40;
		}
		else
		{
			//文件已经是非隐藏属性,无需再次设置
			if ((dwAttrs & FILE_ATTRIBUTE_HIDDEN) == 0) return 0;
			if (SetFileAttributesA(_str_file,dwAttrs & (~FILE_ATTRIBUTE_HIDDEN) )) return 0;
			return -50;
		}
#endif
		return 0;
	}

	__int64 GetFileLength(const char* strFilePath)
	{
#ifdef WIN32
		WIN32_FILE_ATTRIBUTE_DATA fad;

		if(GetFileAttributesExA(strFilePath,GetFileExInfoStandard,&fad))
		{

			__int64 i_size = ((__int64)fad.nFileSizeHigh * ((__int64)MAXDWORD+1)) + (__int64)fad.nFileSizeLow; 
			return i_size;

		}

		return -1;
		
#else
		struct   stat   s; 
		int ret = stat(strFilePath, &s);
		if(ret == 0)
		{
			return s.st_size;
		}

		return -1;
#endif
	}

	wstring str2wstring(const string& str,unsigned int ui_code_page)
	{
		wstring wstr;
		wchar_t* ptemp = NULL;
		int ilen = 0;
		if (char2wchar(str.c_str(),(int)(str.length()),&ptemp,ilen,ui_code_page) == 0)
			wstr = ptemp;
		if (ptemp != NULL)
			delete [] ptemp;
		return wstr;
	}

	string wstr2ICEstring(const wstring& str)
	{
		string result;
		result.reserve(str.length() * 2);

		wchar_t wc;
		for(unsigned int i = 0; i < str.length(); ++i)
		{
			wc = str[i];

			if(wc < 0x80)
			{
				result.push_back((char)wc);
			}
			else if(wc < 0x800)
			{
				result += 0xc0 | (wc>>6);
				result += 0x80 | (wc & 0x3f);
			}
			else if(wc < 0x10000)
			{
				result += 0xe0 | (wc>>12);
				result += 0x80 | ((wc>>6) & 0x3f);
				result += 0x80 | (wc & 0x3f);
			}
			else if(wc < 0x10FFFF)
			{
				result += 0xf0 | (wc>>18);
				result += 0x80 | ((wc>>12) & 0x3f);
				result += 0x80 | ((wc>>6) & 0x3f);
				result += 0x80 | (wc & 0x3f);
			}
			else
			{
				return result; // Error, not encodable.
			}
		}

		return result;
	}

	wstring ICE2wstring(const string& str)
	{
		wstring result;
		result.reserve(str.length());

		unsigned int len(0);
		unsigned char c;
		wchar_t wc;
		int minval;
		for(unsigned int i = 0; i < str.length(); i += len)
		{
			c = str[i];
			if(c < 0x80)
			{
				wc = c;
				len = 1;
				minval = 0;
			}
			else if(c < 0xc0) // Lead byte must not be 10xxxxxx
			{
				return result; // Error, not encodable.
			}
			else if(c < 0xe0) // 110xxxxx
			{
				wc = c & 0x1f;
				len = 2;
				minval = 0x80;
			}
			else if(c < 0xf0) // 1110xxxx
			{
				wc = c & 0xf;
				len = 3;
				minval = 0x800;
			}
#if SIZEOF_WCHAR_T >= 4
			else if(c < 0xf8) // 11110xxx
			{
				wc = c & 7;
				len = 4;
				minval = 0x10000;
			}
			else if(c < 0xfc) // 111110xx
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 3;
				len = 5;
				minval = 0x110000;
			}
			else if(c < 0xfe) // 1111110x
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 1;
				len = 6;
				minval = 0x4000000;
			}
#endif
			else
			{
				return result; // Error, not encodable.
			}

			if(i + len - 1 < str.length())
			{
				for(unsigned int j = 1; j < len; ++j)
				{
					if((str[i + j] & 0xc0) != 0x80) // All other bytes must be 10xxxxxx
					{
						return result; // Error, not encodable.
					}

					wc <<= 6;
					wc |= str[i + j] & 0x3f;
				}

				if(wc < minval)
				{
					return result; // Error, non-shortest form.
				}
				else
				{
					result += wc;
				}
			}
			else
			{
				return result; // Error, not encodable.
			}
		}

		return result;
	}

	string str2ICEstring(const string& str)
	{
		 return wstr2ICEstring(str2wstring(str));
	}

	string ICE2string(const string& str)
	{
		return wstr2string(ICE2wstring(str));
	}

	int ICECountCharNum(const string& str)
	{
		unsigned int len(0),_count(0);
		unsigned char c;
		int minval;
		for(unsigned int i = 0; i < str.length(); i += len)
		{
			c = str[i];
			if(c < 0x80)
			{
				++_count;
				len = 1;
				minval = 0;
			}
			else if(c < 0xc0) // Lead byte must not be 10xxxxxx
			{
				return -1; // Error, not encodable.
			}
			else if(c < 0xe0) // 110xxxxx
			{
				++_count;
				len = 2;
				minval = 0x80;
			}
			else if(c < 0xf0) // 1110xxxx
			{
				++_count;
				len = 3;
				minval = 0x800;
			}
#if SIZEOF_WCHAR_T >= 4
			else if(c < 0xf8) // 11110xxx
			{
				++_count;
				len = 4;
				minval = 0x10000;
			}
			else if(c < 0xfc) // 111110xx
			{
				++_count;
				len = 5;
				minval = 0x110000;
			}
			else if(c < 0xfe) // 1111110x
			{
				++_count;
				len = 6;
				minval = 0x4000000;
			}
#endif
			else
			{
				return -1; // Error, not encodable.
			}
		}

		return _count;
	}

	int ICEGetHeadNCharLength0(const string& strICE,int iWish)
	{
		//如果utf8字符串为空,则返回0
		if(strICE.empty() || iWish <= 0) return 0;
		int ireallen(0);
		unsigned int len(0);
		unsigned char c;
		wchar_t wc;
		int minval;
		unsigned int j(0);
		for(unsigned int i = 0; i < strICE.length(); i += len)
		{
			c = strICE[i];
			if(c < 0x80)
			{
				wc = c;
				len = 1;
				minval = 0;
			}
			else if(c < 0xc0) // Lead byte must not be 10xxxxxx
			{
				return ireallen; // Error, not encodable.就把之前的长度返回回去
			}
			else if(c < 0xe0) // 110xxxxx
			{
				wc = c & 0x1f;
				len = 2;
				minval = 0x80;
			}
			else if(c < 0xf0) // 1110xxxx
			{
				wc = c & 0xf;
				len = 3;
				minval = 0x800;
			}
#if SIZEOF_WCHAR_T >= 4
			else if(c < 0xf8) // 11110xxx
			{
				wc = c & 7;
				len = 4;
				minval = 0x10000;
			}
			else if(c < 0xfc) // 111110xx
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 3;
				len = 5;
				minval = 0x110000;
			}
			else if(c < 0xfe) // 1111110x
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 1;
				len = 6;
				minval = 0x4000000;
			}
#endif
			else
			{
				return ireallen; // Error, not encodable.
			}

			if(i + len - 1 < strICE.length())
			{
				for(j = 1; j < len; ++j)
				{
					if((strICE[i + j] & 0xc0) != 0x80) // All other bytes must be 10xxxxxx
						return ireallen; // Error, not encodable.

					wc <<= 6;
					wc |= strICE[i + j] & 0x3f;
				}

				if(wc < minval) return ireallen; // Error, non-shortest form.

				ireallen += len;

				if(--iWish == 0) return ireallen;
			}
			else
				ireallen; // Error, not encodable.
		}
		return ireallen;
	}

	int ICEGetHeadNCharLength1(const char* buff,int iWish,int ibufflen)
	{
		if((buff == NULL) || (iWish <= 0)) return 0;

		if(ibufflen < 0) ibufflen = (int)strlen(buff);
		if(ibufflen == 0) return 0;
		int ireallen(0);
		unsigned int len(0);
		unsigned char c;
		wchar_t wc;
		int minval;
		unsigned int j(0);
		for(unsigned int i = 0; i < ibufflen; i += len)
		{
			c = buff[i];
			if(c < 0x80)
			{
				wc = c;
				len = 1;
				minval = 0;
			}
			else if(c < 0xc0) // Lead byte must not be 10xxxxxx
			{
				return ireallen; // Error, not encodable.就把之前的长度返回回去
			}
			else if(c < 0xe0) // 110xxxxx
			{
				wc = c & 0x1f;
				len = 2;
				minval = 0x80;
			}
			else if(c < 0xf0) // 1110xxxx
			{
				wc = c & 0xf;
				len = 3;
				minval = 0x800;
			}
#if SIZEOF_WCHAR_T >= 4
			else if(c < 0xf8) // 11110xxx
			{
				wc = c & 7;
				len = 4;
				minval = 0x10000;
			}
			else if(c < 0xfc) // 111110xx
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 3;
				len = 5;
				minval = 0x110000;
			}
			else if(c < 0xfe) // 1111110x
			{
				// Length 5 and 6 is declared invalid in Unicode 3.1 and ISO 10646:2003.
				wc = c & 1;
				len = 6;
				minval = 0x4000000;
			}
#endif
			else
			{
				return ireallen; // Error, not encodable.
			}

			if(i + len - 1 < ibufflen)
			{
				for(j = 1; j < len; ++j)
				{
					if((buff[i + j] & 0xc0) != 0x80) // All other bytes must be 10xxxxxx
						return ireallen; // Error, not encodable.

					wc <<= 6;
					wc |= buff[i + j] & 0x3f;
				}

				if(wc < minval) return ireallen; // Error, non-shortest form.

				ireallen += len;

				if(--iWish == 0) return ireallen;
			}
			else
				ireallen; // Error, not encodable.
		}
		return ireallen;
	}

	string wstr2string(const wstring& wstr,unsigned int ui_code_page)
	{
		if(wstr.empty())
			return "";

		string str;
		char* ptemp = NULL;
		int ilenout = 0;
		if (wchar2char(wstr.c_str(),(int)(wstr.length()),&ptemp,ilenout,ui_code_page) == 0)
			str = ptemp;
		if (ptemp != NULL)
			delete [] ptemp;
		return str;
	}

	int wchar2char(const wchar_t* wstr,int ilenin, char** str,int& ilenout,unsigned int ui_code_page)
	{
#ifdef WIN32

		int i_len = WideCharToMultiByte (ui_code_page, WC_COMPOSITECHECK,wstr, -1, 0, 0, NULL, NULL ); 
		if (i_len <= 0) 
		{
			return -1;
		}

		bool bNeedNewMem = false;
		if (*str == NULL)
		{
			bNeedNewMem = true;
		}
		else if (ilenout < i_len)
		{
			return -3;
		}

		if (bNeedNewMem)
		{
			*str = new char [i_len + 2];
		}

		if (*str == NULL) 
		{
			return -5;
		}

		(*str)[0] = '\0';

		int ret = WideCharToMultiByte(ui_code_page, WC_COMPOSITECHECK, wstr, -1, *str, i_len, NULL, NULL ); 

		if (ret <= 0) 
		{
			delete [] *str;
			*str = NULL;
			return -8;
		}

		(*str)[ret] = '\0';

		return 0;
#else
		if ((wstr == NULL) || (ilenin == 0))
		{
			ilenout = 0;
			*str = new char[2];
			(*str)[0] = 0;
			return 0;
		}

		int inum = wcstombs(NULL,wstr,ilenin);
		if (inum == -1)
			return -1;

		if (ilenout <= 0)
		{
			*str = new char[inum + 2];
			if (*str == NULL)
				return -1234920;;
		}
		else if (inum > ilenout + 1)
		{
			return -3;
		}

		inum = wcstombs(*str,wstr,ilenin);
		if (inum == -1)
		{
			delete [] *str;
			return -5;
		}

		ilenout = inum;
		(*str)[ilenout] = '\0';

		return 0;
#endif
	}

	

	bool isFileExistA(const char* strpath,bool& b_dir)
	{
		b_dir = false;
#ifdef WIN32
		DWORD dw = GetFileAttributesA(strpath);

		if (dw == INVALID_FILE_ATTRIBUTES)
		{
			return false;
		}

		if(dw & FILE_ATTRIBUTE_DIRECTORY)
		{
			b_dir = true;
		}
		else
		{
			b_dir = false;
		}

		return true;
#else
		b_dir = false;
		struct   stat s; 
		int ret = stat(strpath, &s);
		if(ret == 0)
		{
			if(S_ISDIR(s.st_mode))
				b_dir = true;
			return true;
		}
		return false;
#endif	
	}

	bool IsDirEmpty(const char* _strDir)
	{
#ifdef WIN32
		WIN32_FIND_DATAA FindFileData;

		memset(&FindFileData, 0, sizeof(FindFileData));
		HANDLE hFind;
		string strDir = _strDir;
		DirFit(strDir,true);
		strDir.push_back('*');

		hFind = FindFirstFileA(strDir.c_str(),  &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE) return true;

		BOOL b = true;
		while (b) 
		{
			if ((memcmp(FindFileData.cFileName,".",1) != 0) 
				&& (memcmp(FindFileData.cFileName,"..",2) != 0))
			{
				FindClose(hFind);
				return false;
			}
			b = FindNextFileA(hFind, &FindFileData);
		}

		FindClose(hFind);

		return true;
#else
		DIR *dirp;
		struct dirent *direntp;

		if((dirp = opendir(_strDir)) == NULL) return true;

		while((direntp = readdir(dirp)) != NULL)
		{
			if(strcmp(direntp->d_name,".") != 0 &&
				strcmp(direntp->d_name,"..") != 0)   
			{
				closedir(dirp);
				return false;
			}
		}     

		closedir(dirp);
		return true;
#endif
	}

	bool isFileExistA(const char* strpath)
	{
#ifdef WIN32
		DWORD dw = GetFileAttributesA(strpath);

		if (dw == INVALID_FILE_ATTRIBUTES)
			return false;
		return true;
#else
		struct   stat s; 
		int ret = stat(strpath, &s);
		if(ret == 0)
			return true;
		return false;
#endif
	}
#ifdef WIN32
	bool isFileExistW(const wchar_t* strpath)
	{
		DWORD dw = GetFileAttributesW(strpath);

		if (dw == INVALID_FILE_ATTRIBUTES)
		{
			return false;
		}

		return true;
	}
#endif
#ifdef WIN32
	bool isFileExistW(const wchar_t* strpath,bool& b_dir)
	{
		b_dir = false;
		DWORD dw = GetFileAttributesW(strpath);

		if (dw == INVALID_FILE_ATTRIBUTES)
			return false;

		if(dw & FILE_ATTRIBUTE_DIRECTORY)
			b_dir = true;
		else
			b_dir = false;

		return true;
	}
#endif

	int WriteFileCovery(const char* strFile,const char* strContent,int iLen)
	{
#ifdef WIN32
		bool bhide(false);
		DWORD dwAttrs = GetFileAttributesA(strFile); 
		//获取文件属性失败,直接返回
		if ((INVALID_FILE_ATTRIBUTES != dwAttrs) && (dwAttrs & FILE_ATTRIBUTE_HIDDEN))
		{
			bhide = true;
			SetFileAttributesA(strFile,dwAttrs & (~FILE_ATTRIBUTE_HIDDEN));
		}
#endif
		FILE *pfile = fopen(strFile, "wb");
		size_t i_ret(1);
		do
		{
			if(pfile == NULL) break;
			if(iLen > 0) i_ret = fwrite(strContent, 1, iLen, pfile);
		} while (false);

		if(pfile != NULL) fclose(pfile);
#ifdef WIN32
		if(bhide) SetFileAttributesA(strFile,dwAttrs & (~FILE_ATTRIBUTE_HIDDEN));
#endif
		return ((i_ret > 0)?0:-2);
	}

	int WriteFileAfter(const char* strFile,const char* strContent,int iLen)
	{
#ifdef WIN32
		bool bhide(false);
		DWORD dwAttrs = GetFileAttributesA(strFile); 
		//获取文件属性失败,直接返回
		if ((INVALID_FILE_ATTRIBUTES != dwAttrs) && (dwAttrs & FILE_ATTRIBUTE_HIDDEN))
		{
			bhide = true;
			SetFileAttributesA(strFile,dwAttrs & (~FILE_ATTRIBUTE_HIDDEN));
		}
#endif
		FILE *pfile = fopen(strFile, "ab");
		size_t i_ret(1);
		do
		{
			if(pfile == NULL) break;
			if(iLen > 0) i_ret = fwrite(strContent, 1, iLen, pfile);
		} while (false);

		if(pfile != NULL) fclose(pfile);
#ifdef WIN32
		if(bhide) SetFileAttributesA(strFile,dwAttrs & (~FILE_ATTRIBUTE_HIDDEN));
#endif
		return ((i_ret > 0)?0:-2);
	}

	int WriteFileAfterNoClose(FILE*& pfile,const char* strFile,const char* strContent,int iLen)
	{
		if(pfile == NULL) pfile = fopen(strFile, "ab");

		if (pfile == NULL)
		{
			return -1239;
		}

		size_t i_ret = fwrite(strContent, 1, iLen, pfile);
		//fclose(pfile);

		return ((i_ret > 0)?0:-2);
	}

	int WriteFileAfterCloseFILE(FILE*& pfile)
	{
		if(pfile != NULL)
		{
			fflush(pfile);
			fclose(pfile);
			pfile = NULL;
			return 0;
		}
		return -100;
	}

	bool IsTextUTF8(const char* pstr)
	{
		size_t i,length = strlen(pstr);
		int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		unsigned char chr;
		bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
		for(i = 0 ;i < length ; i++)
		{
			chr = *(pstr + i);
			if((chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
				bAllAscii = false;
			if(nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if(chr >= 0x80)
				{
					if((chr >= 0xFC) && (chr <= 0xFD))
						nBytes = 6;
					else if(chr >= 0xF8)
						nBytes = 5;
					else if(chr >= 0xF0)
						nBytes = 4;
					else if(chr >= 0xE0)
						nBytes = 3;
					else if(chr >= 0xC0)
						nBytes = 2;
					else
						return false;
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if((chr&0xC0) != 0x80 )
					return false;
				nBytes--;
			}
		}
		if( nBytes > 0 ) //违返规则
			return false;

		if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
			return false;
		return true;
	}
#ifdef WIN32
	void Utf8ToAnsi(const UCHAR * lpsrc,const int srclen, LPSTR lpdst, int& dstlen)
	{
		WCHAR * Unicode;
		int len = MultiByteToWideChar ( CP_UTF8 , 0 ,(char*) lpsrc ,-1 ,NULL,0);
		Unicode = new WCHAR[len * sizeof(WCHAR)];
		MultiByteToWideChar ( CP_UTF8 , 0 ,( char * ) lpsrc, -1, Unicode , len );
		len = WideCharToMultiByte(CP_ACP,0,Unicode,-1,NULL,0,NULL,NULL);
		dstlen = WideCharToMultiByte (CP_ACP,0,Unicode,-1,lpdst,len,NULL,NULL);
		delete [] Unicode;
	}
#endif

	void StrCut(char* strin,int ilen,int& ilenout,const char* strtoCut)
	{
		ilenout = ilen;
		if ((strin == NULL) || (strtoCut == NULL)) return;

		int icutlen = (int)strlen(strtoCut);
		//can't be cut
		if(icutlen > ilen) return;
		char* buff = strin,*pnow(strin);
		bool bexist(false);
		for (int i = 0;i < ilen + 1;)
		{
			char* p = buff + i;
			if((icutlen + i < ilen + 1) && (memcmp(buff + i,strtoCut,icutlen) == 0))
			{
				i += icutlen;
				bexist = true;
				continue;
			}
			if(!bexist) ++pnow;
			else *pnow++ = *(buff + i);
			++i;
		}
		if(bexist)
		{
			ilenout = (int)(pnow - strin);
		}
	}

	char* trim(char *buff, char ch)
	{
		if (buff == NULL)
		{
			return buff;
		}
		bool bhavechar(false);
		char *ptemp = buff;
		//先预扫一遍，如果没有该字符就直接返回，如果有再往下操作
		for (;*ptemp != 0; ptemp++ )
		{
			if (*ptemp == ch)
			{
				bhavechar = true;
				break;
			}
		}

		if(!bhavechar) return buff;

		char *ptemp1 = buff;
		for (ptemp = buff; *buff != 0; buff++)
		{
			if(*buff == ch) continue;
			*ptemp++ = *buff;
		}
		*ptemp = 0;
		return ptemp1;
	}

	string& trim(string& str, char ch)
	{
		if (str.empty())
		{
			return str;
		}
		bool bhavechar(false);
		unsigned i;
		//先预扫一遍，如果没有该字符就直接返回，如果有再往下操作
		for (i = 0;i < str.length(); i++ )
		{
			if (str[i] == ch)
			{
				bhavechar = true;
				break;
			}
		}

		if(!bhavechar) return str;

		unsigned j(0);
		for (i = 0; i < str.length(); i++)
		{
			if(str[i] == ch) continue;
			str[j++] = str[i];
		}
		str.resize(j);
		return str;
	}

	string& trim(string& str, const string& strlist)
	{
		if (str.empty() || strlist.empty())
		{
			return str;
		}
		bool bhavechar(false);
		unsigned i,k;
		//先预扫一遍，如果没有该字符就直接返回，如果有再往下操作
		for (i = 0;i < str.length(); i++ )
		{
			for (k = 0; k < strlist.length(); k++)
			{
				if (str[i] == strlist[k])
				{
					bhavechar = true;
					break;
				}
			}

			if(bhavechar) break;
		}

		if(!bhavechar) return str;

		unsigned j(0);
		for (i = 0;i < str.length(); i++ )
		{
			bhavechar = false;
			for (k = 0; k < strlist.length(); k++)
			{
				if (str[i] == strlist[k])
				{
					bhavechar = true;
					break;
				}
			}

			if(bhavechar) continue;
			str[j++] = str[i];
		}
		str.resize(j);
		return str;
	}

	char* trim_begin(char *buff,const char* strlist)
	{
		if(buff == NULL || strlist == NULL || strlen(strlist) == 0) return NULL;
		int ilen = (int)strlen(buff),i;
		const char* plist = strlist;
		bool bfind(false);
		for (i = 0;i < ilen; ++i)
		{
			bfind = false;
			for (plist = strlist; *plist != 0;++plist)
			{
				if(buff[i] == *plist){bfind = true;break;}
			}

			if(!bfind)
			{
				if(i != 0)
				{
					memmove(buff,buff + i,ilen - i);
					buff[ilen - i] = 0;
				}
				break;
			}
		}
		if(bfind) buff[0] = 0;
		return buff;
	}

	string& trim_begin(string& str,const char* strlist)
	{
		if(str.empty() || strlist == NULL || strlen(strlist) == 0) return str;
		size_t ilen = str.length(),i;
		const char* plist = strlist;
		bool bfind(false);
		for (i = 0;i < ilen; ++i)
		{
			bfind = false;
			for (plist = strlist; *plist != 0;++plist)
			{
				if(str[i] == *plist){bfind = true;break;}
			}

			if(!bfind)
			{
				if(i != 0)
				{
					for (int j = 0; j < ilen - i; ++j)
					{
						str[j] = str[i+j];
					}
					str.resize(ilen - i);
				}

				return str;
			}
		}

		if(bfind) str.clear();

		return str;
	}

	char* trim_end(char *buff,const char* strlist)
	{
		if(buff == NULL || strlist == NULL || strlen(strlist) == 0) return buff;
		int ilen = (int)(strlen(buff)) - 1;
		const char* plist = strlist;
		bool bfind(false);
		for (;ilen >= 0 ; --ilen)
		{
			bfind = false;
			for (plist = strlist; *plist != 0;++plist)
			{
				if(buff[ilen] == *plist){buff[ilen] = 0;bfind = true;break;}
			}

			if(!bfind) return buff;
		}

		return buff;
	}

	string& trim_end(string& str,const char* strlist)
	{
		if(str.empty() || strlist == NULL || strlen(strlist) == 0) return str;

		int ilen = (int)str.length() - 1;
		const char* plist = strlist;
		bool bfind(false);
		for (;ilen >= 0 ; --ilen)
		{
			bfind = false;
			for (plist = strlist; *plist != 0;++plist)
			{
				if(str[ilen] == *plist){str[ilen] = 0;bfind = true;break;}
			}

			if(!bfind)
			{
				if(str.length() != ilen + 1) str.resize(ilen + 1);
				return str;
			}
		}

		if(bfind) str.clear();

		return str;
	}

	char* trim_both(char *buff,const char* strlist)
	{
		if(buff == NULL || strlist == NULL) return buff;
		trim_end(buff,strlist);
		trim_begin(buff,strlist);
		return buff;
	}

	string& trim_both(string& str,const char* strlist)
	{
		if(str.empty() || strlist == NULL) return str;
		trim_end(str,strlist);
		trim_begin(str,strlist);
		return str;
	}

	char *FindEndBias(char* pstring)
	{
		int len = (int)strlen(pstring);
		int i;
		if(len==0) return NULL;

		for(i=len-1; i>=0; i--)
		{
			if(pstring[i]=='/'||pstring[i]=='\\') return pstring+i;
		}
		return NULL;
	}

	int MakeDir(const string& _str_dir)
	{
		return MakeDir1(_str_dir.c_str(),_str_dir.length());
	}

	int make_dir_real(const char* path)
	{
#ifdef WIN32
		if(_mkdir(path)<0)
		{
			if(GetLastError() != ERROR_ALREADY_EXISTS) return -500;
		}
#else
		if(mkdir(path,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)<0) return -600;
#endif
		return 0;
	}

	int MakeDir1(const char* strDir,int idirlen)
	{
		if(strDir == NULL) return -100;
		if(idirlen <= 0 && ((idirlen = strlen(strDir)) == 0)) return -200;
		if(idirlen >= MAX_PATH*4) return -300;

		bool bdir(false);
		bool bexist = HCommonFun::isFileExistA(strDir,bdir);

		if (bdir) return 0;//如果目录已经存在，则直接返回，不用创建了
		if (bexist) return -1;//如果文件已经存在，而且不是文件夹，下面就不用创建了，肯定会失败

		char path[MAX_PATH*4];
		memcpy(path,strDir,idirlen);path[idirlen] = 0;
		while (path[idirlen - 1] == '\\' || path[idirlen - 1] == '/') --idirlen;

		char c;
		int i(0),ilen(0),icount(0),isizenow(16);
		while (path[i] == '/' || path[i] == '\\') ++i;
		for (; i < idirlen; ++i)
		{
			if(path[i] != '/' && path[i] != '\\') continue;

			c = path[i];path[i] = '\0';

			if((bexist = HCommonFun::isFileExistA(path,bdir)) && (!bdir)) return -400;

			if(bexist) {path[i] = c;continue;}

			if(make_dir_real(path) != 0){return -500;}

			path[i] = c;
		}

		if((bexist = HCommonFun::isFileExistA(path,bdir)) && (!bdir)){return -600;}

		if(!bexist && make_dir_real(path) != 0){return -700;}
		return 0;
	}

	bool is_ipv4_format(const string& str)
	{
		if(str.size() < 7 || str.size() > 15) return false;

		unsigned int ipointcount(0),inumcount(0);
		const char* pbegin = str.c_str(),*p;
		for (p = pbegin; p != pbegin + str.length(); ++p)
		{
			if(*p == '.')//如果是点号
			{
				++ipointcount;
				if(inumcount == 0) return false;//如果当前是点号,而且之前数字个数为0,要么开头即为点号,要么两个点号之间无数字
				if(ipointcount > 3) return false;//大于3个点号则有误
				inumcount = 0;//重置数字计数
				continue;
			}
			if(*p < '0' || *p > '9' ){return false;}
			++inumcount;
			if(inumcount > 3) return false;
		}

		if(inumcount == 0) return false;
		if(ipointcount != 3) return false;//点号不为3,则格式不正确

		return true;
	}

	string GetMyExeFullPath(const HMODULE& hmode)
	{
		char buf[600] = {0};
#ifdef WIN32
		DWORD dwRet = ::GetModuleFileNameA(hmode, buf, 599);

		if (dwRet == 0)
			return "";

		return buf;
#else
		int ret = -1;
		char sLine[1024] = { 0 };
		//void* pSymbol = (void*)"";

#ifdef __x86_64
		void* pSymbol = (void*)0x0400001;
#else
		void* pSymbol = (void*)0x08048001;
#endif

		FILE *fp;
		char *pPath;

		fp = fopen ("/proc/self/maps", "r");
		if ( fp != NULL )
		{
			while (!feof (fp))
			{
				unsigned long start, end;

				if ( !fgets (sLine, sizeof (sLine), fp))
					continue;

				//printf("[%s]\n", sLine);

				if ( !strstr (sLine, " r-xp ") || !strchr (sLine, '/'))
					continue;

				sscanf (sLine, "%lx-%lx ", &start, &end);

				//printf("Start:%lx, Symbol:%lx, End:%lx\n", start, pSymbol, end);

				if (pSymbol >= (void *) start && pSymbol < (void *) end)
				{
					//printf("Enter com\n");

					char *tmp;
					size_t len;

					/* Extract the filename; it is always an absolute path */
					pPath = strchr (sLine, '/');

					/* Get rid of the newline */
					tmp = strrchr (pPath, '\n');
					if (tmp) *tmp = 0;

					ret = 0;
					strncpy( buf,600, pPath );
					//printf("File:[%s]\n", sFileName);
					break;
				}
			}
			fclose (fp);
		}
		if(ret != 0)
			return "";

		return buf;
#endif
	}
#ifdef WIN32
	wstring GetMyExeFullPathW(const HMODULE& hmode)
	{		
		wchar_t buf[260] = {0};
		DWORD dwRet = ::GetModuleFileNameW(hmode, buf, 259);

		if (dwRet == 0)
			return L"";

		return buf;
	}
#endif

	string GetParentPath(const string& strFile,bool bHaveSlash)
	{
		if (strFile.size() < 4)
			return "";

		string strout = strFile;

		int ilen = (int)strout.length() - 2;
		for (; ilen >= 0 ; ilen--)
		{
			if ((strout[ilen] == '\\') || (strout[ilen] == '/'))
			{
				if (bHaveSlash)
					strout.resize(ilen + 1);
				else
					strout.resize(ilen);
				break;
			}
		}

		//表明没有找到斜杠
		if (strout.length() == strFile.length())
			return "";

		return strout;
	}

	bool Spilt_to_Path_and_Short(const string& _strFile,string& str_path,string& str_name,bool bHaveSlash)
	{
		if (_strFile.size() < 4)
			return false;

		if(!str_path.empty()) str_path.clear();
		if(!str_name.empty()) str_name.clear();

		//string strFile = _strFile;
		int i((int)_strFile.length() - 1);
		//把末尾的斜杠去掉
		for (; i >= 0 ; i--)
		{
			if ((_strFile[i] == '\\') || (_strFile[i] == '/')) continue;
			break;
		}

		switch(i)
		{
		case -1:return false;
		case 0:str_name.push_back(_strFile[0]);return true;
		default:break;
		}

		for (int i_from(i); i_from >= 0; i_from--)
		{
			if (_strFile[i_from] == '\\' || _strFile[i_from] == '/')
			{
				++i_from;
				if(bHaveSlash) str_path.append(_strFile.c_str(),i_from);
				else
					str_path.append(_strFile.c_str(),i_from - 1);

				str_name.append(_strFile.c_str() + i_from,_strFile.length() - i_from);
				return true;
			}
		}

		str_name.append(_strFile);
		return true;
	}

	string GetShortName(const string& strFile,bool bHaveSuffix)
	{
		if (strFile.size() < 4)
			return "";

		string strout = strFile;
		int ilen = (int)strout.length() - 1;
		bool bEndBySlash = false;
		//把末尾的斜杠全去掉
		for (;ilen >= 0; ilen--)
		{
			if ((strout[ilen] != '\\') && (strout[ilen] != '/'))
			{
				if (bEndBySlash)
					strout.resize(ilen + 1);
				break;
			}
			bEndBySlash = true;
		}

		for (; ilen >= 0 ; ilen--)
		{
			if ((strout[ilen] == '\\') || (strout[ilen] == '/'))
			{
				strout = strout.substr(ilen + 1,strout.length());
				break;
			}

			if (strout[ilen] == ':')
			{
				return "";
			}
		}

		if(bHaveSuffix || strout.empty())
			return strout;

		for (ilen = (unsigned)strout.length() - 1; ilen >= 0 ; ilen--)
		{
			if (strout[ilen] == '.')
			{
				strout.resize(ilen);
				break;
			}
		}
		return strout;
	}

	string GetNoSuffix(const string& strFile)
	{
		string strsuffix = GetFileSuffix(strFile);
		if (strsuffix.empty())
		{
			return strFile;
		}

		return strFile.substr(0,strFile.length() - strsuffix.length() - 1);
	}

	wstring GetNoSuffixW(const wstring& strFile)
	{
		wstring strsuffix = GetFileSuffixW(strFile);
		if (strsuffix.empty())
		{
			return strFile;
		}

		return strFile.substr(0,strFile.length() - strsuffix.length() - 1);
	}

	string& replace_ex(string& strin,const string& str_old, const string& str_new,bool b_case_sense)
	{
		if (str_old.empty())
			return strin;

		if (strin.empty())
			return strin;

		if(is_equal_str(str_old.c_str(),str_new.c_str(),b_case_sense))
			return strin;


		size_t iPos = 0;
		if (b_case_sense)
		{
			while(1)
			{
				iPos = strin.find(str_old, iPos);

				if (iPos == string::npos)
				{
					break;
				}

				strin.replace(iPos, str_old.length(), str_new.c_str());

				iPos += str_new.length();
			}

			return strin;
		}
		else
		{
			string strin_ = strin;
			str2Lower(strin_);
			string strold_ = str_old;
			str2Lower(strold_);

			while(1)
			{
				iPos = strin_.find(strold_, iPos);

				if (iPos == string::npos)
				{
					break;
				}

				strin.replace(iPos, str_old.length(), str_new.c_str());
				strin_ = strin;
				str2Lower(strin_);

				iPos += str_new.length();
			}

			return strin;
		}
	}

	void DirFit(string& strdir,bool bHaveslash)
	{
		if (strdir.empty())
		{
			if(bHaveslash)
#ifdef WIN32
				strdir.push_back('\\');
#else
				strdir.push_back('/');
#endif
			return;
		}

		for (int ilen((int)strdir.length() - 1);ilen >= 0;--ilen)
		{
			switch(strdir[ilen])
			{
			case '\\':
			case '/':
				if (bHaveslash) return;
				strdir.resize(strdir.length() - 1);
				break;
			default:
				if (!bHaveslash) return;//if no need slash can return
				//else if need slash,append a slash
#ifdef WIN32
				strdir.push_back('\\');
#else
				strdir.push_back('/');
#endif
				return;
			}
		}
	}

	void FixPathSlash(string& strPath)
	{
		for (int i = 0 ; i < strPath.length() ;++i)
		{
#ifdef WIN32
			if (strPath[i] == '/') strPath[i] = '\\';
#else
			if (strPath[i] == '\\') strPath[i] = '/';
#endif
		}
	}

	void FixPathLinuxSlash(string& strPath)
	{
		for (int i = 0 ; i < strPath.length() ;++i)
		{
			if (strPath[i] == '\\')
				strPath[i] = '/';
		}
	}

	string GetFileSuffix(const string& strFile)
	{
		if (strFile.empty())
			return "";

		string strret;
		for(int i = (int)strFile.length() - 1 ; i >= 0 ; i--)
		{
			if (strFile[i] == '.')
			{
				strret = strFile.substr(i + 1,strFile.length());
				return strret;
			}

			if (strFile[i] == '\\' || strFile[i] == '/')//遇到斜线了，还是没找到suffix，就直接返回
			{
				return "";
			}
		}

		return strret;
	}

	wstring GetFileSuffixW(const wstring& strFile)
	{
		if (strFile.empty())
			return L"";

		wstring strret;
		for(int i = (int)strFile.length() - 1 ; i >= 0 ; i--)
		{
			if (strFile[i] == '.')
			{
				strret = strFile.substr(i + 1,strFile.length());
				return strret;
			}

			if (strFile[i] == '\\' || strFile[i] == '/')//遇到斜线了，还是没找到suffix，就直接返回
			{
				return L"";
			}
		}

		return strret;
	}

	int Get_vt_Str_By_Sep(const string& strin,vector<string>& vt_str,const char& c)
	{
		if(!vt_str.empty())vt_str.clear();

		if (strin.empty())
		{
			return -1;
		}

		int ret = 0;

		int offset = 0;
		int find_ret;
		string strTmp;
		while(1)
		{
			find_ret = -1;

			int ret_c = (int)strin.find(c, offset);
			if(ret_c >= 0 && (find_ret == -1 || ret_c < find_ret))
			{
				find_ret = ret_c;
			}

			if(find_ret >= 0)
			{
				strTmp =  strin.substr(offset, find_ret - offset);
				vt_str.push_back(strTmp);
				offset = find_ret + 1;
			}
			else
			{
				strTmp = strin.substr(offset, strin.length() - offset);
				if(strTmp.length() > 0  || vt_str.size() == 0)
					vt_str.push_back(strTmp);
				break;
			}

		}

		return ret;
	}

	int Get_vt_WStr_By_Sep(const wstring& strin,vector<wstring>& vt_str,const wchar_t& wc)
	{
		if(!vt_str.empty())vt_str.clear();

		if (strin.empty())
		{
			return -1;
		}

		int ret = 0;

		int offset = 0;
		int find_ret;
		wstring strTmp;
		while(1)
		{
			find_ret = -1;

			int ret_c = (int)strin.find(wc, offset);
			if(ret_c >= 0 && (find_ret == -1 || ret_c < find_ret))
			{
				find_ret = ret_c;
			}

			if(find_ret >= 0)
			{
				strTmp =  strin.substr(offset, find_ret - offset);
				vt_str.push_back(strTmp);
				offset = find_ret + 1;
			}
			else
			{
				strTmp = strin.substr(offset, strin.length() - offset);
				if(strTmp.length() > 0  || vt_str.size() == 0)
					vt_str.push_back(strTmp);
				break;
			}

		}

		return ret;
	}

	int Get_set_Str_By_Sep(const string& strin,set<string>& set_str,const char& c)
	{
		if(!set_str.empty()) set_str.clear();

		if (strin.empty())
		{
			return -1;
		}

		int ret = 0;

		int offset = 0;
		int find_ret;
		string strTmp;
		size_t ret_c;
		while(1)
		{
			find_ret = -1;

			ret_c = strin.find(c, offset);
			if(ret_c >= 0 && (find_ret == -1 || ret_c < find_ret))
			{
				find_ret = ret_c;
			}

			if(find_ret >= 0)
			{
				strTmp = strin.substr(offset, find_ret - offset);
				set_str.insert(strTmp);
				offset = find_ret + 1;
			}
			else
			{
				strTmp = strin.substr(offset, strin.length() - offset);
				if(strTmp.length() > 0  || set_str.empty())
					set_str.insert(strTmp);
				break;
			}

		}

		return ret;
	}

	bool is_equal_str(const char* s1,const char* s2,bool bCaseSense)
	{
		return is_equal_str(s1,0,s2,0,bCaseSense);
	}

	bool is_equal_str(const string& str1,const string& str2,bool bCaseSense)
	{
		if(str1.length() != str2.length()) return false;

		if(!bCaseSense)
#ifdef WIN32
			return (_strnicmp(str1.c_str(),str2.c_str(),str1.length()) == 0);
#else
			return (strncasecmp(str1.c_str(),str2.c_str(),str1.length()) == 0);
#endif
		return (memcmp(str1.c_str(),str2.c_str(),str1.length()) == 0);
	}

	bool is_equal_str(const char* s1,int ilen1,const char* s2,int ilen2,bool bCaseSense)
	{
		if(s1 == s2) return true;

		if ((s1 == NULL) || (s2 == NULL))
			return false;

		if(ilen1 == 0) ilen1 = (int)strlen(s1);
		if(ilen2 == 0) ilen2 = (int)strlen(s2);
		if(ilen1 != ilen2) return false;

		if(!bCaseSense)
#ifdef WIN32
			return (_strnicmp(s1,s2,ilen1) == 0);
#else
			return (strncasecmp(s1,s2,ilen1) == 0);
#endif
		return (memcmp(s1,s2,ilen1) == 0);
	}

	//转换为大写
	char* str2Upper(char* psString)
	{
		if(NULL == psString)
			return NULL;

		size_t ilen = strlen(psString);
		for(size_t i = 0 ; i < ilen ; i++)
		{
			if((psString[i] >= 'a') && (psString[i] <= 'z'))
				psString[i] -= 32;
		}

		return psString;
	}

	string& str2Upper(string& psString)
	{
		if(psString.empty())
			return psString;

		unsigned int ilen = (unsigned)(psString.length());
		for(unsigned int i = 0 ; i < ilen ; i++)
		{
			if((psString[i] >= 'a') && (psString[i] <= 'z'))
				psString[i] -= 32;
		}

		return psString;
	}

	//转换为大写
	char* str2Lower(char* buff)
	{
		if(buff == NULL) return NULL;

		size_t ilen = strlen(buff);
		if(ilen == 0) return buff;

		for (size_t i = 0 ; i < ilen ; ++i)
		{
			if((buff[i] >= 'A') && (buff[i] <= 'Z'))
				buff[i] += 32;
		}

		return buff;
	}

	//转换为大写
	string& str2Lower(string& psString)
	{
		if(psString.empty())
			return psString;

		unsigned int ilen = (unsigned)psString.length();
		for(unsigned int i = 0 ; i < ilen ; i++)
		{
			if((psString[i] >= 'A') && (psString[i] <= 'Z'))
				psString[i] += 32;
		}

		return psString;
	}

	void String2Upper(const char* psSrc, char*& psDest)
	{
		if( NULL == psSrc)
			return;

		int ilensrc = (int)strlen(psSrc);
		if(psDest == NULL)
		{
			psDest = new char [ilensrc + 2];
#ifdef WIN32
			ZeroMemory(psDest,ilensrc + 2);
#else
			memset(psDest,0,ilensrc + 2);
#endif
			
		}

		memcpy(psDest,psSrc,ilensrc);

		str2Upper(psDest);

		return;
	}

#ifdef WIN32
	// 设置 everyone 权限：
	void set_sa_everyone_priv(SECURITY_DESCRIPTOR& sd, SECURITY_ATTRIBUTES& sa)
	{

		BOOL b = InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
		b = SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = FALSE;
		sa.lpSecurityDescriptor = &sd;

	}

	bool TakeOwner(const string& str_name)
	{

		bool b_found = false;

		string strMutexName;


#ifndef H_OS_VER_WIN9X

		strMutexName = "Global\\" + str_name;
#else
		strMutexName = str_name;
#endif
		SECURITY_DESCRIPTOR sd;
		SECURITY_ATTRIBUTES sa;
		set_sa_everyone_priv(sd, sa);

		HANDLE hMutexOneInstantance = CreateMutexA(&sa, TRUE, strMutexName.c_str());

		DWORD dw = GetLastError();
		if(dw == ERROR_ACCESS_DENIED)
			b_found = true;

		if(dw == ERROR_ALREADY_EXISTS)
			b_found = true;

		if(hMutexOneInstantance)
			ReleaseMutex(hMutexOneInstantance);

		if (b_found)
			CloseHandle(hMutexOneInstantance);

		return !b_found;

	}

	time_t make_by(const FILETIME& ft)
	{
		FILETIME localTime;
		if (!FileTimeToLocalFileTime(&ft, &localTime))
		{		
			return 0;
		}

		SYSTEMTIME sys_time;
		if(FileTimeToSystemTime(&localTime, &sys_time))
		{

			struct tm atm;
			atm.tm_sec = sys_time.wSecond;
			atm.tm_min = sys_time.wMinute;
			atm.tm_hour = sys_time.wHour;
			atm.tm_mday = sys_time.wDay;
			atm.tm_mon = sys_time.wMonth - 1;// tm_mon is 0 based
			atm.tm_year = sys_time.wYear - 1900; // tm_year is 1900 based
			atm.tm_isdst = -1;
			time_t t_ret = mktime(&atm);
			if (t_ret == -1) 
			{
				t_ret = 0;
			}
			return t_ret;
		}

		return 0;
	}

	int make_by(FILETIME& ft,const time_t& t_in)
	{
		memset(&ft,0,sizeof(FILETIME));
		tm* t_temp = localtime(&t_in);

		SYSTEMTIME sys_time;
		memset(&sys_time,0,sizeof(SYSTEMTIME));

		sys_time.wSecond = t_temp->tm_sec;
		sys_time.wMinute = t_temp->tm_min;
		sys_time.wHour = t_temp->tm_hour;
		sys_time.wDay = t_temp->tm_mday;
		sys_time.wMonth = t_temp->tm_mon + 1;// tm_mon is 0 based
		sys_time.wYear = t_temp->tm_year + 1900; // tm_year is 1900 based

		FILETIME flocalTime;
		if(!SystemTimeToFileTime(&sys_time,&flocalTime))
		{
			return -100;
		}

		if(!LocalFileTimeToFileTime(&flocalTime,&ft))
		{
			return -200;
		}

		return 0;
	}
#endif
	
#ifndef WIN32
	int snap(const string& _strPath,vector<MFileNode>& vtout,MFileNode& node,bool bRecurFind)
	{
		DIR *dir = opendir(_strPath.c_str());

		if(dir != NULL)
		{
			struct dirent *direntp; 

			while((direntp = readdir(dir) ) != NULL) 
			{
				bool b_contin = false;

				string strfile = direntp->d_name;

				if ((strfile == ".") || (strfile == "..") )
					continue;

				string strFullFile = _strPath + "/" + strfile;
				//如果获取文件属性失败，就枚举下一个
				if (GetFileAttribute(strFullFile,node) != 0)
					continue;
				//先把这个压倒队列
				vtout.push_back(node);

				if (node.b_dir && bRecurFind)
				{
					//如果是文件夹，就递归调用
					snap(strFullFile,vtout,node,true);
				}
			}

			closedir(dir);
		}

		return 0;
	}
#endif

	int DirSnap(const string& _strPath,vector<MFileNode>& vtout,bool bRecurFind)
	{
		if(!HCommonFun::isFileExistA(_strPath.c_str())) return -100;
#ifdef WIN32
		WIN32_FIND_DATAA data;

		string strDir = _strPath;
		FixPathSlash(strDir);

		if (strDir[strDir.length() - 1] != '\\')
		{
			strDir.push_back('\\');
		}

		string strpath = strDir;
		if (strDir[strDir.length() - 1] != '*')
		{
			strDir.push_back('*');
		}

		HANDLE hfind = FindFirstFileA(strDir.c_str(),&data);

		if (hfind == INVALID_HANDLE_VALUE)
			return -1;

		MFileNode node;

		do
		{
			if ((memcmp(data.cFileName,".",1) == 0) || (memcmp(data.cFileName,"..",2) == 0))
			{
				continue;
			}

			node.strFileFullName = strpath + data.cFileName;
			node.strFileLastName = data.cFileName;
			node.b_dir = ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
			node.t_last_modify = make_by(data.ftLastWriteTime);
			node.t_create = make_by(data.ftCreationTime);

			if (!node.b_dir)
			{
				node.i_size = ((__int64)data.nFileSizeHigh * ((__int64)MAXDWORD+1)) + (__int64)data.nFileSizeLow; 
			}
			else
			{
				if (bRecurFind)
				{
					DirSnap(node.strFileFullName,vtout,true);
				}
			}

			vtout.push_back(node);
		}
		while (FindNextFileA(hfind,&data));

		FindClose(hfind);
		return 0;
#else
		MFileNode node;
		if (GetFileAttribute(_strPath,node) != 0)
			return -1;
		if(!node.b_dir)
		{
			vtout.push_back(node);
			return 0;
		}

		return snap(_strPath,vtout,node,bRecurFind);
#endif
		
	}

	int GetFileAttribute(const string& strFilename,MFileNode& node)
	{
		if(strFilename.empty())
			return -1;
#ifdef WIN32
		WIN32_FIND_DATAA data;
		HANDLE hfind = INVALID_HANDLE_VALUE;
		//对于文件夹，要将斜线去掉才可以findfile，否则find不到
		if((strFilename[strFilename.length() - 1] == '\\') || (strFilename[strFilename.length() - 1] == '/'))
			hfind = FindFirstFileA(strFilename.substr(0,strFilename.length() - 1).c_str(),&data);
		else
			hfind = FindFirstFileA(strFilename.c_str(),&data);

		if (hfind == INVALID_HANDLE_VALUE)
			return -3;

		node.strFileFullName = strFilename;
		node.strFileLastName = data.cFileName;
		node.b_dir = ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		node.t_last_modify = make_by(data.ftLastWriteTime);
		node.t_create = make_by(data.ftCreationTime);

		if (!node.b_dir)
		{
			node.i_size = ((__int64)data.nFileSizeHigh * ((__int64)MAXDWORD+1)) + (__int64)data.nFileSizeLow; 
		}

		FindClose(hfind);

		return 0;
#else
		struct stat s; 
		int ret = stat(strFilename.c_str(), &s);
		if(ret == 0)
		{
			node.strFileFullName = strFilename;
			node.strFileLastName = GetShortName(strFilename);
			node.t_last_modify = s.st_mtime;
			node.t_create = s.st_mtime;
			node.b_dir = S_ISDIR(s.st_mode);
			if(!node.b_dir)
				node.i_size = s.st_size;
			else
				node.i_size = 0;
			return 0;
		}

		return -1;
#endif
		
	}

	int SetFileTimeM(const string& strFilename,const time_t& t_modify,const time_t& t_create,const time_t& t_access)
	{
#ifdef WIN32
		FILETIME ft_create,ft_modify,ft_access,* pcreate(NULL),*pmodify(NULL),*paccess(NULL);;
		if(t_create > 0 && (make_by(ft_create,t_create) != 0)){pcreate = &ft_create; return -10;}
		if(t_modify > 0 && (make_by(ft_modify,t_modify) != 0)){pmodify = &ft_modify; return -20;}
		if(t_access > 0 && (make_by(ft_access,t_access) != 0)){paccess = &ft_access; return -30;}

		HANDLE handle = CreateFileA(strFilename.c_str(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(handle == INVALID_HANDLE_VALUE) return -40;

		if(SetFileTime(handle,pcreate,paccess,pmodify))
		{
			CloseHandle(handle);
			return 0;
		}

		CloseHandle(handle);
		return -50;
#endif
		return 0;
	}

	//str后面追加内部strin会被delete,再new出来
	void StrApend(char** strin,int& ilen,const char* strApend)
	{
		if(strApend == NULL)
			return;

		int iLenApend = (int)strlen(strApend);
		if (iLenApend == 0)
			return;

		char* ptemp = new char[ilen + iLenApend + 10];
		memcpy(ptemp,*strin,ilen);
		memcpy(ptemp + ilen,strApend,iLenApend);
		delete [] *strin;
		ilen += iLenApend;
		*strin = new char [ilen + 10];
		memcpy(*strin,ptemp,ilen);
		(*strin)[ilen] = '\0';
		delete [] ptemp;
	}
	//str前面追加
	void StrPushFront(char** strin,int& ilen,const char* strHead)
	{
		if(strHead == NULL)
			return;

		int iLenApend = (int)strlen(strHead);
		if (iLenApend == 0)
			return;

		char* ptemp = new char[ilen + iLenApend + 10];
		memcpy(ptemp,strHead,iLenApend);
		memcpy(ptemp + iLenApend,*strin,ilen);
		delete [] *strin;
		ilen += iLenApend;
		*strin = new char [ilen + 10];
		memcpy(*strin,ptemp,ilen);
		(*strin)[ilen] = '\0';
		delete [] ptemp;
	}

	bool movefile(const string& strFrom,const string& strTo,bool bFailifExist)
	{
		if(!bFailifExist)
		{
			if(!DeleteFileH(strTo)) return false;
		}

		if(renamefile(strFrom.c_str(),strTo.c_str()) == 0)
			return true;
		return false;
	}

	int renamefile(const char* psFileOld, const char* psFileNew)
	{
		return rename(psFileOld,psFileNew);
	}

	int CopyFileToDir(const string& _strFrom, const string& _strTo)
	{
#ifdef WIN32
		bool bfromdir,btodir;
		if(!HCommonFun::isFileExistA(_strFrom.c_str(),bfromdir)) return -100;

		if(HCommonFun::isFileExistA(_strTo.c_str(),btodir))
		{
			do
			{
				if(bfromdir)
				{
					if(btodir)//from is dir and to is dir
					{
						break;
					}
					else//from is dir ,but to isn't a dir
					{
						return -200;
					}
				}
				else
				{
					if(btodir)//from isn't dir but to is dir
					{
						string strTo = _strTo;
						HCommonFun::DirFit(strTo,true);
						strTo.append(HCommonFun::GetShortName(_strFrom));
						if(::CopyFileA(_strFrom.c_str(),strTo.c_str(),FALSE)) return 0;
						return -300;
					}
					else//from isn't dir and to isn't a dir
					{
						if(::CopyFileA(_strFrom.c_str(),_strTo.c_str(),FALSE)) return 0;
						return -400;
					}
				}

			} while (false);
		}

		string strFromRoot(_strFrom),strToRoot(_strTo),strTemp,strParent;
		HCommonFun::DirFit(strFromRoot,true);HCommonFun::DirFit(strToRoot,true);
		vector<HCommonFun::MFileNode> vt;
		HCommonFun::DirSnap(strFromRoot,vt);
		for (size_t i = 0 ; i < vt.size(); ++i)
		{
			const HCommonFun::MFileNode& node = vt[i];
			strTemp = strToRoot;
			strTemp.append(node.strFileFullName.c_str() + strFromRoot.length(),node.strFileFullName.length() - strFromRoot.length());
			strParent = HCommonFun::GetParentPath(strTemp);
			if(node.b_dir)
			{
				if(HCommonFun::MakeDir(strTemp) != 0)
				{
					return -500;
				}
				continue;
			}

			if(HCommonFun::MakeDir(strParent) != 0)
			{
				return -600;
			}
			if(!::CopyFileA(node.strFileFullName.c_str(),strTemp.c_str(),FALSE))
			{
				return -700;
			}
		}

		return 0;
#else
		return -30000;
#endif
	}

	bool is_begin_by(const char* s1,const char* s2,int isize,bool bCaseSense)
	{
		if(s1 == s2)
			return true;

		if ((s1 == NULL) || (s2 == NULL))
			return false;

		int ilen1 = (int)strlen(s1);
		int ilen2 = (int)strlen(s2);
		//如果任意一个小于ilen，那前ilen个肯定不相等
		if((ilen1 < isize) || (ilen2 < isize))
			return false;

		if(!bCaseSense)
#ifdef WIN32
			return (_strnicmp(s1,s2,isize) == 0);
#else
			return (strncasecmp(s1,s2,isize) == 0);
#endif
		return (memcmp(s1,s2,isize) == 0);
	}

	bool is_begin_by(const string& strin,const string& strBegin,bool bCaseCense)
	{
		if (strBegin.empty()) return true;

		if (strBegin.length() > strin.length())	return false;

		if(!bCaseCense)
#ifdef WIN32
			return (_strnicmp(strin.c_str(),strBegin.c_str(),strBegin.length()) == 0);
#else
			return (strncasecmp(strin.c_str(),strBegin.c_str(),strBegin.length()) == 0);
#endif
		return (memcmp(strin.c_str(),strBegin.c_str(),strBegin.length()) == 0);
	}

	bool is_end_by(const string& strin,const string& strend,bool bCaseCense)
	{
		if (strend.empty())
			return true;

		if (strend.length() > strin.length())
			return false;

		int ilenin = (int)strin.length();
		int ilenend = (int)strend.length();

		for(int i = 1; i <= ilenend; i++)
		{
			if (bCaseCense)
			{
				if (strin[ilenin - i] != strend[ilenend - i])//如果不相等，就表示不是以这个结尾的
					return false;
			}
			else
			{
				char c1 = strin[ilenin - i];
				char c2 = strend[ilenend - i];

				if (c1 >= L'A' && c1 <= L'Z')
					c1 += 32;

				if (c2 >= L'A' && c2 <= L'Z')
					c2 += 32;

				if (c1 != c2)//如果不相等，就表示不是以这个结尾的
					return false;
			}
		}

		return true;//前面都没返回，表示比较到最后一个，都是相等的，就返回true
	}

	const char* find(const char * pin,const char * pstrfind,int ilenin,bool bcasecense)
	{
		if(pin == NULL) return NULL;

		if(pstrfind == NULL) return pin;
		//如果传入的ilenpin为0，就需要计算一下pin的长度
		if (ilenin < 0) ilenin = (int)strlen(pin);

		int ilentofind = (int)strlen(pstrfind);
		//如果目标字符串长度比将要查找的还要短，那就肯定查不到
		if (ilenin < ilentofind) return NULL;
		if(bcasecense)
		{
			//不断的循环知道能找得到
			for (const char* p(pin),*pend(pin + ilenin - ilentofind + 1); p != pend; p++)
			{
				if(memcmp(p,pstrfind,ilentofind) == 0) return p;
			}
			return NULL;
		}
		else
		{
			for (const char* p(pin),*pend(pin + ilenin - ilentofind + 1); p != pend; p++)
			{
#ifdef WIN32
				if (_strnicmp(p,pstrfind,ilentofind) == 0) return p;
#else
				if(strncasecmp(p,pstrfind,ilentofind) == 0) return p;
#endif
			}
			return NULL;

		}
	}

	const char* rfind(const char * pin,const char * pstrfind,int ilenpin,bool bcasecense)
	{
		if(pin == NULL)
			return NULL;

		if(pstrfind == NULL)
			return pin;

		if(ilenpin < 0) ilenpin = strlen(pin);
		//如果没传入长度
		int ilenfind = (int)strlen(pstrfind);
		if (ilenpin < ilenfind)//如果目标字符串长度比将要查找的还要短，那就肯定查不到
			return NULL;

		if(bcasecense)
		{
			//不断的循环知道能找得到
			for (const char* p = pin + ilenpin - ilenfind; p + 1 != pin; --p)
			{
				if (memcmp(p,pstrfind,ilenfind) == 0)
					return p;
			}
		}
		else
		{
			for (const char* p = pin + ilenpin - ilenfind; p + 1 != pin; --p)
			{
#ifdef WIN32
				if (_strnicmp(p,pstrfind,ilenfind) == 0)
					return p;
#else
				if(strncasecmp(p,pstrfind,ilenfind) == 0)
					return p;
#endif
			}
		}
		return NULL;
	}

	bool DeleteFileH(const string& _str_file)
	{

#ifdef WIN32
		if (!::DeleteFileA(_str_file.c_str()))
		{
			//如果文件不存在就不用删除了
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
				return true;
			return false;
		}

		return true;
#else
		int ret = remove(_str_file.c_str());
		if(ret == 0)
			return true;
		//如果文件不存在，就返回true
		if(errno == ENOENT)
			return true;
		return false;
#endif
	}

	bool DeleteFileH1(const char* _str_file)
	{

#ifdef WIN32
		if (!::DeleteFileA(_str_file))
		{
			//如果文件不存在就不用删除了
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
				return true;
			return false;
		}

		return true;
#else
		int ret = remove(_str_file);
		if(ret == 0)
			return true;
		//如果文件不存在，就返回true
		if(errno == ENOENT)
			return true;
		return false;
#endif
	}

	int DeleteDirRecur(const string& strDir)
	{
		vector<MFileNode>vt;
		DirSnap(strDir,vt,true);
		int iFailedCount = 0;
		for (unsigned int i = 0 ; i < vt.size() ;i++)
		{
			if (vt[i].b_dir)
			{
				//只能删除空文件夹
				if (_rmdir(vt[i].strFileFullName.c_str()) != 0)
					iFailedCount++;
			}
			else
			{
				if (!DeleteFileH(vt[i].strFileFullName.c_str()))
					iFailedCount++;
			}
		}

		if (iFailedCount == 0)
		{
			if (_rmdir(strDir.c_str()) != 0)
				iFailedCount++;
		}
		else//可能是因为文件夹不空，所以再删除一遍文件夹
		{
			for (unsigned int i = 0 ; i < vt.size() ;i++)
			{
				if (vt[i].b_dir)
				{
					//只能删除空文件夹
					_rmdir(vt[i].strFileFullName.c_str());
				}
			}
		}

		return iFailedCount;
	}

	int CreateFileWithNoContent(const string& _str_file, __int64 i_len)
	{
		if (i_len <= 0)
		{
			return -1;
		}
#ifdef WIN32
		

		HANDLE hFile = CreateFileA(_str_file.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

		int ret_value = -1;

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return -2;
		}

		LARGE_INTEGER offset,real_offset;

		offset.QuadPart = i_len;

		if (SetFilePointerEx(hFile,offset,&real_offset,FILE_BEGIN))
			if (SetEndOfFile(hFile))
				ret_value = 0;//文件创建成功
			else//设置文件尾失败
				ret_value = -3;
		else//设置偏移失败
			ret_value = -4;

		CloseHandle(hFile);
		return ret_value;
#else

		FILE* fp = fopen(_str_file.c_str(), "wb");

		if (fp == 0)
			return -3;

		fclose(fp);

		int ret = truncate(_str_file.c_str(), i_len);

		if (ret == 0)
			return 0;

		return -7;
#endif
		
	}
	//
#ifdef WIN32
	int ReadFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& _ifrom,int _iwishByte,char*& pbuff,int& iRealByte)
	{
		iRealByte = 0;
		if (_str_file.empty())
			return -1;
		//直接返回成功,但是未读取到任何数据
		if(_iwishByte == 0)
		{
			if (hFile != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
			return 0;
		}

		if (hFile == INVALID_HANDLE_VALUE)
		{
			hFile = ::CreateFileA(_str_file.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				return -4;
		}

		LARGE_INTEGER li_FileSize;
		if (!::GetFileSizeEx(hFile, &li_FileSize)) 
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -5;
		}

		__int64 ifrom = _ifrom;
		if (ifrom < 0) ifrom = 0;

		if (li_FileSize.QuadPart == 0)
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			if(ifrom != 0)
				return -6;
			return 0;
		}

		if (li_FileSize.QuadPart < ifrom)
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -7;
		}

		int iwishByte(_iwishByte);
		if(iwishByte < 0) iwishByte = (int)li_FileSize.QuadPart;
		if(iwishByte > li_FileSize.QuadPart - ifrom) iwishByte = li_FileSize.QuadPart - ifrom;

		LARGE_INTEGER i_pointer;
		i_pointer.QuadPart = ifrom;
		if (!::SetFilePointerEx(hFile, i_pointer, NULL, FILE_BEGIN))
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -8;
		}

		if (pbuff == NULL)
		{
			pbuff = new_charM(iwishByte);
			if(pbuff == NULL)
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
				return -9;
			}
		}

		DWORD dwread = 0;
		if (!::ReadFile(hFile,pbuff,iwishByte,&dwread,0))
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -10;
		}

		iRealByte = (int)dwread;

		return 0;
	}

	int WriteFileFromTo(const string& _str_file,HANDLE& hFile,const __int64& ifrom,int iwishByte,const char* pbuff,int& iRealByte)
	{
		iRealByte = 0;
		if (_str_file.empty())
			return -1;

		if(iwishByte <= 0)
			return -2;

		if (ifrom < 0)
			return -3;

		if (hFile == INVALID_HANDLE_VALUE)
		{
			//写的时候禁止任何共享
			hFile = ::CreateFileA(_str_file.c_str(),GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				return -4;
		}

		LARGE_INTEGER li_FileSize;
		if (!::GetFileSizeEx(hFile, &li_FileSize)) 
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -5;
		}

		if (li_FileSize.QuadPart < ifrom)
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -6;
		}

		LARGE_INTEGER i_pointer;
		i_pointer.QuadPart = ifrom;
		if (!::SetFilePointerEx(hFile, i_pointer, NULL, FILE_BEGIN))
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -7;
		}

		DWORD dwWrite = 0;
		if (!::WriteFile(hFile,pbuff,iwishByte,&dwWrite,0))
		{
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return -8;
		}

		//OVERLAPPED over;
		//ZeroMemory(&over,sizeof(over));
		//if(!GetOverlappedResult(hFile, NULL, &dwWrite, TRUE))//等到写完再返回
		//{
		//	CloseHandle(hFile);
		//	hFile = INVALID_HANDLE_VALUE;
		//	return -9;
		//}

		iRealByte = (int)dwWrite;

		return 0;
	}
#endif
	/*demo:
	FILE* fpread = NULL,*fpwrite = NULL;
	string strfile = "d:\\share\\Share.zip",strfile2 = "d:\\share\\abc123.txt";
	char* buff = new char[16384];
	int iwish = 16384,ireal = 0,irealWrite;
	__int64 ifrom = 0;
	do 
	{
		int iret = HCommonFun::ReadFileFromTo1(fpread,strfile.c_str(),ifrom,iwish,buff,ireal);
		if(ireal > 0)
			HCommonFun::WriteFileFromTo1(fpwrite,strfile2.c_str(),ifrom,ireal,buff,irealWrite);
		ifrom += iwish;
	} while (ireal > 0);
	if(fpread != NULL) fclose(fpread);
	if(fpwrite != NULL)fclose(fpwrite);
	*/

	int ReadFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,char* pbuff,int& iRealByte)
	{
		iRealByte = 0;
		if(fp == NULL)
		{
			fp = fopen(filename,"rb");
			if(fp == NULL)
				return -20;
		}
#ifdef WIN32
		int iret = fsetpos(fp,&ifrom);
#else
		int iret = fseeko(fp,ifrom,SEEK_SET);
#endif
		if(iret != 0)
		{
			fclose(fp);fp = NULL;
			return -30;
		}

		iRealByte = (int)fread(pbuff,1,iwishByte,fp);
		return 0;
	}

	int GetFileLength1(FILE* fp,__int64& ilen)
	{
		ilen = 0;
		if(fp == NULL)
			return -10;

#ifdef WIN32
		fpos_t pos;
		fgetpos(fp,&pos);
		_fseeki64(fp,SEEK_SET, SEEK_END);//将文件指针移动文件结尾
		ilen = _ftelli64(fp); //求出当前文件指针距离文件开始的字节数
		//回复文件句柄之前的位置
		fsetpos(fp,&pos);
#else
		fpos64_t pos;
		fgetpos64(fp,&pos);
		fseeko64(fp,SEEK_SET,SEEK_END);
		ilen = ftello64(fp);
		fsetpos64(fp,&pos);
#endif
		return 0;
	}

	int WriteFileFromTo1(FILE*& fp,const char* filename,const __int64& ifrom,unsigned int iwishByte,const char* pbuff,int& iRealByte)
	{
		if(fp == NULL)
		{
			fp = fopen(filename,"wb");
			if(fp == NULL)
				return -20;
		}
#ifdef WIN32
		int iret = fsetpos(fp,&ifrom);
#else
		int iret = fseeko(fp,ifrom,SEEK_SET);
#endif
		if(iret != 0)
		{
			fclose(fp);fp = NULL;
			return -30;
		}

		iRealByte = (int)fwrite(pbuff,1,iwishByte,fp);
		return 0;
	}

	int cmd_to_argv(const string& str_exe_cmd, char* argv[])
	{	
		argv[0] = 0;

		// abc
		// abc x y z
		// "abc xyz zzz" "1 2 3" 456
		// "abc xyz zzz" '1 2 3' "456"
		// abc          xyz
		// abc "" xy

		vector<string> vt_cmd;

		string str_one;
		bool b_in_quo = false;
		for (int i = 0; i < str_exe_cmd.length(); i++)
		{
			const char& c = str_exe_cmd[i];

			if (c == '"'  || c == '\'' )
			{

				if (b_in_quo)
				{
					vt_cmd.push_back(str_one);
					str_one.clear();
					b_in_quo = false;
					continue;
				}

				b_in_quo = true;
				continue;
			}

			if (c == ' ')
			{
				if (b_in_quo)
				{
					str_one.push_back(c);
					continue;
				}
				else
				{
					if (!str_one.empty())
					{
						vt_cmd.push_back(str_one);
						str_one.clear();
						b_in_quo = false;
						continue;
					}

					continue;
				}
			}

			str_one.push_back(c);
		}

		if (!str_one.empty())
		{
			vt_cmd.push_back(str_one);
		}

		for (int i = 0; i < vt_cmd.size(); i++)
		{
			string& str = vt_cmd[i];
			char* pstr = new char[str.length() + 10];
			memcpy(pstr, str.c_str(),str.length());
			pstr[str.length()] = '\0';
			argv[i] = pstr;
		}

		argv[vt_cmd.size()] = 0;

		return 0;
	}

	int NewProcess(const string& strCmd,int iShowMode,int& iResultCode,bool bWait)
	{
		iResultCode = -123456;
#ifdef WIN32
		STARTUPINFOW si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = iShowMode;
		ZeroMemory( &pi, sizeof(pi) );
		wstring wstrcmd = HCommonFun::str2wstring(strCmd);
		wchar_t* p_exe = new wchar_t[wstrcmd.length() + 1];
		p_exe[wstrcmd.length()] = L'\0';
		wcscpy(p_exe, wstrcmd.c_str());

		BOOL b_create = FALSE;

		b_create = CreateProcessW( NULL, // No module name (use command line). 
			p_exe, // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,
			&si,              // Pointer to STARTUPINFO structure.
			&pi );// Pointer to PROCESS_INFORMATION structure

		if(!b_create)
		{
			DWORD dw = GetLastError();
			delete[] p_exe;
			iResultCode = -111111;
			return -1;
		}

		if (bWait)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD dw_exit_code = 0;
			if(!GetExitCodeProcess(pi.hProcess, &dw_exit_code))
			{
				dw_exit_code = -12345;
			}

			iResultCode = dw_exit_code;
		}
		else
			iResultCode = -654321;

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		delete[] p_exe;
		return 0;
#else
		pid_t pid;
		int fd[2];

		if (!bWait)//不需要等待进程返回时，将fork两次，创建ppid为1的孙进程
		{
			if(pipe(fd)<0)//创建管道失败
			{
				iResultCode = -123457;
				return iResultCode;
			}
		}

		pid = fork();
		if( pid == 0 )		
		{
			// child:
			if (!bWait)
			{
				pid = fork();

				if (pid != 0)//把孙进程的PID通过管道传输给父进程
				{
					close(fd[0]);
					string str_pid_s = HCommonFun::int2str(pid);
					write(fd[1],str_pid_s.c_str(),str_pid_s.size());
					exit(50);//子进程退出		
				}
			}

			char* argv[100];
			cmd_to_argv(strCmd, argv);

			for (int i = 0; i < getdtablesize(); i++)  
			{  
				if (i != STDIN_FILENO && i != STDOUT_FILENO && i != STDERR_FILENO)  
					close(i);
			}  


			int ret = execv(argv[0], argv);
			exit(30);
		}
		else if( pid == -1 )
		{
			iResultCode = -1;
			return iResultCode;
		}

		// parent:	
		if (!bWait)
		{
			close(fd[1]);
			char pipe_buf[64] = {0};
			read(fd[0],pipe_buf,64);
			close(fd[0]);
			//string str_pid = pipe_buf;
			int pid_pipe = atoi(pipe_buf);

			if (pid_pipe != -1)
			{
				int status = 0;
				while(waitpid(pid, &status, WNOHANG) == 0)//回收子进程资源
				{
					sleep(1);
				}
				pid  = pid_pipe;//从管道中读取孙进程的PID成功
			}
		}

		//pro_param.iPID = pid;
		//if (pro_param.timeout_seconds < 0)
		if(!bWait)
		{

			iResultCode = 0;
			return 0;
		}

		int i_loop = 360000000;

		for(int i = 0; i < i_loop; i++)
		{

			int status = 0;
			int ret = waitpid(pid, &status, WNOHANG);

			if(ret > 0)
			{
				bool b_exit_normal = WIFEXITED(status);

				int i_exit_code = WEXITSTATUS(status);

				iResultCode = i_exit_code;
				return 0;

			}

			if(ret != 0)
			{
				iResultCode = -123458;
				return 0;
			}

			sleep(1);

		}

		return 0;
#endif
		
	}

	int GetFileModTime(const string& _str_file,time_t& t_modify)
	{
#ifdef WIN32
		WIN32_FILE_ATTRIBUTE_DATA fad;

		if(GetFileAttributesExA(_str_file.c_str(),GetFileExInfoStandard,&fad))
		{
			t_modify = make_by(fad.ftLastWriteTime);
			return 0;
		}

		return -1;
#else
		struct   stat   s; 
		int ret = stat(_str_file.c_str(), &s);
		if(ret == 0)
		{
			t_modify = s.st_mtim.tv_sec;
			return 0;
		}

		return -1;
		
#endif
		
	}
#ifdef WIN32
	//驱动器和盘符的一一对应关系
	int GetDosDevicePair(map<string,string>& maptemp)
	{
		maptemp.clear();
		char* szDriveStr = new char[512];
		if (szDriveStr == NULL)
			return -1;

		char			szDrive[4];
		char			szDevName[100];
		DWORD				i;

		//获取本地磁盘字符串
		DWORD dw = GetLogicalDriveStringsA(512, szDriveStr);
		if(dw > 0)
		{
			for(i = 0; i < dw; i += 4)
			{
				if((_strnicmp(&(szDriveStr[i]), "A:\\",3) == 0) || (_strnicmp(&(szDriveStr[i]), "B:\\",3) == 0))
					continue;

				szDrive[0] = szDriveStr[i];
				szDrive[1] = szDriveStr[i + 1];
				szDrive[2] = '\0';
				if(!QueryDosDeviceA(szDrive,szDevName, 100))//查询 Dos 设备名
				{
					delete [] szDriveStr;
					return -4;
				}
				maptemp[szDrive] = szDevName;
			}
		}

		delete [] szDriveStr;

		return 0;
	}

	string CommonPathToDosPath(const string& strpath)
	{
		if (strpath.length() < 3)
			return "";

		map<string,string> maptemp;
		int i_ret = GetDosDevicePair(maptemp);
		if (i_ret != 0)
			return "";

		char ch = strpath[0];
		if (ch >= 'a' && ch <= 'z')
			ch -= 32;

		for (map<string,string>::const_iterator ite = maptemp.begin() ; ite != maptemp.end() ;ite++)
		{
			//const string& = ite
			if (ch == ite->first[0])
			{
				string strret = ite->second + strpath.substr(2,strpath.length());
				return strret;
			}
		}

		return "";
	}

	string DosPathToCommonPath(const string& strDospath)
	{
		if (strDospath.length() < 3)
			return "";

		map<string,string> maptemp;
		int i_ret = GetDosDevicePair(maptemp);
		if (i_ret != 0)
			return "";

		for (map<string,string>::const_iterator ite = maptemp.begin() ; ite != maptemp.end() ;ite++)
		{
			//const string& = ite
			if(!HCommonFun::is_begin_by(strDospath,ite->second)) continue;
			string strret = ite->first + strDospath.substr(ite->second.length(),strDospath.length());
			return strret;
		}

		return "";
	}

	string GetProcessPathCommonByPID(unsigned ipid)
	{
		string strRet;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, ipid);
		if(!hProcess) return strRet;

		char* szImagePath = new char [512];
		if (szImagePath == NULL) {CloseHandle(hProcess);return strRet;}

		if(GetProcessImageFileNameA(hProcess, szImagePath, 512))
			strRet = DosPathToCommonPath(szImagePath);

		delete [] szImagePath;
		CloseHandle(hProcess);
		return strRet;
	}

	string GetProcessPathDosByPID(unsigned ipid)
	{
		string strRet;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, ipid);
		if(!hProcess) return strRet;

		char* szImagePath = new char [512];
		if (szImagePath == NULL) {CloseHandle(hProcess);return strRet;}

		if(GetProcessImageFileNameA(hProcess, szImagePath, 512))
			strRet = szImagePath;

		delete [] szImagePath;
		CloseHandle(hProcess);
		return strRet;
	}

	int GetProcessList(set<string>& setout)
	{
		setout.clear();
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return -1;
		}

		char* szImagePath = new char [512];
		if (szImagePath == NULL)
		{
			CloseHandle(hSnapshot);
			return -6;
		}

		PROCESSENTRY32W pe ={0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32FirstW(hSnapshot, &pe))
		{
			CloseHandle(hSnapshot);
			delete [] szImagePath;
			return -8;
		}

		map<string,string>maptemp;
		if(GetDosDevicePair(maptemp) != 0)
		{
			CloseHandle(hSnapshot);
			delete [] szImagePath;
			return -10;
		}

		HANDLE hProcess = NULL;
		size_t ilen = 0;
		do
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pe.th32ProcessID);
			if(!hProcess)
				continue;

			if(!GetProcessImageFileNameA(hProcess, szImagePath, 512))
			{
				CloseHandle(hProcess);
				continue;
			}

			CloseHandle(hProcess);
			ilen = strlen(szImagePath);

			for (map<string,string>::const_iterator ite = maptemp.begin() ; ite != maptemp.end() ; ite++)
			{
				if (ilen < ite->second.length())
					continue;

				if (_strnicmp(ite->second.c_str(),szImagePath,ite->second.length()) != 0)
					continue;

				ilen = ite->second.length() - ite->first.length();
				memcpy(szImagePath + ilen,ite->first.c_str(),ite->first.length());

				setout.insert(szImagePath + ilen);
			}
		} while (Process32NextW(hSnapshot, &pe));

		CloseHandle(hSnapshot);
		delete [] szImagePath;
		return 0;
	}

	bool IsProcessRunning(const string& strExePath)
	{
		set<string>set1;
		GetProcessList(set1);

		for (set<string>::const_iterator ite = set1.begin() ; ite != set1.end() ; ite++)
		{
			if (strExePath.length() != ite->length())continue;

			if (_strnicmp(strExePath.c_str(),ite->c_str(),strExePath.length()) == 0) return true;
		}

		return false;
	}

	int GetProcessPID(const string& _strExePath,vector<int>& vtid)
	{
		vtid.clear();
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
			return -1;

		char* szImagePath = new char [512];
		if (szImagePath == NULL)
		{
			CloseHandle(hSnapshot);
			return -6;
		}

		PROCESSENTRY32W pe ={0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32FirstW(hSnapshot, &pe))
		{
			CloseHandle(hSnapshot);
			delete [] szImagePath;
			return -8;
		}

		string strExePath = CommonPathToDosPath(_strExePath);

		if(strExePath.empty())
		{
			CloseHandle(hSnapshot);
			delete [] szImagePath;
			return -10;
		}

		HANDLE hProcess = NULL;
		unsigned int ilen = 0,itemp = 0;
		do
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pe.th32ProcessID);
			if(!hProcess)
				continue;

			if(!GetProcessImageFileNameA(hProcess, szImagePath, 512))
			{
				CloseHandle(hProcess);
				continue;
			}

			CloseHandle(hProcess);

			if (strlen(szImagePath) != strExePath.length())
				continue;

			if (_strnicmp(strExePath.c_str(),szImagePath,strExePath.length()) != 0)
				continue;

			vtid.push_back(pe.th32ProcessID);

		} while (Process32NextW(hSnapshot, &pe));

		CloseHandle(hSnapshot);
		delete [] szImagePath;
		return 0;
	}

	int GetProcessPIDSaving(const string& strExePath,char* szImagePath,int szlen,vector<int>& vtid)
	{
		vtid.clear();

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
			return -3;

		if ((szImagePath == NULL) || (szlen < 512))
		{
			CloseHandle(hSnapshot);
			return -6;
		}

		PROCESSENTRY32W pe ={0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32FirstW(hSnapshot, &pe))
		{
			CloseHandle(hSnapshot);
			return -8;
		}

		HANDLE hProcess = NULL;
		size_t ilen = 0,itemp = 0;
		do
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pe.th32ProcessID);
			if(!hProcess)
				continue;

			if(!GetProcessImageFileNameA(hProcess, szImagePath, 512))
			{
				CloseHandle(hProcess);
				continue;
			}

			CloseHandle(hProcess);

			if (strlen(szImagePath) != strExePath.length())
				continue;

			if (_strnicmp(strExePath.c_str(),szImagePath,strExePath.length()) != 0)
				continue;

			vtid.push_back(pe.th32ProcessID);

		} while (Process32NextW(hSnapshot, &pe));

		CloseHandle(hSnapshot);
		return 0;
	}

	__int64 GetProcessCurrentMemory(int pid)
	{
		if(pid == 0) return -100;

		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
		if(handle == NULL) return -200;

		PROCESS_MEMORY_COUNTERS mem;
		if(!GetProcessMemoryInfo(handle,&mem,sizeof(mem)))
		{
			CloseHandle(handle);
			return -300;
		}

		return mem.WorkingSetSize;
	}

	int KillProcessByPath(const string& strExePath)
	{
		vector<int> vt;
		int i_ret = GetProcessPID(strExePath,vt);
		if (i_ret != 0)
			return i_ret;

		bool bfailed = false;
		for (unsigned int i = 0 ; i < vt.size() ;i++)
		{
			if ((i_ret = KillProcessByPID(vt[i])) != 0)
				bfailed = true;
		}

		return (bfailed?-100:0);
	}

	int KillProcessByPID(int pid)
	{
		if(pid == 0)
			return 0;

		HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		if(handle == NULL)
			return -1;

		BOOL b = TerminateProcess(handle, 0);

		CloseHandle(handle);

		if(b)
			return 0;

		return -2;
	}

	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	bool IsWow64()
	{
		BOOL bIsWow64 = FALSE;

		LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;

		//IsWow64Process is not available on all supported versions of Windows.
		//Use GetModuleHandle to get a handle to the DLL that contains the function
		//and GetProcAddress to get a pointer to the function if available.

		fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

		if(NULL == fnIsWow64Process)
			return false;

		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			DWORD dw = GetLastError();
		}

		return bIsWow64?true:false;
	}

	int StartServiceM(const string& strServiceName,int nArg,const char** pArg)
	{
		DWORD dwRet = 0;

		SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
		if (schSCManager == NULL)
			return GetLastError();

		SC_HANDLE schService = OpenServiceA(schSCManager, strServiceName.c_str(), SERVICE_ALL_ACCESS);
		if (schService == NULL)
		{
			CloseServiceHandle(schSCManager);
			return GetLastError();
		}

		if(!::StartServiceA(schService,nArg,pArg))
		{
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return GetLastError();
		}

		int nCount = 7;
		while(nCount > 0)
		{
			Sleep(100);
			SERVICE_STATUS serviceStatus = {0};
			if (!::QueryServiceStatus(schService, &serviceStatus))
			{
				CloseServiceHandle(schService);
				CloseServiceHandle(schSCManager);
				return GetLastError();;
			}

			if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
				break;

			nCount--;
		}

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager); 

		return dwRet;
	}
	int StopServiceM(const string& strServiceName)
	{

		DWORD dwRet = 0;

		SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
		if (schSCManager==0) 
		{
			return GetLastError();
		}

		SC_HANDLE schService = OpenServiceA(schSCManager, strServiceName.c_str(), SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			dwRet = GetLastError();
		}
		else
		{
			SERVICE_STATUS svcStatus = {0,0,0,0,0,0,0};;
			if(ControlService(schService,SERVICE_CONTROL_STOP, &svcStatus))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return 0;
			}
			else
			{
				dwRet = GetLastError();
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 

		return dwRet;
	}
#endif

	int GetCPUNumbers()
	{
#ifdef WIN32
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
#else
		return sysconf(_SC_NPROCESSORS_ONLN);
#endif
		
	}

	//创建一个32位长度的随机字串，只有数字和字母
	string MakeUUID()
	{
		
#ifdef WIN32
		string struuidout;
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char* str;

		if(UuidToStringA(&uuid, &str) == RPC_S_OK)
		{
			//int i = 0;
			for (int i = 0 ; *(str + i) != '\0'; ++i)
			{
				if (*(str + i) == '-')
				{
					continue;
				}
				struuidout.push_back(*(str + i));
			}

			RpcStringFreeA(&str);
			return struuidout;
		}

		
		char uid[64];
		unsigned int ui_now = (unsigned int )GetTickCount();
		//srand(ui_now);
		sprintf_s(uid,64,"%u%u%u",ui_now,(unsigned int)time(0),(unsigned int)rand());
		struuidout = uid;

		return struuidout;
#else
		char uid[64] = {0};
		unsigned long long t_temp = time(0);
		int ilen = sprintf_s(uid,64,"%llu",t_temp);
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		t_temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		ilen += sprintf_s(uid + ilen,32,"%llu",t_temp);
		while (ilen < 32)
		{
			ilen = sprintf_s(uid + ilen,32,"%x",rand());
		}
		uid[32] = 0;
		return uid;
#endif
	}

	//创建一个32位长度的随机字串，只有数字和字母
	int MakeUUID(string& struuid)
	{
		if(!struuid.empty()) struuid.clear();
#ifdef WIN32
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char* str;
		if(UuidToStringA(&uuid, &str) == RPC_S_OK)
		{
			struuid.reserve(32);
			//int i = 0;
			for (int i = 0 ; *(str + i) != '\0'; ++i)
			{
				if (*(str + i) == '-')
				{
					continue;
				}
				struuid.push_back(*(str + i));
			}

			RpcStringFreeA(&str);
			return 0;
		}

		char uid[64];
		unsigned int ui_now = (unsigned int )GetTickCount();
		//srand(ui_now);
		sprintf_s(uid,64,"%u%u%u",ui_now,(unsigned int)time(0),(unsigned int)rand());
		struuid = uid,strlen(uid);
		return 0;
#else
		char uid[64] = {0};
		unsigned long long t_temp = time(0);
		int ilen = sprintf_s(uid,64,"%llu",t_temp);
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		t_temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		ilen += sprintf_s(uid + ilen,32,"%llu",t_temp);
		while (ilen < 32)
		{
			ilen = sprintf_s(uid + ilen,32,"%x",rand());
		}
		uid[32] = 0;
		struuid = uid;
		return 0;
#endif
	}

#if 0
	volatile long __index__id__ = 0;
	int make_exclusive_id_by_time(char* buff)
	{
		time_t t_now;
		//static volatile long index = 0;
		time(&t_now);
		char sztemp[24];
		char* p(sztemp),*pbegin(buff);
		short digital,ilen(0);
		do
		{
			digital = t_now%16;
			t_now /= 16;
			if(digital > 9) *p++ = digital - 10 + 'a';
			else *p++ = digital + '0';
		} while (t_now > 0);
		--p;

		do 
		{
			*buff++ = *p--;
		} while (p != sztemp - 1);

		unsigned long itemp = ((unsigned long)HCommonFun::InterlockedIncrementM(&__index__id__))%0xfffffff;

		do
		{
			digital = itemp%16;
			itemp /= 16;
			if(digital > 9) *p++ = digital - 10 + 'a';
			else *p++ = digital + '0';
		} while (t_now > 0);

		for (digital = p - sztemp;digital < 8; ++digital)
		{
			*p++ = '0';
		}
		--p;

		do 
		{
			*buff++ = *p--;
		} while (p != sztemp-1);

		*buff = 0;
		return buff - pbegin;
	}
#else
	volatile long __index__id__ = 0;
	int make_exclusive_id_by_time(char* buff)
	{
		time_t t_now;
		
		buff[15] = 0;
		time(&t_now);
		//char sztemp[24];
		char* p(buff + 7),*pbegin(buff);
		short digital,ilen(0);
		do
		{
			digital = t_now%16;
			t_now /= 16;
			if(digital > 9) *p-- = digital - 10 + 'a';
			else *p-- = digital + '0';
		} while (p + 1 != buff);

		unsigned long itemp = ((unsigned long)HCommonFun::InterlockedIncrementM(&__index__id__))%0xfffffff;

		p = buff + 14;
		do
		{
			digital = itemp%16;
			itemp /= 16;
			if(digital > 9) *p-- = digital - 10 + 'a';
			else *p-- = digital + '0';
		} while (p != buff + 7);

		for (buff = buff + 8; buff != p + 1;++buff)
		{
			*buff = '0';
		}
		//for (digital = p - (buff + 7);digital < 8; ++digital)
		//{
		//	*p-- = '0';
		//}
		return (int)(buff - pbegin);
	}
#endif

	void make_exclusive_id_rand()
	{
		HCommonFun::set_def_locale();
		HCommonFun::InterlockedComparedExchangeM(&__index__id__,(long)(rand()*HCommonFun::GetCurrentTId()),0);
	}

	void make_exclusive_id_by_time(string& strid)
	{
		char buff[16];
		make_exclusive_id_by_time(buff);
		strid = buff;
	}

	int WSAStartupWin32()
	{
#ifdef _WIN32
		static bool binited = false;
		if(!binited)
		{
			binited = true;
			WSADATA wsaData;
			return WSAStartup(MAKEWORD(2, 2), &wsaData);
		}
#endif
		return -100;
	}

#ifdef WIN32
	int NetToIP(const char* strNewAddr,char* strip,int ilen)
	{
		if (ilen < 20)
			return -1;

		int iResult = WSAStartupWin32();
		if (iResult != 0)
		{
			return -3;
		}

		HOSTENT *host_entry = gethostbyname(strNewAddr);
		//printf("%s\n", host_name);
		if(host_entry != 0)
			sprintf_s(strip,16,"%d.%d.%d.%d",(host_entry->h_addr_list[0][0]&0x00ff),(host_entry->h_addr_list[0][1]&0x00ff),(host_entry->h_addr_list[0][2]&0x00ff),(host_entry->h_addr_list[0][3]&0x00ff));
		else//gethostbyname 失败了
		{
			WSACleanup();
			return -9;
		}

		WSACleanup();
		return 0;
	}

	int GetIPListLocal(vector<string>& vt)
	{
		if(!vt.empty()) vt.clear();
		ULONG ulOutBufLen;
		// 第一次调用GetAdapterInfo获取ulOutBufLen大小 
		if (GetAdaptersInfo(NULL, &ulOutBufLen) != ERROR_BUFFER_OVERFLOW)
		{
			return -100;
		}

		DWORD dwRetVal = 0;
		IP_ADAPTER_INFO* pAdapterInfo = (IP_ADAPTER_INFO* ) new char[ulOutBufLen];

		if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != NO_ERROR) 
		{
			delete [] (char*)pAdapterInfo;
			return -200;
		}
		IP_ADAPTER_INFO* pAdapter = pAdapterInfo; 
		while (pAdapter)
		{
			PIP_ADDR_STRING pIPAddr;
			pIPAddr = &pAdapter->IpAddressList;
			while (pIPAddr)
			{
				vt.push_back(pIPAddr->IpAddress.String);
				pIPAddr = pIPAddr->Next;
			}
			pAdapter = pAdapter->Next; 
		}
		vt.push_back("127.0.0.1");
		delete [] (char*)pAdapterInfo;
		return 0;
	}

	int TestIPAndPortBindable(const char* strip,unsigned short port)
	{
		WSAStartupWin32();

		if(strip == NULL) return -100;
		if(!is_ipv4_format(strip)) return -200;
		if(port == 0) return -300;
		SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
		if(sock == INVALID_SOCKET) return -400;

		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr=inet_addr(strip);
		addrSrv.sin_family=AF_INET;
		addrSrv.sin_port=htons(port);
		int iret = bind(sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
		iret = WSAGetLastError();
		closesocket(sock);
		return iret;
	}
#endif
	__int64 GetCurrentPId()
	{
#ifdef WIN32
		return GetCurrentProcessId();
#else
		return getpid();
#endif
	}
	__int64 GetCurrentTId()
	{
#ifdef WIN32
		return ::GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}

	void Sleep(unsigned int imillSec)
	{
#ifdef WIN32
		::Sleep(imillSec);
#else
		::usleep(imillSec * 1000);
#endif
	}

	string GetSystem32Dir(bool bHaveSlash)
	{
#ifdef WIN32
		char buf[MAX_PATH] = {0};
		GetSystemDirectoryA(buf,MAX_PATH);
		string str = buf;
		DirFit(str,bHaveSlash);
		return str;
#else

		string str = "/etc";

		DirFit(str,bHaveSlash);
		return str;
#endif
	}

	__int64 GetTickCountM()
	{
#ifdef WIN32
		return ::GetTickCount();
#else
		struct timespec ts;

		clock_gettime(CLOCK_MONOTONIC, &ts);

		__int64 itick = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		return itick;
#endif
	}

	long InterlockedExchangeM(volatile long* i,int iexchange)
	{
#ifdef WIN32
		return InterlockedExchange(i,iexchange);
#else
		return __sync_lock_test_and_set(i,iexchange);
#endif
	}

	long long InterlockedExchange64M(volatile long long* i,long long iexchange)
	{
#ifdef WIN32
		return InterlockedExchange64(i,iexchange);
#else
		return __sync_lock_test_and_set(i,iexchange);
#endif
	}

	long InterlockedIncrementM(volatile long* i,int iadd)
	{
#ifdef WIN32
		return InterlockedExchangeAdd(i,iadd);
#else
		return __sync_fetch_and_add(i,iadd);
#endif
	}

	long long InterlockedIncrement64M(volatile long long* i,long long iadd)
	{
#ifdef WIN32
		return InterlockedExchangeAdd64(i,iadd);
#else
		return __sync_fetch_and_add(i,iadd);
#endif
	}

	long InterlockedComparedExchangeM(volatile long* i,long iexchange,long lcompvalue)
	{
#ifdef WIN32
			return InterlockedCompareExchange(i,iexchange,lcompvalue);
#else
			return __sync_val_compare_and_swap(i,lcompvalue,iexchange);
#endif
	}
	long GetIncreasementID()
	{
		static volatile long i = 0;
#ifdef WIN32
		return InterlockedExchangeAdd(&i,1);
#else
		return __sync_fetch_and_add(&i,1);
#endif
	}
};