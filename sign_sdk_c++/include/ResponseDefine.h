#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

typedef struct _TokenResponse
{
	int nExpiresIn;	//����ʱ��
	string strAccessToken;	//�û�����
	string strScope;	//��Ȩ����
	string strTokenType;	//Token����
}TokenResponse;

typedef struct _RegisterResponse
{
	string strUserId;	//�û�ID
}RegisterResponse;

//��ѯ�����û���Ϣ
typedef struct _GetPersonalUserInfoResponse
{
	CertInfo certInfo;
	PersonalUserInfo personalUserInfo;
}GetPersonalUserInfoResponse;

//�û�������
typedef struct _UserInfoHandleResponse
{
	bool bResult;
	string strUserId;	//�û�ID
	_UserInfoHandleResponse() : bResult(false) {}
}UserInfoHandleResponse;

//��ѯ��ҵ�û���Ϣ
typedef struct _GetCompanyInfoResponse
{
	CertInfo certInfo;
	CompanyInfo CompanyInfo;
}GetCompanyInfoResponse;

//��ѯ֤�鷵��
typedef struct _GetCertInfoResponse
{
	string strUserId;
	CertInfo certInfo;
}GetCertInfoResponse;

//����ӡ�·���
typedef struct _UploadAndAddSealInfoResponse
{
	string strSealId;
}UploadAndAddSealInfoResponse;

//ӡ�´�����
typedef struct _SealInfoHandleResponse
{
	bool bResult;
	string strSealId;	//ӡ��ID
	_SealInfoHandleResponse() : bResult(false) {}
}SealInfoHandleResponse;

//����ģ�淵��
typedef struct _UploadAndAddTemplateInfoResponse
{
	string strTemplateId;
}UploadAndAddTemplateInfoResponse;

//ģ�洦����
typedef struct _TemplateHandleResponse
{
	bool bResult;
	string strTemplateId;	//ģ��ID
	_TemplateHandleResponse() : bResult(false) {}
}TemplateHandleResponse;

//���Ӻ�ͬ����
typedef struct _UploadAndAddContractInfoResponse
{
	string strContractId;
}UploadAndAddContractInfoResponse;

//��ͬ������
typedef struct _ContractHandleResponse
{
	bool bResult;
	string strContractId;	//ģ��ID
	_ContractHandleResponse() : bResult(false) {}
}ContractHandleResponse;

//��ͬ����
typedef struct _ContractAbolishResponse
{
	bool bResult;
	string strSignId;
	string strFileId;
	_ContractAbolishResponse() : bResult(false) {}
}ContractAbolishResponse;

//��Ⱦ��ͬ���
typedef struct _ContractRenderResponse
{
	string strContractId;
}ContractRenderResponse;

//��λ���ͬ�ӿڷ���
typedef struct _ScanContractAddResponse
{
	string strPrepareId;	//Ԥ����ID
	string strQr;	//Base64���ص���ַ׷�Ӳ���Ԥ����Id�������ʽ�Ķ�ά��ͼƬ
}ScanContractAddResponse;

//��ͬ���ֻ��ŷ���
typedef struct _BindContractPhoneResponse
{
	string strContractId;	//��ͬID
}BindContractPhoneResponse;

//����ģ��ǩ��
typedef struct _BatchTemplateSignResponse
{
	string strBatchId;	//ҵ���
	BatchSignBackList BatchSignBackList;
}BatchTemplateSignResponse;

//֤��ǩ���¼�����
typedef struct _SignEventBackResponse
{
	SignBackInfo SignBackInfo;
	SignCertEventList SignCertEventList;
}SignEventBackResponse;

//����ģ��֤��ǩ���¼�����
typedef struct _BatchSignEventBackResponse
{
	string strBatchId;	//ҵ���
	BatchSignBackList BatchSignBackList;
	SignCertEventList SignCertEventList;
}BatchSignEventBackResponse;

//һ��ǩ��
typedef struct _SignOneStepAllResponse
{
	bool bSignState;	//ǩ����
	string strContractFile;	//ǩ����ɺ��ļ���������ת��Ϊ�ļ�
	string strCertIssuer;	//ǩ��ʱ֤��䷢��
	string strCertSn;	//ǩ��ʱ֤�����к�
	string strMessage;	//������Ϣ
	string strSignTime;	//ǩ��ʱ��
	string strSigner;	//ǩ����
	string strCertSubject;	//֤������
	string strSignId;	//����ǩ��ID
	string strHash;	//hashֵ
}SignOneStepAllResponse;

//��ͬ��ǩ
typedef struct _SignCheckResponse
{
	bool bResult;	//��֤���
	string strMessage;	//��֤��Ϣ
	SignCheckResultsList SignCheckResultsList;
}SignCheckResponse;