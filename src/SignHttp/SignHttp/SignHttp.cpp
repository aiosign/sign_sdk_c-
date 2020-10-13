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
#include "IniFileOpt.h"
#include "MyGlobal.h"


using namespace std;

int nrand;

//SIGNHTTP int Http_InitLog(const char* szPath, const char* szFilename)
//{
//	log_init(szPath, szFilename, true, 10, LOG_INFO, DISPLAY_FILE);
//	return 0;
//}

//SIGNHTTP int Http_GetToken(const char * strUrl, string & jsonvalue)
//{
//	CHttp pHttp;
//	char* szUtf8PostData = pHttp.GBKToUTF8(jsonvalue.c_str());//GB2312中文信息转Utf8
//	std::string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(strUrl, Hr_Post, "", szUtf8PostData);
//	jsonvalue.clear();
//	jsonvalue = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312
//	return 0;
//}

SIGNHTTP int Http_Init()
{
	TCHAR szCurrentDir[512] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, MAX_PATH);
	//*_tcsrchr(szCurrentDir, '\\') = '\0';
	(strrchr(szCurrentDir, '\\'))[1] = '\0';

	CHAR szPathName[300] = { 0 };
	sprintf_s(szPathName, 300, "%s\\", szCurrentDir);

	log_init(szPathName, "SignHttp", true, 10, LOG_INFO, DISPLAY_FILE);

	CHAR szIniPathName[300] = { 0 };
	sprintf_s(szIniPathName, 300, "%s\\SignHttp.ini", szCurrentDir);

	CIniFileOpt iniObj(szIniPathName);

	iniObj.ReadString("HTTPIP", "IP", CMyGlobal::m_szUrl, 256);
	return 0;
}

SIGNHTTP int Http_GetToken(AppInfo* pDataIn, TokenResponse* pDataOut)
{
	string szUrl = string(CMyGlobal::m_szUrl) + "/oauth/token";

	Json::Value root;
	Json::FastWriter write;
	root["app_id"] = pDataIn->strAppID;
	root["app_secret"] = pDataIn->strAppSecret;
	root["grant_type"] = pDataIn->strGrantType;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl.c_str(),strJson.c_str());
	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code","").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strAccessToken = dataObject.get("access_token", "").asString();
	pDataOut->nExpiresIn = dataObject.get("expires_in", 0).asInt();
	pDataOut->strScope = dataObject.get("scope", "").asString();
	pDataOut->strTokenType = dataObject.get("token_type", "").asString();

	sprintf(CMyGlobal::m_szToken, "%s", pDataOut->strAccessToken.c_str());
	sprintf(CMyGlobal::m_szAppSecret, "%s", pDataIn->strAppSecret.c_str());
	sprintf(CMyGlobal::m_strAppId, "%s", pDataIn->strAppID.c_str());
	return 0;
}

SIGNHTTP const char* Http_GetSignData(const std::string & strJson, const std::string & secret_key)
{
	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, secret_key);
	return strSign.c_str();
}

SIGNHTTP int Http_RegisterPersonalInfo(PersonalUserInfo* pDataIn, RegisterResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"register?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/personal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_name"] = pDataIn->strUserName;
	root["area_code"] = pDataIn->strAreaCode;
	root["phone"] = pDataIn->strPhone;
	root["id_type"] = pDataIn->strTypeId;
	root["id_number"] = pDataIn->strNumberId;
	root["mail"] = pDataIn->strMail;
	root["description"] = pDataIn->strDescription;
	std::string strJson = write.write(root);

	Json::Reader reader(Json::Features::strictMode());

	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, CMyGlobal::m_szAppSecret);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(szUrl, Hr_Post, strSign, szUtf8PostData);
	//返回数据后申请证书
	//string strCertData = pHttp.GetCertApplyData(strResponseUtf8Data);
	Json::Value json_userObject;
	if (!reader.parse(strResponseUtf8Data.c_str(), json_userObject))
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	string strReturnCodeUser = json_userObject.get("return_code", "").asString();
	string strReturnMessageUser = json_userObject.get("return_message", "").asString();
	if (strReturnCodeUser != "1000")
	{
		return atoi(strReturnCodeUser.c_str());
	}
	string strResultCodeUser = json_userObject.get("result_code", "").asString();
	string strResultMessageUser = json_userObject.get("result_message", "").asString();
	if (strResultCodeUser != "0")
	{
		return atoi(strResultCodeUser.c_str());
	}
	Json::Value userData = json_userObject["data"];
	if (userData.isNull() || !userData.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	Json::Value root1;
	string strUserId = userData.get("user_id", "").asString();
	root1["user_id"] = strUserId;
	std::string strCertData = write.write(root1);
	if (strCertData.length() != 0)
	{
		//string strToken, strBaseUrl;
		//pHttp.ParseURLWeb1(szUrl,"user/personal", strToken, strBaseUrl);
		string strBaseUrl = CMyGlobal::m_szUrl;
		strBaseUrl += "cert/apply?access_token=";
		strBaseUrl += CMyGlobal::m_szToken;
		string strData1 = pHttp.GetRequestData(strBaseUrl.c_str(), strCertData.c_str());
		Json::Value json_object1;
		if (!reader.parse(strData1.c_str(), json_object1))
		{
			return HTTP_PARSE_JSON_FAILED;
		}

		string strReturnCode1 = json_object1.get("return_code", "").asString();
		string strReturnMessage1 = json_object1.get("return_message", "").asString();
		if (strReturnCode1 != "1000")
		{
			return atoi(strReturnCode1.c_str());
		}
		string strResultCode1 = json_object1.get("result_code", "").asString();
		string strResultMessage1 = json_object1.get("result_message", "").asString();
		if (strResultCode1 != "0")
		{
			return atoi(strResultCode1.c_str());
		}
	}
	pDataOut->strUserId = strUserId;
	return 0;
}

