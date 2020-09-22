// SignHttp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <time.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "SignHttp.h"
#include "..\\SealLog\SealLog.h"
#include "Http.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"


using namespace std;

int nrand;

SIGNHTTP int Http_InitLog(const char* szPath, const char* szFilename)
{
	log_init(szPath, szFilename, true, 10, LOG_INFO, DISPLAY_FILE);
	return 0;
}

//SIGNHTTP int Http_GetToken(const char * strUrl, string & jsonvalue)
//{
//	CHttp pHttp;
//	char* szUtf8PostData = pHttp.GBKToUTF8(jsonvalue.c_str());//GB2312中文信息转Utf8
//	std::string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, "", szUtf8PostData);
//	jsonvalue.clear();
//	jsonvalue = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
//	return 0;
//}

SIGNHTTP int Http_GetToken(const char* szUrl, const char* szJsonData, string& strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(szUrl, Hr_Post, "", szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_RegisterPersonalInfo(const char* strUrl, const char* szKey, const char* szJsonData, string& strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	//返回数据后申请证书
	string strCertData = pHttp.GetCertApplyData(strResponseUtf8Data);
	if (strCertData.length() != 0)
	{
		string strToken, strBaseUrl;
		pHttp.ParseURLWeb1(strUrl,"user/personal", strToken, strBaseUrl);
		strBaseUrl += "cert/apply?access_token=";
		strBaseUrl += strToken;
		char* szUtf8PostData1 = pHttp.GBKToUTF8(strCertData.c_str());
		strSign = pHttp.GetSignData(szUtf8PostData1, szKey);
		string strResponseUtf8Data1 = pHttp.RequestHttpsJsonInfo1(strBaseUrl, Hr_Post, strSign, szUtf8PostData1);
	}
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_GetPersonalUserInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_PersonalUnlock(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_PersonalLock(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_RegisterCompanyInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	//返回数据后申请证书
	string strCertData = pHttp.GetCertApplyData(strResponseUtf8Data);
	if (strCertData.length() != 0)
	{
		string strToken, strBaseUrl;
		pHttp.ParseURLWeb1(strUrl, "user/company", strToken, strBaseUrl);
		strBaseUrl += "cert/apply?access_token=";
		strBaseUrl += strToken;
		char* szUtf8PostData1 = pHttp.GBKToUTF8(strCertData.c_str());
		strSign = pHttp.GetSignData(szUtf8PostData1, szKey);
		string strResponseUtf8Data1 = pHttp.RequestHttpsJsonInfo1(strBaseUrl, Hr_Post, strSign, szUtf8PostData1);
	}
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_GetCompanyUserInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_CompanyUnlock(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_CompanyLock(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_GetCertinfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_UploadFileAndSealInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	FileAndSealInfo sealInfo;
	pHttp.GetFileAndSealInfo(szJsonData, sealInfo);
	string strFileData = pHttp.UploadFile(strUrl, sealInfo.strFilePath, sealInfo.strFileName, sealInfo.strFileType,sealInfo.strUserId);
	strFileData = pHttp.UTF8ToGBK(strFileData.c_str());//Utf8中文信息转GB2312
	//文件上传成功后，重新组装增加印章接口
	Json::Value root;
	Json::FastWriter writer;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strFileData.c_str(), json_object))
	{
		return -1;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return -1;
	}
	sealInfo.strFileId = fileData.get("file_id", "").asString();
	sealInfo.strFileName = fileData.get("file_name", "").asString();
	sealInfo.strFileType = fileData.get("file_type", "").asString();

	root["user_id"] = sealInfo.strUserId;
	root["seal_name"] = sealInfo.strSealName;
	root["seal_type"] = sealInfo.strSealType;
	root["file_id"] = sealInfo.strFileId;
	root["size"] = sealInfo.strSealSize;
	root["description"] = sealInfo.strDescription;
	std::string strJson = writer.write(root);

	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);

	string strToken, strBaseUrl;
	pHttp.ParseURLWeb1(strUrl, "file/upload", strToken, strBaseUrl);
	strBaseUrl += "seal/add?access_token=";
	strBaseUrl += strToken;
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strBaseUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_QuerySealInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_GetSealInfosFromUser(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_RemoveSealInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_LockSealStatus(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_UnLockSealStatus(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_DownloadFile(const char * strUrl, const char * szPath)
{
	CHttp pHttp;
	return pHttp.GetFile(strUrl, szPath);
}

SIGNHTTP int Http_UploadFileAndContractInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	FileAndSealInfo sealInfo;
	pHttp.GetFileAndSealInfo(szJsonData, sealInfo);
	string strFileData = pHttp.UploadFile(strUrl, sealInfo.strFilePath, sealInfo.strFileName, sealInfo.strFileType, sealInfo.strUserId);
	strFileData = pHttp.UTF8ToGBK(strFileData.c_str());//Utf8中文信息转GB2312
													   //文件上传成功后，重新组装增加印章接口
	Json::Value root;
	Json::FastWriter writer;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strFileData.c_str(), json_object))
	{
		return -1;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return -1;
	}
	sealInfo.strFileId = fileData.get("file_id", "").asString();
	sealInfo.strFileName = fileData.get("file_name", "").asString();
	sealInfo.strFileType = fileData.get("file_type", "").asString();

	root["user_id"] = sealInfo.strUserId;
	root["name"] = sealInfo.strFileName;
	root["file_id"] = sealInfo.strFileId;
	root["description"] = sealInfo.strDescription;
	std::string strJson = writer.write(root);

	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);

	string strToken, strBaseUrl;
	pHttp.ParseURLWeb1(strUrl, "file/upload", strToken, strBaseUrl);
	strBaseUrl += "contract/add?access_token=";
	strBaseUrl += strToken;
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strBaseUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_QueryContractInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_DeleteContractInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_AbolishContractInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_UploadFileAndTemplateInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	FileAndSealInfo sealInfo;
	pHttp.GetFileAndSealInfo(szJsonData, sealInfo);
	string strFileData = pHttp.UploadFile(strUrl, sealInfo.strFilePath, sealInfo.strFileName, sealInfo.strFileType, sealInfo.strUserId);
	strFileData = pHttp.UTF8ToGBK(strFileData.c_str());//Utf8中文信息转GB2312
													   //文件上传成功后，重新组装增加印章接口
	Json::Value root;
	Json::FastWriter writer;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strFileData.c_str(), json_object))
	{
		return -1;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return -1;
	}
	sealInfo.strFileId = fileData.get("file_id", "").asString();
	sealInfo.strFileName = fileData.get("file_name", "").asString();
	sealInfo.strFileType = fileData.get("file_type", "").asString();

	root["name"] = sealInfo.strFileName;
	root["file_id"] = sealInfo.strFileId;
	std::string strJson = writer.write(root);

	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);

	string strToken, strBaseUrl;
	pHttp.ParseURLWeb1(strUrl, "file/upload", strToken, strBaseUrl);
	strBaseUrl += "template/add?access_token=";
	strBaseUrl += strToken;
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strBaseUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_QueryTemplateInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_DeleteTemplateInfo(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_LockTemplateStatus(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_UnLockTemplateStatus(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignCommon(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignKeywordSign(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_ContractRender(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_ScanContractAdd(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_ScanCertContractAdd(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_QueryScanContract(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_BindContractPhone(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_QueryBindContract(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SingleTemplateSign(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_BatchTemplateSign(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignMeetingSingle(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignScanSign(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignCommonEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignKeywordSignEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SingleTemplateSignEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_BatchTemplateSignEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignMeetingSingleEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignScanSignEvent(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignOneStepAll(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignCheckCommon(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, szKey);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, strSign, szUtf8PostData);
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

SIGNHTTP int Http_SignCheckFile(const char * strUrl, const char * szKey, const char * szJsonData, string & strData)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(szJsonData);//GB2312中文信息转Utf8
	string strResponseUtf8Data = pHttp.UploadFile1(strUrl, szJsonData, "");
	strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
	return 0;
}

