#include "DataEncToolDlg.h"
#include <QDebug>
#include <QMessageBox>
#include <QMessageAuthenticationCode>
#include <QSettings>
#include "funexport.h"
#include "SignHttp.h"
#include <QFile>
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

DataEncToolDlg::DataEncToolDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataEncToolDlg)
{
    ui->setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	Http_Init();

}

DataEncToolDlg::~DataEncToolDlg()
{
    delete ui;
}

void DataEncToolDlg::on_pushButton_token_clicked()
{
	GetToken();
}

void DataEncToolDlg::on_pushButton_RegisterPersonal_clicked()
{
	RegisterPersonal();
}

void DataEncToolDlg::on_pushButton_GetPersonalUserInfo_clicked()
{
	GetPersonalUserInfo();
}

void DataEncToolDlg::on_pushButton_PersonalLock_clicked()
{
	PersonalLock();
}

void DataEncToolDlg::on_pushButton_PersonalUnlock_clicked()
{
	PersonalUnlock();
}

void DataEncToolDlg::on_pushButton_RegisterCompany_clicked()
{
	RegisterCompany();
}

void DataEncToolDlg::on_pushButton_GetCompanyUserInfo_clicked()
{
	GetCompanyUserInfo();
}

void DataEncToolDlg::on_pushButton_CompanyLock_clicked()
{
	CompanyLock();
}

void DataEncToolDlg::on_pushButton_CompanyUnlock_clicked()
{
	CompanyUnlock();
}

void DataEncToolDlg::on_pushButton_UploadFileSeal_clicked()
{
	UploadFileAndSealInfo();
}

void DataEncToolDlg::on_pushButton_UploadFileContract_clicked()
{
	UploadFileAndContractInfo();
}

void DataEncToolDlg::on_pushButton_UploadFileTemplate_clicked()
{
	UploadFileAndTemplateInfo();
}

void DataEncToolDlg::on_pushButton_SingleTemplateSign_clicked()
{
	SingleTemplateSign();
}

void DataEncToolDlg::on_pushButton_BatchTemplateSign_clicked()
{
	BatchTemplateSign();
}

void DataEncToolDlg::on_pushButton_SignCommon_clicked()
{
	SignCommon();
}

void DataEncToolDlg::on_pushButton_SignKeywordSign_clicked()
{
	SignKeywordSign();
}

void DataEncToolDlg::on_pushButton_SignMeetingSingle_clicked()
{
	SignMeetingSingle();
}

void DataEncToolDlg::on_pushButton_SingleTemplateSignEvent_clicked()
{
	SingleTemplateSignEvent();
}

void DataEncToolDlg::on_pushButton_BatchTemplateSignEvent_clicked()
{
	BatchTemplateSignEvent();
}

void DataEncToolDlg::on_pushButton_SignCommonEvent_clicked()
{
	SignCommonEvent();
}

void DataEncToolDlg::on_pushButton_SignKeywordSignEvent_clicked()
{
	SignKeywordSignEvent();
}

void DataEncToolDlg::on_pushButton_SignMeetingSingleEvent_clicked()
{
	SignMeetingSingleEvent();
}

void DataEncToolDlg::on_pushButton_SignOneStepAll_clicked()
{
	SignOneStepAll();
}

void DataEncToolDlg::on_pushButton_SignCheckCommon_clicked()
{
	SignCheckCommon();
}

void DataEncToolDlg::on_pushButton_SignCheckFile_clicked()
{
	SignCheckFile();
}

void DataEncToolDlg::on_pushButton_GetCertinfo_clicked()
{
	GetCertinfo();
}

void DataEncToolDlg::on_pushButton_GetReportFile_clicked()
{
	GetDownloadReportFile();
}

void DataEncToolDlg::on_pushButton_QuerySealInfo_clicked()
{
	QuerySealInfo();
}

void DataEncToolDlg::on_pushButton_GetSealInfosFromUser_clicked()
{
	GetSealInfosFromUser();
}

void DataEncToolDlg::on_pushButton_RemoveSealInfo_clicked()
{
	RemoveSealInfo();
}

void DataEncToolDlg::on_pushButton_LockSealStatus_clicked()
{
	LockSealStatus();
}

void DataEncToolDlg::on_pushButton_UnLockSealStatus_clicked()
{
	UnLockSealStatus();
}

void DataEncToolDlg::on_pushButton_QueryTemplate_clicked()
{
	QueryTemplateInfo();
}

void DataEncToolDlg::on_pushButton_DeleteTemplate_clicked()
{
	DeleteTemplateInfo();
}

void DataEncToolDlg::on_pushButton_LockTemplate_clicked()
{
	LockTemplateStatus();
}

void DataEncToolDlg::on_pushButton_UnLockTemplate_clicked()
{
	UnLockTemplateStatus();
}

void DataEncToolDlg::on_pushButton_QueryContractInfo_clicked()
{
	QueryContractInfo();
}

void DataEncToolDlg::on_pushButton_DeleteContract_clicked()
{
	DeleteContractInfo();
}

void DataEncToolDlg::on_pushButton_AbolishContract_clicked()
{
	AbolishContractInfo();
}

void DataEncToolDlg::on_pushButton_ContractRender_clicked()
{
	ContractRender();
}

void DataEncToolDlg::on_pushButton_ScanContractAdd_clicked()
{
	ScanContractAdd();
}

void DataEncToolDlg::on_pushButton_ScanContractAddEvent_clicked()
{
	ScanContractAddEvent();
}

void DataEncToolDlg::on_pushButton_SignScanSign_clicked()
{
	SignScanSign();
}