SIGNHTTP int Http_GetPersonalUserInfo(UserIdInfoReq* pDataIn, GetPersonalUserInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"userinfo?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/personal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	Json::Value certObject = dataObject["cert_info"];
	if (certObject.isNull() || !certObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->certInfo.nStatus = certObject.get("status", 1).asInt();
	pDataOut->certInfo.nVersion = certObject.get("version", 3).asInt();
	pDataOut->certInfo.strAwardTo = certObject.get("award_to", "").asString();
	pDataOut->certInfo.strCertId = certObject.get("cert_id", "").asString();
	pDataOut->certInfo.strCertName = certObject.get("cert_name", "").asString();
	pDataOut->certInfo.strCertSn = certObject.get("sn", "").asString();
	pDataOut->certInfo.strEndDate = certObject.get("end_date", "").asString();
	pDataOut->certInfo.strIssuer = certObject.get("issuer", "").asString();
	pDataOut->certInfo.strNumberId = certObject.get("id_number", "").asString();
	pDataOut->certInfo.strStartDate = certObject.get("start_date", "").asString();

	Json::Value personalInfoObject = dataObject["personal_info"];
	if (personalInfoObject.isNull() || !personalInfoObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->personalUserInfo.nStatus = personalInfoObject.get("status", 1).asInt();
	pDataOut->personalUserInfo.strAreaCode = personalInfoObject.get("area_code", "").asString();
	pDataOut->personalUserInfo.strMail = personalInfoObject.get("mail", "").asString();
	pDataOut->personalUserInfo.strNumberId = personalInfoObject.get("id_number", "").asString();
	pDataOut->personalUserInfo.strPhone = personalInfoObject.get("phone", "").asString();
	pDataOut->personalUserInfo.strTypeId = personalInfoObject.get("id_type", "").asString();
	pDataOut->personalUserInfo.strUserId = personalInfoObject.get("user_id", "").asString();
	pDataOut->personalUserInfo.strUserName = personalInfoObject.get("user_name", "").asString();

	return 0;
}

SIGNHTTP int Http_PersonalUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"unlock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/personal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();

	return 0;
}

SIGNHTTP int Http_PersonalLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"lock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/personal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();

	return 0;
}

SIGNHTTP int Http_RegisterCompanyInfo(CompanyInfo* pDataIn, RegisterResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"register?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/company/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_name"] = pDataIn->strUserName;
	root["area_code"] = pDataIn->strAreaCode;
	root["unit_type"] = pDataIn->strUnitType;
	root["credi_code"] = pDataIn->strCrediCode;
	root["legal_name"] = pDataIn->strLegalName;
	root["legal_id_number"] = pDataIn->strLegalIdNum;
	root["legal_id_type"] = pDataIn->strLegalIdType;
	root["legal_phone"] = pDataIn->strLegalPhone;
	root["legal_email"] = pDataIn->strLegalMail;
	root["agent_name"] = pDataIn->strAgentName;
	root["agent_id_number"] = pDataIn->strAgentIdNum;
	root["agent_id_type"] = pDataIn->strAgentIdType;
	root["agent_phone"] = pDataIn->strAgentPhone;
	root["agent_email"] = pDataIn->strAgentMail;
	root["description"] = pDataIn->strDescription;
	std::string strJson = write.write(root);

	Json::Reader reader(Json::Features::strictMode());

	CHttp pHttp;
	char* szUtf8PostData = pHttp.GBKToUTF8(strJson.c_str());//GB2312中文信息转Utf8
	string strSign = pHttp.GetSignData(szUtf8PostData, CMyGlobal::m_szAppSecret);
	string strResponseUtf8Data = pHttp.RequestHttpsJsonInfo1(szUrl, Hr_Post, strSign, szUtf8PostData);

	Json::Value json_userObject;
	if (!reader.parse(strResponseUtf8Data.c_str(), json_userObject))
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	string strReturnCodeUser = json_userObject.get("return_code", "").asString();
	string strReturnMessageUser = json_userObject.get("return_message", "").asString();
	if (strReturnCodeUser != "1000")
	{
		return atoi(strReturnCodeUser.c_str());
	}
	string strResultCodeUser = json_userObject.get("result_code", "").asString();
	string strResultMessageUser = json_userObject.get("result_message", "").asString();
	if (strResultCodeUser != "0")
	{
		return atoi(strResultCodeUser.c_str());
	}
	Json::Value userData = json_userObject["data"];
	if (userData.isNull() || !userData.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	Json::Value root1;
	string strUserId = userData.get("user_id", "").asString();
	root1["user_id"] = strUserId;
	std::string strCertData = write.write(root1);
	if (strCertData.length() != 0)
	{
		//string strToken, strBaseUrl;
		//pHttp.ParseURLWeb1(szUrl, "user/company", strToken, strBaseUrl);
		//strBaseUrl += "cert/apply?access_token=";
		//strBaseUrl += strToken;
		string strBaseUrl = CMyGlobal::m_szUrl;
		strBaseUrl += "cert/apply?access_token=";
		strBaseUrl += CMyGlobal::m_szToken;

		string strData1 = pHttp.GetRequestData(strBaseUrl.c_str(), strCertData.c_str());
		Json::Value json_object1;
		if (!reader.parse(strData1.c_str(), json_object1))
		{
			return HTTP_PARSE_JSON_FAILED;
		}

		string strReturnCode1 = json_object1.get("return_code", "").asString();
		string strReturnMessage1 = json_object1.get("return_message", "").asString();
		if (strReturnCode1 != "1000")
		{
			return atoi(strReturnCode1.c_str());
		}
		string strResultCode1 = json_object1.get("result_code", "").asString();
		string strResultMessage1 = json_object1.get("result_message", "").asString();
		if (strResultCode1 != "0")
		{
			return atoi(strResultCode1.c_str());
		}
	}
	pDataOut->strUserId = strUserId;
	return 0;
}

