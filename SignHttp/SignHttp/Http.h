#pragma once
#include <Windows.h>
#include <string>
#include <windows.h>
#include <wininet.h>

using namespace std;

//每次读取的字节数
#define READ_BUFFER_SIZE        4096

enum HttpInterfaceError
{
	Hir_Success = 0,		//成功
	Hir_InitErr,			//初始化失败
	Hir_ConnectErr,			//连接HTTP服务器失败
	Hir_SendErr,			//发送请求失败
	Hir_QueryErr,			//查询HTTP请求头失败
	Hir_404,				//页面不存在
	Hir_IllegalUrl,			//无效的URL
	Hir_CreateFileErr,		//创建文件失败
	Hir_DownloadErr,		//下载失败
	Hir_QueryIPErr,			//获取域名对应的地址失败
	Hir_SocketErr,			//套接字错误
	Hir_UserCancel,			//用户取消下载
	Hir_BufferErr,			//文件太大，缓冲区不足
	Hir_HeaderErr,			//HTTP请求头错误
	Hir_ParamErr,			//参数错误，空指针，空字符
	Hir_UnknowErr,
};

enum HttpRequest
{
	Hr_Get,
	Hr_Post
};

typedef struct _FileAndSealInfo
{
	string strFilePath;
	string strFileName;
	string strFileType;
	string strFileId;
	string strUserId;
	string strSealName;
	string strSealType;
	string strSealSize;
	string strDescription;

}FileAndSealInfo;

class CHttp
{
public:
	CHttp();
	~CHttp();

public:

	std::string UTF8ToGBK(const char* szUtf8);
	// GBK转UTF-8  注意删返回值
	char* GBKToUTF8(const char* szGbk);

	//修复UTF-8转换成GBK的乱码 2020-07-03 cxr
	std::string ConvertSpace(const std::string& strOrg);

	// 通过HTTP请求：Get或Post方式获取JSON信息 
	std::string RequestJsonInfo(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	std::string RequestHttpsJsonInfo(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	// 通过HTTP请求：Get或Post方式获取JSON信息 
	std::string RequestHttpsJsonInfo1(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	/*下载文件*/
	int GetFile(string strUrl, string strPath);

	//获取签名数据
	std::string GetSignData(const std::string& strSrc, const std::string& secret_key);
    
	//解析注册数据，返回组装申请证书JSON数据
	std::string GetCertApplyData(const std::string& strSrc);
    
	//获取TOKEN
	void ParseURLWeb1(std::string strUrl, std::string strRegExp, std::string& strToken, std::string& strBaseUrl);

	//上传文件
	std::string UploadFile1(const std::string& strUrl, const std::string& strFilePath, const std::string& strFileName);

	//上传文件
	std::string UploadFile(const std::string& strUrl, const std::string& strFilePath, const std::string& strFileName, const std::string& strFileType, const std::string& strUserId);

	//文件上传和增加印章接口
	void GetFileAndSealInfo(const std::string& strSrc, FileAndSealInfo& sealInfo);

protected:
	// 关闭句柄 
	void Release();

	// 释放句柄 
	void ReleaseHandle(HINTERNET& hInternet);
	// 解析URL地址
	void ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort);
	
private:
	HINTERNET            m_hSession;
	HINTERNET            m_hConnect;
	HINTERNET            m_hRequest;
	HttpInterfaceError   m_error;
	int					 m_iConnTimeout;
};