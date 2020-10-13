#pragma once

#include <Windows.h>
#include <string>
#include "RequestDefine.h"
#include "ResponseDefine.h"

using namespace std;

#ifndef SIGNHTTP
#define SIGNHTTP _declspec(dllexport)
#endif

//错误码
#define HTTP_SUCCESS                        0x00000000 // 成功
#define HTTP_DATA_FAILED				    0x0A000001			
#define HTTP_PARSE_JSON_FAILED			    4001		//	参数读取异常
#define HTTP_SYSTEM_EXCEPTION		        4002		//	系统异常
#define HTTP_PARAM_INCORRECT			    4003		//	参数有误
#define HTTP_SIGN_FAILED			        4004		//	签章失败
#define HTTP_AREACODE_NOT_SUPPORT		    4005		//	未查询到有效的地区编码
#define HTTP_TEXT_VALUE_EMPTY			    4006		//	文本域value不能为空

#define HTTP_REGISTER_PERSONAL_FAILED	    4101		//	个人用户注册失败
#define HTTP_USER_NOT_EXIST			        4102		//	用户不存在
#define HTTP_USER_STATUS_CANCELED		    4103		//	用户已注销
#define HTTP_USER_STATUS_DISABLE		    4104		//	用户已被禁用
#define HTTP_USER_STATUS_EXCEPTION		    4105		//  用户状态异常
#define HTTP_USER_NUMBERID_EXISTS		    4106		//  个人证件号码已被占用
#define HTTP_USER_OR_COMPANY_INVALID	    4107		//  不是有效的个人用户id或企业用户id

#define HTTP_REGISTER_COMPANY_FAILED	    4201		//  企业用户注册失败
#define HTTP_COMPANY_EXISTS	                4202		//  企业名称已被占用
#define HTTP_LICNUM_EXIST				    4203		//	信用代码已被占用

#define HTTP_REGISTER_CERT_FAILED		    4301		//	证书申请失败
#define HTTP_CERT_RECORD_NOMORE		        4302		//	证书未查询到，请检查您的参数
#define HTTP_CERT_REGISTERING		        4303		//	证书正在申请中
#define HTTP_CERT_CANCELED		            4304		//	证书已被注销
#define HTTP_CERT_NOT_RECORD_EXIST		    4305		//	证书文件未查询到
#define HTTP_CERT_EXPIRE		            4306		//	证书已过期

#define HTTP_FILE_ID_EXISTS		            4401		//	文件id已被占用
#define HTTP_FILE_UPLOAD_FAILED		        4402		//	文件上传失败
#define HTTP_FILE_STATUS_LOCK			    4403		//	文件已被锁定
#define HTTP_FILE_NOT_EXIST				    4404		//	文件不存在
#define HTTP_FILE_TYPE_ERROR		        4405		//	文件类型不正确

#define HTTP_CONTRACT_NOT_EXIST				4501		//	合同不存在
#define HTTP_CONTRACT_ADD_FAILED	        4502		//	合同添加失败
#define HTTP_CONTRACT_STATUS_INVALID	    4503		//	合同状态无效
#define HTTP_CONTRACT_ABOLISH	            4504		//	合同已被作废
#define HTTP_CONTRACT_NOT_RECORD_EXIST	    4505		//	合同文件未查询到
#define HTTP_CONTRACT_NOT_REMOVE		    4506		//	已签署合同不能删除

#define HTTP_SEAL_ADD_FAILED	                    4601		//	印章添加失败
#define HTTP_SEAL_NOT_RECORD_EXIST		            4602		//	印章未找到
#define HTTP_SEAL_STATUS_LOCK	                    4603		//	印章已被锁定

#define HTTP_TEMPLATE_ADD_FAILED	                4701		//	模板添加失败
#define HTTP_TEMPLATE_NOT_RECORD_EXIST	            4702		//	模板未找到
#define HTTP_TEMPLATE_FILE_NOT_EXIST                4703		//	模板文件未找到
#define HTTP_TEMPLATE_SGIN_DOMAIN_NOT_EXIST	        4704		//	模板签名域参数未找到
#define HTTP_TEMPLATE_STATUS_LOCK	                4705		//	模板已被锁定
#define HTTP_TEMPLATE_TEXT_DOMAIN_PAR_NOMATCH	    4706		//	模板文字域参数不匹配
#define HTTP_TEMPLATE_SGIN_DOMAIN_PAR_NOMATCH	    4707		//	模板签名域参数不匹配

#define HTTP_ERROE_UNKOWN				100000		//	未知错误


/*初始化日志*/
//SIGNHTTP int Http_InitLog(const char* szPath,const char* szFilename);

/*初始化SDK*/
SIGNHTTP int Http_Init();

/*获取TOKEN*/
SIGNHTTP int Http_GetToken(AppInfo* pDataIn, TokenResponse* pDataOut);

/*获取注册个人信息并申请证书*/
SIGNHTTP int Http_RegisterPersonalInfo(PersonalUserInfo* pDataIn, RegisterResponse* pDataOut);

/*获取用户详情信息*/
SIGNHTTP int Http_GetPersonalUserInfo(UserIdInfoReq* pDataIn, GetPersonalUserInfoResponse* pDataOut);

/*用户状态解锁*/
SIGNHTTP int Http_PersonalUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*用户状态锁定*/
SIGNHTTP int Http_PersonalLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*注册企业信息并申请证书*/
SIGNHTTP int Http_RegisterCompanyInfo(CompanyInfo* pDataIn, RegisterResponse* pDataOut);

/*获取企业详情信息*/
SIGNHTTP int Http_GetCompanyUserInfo(UserIdInfoReq* pDataIn, GetCompanyInfoResponse* pDataOut);