SIGNHTTP int Http_GetCompanyUserInfo(UserIdInfoReq* pDataIn, GetCompanyInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"userinfo?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/company/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	Json::Value certObject = dataObject["cert_info"];
	if (certObject.isNull() || !certObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->certInfo.nStatus = certObject.get("status", 1).asInt();
	pDataOut->certInfo.nVersion = certObject.get("version", 3).asInt();
	pDataOut->certInfo.strAwardTo = certObject.get("award_to", "").asString();
	pDataOut->certInfo.strCertId = certObject.get("cert_id", "").asString();
	pDataOut->certInfo.strCertName = certObject.get("cert_name", "").asString();
	pDataOut->certInfo.strCertSn = certObject.get("sn", "").asString();
	pDataOut->certInfo.strEndDate = certObject.get("end_date", "").asString();
	pDataOut->certInfo.strIssuer = certObject.get("issuer", "").asString();
	pDataOut->certInfo.strNumberId = certObject.get("id_number", "").asString();
	pDataOut->certInfo.strStartDate = certObject.get("start_date", "").asString();

	Json::Value companyInfoObject = dataObject["company_user_info"];
	if (companyInfoObject.isNull() || !companyInfoObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->CompanyInfo.nStatus = companyInfoObject.get("status", 1).asInt();
	pDataOut->CompanyInfo.strAreaCode = companyInfoObject.get("area_code", "").asString();
	pDataOut->CompanyInfo.strAgentIdNum = companyInfoObject.get("agent_id_number", "").asString();
	pDataOut->CompanyInfo.strAgentIdType = companyInfoObject.get("agent_id_type", "").asString();
	pDataOut->CompanyInfo.strAgentMail = companyInfoObject.get("agent_email", "").asString();
	pDataOut->CompanyInfo.strAgentName = companyInfoObject.get("agent_name", "").asString();
	pDataOut->CompanyInfo.strAgentPhone = companyInfoObject.get("agent_phone", "").asString();
	pDataOut->CompanyInfo.strCrediCode = companyInfoObject.get("credi_code", "").asString();
	pDataOut->CompanyInfo.strDescription = companyInfoObject.get("description", "").asString();
	pDataOut->CompanyInfo.strLegalIdNum = companyInfoObject.get("legal_id_number", "").asString();
	pDataOut->CompanyInfo.strLegalIdType = companyInfoObject.get("legal_id_type", "").asString();
	pDataOut->CompanyInfo.strLegalMail = companyInfoObject.get("legal_email", "").asString();
	pDataOut->CompanyInfo.strLegalName = companyInfoObject.get("legal_name", "").asString();
	pDataOut->CompanyInfo.strLegalPhone = companyInfoObject.get("legal_phone", "").asString();
	pDataOut->CompanyInfo.strUnitType = companyInfoObject.get("unit_type", "").asString();
	pDataOut->CompanyInfo.strUserId = companyInfoObject.get("user_id", "").asString();
	pDataOut->CompanyInfo.strUserName = companyInfoObject.get("user_name", "").asString();
	return 0;
}

SIGNHTTP int Http_CompanyUnlock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"unlock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/company/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();
	return 0;
}

SIGNHTTP int Http_CompanyLock(UserIdInfoReq* pDataIn, UserInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"lock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/user/company/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();

	return 0;
}

SIGNHTTP int Http_GetCertinfo(CertInfoReq* pDataIn, GetCertInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"certinfo?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/cert/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["prepare_id"] = pDataIn->strPrepareId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	pDataOut->strUserId = dataObject.get("user_id", "").asString();

	Json::Value certObject = dataObject["cert_info"];
	if (certObject.isNull() || !certObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->certInfo.nStatus = certObject.get("status", 1).asInt();
	pDataOut->certInfo.nVersion = certObject.get("version", 3).asInt();
	pDataOut->certInfo.strAwardTo = certObject.get("award_to", "").asString();
	pDataOut->certInfo.strCertId = certObject.get("cert_id", "").asString();
	pDataOut->certInfo.strCertName = certObject.get("cert_name", "").asString();
	pDataOut->certInfo.strCertSn = certObject.get("sn", "").asString();
	pDataOut->certInfo.strEndDate = certObject.get("end_date", "").asString();
	pDataOut->certInfo.strIssuer = certObject.get("issuer", "").asString();
	pDataOut->certInfo.strNumberId = certObject.get("id_number", "").asString();
	pDataOut->certInfo.strStartDate = certObject.get("start_date", "").asString();

	return 0;
}

SIGNHTTP int Http_UploadFileAndSealInfo(UploadAndAddSealInfoReq* pDataIn, UploadAndAddSealInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"upload?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/file/",
		CMyGlobal::m_szToken
	);

	CHttp pHttp;
	string strFileData = pHttp.UploadFile(szUrl, pDataIn->FileInfo.strFilePath, pDataIn->FileInfo.strFileName, pDataIn->FileInfo.strFileType, pDataIn->FileInfo.strUserId);
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
	string strFileId = fileData.get("file_id", "").asString();
	string strFileName = fileData.get("file_name", "").asString();
	string strFileType = fileData.get("file_type", "").asString();

	root["user_id"] = pDataIn->FileInfo.strUserId;
	root["seal_name"] = pDataIn->SealInfo.strSealName;
	root["seal_type"] = pDataIn->SealInfo.strSealType;
	root["file_id"] = strFileId;
	root["size"] = pDataIn->SealInfo.strSealSize;
	root["description"] = pDataIn->SealInfo.strDescription;
	std::string strJson = writer.write(root);

	string strBaseUrl = CMyGlobal::m_szUrl;
	strBaseUrl += "/seal/add?access_token=";
	strBaseUrl += CMyGlobal::m_szToken;

	string strData = pHttp.GetRequestData(strBaseUrl.c_str(), strJson.c_str());

	Json::Value json_object1;
	if (!reader.parse(strData.c_str(), json_object1))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object1.get("return_code", "").asString();
	string strReturnMessage = json_object1.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object1.get("result_code", "").asString();
	string strResultMessage = json_object1.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object1["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strSealId = dataObject.get("seal_id", "").asString();
	return 0;
}

SIGNHTTP int Http_QuerySealInfo(SealIdInfoReq* pDataIn, SealInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"query?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/seal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["seal_id"] = pDataIn->strSealId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->nStatus = dataObject.get("status", 1).asInt();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();
	pDataOut->strSealSize = dataObject.get("size", "").asString();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strSealName = dataObject.get("seal_name", "").asString();
	pDataOut->strSealType = dataObject.get("seal_type", "").asString();
	return 0;
}

SIGNHTTP int Http_GetSealInfosFromUser(UserIdInfoReq* pDataIn, SealInfoList* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"getSealInfos?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/seal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (!dataObject.isNull())
	{
		for (size_t i = 0; i < dataObject.size(); ++i)
		{
			SealInfo SealInfo;
			SealInfo.nStatus = dataObject[i].get("status", 1).asInt();
			SealInfo.strUserId = dataObject[i].get("user_id", "").asString();
			SealInfo.strSealSize = dataObject[i].get("size", "").asString();
			SealInfo.strFileId = dataObject[i].get("file_id", "").asString();
			SealInfo.strSealName = dataObject[i].get("seal_name", "").asString();
			SealInfo.strSealType = dataObject[i].get("seal_type", "").asString();
			pDataOut->push_back(SealInfo);
		}
	}
	return 0;
}

SIGNHTTP int Http_RemoveSealInfo(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"remove?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/seal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["seal_id"] = pDataIn->strSealId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strSealId = dataObject.get("seal_id", "").asString();
	return 0;
}

SIGNHTTP int Http_LockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"lock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/seal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["seal_id"] = pDataIn->strSealId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strSealId = dataObject.get("seal_id", "").asString();
	return 0;
}

SIGNHTTP int Http_UnLockSealStatus(SealIdInfoReq* pDataIn, SealInfoHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"unlock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/seal/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["seal_id"] = pDataIn->strSealId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strSealId = dataObject.get("seal_id", "").asString();
	return 0;
}

SIGNHTTP int Http_DownloadFile(DownloadFileInfo* pDataIn)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"download?"
		"&access_token=%s"
		"&fileId=%s",
		CMyGlobal::m_szUrl,
		"/file/",
		CMyGlobal::m_szToken,
		pDataIn->strFileId.c_str()
	);

	CHttp pHttp;
	return pHttp.GetFile(szUrl, pDataIn->strFilePath);
}
SIGNHTTP int Http_UploadFileAndTemplateInfo(FileInfo* pDataIn, UploadAndAddTemplateInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"upload?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/file/",
		CMyGlobal::m_szToken
	);

	CHttp pHttp;
	string strFileData = pHttp.UploadFile(szUrl, pDataIn->strFilePath, pDataIn->strFileName, pDataIn->strFileType, pDataIn->strUserId);
	strFileData = pHttp.UTF8ToGBK(strFileData.c_str());//Utf8中文信息转GB2312

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
	string strFileId = fileData.get("file_id", "").asString();
	string strFileName = fileData.get("file_name", "").asString();
	string strFileType = fileData.get("file_type", "").asString();

	root["name"] = strFileName;
	root["file_id"] = strFileId;
	std::string strJson = writer.write(root);

	string strBaseUrl;
	strBaseUrl = CMyGlobal::m_szUrl;
	strBaseUrl += "/template/add?access_token=";
	strBaseUrl += CMyGlobal::m_szToken;
	string strData = pHttp.GetRequestData(strBaseUrl.c_str(), strJson.c_str());//Utf8中文信息转GB2312

	Json::Value json_object1;
	if (!reader.parse(strData.c_str(), json_object1))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object1.get("return_code", "").asString();
	string strReturnMessage = json_object1.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object1.get("result_code", "").asString();
	string strResultMessage = json_object1.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object1["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strTemplateId = dataObject.get("template_id", "").asString();
	return 0;
}

