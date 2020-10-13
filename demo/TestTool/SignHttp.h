#pragma once

#include <Windows.h>
#include <string>
#include "RequestDefine.h"
#include "ResponseDefine.h"

using namespace std;

#ifndef SIGNHTTP
#define SIGNHTTP _declspec(dllexport)
#endif

//������
#define HTTP_SUCCESS                        0x00000000 // �ɹ�
#define HTTP_DATA_FAILED				    0x0A000001			
#define HTTP_PARSE_JSON_FAILED			    4001		//	������ȡ�쳣
#define HTTP_SYSTEM_EXCEPTION		        4002		//	ϵͳ�쳣
#define HTTP_PARAM_INCORRECT			    4003		//	��������
#define HTTP_SIGN_FAILED			        4004		//	ǩ��ʧ��
#define HTTP_AREACODE_NOT_SUPPORT		    4005		//	δ��ѯ����Ч�ĵ�������
#define HTTP_TEXT_VALUE_EMPTY			    4006		//	�ı���value����Ϊ��

#define HTTP_REGISTER_PERSONAL_FAILED	    4101		//	�����û�ע��ʧ��
#define HTTP_USER_NOT_EXIST			        4102		//	�û�������
#define HTTP_USER_STATUS_CANCELED		    4103		//	�û���ע��
#define HTTP_USER_STATUS_DISABLE		    4104		//	�û��ѱ�����
#define HTTP_USER_STATUS_EXCEPTION		    4105		//  �û�״̬�쳣
#define HTTP_USER_NUMBERID_EXISTS		    4106		//  ����֤�������ѱ�ռ��
#define HTTP_USER_OR_COMPANY_INVALID	    4107		//  ������Ч�ĸ����û�id����ҵ�û�id

#define HTTP_REGISTER_COMPANY_FAILED	    4201		//  ��ҵ�û�ע��ʧ��
#define HTTP_COMPANY_EXISTS	                4202		//  ��ҵ�����ѱ�ռ��
#define HTTP_LICNUM_EXIST				    4203		//	���ô����ѱ�ռ��

#define HTTP_REGISTER_CERT_FAILED		    4301		//	֤������ʧ��
#define HTTP_CERT_RECORD_NOMORE		        4302		//	֤��δ��ѯ�����������Ĳ���
#define HTTP_CERT_REGISTERING		        4303		//	֤������������
#define HTTP_CERT_CANCELED		            4304		//	֤���ѱ�ע��
#define HTTP_CERT_NOT_RECORD_EXIST		    4305		//	֤���ļ�δ��ѯ��
#define HTTP_CERT_EXPIRE		            4306		//	֤���ѹ���

#define HTTP_FILE_ID_EXISTS		            4401		//	�ļ�id�ѱ�ռ��
#define HTTP_FILE_UPLOAD_FAILED		        4402		//	�ļ��ϴ�ʧ��
#define HTTP_FILE_STATUS_LOCK			    4403		//	�ļ��ѱ�����
#define HTTP_FILE_NOT_EXIST				    4404		//	�ļ�������
#define HTTP_FILE_TYPE_ERROR		        4405		//	�ļ����Ͳ���ȷ

#define HTTP_CONTRACT_NOT_EXIST				4501		//	��ͬ������
#define HTTP_CONTRACT_ADD_FAILED	        4502		//	��ͬ���ʧ��
#define HTTP_CONTRACT_STATUS_INVALID	    4503		//	��ͬ״̬��Ч
#define HTTP_CONTRACT_ABOLISH	            4504		//	��ͬ�ѱ�����
#define HTTP_CONTRACT_NOT_RECORD_EXIST	    4505		//	��ͬ�ļ�δ��ѯ��
#define HTTP_CONTRACT_NOT_REMOVE		    4506		//	��ǩ���ͬ����ɾ��

#define HTTP_SEAL_ADD_FAILED	                    4601		//	ӡ�����ʧ��
#define HTTP_SEAL_NOT_RECORD_EXIST		            4602		//	ӡ��δ�ҵ�
#define HTTP_SEAL_STATUS_LOCK	                    4603		//	ӡ���ѱ�����

