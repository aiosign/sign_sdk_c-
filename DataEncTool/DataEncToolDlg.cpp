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

	QString strPath = QCoreApplication::applicationDirPath();
	QString strTemp = strPath;
	strTemp.replace("/", "\\");
	strTemp += "\\";

	Http_InitLog(strTemp.toStdString().c_str(), "SignHttp");

	QString cfgFileName = strPath + "/DataEncTool.ini";
	QSettings *pSetting = new QSettings(cfgFileName, QSettings::IniFormat);
	if (pSetting)
	{
		pSetting->setIniCodec("UTF-8");
		m_strUrl = pSetting->value("HTTPIP/IP").toString();
	}
	delete pSetting;
	//m_strToken = "5241cc79-7481-4ab6-bbfd-d04a7210837f";
	m_bEnable = false;

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
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + "/oauth/token";
	
	//Json::Value root;
	//Json::FastWriter write;
	//root["app_id"] = "748489100734648320";
	//root["app_secret"] = "ZBvjjigCFjizUwdDny";
	//root["grant_type"] = "client_credentials";

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_GetToken(strUrl.toStdString().c_str(), strJson.c_str(), strData);

	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return;
	}
	string strToken = fileData.get("access_token", "").asString();
	m_strToken = str2qstr(strToken);

	Json::Value json_object1;
	if (!reader.parse(strJson.c_str(), json_object1))
	{
		return;
	}
	string strAppSecret = json_object1.get("app_secret", "").asString();
	m_strAppSecret = str2qstr(strAppSecret);
	m_bEnable = true;
}