/*企业状态解锁*/
SIGNHTTP int Http_CompanyUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*企业状态锁定*/
SIGNHTTP int Http_CompanyLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut);

/*证书查询*/
SIGNHTTP int Http_GetCertinfo(CertInfoReq* pDataIn, GetCertInfoResponse* pDataOut);

/*上传印模并添加印章*/
SIGNHTTP int Http_UploadFileAndSealInfo(UploadAndAddSealInfoReq* pDataIn, UploadAndAddSealInfoResponse* pDataOut);

/*查询印章接口*/
SIGNHTTP int Http_QuerySealInfo(SealIdInfoReq* pDataIn, SealInfo* pDataOut);

/*查询用户所有的印章*/
SIGNHTTP int Http_GetSealInfosFromUser(UserIdInfoReq* pDataIn, SealInfoList* pDataOut);

/*注销印章*/
SIGNHTTP int Http_RemoveSealInfo(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*印章状态锁定*/
SIGNHTTP int Http_LockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*印章状态解锁*/
SIGNHTTP int Http_UnLockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut);

/*下载文件*/
SIGNHTTP int Http_DownloadFile(DownloadFileInfo* pDataIn);

/*上传模版并添加模版*/
SIGNHTTP int Http_UploadFileAndTemplateInfo(FileInfo* pDataIn, UploadAndAddTemplateInfoResponse* pDataOut);

/*获取模版信息*/
SIGNHTTP int Http_QueryTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateInfo* pDataOut);

/*删除模版接口*/
SIGNHTTP int Http_DeleteTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*模版状态锁定*/
SIGNHTTP int Http_LockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*模版状态解锁*/
SIGNHTTP int Http_UnLockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut);

/*上传合同并添加合同*/
SIGNHTTP int Http_UploadFileAndContractInfo(FileInfo* pDataIn, UploadAndAddContractInfoResponse* pDataOut);

/*查询合同接口*/
SIGNHTTP int Http_QueryContractInfo(ContractIdInfoReq* pDataIn, ContractInfo* pDataOut);

/*删除合同接口*/
SIGNHTTP int Http_DeleteContractInfo(ContractIdInfoReq* pDataIn, ContractHandleResponse* pDataOut);

/*作废合同接口*/
SIGNHTTP int Http_AbolishContractInfo(ContractAbolishReq* pDataIn, ContractAbolishResponse* pDataOut);

/*合同异步渲染接口*/
SIGNHTTP int Http_ContractRender(ContractIdInfoReq* pDataIn, ContractRenderResponse* pDataOut);

/*增加二维码合同接口*/
SIGNHTTP int Http_ScanContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut);

/*增加二维码合同接口证书事件*/
SIGNHTTP int Http_ScanCertContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut);

/*查询二维码合同*/
SIGNHTTP int Http_QueryScanContract(ScanContractQueryReq* pDataIn, ScanContractInfo* pDataOut);

/*合同绑定手机号*/
SIGNHTTP int Http_BindContractPhone(BindContractPhoneReq* pDataIn, BindContractPhoneResponse* pDataOut);

/*查询待签署合同*/
SIGNHTTP int Http_QueryBindContract(BindContractQueryReq* pDataIn, BindContractInfo* pDataOut);

/*合同坐标签章*/
SIGNHTTP int Http_SignCommon(SignCommonReq* pDataIn, SignBackInfo* pDataOut);

/*合同关键字签章*/
SIGNHTTP int Http_SignKeywordSign(SignKeywordReq* pDataIn, SignBackInfo* pDataOut);

/*单次模版签章*/
SIGNHTTP int Http_SingleTemplateSign(SingleTemplateSignReq* pDataIn, SignBackInfo* pDataOut);

/*批量模版签章*/
SIGNHTTP int Http_BatchTemplateSign(BatchTemplateSignReq* pDataIn, BatchTemplateSignResponse* pDataOut);

/*会签接口*/
SIGNHTTP int Http_SignMeetingSingle(SignMeetingSingleReq* pDataIn, SignBackInfo* pDataOut);

/*扫描签章*/
SIGNHTTP int Http_SignScanSign(SignScanReq* pDataIn, SignBackInfo* pDataOut);

/*合同坐标签章-证书事件*/
SIGNHTTP int Http_SignCommonEvent(SignCommonReq* pDataIn, SignEventBackResponse* pDataOut);

/*合同关键字签章-证书事件*/
SIGNHTTP int Http_SignKeywordSignEvent(SignKeywordReq* pDataIn, SignEventBackResponse* pDataOut);

/*单次模版签章-证书事件*/
SIGNHTTP int Http_SingleTemplateSignEvent(SingleTemplateSignReq* pDataIn, SignEventBackResponse* pDataOut);

/*批量模版签章-证书事件*/
SIGNHTTP int Http_BatchTemplateSignEvent(BatchTemplateSignReq* pDataIn, BatchSignEventBackResponse* pDataOut);

/*会签接口-证书事件*/
SIGNHTTP int Http_SignMeetingSingleEvent(SignMeetingSingleReq* pDataIn, SignEventBackResponse* pDataOut);

/*扫描签章-证书事件*/
SIGNHTTP int Http_SignScanSignEvent(SignScanReq* pDataIn, SignEventBackResponse* pDataOut);

/*一步签章*/
SIGNHTTP int Http_SignOneStepAll(SignOneStepAllReq* pDataIn, SignOneStepAllResponse* pDataOut);

/*合同验签-文件id验签*/
SIGNHTTP int Http_SignCheckCommon(SignCheckCommonReq* pDataIn, SignCheckResponse* pDataOut);

/*合同验签-文件验签*/
SIGNHTTP int Http_SignCheckFile(SignCheckFileReq* pDataIn, SignCheckResponse* pDataOut);