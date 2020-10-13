// IniFileOpt.h: interface for the CIniFileOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IniFileOpt_H__A7F682B7_9100_4345_B232_701448320253__INCLUDED_)
#define AFX_IniFileOpt_H__A7F682B7_9100_4345_B232_701448320253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFileOpt  
{
public:
	CIniFileOpt(const char* szIniFileName);
	virtual ~CIniFileOpt();

public:
	int ReadString(const char* _szSection, const char* _szKey, char* _szResult, int _nLen);
	int WriteString(const char* _szSection, const char* _szKey, const char* _szText);
	int ReadInt(const char* _szSection, const char* _szKey);
	
private:
	char m_szIniPathName[512];
};

#endif // !defined(AFX_IniFileOpt_H__A7F682B7_9100_4345_B232_701448320253__INCLUDED_)
