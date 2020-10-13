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
	AppInfo AppInfo;
	AppInfo.strAppID = "748489100734648320";
	AppInfo.strAppSecret = "ZBvjjigCFjizUwdDny";
	AppInfo.strGrantType = "client_credentials";

	TokenResponse TokenResponse;
	Http_GetToken(&AppInfo, &TokenResponse);
}

void DataEncToolDlg::RegisterPersonal()
{
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
	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	GetPersonalUserInfoResponse UserInfoResponse;
	Http_GetPersonalUserInfo(&UserInfoReqInfo, &UserInfoResponse);
}

void DataEncToolDlg::PersonalLock()
{
	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	UserInfoHandleResponse UserHandleResponse;
	Http_PersonalLock(&UserInfoReqInfo, &UserHandleResponse);
}

void DataEncToolDlg::PersonalUnlock()
{
	UserIdInfoReq UserInfoReqInfo;
	UserInfoReqInfo.strUserId = "00764127286663991296";

	UserInfoHandleResponse UserHandleResponse;
	Http_PersonalUnlock(&UserInfoReqInfo, &UserHandleResponse);
}

void DataEncToolDlg::RegisterCompany()
{
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
	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	GetCompanyInfoResponse GetCompanyInfoResponse;
	Http_GetCompanyUserInfo(&UserIdInfoReq,&GetCompanyInfoResponse);
}

void DataEncToolDlg::CompanyLock()
{
	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	UserInfoHandleResponse UserInfoHandleResponse;
	Http_CompanyLock(&UserIdInfoReq, &UserInfoHandleResponse);
}

void DataEncToolDlg::CompanyUnlock()
{
	UserIdInfoReq UserIdInfoReq;
	UserIdInfoReq.strUserId = "10753989794048659456";

	UserInfoHandleResponse UserInfoHandleResponse;
	Http_CompanyUnlock(&UserIdInfoReq, &UserInfoHandleResponse);
}

void DataEncToolDlg::UploadFileAndSealInfo()
{
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
	DownloadFileInfo pDataIn;
	pDataIn.strFileId = "fce78fb19a24b9f6538d969b78889105";
	pDataIn.strFilePath = "D:\\textco111.pdf";
	Http_DownloadFile(&pDataIn);
}

void DataEncToolDlg::SingleTemplateSign()
{
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
	SignCheckCommonReq pDataIn;
	pDataIn.strFileId = "14cf319c1f06b6306592eb9013788f8a";

	SignCheckResponse pDataOut;
	Http_SignCheckCommon(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignCheckFile()
{
	SignCheckFileReq pDataIn;
	pDataIn.strFilePath = "D:\\11111111111111.pdf";

	SignCheckResponse pDataOut;
	Http_SignCheckFile(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetCertinfo()
{
	CertInfoReq pDataIn;
	pDataIn.strPrepareId = "42f13ccf802f4c0da1d2415e319fafcf";

	GetCertInfoResponse pDataOut;
	Http_GetCertinfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetDownloadReportFile()
{
	DownloadFileInfo pDataIn;
	pDataIn.strFileId = "14cf319c1f06b6306592eb9013788f8a";
	pDataIn.strFilePath = "D:\\22222222222.pdf";
	int nRet =  Http_DownloadFile(&pDataIn);
}

void DataEncToolDlg::QuerySealInfo()
{
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "c1ae0a73077c69e93860d7a25a974e10";
	SealInfo pDataOut;
	Http_QuerySealInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::GetSealInfosFromUser()
{
	UserIdInfoReq pDataIn;
	pDataIn.strUserId = "00753189563652526080";

	SealInfoList pDataOut;
	Http_GetSealInfosFromUser(&pDataIn, &pDataOut);
}

void DataEncToolDlg::RemoveSealInfo()
{
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "56a14c43d7b2a53b7eddf8358d0dacaf";

	SealInfoHandleResponse pDataOut;
	Http_RemoveSealInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::LockSealStatus()
{
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "8858f20873c59e477a329bf9f370cd49";

	SealInfoHandleResponse pDataOut;
	Http_LockSealStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::UnLockSealStatus()
{
	SealIdInfoReq pDataIn;
	pDataIn.strSealId = "8858f20873c59e477a329bf9f370cd49";

	SealInfoHandleResponse pDataOut;
	Http_UnLockSealStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryTemplateInfo()
{
	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateInfo pDataOut;
	Http_QueryTemplateInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::DeleteTemplateInfo()
{
	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_DeleteTemplateInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::LockTemplateStatus()
{
	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_LockTemplateStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::UnLockTemplateStatus()
{
	TemplateIdInfoReq pDataIn;
	pDataIn.strTemplateId = "45327f42e3388aa0baa10ac29b6bf8af";

	TemplateHandleResponse pDataOut;
	Http_UnLockTemplateStatus(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryContractInfo()
{
	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	ContractInfo pDataOut;
	Http_QueryContractInfo(&pDataIn,&pDataOut);
}

void DataEncToolDlg::DeleteContractInfo()
{
	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "f6eb5de0d760538929a78d7c4a85b22f";

	ContractHandleResponse pDataOut;
	Http_DeleteContractInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::AbolishContractInfo()
{
	ContractAbolishReq pDataIn;
	pDataIn.strSignId = "fb5f0849216940d9a728443d2e46f02e";
	pDataIn.strUserId = "00753189563652526080";

	ContractAbolishResponse pDataOut;
	Http_AbolishContractInfo(&pDataIn, &pDataOut);
}

void DataEncToolDlg::ContractRender()
{
	ContractIdInfoReq pDataIn;
	pDataIn.strContractId = "ba7b0b35174ab82395d29b47d4ff90b6";

	ContractRenderResponse pDataOut;
	Http_ContractRender(&pDataIn, &pDataOut);
}

void DataEncToolDlg::ScanContractAdd()
{
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
	SignScanReq pDataIn;
	pDataIn.strPrepareId = "e35674d010084021a2ca6878d5383ec0";

	SignBackInfo pDataOut;
	Http_SignScanSign(&pDataIn, &pDataOut);
}

void DataEncToolDlg::SignScanSignEvent()
{
	SignScanReq pDataIn;
	pDataIn.strPrepareId = "ec2af96a88434b68b089b39cf28c7397";

	SignEventBackResponse pDataOut;
	Http_SignScanSignEvent(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryScanContract()
{
	ScanContractQueryReq pDataIn;
	pDataIn.strPrepareId= "f3edeb7b6372486cb9c5750886d7781f";

	ScanContractInfo pDataOut;
	Http_QueryScanContract(&pDataIn,&pDataOut);
}

void DataEncToolDlg::BindContractPhone()
{
	BindContractPhoneReq pDataIn;
	pDataIn.strContractId = "f1f78e8cf46fe298322c0f3ccdcf9ba4";
	pDataIn.phoneList.push_back("15118014986");
	pDataIn.phoneList.push_back("18682091058");

	BindContractPhoneResponse pDataOut;
	Http_BindContractPhone(&pDataIn, &pDataOut);
}

void DataEncToolDlg::QueryBindContract()
{
	BindContractQueryReq pDataIn;
	pDataIn.strPhone = "15118014986";
	//pDataIn.stContractName = "testcon.pdf";  //非必填

	BindContractInfo pDataOut;
	Http_QueryBindContract(&pDataIn, &pDataOut);
}
