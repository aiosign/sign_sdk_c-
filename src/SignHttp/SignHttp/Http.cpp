#include "stdafx.h"
#include "Http.h"
#include <sstream>
#include "Utility.h";
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

#include "..\\SealLog\SealLog.h"

#pragma comment(lib, "Wininet.lib")

using namespace utils;

CHttp::CHttp(void) :m_hSession(NULL), m_hConnect(NULL), m_hRequest(NULL)
{
	m_iConnTimeout = 4000; //ms
}

CHttp::~CHttp(void)
{
	Release();
}

// 关闭句柄 [3/14/2017/shike]
void CHttp::Release()
{
	ReleaseHandle(m_hRequest);
	ReleaseHandle(m_hConnect);
	ReleaseHandle(m_hSession);
}

// 释放句柄 [3/14/2017/shike]
void CHttp::ReleaseHandle(HINTERNET& hInternet)
{
	if (hInternet)
	{
		InternetCloseHandle(hInternet);
		hInternet = NULL;
	}
}

//  通过HTTP请求：Get或Post方式获取JSON信息 
std::string CHttp::RequestJsonInfo(const std::string& lpUrl, HttpRequest type, std::string strHeader, std::string strPostData)
{
	//这个函数有问题，同一个链接在某些客户的电脑中会频繁出现404的情况，让客户重装系统后就正常了
	std::string strRet = "";
	int nSendLastErr = 0;
	try
	{
		if (lpUrl.empty())
		{
			throw Hir_ParamErr;
		}

		Release();
		m_hSession = InternetOpen("Http-connect", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);    //局部
		if (NULL == m_hSession)
		{
			throw Hir_InitErr;
		}

		INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT;
		std::string strHostName = "";
		std::string strPageName = "";

		ParseURLWeb(lpUrl, strHostName, strPageName, port);
		printf("lpUrl:%s,\nstrHostName:%s,\nstrPageName:%s,\nport:%d\n", lpUrl.c_str(), strHostName.c_str(), strPageName.c_str(), (int)port);
		m_hConnect = InternetConnect(m_hSession, strHostName.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (NULL == m_hConnect)
		{
			nSendLastErr = GetLastError();
			throw Hir_ConnectErr;
		}
		std::string strRequestType;
		if (Hr_Get == type)
		{
			strRequestType = "GET";
		}
		else
		{
			strRequestType = "POST";
		}

		m_hRequest = HttpOpenRequest(m_hConnect, strRequestType.c_str(), strPageName.c_str(), "HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, NULL);
		if (NULL == m_hRequest)
		{
			nSendLastErr = GetLastError();
			throw Hir_InitErr;
		}
		BOOL bRet = FALSE;
		DWORD dwHeaderSize = (strHeader.empty()) ? 0 : strlen(strHeader.c_str());

		if (Hr_Get == type)
		{
			bRet = HttpSendRequest(m_hRequest, strHeader.c_str(), dwHeaderSize, NULL, 0);
		}
		else
		{
			DWORD dwSize = strPostData.length();
			bRet = HttpSendRequest(m_hRequest, strHeader.c_str(), dwHeaderSize, (LPVOID)strPostData.c_str(), dwSize);
		}
		if (!bRet)
		{
			nSendLastErr = GetLastError();
			throw Hir_SendErr;
		}

		TCHAR szBuffer[READ_BUFFER_SIZE] = { 0 };
		DWORD dwReadSize = READ_BUFFER_SIZE - 1;
		if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_RAW_HEADERS, szBuffer, &dwReadSize, NULL))
		{
			nSendLastErr = GetLastError();
			throw Hir_QueryErr;
		}
		if (NULL != strstr(szBuffer, "404"))
		{
			nSendLastErr = GetLastError();
			string strPostLen = "RequestJsonInfo HttpQueryInfo Error: szBuffer=" + string(szBuffer) + "\r\n";
			Writelog(strPostLen.c_str());
			throw Hir_404;
		}
		DWORD readed = 0;
		char buffer[1025];
		do
		{
			ZeroMemory(buffer, 1025);
			InternetReadFile(m_hRequest, buffer, 1024, &readed);
			buffer[readed] = '\0';
			strRet.append(buffer);;
		} while (readed);
	}
	catch (HttpInterfaceError error)
	{
		m_error = error;
		string strPostLen = "RequestJsonInfo throw: lpUrl=" + lpUrl + ", m_error=" + std::to_string(m_error)
			+ ", nSendLastErr=" + std::to_string(nSendLastErr) + "\r\n";
		Writelog(strPostLen.c_str());
	}
	//是为性能而生 是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝。
	return std::move(strRet);
}