SIGNHTTP int Http_QueryTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"query?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->nStatus = dataObject.get("status", 1).asInt();
	pDataOut->strCreateTime = dataObject.get("create_time", "").asString();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strSignParams = dataObject.get("sign_params", "").asString();
	pDataOut->strTemplateId = dataObject.get("template_id", "").asString();
	pDataOut->strTemplateName = dataObject.get("name", "").asString();
	pDataOut->strTextParamsn = dataObject.get("text_params", "").asString();
	return 0;
}

SIGNHTTP int Http_DeleteTemplateInfo(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"delete?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strTemplateId = dataObject.get("template_id", "").asString();
	return 0;
}

SIGNHTTP int Http_LockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"lock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strTemplateId = dataObject.get("template_id", "").asString();
	return 0;
}

SIGNHTTP int Http_UnLockTemplateStatus(TemplateIdInfoReq* pDataIn, TemplateHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"unlock?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strTemplateId = dataObject.get("template_id", "").asString();
	return 0;
}

SIGNHTTP int Http_UploadFileAndContractInfo(FileInfo* pDataIn, UploadAndAddContractInfoResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"upload?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/file/",
		CMyGlobal::m_szToken
	);

	CHttp pHttp;
	string strFileData = pHttp.UploadFile(szUrl, pDataIn->strFilePath, pDataIn->strFileName, pDataIn->strFileType, pDataIn->strUserId);
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
	string strFileId = fileData.get("file_id", "").asString();
	string strFileName = fileData.get("file_name", "").asString();
	string strFileType = fileData.get("file_type", "").asString();

	root["user_id"] = pDataIn->strUserId;
	root["name"] = strFileName;
	root["file_id"] = strFileId;
	root["description"] = pDataIn->strDescription;
	std::string strJson = writer.write(root);

	string strBaseUrl;
	strBaseUrl = CMyGlobal::m_szUrl;
	strBaseUrl += "/contract/add?access_token=";
	strBaseUrl += CMyGlobal::m_szToken;
	string strData = pHttp.GetRequestData(strBaseUrl.c_str(), strJson.c_str());

	Json::Value json_object1;
	if (!reader.parse(strData.c_str(), json_object1))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object1.get("return_code", "").asString();
	string strReturnMessage = json_object1.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object1.get("result_code", "").asString();
	string strResultMessage = json_object1.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object1["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	return 0;
}

SIGNHTTP int Http_QueryContractInfo(ContractIdInfoReq* pDataIn, ContractInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"query?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	pDataOut->nPageCount = dataObject.get("page_count", 0).asInt();
	pDataOut->nRenderComplete = dataObject.get("render_complete", 1).asInt();
	pDataOut->nSize = dataObject.get("size", 0).asInt();
	pDataOut->nStatus = dataObject.get("status", 1).asInt();
	pDataOut->strContractName = dataObject.get("name", "").asString();
	pDataOut->strCreateTime = dataObject.get("create_time", "").asString();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();

	Json::Value detailObject = dataObject["contract_infos"];
	if (!detailObject.isNull())
	{
		for (size_t i = 0; i < detailObject.size(); ++i)
		{
			ContractDetail DetailInfo;
			DetailInfo.nNumber = detailObject[i].get("number", 0).asInt();
			DetailInfo.strImageFileId = detailObject[i].get("image_file_id", "").asString();
			pDataOut->detailList.push_back(DetailInfo);
		}
	}
	return 0;
}