void DataEncToolDlg::on_pushButton_SignScanSignEvent_clicked()
{
	SignScanSignEvent();
}

void DataEncToolDlg::on_pushButton_QueryScanContract_clicked()
{
	QueryScanContract();
}

void DataEncToolDlg::on_pushButton_BindContractPhone_clicked()
{
	BindContractPhone();
	
}

void DataEncToolDlg::on_pushButton_QueryBindContract_clicked()
{
	QueryBindContract();
}

void DataEncToolDlg::on_pushButton_DownFile_clicked()
{
	DownFile();
}

void DataEncToolDlg::on_pushButton_clear_clicked()
{
	ui->SrctextEdit->clear();
	ui->EnctextEdit->clear();
}

QString DataEncToolDlg::str2qstr(const string str)
{
	return QString::fromLocal8Bit(str.data());
}

string DataEncToolDlg::qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);

}

void DataEncToolDlg::sign(const QString & strData, const QString& strKey, QString & strDec)
{
	QByteArray skey = strKey.toLocal8Bit();
	QString strDataTemp = strData.mid(0,strData.lastIndexOf("}")+1);
	QByteArray info = strDataTemp.toLocal8Bit();
	ui->SrctextEdit->setText(info);
	QByteArray base64 = QMessageAuthenticationCode::hash(info, skey, QCryptographicHash::Sha256).toBase64();
	QByteArray bamd5 = QCryptographicHash::hash(base64, QCryptographicHash::Md5).toHex();
	strDec = bamd5.toUpper();
}

void DataEncToolDlg::GetToken()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + "/oauth/token";
	//
	////Json::Value root;
	////Json::FastWriter write;
	////root["app_id"] = "748489100734648320";
	////root["app_secret"] = "ZBvjjigCFjizUwdDny";
	////root["grant_type"] = "client_credentials";

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_GetToken(strUrl.toStdString().c_str(), strJson.c_str(), strData);

	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	//Json::Reader reader(Json::Features::strictMode());
	//Json::Value json_object;
	//if (!reader.parse(strData.c_str(), json_object))
	//{
	//	return;
	//}
	//Json::Value fileData = json_object["data"];
	//if (fileData.isNull() || !fileData.isObject())
	//{
	//	return;
	//}
	//string strToken = fileData.get("access_token", "").asString();
	//m_strToken = str2qstr(strToken);

	//Json::Value json_object1;
	//if (!reader.parse(strJson.c_str(), json_object1))
	//{
	//	return;
	//}
	//string strAppSecret = json_object1.get("app_secret", "").asString();
	//m_strAppSecret = str2qstr(strAppSecret);
	//m_bEnable = true;
	AppInfo AppInfo;
	AppInfo.strAppID = "748489100734648320";
	AppInfo.strAppSecret = "ZBvjjigCFjizUwdDny";
	AppInfo.strGrantType = "client_credentials";

	//string strData;
	TokenResponse TokenResponse;
	Http_GetToken(&AppInfo, &TokenResponse);

	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::RegisterPersonal()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/personal/register?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_name"] = "测试003";
	////root["area_code"] = "370101";
	////root["phone"] = "15118014986";
	////root["id_type"] = "111";
	////root["id_number"] = "370101198001016111";
	////root["mail"] = "1562310354@qq.com";
	////root["description"] = "测试003";

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_RegisterPersonalInfo(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	PersonalUserInfo RegisterInfo;
	RegisterInfo.strUserName = "测试005";
	RegisterInfo.strAreaCode = "370101";
	RegisterInfo.strPhone = "15118014986";
	RegisterInfo.strTypeId = "111";
	RegisterInfo.strNumberId = "370101198001019996";
	RegisterInfo.strMail = "1562310354@qq.com";
	RegisterInfo.strDescription = "测试005";

	RegisterResponse RegisterResponse;
	Http_RegisterPersonalInfo(&RegisterInfo, &RegisterResponse);
}

void DataEncToolDlg::GetPersonalUserInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/personal/userinfo?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_GetPersonalUserInfo(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	GetPersonalUserInfoResponse UserInfoResponse;
	Http_GetPersonalUserInfo(&UserInfoReqInfo, &UserInfoResponse);
}

void DataEncToolDlg::PersonalLock()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/personal/lock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_PersonalLock(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	UserInfoHandleResponse UserHandleResponse;
	Http_PersonalLock(&UserInfoReqInfo, &UserHandleResponse);
}

void DataEncToolDlg::PersonalUnlock()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/personal/unlock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_PersonalUnlock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	UserInfoHandleResponse UserHandleResponse;
	Http_PersonalUnlock(&UserInfoReqInfo, &UserHandleResponse);
}

