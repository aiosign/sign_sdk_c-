#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

//֤��ṹ��
typedef struct _CertInfo
{
	int nStatus;
	int nVersion;
	string strAwardTo;	//֤��ʹ��������
	string strCertId;	//֤��id
	string strCertName;	//֤������
	string strEndDate;	//֤��ʧЧʱ��
	string strStartDate;	//��ʼʱ��
	string strNumberId;	//֤��֤����
	string strIssuer;	//�䷢��
	string strCertSn;	//֤�����к�
	_CertInfo() : nStatus(1) {};
}CertInfo;

//�����û��ṹ��
typedef struct _PersonalUserInfo
{
	int nStatus;
	string strUserId;	//�û�ID
	string strUserName;	//�û���
	string strAreaCode;	//��������
	string strPhone;	//�ֻ�����
	string strTypeId;	//֤������ 1,���֤;2,����;3,��ʻ֤;4, ��ס֤
	string strNumberId;	//֤����
	string strMail;	//����
	string strDescription;	//����
	_PersonalUserInfo() : nStatus(1) {};
}PersonalUserInfo;

//��ҵ�ṹ��
typedef struct _CompanyInfo
{
	int nStatus;
	string strUserId;	//�û�ID
	string strUserName;	//�û���
	string strAreaCode;	//��������
	string strUnitType;	//��ҵ����
	string strCrediCode;	//ͳһ������ô���
	string strLegalName;	//��������
	string strLegalIdNum;	//����֤����
	string strLegalIdType;	//����֤������
	string strLegalPhone;	//�����ֻ�����
	string strLegalMail;	//��������
	string strAgentName;	//����������
	string strAgentIdNum;	//������֤����
	string strAgentIdType;	//������֤������
	string strAgentPhone;	//�������ֻ�����
	string strAgentMail;	//����������
	string strDescription;	//����
	_CompanyInfo() : nStatus(1) {};
}CompanyInfo;

//�ļ���Ϣ
typedef struct _FileInfo
{
	int nStatus;
	string strFilePath;	//�ļ�·��
	string strFileName;	//�ļ���
	string strFileType;	//�ļ�����
	string strUserId;	//�û�ID
	string strFileId;	//�ļ�ID
	string strDescription;	//����
	_FileInfo() : nStatus(1) {};
}FileInfo;

//ӡ����Ϣ
typedef struct _SealInfo
{
	int nStatus;
	string strFileId;	//�ļ�ID
	string strSealName;	//ӡ������
	string strSealType;	//ӡ������
	string strSealSize;	//ӡ�¹��
	string strDescription;	//����
	string strUserId;	//�û�ID
	_SealInfo() : nStatus(1) {}
}SealInfo;

typedef vector<SealInfo>SealInfoList;

//ģ����Ϣ
typedef struct _TemplateInfo
{
	int nStatus;
	string strTemplateId;	//ģ��ID
	string strFileId;	//�ļ�ID
	string strTemplateName;	//����
	string strCreateTime;	//����ʱ��
	string strSignParams;	//ǩ����
	string strTextParamsn;	//������
	_TemplateInfo() : nStatus(1) {}
}TemplateInfo;

//��ͬ��ϸ����
typedef struct _ContractDetail
{
	int nNumber;	//�ڼ�ҳ
	string strImageFileId;	//ͼƬID
	_ContractDetail() : nNumber(0) {}
}ContractDetail;

typedef vector<ContractDetail>ContractDetailList;

//��ͬ��Ϣ
typedef struct _ContractInfo
{
	int nStatus;
	int nPageCount; //��ͬҳ��
	int nRenderComplete; //�Ƿ���Ⱦ���:0��Ⱦʧ��1��Ⱦ�ɹ�2��Ⱦ��
	int nSize; //�ļ���С
	string strContractId;	//��ͬID
	string strFileId;	//�ļ�ID
	string strContractName;	//����
	string strCreateTime;	//����ʱ��
	ContractDetailList detailList;
	_ContractInfo() : nStatus(1), nPageCount(0), nRenderComplete(1), nSize(0){}
}ContractInfo;

//ǩ������
typedef struct _FileDetailItem
{
	double height;	//ӡ�¸߶�
	double width;	//ӡ�¿��
	double horizontal;	//ˮƽ������
	double vertical;	//��ֱ������
	int nPageNumber;	//ǩ��ҳ��
	string strSealId;	//ӡ��ID
	string strFileId;	//�ļ�ID
	_FileDetailItem() : nPageNumber(0) {}
}FileDetailItem;

typedef vector<FileDetailItem>FileDetailList;