std::string CHttp::RequestHttpsJsonInfo(const std::string& lpUrl, HttpRequest type, std::string strHeader, std::string strPostData)
{
	//HINTERNET hInternet = InternetOpen("WinInetGet/0.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//if (!hInternet) 
	//{
	//	return "";
	//}
	//LPCTSTR lpszServerName = "60.216.5.244"; //设置server   
	////INTERNET_PORT nServerPort = 48103; // HTTPS端口443   
	//LPCTSTR lpszUserName = NULL; //无登录用户名   
	//LPCTSTR lpszPassword = NULL; //无登录密码   
	//DWORD dwConnectFlags = 0;
	//DWORD dwConnectContext = 0;
	//INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTPS_PORT; // HTTPS端口443   
	//HINTERNET hConnect = InternetConnect(hInternet,
	//	lpszServerName, nServerPort,
	//	lpszUserName, lpszPassword,
	//	INTERNET_SERVICE_HTTP,
	//	dwConnectFlags, dwConnectContext);
	//if (!hConnect) {
	//	return "";

	//}
	//LPCTSTR lpszVerb = "POST";
	//LPCTSTR lpszObjectName = "/sdca/ca_service.php?act=register&application_code=SD20190328-YY01";
	//LPCTSTR lpszVersion = NULL;    // 默认.   
	//LPCTSTR lpszReferrer = NULL;   // 没有引用页   
	//LPCTSTR *lplpszAcceptTypes = NULL; // Accpet所有类型.   
	//DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
	//	INTERNET_FLAG_KEEP_CONNECTION |
	//	INTERNET_FLAG_NO_AUTH |
	//	INTERNET_FLAG_NO_COOKIES |
	//	INTERNET_FLAG_NO_UI |
	//	//设置启用HTTPS   
	//	INTERNET_FLAG_SECURE |
	//	INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
	//	INTERNET_FLAG_RELOAD;
	//DWORD dwOpenRequestContext = 0;
	////初始化Request   
	//HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
	//	lpszReferrer, lplpszAcceptTypes,
	//	dwOpenRequestFlags, dwOpenRequestContext);
	//if (!hRequest) {
	//	return "";
	//}
	//BOOL bResult = FALSE;
	////string szHeaderBuff = "Content-Type:application/x-www-form-urlencoded";
	//string szHeaderBuff = "Content-Type:application/json";
	//int nReqSize = 0;
	//const char* szPostData = strPostData.c_str();
	//DWORD dwSize = strlen(szPostData);
	////发送Request   
	//bResult = HttpSendRequest(hRequest, szHeaderBuff.c_str(), szHeaderBuff.length(), (LPVOID)szPostData, dwSize);
	//if (!bResult && GetLastError() == ERROR_INTERNET_INVALID_CA)
	//{
	//	DWORD dwFlags;
	//	DWORD dwBuffLen = sizeof(dwFlags);
	//	InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);
	//	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	//	InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
	//	bResult = HttpSendRequest(hRequest, szHeaderBuff.c_str(), szHeaderBuff.length(), (LPVOID)szPostData, dwSize);
	//}
	//if (!bResult)
	//{
	//	DWORD dwError = GetLastError();
	//	printf("%d/n", dwError);
	//	return "";
	//}
	////获得HTTP Response Header信息   
	//char szBuff[1024];
	//DWORD dwReadSize = 1024;
	//bResult = HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szBuff, &dwReadSize, NULL);
	//if (!bResult) {
	//	return "";

	//}
	//szBuff[dwReadSize] = 0;
	//printf("%s/n", szBuff);
	////HTTP Response 的 Body   
	//DWORD dwBytesAvailable;
	//bResult = InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0);
	//if (!bResult) {
	//	return "";

	//}
	std::string strRet = "";
	int nRet = 1;
	LPCTSTR lpszAgent = "WinInetGet/0.1";
	HINTERNET hInternet = InternetOpen(lpszAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternet) 
	{
		return "";
	}
	LPCTSTR lpszServerName = "60.216.5.244"; //设置server   
	INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTPS_PORT; // HTTPS端口443   
	LPCTSTR lpszUserName = NULL; //无登录用户名   
	LPCTSTR lpszPassword = NULL; //无登录密码   
	DWORD dwConnectFlags = 0;
	DWORD dwConnectContext = 0;
	HINTERNET hConnect = InternetConnect(hInternet,
		lpszServerName, nServerPort,
		lpszUserName, lpszPassword,
		INTERNET_SERVICE_HTTP,
		dwConnectFlags, dwConnectContext);
	if (!hConnect) 
	{
		return "";
	}
	LPCTSTR lpszVerb = "POST";
	LPCTSTR lpszObjectName = "/sdca/ca_service.php?act=register&application_code=SD20190328-YY01";
	LPCTSTR lpszVersion = NULL;    // 默认.   
	LPCTSTR lpszReferrer = NULL;   // 没有引用页   
	LPCTSTR *lplpszAcceptTypes = NULL; // Accpet所有类型.   
	DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_KEEP_CONNECTION |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_COOKIES |
		INTERNET_FLAG_NO_UI |
		//设置启用HTTPS   
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_RELOAD;
	DWORD dwOpenRequestContext = 0;
	//初始化Request   
	HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
		lpszReferrer, lplpszAcceptTypes,
		dwOpenRequestFlags, dwOpenRequestContext);
	string szHeaderBuff = "Content-Type:application/json";
	if (!hRequest) 
	{
		goto GOTO_EXIT;
	}
	bool bResult = false;
	const char* szPostData = strPostData.c_str();
	DWORD dwSize = strlen(szPostData);
	//发送Request   
	bResult = HttpSendRequest(hRequest, szHeaderBuff.c_str(), szHeaderBuff.length(), (LPVOID)szPostData, dwSize);
	if (!bResult && GetLastError() == ERROR_INTERNET_INVALID_CA)
	{
		DWORD dwFlags;
		DWORD dwBuffLen = sizeof(dwFlags);
		InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);
		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
		InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
		bResult = HttpSendRequest(hRequest, szHeaderBuff.c_str(), szHeaderBuff.length(), (LPVOID)szPostData, dwSize);
	}
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		goto GOTO_EXIT;
	}
	//获得HTTP Response Header信息   
	char szBuff[2014];
	DWORD dwReadSize = 1024;
	bResult = HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szBuff, &dwReadSize, NULL);
	if (!bResult)
	{
		goto GOTO_EXIT;
	}
	szBuff[dwReadSize] = '/0';
	//HTTP Response 的 Body   
	DWORD dwBytesAvailable;
	bResult = InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0);
	if (!bResult) 
	{
		goto GOTO_EXIT;
	}
	if (dwBytesAvailable > 1024)
	{
		//printf("tool long %d /n", GetLastError(), dwBytesAvailable);
		goto GOTO_EXIT;
	}
	//DWORD dwBytesRead;
	//bResult = InternetReadFile(hRequest, szBuff, dwBytesAvailable, &dwBytesRead);
	//if (!bResult) 
	//{
	//	goto GOTO_EXIT;
	//}
	//szBuff[dwBytesRead] = '/0';

	DWORD readed = 0;
	char buffer[1025];
	do
	{
		ZeroMemory(buffer, 1025);
		InternetReadFile(hRequest, buffer, 1024, &readed);
		buffer[readed] = '\0';
		strRet.append(buffer);;
	} while (readed);