void DataEncToolDlg::RegisterCompany()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/company/register?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_name"] = "测试西乡塘法人用户003";
	////root["area_code"] = "370101";
	////root["unit_type"] = "03";
	////root["credi_code"] = "913700006135889860";
	////root["legal_name"] = "测试法人用户";
	////root["legal_id_number"] = "371314288798766356";
	////root["legal_id_type"] = "111";
	////root["legal_phone"] = "15118014986";
	////root["legal_email"] = "12388373@qq.com";
	////root["agent_name"] = "测试经办人";
	////root["agent_id_number"] = "326565678765465789";
	////root["agent_id_type"] = "111";
	////root["agent_phone"] = "15118014986";
	////root["agent_email"] = "18762873638@qq.com";
	////root["description"] = "测试企业用户002";

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_RegisterCompanyInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	CompanyInfo CompanyInfo;
	CompanyInfo.strUserName = "测试西乡塘法人用户003";
	CompanyInfo.strAreaCode = "370101";
	CompanyInfo.strUnitType = "03";
	CompanyInfo.strCrediCode = "913700006135889860";
	CompanyInfo.strLegalName = "测试法人用户";
	CompanyInfo.strLegalIdNum = "371314288798766356";
	CompanyInfo.strLegalIdType = "111";
	CompanyInfo.strLegalPhone = "15118014986";
	CompanyInfo.strLegalMail = "12388373@qq.com";
	CompanyInfo.strAgentName = "测试经办人";
	CompanyInfo.strAgentIdNum = "326565678765465789";
	CompanyInfo.strAgentIdType = "111";
	CompanyInfo.strAgentPhone = "15118014986";
	CompanyInfo.strAgentMail = "18762873638@qq.com";
	CompanyInfo.strDescription = "测试企业用户003";

	RegisterResponse RegisterResponse;
	Http_RegisterCompanyInfo(&CompanyInfo, &RegisterResponse);

}

void DataEncToolDlg::GetCompanyUserInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/company/userinfo?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "10753989794048659456";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_GetCompanyUserInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	GetCompanyInfoResponse GetCompanyInfoResponse;
	Http_GetCompanyUserInfo(&UserIdInfoReq,&GetCompanyInfoResponse);
}

void DataEncToolDlg::CompanyLock()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/company/lock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "10753989794048659456";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_CompanyLock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	UserInfoHandleResponse UserInfoHandleResponse;
	Http_CompanyLock(&UserIdInfoReq, &UserInfoHandleResponse);
}

void DataEncToolDlg::CompanyUnlock()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/user/company/unlock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "10753989794048659456";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_CompanyUnlock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	UserInfoHandleResponse UserInfoHandleResponse;
	Http_CompanyUnlock(&UserIdInfoReq, &UserInfoHandleResponse);
}

void DataEncToolDlg::UploadFileAndSealInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;

	////root["file_path"] = "F:\\Projects\\SignHttp\\4501070057171.png";
	////root["file_name"] = "4501070057171.png";
	////root["file_type"] = "impression";
	////root["user_id"] = "00753189563652526080";

	////Json::Value sealInfo;
	////sealInfo["seal_name"] = "测试西乡塘流程数据修改财务专用章01";
	////sealInfo["seal_type"] = "02";
	////sealInfo["size"] = "38*38";
	////sealInfo["description"] = "测试";
	////root["sealInfo"] = sealInfo;

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_UploadFileAndSealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	UploadAndAddSealInfoReq UploadAndAddSealInfoReq;
	UploadAndAddSealInfoReq.FileInfo.strFilePath = "F:\\Projects\\SignHttp\\4501070057171.png";
	UploadAndAddSealInfoReq.FileInfo.strFileName = "4501070057171.png";
	UploadAndAddSealInfoReq.FileInfo.strFileType = "impression";
	UploadAndAddSealInfoReq.FileInfo.strUserId = "00753189563652526080";
	
	UploadAndAddSealInfoReq.SealInfo.strSealName = "测试西乡塘流程数据修改财务专用章01";
	UploadAndAddSealInfoReq.SealInfo.strSealType = "02";
	UploadAndAddSealInfoReq.SealInfo.strSealSize = "38*38";
	UploadAndAddSealInfoReq.SealInfo.strDescription = "测试";

	UploadAndAddSealInfoResponse UploadAndAddSealInfoResponse;

	Http_UploadFileAndSealInfo(&UploadAndAddSealInfoReq, &UploadAndAddSealInfoResponse);

}

void DataEncToolDlg::UploadFileAndContractInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["file_path"] = "E:\\testcon.pdf";
	////root["file_name"] = "testcon.pdf";
	////root["file_type"] = "contract";
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_UploadFileAndContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	FileInfo pDataIn;
	pDataIn.strFilePath = "E:\\testcon.pdf";
	pDataIn.strFileName = "testcon.pdf";
	pDataIn.strFileType = "contract";
	pDataIn.strUserId = "00753189563652526080";

	UploadAndAddContractInfoResponse pDataOut;
	Http_UploadFileAndContractInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::UploadFileAndTemplateInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["file_path"] = "E:\\textTemplate1.pdf";
	////root["file_name"] = "textTemplate1.pdf";
	////root["file_type"] = "template";
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_UploadFileAndTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	FileInfo pDataIn;
	pDataIn.strFilePath = "E:\\textTemplate1.pdf";
	pDataIn.strFileName = "textTemplate1.pdf";
	pDataIn.strFileType = "template";
	pDataIn.strUserId = "00753189563652526080";

	UploadAndAddTemplateInfoResponse pDataOut;
	Http_UploadFileAndTemplateInfo(&pDataIn,&pDataOut);
}

void DataEncToolDlg::DownFile()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	////QString strUrl = strBaseUrl + QString("/file/download?access_token=%1&fileId=551b63d5dd85b160d599c0117317460a").arg(m_strToken);

	////int nRet = Http_DownloadFile(strUrl.toStdString().c_str(),"D:\\textco111.pdf");
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//std::string strPath = qstr2str(ui->EnctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存路径！"));
	//	return;
	//}
	//int nRet = Http_DownloadFile(strJson.c_str(), strPath.c_str());
	DownloadFileInfo pDataIn;
	pDataIn.strFileId = "fce78fb19a24b9f6538d969b78889105";
	pDataIn.strFilePath = "D:\\textco111.pdf";
	Http_DownloadFile(&pDataIn);
}