#define HTTP_TEMPLATE_ADD_FAILED	                4701		//	ģ�����ʧ��
#define HTTP_TEMPLATE_NOT_RECORD_EXIST	            4702		//	ģ��δ�ҵ�
#define HTTP_TEMPLATE_FILE_NOT_EXIST                4703		//	ģ���ļ�δ�ҵ�
#define HTTP_TEMPLATE_SGIN_DOMAIN_NOT_EXIST	        4704		//	ģ��ǩ�������δ�ҵ�
#define HTTP_TEMPLATE_STATUS_LOCK	                4705		//	ģ���ѱ�����
#define HTTP_TEMPLATE_TEXT_DOMAIN_PAR_NOMATCH	    4706		//	ģ�������������ƥ��
#define HTTP_TEMPLATE_SGIN_DOMAIN_PAR_NOMATCH	    4707		//	ģ��ǩ���������ƥ��

#define HTTP_ERROE_UNKOWN				100000		//	δ֪����


/*��ʼ����־*/
//SIGNHTTP int Http_InitLog(const char* szPath,const char* szFilename);

/*��ʼ��SDK*/
SIGNHTTP int Http_Init();

/*��ȡTOKEN*/
SIGNHTTP int Http_GetToken(AppInfo* pDataIn, TokenResponse* pDataOut);

/*��ȡע�������Ϣ������֤��*/
SIGNHTTP int Http_RegisterPersonalInfo(PersonalUserInfo* pDataIn, RegisterResponse* pDataOut);

/*��ȡ�û�������Ϣ*/
SIGNHTTP int Http_GetPersonalUserInfo(UserIdInfoReq* pDataIn, GetPersonalUserInfoResponse* pDataOut);

/*�û�״̬����*/
SIGNHTTP int Http_PersonalUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*�û�״̬����*/
SIGNHTTP int Http_PersonalLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*ע����ҵ��Ϣ������֤��*/
SIGNHTTP int Http_RegisterCompanyInfo(CompanyInfo* pDataIn, RegisterResponse* pDataOut);

/*��ȡ��ҵ������Ϣ*/
SIGNHTTP int Http_GetCompanyUserInfo(UserIdInfoReq* pDataIn, GetCompanyInfoResponse* pDataOut);

/*��ҵ״̬����*/
SIGNHTTP int Http_CompanyUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*��ҵ״̬����*/
SIGNHTTP int Http_CompanyLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*֤���ѯ*/
SIGNHTTP int Http_GetCertinfo(CertInfoReq* pDataIn, GetCertInfoResponse* pDataOut);

/*�ϴ�ӡģ�����ӡ��*/
SIGNHTTP int Http_UploadFileAndSealInfo(UploadAndAddSealInfoReq* pDataIn, UploadAndAddSealInfoResponse* pDataOut);

/*��ѯӡ�½ӿ�*/
SIGNHTTP int Http_QuerySealInfo(SealIdInfoReq* pDataIn, SealInfo* pDataOut);

/*��ѯ�û����е�ӡ��*/
SIGNHTTP int Http_GetSealInfosFromUser(UserIdInfoReq* pDataIn, SealInfoList* pDataOut);

/*ע��ӡ��*/
SIGNHTTP int Http_RemoveSealInfo(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*ӡ��״̬����*/
SIGNHTTP int Http_LockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*ӡ��״̬����*/
SIGNHTTP int Http_UnLockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*�����ļ�*/
SIGNHTTP int Http_DownloadFile(DownloadFileInfo* pDataIn);

/*�ϴ�ģ�沢���ģ��*/
SIGNHTTP int Http_UploadFileAndTemplateInfo(FileInfo* pDataIn, UploadAndAddTemplateInfoResponse* pDataOut);

/*��ȡģ����Ϣ*/
SIGNHTTP int Http_QueryTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateInfo* pDataOut);

/*ɾ��ģ��ӿ�*/
SIGNHTTP int Http_DeleteTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*ģ��״̬����*/
SIGNHTTP int Http_LockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*ģ��״̬����*/
SIGNHTTP int Http_UnLockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*�ϴ���ͬ����Ӻ�ͬ*/
SIGNHTTP int Http_UploadFileAndContractInfo(FileInfo* pDataIn, UploadAndAddContractInfoResponse* pDataOut);

/*��ѯ��ͬ�ӿ�*/
SIGNHTTP int Http_QueryContractInfo(ContractIdInfoReq* pDataIn, ContractInfo* pDataOut);

