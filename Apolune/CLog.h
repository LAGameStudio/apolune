#pragma once

#ifndef __CLog_h__
#define __CLog_h__

/*
Copyright (c) 2000, Sean O'Neil (s_p_oneil@hotmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the project nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Windows.h>
#include <stdio.h>

#include "CDateTime.h"
#include "Strings.h"
#include "macros.h"

#if defined(ENABLE_LOGGING)

enum LoggingLevel
{
	None = 0,
	Critical = 1,	// Use for logging critical errors
	Error = 2,	 	// Use for logging non-critical errors
	Warning = 3,	// Use for logging warnings
	Info = 4,		// Use for logging key informational events
	Notice = 5,		// Slightly less than Info, but not debug spew
	Debug = 6,		// Use for logging debug events
	Spam = 7		// Use for logging events that may spam the log files
};

class CLog
{
protected:
	static CLog *m_pSingleton;
	const char **m_pszSeverity;
	Strings m_ofLog;
	char m_szLogName[256];
	int m_nLoggingLevel;

	void LogSwitch()
	{
		// Get today's date for the log switch
		static char szPath[_MAX_PATH];
		sprintf_s(szPath, "%s.log", m_szLogName);
		return;
	}

public:
	CLog()
	{
		m_pSingleton = this;
	}
	~CLog()
	{
		m_pSingleton = NULL;
	}

	static bool IsValid()	{ return m_pSingleton != NULL; }
	static CLog *GetPtr()
	{
		return m_pSingleton;
	}
	static CLog &GetRef()
	{
		return *m_pSingleton;
	}

	// nLoggingLevel is an enummed loglevel or -1 if you want nothing
	void Init(int nLoggingLevel, const char *pszLogName)
	{
		static const char *pszSeverity[] = {"", "CRITICAL", "ERROR", "WARNING", "INFORMATION", "NOTICE", "DEBUG", "SPAM"};
		m_pszSeverity = pszSeverity;
		strcpy(m_szLogName, pszLogName);
		m_nLoggingLevel = nLoggingLevel;
		LogSwitch();
	}

	bool IsLogged(LoggingLevel nSeverity)			{ return (nSeverity <= m_nLoggingLevel); }

	void Log(LoggingLevel nSeverity, const char *pszMessage)
	{
		char szBuffer[256], szDateTime[40];
		CDateTime dtCurrent = CDateTime::GetCurrent();
#ifdef MODULE_NAME
		sprintf(szBuffer, "%s  Severity: %-12.12s  Module: %-30.30s", dtCurrent.GetString(szDateTime), m_pszSeverity[nSeverity], MODULE_NAME);
#else
		sprintf(szBuffer, "%s  Severity: %-12.12s", dtCurrent.GetString(szDateTime), m_pszSeverity[nSeverity]);
#endif

		m_ofLog.printf( "%s\n%s\n", szBuffer, pszMessage );
//		m_ofLog.Save(m_szLogName);
//  m_ofLog.flush();
	}

	void FormatLog_va(LoggingLevel nSeverity, const char *pszFormat, va_list args)
	{
		char szBuffer[16384];
		vsprintf(szBuffer, pszFormat, args);
		Log(nSeverity, szBuffer);
	}

	void FormatLog(LoggingLevel nSeverity, const char *pszFormat, ...)
	{
		va_list va;
		va_start(va, pszFormat);
		FormatLog_va(nSeverity, pszFormat, va);
		va_end(va);
	}
};

// These functions can be called from anywhere at any time. If the singleton CLog object
// does not exist, logging is disabled. This provides a simple way to turn logging on and off.
inline bool IsLogged(LoggingLevel nSeverity)
{
	if(CLog::IsValid())
		return CLog::GetRef().IsLogged(nSeverity);
	return false;
}

inline void Log(LoggingLevel nSeverity, const char *pszMessage)
{
	if(IsLogged(nSeverity))
		CLog::GetRef().Log(nSeverity, pszMessage);
}

inline void FormatLog(LoggingLevel nSeverity, const char *pszFormat, ...)
{
	if(IsLogged(nSeverity))
	{
		va_list va;
		va_start(va, pszFormat);
		char szBuffer[16384];
		vsprintf(szBuffer, pszFormat, va);
		CLog::GetRef().Log(nSeverity, szBuffer);
		va_end(va);
	}
}

// These macros should help avoid expensive calculations or function calls that may be made in the parameter list of a Log() or FormatLog() call
#define DECLARE_LOG_FUNC(func, level) \
inline void func(const char *pszFormat, ...)\
{\
	va_list va;\
	va_start(va, pszFormat);\
	CLog::GetRef().FormatLog_va(level, pszFormat, va);\
	va_end(va);\
}

DECLARE_LOG_FUNC(Critical_LogFunc, Critical);
DECLARE_LOG_FUNC(Error_LogFunc, Error);
DECLARE_LOG_FUNC(Warning_LogFunc, Warning);
DECLARE_LOG_FUNC(Info_LogFunc, Info);
DECLARE_LOG_FUNC(Notice_LogFunc, Notice);
DECLARE_LOG_FUNC(Debug_LogFunc, Debug);
DECLARE_LOG_FUNC(Spam_LogFunc, Spam);

#define LogCritical	if(IsLogged(Critical)) Critical_LogFunc
#define OUTPUT	if(IsLogged(Error)) Error_LogFunc
#define OUTPUT	if(IsLogged(Warning)) Warning_LogFunc
#define OUTPUT		if(IsLogged(Info)) Info_LogFunc
#define LogNotice	if(IsLogged(Notice)) Notice_LogFunc
#define OUTPUT	if(IsLogged(Debug)) Debug_LogFunc
#define OUTPUT		if(IsLogged(Spam)) Spam_LogFunc

#ifdef _DEBUG
#define LogAssert(expr) \
	do {\
		if(!(expr)) {\
			LogCritical("Assertion failure at %s:%d", __FILE__, __LINE__);\
			if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))\
				_CrtDbgBreak();\
		}\
	} while (0)
#else
#define LogAssert(expr) \
	do {\
		if(!(expr))\
			LogCritical("Assertion failure at %s:%d", __FILE__, __LINE__);\
	} while (0)
#endif

#ifdef WIN32
inline char *FormatLastError(DWORD nLastError, int nLength=8192)
{
 static char pszBuffer[8192];
	if(!::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pszBuffer, nLength, NULL))
		sprintf_s(pszBuffer, "GetLastError() returned 0x%X", nLastError);
	return pszBuffer;
}
#endif

#else

#define LogAssert(log)
#endif

#endif