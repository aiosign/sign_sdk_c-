#pragma once

#include <Windows.h>
#include <string>

using namespace std;

#ifndef SIGNHTTP
#define SIGNHTTP _declspec(dllexport)
#endif

//错误码

/*初始化日志*/
SIGNHTTP int Http_InitLog(const char* szPath,const char* szFilename);

///*获取TOKEN*/
//SIGNHTTP int Http_GetToken(const char* strUrl, string& jsonvalue);

/*获取TOKEN*/
SIGNHTTP int Http_GetToken(const char* szUrl, const char* szJsonData, string& strData);

/*获取注册个人信息并申请证书*/
SIGNHTTP int Http_RegisterPersonalInfo(const char* strUrl,const char* szKey, const char* szJsonData, string& strData);

/*获取用户详情信息*/
SIGNHTTP int Http_GetPersonalUserInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*用户状态解锁*/
SIGNHTTP int Http_PersonalUnlock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*用户状态锁定*/
SIGNHTTP int Http_PersonalLock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*注册企业信息并申请证书*/
SIGNHTTP int Http_RegisterCompanyInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*获取企业详情信息*/
SIGNHTTP int Http_GetCompanyUserInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*企业状态解锁*/
SIGNHTTP int Http_CompanyUnlock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*企业状态锁定*/
SIGNHTTP int Http_CompanyLock(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*证书查询*/
SIGNHTTP int Http_GetCertinfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*上传印模并添加印章*/
SIGNHTTP int Http_UploadFileAndSealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*查询印章接口*/
SIGNHTTP int Http_QuerySealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*查询用户所有的印章*/
SIGNHTTP int Http_GetSealInfosFromUser(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*注销印章*/
SIGNHTTP int Http_RemoveSealInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*印章状态锁定*/
SIGNHTTP int Http_LockSealStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*印章状态解锁*/
SIGNHTTP int Http_UnLockSealStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*下载文件*/
SIGNHTTP int Http_DownloadFile(const char* strUrl, const char* szPath);

/*上传模版并添加模版*/
SIGNHTTP int Http_UploadFileAndTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*获取模版信息*/
SIGNHTTP int Http_QueryTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*删除模版接口*/
SIGNHTTP int Http_DeleteTemplateInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*模版状态锁定*/
SIGNHTTP int Http_LockTemplateStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*模版状态解锁*/
SIGNHTTP int Http_UnLockTemplateStatus(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*上传合同并添加合同*/
SIGNHTTP int Http_UploadFileAndContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*查询合同接口*/
SIGNHTTP int Http_QueryContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*删除合同接口*/
SIGNHTTP int Http_DeleteContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*作废合同接口*/
SIGNHTTP int Http_AbolishContractInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同异步渲染接口*/
SIGNHTTP int Http_ContractRender(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*增加二维码合同接口*/
SIGNHTTP int Http_ScanContractAdd(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*增加二维码合同接口证书事件*/
SIGNHTTP int Http_ScanCertContractAdd(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*查询二维码合同*/
SIGNHTTP int Http_QueryScanContract(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同绑定手机号*/
SIGNHTTP int Http_BindContractPhone(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*查询待签署合同*/
SIGNHTTP int Http_QueryBindContract(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同坐标签章*/
SIGNHTTP int Http_SignCommon(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同关键字签章*/
SIGNHTTP int Http_SignKeywordSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*单次模版签章*/
SIGNHTTP int Http_SingleTemplateSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*批量模版签章*/
SIGNHTTP int Http_BatchTemplateSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*会签接口*/
SIGNHTTP int Http_SignMeetingSingle(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*扫描签章*/
SIGNHTTP int Http_SignScanSign(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同坐标签章-证书事件*/
SIGNHTTP int Http_SignCommonEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同关键字签章-证书事件*/
SIGNHTTP int Http_SignKeywordSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*单次模版签章-证书事件*/
SIGNHTTP int Http_SingleTemplateSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*批量模版签章-证书事件*/
SIGNHTTP int Http_BatchTemplateSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*会签接口-证书事件*/
SIGNHTTP int Http_SignMeetingSingleEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*扫描签章-证书事件*/
SIGNHTTP int Http_SignScanSignEvent(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*一步签章*/
SIGNHTTP int Http_SignOneStepAll(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同验签-文件id验签*/
SIGNHTTP int Http_SignCheckCommon(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);

/*合同验签-文件验签*/
SIGNHTTP int Http_SignCheckFile(const char* strUrl, const char* szKey, const char* szJsonData, string& strData);