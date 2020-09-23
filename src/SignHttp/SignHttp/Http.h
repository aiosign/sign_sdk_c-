#pragma once
#include <Windows.h>
#include <string>
#include <windows.h>
#include <wininet.h>

using namespace std;

//ÿ�ζ�ȡ���ֽ���
#define READ_BUFFER_SIZE        4096

enum HttpInterfaceError
{
	Hir_Success = 0,		//�ɹ�
	Hir_InitErr,			//��ʼ��ʧ��
	Hir_ConnectErr,			//����HTTP������ʧ��
	Hir_SendErr,			//��������ʧ��
	Hir_QueryErr,			//��ѯHTTP����ͷʧ��
	Hir_404,				//ҳ�治����
	Hir_IllegalUrl,			//��Ч��URL
	Hir_CreateFileErr,		//�����ļ�ʧ��
	Hir_DownloadErr,		//����ʧ��
	Hir_QueryIPErr,			//��ȡ������Ӧ�ĵ�ַʧ��
	Hir_SocketErr,			//�׽��ִ���
	Hir_UserCancel,			//�û�ȡ������
	Hir_BufferErr,			//�ļ�̫�󣬻���������
	Hir_HeaderErr,			//HTTP����ͷ����
	Hir_ParamErr,			//�������󣬿�ָ�룬���ַ�
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
	// GBKתUTF-8  ע��ɾ����ֵ
	char* GBKToUTF8(const char* szGbk);

	//�޸�UTF-8ת����GBK������ 2020-07-03 cxr
	std::string ConvertSpace(const std::string& strOrg);

	// ͨ��HTTP����Get��Post��ʽ��ȡJSON��Ϣ 
	std::string RequestJsonInfo(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	std::string RequestHttpsJsonInfo(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	// ͨ��HTTP����Get��Post��ʽ��ȡJSON��Ϣ 
	std::string RequestHttpsJsonInfo1(const std::string& strUrl, HttpRequest type = Hr_Get, std::string lpHeader = "", std::string lpPostData = "");
	/*�����ļ�*/
	int GetFile(string strUrl, string strPath);

	//��ȡǩ������
	std::string GetSignData(const std::string& strSrc, const std::string& secret_key);
    
	//����ע�����ݣ�������װ����֤��JSON����
	std::string GetCertApplyData(const std::string& strSrc);
    
	//��ȡTOKEN
	void ParseURLWeb1(std::string strUrl, std::string strRegExp, std::string& strToken, std::string& strBaseUrl);

	//�ϴ��ļ�
	std::string UploadFile1(const std::string& strUrl, const std::string& strFilePath, const std::string& strFileName);

	//�ϴ��ļ�
	std::string UploadFile(const std::string& strUrl, const std::string& strFilePath, const std::string& strFileName, const std::string& strFileType, const std::string& strUserId);

	//�ļ��ϴ�������ӡ�½ӿ�
	void GetFileAndSealInfo(const std::string& strSrc, FileAndSealInfo& sealInfo);

protected:
	// �رվ�� 
	void Release();

	// �ͷž�� 
	void ReleaseHandle(HINTERNET& hInternet);
	// ����URL��ַ
	void ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort);
	
private:
	HINTERNET            m_hSession;
	HINTERNET            m_hConnect;
	HINTERNET            m_hRequest;
	HttpInterfaceError   m_error;
	int					 m_iConnTimeout;
};