SIGNHTTP int Http_DeleteContractInfo(ContractIdInfoReq* pDataIn, ContractHandleResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"remove?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	return 0;
}

SIGNHTTP int Http_AbolishContractInfo(ContractAbolishReq* pDataIn, ContractAbolishResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"abolish?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["sign_id"] = pDataIn->strSignId;
	root["user_id"] = pDataIn->strUserId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", false).asBool();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	return 0;
}

SIGNHTTP int Http_ContractRender(ContractIdInfoReq* pDataIn, ContractRenderResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"render?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	return 0;
}

SIGNHTTP int Http_ScanContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"add?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/scan/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["url"] = pDataIn->strUrl;
	root["expire_time"] = pDataIn->strExpireTime;
	root["qr_code_width"] = pDataIn->nQrCodeWidth;
	root["qr_code_height"] = pDataIn->nQrcCodeHeight;
	root["remark"] = pDataIn->strRemark;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->FileDetailList.size(); ++i)
	{
		Json::Value item;
		FileDetailItem pItem = pDataIn->FileDetailList.at(i);
		item["height"] = pItem.height;
		item["horizontal"] = pItem.horizontal;
		item["page_number"] = pItem.nPageNumber;
		item["seal_id"] = pItem.strSealId;
		item["vertical"] = pItem.vertical;
		item["width"] = pItem.width;
		fields.append(item);
	}
	root["fields"] = fields;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strPrepareId = dataObject.get("prepare_id", "").asString();
	pDataOut->strQr = dataObject.get("qr", "").asString();
	return 0;
}

SIGNHTTP int Http_ScanCertContractAdd(ScanContractAddReq* pDataIn, ScanContractAddResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"add?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/scan/event_cert_contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["url"] = pDataIn->strUrl;
	root["expire_time"] = pDataIn->strExpireTime;
	root["qr_code_width"] = pDataIn->nQrCodeWidth;
	root["qr_code_height"] = pDataIn->nQrcCodeHeight;
	root["remark"] = pDataIn->strRemark;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->FileDetailList.size(); ++i)
	{
		Json::Value item;
		FileDetailItem pItem = pDataIn->FileDetailList.at(i);
		item["height"] = pItem.height;
		item["horizontal"] = pItem.horizontal;
		item["page_number"] = pItem.nPageNumber;
		item["seal_id"] = pItem.strSealId;
		item["vertical"] = pItem.vertical;
		item["width"] = pItem.width;
		fields.append(item);
	}
	root["fields"] = fields;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strPrepareId = dataObject.get("prepare_id", "").asString();
	pDataOut->strQr = dataObject.get("qr", "").asString();
	return 0;
}

