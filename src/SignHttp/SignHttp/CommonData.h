#pragma once

#include <Windows.h>
#include <string>
#include "CommonData.h"

using namespace std;

//证书结构体
typedef struct _CertInfo
{
	int nStatus;
	int nVersion;
	string strAwardTo;	//证书使用者名称
	string strCertId;	//证书id
	string strCertName;	//证书名称
	string strEndDate;	//证书失效时间
	string strStartDate;	//开始时间
	string strNumberId;	//证书证件号
	string strIssuer;	//颁发者
	string strCertSn;	//证书序列号
	_CertInfo() : nStatus(1) {};
}CertInfo;

//个人用户结构体
typedef struct _PersonalUserInfo
{
	int nStatus;
	string strUserId;	//用户ID
	string strUserName;	//用户名
	string strAreaCode;	//地区编码
	string strPhone;	//手机号码
	string strTypeId;	//证件类型 1,身份证;2,护照;3,驾驶证;4, 居住证
	string strNumberId;	//证件号
	string strMail;	//邮箱
	string strDescription;	//描述
	_PersonalUserInfo() : nStatus(1) {};
}PersonalUserInfo;

//企业结构体
typedef struct _CompanyInfo
{
	int nStatus;
	string strUserId;	//用户ID
	string strUserName;	//用户名
	string strAreaCode;	//地区编码
	string strUnitType;	//企业类型
	string strCrediCode;	//统一社会信用代码
	string strLegalName;	//法人名称
	string strLegalIdNum;	//法人证件号
	string strLegalIdType;	//法人证件类型
	string strLegalPhone;	//法人手机号码
	string strLegalMail;	//法人邮箱
	string strAgentName;	//经办人名称
	string strAgentIdNum;	//经办人证件号
	string strAgentIdType;	//经办人证件类型
	string strAgentPhone;	//经办人手机号码
	string strAgentMail;	//经办人邮箱
	string strDescription;	//描述
	_CompanyInfo() : nStatus(1) {};
}CompanyInfo;

//文件信息
typedef struct _FileInfo
{
	int nStatus;
	string strFilePath;	//文件路径
	string strFileName;	//文件名
	string strFileType;	//文件类型
	string strUserId;	//用户ID
	string strFileId;	//文件ID
	string strDescription;	//描述
	_FileInfo() : nStatus(1) {};
}FileInfo;

//印章信息
typedef struct _SealInfo
{
	int nStatus;
	string strFileId;	//文件ID
	string strSealName;	//印章名称
	string strSealType;	//印章类型
	string strSealSize;	//印章规格
	string strDescription;	//描述
	string strUserId;	//用户ID
	_SealInfo() : nStatus(1) {}
}SealInfo;

typedef vector<SealInfo>SealInfoList;

//模版信息
typedef struct _TemplateInfo
{
	int nStatus;
	string strTemplateId;	//模版ID
	string strFileId;	//文件ID
	string strTemplateName;	//名称
	string strCreateTime;	//创建时间
	string strSignParams;	//签名域
	string strTextParamsn;	//文字域
	_TemplateInfo() : nStatus(1) {}
}TemplateInfo;

//合同详细数据
typedef struct _ContractDetail
{
	int nNumber;	//第几页
	string strImageFileId;	//图片ID
	_ContractDetail() : nNumber(0) {}
}ContractDetail;

typedef vector<ContractDetail>ContractDetailList;

//合同信息
typedef struct _ContractInfo
{
	int nStatus;
	int nPageCount; //合同页数
	int nRenderComplete; //是否渲染完成:0渲染失败1渲染成功2渲染中
	int nSize; //文件大小
	string strContractId;	//合同ID
	string strFileId;	//文件ID
	string strContractName;	//名称
	string strCreateTime;	//创建时间
	ContractDetailList detailList;
	_ContractInfo() : nStatus(1), nPageCount(0), nRenderComplete(1), nSize(0){}
}ContractInfo;

//签章详情
typedef struct _FileDetailItem
{
	double height;	//印章高度
	double width;	//印章宽度
	double horizontal;	//水平横坐标
	double vertical;	//垂直纵坐标
	int nPageNumber;	//签章页数
	string strSealId;	//印章ID
	string strFileId;	//文件ID
	_FileDetailItem() : nPageNumber(0) {}
}FileDetailItem;

typedef vector<FileDetailItem>FileDetailList;

