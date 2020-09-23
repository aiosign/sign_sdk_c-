#pragma once

#include <Windows.h>
#include <string>
#include "HLogFile.h"
using namespace std;

#ifndef SEALLOG
#define SEALLOG _declspec(dllexport)
#endif

///*初始化*/
//SEALLOG void log_init(const char* szFilename, bool bdirectlog = true,//日志是否直接写日志(即,不做批量写日志)，如果为true，iminflushnum将被忽略
//	unsigned iminflushnum = 10,//最小的写入日志文件的条目数,(即多少条后批量谢日日志文件)
//	int iOutputLevel = LOG_INFO,//日志级别
//	int iDisplayWhere = DISPLAY_BOTH);//日志写到哪里
//
///*写入*/
//SEALLOG void Writelog(const char* szValue);

/*初始化*/
//SEALLOG void log_init(string fileDir, string filename, bool bdirectlog, unsigned iminflushnum, int iOutputLevel, int iDisplayWhere)
SEALLOG void log_init(const char* filedir, const char* filename, bool bdirectlog = true,//日志是否直接写日志(即,不做批量写日志)，如果为true，iminflushnum将被忽略
	unsigned iminflushnum = 10,//最小的写入日志文件的条目数,(即多少条后批量谢日日志文件)
	int iOutputLevel = LOG_INFO,//日志级别
	int iDisplayWhere = DISPLAY_BOTH);//日志写到哪里

/*写入*/
SEALLOG void Writelog(const char* strvalue);