GOTO_EXIT:
	if (hRequest) 
	{
		InternetCloseHandle(hRequest);
	}
	if (hConnect) 
	{
		InternetCloseHandle(hConnect);
	}
	if (hInternet) 
	{
		InternetCloseHandle(hInternet);
	}
	return strRet;
}

std::string CHttp::RequestHttpsJsonInfo1(const std::string & lpUrl, HttpRequest type, std::string lpHeader, std::string lpPostData)
{
	//检测网路状态
	//if (!InternetCheckConnection(lpUrl.c_str(), FLAG_ICC_FORCE_CONNECTION, 0))
	//{
	//	return "";
	//}
	m_hSession = InternetOpen("WinInetGet/0.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (m_hSession <= 0)
	{
		return "";
	}
	INTERNET_PORT port = INTERNET_DEFAULT_HTTPS_PORT;
	std::string strHostName = "";
	std::string strPageName = "";

	ParseURLWeb(lpUrl, strHostName, strPageName, port);
	printf("lpUrl:%s,\nstrHostName:%s,\nstrPageName:%s,\nport:%d\n", lpUrl.c_str(), strHostName.c_str(), strPageName.c_str(), (int)port);
	m_hConnect = InternetConnect(m_hSession, strHostName.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	if (m_hConnect <= 0)
	{
		//InternetCloseHandle(hOpen);
		return "";
	}
	DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
	std::string Data = "";
	dwFlags = dwFlags | INTERNET_FLAG_SECURE;

	std::string strRequestType;
	if (Hr_Get == type)
	{
		strRequestType = "GET";
	}
	else
	{
		strRequestType = "POST";
	}
	m_hRequest = HttpOpenRequest(m_hConnect, strRequestType.c_str(), strPageName.c_str(), "HTTP/1.1", NULL, NULL, dwFlags, NULL);
	if (m_hRequest <= 0)
	{
		//InternetCloseHandle(hCon);
		//InternetCloseHandle(hOpen);
		return "";
	}
	std::string Headers;
	Headers = "Content-Type:application/json";
	if (lpHeader.length() != 0)
	{
		Headers += "\r\nsign:";
		Headers += lpHeader;
		Headers += "\r\n";
	}

	BOOL bRet = FALSE;
	const char* szPostData = lpPostData.c_str();
	bRet = HttpSendRequest(m_hRequest, Headers.c_str(), Headers.length(), (LPVOID)szPostData, strlen(szPostData));
	if (!bRet)
	{
		//InternetCloseHandle(hCon);
		//InternetCloseHandle(hOpen);
		//InternetCloseHandle(hReq);
		return "";
	}
	//查询状态码
	DWORD dwRetCode = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	bRet = HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
	//if (dwRetCode == HTTP_STATUS_OK || dwRetCode == HTTP_STATUS_CREATED)
	//{

	//}

	// 获取返回数据的大小
	//DWORD dwNumberOfBytesAvailable = 0;
	//bRet = InternetQueryDataAvailable(hReq, &dwNumberOfBytesAvailable, NULL, NULL);

	//// 读取网页内容
	//char* lpBuffer = new char[dwNumberOfBytesAvailable];
	//bRet = InternetReadFile(hReq, lpBuffer, dwNumberOfBytesAvailable, &dwNumberOfBytesAvailable);
	//Data.append(lpBuffer, dwNumberOfBytesAvailable);
	//delete[] lpBuffer;
	//lpBuffer = NULL;

	DWORD dwNumberOfBytesAvailable = 0;
	while (InternetQueryDataAvailable(m_hRequest, &dwNumberOfBytesAvailable, 0, 0))
	{
		//BYTE *pMessageBody = (BYTE *)malloc(dwNumberOfBytesAvailable+1);
		char* lpBuffer = new char[dwNumberOfBytesAvailable];
		DWORD dwBytesRead;
		BOOL bResult = InternetReadFile(m_hRequest, lpBuffer,dwNumberOfBytesAvailable, &dwBytesRead);
		if (!bResult) 
		{
			string strPostLen = "InternetReadFile failed: lpUrl=" + lpUrl + ", nSendLastErr=" + std::to_string(GetLastError()) + "\r\n";
			Writelog(strPostLen.c_str());
			break;
		}
		if (dwBytesRead == 0)
		{
			break; // End of File.
		}
		Data.append(lpBuffer, 0, dwBytesRead);
		delete[] lpBuffer;
		lpBuffer = NULL;
	}
	//InternetCloseHandle(hReq);
	//InternetCloseHandle(hCon);
	//InternetCloseHandle(hOpen);
	return std::move(Data);
}


int CHttp::GetFile(string strUrl, string strPath)
{
	//HINTERNET internetopen;
	//internetopen = InternetOpen("Testing", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//if (internetopen == NULL)
	//{
	//	return -1;
	//}
	//HINTERNET internetopenurl;
	//internetopenurl = InternetOpenUrl(internetopen, strUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	//if (internetopenurl == NULL) 
	//{
	//	return -2;
	//}
	//BOOL hwrite = 0;
	//DWORD written = 0;
	//HANDLE createfile;
	//DWORD byteread = 0;
	//char buffer[1024 * 3];
	//memset(buffer, 0, 1024 * 3);
	//createfile = CreateFile(strPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	//if (createfile == INVALID_HANDLE_VALUE) 
	//{
	//	return -3;
	//}
	//BOOL internetreadfile = 0;
	//internetreadfile = InternetReadFile(internetopenurl, buffer, sizeof(buffer), &byteread);
	//	
	//hwrite = WriteFile(createfile, buffer, sizeof(buffer), &written, NULL);
	//if (hwrite == 0) 
	//{
	//	return -4;
	//}
	//InternetCloseHandle(internetopenurl);
	//InternetCloseHandle(internetopen);
	//return 0;

	HINTERNET internetopen;
	internetopen = InternetOpen("FileDownload", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (internetopen == NULL)
	{
		return -1;
	}
	HINTERNET internetopenurl;
	internetopenurl = InternetOpenUrl(internetopen, strUrl.c_str(), NULL, 0, INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_RELOAD, 0);
	if (internetopenurl == NULL) 
	{
		return -2;
	}

	HANDLE createfile;
	createfile = CreateFile(strPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (createfile == INVALID_HANDLE_VALUE) 
	{
		return -3;
	}

	BOOL hwrite = 0;
	DWORD written = 0;

	//获取返回数据
	string Data;
	DWORD dwRetCode = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	BOOL bRet = FALSE;
	bRet = HttpQueryInfo(internetopenurl, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
	//if (dwRetCode == HTTP_STATUS_OK || dwRetCode == HTTP_STATUS_CREATED)
	//{
	//}

	DWORD dwNumberOfBytesAvailable = 0;
	while (InternetQueryDataAvailable(internetopenurl, &dwNumberOfBytesAvailable, 0, 0))
	{
		//BYTE *pMessageBody = (BYTE *)malloc(dwNumberOfBytesAvailable+1);
		BYTE* lpBuffer = new BYTE[dwNumberOfBytesAvailable+1];
		DWORD dwBytesRead = 0;
		BOOL bResult = InternetReadFile(internetopenurl, lpBuffer, dwNumberOfBytesAvailable, &dwBytesRead);
		if (!bResult)
		{
			string strPostLen = "InternetReadFile failed: lpUrl=" + strUrl + ", nSendLastErr=" + std::to_string(GetLastError()) + "\r\n";
			Writelog(strPostLen.c_str());
			break;
		}
		if (dwBytesRead == 0)
		{
			break; // End of File.
		}
		//Data.append(lpBuffer, 0, dwBytesRead);
		hwrite = WriteFile(createfile, lpBuffer, dwBytesRead, &dwBytesRead, NULL);
		delete[] lpBuffer;
		lpBuffer = NULL;
	}
	CloseHandle(createfile);
	InternetCloseHandle(internetopenurl);
	InternetCloseHandle(internetopen);
	return 0;
}

std::string CHttp::GetSignData(const std::string & strSrc, const std::string& secret_key)
{
	std::string strTemp = strSrc.substr(0, strSrc.find_last_of("}") + 1);
	int r = strTemp.find('\r\n');
	while (r != string::npos)
	{
		if (r != string::npos)
		{
			strTemp.replace(r, 1, "");
			r = strTemp.find('\r\n');
		}
	}
	int index = 0;
	if (!strTemp.empty())
	{
		while ((index = strTemp.find(' ', index)) != string::npos)
		{
			strTemp.erase(index, 1);
		}
	}
	std::string strRetData;
	hmac_sha256(strTemp, secret_key.c_str(), strRetData);
	char *szbase64 = new char[strRetData.length() * 3];
	int nbase64len = 0;
	base64encode(strRetData.c_str(), strRetData.length(), szbase64, nbase64len);
	strRetData.clear();
	md5(szbase64,strRetData);
	return std::move(strRetData);
}

std::string CHttp::GetCertApplyData(const std::string & strSrc)
{
	Json::Value root;
	Json::FastWriter writer;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strSrc.c_str(), json_object))
	{
		return "";
	}
	Json::Value data = json_object["data"];
	if (data.isNull() || !data.isObject())
	{
		return "";
	}
	string strUserId = data.get("user_id", "").asString();
	root["user_id"] = strUserId;
	std::string strJson = writer.write(root);
	return std::move(strJson);
}

// 解析URL地址 [3/14/2017/shike]
void CHttp::ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort)
{
	string strTemp(strUrl);
	std::size_t nPos = 0;
	nPos = strTemp.find("http://");
	if (nPos != std::string::npos)
	{
		sPort = 80;
		strTemp = strTemp.substr(nPos + 7, strTemp.size() - nPos - 7);
	}
	else
	{
		sPort = 443;
		nPos = strTemp.find("https://");
		if (nPos != std::string::npos)
		{
			strTemp = strTemp.substr(nPos + 8, strTemp.size() - nPos - 8);
		}
	}
	

	nPos = strTemp.find('/');
	if (nPos == std::string::npos)    //没有找到
	{
		strHostName = strTemp;
	}
	else
	{
		strHostName = strTemp.substr(0, nPos);
	}

	std::size_t nPos1 = strHostName.find(':');
	if (nPos1 != std::string::npos)
	{
		std::string strPort = strTemp.substr(nPos1 + 1, strHostName.size() - nPos1 - 1);
		strHostName = strHostName.substr(0, nPos1);
		sPort = (WORD)atoi(strPort.c_str());
	}
	if (nPos == std::string::npos)
	{
		return;
	}
	strPageName = strTemp.substr(nPos, strTemp.size() - nPos);
}

void CHttp::ParseURLWeb1(std::string strUrl, std::string strRegExp,std::string & strToken, std::string & strBaseUrl)
{
	std::string strFindData = "token=";
	int nPos = strUrl.find(strFindData);
	strToken = strUrl.substr(nPos + strFindData.length());
	strBaseUrl = strUrl.substr(0,strUrl.find(strRegExp));
}

std::string CHttp::UploadFile1(const std::string & strUrl, const std::string & strFilePath, const std::string& strFileName)
{
	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;

	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwBytesSend = 0;

	INTERNET_BUFFERS BufferIn;
	DWORD dwFlag;
	char* boundary = "-----------------------------67491722032265"; //随机字符串
	char* aboundary = "-----------------------------67491722032265"; //ansi
	HANDLE hFile;
	hFile = CreateFile(strFilePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		string strPostLen = "Failed to open local file" + strFileName + "\r\n";
		Writelog(strPostLen.c_str());
		return "";
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);

	char content_type[128];
	sprintf(content_type, ("Content-Type: multipart/form-data; boundary=%s"), boundary);

	hSession = InternetOpen("WinInetPost/0.1", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (0 == hSession)
	{
		return "";
	}
	INTERNET_PORT port = INTERNET_DEFAULT_HTTPS_PORT;
	std::string strHostName = "";
	std::string strPageName = "";
	ParseURLWeb(strUrl, strHostName, strPageName, port);

	hConnect = InternetConnect(hSession, strHostName.c_str(), INTERNET_DEFAULT_HTTPS_PORT, "", "", INTERNET_SERVICE_HTTP, 0, 0);
	if (0 == hConnect)
	{
		InternetCloseHandle(hSession);
		return "";
	}

	//dwFlag = INTERNET_FLAG_KEEP_CONNECTION;
	dwFlag = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
		INTERNET_FLAG_KEEP_CONNECTION |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_COOKIES |
		INTERNET_FLAG_NO_UI;
	dwFlag = dwFlag | INTERNET_FLAG_SECURE;

	hRequest = HttpOpenRequest(hConnect,
		"POST",
		strPageName.c_str(),
		HTTP_VERSION,
		0,                //Referrer
		0,                //AcceptTypes 
		dwFlag,
		0);
	if (0 == hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}
	HttpAddRequestHeaders(hRequest, content_type, -1, HTTP_ADDREQ_FLAG_ADD);

	BYTE* lpBuffer = (BYTE*)VirtualAlloc(0, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
	if (0 == lpBuffer)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}
	DWORD dwRead;
	ReadFile(hFile, lpBuffer, dwFileSize, &dwRead, 0);
	CloseHandle(hFile);

	char first_boundary[128];
	char delimiter[128];
	char end_boundary[128];
	sprintf_s(first_boundary, "--%s\r\n", aboundary);
	sprintf_s(delimiter, "\r\n--%s\r\n", aboundary);
	sprintf_s(end_boundary, "\r\n--%s--\r\n", aboundary);

	char content_dispos[128] = { 0 };
	if (strFilePath.length() > 0)
	{
		sprintf_s(content_dispos, "Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n", strFilePath.c_str());
	}
	//char* content_type2 = "Content-Type: image/jpeg\r\n\r\n";
	char* content_type2 = "Content-Type: application/octe-stream\r\n\r\n";
	

	std::string strMultipartFirst = first_boundary + string(content_dispos) + content_type2;

	std::string strMultipartEnd = end_boundary;

	int cSendBufLen = strMultipartFirst.size() + dwFileSize + strMultipartEnd.size();
	PCHAR pchSendBuf = new CHAR[cSendBufLen];
	memcpy(pchSendBuf, strMultipartFirst.c_str(), strMultipartFirst.size());
	memcpy(pchSendBuf + strMultipartFirst.size(), (const char *)lpBuffer, dwFileSize);
	memcpy(pchSendBuf + strMultipartFirst.size() + dwFileSize , strMultipartEnd.c_str(), strMultipartEnd.size());

	if (!HttpSendRequest(hRequest, NULL, 0, (LPVOID)pchSendBuf, cSendBufLen))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}

	//获取返回数据
	string Data;
	DWORD dwRetCode = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	BOOL bRet = FALSE;
	bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);

	DWORD dwNumberOfBytesAvailable = 0;
	while (InternetQueryDataAvailable(hRequest, &dwNumberOfBytesAvailable, 0, 0))
	{
		//BYTE *pMessageBody = (BYTE *)malloc(dwNumberOfBytesAvailable+1);
		char* lpBufferR = new char[dwNumberOfBytesAvailable];
		DWORD dwBytesRead;
		BOOL bResult = InternetReadFile(hRequest, lpBufferR, dwNumberOfBytesAvailable, &dwBytesRead);
		if (!bResult)
		{
			string strPostLen = "InternetReadFile failed: lpUrl=" + strUrl + ", nSendLastErr=" + std::to_string(GetLastError()) + "\r\n";
			Writelog(strPostLen.c_str());
			break;
		}
		if (dwBytesRead == 0)
		{
			break; // End of File.
		}
		Data.append(lpBufferR, 0, dwBytesRead);
		delete[] lpBufferR;
		lpBufferR = NULL;
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	VirtualFree(lpBuffer, 0, MEM_RELEASE);
	delete[] pchSendBuf;
	pchSendBuf = NULL;
	return std::move(Data);
}

std::string CHttp::UploadFile(const std::string & strUrl, const std::string & strFilePath, const std::string & strFileName, const std::string & strFileType, const std::string & strUserId)
{
	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;

	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwBytesSend = 0;

	INTERNET_BUFFERS BufferIn;
	DWORD dwFlag;
	char* boundary = "-----------------------------67491722032265"; //随机字符串
	char* aboundary = "-----------------------------67491722032265"; //ansi
	HANDLE hFile;
	hFile = CreateFile(strFilePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		string strPostLen = "Failed to open local file" + strFileName + "\r\n";
		Writelog(strPostLen.c_str());
		return "";
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);

	char content_type[128];
	sprintf(content_type, ("Content-Type: multipart/form-data; boundary=%s"), boundary);

	hSession = InternetOpen("WinInetPost/0.1", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (0 == hSession)
	{
		return "";
	}
	INTERNET_PORT port = INTERNET_DEFAULT_HTTPS_PORT;
	std::string strHostName = "";
	std::string strPageName = "";
	ParseURLWeb(strUrl, strHostName, strPageName, port);

	hConnect = InternetConnect(hSession, strHostName.c_str(), INTERNET_DEFAULT_HTTPS_PORT, "", "", INTERNET_SERVICE_HTTP, 0, 0);
	if (0 == hConnect)
	{
		InternetCloseHandle(hSession);
		return "";
	}

	//dwFlag = INTERNET_FLAG_KEEP_CONNECTION;
	dwFlag = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
		INTERNET_FLAG_KEEP_CONNECTION |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_COOKIES |
		INTERNET_FLAG_NO_UI;
	dwFlag = dwFlag | INTERNET_FLAG_SECURE;

	hRequest = HttpOpenRequest(hConnect,
		"POST",
		strPageName.c_str(),
		HTTP_VERSION,
		0,                //Referrer
		0,                //AcceptTypes 
		dwFlag,
		0);
	if (0 == hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}
	HttpAddRequestHeaders(hRequest, content_type, -1, HTTP_ADDREQ_FLAG_ADD);

	BYTE* lpBuffer = (BYTE*)VirtualAlloc(0, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
	if (0 == lpBuffer)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}
	DWORD dwRead;
	ReadFile(hFile, lpBuffer, dwFileSize, &dwRead, 0);
	CloseHandle(hFile);

	char first_boundary[128];
	char delimiter[128];
	char end_boundary[128];
	sprintf_s(first_boundary, "--%s\r\n", aboundary);
	sprintf_s(delimiter, "\r\n--%s\r\n", aboundary);
	sprintf_s(end_boundary, "\r\n--%s--\r\n", aboundary);

	char content_dispos[128] = { 0 };
	if (strFileName.length() > 0)
	{
		sprintf_s(content_dispos, "Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n", strFilePath.c_str());
	}
	char* content_type2 = "Content-Type: image/jpeg\r\n\r\n";

	char content_dispos1[128] = { 0 };
	sprintf_s(content_dispos1, "Content-Disposition: form-data; name=\"file_name\"\r\n\r\n%s", strFileName.c_str());

	char content_dispos3[128] = { 0 };
	sprintf_s(content_dispos3, "Content-Disposition: form-data; name=\"user_id\"\r\n\r\n%s", strUserId.c_str());

	char content_dispos4[128] = { 0 };
	sprintf_s(content_dispos4, "Content-Disposition: form-data; name=\"file_type\"\r\n\r\n%s", strFileType.c_str());

	std::string strMultipartFirst = first_boundary + string(content_dispos) + content_type2;
	std::string strMultipartInter;
	//if (strFileType == "template")
	//{
	//	strMultipartInter = delimiter + string(content_dispos1) + string(delimiter) + string(content_dispos4);
	//}
	//else
	//{
	//	strMultipartInter = delimiter + string(content_dispos1) + string(delimiter) + string(content_dispos3) + string(delimiter) + string(content_dispos4);
	//}
	strMultipartInter = delimiter + string(content_dispos1) + string(delimiter) + string(content_dispos3) + string(delimiter) + string(content_dispos4);
	std::string strMultipartEnd = end_boundary;

	int cSendBufLen = strMultipartFirst.size() + dwFileSize + strMultipartInter.size() + strMultipartEnd.size();
	PCHAR pchSendBuf = new CHAR[cSendBufLen];
	memcpy(pchSendBuf, strMultipartFirst.c_str(), strMultipartFirst.size());
	memcpy(pchSendBuf + strMultipartFirst.size(), (const char *)lpBuffer, dwFileSize);
	memcpy(pchSendBuf + strMultipartFirst.size() + dwFileSize, strMultipartInter.c_str(), strMultipartInter.size());
	memcpy(pchSendBuf + strMultipartFirst.size() + dwFileSize + strMultipartInter.size(), strMultipartEnd.c_str(), strMultipartEnd.size());

	if (!HttpSendRequest(hRequest, NULL, 0, (LPVOID)pchSendBuf, cSendBufLen))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return "";
	}

	//获取返回数据
	string Data;
	DWORD dwRetCode = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	BOOL bRet = FALSE;
	bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
	//if (dwRetCode == HTTP_STATUS_OK || dwRetCode == HTTP_STATUS_CREATED)
	//{

	//}

	// 获取返回数据的大小
	//DWORD dwNumberOfBytesAvailable = 0;
	//bRet = InternetQueryDataAvailable(hReq, &dwNumberOfBytesAvailable, NULL, NULL);

	//// 读取网页内容
	//char* lpBuffer = new char[dwNumberOfBytesAvailable];
	//bRet = InternetReadFile(hReq, lpBuffer, dwNumberOfBytesAvailable, &dwNumberOfBytesAvailable);
	//Data.append(lpBuffer, dwNumberOfBytesAvailable);
	//delete[] lpBuffer;
	//lpBuffer = NULL;

	DWORD dwNumberOfBytesAvailable = 0;
	while (InternetQueryDataAvailable(hRequest, &dwNumberOfBytesAvailable, 0, 0))
	{
		//BYTE *pMessageBody = (BYTE *)malloc(dwNumberOfBytesAvailable+1);
		char* lpBufferR = new char[dwNumberOfBytesAvailable];
		DWORD dwBytesRead;
		BOOL bResult = InternetReadFile(hRequest, lpBufferR, dwNumberOfBytesAvailable, &dwBytesRead);
		if (!bResult)
		{
			string strPostLen = "InternetReadFile failed: lpUrl=" + strUrl + ", nSendLastErr=" + std::to_string(GetLastError()) + "\r\n";
			Writelog(strPostLen.c_str());
			break;
		}
		if (dwBytesRead == 0)
		{
			break; // End of File.
		}
		Data.append(lpBufferR, 0, dwBytesRead);
		delete[] lpBufferR;
		lpBufferR = NULL;
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	VirtualFree(lpBuffer, 0, MEM_RELEASE);
	delete[] pchSendBuf;
	pchSendBuf = NULL;
	return std::move(Data);
}

void CHttp::GetFileAndSealInfo(const std::string & strSrc, FileAndSealInfo & sealInfo)
{
	Json::Value root;
	Json::FastWriter writer;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strSrc.c_str(), json_object))
	{
		return ;
	}

	sealInfo.strFilePath = json_object.get("file_path", "").asString();
	sealInfo.strFileName = json_object.get("file_name", "").asString();
	sealInfo.strFileType = json_object.get("file_type", "").asString();
	sealInfo.strUserId = json_object.get("user_id", "").asString();

	Json::Value sealDataValue = json_object["sealInfo"];
	if (sealDataValue.isNull() || !sealDataValue.isObject())
	{
		return;
	}
	sealInfo.strSealName = sealDataValue.get("seal_name", "").asString();
	sealInfo.strSealType = sealDataValue.get("seal_type", "").asString();
	sealInfo.strSealSize = sealDataValue.get("size", "").asString();
	sealInfo.strDescription = sealDataValue.get("description", "").asString();
}

// UTF-8转为GBK
std::string CHttp::UTF8ToGBK(const char* szUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr)
	{
		delete[] wstr;
	}

	std::string strRet(str);
	delete[] str;
	return std::move(strRet);
}

