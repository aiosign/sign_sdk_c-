#ifndef DATAENCTOOLDLG_H
#define DATAENCTOOLDLG_H

#include <QDialog>
#include "ui_DataEncToolDlg.h"
#include <string>
using namespace std;


class DataEncToolDlg : public QDialog
{
    Q_OBJECT
public:
    DataEncToolDlg(QWidget *parent = 0);
    ~DataEncToolDlg();

public:
	void sign(const QString& strData, const QString& strKey, QString& strDec);
	void GetToken(); 
	void RegisterPersonal(); //注册个人信息并申请证书
	void GetPersonalUserInfo(); //查询用户信息
	void PersonalLock();  //用户锁定状态
	void PersonalUnlock();  //用户解锁状态
	void RegisterCompany();	//注册企业信息并申请证书
	void GetCompanyUserInfo(); //获取企业详情信息
	void CompanyLock(); //企业状态锁定
	void CompanyUnlock(); //企业状态解锁
	void UploadFileAndSealInfo(); //上传印章并添加印章
	void UploadFileAndContractInfo(); //上传合同并增加合同信息
	void UploadFileAndTemplateInfo();	//上传模版并增加模版信息
	void DownFile();  //下载文件
	void SingleTemplateSign(); //模版单次签章
	void BatchTemplateSign(); //模版批量签章
	void SignCommon(); //坐标签章接口
	void SignKeywordSign();	//关键字签章
	void SignMeetingSingle(); //会签接口
	void SingleTemplateSignEvent(); //模版单次签章证书事件
	void BatchTemplateSignEvent(); //模版批量签章证书事件
	void SignCommonEvent(); //坐标签章接口证书事件
	void SignKeywordSignEvent();	//关键字签章证书事件
	void SignMeetingSingleEvent(); //会签接口证书事件
	void SignOneStepAll(); //一步签章接口
	void SignCheckCommon(); //通过传文件ID，验证文档
	void SignCheckFile();	//通过传入文档，验证文档
	void GetCertinfo(); //获取证书信息
	void GetDownloadReportFile(); //获取下载电子合同存证报告
	void QuerySealInfo();   //查询印章
	void GetSealInfosFromUser(); //查询用户所有的印章
	void RemoveSealInfo();	//删除印章
	void LockSealStatus(); //印章状态锁定
	void UnLockSealStatus(); //印章状态解锁
	void QueryTemplateInfo();   //查询模版
	void DeleteTemplateInfo();	//删除模版
	void LockTemplateStatus(); //模版状态锁定
	void UnLockTemplateStatus(); //模版状态解锁
	void QueryContractInfo(); //查询合同
	void DeleteContractInfo(); //删除合同
	void AbolishContractInfo(); //作废合同
	void ContractRender(); //异步渲染合同
	void ScanContractAdd(); //添加二维码合同接口
	void ScanContractAddEvent(); //添加二维码合同接口事件
	void SignScanSign(); //扫描签章
	void SignScanSignEvent(); //扫描签章事件
	void QueryScanContract(); //查询二维码合同
	void BindContractPhone(); //合同绑定手机号
	void QueryBindContract(); //绑定手机号后查询待签署合同信息
private slots:
	void on_pushButton_token_clicked();
	void on_pushButton_RegisterPersonal_clicked();
	void on_pushButton_GetPersonalUserInfo_clicked();
	void on_pushButton_PersonalLock_clicked();
	void on_pushButton_PersonalUnlock_clicked();
	void on_pushButton_RegisterCompany_clicked();
	void on_pushButton_GetCompanyUserInfo_clicked();
	void on_pushButton_CompanyLock_clicked();
	void on_pushButton_CompanyUnlock_clicked();
	void on_pushButton_UploadFileSeal_clicked();
	void on_pushButton_UploadFileContract_clicked();
	void on_pushButton_UploadFileTemplate_clicked();
	void on_pushButton_SingleTemplateSign_clicked();
	void on_pushButton_BatchTemplateSign_clicked();
	void on_pushButton_SignCommon_clicked();
	void on_pushButton_SignKeywordSign_clicked();
	void on_pushButton_SignMeetingSingle_clicked();
	void on_pushButton_SingleTemplateSignEvent_clicked();
	void on_pushButton_BatchTemplateSignEvent_clicked();
	void on_pushButton_SignCommonEvent_clicked();
	void on_pushButton_SignKeywordSignEvent_clicked();
	void on_pushButton_SignMeetingSingleEvent_clicked();
	void on_pushButton_SignOneStepAll_clicked();
	void on_pushButton_SignCheckCommon_clicked();
	void on_pushButton_SignCheckFile_clicked();
	void on_pushButton_GetCertinfo_clicked();
	void on_pushButton_GetReportFile_clicked();
	void on_pushButton_QuerySealInfo_clicked();
	void on_pushButton_GetSealInfosFromUser_clicked();
	void on_pushButton_RemoveSealInfo_clicked();
	void on_pushButton_LockSealStatus_clicked();
	void on_pushButton_UnLockSealStatus_clicked();
	void on_pushButton_QueryTemplate_clicked();
	void on_pushButton_DeleteTemplate_clicked();
	void on_pushButton_LockTemplate_clicked();
	void on_pushButton_UnLockTemplate_clicked();
	void on_pushButton_QueryContractInfo_clicked();
	void on_pushButton_DeleteContract_clicked();
	void on_pushButton_AbolishContract_clicked();
	void on_pushButton_ContractRender_clicked();
	void on_pushButton_ScanContractAdd_clicked();
	void on_pushButton_ScanContractAddEvent_clicked();
	void on_pushButton_SignScanSign_clicked();
	void on_pushButton_SignScanSignEvent_clicked();
	void on_pushButton_QueryScanContract_clicked();
	void on_pushButton_BindContractPhone_clicked();
	void on_pushButton_QueryBindContract_clicked();
	void on_pushButton_DownFile_clicked();
    void on_pushButton_clear_clicked();
private:
	QString str2qstr(const string str);
	string qstr2str(const QString qstr);
private:
    Ui::DataEncToolDlg *ui;
	QString m_strSign;
	QString m_strToken;
	QString m_strAppSecret;
	bool m_bEnable;
	QString m_strUrl;
};

#endif // SEALIMGREENCRYPTDLG_H