void DataEncToolDlg::SingleTemplateSign()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/template/single?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	////Json::Value fields;

	////Json::Value sign_params;
	////Json::Value item;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_key"] = "sign1";
	////item["user_id"] = "00753189563652526080";
	////item["width"] = 113;
	////item["height"] = 113;
	////sign_params.append(item);

	////Json::Value text_params;
	////Json::Value item1;
	////item1["key"] = "name1";
	////item1["value"] = "测试";
	////text_params.append(item1);

	////fields["sign_params"] = sign_params;
	////fields["text_params"] = text_params;

	////root["sign_field"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SingleTemplateSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SingleTemplateSignReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	SignParamItem SignParamItem;
	SignParamItem.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem.strSignKey = "sign1";
	SignParamItem.strUserId = "00753189563652526080";
	SignParamItem.width = 113;
	SignParamItem.height = 113;
	pDataIn.SignParamList.push_back(SignParamItem);

	TextParamItem TextParamItem;
	TextParamItem.strKey = "name1";
	TextParamItem.strValue = "测试";
	pDataIn.TextParamList.push_back(TextParamItem);

	SignBackInfo pDataOut;
	Http_SingleTemplateSign(&pDataIn, &pDataOut);
}

void DataEncToolDlg::BatchTemplateSign()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/template/batch?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	////Json::Value templates;

	////Json::Value templatesItem;
	////Json::Value sign_params;
	////Json::Value item;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_key"] = "sign1";
	////item["user_id"] = "00753189563652526080";
	////item["width"] = 113;
	////item["height"] = 113;
	////sign_params.append(item);

	////Json::Value text_params;
	////Json::Value item1;
	////item1["key"] = "name1";
	////item1["value"] = "测试";
	////text_params.append(item1);

	////templatesItem["custom_id"] = "12";
	////templatesItem["sign_params"] = sign_params;
	////templatesItem["text_params"] = text_params;

	////Json::Value templatesItem1;
	////Json::Value sign_params1;
	////Json::Value item3;
	////item3["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item3["sign_key"] = "sign2";
	////item3["user_id"] = "00753189563652526080";
	////item3["width"] = 113;
	////item3["height"] = 113;
	////sign_params1.append(item3);

	////Json::Value text_params1;
	////Json::Value item4;
	////item4["key"] = "name2";
	////item4["value"] = "测试";
	////text_params1.append(item4);


	////templatesItem1["custom_id"] = "13";
	////templatesItem1["sign_params"] = sign_params1;
	////templatesItem1["text_params"] = text_params1;

	////templates.append(templatesItem);
	////templates.append(templatesItem1);

	////root["batch_templates"] = templates;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_BatchTemplateSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	BatchTemplateSignReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	SignParamList SignParamList1;
	SignParamItem SignParamItem1;
	SignParamItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem1.strSignKey = "sign1";
	SignParamItem1.strUserId = "00753189563652526080";
	SignParamItem1.width = 113;
	SignParamItem1.height = 113;
	SignParamList1.push_back(SignParamItem1);

	SignParamList SignParamList2;
	SignParamItem SignParamItem2;
	SignParamItem2.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem2.strSignKey = "sign2";
	SignParamItem2.strUserId = "00753189563652526080";
	SignParamItem2.width = 113;
	SignParamItem2.height = 113;
	SignParamList2.push_back(SignParamItem2);

	TextParamList TextParamList1;
	TextParamItem TextParamItem1;
	TextParamItem1.strKey = "name1";
	TextParamItem1.strValue = "测试";
	TextParamList1.push_back(TextParamItem1);

	TextParamList TextParamList2;
	TextParamItem TextParamItem2;
	TextParamItem2.strKey = "name2";
	TextParamItem2.strValue = "测试";
	TextParamList2.push_back(TextParamItem2);

	BatchSignItem BatchSignItem1;
	BatchSignItem1.strcCustomId = "12";
	BatchSignItem1.SignParamList = SignParamList1;
	BatchSignItem1.TextParamList = TextParamList1;
	pDataIn.BatchSignList.push_back(BatchSignItem1);

	BatchSignItem BatchSignItem2;
	BatchSignItem2.strcCustomId = "13";
	BatchSignItem2.SignParamList = SignParamList2;
	BatchSignItem2.TextParamList = TextParamList2;
	pDataIn.BatchSignList.push_back(BatchSignItem2);

	BatchTemplateSignResponse pDataOut;
	Http_BatchTemplateSign(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignCommon()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/common?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	////root["user_id"] = "00753189563652526080";
	////root["remark"] = "test";

	////Json::Value fields;
	////Json::Value item;
	////item["height"] = 113;
	////item["horizontal"] = 150;
	////item["page_number"] = 2;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["vertical"] = 100;
	////item["width"] = 113;
	////fields.append(item);

	////root["fields"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignCommon(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SignCommonReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strRemark = "test";

	FileDetailItem FileDetailItem;
	FileDetailItem.height = 113;
	FileDetailItem.horizontal = 150;
	FileDetailItem.nPageNumber = 2;
	FileDetailItem.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	FileDetailItem.vertical = 100;
	FileDetailItem.width = 113;
	pDataIn.FileDetailList.push_back(FileDetailItem);

	SignBackInfo pDataOut;
	Http_SignCommon(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignKeywordSign()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/keywordSign?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	////root["user_id"] = "00753189563652526080";
	////root["keyword"] = "11";
	////root["height"] = 113;
	////root["sign_all"] = false;
	////root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////root["width"] = 113;
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_SignKeywordSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignKeywordReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strKeyword = "11";
	pDataIn.height = 113;
	pDataIn.bSignAll = false;
	pDataIn.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	pDataIn.width = 113;

	SignBackInfo pDataOut;
	Http_SignKeywordSign(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignMeetingSingle()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/meeting/single?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";

	////Json::Value fields;
	////Json::Value item;
	////item["sign_width"] = 113;
	////item["sign_top"] = 33;
	////item["page_num"] = 3;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_height"] = 113;
	////item["sign_left"] = 12;
	////item["user_id"] = "00753189563652526080";
	////fields.append(item);

	////root["sign_details"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_SignMeetingSingle(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignMeetingSingleReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	MeetingSignItem MeetingSignItem1;
	MeetingSignItem1.width = 113;
	MeetingSignItem1.nSignTop = 33;
	MeetingSignItem1.nPageNum = 3;
	MeetingSignItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	MeetingSignItem1.height = 113;
	MeetingSignItem1.nSignLeft = 12;
	MeetingSignItem1.strUserId = "00753189563652526080";
	pDataIn.MeetingSignList.push_back(MeetingSignItem1);

	SignBackInfo pDataOut;
	Http_SignMeetingSingle(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SingleTemplateSignEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/template/single?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	////Json::Value fields;

	////Json::Value sign_params;
	////Json::Value item;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_key"] = "sign1";
	////item["user_id"] = "00753189563652526080";
	////item["width"] = 113;
	////item["height"] = 113;
	////sign_params.append(item);

	////Json::Value text_params;
	////Json::Value item1;
	////item1["key"] = "name1";
	////item1["value"] = "测试";
	////text_params.append(item1);

	////fields["sign_params"] = sign_params;
	////fields["text_params"] = text_params;

	////root["sign_field"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SingleTemplateSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SingleTemplateSignReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	SignParamItem SignParamItem;
	SignParamItem.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem.strSignKey = "sign1";
	SignParamItem.strUserId = "00753189563652526080";
	SignParamItem.width = 113;
	SignParamItem.height = 113;
	pDataIn.SignParamList.push_back(SignParamItem);

	TextParamItem TextParamItem;
	TextParamItem.strKey = "name1";
	TextParamItem.strValue = "测试";
	pDataIn.TextParamList.push_back(TextParamItem);

	SignEventBackResponse pDataOut;
	Http_SingleTemplateSignEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::BatchTemplateSignEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/template/batch?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	////Json::Value templates;

	////Json::Value templatesItem;
	////Json::Value sign_params;
	////Json::Value item;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_key"] = "sign1";
	////item["user_id"] = "00753189563652526080";
	////item["width"] = 113;
	////item["height"] = 113;
	////sign_params.append(item);

	////Json::Value text_params;
	////Json::Value item1;
	////item1["key"] = "name1";
	////item1["value"] = "测试";
	////text_params.append(item1);

	////templatesItem["custom_id"] = "12";
	////templatesItem["sign_params"] = sign_params;
	////templatesItem["text_params"] = text_params;

	////Json::Value templatesItem1;
	////Json::Value sign_params1;
	////Json::Value item3;
	////item3["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item3["sign_key"] = "sign2";
	////item3["user_id"] = "00753189563652526080";
	////item3["width"] = 113;
	////item3["height"] = 113;
	////sign_params1.append(item3);

	////Json::Value text_params1;
	////Json::Value item4;
	////item4["key"] = "name2";
	////item4["value"] = "测试";
	////text_params1.append(item4);


	////templatesItem1["custom_id"] = "13";
	////templatesItem1["sign_params"] = sign_params1;
	////templatesItem1["text_params"] = text_params1;

	////templates.append(templatesItem);
	////templates.append(templatesItem1);

	////root["batch_templates"] = templates;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_BatchTemplateSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	BatchTemplateSignReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	SignParamList SignParamList1;
	SignParamItem SignParamItem1;
	SignParamItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem1.strSignKey = "sign1";
	SignParamItem1.strUserId = "00753189563652526080";
	SignParamItem1.width = 113;
	SignParamItem1.height = 113;
	SignParamList1.push_back(SignParamItem1);

	SignParamList SignParamList2;
	SignParamItem SignParamItem2;
	SignParamItem2.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SignParamItem2.strSignKey = "sign2";
	SignParamItem2.strUserId = "00753189563652526080";
	SignParamItem2.width = 113;
	SignParamItem2.height = 113;
	SignParamList2.push_back(SignParamItem2);

	TextParamList TextParamList1;
	TextParamItem TextParamItem1;
	TextParamItem1.strKey = "name1";
	TextParamItem1.strValue = "测试";
	TextParamList1.push_back(TextParamItem1);

	TextParamList TextParamList2;
	TextParamItem TextParamItem2;
	TextParamItem2.strKey = "name2";
	TextParamItem2.strValue = "测试";
	TextParamList2.push_back(TextParamItem2);

	BatchSignItem BatchSignItem1;
	BatchSignItem1.strcCustomId = "12";
	BatchSignItem1.SignParamList = SignParamList1;
	BatchSignItem1.TextParamList = TextParamList1;
	pDataIn.BatchSignList.push_back(BatchSignItem1);

	BatchSignItem BatchSignItem2;
	BatchSignItem2.strcCustomId = "13";
	BatchSignItem2.SignParamList = SignParamList2;
	BatchSignItem2.TextParamList = TextParamList2;
	pDataIn.BatchSignList.push_back(BatchSignItem2);

	BatchSignEventBackResponse pDataOut;
	Http_BatchTemplateSignEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignCommonEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/common?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	////root["user_id"] = "00753189563652526080";
	////root["remark"] = "test";

	////Json::Value fields;
	////Json::Value item;
	////item["height"] = 113;
	////item["horizontal"] = 150;
	////item["page_number"] = 2;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["vertical"] = 100;
	////item["width"] = 113;
	////fields.append(item);

	////root["fields"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignCommonEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignCommonReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strRemark = "test";

	FileDetailItem FileDetailItem;
	FileDetailItem.height = 113;
	FileDetailItem.horizontal = 150;
	FileDetailItem.nPageNumber = 2;
	FileDetailItem.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	FileDetailItem.vertical = 100;
	FileDetailItem.width = 113;
	pDataIn.FileDetailList.push_back(FileDetailItem);

	SignEventBackResponse pDataOut;
	Http_SignCommonEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignKeywordSignEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/keywordSign?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	////root["user_id"] = "00753189563652526080";
	////root["keyword"] = "11";
	////root["height"] = 113;
	////root["sign_all"] = false;
	////root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////root["width"] = 113;

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_SignKeywordSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignKeywordReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strKeyword = "11";
	pDataIn.height = 113;
	pDataIn.bSignAll = false;
	pDataIn.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	pDataIn.width = 113;

	SignEventBackResponse pDataOut;
	Http_SignKeywordSignEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignMeetingSingleEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/meeting/single?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";

	////Json::Value fields;
	////Json::Value item;
	////item["sign_width"] = 113;
	////item["sign_top"] = 33;
	////item["page_num"] = 3;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["sign_height"] = 113;
	////item["sign_left"] = 12;
	////item["user_id"] = "00753189563652526080";
	////fields.append(item);

	////root["sign_details"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignMeetingSingleEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SignMeetingSingleReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	MeetingSignItem MeetingSignItem1;
	MeetingSignItem1.width = 113;
	MeetingSignItem1.nSignTop = 33;
	MeetingSignItem1.nPageNum = 3;
	MeetingSignItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	MeetingSignItem1.height = 113;
	MeetingSignItem1.nSignLeft = 12;
	MeetingSignItem1.strUserId = "00753189563652526080";
	pDataIn.MeetingSignList.push_back(MeetingSignItem1);

	SignEventBackResponse pDataOut;
	Http_SignMeetingSingleEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignOneStepAll()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/directSign?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;

	////QFile fichero("F:\\Projects\\SignHttp\\test03.pdf", this);
	////QByteArray qbFileData;
	////if (fichero.open(QIODevice::ReadOnly))
	////{
	////	qbFileData = fichero.readAll().toBase64();
	////}
	////fichero.close();

	////QFile fichero1("F:\\Projects\\SignHttp\\4501070057171.png", this);
	////QByteArray qbMouleData;
	////if (fichero1.open(QIODevice::ReadOnly))
	////{
	////	qbMouleData = fichero1.readAll().toBase64();
	////}
	////fichero1.close();

	////root["user_name"] = "测试用户003";
	////root["id_number"] = "370101198001016736";
	////root["user_type"] = "1";
	////root["contract_file_content"] = qbFileData.data();
	////

	////Json::Value fields;
	////Json::Value item;
	////item["seal_height"] = 113;
	////item["horizontal"] = 150;
	////item["page_num"] = 2;
	////item["seal_file_content"] = qbMouleData.data();
	////item["vertical"] = 130;
	////item["seal_width"] = 113;
	////fields.append(item);

	////root["sign_fields"] = fields;

	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignOneStepAll(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);

	//Json::Reader reader(Json::Features::strictMode());
	//Json::Value json_object;
	//if (!reader.parse(strData.c_str(), json_object))
	//{
	//	return;
	//}
	//Json::Value fileData = json_object["data"];
	//if (fileData.isNull() || !fileData.isObject())
	//{
	//	return;
	//}
	//string strFileData = fileData.get("contract_file", "").asString();
	//const char* szFileData = strFileData.c_str();
	//QByteArray qbFileData2(szFileData);
	//QFile fichero2("F:\\Projects\\SignHttp\\111.pdf", this);
	//if (fichero2.open(QIODevice::WriteOnly))
	//{
	//	fichero2.write(QByteArray::fromBase64(qbFileData2));
	//}
	//fichero2.close();
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	QFile fichero("F:\\Projects\\SignHttp\\test03.pdf", this);
	QByteArray qbFileData;
	if (fichero.open(QIODevice::ReadOnly))
	{
		qbFileData = fichero.readAll().toBase64();
	}
	fichero.close();

	QFile fichero1("F:\\Projects\\SignHttp\\4501070057171.png", this);
	QByteArray qbMouleData;
	if (fichero1.open(QIODevice::ReadOnly))
	{
		qbMouleData = fichero1.readAll().toBase64();
	}
	fichero1.close();

	SignOneStepAllReq pDataIn;
	pDataIn.strUserName = "测试用户003";
	pDataIn.strIdNum = "370101198001016736";
	pDataIn.strUserType = "1";
	pDataIn.strContractFileContent = qbFileData.data();

	FileContentDetailItem FileContentDetailItem1;
	FileContentDetailItem1.height = 113;
	FileContentDetailItem1.horizontal = 150;
	FileContentDetailItem1.nPageNumber = 2;
	FileContentDetailItem1.strSealFileContent = qbMouleData.data();
	FileContentDetailItem1.vertical = 130;
	FileContentDetailItem1.width = 113;
	pDataIn.FileContentDetailList.push_back(FileContentDetailItem1);

	SignOneStepAllResponse pDataOut;
	Http_SignOneStepAll(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignCheckCommon()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/check/common?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["file_id"] = "14cf319c1f06b6306592eb9013788f8a";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignCheckCommon(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignCheckCommonReq pDataIn;
	pDataIn.strFileId = "14cf319c1f06b6306592eb9013788f8a";

	SignCheckResponse pDataOut;
	Http_SignCheckCommon(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignCheckFile()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";

	//Json::Value root;
	//Json::FastWriter write;

	//QString strUrl = m_strUrl + QString("/sign/check/file?access_token=%1").arg(m_strToken);
	////QString strPath = "D:\\11111111111111.pdf";

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}

	//string strData;
	//Http_SignCheckFile(strUrl.toStdString().c_str(), "", /*strPath.toStdString().c_str()*/strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SignCheckFileReq pDataIn;
	pDataIn.strFilePath = "D:\\11111111111111.pdf";

	SignCheckResponse pDataOut;
	Http_SignCheckFile(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetCertinfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/cert/certinfo?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["prepare_id"] = "";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_GetCertinfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	CertInfoReq pDataIn;
	pDataIn.strPrepareId = "42f13ccf802f4c0da1d2415e319fafcf";

	GetCertInfoResponse pDataOut;
	Http_GetCertinfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetDownloadReportFile()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	////QString strUrl = strBaseUrl + QString("/file/downloadReport?access_token=%1&fileId=14cf319c1f06b6306592eb9013788f8a").arg(m_strToken);

	////int nRet = Http_DownloadFile(strUrl.toStdString().c_str(), "D:\\22222222222.pdf");

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//std::string strPath = qstr2str(ui->EnctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存路径！"));
	//	return;
	//}
	//int nRet = Http_DownloadFile(strJson.c_str(), strPath.c_str());
	DownloadFileInfo pDataIn;
	pDataIn.strFileId = "14cf319c1f06b6306592eb9013788f8a";
	pDataIn.strFilePath = "D:\\22222222222.pdf";
	int nRet =  Http_DownloadFile(&pDataIn);
}

void DataEncToolDlg::QuerySealInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/seal/query?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_QuerySealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SealInfo pDataOut;
	Http_QuerySealInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetSealInfosFromUser()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/seal/getSealInfos?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_GetSealInfosFromUser(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	UserIdInfoReq pDataIn;
	pDataIn.strUserId = "00753189563652526080";

	SealInfoList pDataOut;
	Http_GetSealInfosFromUser(&pDataIn, &pDataOut);
}

void DataEncToolDlg::RemoveSealInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/seal/remove?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_RemoveSealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "56a14c43d7b2a53b7eddf8358d0dacaf";

	SealInfoHandleResponse pDataOut;
	Http_RemoveSealInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::LockSealStatus()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/seal/lock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	////std::string strJson = write.write(root);

	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_LockSealStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "8858f20873c59e477a329bf9f370cd49";

	SealInfoHandleResponse pDataOut;
	Http_LockSealStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::UnLockSealStatus()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/seal/unlock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_UnLockSealStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "8858f20873c59e477a329bf9f370cd49";

	SealInfoHandleResponse pDataOut;
	Http_UnLockSealStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryTemplateInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/template/query?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_QueryTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateInfo pDataOut;
	Http_QueryTemplateInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::DeleteTemplateInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/template/delete?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_DeleteTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_DeleteTemplateInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::LockTemplateStatus()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/template/lock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_LockTemplateStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_LockTemplateStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::UnLockTemplateStatus()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/template/unlock?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_UnLockTemplateStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_UnLockTemplateStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryContractInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/contract/query?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_QueryContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	ContractInfo pDataOut;
	Http_QueryContractInfo(&pDataIn,&pDataOut);
}

void DataEncToolDlg::DeleteContractInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/contract/remove?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_DeleteContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "f6eb5de0d760538929a78d7c4a85b22f";

	ContractHandleResponse pDataOut;
	Http_DeleteContractInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::AbolishContractInfo()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/contract/abolish?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["sign_id"] = "fb5f0849216940d9a728443d2e46f02e";
	////root["user_id"] = "00753189563652526080";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_AbolishContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ContractAbolishReq pDataIn;
	pDataIn.strSignId = "fb5f0849216940d9a728443d2e46f02e";
	pDataIn.strUserId = "00753189563652526080";

	ContractAbolishResponse pDataOut;
	Http_AbolishContractInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::ContractRender()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/contract/render?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_ContractRender(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	ContractRenderResponse pDataOut;
	Http_ContractRender(&pDataIn, &pDataOut);
}

void DataEncToolDlg::ScanContractAdd()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/scan/contract/add?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	////root["user_id"] = "00753189563652526080";
	////root["url"] = "https://www.baidu.com/";
	////root["expire_time"] = "2020-12-10 23:59:00";
	////root["qr_code_width"] = 125;
	////root["qr_code_height"] = 125;
	////root["remark"] = "测试";

	////Json::Value fields;
	////Json::Value item;
	////item["height"] = 113;
	////item["horizontal"] = 150;
	////item["page_number"] = 2;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["vertical"] = 100;
	////item["width"] = 113;
	////fields.append(item);

	////root["fields"] = fields;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_ScanContractAdd(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);

	//Json::Reader reader(Json::Features::strictMode());
	//Json::Value json_object;
	//if (!reader.parse(strData.c_str(), json_object))
	//{
	//	return;
	//}
	//Json::Value fileData = json_object["data"];
	//if (fileData.isNull() || !fileData.isObject())
	//{
	//	return;
	//}
	//string strPrepareId = fileData.get("prepare_id", "").asString();
	//string strImageData = fileData.get("qr", "").asString();
	//strImageData = strImageData.substr((strImageData.find(","))+1);
	//const char* szFileData = strImageData.c_str();
	//QByteArray qbFileData2(szFileData);
	//QFile fichero2("F:\\Projects\\SignHttp\\qr.png", this);
	//if (fichero2.open(QIODevice::WriteOnly))
	//{
	//	fichero2.write(QByteArray::fromBase64(qbFileData2));
	//}
	//fichero2.close();
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ScanContractAddReq pDataIn;
	pDataIn.strContractId = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strUrl = "https://www.baidu.com/";
	pDataIn.strExpireTime = "2020-12-10 23:59:00";
	pDataIn.nQrCodeWidth = 125;
	pDataIn.nQrcCodeHeight = 125;
	pDataIn.strRemark = "测试";

	FileDetailItem FileDetailItem1;
	FileDetailItem1.height = 113;
	FileDetailItem1.horizontal = 150;
	FileDetailItem1.nPageNumber = 2;
	FileDetailItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	FileDetailItem1.vertical = 100;
	FileDetailItem1.width = 113;
	pDataIn.FileDetailList.push_back(FileDetailItem1);

	ScanContractAddResponse pDataOut;
	Http_ScanContractAdd(&pDataIn, &pDataOut);
}

void DataEncToolDlg::ScanContractAddEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/scan/event_cert_contract/add?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	////root["user_id"] = "00753189563652526080";
	////root["url"] = "https://www.baidu.com/";
	////root["expire_time"] = "2020-12-10 23:59:00";
	////root["qr_code_width"] = 125;
	////root["qr_code_height"] = 125;
	////root["remark"] = "测试";

	////Json::Value fields;
	////Json::Value item;
	////item["height"] = 113;
	////item["horizontal"] = 150;
	////item["page_number"] = 2;
	////item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	////item["vertical"] = 100;
	////item["width"] = 113;
	////fields.append(item);

	////root["fields"] = fields;
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_ScanCertContractAdd(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ScanContractAddReq pDataIn;
	pDataIn.strContractId = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	pDataIn.strUserId = "00753189563652526080";
	pDataIn.strUrl = "https://www.baidu.com/";
	pDataIn.strExpireTime = "2020-12-10 23:59:00";
	pDataIn.nQrCodeWidth = 125;
	pDataIn.nQrcCodeHeight = 125;
	pDataIn.strRemark = "测试";

	FileDetailItem FileDetailItem1;
	FileDetailItem1.height = 113;
	FileDetailItem1.horizontal = 150;
	FileDetailItem1.nPageNumber = 2;
	FileDetailItem1.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	FileDetailItem1.vertical = 100;
	FileDetailItem1.width = 113;
	pDataIn.FileDetailList.push_back(FileDetailItem1);

	ScanContractAddResponse pDataOut;
	Http_ScanCertContractAdd(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignScanSign()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/sign/scanSign?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["prepare_id"] = "ccaa318688454f9babe57eee6a329b14";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignScanSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	SignScanReq pDataIn;
	pDataIn.strPrepareId = "e35674d010084021a2ca6878d5383ec0";

	SignBackInfo pDataOut;
	Http_SignScanSign(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignScanSignEvent()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/event_cert_sign/scanSign?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["prepare_id"] = "ccaa318688454f9babe57eee6a329b14";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_SignScanSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	SignScanReq pDataIn;
	pDataIn.strPrepareId = "ec2af96a88434b68b089b39cf28c7397";

	SignEventBackResponse pDataOut;
	Http_SignScanSignEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryScanContract()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/scan/contract/query?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["prepare_id"] = "f3edeb7b6372486cb9c5750886d7781f";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_QueryScanContract(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	ScanContractQueryReq pDataIn;
	pDataIn.strPrepareId= "f3edeb7b6372486cb9c5750886d7781f";

	ScanContractInfo pDataOut;
	Http_QueryScanContract(&pDataIn,&pDataOut);
}

void DataEncToolDlg::BindContractPhone()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/bind_contract/bind_contract_phone?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";

	////Json::Value params;

	////Json::Value item;
	////item["phone"] = "15118014986";
	////params.append(item);

	////Json::Value item1;
	////item1["phone"] = "18682091058";
	////params.append(item1);

	////root["params"] = params;

	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_BindContractPhone(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));
	BindContractPhoneReq pDataIn;
	pDataIn.strContractId = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	pDataIn.phoneList.push_back("15118014986");
	pDataIn.phoneList.push_back("18682091058");

	BindContractPhoneResponse pDataOut;
	Http_BindContractPhone(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryBindContract()
{
	////QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = m_strUrl + QString("/bind_contract/query_bind_contract?access_token=%1").arg(m_strToken);

	////Json::Value root;
	////Json::FastWriter write;
	////root["phone"] = "15118014986";
	//////root["contract_name"] = "testcon.pdf";
	////std::string strJson = write.write(root);
	//std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	//if (strJson.empty())
	//{
	//	QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
	//	return;
	//}
	//string strData;
	//Http_QueryBindContract(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	//ui->EnctextEdit->clear();
	//ui->EnctextEdit->setText(str2qstr(strData));

	BindContractQueryReq pDataIn;
	pDataIn.strPhone = "15118014986";
	//pDataIn.stContractName = "testcon.pdf";  //非必填

	BindContractInfo pDataOut;
	Http_QueryBindContract(&pDataIn, &pDataOut);
}