SIGNHTTP int Http_QueryScanContract(ScanContractQueryReq* pDataIn, ScanContractInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"query?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/scan/contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["prepare_id"] = pDataIn->strPrepareId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bQrCodeState = dataObject.get("qr_code_state", false).asBool();
	pDataOut->bSignState = dataObject.get("sign_state", false).asBool();
	pDataOut->nStatus = dataObject.get("status", 0).asInt();
	pDataOut->nSize = dataObject.get("size", 0).asInt();
	pDataOut->nRenderComplete = dataObject.get("render_complete", 1).asInt();
	pDataOut->nPageCount = dataObject.get("page_count", 0).asInt();
	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	pDataOut->strContractName = dataObject.get("name", "").asString();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strUserId = dataObject.get("user_id", "").asString();

	Json::Value contractDetailObject = dataObject["contract_infos"];
	if (!contractDetailObject.isNull() && contractDetailObject.isObject())
	{
		for (size_t i = 0; i < contractDetailObject.size(); ++i)
		{
			ContractDetail pItem;
			pItem.nNumber = contractDetailObject[i].get("number", 0).asInt();
			pItem.strImageFileId = contractDetailObject[i].get("image_file_id", "").asString();
			pDataOut->detailList.push_back(pItem);
		}
	}
	Json::Value fieldsObject = dataObject["fields"];
	if (!fieldsObject.isNull())
	{
		for (size_t i = 0; i < fieldsObject.size(); ++i)
		{
			FileDetailItem pItem;
			pItem.height = fieldsObject[i].get("height", 0).asDouble();
			pItem.horizontal = fieldsObject[i].get("horizontal", 0).asDouble();
			pItem.nPageNumber = fieldsObject[i].get("page_number", 0).asInt();
			pItem.vertical = fieldsObject[i].get("vertical", 0).asDouble();
			pItem.width = fieldsObject[i].get("width", 0).asDouble();
			pItem.strSealId = fieldsObject[i].get("seal_id", "").asString();
			pItem.strFileId = fieldsObject[i].get("file_id", "").asString();
			pDataOut->FileDetailList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_BindContractPhone(BindContractPhoneReq* pDataIn, BindContractPhoneResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"bind_contract_phone?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/bind_contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;

	Json::Value params;
	for (size_t i = 0; i < pDataIn->phoneList.size(); ++i)
	{
		Json::Value item;
		item["phone"] = pDataIn->phoneList.at(i).c_str();
		params.append(item);
	}
	root["params"] = params;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strContractId = dataObject.get("contract_id", "").asString();
	return 0;
}

SIGNHTTP int Http_QueryBindContract(BindContractQueryReq* pDataIn, BindContractInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"query_bind_contract?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/bind_contract/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["phone"] = pDataIn->strPhone;
	if (!pDataIn->stContractName.empty())
	{
		root["contract_name"] = pDataIn->stContractName;
	}
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (!dataObject.isNull())
	{
		for (size_t i = 0; i < dataObject.size(); ++i)
		{
			BindContractItem pItem;
			pItem.nStatus = dataObject[i].get("status", 1).asInt();
			pItem.strAppId = dataObject[i].get("app_id", "").asString();
			pItem.strContractCreateTime = dataObject[i].get("contract_create_time", "").asString();
			pItem.strContractId = dataObject[i].get("contract_id", "").asString();
			pItem.strContractName = dataObject[i].get("contract_name", "").asString();
			pItem.strCreateTime = dataObject[i].get("create_time", "").asString();
			pItem.strId = dataObject[i].get("id", "").asString();
			pItem.strUniqueIdentifier = dataObject[i].get("unique_identifier", "").asString();
			pItem.strUpdateTime = dataObject[i].get("update_time", "").asString();
			pDataOut->BindContractList.push_back(pItem);
		}
	}
	return 0;
}
SIGNHTTP int Http_SignCommon(SignCommonReq* pDataIn, SignBackInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"common?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["remark"] = pDataIn->strRemark;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->FileDetailList.size(); ++i)
	{
		FileDetailItem FileDetailItem = pDataIn->FileDetailList.at(i);
		Json::Value item;
		item["height"] = FileDetailItem.height;
		item["horizontal"] = FileDetailItem.horizontal;
		item["page_number"] = FileDetailItem.nPageNumber;
		item["seal_id"] = FileDetailItem.strSealId;
		item["vertical"] = FileDetailItem.vertical;
		item["width"] = FileDetailItem.width;
		fields.append(item);
	}
	root["fields"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_SignKeywordSign(SignKeywordReq* pDataIn, SignBackInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"keywordSign?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["keyword"] = pDataIn->strKeyword;
	root["height"] = pDataIn->height;
	root["sign_all"] = pDataIn->bSignAll;
	root["seal_id"] = pDataIn->strSealId;
	root["width"] = pDataIn->width;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_SingleTemplateSign(SingleTemplateSignReq* pDataIn, SignBackInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"single?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;

	Json::Value fields;
	Json::Value sign_params;
	for (size_t i = 0; i < pDataIn->SignParamList.size(); ++i)
	{
		SignParamItem SignParamItem = pDataIn->SignParamList.at(i);
		Json::Value item;
		item["seal_id"] = SignParamItem.strSealId;
		item["sign_key"] = SignParamItem.strSignKey;
		item["user_id"] = SignParamItem.strUserId;
		item["width"] = SignParamItem.width;
		item["height"] = SignParamItem.height;
		sign_params.append(item);
	}
	Json::Value text_params;
	for (size_t i = 0; i < pDataIn->TextParamList.size(); ++i)
	{
		TextParamItem TextParamItem = pDataIn->TextParamList.at(i);
		Json::Value item;
		item["key"] = TextParamItem.strKey;
		item["value"] = TextParamItem.strValue;
		text_params.append(item);
	}
	fields["sign_params"] = sign_params;
	fields["text_params"] = text_params;

	root["sign_field"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_BatchTemplateSign(BatchTemplateSignReq* pDataIn, BatchTemplateSignResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"batch?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;

	Json::Value templates;
	for (size_t i = 0; i < pDataIn->BatchSignList.size(); ++i)
	{
		BatchSignItem BatchSignItem = pDataIn->BatchSignList.at(i);
		Json::Value templatesItem;
		templatesItem["custom_id"] = BatchSignItem.strcCustomId;
		Json::Value sign_params;
		for (size_t i = 0; i < BatchSignItem.SignParamList.size(); ++i)
		{
			SignParamItem SignParamItem = BatchSignItem.SignParamList.at(i);
			Json::Value item;
			item["seal_id"] = SignParamItem.strSealId;
			item["sign_key"] = SignParamItem.strSignKey;
			item["user_id"] = SignParamItem.strUserId;
			item["width"] = SignParamItem.width;
			item["height"] = SignParamItem.height;
			sign_params.append(item);
		}
		templatesItem["sign_params"] = sign_params;
		Json::Value text_params;
		for (size_t i = 0; i < BatchSignItem.TextParamList.size(); ++i)
		{
			TextParamItem TextParamItem = BatchSignItem.TextParamList.at(i);
			Json::Value item;
			item["key"] = TextParamItem.strKey;
			item["value"] = TextParamItem.strValue;
			text_params.append(item);
		}
		templatesItem["text_params"] = text_params;
		templates.append(templatesItem);
	}
	root["batch_templates"] = templates;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strBatchId = dataObject.get("batch_id", "").asString();
	Json::Value signInfosObject = dataObject["sign_infos"];
	if (!signInfosObject.isNull() || signInfosObject.isObject())
	{
		for (size_t i = 0; i < signInfosObject.size(); ++i)
		{
			BatchSignBackItem pItem;
			pItem.bSignState = signInfosObject[i].get("sign_state", true).asBool();
			pItem.strCustomId = signInfosObject[i].get("custom_id", "").asString();
			pItem.strFileId = signInfosObject[i].get("file_id", "").asString();
			pItem.strHash = signInfosObject[i].get("hash", "").asString();
			pItem.strSignId = signInfosObject[i].get("sign_id", "").asString();
			pDataOut->BatchSignBackList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignMeetingSingle(SignMeetingSingleReq* pDataIn, SignBackInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"single?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/meeting/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->MeetingSignList.size(); ++i)
	{
		MeetingSignItem MeetingSignItem = pDataIn->MeetingSignList.at(i);
		Json::Value item;
		item["sign_width"] = MeetingSignItem.width;
		item["sign_top"] = MeetingSignItem.nSignTop;
		item["page_num"] = MeetingSignItem.nPageNum;
		item["seal_id"] = MeetingSignItem.strSealId;
		item["sign_height"] = MeetingSignItem.height;
		item["sign_left"] = MeetingSignItem.nSignLeft;
		item["user_id"] = MeetingSignItem.strUserId;
		fields.append(item);
	}
	root["sign_details"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_SignScanSign(SignScanReq* pDataIn, SignBackInfo* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"scanSign?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["prepare_id"] = pDataIn->strPrepareId;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strFileId = dataObject.get("file_id", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_SignCommonEvent(SignCommonReq* pDataIn, SignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"common?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["remark"] = pDataIn->strRemark;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->FileDetailList.size(); ++i)
	{
		FileDetailItem FileDetailItem = pDataIn->FileDetailList.at(i);
		Json::Value item;
		item["height"] = FileDetailItem.height;
		item["horizontal"] = FileDetailItem.horizontal;
		item["page_number"] = FileDetailItem.nPageNumber;
		item["seal_id"] = FileDetailItem.strSealId;
		item["vertical"] = FileDetailItem.vertical;
		item["width"] = FileDetailItem.width;
		fields.append(item);
	}
	root["fields"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->SignBackInfo.bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->SignBackInfo.strFileId = dataObject.get("file_id", "").asString();
	pDataOut->SignBackInfo.strHash = dataObject.get("hash", "").asString();
	pDataOut->SignBackInfo.strSignId = dataObject.get("sign_id", "").asString();

	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignKeywordSignEvent(SignKeywordReq* pDataIn, SignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"keywordSign?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;
	root["user_id"] = pDataIn->strUserId;
	root["keyword"] = pDataIn->strKeyword;
	root["height"] = pDataIn->height;
	root["sign_all"] = pDataIn->bSignAll;
	root["seal_id"] = pDataIn->strSealId;
	root["width"] = pDataIn->width;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->SignBackInfo.bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->SignBackInfo.strFileId = dataObject.get("file_id", "").asString();
	pDataOut->SignBackInfo.strHash = dataObject.get("hash", "").asString();
	pDataOut->SignBackInfo.strSignId = dataObject.get("sign_id", "").asString();

	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SingleTemplateSignEvent(SingleTemplateSignReq* pDataIn, SignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"single?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;

	Json::Value fields;
	Json::Value sign_params;
	for (size_t i = 0; i < pDataIn->SignParamList.size(); ++i)
	{
		SignParamItem SignParamItem = pDataIn->SignParamList.at(i);
		Json::Value item;
		item["seal_id"] = SignParamItem.strSealId;
		item["sign_key"] = SignParamItem.strSignKey;
		item["user_id"] = SignParamItem.strUserId;
		item["width"] = SignParamItem.width;
		item["height"] = SignParamItem.height;
		sign_params.append(item);
	}
	Json::Value text_params;
	for (size_t i = 0; i < pDataIn->TextParamList.size(); ++i)
	{
		TextParamItem TextParamItem = pDataIn->TextParamList.at(i);
		Json::Value item;
		item["key"] = TextParamItem.strKey;
		item["value"] = TextParamItem.strValue;
		text_params.append(item);
	}
	fields["sign_params"] = sign_params;
	fields["text_params"] = text_params;

	root["sign_field"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->SignBackInfo.bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->SignBackInfo.strFileId = dataObject.get("file_id", "").asString();
	pDataOut->SignBackInfo.strHash = dataObject.get("hash", "").asString();
	pDataOut->SignBackInfo.strSignId = dataObject.get("sign_id", "").asString();

	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_BatchTemplateSignEvent(BatchTemplateSignReq* pDataIn, BatchSignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"batch?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/template/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["template_id"] = pDataIn->strTemplateId;

	Json::Value templates;
	for (size_t i = 0; i < pDataIn->BatchSignList.size(); ++i)
	{
		BatchSignItem BatchSignItem = pDataIn->BatchSignList.at(i);
		Json::Value templatesItem;
		templatesItem["custom_id"] = BatchSignItem.strcCustomId;
		Json::Value sign_params;
		for (size_t i = 0; i < BatchSignItem.SignParamList.size(); ++i)
		{
			SignParamItem SignParamItem = BatchSignItem.SignParamList.at(i);
			Json::Value item;
			item["seal_id"] = SignParamItem.strSealId;
			item["sign_key"] = SignParamItem.strSignKey;
			item["user_id"] = SignParamItem.strUserId;
			item["width"] = SignParamItem.width;
			item["height"] = SignParamItem.height;
			sign_params.append(item);
		}
		templatesItem["sign_params"] = sign_params;
		Json::Value text_params;
		for (size_t i = 0; i < BatchSignItem.TextParamList.size(); ++i)
		{
			TextParamItem TextParamItem = BatchSignItem.TextParamList.at(i);
			Json::Value item;
			item["key"] = TextParamItem.strKey;
			item["value"] = TextParamItem.strValue;
			text_params.append(item);
		}
		templatesItem["text_params"] = text_params;
		templates.append(templatesItem);
	}
	root["batch_templates"] = templates;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->strBatchId = dataObject.get("batch_id", "").asString();
	Json::Value signInfosObject = dataObject["sign_infos"];
	if (!signInfosObject.isNull() || signInfosObject.isObject())
	{
		for (size_t i = 0; i < signInfosObject.size(); ++i)
		{
			BatchSignBackItem pItem;
			pItem.bSignState = signInfosObject[i].get("sign_state", true).asBool();
			pItem.strCustomId = signInfosObject[i].get("custom_id", "").asString();
			pItem.strFileId = signInfosObject[i].get("file_id", "").asString();
			pItem.strHash = signInfosObject[i].get("hash", "").asString();
			pItem.strSignId = signInfosObject[i].get("sign_id", "").asString();
			pDataOut->BatchSignBackList.push_back(pItem);
		}
	}
	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignMeetingSingleEvent(SignMeetingSingleReq* pDataIn, SignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"single?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/meeting/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["contract_id"] = pDataIn->strContractId;

	Json::Value fields;
	for (size_t i = 0; i < pDataIn->MeetingSignList.size(); ++i)
	{
		MeetingSignItem MeetingSignItem = pDataIn->MeetingSignList.at(i);
		Json::Value item;
		item["sign_width"] = MeetingSignItem.width;
		item["sign_top"] = MeetingSignItem.nSignTop;
		item["page_num"] = MeetingSignItem.nPageNum;
		item["seal_id"] = MeetingSignItem.strSealId;
		item["sign_height"] = MeetingSignItem.height;
		item["sign_left"] = MeetingSignItem.nSignLeft;
		item["user_id"] = MeetingSignItem.strUserId;
		fields.append(item);
	}
	root["sign_details"] = fields;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->SignBackInfo.bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->SignBackInfo.strFileId = dataObject.get("file_id", "").asString();
	pDataOut->SignBackInfo.strHash = dataObject.get("hash", "").asString();
	pDataOut->SignBackInfo.strSignId = dataObject.get("sign_id", "").asString();

	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignScanSignEvent(SignScanReq* pDataIn, SignEventBackResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"scanSign?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["prepare_id"] = pDataIn->strPrepareId;

	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->SignBackInfo.bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->SignBackInfo.strFileId = dataObject.get("file_id", "").asString();
	pDataOut->SignBackInfo.strHash = dataObject.get("hash", "").asString();
	pDataOut->SignBackInfo.strSignId = dataObject.get("sign_id", "").asString();

	Json::Value eventCertsObject = dataObject["event_certs"];
	if (!eventCertsObject.isNull() || eventCertsObject.isObject())
	{
		for (size_t i = 0; i < eventCertsObject.size(); ++i)
		{
			SignCertEventItem pItem;
			pItem.strCertId = eventCertsObject[i].get("user_id", "").asString();
			pItem.strUserId = eventCertsObject[i].get("cert_id", "").asString();
			pDataOut->SignCertEventList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignOneStepAll(SignOneStepAllReq* pDataIn, SignOneStepAllResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"directSign?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/event_cert_sign/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;

	root["user_name"] = pDataIn->strUserName;
	root["id_number"] = pDataIn->strIdNum;
	root["user_type"] = pDataIn->strUserType;
	root["contract_file_content"] = pDataIn->strContractFileContent;
	
	Json::Value fields;
	for (size_t i = 0; i < pDataIn->FileContentDetailList.size(); ++i)
	{
		FileContentDetailItem FileContentDetailItem = pDataIn->FileContentDetailList.at(i);
		Json::Value item;
		item["seal_height"] = FileContentDetailItem.height;
		item["horizontal"] = FileContentDetailItem.horizontal;
		item["page_num"] = FileContentDetailItem.nPageNumber;
		item["seal_file_content"] = FileContentDetailItem.strSealFileContent;
		item["vertical"] = FileContentDetailItem.vertical;
		item["seal_width"] = FileContentDetailItem.width;
		fields.append(item);
	}
	root["sign_fields"] = fields;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bSignState = dataObject.get("sign_state", true).asBool();
	pDataOut->strCertIssuer = dataObject.get("cert_issuer", "").asString();
	pDataOut->strCertSn = dataObject.get("cert_sn", "").asString();
	pDataOut->strCertSubject = dataObject.get("cert_subject", "").asString();
	pDataOut->strContractFile = dataObject.get("contract_file", "").asString();
	pDataOut->strMessage = dataObject.get("message", "").asString();
	pDataOut->strSigner = dataObject.get("signer", "").asString();
	pDataOut->strSignTime = dataObject.get("sign_time", "").asString();
	pDataOut->strHash = dataObject.get("hash", "").asString();
	pDataOut->strSignId = dataObject.get("sign_id", "").asString();
	return 0;
}

SIGNHTTP int Http_SignCheckCommon(SignCheckCommonReq* pDataIn, SignCheckResponse* pDataOut)
{
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"common?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/check/",
		CMyGlobal::m_szToken
	);

	Json::Value root;
	Json::FastWriter write;
	root["file_id"] = pDataIn->strFileId;
	std::string strJson = write.write(root);

	CHttp pHttp;
	string strData = pHttp.GetRequestData(szUrl, strJson.c_str());

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", true).asBool();
	pDataOut->strMessage = dataObject.get("message", "").asString();

	Json::Value signCheckResultsObject = dataObject["sign_check_results"];
	if (!signCheckResultsObject.isNull() || signCheckResultsObject.isObject())
	{
		for (size_t i = 0; i < signCheckResultsObject.size(); ++i)
		{
			SignCheckResultsItem pItem;
			pItem.bResult = signCheckResultsObject[i].get("result", true).asBool();
			pItem.nPageNumber = signCheckResultsObject[i].get("page_number", 0).asInt();
			pItem.strCertSn = signCheckResultsObject[i].get("cert_sn", "").asString();
			pItem.strSigner = signCheckResultsObject[i].get("signer", "").asString();
			pItem.strSignTime = signCheckResultsObject[i].get("sign_time", "").asString();
			pItem.strTimestamp = signCheckResultsObject[i].get("timestamp", "").asString();
			pDataOut->SignCheckResultsList.push_back(pItem);
		}
	}
	return 0;
}

SIGNHTTP int Http_SignCheckFile(SignCheckFileReq* pDataIn, SignCheckResponse* pDataOut)
{	
	char szUrl[1024] = { 0 };
	sprintf(szUrl, "%s""%s"
		"file?"
		"&access_token=%s",
		CMyGlobal::m_szUrl,
		"/sign/check/",
		CMyGlobal::m_szToken
	);

	CHttp pHttp;
	string strResponseUtf8Data = pHttp.UploadFile1(szUrl, pDataIn->strFilePath, "");
	string strData = pHttp.UTF8ToGBK(strResponseUtf8Data.c_str());//Utf8中文信息转GB2312

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return HTTP_PARSE_JSON_FAILED;
	}

	string strReturnCode = json_object.get("return_code", "").asString();
	string strReturnMessage = json_object.get("return_message", "").asString();
	if (strReturnCode != "1000")
	{
		return atoi(strReturnCode.c_str());
	}
	string strResultCode = json_object.get("result_code", "").asString();
	string strResultMessage = json_object.get("result_message", "").asString();
	if (strResultCode != "0")
	{
		return atoi(strResultCode.c_str());
	}
	Json::Value dataObject = json_object["data"];
	if (dataObject.isNull() || !dataObject.isObject())
	{
		return HTTP_PARSE_JSON_FAILED;
	}
	pDataOut->bResult = dataObject.get("result", true).asBool();
	pDataOut->strMessage = dataObject.get("message", "").asString();

	Json::Value signCheckResultsObject = dataObject["sign_check_results"];
	if (!signCheckResultsObject.isNull() || signCheckResultsObject.isObject())
	{
		for (size_t i = 0; i < signCheckResultsObject.size(); ++i)
		{
			SignCheckResultsItem pItem;
			pItem.bResult = signCheckResultsObject[i].get("result", true).asBool();
			pItem.nPageNumber = signCheckResultsObject[i].get("page_number", 0).asInt();
			pItem.strCertSn = signCheckResultsObject[i].get("cert_sn", "").asString();
			pItem.strSigner = signCheckResultsObject[i].get("signer", "").asString();
			pItem.strSignTime = signCheckResultsObject[i].get("sign_time", "").asString();
			pItem.strTimestamp = signCheckResultsObject[i].get("timestamp", "").asString();
			pDataOut->SignCheckResultsList.push_back(pItem);
		}
	}
	return 0;
}

