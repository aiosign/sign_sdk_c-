#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

typedef struct _TokenResponse
{
	int nExpiresIn;	//过期时间
	string strAccessToken;	//用户令牌
	string strScope;	//授权类型
	string strTokenType;	//Token类型
}TokenResponse;

typedef struct _RegisterResponse
{
	string strUserId;	//用户ID
}RegisterResponse;

//查询个人用户信息
typedef struct _GetPersonalUserInfoResponse
{
	CertInfo certInfo;
	PersonalUserInfo personalUserInfo;
}GetPersonalUserInfoResponse;

//用户处理返回
typedef struct _UserInfoHandleResponse
{
	bool bResult;
	string strUserId;	//用户ID
	_UserInfoHandleResponse() : bResult(false) {}
}UserInfoHandleResponse;

//查询企业用户信息
typedef struct _GetCompanyInfoResponse
{
	CertInfo certInfo;
	CompanyInfo CompanyInfo;
}GetCompanyInfoResponse;

//查询证书返回
typedef struct _GetCertInfoResponse
{
	string strUserId;
	CertInfo certInfo;
}GetCertInfoResponse;

//增加印章返回
typedef struct _UploadAndAddSealInfoResponse
{
	string strSealId;
}UploadAndAddSealInfoResponse;

//印章处理返回
typedef struct _SealInfoHandleResponse
{
	bool bResult;
	string strSealId;	//印章ID
	_SealInfoHandleResponse() : bResult(false) {}
}SealInfoHandleResponse;

//增加模版返回
typedef struct _UploadAndAddTemplateInfoResponse
{
	string strTemplateId;
}UploadAndAddTemplateInfoResponse;

//模版处理返回
typedef struct _TemplateHandleResponse
{
	bool bResult;
	string strTemplateId;	//模版ID
	_TemplateHandleResponse() : bResult(false) {}
}TemplateHandleResponse;

//增加合同返回
typedef struct _UploadAndAddContractInfoResponse
{
	string strContractId;
}UploadAndAddContractInfoResponse;

//合同处理返回
typedef struct _ContractHandleResponse
{
	bool bResult;
	string strContractId;	//模版ID
	_ContractHandleResponse() : bResult(false) {}
}ContractHandleResponse;

//合同作废
typedef struct _ContractAbolishResponse
{
	bool bResult;
	string strSignId;
	string strFileId;
	_ContractAbolishResponse() : bResult(false) {}
}ContractAbolishResponse;

//渲染合同结果
typedef struct _ContractRenderResponse
{
	string strContractId;
}ContractRenderResponse;

//二位码合同接口返回
typedef struct _ScanContractAddResponse
{
	string strPrepareId;	//预处理ID
	string strQr;	//Base64（回调地址追加参数预处理Id）编码格式的二维码图片
}ScanContractAddResponse;

//合同绑定手机号返回
typedef struct _BindContractPhoneResponse
{
	string strContractId;	//合同ID
}BindContractPhoneResponse;

//批量模版签章
typedef struct _BatchTemplateSignResponse
{
	string strBatchId;	//业务号
	BatchSignBackList BatchSignBackList;
}BatchTemplateSignResponse;

//证书签章事件返回
typedef struct _SignEventBackResponse
{
	SignBackInfo SignBackInfo;
	SignCertEventList SignCertEventList;
}SignEventBackResponse;

//批量模版证书签章事件返回
typedef struct _BatchSignEventBackResponse
{
	string strBatchId;	//业务号
	BatchSignBackList BatchSignBackList;
	SignCertEventList SignCertEventList;
}BatchSignEventBackResponse;

//一步签章
typedef struct _SignOneStepAllResponse
{
	bool bSignState;	//签署结果
	string strContractFile;	//签署完成后文件流需自行转换为文件
	string strCertIssuer;	//签章时证书颁发者
	string strCertSn;	//签章时证书序列号
	string strMessage;	//错误信息
	string strSignTime;	//签署时间
	string strSigner;	//签署人
	string strCertSubject;	//证书主题
	string strSignId;	//本次签署ID
	string strHash;	//hash值
}SignOneStepAllResponse;

//合同验签
typedef struct _SignCheckResponse
{
	bool bResult;	//验证结果
	string strMessage;	//验证信息
	SignCheckResultsList SignCheckResultsList;
}SignCheckResponse;