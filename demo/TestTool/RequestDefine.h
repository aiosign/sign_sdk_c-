#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

typedef struct _AppInfo
{
	string strAppID;	//设备ID
	string strAppSecret;	//设备密钥
	string strGrantType;
	_AppInfo() :strGrantType("client_credentials") {}
}AppInfo;

typedef struct _UserIdInfoReq
{
	string strUserId;	//用户ID
}UserIdInfoReq;

//查询证书
typedef struct _CertInfoReq
{
	string strPrepareId;	//预处理ID
}CertInfoReq;

//增加印章请求接口
typedef struct _UploadAndAddSealInfoReq
{
	FileInfo FileInfo;
	SealInfo SealInfo;
}UploadAndAddSealInfoReq;

//传入印章ID
typedef struct _SealIdInfoReq
{
	string strSealId;
}SealIdInfoReq;

//传入模版ID
typedef struct _TemplateIdInfoReq
{
	string strTemplateId;
}TemplateIdInfoReq;

//传入合同ID
typedef struct _ContractIdInfoReq
{
	string strContractId;
}ContractIdInfoReq;

//合同作废
typedef struct _ContractAbolishReq
{
	string strSignId;
	string strUserId;
}ContractAbolishReq;

//二位码合同接口请求
typedef struct _ScanContractAddReq
{
	int nQrCodeWidth;	//二维码宽度
	int nQrcCodeHeight;	//二维码高度
	string strContractId;	//合同文件ID
	string strUserId;	//用户ID
	string strUrl;	//扫码回调地址
	string strExpireTime;	//二维码失效时间
	string strRemark;	//本次签章备注
	FileDetailList FileDetailList;
}ScanContractAddReq;

//查询二维码
typedef struct _ScanContractQueryReq
{
	string strPrepareId;	//预处理ID
}ScanContractQueryReq;

//合同绑定手机号
typedef struct _BindContractPhoneReq
{
	string strContractId;	//合同ID
	vector<string> phoneList;  //手机号列表
}BindContractPhoneReq;

//查询待签署合同
typedef struct _BindContractQueryReq
{
	string strPhone;	//手机号
	string stContractName;	//合同名,做条件查询
}BindContractQueryReq;

//坐标签章
typedef struct _SignCommonReq
{
	string strContractId;	//合同ID
	string strRemark;	//本次签章备注
	string strUserId;	//用户ID
	FileDetailList FileDetailList;
}SignCommonReq;

//关键字签章
typedef struct _SignKeywordReq
{
	bool bSignAll;	//true：合同内所有匹配位置全部签署；false：只签署第一个匹配；默认false
	double height;	//印章高度
	double width;	//印章宽度
	string strContractId;	//合同ID
	string strUserId;	//用户ID
	string strKeyword;	//关键字
	string strSealId;	//印章ID
}SignKeywordReq;

//单次模版签章
typedef struct _SingleTemplateSignReq
{
	string strTemplateId;
	SignParamList SignParamList;
	TextParamList TextParamList;
}SingleTemplateSignReq;

//批量模版签章
typedef struct _BatchTemplateSignReq
{
	string strTemplateId;
	BatchSignList BatchSignList;
}BatchTemplateSignReq;

//会签
typedef struct _SignMeetingSingleReq
{
	string strContractId;	//合同ID
	MeetingSignList MeetingSignList;
}SignMeetingSingleReq;

//扫描签章
typedef struct _SignScanReq
{
	string strPrepareId;	//预处理ID
}SignScanReq;

//一步签章
typedef struct _SignOneStepAllReq
{
	string strUserName;	//用户名
	string strIdNum;	//注册人的证件号
	string strUserType;	//用户类型（1个人；2企业）
	string strContractFileContent;	//合同文件流格式pdf Base64
	FileContentDetailList FileContentDetailList;
}SignOneStepAllReq;

//合同验签，文件ID
typedef struct _SignCheckCommonReq
{
	string strFileId;	//合同文件ID
}SignCheckCommonReq;

//合同验签，文件ID
typedef struct _SignCheckFileReq
{
	string strFilePath;	//文件路径
}SignCheckFileReq;