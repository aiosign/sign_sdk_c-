// IniFileOpt.cpp: implementation of the CIniFileOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFileOpt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFileOpt::CIniFileOpt(const char* szIniFileName)
{	
	m_szIniPathName[0] = 0x00;
	strcat_s(m_szIniPathName, 512, szIniFileName);	
	//printf_s("szIniFileName=%s\n", szIniFileName);
}

CIniFileOpt::~CIniFileOpt()
{

}

int CIniFileOpt::ReadString(const char* _szSection, const char* _szKey, char* _szResult, int _nLen)
{
	return GetPrivateProfileStringA(_szSection, _szKey, "", _szResult, _nLen, m_szIniPathName);
}

int CIniFileOpt::WriteString(const char* _szSection, const char* _szKey, const char* _szText)
{
	return WritePrivateProfileStringA(_szSection, _szKey, _szText, m_szIniPathName);
}

int CIniFileOpt::ReadInt(const char* _szSection, const char* _szKey)
{
	return GetPrivateProfileIntA(_szSection, _szKey, 0, m_szIniPathName);
}
