#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

typedef struct _AppInfo
{
	string strAppID;	//�豸ID
	string strAppSecret;	//�豸��Կ
	string strGrantType;
	_AppInfo() :strGrantType("client_credentials") {}
}AppInfo;

typedef struct _UserIdInfoReq
{
	string strUserId;	//�û�ID
}UserIdInfoReq;

//��ѯ֤��
typedef struct _CertInfoReq
{
	string strPrepareId;	//Ԥ����ID
}CertInfoReq;

//����ӡ������ӿ�
typedef struct _UploadAndAddSealInfoReq
{
	FileInfo FileInfo;
	SealInfo SealInfo;
}UploadAndAddSealInfoReq;

//����ӡ��ID
typedef struct _SealIdInfoReq
{
	string strSealId;
}SealIdInfoReq;

//����ģ��ID
typedef struct _TemplateIdInfoReq
{
	string strTemplateId;
}TemplateIdInfoReq;

//�����ͬID
typedef struct _ContractIdInfoReq
{
	string strContractId;
}ContractIdInfoReq;

//��ͬ����
typedef struct _ContractAbolishReq
{
	string strSignId;
	string strUserId;
}ContractAbolishReq;

//��λ���ͬ�ӿ�����
typedef struct _ScanContractAddReq
{
	int nQrCodeWidth;	//��ά����
	int nQrcCodeHeight;	//��ά��߶�
	string strContractId;	//��ͬ�ļ�ID
	string strUserId;	//�û�ID
	string strUrl;	//ɨ��ص���ַ
	string strExpireTime;	//��ά��ʧЧʱ��
	string strRemark;	//����ǩ�±�ע
	FileDetailList FileDetailList;
}ScanContractAddReq;

//��ѯ��ά��
typedef struct _ScanContractQueryReq
{
	string strPrepareId;	//Ԥ����ID
}ScanContractQueryReq;

//��ͬ���ֻ���
typedef struct _BindContractPhoneReq
{
	string strContractId;	//��ͬID
	vector<string> phoneList;  //�ֻ����б�
}BindContractPhoneReq;

//��ѯ��ǩ���ͬ
typedef struct _BindContractQueryReq
{
	string strPhone;	//�ֻ���
	string stContractName;	//��ͬ��,��������ѯ
}BindContractQueryReq;

//����ǩ��
typedef struct _SignCommonReq
{
	string strContractId;	//��ͬID
	string strRemark;	//����ǩ�±�ע
	string strUserId;	//�û�ID
	FileDetailList FileDetailList;
}SignCommonReq;

//�ؼ���ǩ��
typedef struct _SignKeywordReq
{
	bool bSignAll;	//true����ͬ������ƥ��λ��ȫ��ǩ��false��ֻǩ���һ��ƥ�䣻Ĭ��false
	double height;	//ӡ�¸߶�
	double width;	//ӡ�¿��
	string strContractId;	//��ͬID
	string strUserId;	//�û�ID
	string strKeyword;	//�ؼ���
	string strSealId;	//ӡ��ID
}SignKeywordReq;

//����ģ��ǩ��
typedef struct _SingleTemplateSignReq
{
	string strTemplateId;
	SignParamList SignParamList;
	TextParamList TextParamList;
}SingleTemplateSignReq;

//����ģ��ǩ��
typedef struct _BatchTemplateSignReq
{
	string strTemplateId;
	BatchSignList BatchSignList;
}BatchTemplateSignReq;

//��ǩ
typedef struct _SignMeetingSingleReq
{
	string strContractId;	//��ͬID
	MeetingSignList MeetingSignList;
}SignMeetingSingleReq;

//ɨ��ǩ��
typedef struct _SignScanReq
{
	string strPrepareId;	//Ԥ����ID
}SignScanReq;

//һ��ǩ��
typedef struct _SignOneStepAllReq
{
	string strUserName;	//�û���
	string strIdNum;	//ע���˵�֤����
	string strUserType;	//�û����ͣ�1���ˣ�2��ҵ��
	string strContractFileContent;	//��ͬ�ļ�����ʽpdf Base64
	FileContentDetailList FileContentDetailList;
}SignOneStepAllReq;

//��ͬ��ǩ���ļ�ID
typedef struct _SignCheckCommonReq
{
	string strFileId;	//��ͬ�ļ�ID
}SignCheckCommonReq;

//��ͬ��ǩ���ļ�ID
typedef struct _SignCheckFileReq
{
	string strFilePath;	//�ļ�·��
}SignCheckFileReq;