void DataEncToolDlg::RegisterPersonal()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/personal/register?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_name"] = "测试003";
	//root["area_code"] = "370101";
	//root["phone"] = "15118014986";
	//root["id_type"] = "111";
	//root["id_number"] = "370101198001016111";
	//root["mail"] = "1562310354@qq.com";
	//root["description"] = "测试003";

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_RegisterPersonalInfo(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::GetPersonalUserInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/personal/userinfo?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_GetPersonalUserInfo(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::PersonalLock()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/personal/lock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_PersonalLock(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::PersonalUnlock()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/personal/unlock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_PersonalUnlock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::RegisterCompany()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/company/register?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_name"] = "测试西乡塘法人用户003";
	//root["area_code"] = "370101";
	//root["unit_type"] = "03";
	//root["credi_code"] = "913700006135889860";
	//root["legal_name"] = "测试法人用户";
	//root["legal_id_number"] = "371314288798766356";
	//root["legal_id_type"] = "111";
	//root["legal_phone"] = "15118014986";
	//root["legal_email"] = "12388373@qq.com";
	//root["agent_name"] = "测试经办人";
	//root["agent_id_number"] = "326565678765465789";
	//root["agent_id_type"] = "111";
	//root["agent_phone"] = "15118014986";
	//root["agent_email"] = "18762873638@qq.com";
	//root["description"] = "测试企业用户002";

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_RegisterCompanyInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::GetCompanyUserInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/company/userinfo?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "10753989794048659456";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_GetCompanyUserInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::CompanyLock()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/company/lock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "10753989794048659456";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_CompanyLock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::CompanyUnlock()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/user/company/unlock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "10753989794048659456";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_CompanyUnlock(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::UploadFileAndSealInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;

	//root["file_path"] = "F:\\Projects\\SignHttp\\4501070057171.png";
	//root["file_name"] = "4501070057171.png";
	//root["file_type"] = "impression";
	//root["user_id"] = "00753189563652526080";

	//Json::Value sealInfo;
	//sealInfo["seal_name"] = "测试西乡塘流程数据修改财务专用章01";
	//sealInfo["seal_type"] = "02";
	//sealInfo["size"] = "38*38";
	//sealInfo["description"] = "测试";
	//root["sealInfo"] = sealInfo;

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_UploadFileAndSealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::UploadFileAndContractInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["file_path"] = "E:\\testcon.pdf";
	//root["file_name"] = "testcon.pdf";
	//root["file_type"] = "contract";
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_UploadFileAndContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::UploadFileAndTemplateInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/file/upload?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["file_path"] = "E:\\textTemplate1.pdf";
	//root["file_name"] = "textTemplate1.pdf";
	//root["file_type"] = "template";
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_UploadFileAndTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::DownFile()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = strBaseUrl + QString("/file/download?access_token=%1&fileId=551b63d5dd85b160d599c0117317460a").arg(m_strToken);

	//int nRet = Http_DownloadFile(strUrl.toStdString().c_str(),"D:\\textco111.pdf");
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	std::string strPath = qstr2str(ui->EnctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存路径！"));
		return;
	}
	int nRet = Http_DownloadFile(strJson.c_str(), strPath.c_str());
}

void DataEncToolDlg::SingleTemplateSign()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/template/single?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	//Json::Value fields;

	//Json::Value sign_params;
	//Json::Value item;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_key"] = "sign1";
	//item["user_id"] = "00753189563652526080";
	//item["width"] = 113;
	//item["height"] = 113;
	//sign_params.append(item);

	//Json::Value text_params;
	//Json::Value item1;
	//item1["key"] = "name1";
	//item1["value"] = "测试";
	//text_params.append(item1);

	//fields["sign_params"] = sign_params;
	//fields["text_params"] = text_params;

	//root["sign_field"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SingleTemplateSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::BatchTemplateSign()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/template/batch?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	//Json::Value templates;

	//Json::Value templatesItem;
	//Json::Value sign_params;
	//Json::Value item;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_key"] = "sign1";
	//item["user_id"] = "00753189563652526080";
	//item["width"] = 113;
	//item["height"] = 113;
	//sign_params.append(item);

	//Json::Value text_params;
	//Json::Value item1;
	//item1["key"] = "name1";
	//item1["value"] = "测试";
	//text_params.append(item1);

	//templatesItem["custom_id"] = "12";
	//templatesItem["sign_params"] = sign_params;
	//templatesItem["text_params"] = text_params;

	//Json::Value templatesItem1;
	//Json::Value sign_params1;
	//Json::Value item3;
	//item3["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item3["sign_key"] = "sign2";
	//item3["user_id"] = "00753189563652526080";
	//item3["width"] = 113;
	//item3["height"] = 113;
	//sign_params1.append(item3);

	//Json::Value text_params1;
	//Json::Value item4;
	//item4["key"] = "name2";
	//item4["value"] = "测试";
	//text_params1.append(item4);


	//templatesItem1["custom_id"] = "13";
	//templatesItem1["sign_params"] = sign_params1;
	//templatesItem1["text_params"] = text_params1;

	//templates.append(templatesItem);
	//templates.append(templatesItem1);

	//root["batch_templates"] = templates;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_BatchTemplateSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignCommon()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/common?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	//root["user_id"] = "00753189563652526080";
	//root["remark"] = "test";

	//Json::Value fields;
	//Json::Value item;
	//item["height"] = 113;
	//item["horizontal"] = 150;
	//item["page_number"] = 2;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["vertical"] = 100;
	//item["width"] = 113;
	//fields.append(item);

	//root["fields"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignCommon(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignKeywordSign()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/keywordSign?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	//root["user_id"] = "00753189563652526080";
	//root["keyword"] = "11";
	//root["height"] = 113;
	//root["sign_all"] = false;
	//root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//root["width"] = 113;
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_SignKeywordSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignMeetingSingle()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/meeting/single?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";

	//Json::Value fields;
	//Json::Value item;
	//item["sign_width"] = 113;
	//item["sign_top"] = 33;
	//item["page_num"] = 3;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_height"] = 113;
	//item["sign_left"] = 12;
	//item["user_id"] = "00753189563652526080";
	//fields.append(item);

	//root["sign_details"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_SignMeetingSingle(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SingleTemplateSignEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/template/single?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	//Json::Value fields;

	//Json::Value sign_params;
	//Json::Value item;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_key"] = "sign1";
	//item["user_id"] = "00753189563652526080";
	//item["width"] = 113;
	//item["height"] = 113;
	//sign_params.append(item);

	//Json::Value text_params;
	//Json::Value item1;
	//item1["key"] = "name1";
	//item1["value"] = "测试";
	//text_params.append(item1);

	//fields["sign_params"] = sign_params;
	//fields["text_params"] = text_params;

	//root["sign_field"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SingleTemplateSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::BatchTemplateSignEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/template/batch?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";

	//Json::Value templates;

	//Json::Value templatesItem;
	//Json::Value sign_params;
	//Json::Value item;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_key"] = "sign1";
	//item["user_id"] = "00753189563652526080";
	//item["width"] = 113;
	//item["height"] = 113;
	//sign_params.append(item);

	//Json::Value text_params;
	//Json::Value item1;
	//item1["key"] = "name1";
	//item1["value"] = "测试";
	//text_params.append(item1);

	//templatesItem["custom_id"] = "12";
	//templatesItem["sign_params"] = sign_params;
	//templatesItem["text_params"] = text_params;

	//Json::Value templatesItem1;
	//Json::Value sign_params1;
	//Json::Value item3;
	//item3["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item3["sign_key"] = "sign2";
	//item3["user_id"] = "00753189563652526080";
	//item3["width"] = 113;
	//item3["height"] = 113;
	//sign_params1.append(item3);

	//Json::Value text_params1;
	//Json::Value item4;
	//item4["key"] = "name2";
	//item4["value"] = "测试";
	//text_params1.append(item4);


	//templatesItem1["custom_id"] = "13";
	//templatesItem1["sign_params"] = sign_params1;
	//templatesItem1["text_params"] = text_params1;

	//templates.append(templatesItem);
	//templates.append(templatesItem1);

	//root["batch_templates"] = templates;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_BatchTemplateSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignCommonEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/common?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	//root["user_id"] = "00753189563652526080";
	//root["remark"] = "test";

	//Json::Value fields;
	//Json::Value item;
	//item["height"] = 113;
	//item["horizontal"] = 150;
	//item["page_number"] = 2;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["vertical"] = 100;
	//item["width"] = 113;
	//fields.append(item);

	//root["fields"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignCommonEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignKeywordSignEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/keywordSign?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";
	//root["user_id"] = "00753189563652526080";
	//root["keyword"] = "11";
	//root["height"] = 113;
	//root["sign_all"] = false;
	//root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//root["width"] = 113;

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_SignKeywordSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignMeetingSingleEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/meeting/single?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "50a0c4ceb5f9b36a49b761decf911c14";

	//Json::Value fields;
	//Json::Value item;
	//item["sign_width"] = 113;
	//item["sign_top"] = 33;
	//item["page_num"] = 3;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["sign_height"] = 113;
	//item["sign_left"] = 12;
	//item["user_id"] = "00753189563652526080";
	//fields.append(item);

	//root["sign_details"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignMeetingSingleEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignOneStepAll()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/directSign?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;

	//QFile fichero("F:\\Projects\\SignHttp\\test03.pdf", this);
	//QByteArray qbFileData;
	//if (fichero.open(QIODevice::ReadOnly))
	//{
	//	qbFileData = fichero.readAll().toBase64();
	//}
	//fichero.close();

	//QFile fichero1("F:\\Projects\\SignHttp\\4501070057171.png", this);
	//QByteArray qbMouleData;
	//if (fichero1.open(QIODevice::ReadOnly))
	//{
	//	qbMouleData = fichero1.readAll().toBase64();
	//}
	//fichero1.close();

	//root["user_name"] = "测试用户003";
	//root["id_number"] = "370101198001016736";
	//root["user_type"] = "1";
	//root["contract_file_content"] = qbFileData.data();
	//

	//Json::Value fields;
	//Json::Value item;
	//item["seal_height"] = 113;
	//item["horizontal"] = 150;
	//item["page_num"] = 2;
	//item["seal_file_content"] = qbMouleData.data();
	//item["vertical"] = 130;
	//item["seal_width"] = 113;
	//fields.append(item);

	//root["sign_fields"] = fields;

	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignOneStepAll(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return;
	}
	string strFileData = fileData.get("contract_file", "").asString();
	const char* szFileData = strFileData.c_str();
	QByteArray qbFileData2(szFileData);
	QFile fichero2("F:\\Projects\\SignHttp\\111.pdf", this);
	if (fichero2.open(QIODevice::WriteOnly))
	{
		fichero2.write(QByteArray::fromBase64(qbFileData2));
	}
	fichero2.close();
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignCheckCommon()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/check/common?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["file_id"] = "14cf319c1f06b6306592eb9013788f8a";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignCheckCommon(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignCheckFile()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";

	Json::Value root;
	Json::FastWriter write;

	QString strUrl = m_strUrl + QString("/sign/check/file?access_token=%1").arg(m_strToken);
	//QString strPath = "D:\\11111111111111.pdf";

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}

	string strData;
	Http_SignCheckFile(strUrl.toStdString().c_str(), "", /*strPath.toStdString().c_str()*/strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::GetCertinfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/cert/certinfo?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["prepare_id"] = "";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_GetCertinfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::GetDownloadReportFile()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	//QString strUrl = strBaseUrl + QString("/file/downloadReport?access_token=%1&fileId=14cf319c1f06b6306592eb9013788f8a").arg(m_strToken);

	//int nRet = Http_DownloadFile(strUrl.toStdString().c_str(), "D:\\22222222222.pdf");

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	std::string strPath = qstr2str(ui->EnctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存路径！"));
		return;
	}
	int nRet = Http_DownloadFile(strJson.c_str(), strPath.c_str());
}

void DataEncToolDlg::QuerySealInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/seal/query?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_QuerySealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::GetSealInfosFromUser()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/seal/getSealInfos?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_GetSealInfosFromUser(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::RemoveSealInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/seal/remove?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_RemoveSealInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::LockSealStatus()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/seal/lock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	//std::string strJson = write.write(root);

	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_LockSealStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::UnLockSealStatus()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/seal/unlock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["seal_id"] = "56a14c43d7b2a53b7eddf8358d0dacaf";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_UnLockSealStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::QueryTemplateInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/template/query?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_QueryTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::DeleteTemplateInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/template/delete?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_DeleteTemplateInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::LockTemplateStatus()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/template/lock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_LockTemplateStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::UnLockTemplateStatus()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/template/unlock?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["template_id"] = "45327f42e3388aa0baa10ac29b6bf8af";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_UnLockTemplateStatus(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::QueryContractInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/contract/query?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_QueryContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::DeleteContractInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/contract/remove?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_DeleteContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::AbolishContractInfo()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/contract/abolish?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["sign_id"] = "fb5f0849216940d9a728443d2e46f02e";
	//root["user_id"] = "00753189563652526080";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_AbolishContractInfo(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::ContractRender()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/contract/render?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f6eb5de0d760538929a78d7c4a85b22f";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_ContractRender(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::ScanContractAdd()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/scan/contract/add?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	//root["user_id"] = "00753189563652526080";
	//root["url"] = "https://www.baidu.com/";
	//root["expire_time"] = "2020-12-10 23:59:00";
	//root["qr_code_width"] = 125;
	//root["qr_code_height"] = 125;
	//root["remark"] = "测试";

	//Json::Value fields;
	//Json::Value item;
	//item["height"] = 113;
	//item["horizontal"] = 150;
	//item["page_number"] = 2;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["vertical"] = 100;
	//item["width"] = 113;
	//fields.append(item);

	//root["fields"] = fields;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_ScanContractAdd(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);

	Json::Reader reader(Json::Features::strictMode());
	Json::Value json_object;
	if (!reader.parse(strData.c_str(), json_object))
	{
		return;
	}
	Json::Value fileData = json_object["data"];
	if (fileData.isNull() || !fileData.isObject())
	{
		return;
	}
	string strPrepareId = fileData.get("prepare_id", "").asString();
	string strImageData = fileData.get("qr", "").asString();
	strImageData = strImageData.substr((strImageData.find(","))+1);
	const char* szFileData = strImageData.c_str();
	QByteArray qbFileData2(szFileData);
	QFile fichero2("F:\\Projects\\SignHttp\\qr.png", this);
	if (fichero2.open(QIODevice::WriteOnly))
	{
		fichero2.write(QByteArray::fromBase64(qbFileData2));
	}
	fichero2.close();
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::ScanContractAddEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/scan/event_cert_contract/add?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	//root["user_id"] = "00753189563652526080";
	//root["url"] = "https://www.baidu.com/";
	//root["expire_time"] = "2020-12-10 23:59:00";
	//root["qr_code_width"] = 125;
	//root["qr_code_height"] = 125;
	//root["remark"] = "测试";

	//Json::Value fields;
	//Json::Value item;
	//item["height"] = 113;
	//item["horizontal"] = 150;
	//item["page_number"] = 2;
	//item["seal_id"] = "c1ae0a73077c69e93860d7a25a974e10";
	//item["vertical"] = 100;
	//item["width"] = 113;
	//fields.append(item);

	//root["fields"] = fields;
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_ScanCertContractAdd(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignScanSign()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/sign/scanSign?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["prepare_id"] = "ccaa318688454f9babe57eee6a329b14";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignScanSign(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::SignScanSignEvent()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/event_cert_sign/scanSign?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["prepare_id"] = "ccaa318688454f9babe57eee6a329b14";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_SignScanSignEvent(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::QueryScanContract()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/scan/contract/query?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["prepare_id"] = "f3edeb7b6372486cb9c5750886d7781f";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_QueryScanContract(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::BindContractPhone()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/bind_contract/bind_contract_phone?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["contract_id"] = "f1f78e8cf46fe298322c0f3ccdcf9ba4";

	//Json::Value params;

	//Json::Value item;
	//item["phone"] = "15118014986";
	//params.append(item);

	//Json::Value item1;
	//item1["phone"] = "18682091058";
	//params.append(item1);

	//root["params"] = params;

	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_BindContractPhone(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}

void DataEncToolDlg::QueryBindContract()
{
	//QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = m_strUrl + QString("/bind_contract/query_bind_contract?access_token=%1").arg(m_strToken);

	//Json::Value root;
	//Json::FastWriter write;
	//root["phone"] = "15118014986";
	////root["contract_name"] = "testcon.pdf";
	//std::string strJson = write.write(root);
	std::string strJson = qstr2str(ui->SrctextEdit->toPlainText());
	if (strJson.empty())
	{
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("发送数据不能为空！"));
		return;
	}
	string strData;
	Http_QueryBindContract(strUrl.toStdString().c_str(), m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}
