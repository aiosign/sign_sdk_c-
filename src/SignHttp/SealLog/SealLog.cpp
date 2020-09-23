#include "stdafx.h"
#include "SealLog.h"


//SEALLOG void log_init(const char* szFilename, bool bdirectlog, unsigned iminflushnum, int iOutputLevel, int iDisplayWhere)
//{
//	log_init_real(szFilename, true, 10, LOG_INFO, DISPLAY_FILE);
//}
//
//SEALLOG void Writelog(const char* szvalue)
//{
//	HLog(HLogError << szvalue);
//}

SEALLOG void log_init(const char* filedir, const char* filename, bool bdirectlog, unsigned iminflushnum, int iOutputLevel, int iDisplayWhere)
{
	log_init_real(filedir, filename, true, 10, LOG_INFO, DISPLAY_FILE);
}

SEALLOG void Writelog(const char* strvalue)
{
	HLog(HLogError << strvalue);
}