// GBK转UTF-8  注意删返回值
char* CHttp::GBKToUTF8(const char* szGbk)
{
	DWORD dWideBufSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szGbk, -1, NULL, 0);
	wchar_t * pWideBuf = new wchar_t[dWideBufSize];
	wmemset(pWideBuf, 0, dWideBufSize);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szGbk, -1, pWideBuf, dWideBufSize);

	DWORD dUTF8BufSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, NULL, 0, NULL, NULL);

	char* pUTF8Buf = new char[dUTF8BufSize];
	memset(pUTF8Buf, 0, dUTF8BufSize);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, pUTF8Buf, dUTF8BufSize, NULL, NULL);

	delete[]pWideBuf;
	return pUTF8Buf;
}

std::string CHttp::ConvertSpace(const std::string& strOrg)
{	
	size_t iLen = strOrg.length();
	//FILE* fW = fopen("2.dat","wb+");
	//if (fW != NULL)
	//{
	//	fwrite(strOrg.data(), 1, iLen, fW);
	//	fclose(fW);
	//}

	//处理 0xC2 0xA0
	char* szNewBuf = new char[iLen];
	memset(szNewBuf, 0, iLen);
	const char* szOrg = strOrg.data();
	int j = 0;
	int iCopyCnt = 1;

	for (int k = 0; k < iLen;)
	{
		iCopyCnt = 1;

		unsigned char by1 = szOrg[k];
		if (by1 <= 0x7F)
		{
			szNewBuf[j++] = szOrg[k++];
			continue;
		}

		if ((by1 & 0xC0) == 0xC0 && k < (iLen - 1))
		{
			unsigned char by2 = szOrg[k + 1];
			if (k < (iLen - 2) && by1 == 0xC2 && by2 == 0xA0)
			{
				szNewBuf[j++] = 0x20;
				k += 2;
				continue;
			}
			//iCopyCnt = 2;			
		}
		else if ((by1 & 0xE0) == 0xE0 && k < (iLen - 2))
		{
			iCopyCnt = 3;
		}
		else if ((by1 & 0xF0) == 0xF0 && k < (iLen - 3))
		{
			iCopyCnt = 4;
		}
		else if ((by1 & 0xF8) == 0xF8 && k < (iLen - 4))
		{
			iCopyCnt = 5;
		}
		else if ((by1 & 0xFC) == 0xFC && k < (iLen - 5))
		{
			iCopyCnt = 6;
		}

		for (int m = 0; m<iCopyCnt; ++m)
		{
			szNewBuf[j++] = szOrg[k++];
		}
	}

	//fW = fopen("3.dat", "wb+");
	//if (fW != NULL)
	//{
	//	fwrite(szNewBuf, 1, strlen(szNewBuf), fW);
	//	fclose(fW);
	//}

	std::string strRet(szNewBuf);
	delete[] szNewBuf;
	return std::move(strRet);
}