/*ɾ����ͬ�ӿ�*/
SIGNHTTP int Http_DeleteContractInfo(ContractIdInfoReq* pDataIn, ContractHandleResponse* pDataOut);

/*���Ϻ�ͬ�ӿ�*/
SIGNHTTP int Http_AbolishContractInfo(ContractAbolishReq* pDataIn, ContractAbolishResponse* pDataOut);

/*��ͬ�첽��Ⱦ�ӿ�*/
SIGNHTTP int Http_ContractRender(ContractIdInfoReq* pDataIn, ContractRenderResponse* pDataOut);

/*���Ӷ�ά���ͬ�ӿ�*/
SIGNHTTP int Http_ScanContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut);

/*���Ӷ�ά���ͬ�ӿ�֤���¼�*/
SIGNHTTP int Http_ScanCertContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut);

/*��ѯ��ά���ͬ*/
SIGNHTTP int Http_QueryScanContract(ScanContractQueryReq* pDataIn, ScanContractInfo* pDataOut);

/*��ͬ���ֻ���*/
SIGNHTTP int Http_BindContractPhone(BindContractPhoneReq* pDataIn, BindContractPhoneResponse* pDataOut);

/*��ѯ��ǩ���ͬ*/
SIGNHTTP int Http_QueryBindContract(BindContractQueryReq* pDataIn, BindContractInfo* pDataOut);

/*��ͬ����ǩ��*/
SIGNHTTP int Http_SignCommon(SignCommonReq* pDataIn, SignBackInfo* pDataOut);

/*��ͬ�ؼ���ǩ��*/
SIGNHTTP int Http_SignKeywordSign(SignKeywordReq* pDataIn, SignBackInfo* pDataOut);

/*����ģ��ǩ��*/
SIGNHTTP int Http_SingleTemplateSign(SingleTemplateSignReq* pDataIn, SignBackInfo* pDataOut);

/*����ģ��ǩ��*/
SIGNHTTP int Http_BatchTemplateSign(BatchTemplateSignReq* pDataIn, BatchTemplateSignResponse* pDataOut);

/*��ǩ�ӿ�*/
SIGNHTTP int Http_SignMeetingSingle(SignMeetingSingleReq* pDataIn, SignBackInfo* pDataOut);

/*ɨ��ǩ��*/
SIGNHTTP int Http_SignScanSign(SignScanReq* pDataIn, SignBackInfo* pDataOut);

/*��ͬ����ǩ��-֤���¼�*/
SIGNHTTP int Http_SignCommonEvent(SignCommonReq* pDataIn, SignEventBackResponse* pDataOut);

/*��ͬ�ؼ���ǩ��-֤���¼�*/
SIGNHTTP int Http_SignKeywordSignEvent(SignKeywordReq* pDataIn, SignEventBackResponse* pDataOut);

/*����ģ��ǩ��-֤���¼�*/
SIGNHTTP int Http_SingleTemplateSignEvent(SingleTemplateSignReq* pDataIn, SignEventBackResponse* pDataOut);

/*����ģ��ǩ��-֤���¼�*/
SIGNHTTP int Http_BatchTemplateSignEvent(BatchTemplateSignReq* pDataIn, BatchSignEventBackResponse* pDataOut);

/*��ǩ�ӿ�-֤���¼�*/
SIGNHTTP int Http_SignMeetingSingleEvent(SignMeetingSingleReq* pDataIn, SignEventBackResponse* pDataOut);

/*ɨ��ǩ��-֤���¼�*/
SIGNHTTP int Http_SignScanSignEvent(SignScanReq* pDataIn, SignEventBackResponse* pDataOut);

/*һ��ǩ��*/
SIGNHTTP int Http_SignOneStepAll(SignOneStepAllReq* pDataIn, SignOneStepAllResponse* pDataOut);

/*��ͬ��ǩ-�ļ�id��ǩ*/
SIGNHTTP int Http_SignCheckCommon(SignCheckCommonReq* pDataIn, SignCheckResponse* pDataOut);

/*��ͬ��ǩ-�ļ���ǩ*/
SIGNHTTP int Http_SignCheckFile(SignCheckFileReq* pDataIn, SignCheckResponse* pDataOut);