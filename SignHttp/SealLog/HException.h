#pragma once
#include <stdio.h>
#include <string>
#define __HException_max_length__ (2048)
#define HEXCEPTION HException(__FILE__,__LINE__)

class HException
{
public:
	const char* what() const
	{
		return __error__;
	}
	HException(const char* file,int iline):__ilen(0)
	{
		__error__ = new char[__HException_max_length__];
		init_exception(file,iline);
		//__error__[0] = 0;
	}
	HException(const HException& e)
	{
		__error__ = e.__error__;
		__ilen = e.__ilen;
		HException* p = const_cast<HException*>(&e);
		p->__error__ = NULL;
		p->__ilen = 0;
	}

	~HException()
	{
		if(__error__ != NULL) delete [] __error__;
	}

	HException& operator = (const HException& e)
	{
		if(__error__ != NULL) delete [] __error__;
		__error__ = e.__error__;
		__ilen = e.__ilen;
		HException* p = const_cast<HException*>(&e);
		p->__error__ = NULL;
		p->__ilen = 0;
		return *this;
	}

	HException& operator << (const char* msg)
	{
		if(msg == NULL) return *this;
		int ilen = strlen(msg);
		this->Append(msg,ilen);
		return *this;
	}
	HException& operator << (const std::string& strmsg)
	{
		this->Append(strmsg.c_str(),strmsg.length());
		return *this;
	}
	HException& operator << (int ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%d",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (unsigned int ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%u",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (long ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%d",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (unsigned long ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%u",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	//WLog& operator << (unsigned int ilog);
	HException& operator << (short ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%d",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (unsigned short ilog)
	{
		char plog[16];
		int ilen = sprintf_s(plog,16,"%d",ilog);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (const long long& i64log)
	{
		char plog[24];
		int ilen = sprintf_s(plog,24,"%lld",i64log);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (const unsigned long long& i64log)
	{
		char plog[24];
		int ilen = sprintf_s(plog,24,"%llu",i64log);
		this->Append(plog,ilen);
		return *this;
	}
	HException& operator << (const double& doulog)
	{
		char plog[24];
		int ilen = sprintf_s(plog,24,"%f",doulog);
		this->Append(plog,ilen);
		return *this;
	}

	void init_exception(const char* filename,int linenum)
	{
		if(filename == NULL) filename = "w3unkowncpp";
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

		memcpy(__error__ + __ilen,filename,t);
		__ilen += t;
		__ilen += sprintf_s(__error__ + __ilen,__HException_max_length__,",%.4d>:",linenum);
		return;
	}
private:
	inline void Append(const char* msg,int ilen)
	{
		if(__ilen + ilen >= __HException_max_length__) ilen = __HException_max_length__ - (__ilen + 1);
		memcpy(__error__ + __ilen,msg,ilen);
		__ilen += ilen;
		__error__[__ilen] = 0;
	}
public:
	char* __error__;
	int __ilen;
};