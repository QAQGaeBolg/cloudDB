#include <stdarg.h>

#ifndef WIN32
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "lo_logs.hpp"
#include "lo_utilities.hpp"
#include <time.h>
#include <tchar.h>

static Mutex m_gLogMutex(0);

FILE *LoLogs::m_fileSvrLog = NULL;
int LoLogs::m_nFileStatus = FILESTATUS_CLOSE;

//////////////////////////////////////////////////////////////////////////
// CLogs class implement region

// CLogs class construct&deconstruct functions

LoLogs::LoLogs()
{
}

LoLogs::~LoLogs()
{
}

// CLogs class member functions
const char *LoLogs::GetCurDateTimeWithString(char *pStringBuf)
{
	time_t	tCurDateTime;
	tm		*ptagCurDataTime;
	char	szTimeStringBuf[MAXSIZE_TIMESTRING] = { 0 };

	char *pWriteStringBuf = pStringBuf;
	if (NULL == pWriteStringBuf)
	{
		pWriteStringBuf = szTimeStringBuf;
	}

	time(&tCurDateTime);
	ptagCurDataTime = localtime(&tCurDateTime);

	sprintf(pWriteStringBuf, "%d-%02d-%02d %02d:%02d:%02d",
		ptagCurDataTime->tm_year + 1900,
		ptagCurDataTime->tm_mon + 1,
		ptagCurDataTime->tm_mday,
		ptagCurDataTime->tm_hour,
		ptagCurDataTime->tm_min,
		ptagCurDataTime->tm_sec);

	return pWriteStringBuf;
}

void LoLogs::WriteLogInner(const char *pcLogMsg,
	const char *pcLogLead,
	unsigned char bKeepOpen/* = 0*/,
	unsigned char bNeedSeparator/* = 0*/)
{
	if (pcLogMsg == NULL)
	{
		return;
	}

	m_gLogMutex.lock();

	int nStatus = OpenLog(pcLogLead);
	if (nStatus != -1)
	{
		char szCurTime[MAXSIZE_CURTIME] = { 0 };
		GetCurDateTimeWithString(szCurTime);
		strcat(szCurTime, SERVERLOG_SKIP);
		WriteData(szCurTime);
		WriteData(pcLogMsg);
		if (bNeedSeparator)
		{
			WriteData(SERVERLOG_ENTER);
		}

		if (!bKeepOpen)
		{
			CloseLog();
		}
	}

	m_gLogMutex.unlock();
}

void LoLogs::WriteLog(const char *pcLogMsg, ...)
{
	char szMsgBuf[MAXSIZE_MSGBUF] = { 0 };
	va_list va;
	va_start(va, pcLogMsg);
	vsprintf(szMsgBuf, pcLogMsg, va);
#ifdef WIN32
	printf(szMsgBuf);
#endif
	va_end(va);
	WriteLogInner(szMsgBuf, LOGLEADNAME_BOOT);
}

void LoLogs::WriteDebugLog(const char *pcLogMsg, ...)
{

	char szMsgBuf[MAXSIZE_MSGBUF] = { 0 };
	va_list va;
	va_start(va, pcLogMsg);
	vsprintf(szMsgBuf, pcLogMsg, va);
#ifdef WIN32
	//printf( szMsgBuf );
#endif
	va_end(va);
	WriteLogInner(szMsgBuf, LOGLEADNAME_DEBUG);

}

void LoLogs::WriteFatalLog(const char *pcLogMsg, ...)
{
	char szMsgBuf[MAXSIZE_MSGBUF] = { 0 };
	va_list va;
	va_start(va, pcLogMsg);
	vsprintf(szMsgBuf, pcLogMsg, va);
#ifdef WIN32
	//printf( szMsgBuf );
#endif
	va_end(va);
	WriteLogInner(szMsgBuf, LOGLEADNAME_FATAL);
}

void LoLogs::WriteWarningLog(const char *pcLogMsg, ...)
{
	char szMsgBuf[MAXSIZE_MSGBUF] = { 0 };
	va_list va;
	va_start(va, pcLogMsg);
	vsprintf(szMsgBuf, pcLogMsg, va);
#ifdef WIN32
	//printf( szMsgBuf );
#endif
	va_end(va);
	WriteLogInner(szMsgBuf, LOGLEADNAME_WARNING);
}

int LoLogs::OpenLog(const char *pcLogLead)
{
	int nRet = 0;

	if (m_nFileStatus == FILESTATUS_CLOSE)
	{
		char szOpenPathName[MAXSIZE_PATHNAME] = { 0 };

		time_t	ltime;
		tm		tmStruct;
		time(&ltime);
		tmStruct = *localtime(&ltime);

		strcat(szOpenPathName, SUBDIRNAME_LOG);
		sprintf(szOpenPathName + strlen(szOpenPathName), "%s%04d-%02d-%02d.log", pcLogLead, tmStruct.tm_year + 1900, tmStruct.tm_mon + 1, tmStruct.tm_mday);
#ifdef WIN32
		CreateDirectory(_T(SUBDIRNAME_LOG), NULL);
#else /* LINUX */
		mkdir(SUBDIRNAME_LOG, 0700);
#endif

		m_fileSvrLog = fopen(szOpenPathName, "a+");
		if (m_fileSvrLog)
		{
			m_nFileStatus = FILESTATUS_OPEN;
		}
		else
		{
			nRet = -1;
		}
	}

	return nRet;
}

void LoLogs::CloseLog()
{
	if (m_nFileStatus == FILESTATUS_OPEN)
	{
		fclose(m_fileSvrLog);
		m_fileSvrLog = NULL;
		m_nFileStatus = FILESTATUS_CLOSE;
	}
}

void LoLogs::WriteData(const char *pcLogMsg)
{
	if (pcLogMsg == NULL)
	{
		return;
	}

	if (m_nFileStatus == FILESTATUS_OPEN)
	{
		fwrite(pcLogMsg, 1, strlen(pcLogMsg), m_fileSvrLog);
	}
}