//二维码合同信息
typedef struct _ScanContractInfo
{
	bool bQrCodeState;	//二维码是否过期
	bool bSignState;	//签署状态
	int nStatus;
	int nPageCount; //合同页数
	int nRenderComplete; //是否渲染完成:0渲染失败1渲染成功2渲染中
	int nSize; //文件大小
	string strContractId;	//合同ID
	string strFileId;	//文件ID
	string strContractName;	//名称
	string strUserId;	//用户ID
	FileDetailList FileDetailList;
	ContractDetailList detailList;
	_ScanContractInfo() : bQrCodeState(false), bSignState(false),nStatus(1), nPageCount(0), nRenderComplete(1), nSize(0) {}
}ScanContractInfo;

//待签署的合同项
typedef struct _BindContractItem
{
	int nStatus;
	string strId;	//数据库ID
	string strContractId;	//合同ID
	string strContractName;	//名称
	string strContractCreateTime;	//合同创建时间
	string strUniqueIdentifier;	//唯一标识
	string strAppId;	//APPID
	string strCreateTime;	//手机绑定时间
	string strUpdateTime;	//手机更新时间
	_BindContractItem() : nStatus(1) {}
}BindContractItem;

typedef vector<BindContractItem>BindContractList;


//绑定手机号合同信息
typedef struct _BindContractInfo
{
	BindContractList BindContractList;
}BindContractInfo;

//签章后返回的结构
typedef struct _SignBackInfo
{
	bool bSignState;	//签署结果
	string strSignId;	//本次签署ID
	string strFileId;	//文件下载ID
	string strHash;	//hash值
}SignBackInfo;

//模版批量签章后返回的结构
typedef struct _BatchSignBackItem
{
	bool bSignState;	//签署结果
	string strCustomId;	//自定义号
	string strSignId;	//本次签署ID
	string strFileId;	//文件下载ID
	string strHash;	//hash值
}BatchSignBackItem;
typedef vector<BatchSignBackItem>BatchSignBackList;

//签名域项
typedef struct _SignParamItem
{
	bool bIsPicture;	//是否为图章入参 true:为图章信息false:签名图片（默认）
	double height;	//印章高度
	double width;	//印章宽度
	string strSealId;	//印章ID
	string strSignKey;	//签名域
	string strUserId;	//用户ID
}SignParamItem;
typedef vector<SignParamItem>SignParamList;

//文字域项
typedef struct _TextParamItem
{
	string strKey;	//填充参数key
	string strValue;	//填充值
}TextParamItem;
typedef vector<TextParamItem>TextParamList;

//批量模版签章项
typedef struct _BatchSignItem
{
	string strcCustomId;	//用户自定义ID
	SignParamList SignParamList;
	TextParamList TextParamList;
}BatchSignItem;
typedef vector<BatchSignItem>BatchSignList;

//会签接口项
typedef struct _MeetingSignItem
{
	int nPageNum;	//签章页码
	int nSignLeft;	//签署左距离
	int nSignTop;	//签署上距离
	double height;	//印章高度
	double width;	//印章宽度
	string strSealId;	//印章ID
	string strUserId;	//用户ID
}MeetingSignItem;
typedef vector<MeetingSignItem>MeetingSignList;

//证书事件
typedef struct _SignCertEventItem
{
	string strUserId;	//用户ID
	string strCertId;	//证书ID
}SignCertEventItem;
typedef vector<SignCertEventItem>SignCertEventList;

//签章详情--一步签章带有签章内容
typedef struct _FileContentDetailItem
{
	double height;	//印章高度
	double width;	//印章宽度
	double horizontal;	//水平横坐标
	double vertical;	//垂直纵坐标
	int nPageNumber;	//签章页数
	string strSealFileContent;	//印章文件内容Base64
	_FileContentDetailItem() : nPageNumber(0) {}
}FileContentDetailItem;
typedef vector<FileContentDetailItem>FileContentDetailList;

//验签结果
typedef struct _SignCheckResultsItem
{
	bool bResult;	//签署结果
	int nPageNumber;	//签章页数
	string strCertSn;	//证书序列号
	string strSignTime;	//签署时间
	string strTimestamp;	//时间戳日期
	string strSigner;	//签署人
	_SignCheckResultsItem() : nPageNumber(0) {}
}SignCheckResultsItem;
typedef vector<SignCheckResultsItem>SignCheckResultsList;

//下载文件
typedef struct _DownloadFileInfo
{
	string strFileId;	//文件ID
	string strFilePath;	//文件保存路径
}DownloadFileInfo;