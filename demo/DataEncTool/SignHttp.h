#pragma once

#include <Windows.h>
#include <string>

using namespace std;

#ifndef SIGNHTTP
#define SIGNHTTP _declspec(dllexport)
#endif

//������

/*��ʼ����־*/
SIGNHTTP int Http_InitLog(const char* szPath,const char* szFilename);

///*��ȡTOKEN*/
//SIGNHTTP int Http_GetToken(const char* strUrl, string& jsonvalue);

/*��ȡTOKEN*/
SIGNHTTP int Http_GetToken(const char* szUrl, const char* szJsonData, string& strData);

/*��ȡע�������Ϣ������֤��*/
SIGNHTTP int Http_RegisterPersonalInfo(const char* strUrl,const char* szKey, const char* szJsonData, string& strData);

/*��ȡ�û�������Ϣ*/
SIGNHTTP int Http_GetPersonalUserInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*�û�״̬����*/
SIGNHTTP int Http_PersonalUnlock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*�û�״̬����*/
SIGNHTTP int Http_PersonalLock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ע����ҵ��Ϣ������֤��*/
SIGNHTTP int Http_RegisterCompanyInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ȡ��ҵ������Ϣ*/
SIGNHTTP int Http_GetCompanyUserInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ҵ״̬����*/
SIGNHTTP int Http_CompanyUnlock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ҵ״̬����*/
SIGNHTTP int Http_CompanyLock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*֤���ѯ*/
SIGNHTTP int Http_GetCertinfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*�ϴ�ӡģ�����ӡ��*/
SIGNHTTP int Http_UploadFileAndSealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ѯӡ�½ӿ�*/
SIGNHTTP int Http_QuerySealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ѯ�û����е�ӡ��*/
SIGNHTTP int Http_GetSealInfosFromUser(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ע��ӡ��*/
SIGNHTTP int Http_RemoveSealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ӡ��״̬����*/
SIGNHTTP int Http_LockSealStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ӡ��״̬����*/
SIGNHTTP int Http_UnLockSealStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*�����ļ�*/
SIGNHTTP int Http_DownloadFile(const char* strUrl, const char* szPath);

/*�ϴ�ģ�沢���ģ��*/
SIGNHTTP int Http_UploadFileAndTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ȡģ����Ϣ*/
SIGNHTTP int Http_QueryTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ɾ��ģ��ӿ�*/
SIGNHTTP int Http_DeleteTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ģ��״̬����*/
SIGNHTTP int Http_LockTemplateStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ģ��״̬����*/
SIGNHTTP int Http_UnLockTemplateStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*�ϴ���ͬ����Ӻ�ͬ*/
SIGNHTTP int Http_UploadFileAndContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ѯ��ͬ�ӿ�*/
SIGNHTTP int Http_QueryContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ɾ����ͬ�ӿ�*/
SIGNHTTP int Http_DeleteContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*���Ϻ�ͬ�ӿ�*/
SIGNHTTP int Http_AbolishContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ�첽��Ⱦ�ӿ�*/
SIGNHTTP int Http_ContractRender(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*���Ӷ�ά���ͬ�ӿ�*/
SIGNHTTP int Http_ScanContractAdd(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*���Ӷ�ά���ͬ�ӿ�֤���¼�*/
SIGNHTTP int Http_ScanCertContractAdd(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ѯ��ά���ͬ*/
SIGNHTTP int Http_QueryScanContract(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ���ֻ���*/
SIGNHTTP int Http_BindContractPhone(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ѯ��ǩ���ͬ*/
SIGNHTTP int Http_QueryBindContract(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ����ǩ��*/
SIGNHTTP int Http_SignCommon(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ�ؼ���ǩ��*/
SIGNHTTP int Http_SignKeywordSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*����ģ��ǩ��*/
SIGNHTTP int Http_SingleTemplateSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*����ģ��ǩ��*/
SIGNHTTP int Http_BatchTemplateSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ǩ�ӿ�*/
SIGNHTTP int Http_SignMeetingSingle(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ɨ��ǩ��*/
SIGNHTTP int Http_SignScanSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ����ǩ��-֤���¼�*/
SIGNHTTP int Http_SignCommonEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ�ؼ���ǩ��-֤���¼�*/
SIGNHTTP int Http_SignKeywordSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*����ģ��ǩ��-֤���¼�*/
SIGNHTTP int Http_SingleTemplateSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*����ģ��ǩ��-֤���¼�*/
SIGNHTTP int Http_BatchTemplateSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ǩ�ӿ�-֤���¼�*/
SIGNHTTP int Http_SignMeetingSingleEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*ɨ��ǩ��-֤���¼�*/
SIGNHTTP int Http_SignScanSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*һ��ǩ��*/
SIGNHTTP int Http_SignOneStepAll(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ��ǩ-�ļ�id��ǩ*/
SIGNHTTP int Http_SignCheckCommon(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*��ͬ��ǩ-�ļ���ǩ*/
SIGNHTTP int Http_SignCheckFile(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);