//��ά���ͬ��Ϣ
typedef struct _ScanContractInfo
{
	bool bQrCodeState;	//��ά���Ƿ����
	bool bSignState;	//ǩ��״̬
	int nStatus;
	int nPageCount; //��ͬҳ��
	int nRenderComplete; //�Ƿ���Ⱦ���:0��Ⱦʧ��1��Ⱦ�ɹ�2��Ⱦ��
	int nSize; //�ļ���С
	string strContractId;	//��ͬID
	string strFileId;	//�ļ�ID
	string strContractName;	//����
	string strUserId;	//�û�ID
	FileDetailList FileDetailList;
	ContractDetailList detailList;
	_ScanContractInfo() : bQrCodeState(false), bSignState(false),nStatus(1), nPageCount(0), nRenderComplete(1), nSize(0) {}
}ScanContractInfo;

//��ǩ��ĺ�ͬ��
typedef struct _BindContractItem
{
	int nStatus;
	string strId;	//���ݿ�ID
	string strContractId;	//��ͬID
	string strContractName;	//����
	string strContractCreateTime;	//��ͬ����ʱ��
	string strUniqueIdentifier;	//Ψһ��ʶ
	string strAppId;	//APPID
	string strCreateTime;	//�ֻ���ʱ��
	string strUpdateTime;	//�ֻ�����ʱ��
	_BindContractItem() : nStatus(1) {}
}BindContractItem;

typedef vector<BindContractItem>BindContractList;


//���ֻ��ź�ͬ��Ϣ
typedef struct _BindContractInfo
{
	BindContractList BindContractList;
}BindContractInfo;

//ǩ�º󷵻صĽṹ
typedef struct _SignBackInfo
{
	bool bSignState;	//ǩ����
	string strSignId;	//����ǩ��ID
	string strFileId;	//�ļ�����ID
	string strHash;	//hashֵ
}SignBackInfo;

//ģ������ǩ�º󷵻صĽṹ
typedef struct _BatchSignBackItem
{
	bool bSignState;	//ǩ����
	string strCustomId;	//�Զ����
	string strSignId;	//����ǩ��ID
	string strFileId;	//�ļ�����ID
	string strHash;	//hashֵ
}BatchSignBackItem;
typedef vector<BatchSignBackItem>BatchSignBackList;

//ǩ������
typedef struct _SignParamItem
{
	bool bIsPicture;	//�Ƿ�Ϊͼ����� true:Ϊͼ����Ϣfalse:ǩ��ͼƬ��Ĭ�ϣ�
	double height;	//ӡ�¸߶�
	double width;	//ӡ�¿��
	string strSealId;	//ӡ��ID
	string strSignKey;	//ǩ����
	string strUserId;	//�û�ID
}SignParamItem;
typedef vector<SignParamItem>SignParamList;

//��������
typedef struct _TextParamItem
{
	string strKey;	//������key
	string strValue;	//���ֵ
}TextParamItem;
typedef vector<TextParamItem>TextParamList;

//����ģ��ǩ����
typedef struct _BatchSignItem
{
	string strcCustomId;	//�û��Զ���ID
	SignParamList SignParamList;
	TextParamList TextParamList;
}BatchSignItem;
typedef vector<BatchSignItem>BatchSignList;

//��ǩ�ӿ���
typedef struct _MeetingSignItem
{
	int nPageNum;	//ǩ��ҳ��
	int nSignLeft;	//ǩ�������
	int nSignTop;	//ǩ���Ͼ���
	double height;	//ӡ�¸߶�
	double width;	//ӡ�¿��
	string strSealId;	//ӡ��ID
	string strUserId;	//�û�ID
}MeetingSignItem;
typedef vector<MeetingSignItem>MeetingSignList;

//֤���¼�
typedef struct _SignCertEventItem
{
	string strUserId;	//�û�ID
	string strCertId;	//֤��ID
}SignCertEventItem;
typedef vector<SignCertEventItem>SignCertEventList;

//ǩ������--һ��ǩ�´���ǩ������
typedef struct _FileContentDetailItem
{
	double height;	//ӡ�¸߶�
	double width;	//ӡ�¿��
	double horizontal;	//ˮƽ������
	double vertical;	//��ֱ������
	int nPageNumber;	//ǩ��ҳ��
	string strSealFileContent;	//ӡ���ļ�����Base64
	_FileContentDetailItem() : nPageNumber(0) {}
}FileContentDetailItem;
typedef vector<FileContentDetailItem>FileContentDetailList;

//��ǩ���
typedef struct _SignCheckResultsItem
{
	bool bResult;	//ǩ����
	int nPageNumber;	//ǩ��ҳ��
	string strCertSn;	//֤�����к�
	string strSignTime;	//ǩ��ʱ��
	string strTimestamp;	//ʱ�������
	string strSigner;	//ǩ����
	_SignCheckResultsItem() : nPageNumber(0) {}
}SignCheckResultsItem;
typedef vector<SignCheckResultsItem>SignCheckResultsList;

//�����ļ�
typedef struct _DownloadFileInfo
{
	string strFileId;	//�ļ�ID
	string strFilePath;	//�ļ�����·��
}DownloadFileInfo;