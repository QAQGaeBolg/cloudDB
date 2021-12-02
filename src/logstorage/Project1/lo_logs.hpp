#ifndef _DB_LOGS_HPP_
#define _DB_LOGS_HPP_

#include <stdio.h>

// Macro define region
#define FILESTATUS_CLOSE			0
#define FILESTATUS_OPEN				1

#define MAXSIZE_TIMESTRING			64
#define MAXSIZE_CURTIME				64
#define MAXSIZE_PATHNAME			512
#define MAXSIZE_MSGBUF				(1024*8)
#ifdef _WIN32
#define SUBDIRNAME_LOG				"logs\\"
#else
#define SUBDIRNAME_LOG				"./logs/"
#endif
#define SERVERLOG_SKIP				" -> "
#define SERVERLOG_ENTER				"\r\n"

#define LOGLEADNAME_BOOT			"boot"
#define LOGLEADNAME_DEBUG			"debug"
#define LOGLEADNAME_FATAL			"fatal"
#define LOGLEADNAME_WARNING			"warning"

class LoLogs
{
public:
	LoLogs();
	virtual ~LoLogs();

	static void WriteLog(const char *pcLogMsg, ...);
	static void WriteDebugLog(const char *pcLogMsg, ...);
	static void WriteFatalLog(const char *pcLogMsg, ...);
	static void WriteWarningLog(const char *pcLogMsg, ...);
private:
	static FILE *m_fileSvrLog;
	static int m_nFileStatus;

	static void WriteLogInner(const char *pcLogMsg, const char *pcLogLead, unsigned char bKeepOpen = 0, unsigned char bNeedSeparator = 0);
	static void CloseLog();

	static int OpenLog(const char *pcLogLead);
	static void WriteData(const char *pcLogMsg);


	static const char *GetCurDateTimeWithString(char *pStringBuf);
};

#endif // _DB_LOGS_HPP_
