/********************************************************************************
** Form generated from reading UI file 'DataEncToolDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAENCTOOLDLG_H
#define UI_DATAENCTOOLDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DataEncToolDlg
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QTextEdit *SrctextEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QTextEdit *EnctextEdit;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *pushButton_token;
    QPushButton *pushButton_RegisterPersonal;
    QPushButton *pushButton_GetPersonalUserInfo;
    QPushButton *pushButton_PersonalLock;
    QPushButton *pushButton_PersonalUnlock;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_RegisterCompany;
    QPushButton *pushButton_GetCompanyUserInfo;
    QPushButton *pushButton_CompanyLock;
    QPushButton *pushButton_CompanyUnlock;
    QPushButton *pushButton_UploadFileSeal;
    QPushButton *pushButton_UploadFileContract;
    QPushButton *pushButton_UploadFileTemplate;
    QPushButton *pushButton_SingleTemplateSign;
    QPushButton *pushButton_BatchTemplateSign;
    QPushButton *pushButton_SignCommon;
    QPushButton *pushButton_SignKeywordSign;
    QPushButton *pushButton_SignMeetingSingle;
    QPushButton *pushButton_SignMeetingSingleEvent;
    QPushButton *pushButton_SignOneStepAll;
    QPushButton *pushButton_SignCheckCommon;
    QPushButton *pushButton_SignCheckFile;
    QPushButton *pushButton_GetCertinfo;
    QPushButton *pushButton_QuerySealInfo;
    QPushButton *pushButton_GetSealInfosFromUser;
    QPushButton *pushButton_RemoveSealInfo;
    QPushButton *pushButton_LockSealStatus;
    QPushButton *pushButton_UnLockSealStatus;
    QPushButton *pushButton_QueryTemplate;
    QPushButton *pushButton_DeleteTemplate;
    QPushButton *pushButton_LockTemplate;
    QPushButton *pushButton_UnLockTemplate;
    QPushButton *pushButton_QueryContractInfo;
    QPushButton *pushButton_DeleteContract;
    QPushButton *pushButton_AbolishContract;
    QPushButton *pushButton_ContractRender;
    QPushButton *pushButton_ScanContractAdd;
    QPushButton *pushButton_ScanContractAddEvent;
    QPushButton *pushButton_SignScanSign;
    QPushButton *pushButton_SignScanSignEvent;
    QPushButton *pushButton_QueryScanContract;
    QPushButton *pushButton_BindContractPhone;
    QPushButton *pushButton_QueryBindContract;
    QPushButton *pushButton_DownFile;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_SingleTemplateSignEvent;
    QPushButton *pushButton_BatchTemplateSignEvent;
    QPushButton *pushButton_SignCommonEvent;
    QPushButton *pushButton_SignKeywordSignEvent;
    QPushButton *pushButton_GetReportFile;

    void setupUi(QDialog *DataEncToolDlg)
    {
        if (DataEncToolDlg->objectName().isEmpty())
            DataEncToolDlg->setObjectName(QStringLiteral("DataEncToolDlg"));
        DataEncToolDlg->resize(758, 554);
        DataEncToolDlg->setMinimumSize(QSize(90, 25));
        verticalLayout_3 = new QVBoxLayout(DataEncToolDlg);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(DataEncToolDlg);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        SrctextEdit = new QTextEdit(DataEncToolDlg);
        SrctextEdit->setObjectName(QStringLiteral("SrctextEdit"));

        horizontalLayout->addWidget(SrctextEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(DataEncToolDlg);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        EnctextEdit = new QTextEdit(DataEncToolDlg);
        EnctextEdit->setObjectName(QStringLiteral("EnctextEdit"));

        horizontalLayout_2->addWidget(EnctextEdit);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(verticalLayout);

        label = new QLabel(DataEncToolDlg);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_token = new QPushButton(DataEncToolDlg);
        pushButton_token->setObjectName(QStringLiteral("pushButton_token"));
        pushButton_token->setMinimumSize(QSize(90, 25));
        pushButton_token->setMaximumSize(QSize(80, 30));

        gridLayout->addWidget(pushButton_token, 0, 0, 1, 1);

        pushButton_RegisterPersonal = new QPushButton(DataEncToolDlg);
        pushButton_RegisterPersonal->setObjectName(QStringLiteral("pushButton_RegisterPersonal"));
        pushButton_RegisterPersonal->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_RegisterPersonal, 0, 1, 1, 1);

        pushButton_GetPersonalUserInfo = new QPushButton(DataEncToolDlg);
        pushButton_GetPersonalUserInfo->setObjectName(QStringLiteral("pushButton_GetPersonalUserInfo"));
        pushButton_GetPersonalUserInfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_GetPersonalUserInfo, 0, 2, 1, 1);

        pushButton_PersonalLock = new QPushButton(DataEncToolDlg);
        pushButton_PersonalLock->setObjectName(QStringLiteral("pushButton_PersonalLock"));
        pushButton_PersonalLock->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_PersonalLock, 0, 3, 1, 1);

        pushButton_PersonalUnlock = new QPushButton(DataEncToolDlg);
        pushButton_PersonalUnlock->setObjectName(QStringLiteral("pushButton_PersonalUnlock"));
        pushButton_PersonalUnlock->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_PersonalUnlock, 0, 4, 1, 1);

        pushButton_clear = new QPushButton(DataEncToolDlg);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));
        pushButton_clear->setMinimumSize(QSize(90, 25));
        pushButton_clear->setMaximumSize(QSize(80, 30));

        gridLayout->addWidget(pushButton_clear, 0, 5, 1, 1);

        pushButton_RegisterCompany = new QPushButton(DataEncToolDlg);
        pushButton_RegisterCompany->setObjectName(QStringLiteral("pushButton_RegisterCompany"));
        pushButton_RegisterCompany->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_RegisterCompany, 1, 0, 1, 1);

        pushButton_GetCompanyUserInfo = new QPushButton(DataEncToolDlg);
        pushButton_GetCompanyUserInfo->setObjectName(QStringLiteral("pushButton_GetCompanyUserInfo"));
        pushButton_GetCompanyUserInfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_GetCompanyUserInfo, 1, 1, 1, 1);

        pushButton_CompanyLock = new QPushButton(DataEncToolDlg);
        pushButton_CompanyLock->setObjectName(QStringLiteral("pushButton_CompanyLock"));
        pushButton_CompanyLock->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_CompanyLock, 1, 2, 1, 1);

        pushButton_CompanyUnlock = new QPushButton(DataEncToolDlg);
        pushButton_CompanyUnlock->setObjectName(QStringLiteral("pushButton_CompanyUnlock"));
        pushButton_CompanyUnlock->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_CompanyUnlock, 1, 3, 1, 1);

        pushButton_UploadFileSeal = new QPushButton(DataEncToolDlg);
        pushButton_UploadFileSeal->setObjectName(QStringLiteral("pushButton_UploadFileSeal"));
        pushButton_UploadFileSeal->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_UploadFileSeal, 1, 4, 1, 1);

        pushButton_UploadFileContract = new QPushButton(DataEncToolDlg);
        pushButton_UploadFileContract->setObjectName(QStringLiteral("pushButton_UploadFileContract"));
        pushButton_UploadFileContract->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_UploadFileContract, 1, 5, 1, 1);

        pushButton_UploadFileTemplate = new QPushButton(DataEncToolDlg);
        pushButton_UploadFileTemplate->setObjectName(QStringLiteral("pushButton_UploadFileTemplate"));
        pushButton_UploadFileTemplate->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_UploadFileTemplate, 2, 0, 1, 1);

        pushButton_SingleTemplateSign = new QPushButton(DataEncToolDlg);
        pushButton_SingleTemplateSign->setObjectName(QStringLiteral("pushButton_SingleTemplateSign"));
        pushButton_SingleTemplateSign->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SingleTemplateSign, 2, 1, 1, 1);

        pushButton_BatchTemplateSign = new QPushButton(DataEncToolDlg);
        pushButton_BatchTemplateSign->setObjectName(QStringLiteral("pushButton_BatchTemplateSign"));
        pushButton_BatchTemplateSign->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_BatchTemplateSign, 2, 2, 1, 1);

        pushButton_SignCommon = new QPushButton(DataEncToolDlg);
        pushButton_SignCommon->setObjectName(QStringLiteral("pushButton_SignCommon"));
        pushButton_SignCommon->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignCommon, 2, 3, 1, 1);

        pushButton_SignKeywordSign = new QPushButton(DataEncToolDlg);
        pushButton_SignKeywordSign->setObjectName(QStringLiteral("pushButton_SignKeywordSign"));
        pushButton_SignKeywordSign->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignKeywordSign, 2, 4, 1, 1);

        pushButton_SignMeetingSingle = new QPushButton(DataEncToolDlg);
        pushButton_SignMeetingSingle->setObjectName(QStringLiteral("pushButton_SignMeetingSingle"));
        pushButton_SignMeetingSingle->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignMeetingSingle, 2, 5, 1, 1);

        pushButton_SignMeetingSingleEvent = new QPushButton(DataEncToolDlg);
        pushButton_SignMeetingSingleEvent->setObjectName(QStringLiteral("pushButton_SignMeetingSingleEvent"));
        pushButton_SignMeetingSingleEvent->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignMeetingSingleEvent, 3, 0, 1, 1);

        pushButton_SignOneStepAll = new QPushButton(DataEncToolDlg);
        pushButton_SignOneStepAll->setObjectName(QStringLiteral("pushButton_SignOneStepAll"));
        pushButton_SignOneStepAll->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignOneStepAll, 3, 1, 1, 1);

        pushButton_SignCheckCommon = new QPushButton(DataEncToolDlg);
        pushButton_SignCheckCommon->setObjectName(QStringLiteral("pushButton_SignCheckCommon"));
        pushButton_SignCheckCommon->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(pushButton_SignCheckCommon, 3, 2, 1, 1);

        pushButton_SignCheckFile = new QPushButton(DataEncToolDlg);
        pushButton_SignCheckFile->setObjectName(QStringLiteral("pushButton_SignCheckFile"));
        pushButton_SignCheckFile->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignCheckFile, 3, 3, 1, 1);

        pushButton_GetCertinfo = new QPushButton(DataEncToolDlg);
        pushButton_GetCertinfo->setObjectName(QStringLiteral("pushButton_GetCertinfo"));
        pushButton_GetCertinfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_GetCertinfo, 3, 4, 1, 1);

        pushButton_QuerySealInfo = new QPushButton(DataEncToolDlg);
        pushButton_QuerySealInfo->setObjectName(QStringLiteral("pushButton_QuerySealInfo"));
        pushButton_QuerySealInfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_QuerySealInfo, 3, 5, 1, 1);

        pushButton_GetSealInfosFromUser = new QPushButton(DataEncToolDlg);
        pushButton_GetSealInfosFromUser->setObjectName(QStringLiteral("pushButton_GetSealInfosFromUser"));
        pushButton_GetSealInfosFromUser->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_GetSealInfosFromUser, 4, 0, 1, 1);

        pushButton_RemoveSealInfo = new QPushButton(DataEncToolDlg);
        pushButton_RemoveSealInfo->setObjectName(QStringLiteral("pushButton_RemoveSealInfo"));
        pushButton_RemoveSealInfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_RemoveSealInfo, 4, 1, 1, 1);

        pushButton_LockSealStatus = new QPushButton(DataEncToolDlg);
        pushButton_LockSealStatus->setObjectName(QStringLiteral("pushButton_LockSealStatus"));
        pushButton_LockSealStatus->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_LockSealStatus, 4, 2, 1, 1);

        pushButton_UnLockSealStatus = new QPushButton(DataEncToolDlg);
        pushButton_UnLockSealStatus->setObjectName(QStringLiteral("pushButton_UnLockSealStatus"));
        pushButton_UnLockSealStatus->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_UnLockSealStatus, 4, 3, 1, 1);

        pushButton_QueryTemplate = new QPushButton(DataEncToolDlg);
        pushButton_QueryTemplate->setObjectName(QStringLiteral("pushButton_QueryTemplate"));
        pushButton_QueryTemplate->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_QueryTemplate, 4, 4, 1, 1);

        pushButton_DeleteTemplate = new QPushButton(DataEncToolDlg);
        pushButton_DeleteTemplate->setObjectName(QStringLiteral("pushButton_DeleteTemplate"));
        pushButton_DeleteTemplate->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_DeleteTemplate, 4, 5, 1, 1);

        pushButton_LockTemplate = new QPushButton(DataEncToolDlg);
        pushButton_LockTemplate->setObjectName(QStringLiteral("pushButton_LockTemplate"));
        pushButton_LockTemplate->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_LockTemplate, 5, 0, 1, 1);

        pushButton_UnLockTemplate = new QPushButton(DataEncToolDlg);
        pushButton_UnLockTemplate->setObjectName(QStringLiteral("pushButton_UnLockTemplate"));
        pushButton_UnLockTemplate->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_UnLockTemplate, 5, 1, 1, 1);

        pushButton_QueryContractInfo = new QPushButton(DataEncToolDlg);
        pushButton_QueryContractInfo->setObjectName(QStringLiteral("pushButton_QueryContractInfo"));
        pushButton_QueryContractInfo->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_QueryContractInfo, 5, 2, 1, 1);

        pushButton_DeleteContract = new QPushButton(DataEncToolDlg);
        pushButton_DeleteContract->setObjectName(QStringLiteral("pushButton_DeleteContract"));
        pushButton_DeleteContract->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_DeleteContract, 5, 3, 1, 1);

        pushButton_AbolishContract = new QPushButton(DataEncToolDlg);
        pushButton_AbolishContract->setObjectName(QStringLiteral("pushButton_AbolishContract"));
        pushButton_AbolishContract->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_AbolishContract, 5, 4, 1, 1);

        pushButton_ContractRender = new QPushButton(DataEncToolDlg);
        pushButton_ContractRender->setObjectName(QStringLiteral("pushButton_ContractRender"));
        pushButton_ContractRender->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_ContractRender, 5, 5, 1, 1);

        pushButton_ScanContractAdd = new QPushButton(DataEncToolDlg);
        pushButton_ScanContractAdd->setObjectName(QStringLiteral("pushButton_ScanContractAdd"));
        pushButton_ScanContractAdd->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_ScanContractAdd, 6, 0, 1, 1);

        pushButton_ScanContractAddEvent = new QPushButton(DataEncToolDlg);
        pushButton_ScanContractAddEvent->setObjectName(QStringLiteral("pushButton_ScanContractAddEvent"));
        pushButton_ScanContractAddEvent->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_ScanContractAddEvent, 6, 1, 1, 1);

        pushButton_SignScanSign = new QPushButton(DataEncToolDlg);
        pushButton_SignScanSign->setObjectName(QStringLiteral("pushButton_SignScanSign"));
        pushButton_SignScanSign->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignScanSign, 6, 2, 1, 1);

        pushButton_SignScanSignEvent = new QPushButton(DataEncToolDlg);
        pushButton_SignScanSignEvent->setObjectName(QStringLiteral("pushButton_SignScanSignEvent"));
        pushButton_SignScanSignEvent->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_SignScanSignEvent, 6, 3, 1, 1);

        pushButton_QueryScanContract = new QPushButton(DataEncToolDlg);
        pushButton_QueryScanContract->setObjectName(QStringLiteral("pushButton_QueryScanContract"));
        pushButton_QueryScanContract->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_QueryScanContract, 6, 4, 1, 1);

        pushButton_BindContractPhone = new QPushButton(DataEncToolDlg);
        pushButton_BindContractPhone->setObjectName(QStringLiteral("pushButton_BindContractPhone"));
        pushButton_BindContractPhone->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_BindContractPhone, 6, 5, 1, 1);

        pushButton_QueryBindContract = new QPushButton(DataEncToolDlg);
        pushButton_QueryBindContract->setObjectName(QStringLiteral("pushButton_QueryBindContract"));
        pushButton_QueryBindContract->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_QueryBindContract, 7, 0, 1, 1);

        pushButton_DownFile = new QPushButton(DataEncToolDlg);
        pushButton_DownFile->setObjectName(QStringLiteral("pushButton_DownFile"));
        pushButton_DownFile->setMinimumSize(QSize(90, 25));

        gridLayout->addWidget(pushButton_DownFile, 7, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_SingleTemplateSignEvent = new QPushButton(DataEncToolDlg);
        pushButton_SingleTemplateSignEvent->setObjectName(QStringLiteral("pushButton_SingleTemplateSignEvent"));
        pushButton_SingleTemplateSignEvent->setMinimumSize(QSize(110, 25));

        horizontalLayout_3->addWidget(pushButton_SingleTemplateSignEvent);

        pushButton_BatchTemplateSignEvent = new QPushButton(DataEncToolDlg);
        pushButton_BatchTemplateSignEvent->setObjectName(QStringLiteral("pushButton_BatchTemplateSignEvent"));
        pushButton_BatchTemplateSignEvent->setMinimumSize(QSize(110, 25));

        horizontalLayout_3->addWidget(pushButton_BatchTemplateSignEvent);

        pushButton_SignCommonEvent = new QPushButton(DataEncToolDlg);
        pushButton_SignCommonEvent->setObjectName(QStringLiteral("pushButton_SignCommonEvent"));
        pushButton_SignCommonEvent->setMinimumSize(QSize(110, 25));

        horizontalLayout_3->addWidget(pushButton_SignCommonEvent);

        pushButton_SignKeywordSignEvent = new QPushButton(DataEncToolDlg);
        pushButton_SignKeywordSignEvent->setObjectName(QStringLiteral("pushButton_SignKeywordSignEvent"));
        pushButton_SignKeywordSignEvent->setMinimumSize(QSize(110, 25));

        horizontalLayout_3->addWidget(pushButton_SignKeywordSignEvent);

        pushButton_GetReportFile = new QPushButton(DataEncToolDlg);
        pushButton_GetReportFile->setObjectName(QStringLiteral("pushButton_GetReportFile"));
        pushButton_GetReportFile->setMinimumSize(QSize(110, 25));

        horizontalLayout_3->addWidget(pushButton_GetReportFile);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(DataEncToolDlg);

        QMetaObject::connectSlotsByName(DataEncToolDlg);
    } // setupUi

    void retranslateUi(QDialog *DataEncToolDlg)
    {
        DataEncToolDlg->setWindowTitle(QApplication::translate("DataEncToolDlg", "\346\265\213\350\257\225\345\267\245\345\205\267", 0));
        label_2->setText(QApplication::translate("DataEncToolDlg", "\345\217\221\351\200\201\346\225\260\346\215\256 \357\274\232", 0));
        label_3->setText(QApplication::translate("DataEncToolDlg", "\350\276\223\345\207\272\347\273\223\346\236\234 \357\274\232", 0));
        label->setText(QString());
        pushButton_token->setText(QApplication::translate("DataEncToolDlg", "\350\216\267\345\217\226Token", 0));
        pushButton_RegisterPersonal->setText(QApplication::translate("DataEncToolDlg", "\346\263\250\345\206\214\344\270\252\344\272\272\344\277\241\346\201\257", 0));
        pushButton_GetPersonalUserInfo->setText(QApplication::translate("DataEncToolDlg", "\350\216\267\345\217\226\344\270\252\344\272\272\344\277\241\346\201\257", 0));
        pushButton_PersonalLock->setText(QApplication::translate("DataEncToolDlg", "\347\224\250\346\210\267\351\224\201\345\256\232", 0));
        pushButton_PersonalUnlock->setText(QApplication::translate("DataEncToolDlg", "\347\224\250\346\210\267\350\247\243\351\224\201", 0));
        pushButton_clear->setText(QApplication::translate("DataEncToolDlg", "\346\270\205\347\251\272", 0));
        pushButton_RegisterCompany->setText(QApplication::translate("DataEncToolDlg", "\346\263\250\345\206\214\344\274\201\344\270\232", 0));
        pushButton_GetCompanyUserInfo->setText(QApplication::translate("DataEncToolDlg", "\350\216\267\345\217\226\344\274\201\344\270\232\344\277\241\346\201\257", 0));
        pushButton_CompanyLock->setText(QApplication::translate("DataEncToolDlg", "\344\274\201\344\270\232\351\224\201\345\256\232", 0));
        pushButton_CompanyUnlock->setText(QApplication::translate("DataEncToolDlg", "\344\274\201\344\270\232\350\247\243\351\224\201", 0));
        pushButton_UploadFileSeal->setText(QApplication::translate("DataEncToolDlg", "\344\270\212\344\274\240\345\215\260\347\253\240\344\277\241\346\201\257", 0));
        pushButton_UploadFileContract->setText(QApplication::translate("DataEncToolDlg", "\344\270\212\344\274\240\345\220\210\345\220\214\344\277\241\346\201\257", 0));
        pushButton_UploadFileTemplate->setText(QApplication::translate("DataEncToolDlg", "\344\270\212\344\274\240\346\250\241\347\211\210\344\277\241\346\201\257", 0));
        pushButton_SingleTemplateSign->setText(QApplication::translate("DataEncToolDlg", "\345\215\225\346\254\241\346\250\241\347\211\210\347\255\276\347\253\240", 0));
        pushButton_BatchTemplateSign->setText(QApplication::translate("DataEncToolDlg", "\346\211\271\351\207\217\346\250\241\347\211\210\347\255\276\347\253\240", 0));
        pushButton_SignCommon->setText(QApplication::translate("DataEncToolDlg", "\345\235\220\346\240\207\347\255\276\347\253\240", 0));
        pushButton_SignKeywordSign->setText(QApplication::translate("DataEncToolDlg", "\345\205\263\351\224\256\345\255\227\347\255\276\347\253\240", 0));
        pushButton_SignMeetingSingle->setText(QApplication::translate("DataEncToolDlg", "\344\274\232\347\255\276", 0));
        pushButton_SignMeetingSingleEvent->setText(QApplication::translate("DataEncToolDlg", "\344\274\232\347\255\276\344\272\213\344\273\266", 0));
        pushButton_SignOneStepAll->setText(QApplication::translate("DataEncToolDlg", "\344\270\200\346\255\245\347\255\276\347\253\240", 0));
        pushButton_SignCheckCommon->setText(QApplication::translate("DataEncToolDlg", "\346\226\207\346\241\243\351\252\214\350\257\201(ID)", 0));
        pushButton_SignCheckFile->setText(QApplication::translate("DataEncToolDlg", "\346\226\207\346\241\243\351\252\214\350\257\201", 0));
        pushButton_GetCertinfo->setText(QApplication::translate("DataEncToolDlg", "\350\216\267\345\217\226\350\257\201\344\271\246", 0));
        pushButton_QuerySealInfo->setText(QApplication::translate("DataEncToolDlg", "\346\237\245\350\257\242\345\215\260\347\253\240", 0));
        pushButton_GetSealInfosFromUser->setText(QApplication::translate("DataEncToolDlg", "\347\224\250\346\210\267\346\211\200\346\234\211\345\215\260\347\253\240", 0));
        pushButton_RemoveSealInfo->setText(QApplication::translate("DataEncToolDlg", "\345\210\240\351\231\244\345\215\260\347\253\240", 0));
        pushButton_LockSealStatus->setText(QApplication::translate("DataEncToolDlg", "\345\215\260\347\253\240\351\224\201\345\256\232", 0));
        pushButton_UnLockSealStatus->setText(QApplication::translate("DataEncToolDlg", "\345\215\260\347\253\240\350\247\243\351\224\201", 0));
        pushButton_QueryTemplate->setText(QApplication::translate("DataEncToolDlg", "\346\237\245\350\257\242\346\250\241\347\211\210", 0));
        pushButton_DeleteTemplate->setText(QApplication::translate("DataEncToolDlg", "\345\210\240\351\231\244\346\250\241\347\211\210", 0));
        pushButton_LockTemplate->setText(QApplication::translate("DataEncToolDlg", "\346\250\241\347\211\210\351\224\201\345\256\232", 0));
        pushButton_UnLockTemplate->setText(QApplication::translate("DataEncToolDlg", "\346\250\241\347\211\210\350\247\243\351\224\201", 0));
        pushButton_QueryContractInfo->setText(QApplication::translate("DataEncToolDlg", "\346\237\245\350\257\242\345\220\210\345\220\214", 0));
        pushButton_DeleteContract->setText(QApplication::translate("DataEncToolDlg", "\345\210\240\351\231\244\345\220\210\345\220\214", 0));
        pushButton_AbolishContract->setText(QApplication::translate("DataEncToolDlg", "\344\275\234\345\272\237\345\220\210\345\220\214", 0));
        pushButton_ContractRender->setText(QApplication::translate("DataEncToolDlg", "\345\274\202\346\255\245\346\270\262\346\237\223\345\220\210\345\220\214", 0));
        pushButton_ScanContractAdd->setText(QApplication::translate("DataEncToolDlg", "\346\267\273\345\212\240\344\272\214\347\273\264\347\240\201", 0));
        pushButton_ScanContractAddEvent->setText(QApplication::translate("DataEncToolDlg", "\346\267\273\345\212\240\344\272\214\344\275\215\347\240\201\344\272\213\344\273\266", 0));
        pushButton_SignScanSign->setText(QApplication::translate("DataEncToolDlg", "\346\211\253\346\217\217\347\255\276\347\253\240", 0));
        pushButton_SignScanSignEvent->setText(QApplication::translate("DataEncToolDlg", "\346\211\253\346\217\217\347\255\276\347\253\240\344\272\213\344\273\266", 0));
        pushButton_QueryScanContract->setText(QApplication::translate("DataEncToolDlg", "\346\237\245\350\257\242\344\272\214\347\273\264\347\240\201", 0));
        pushButton_BindContractPhone->setText(QApplication::translate("DataEncToolDlg", "\345\220\210\345\220\214\347\273\221\345\256\232\346\211\213\346\234\272\345\217\267", 0));
        pushButton_QueryBindContract->setText(QApplication::translate("DataEncToolDlg", "\346\237\245\350\257\242\347\255\276\347\275\262\345\220\210\345\220\214", 0));
        pushButton_DownFile->setText(QApplication::translate("DataEncToolDlg", "\344\270\213\350\275\275\346\226\207\344\273\266", 0));
        pushButton_SingleTemplateSignEvent->setText(QApplication::translate("DataEncToolDlg", "\345\215\225\346\254\241\346\250\241\347\211\210\347\255\276\347\253\240\344\272\213\344\273\266", 0));
        pushButton_BatchTemplateSignEvent->setText(QApplication::translate("DataEncToolDlg", "\346\211\271\351\207\217\346\250\241\347\211\210\347\255\276\347\253\240\344\272\213\344\273\266", 0));
        pushButton_SignCommonEvent->setText(QApplication::translate("DataEncToolDlg", "\345\235\220\346\240\207\347\255\276\347\253\240\344\272\213\344\273\266", 0));
        pushButton_SignKeywordSignEvent->setText(QApplication::translate("DataEncToolDlg", "\345\205\263\351\224\256\345\255\227\347\255\276\347\253\240\344\272\213\344\273\266", 0));
        pushButton_GetReportFile->setText(QApplication::translate("DataEncToolDlg", "\344\270\213\350\275\275\347\224\265\345\255\220\345\220\210\345\220\214\345\207\255\350\257\201", 0));
    } // retranslateUi

};

namespace Ui {
    class DataEncToolDlg: public Ui_DataEncToolDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAENCTOOLDLG_H
