#pragma once

#include <Windows.h>
#include <string>

using namespace std;
class CMyGlobal
{
public:
	//static string m_strUrl;
	//static string m_strToken;
	//static string m_strAppSecret;
	//static string m_strAppId;

	static char m_szUrl[256];
	static char m_szToken[128];
	static char m_szAppSecret[64];
    static char m_strAppId[64];
};