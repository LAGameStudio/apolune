#pragma once
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
#include <string>
#include <map>

#include "Singleton.h"


//#define ENABLE_PROFILING
#ifdef ENABLE_PROFILING

class CProfileTimer
{
protected:
	CProfileTimer *m_pParent;
	std::map<std::string, CProfileTimer*> m_mapChildren;

	std::string m_strName;
	unsigned long m_nStartTime;
	unsigned long m_nTotalTime;
	unsigned long m_nCallCount;

public:
	CProfileTimer(std::string strName, CProfileTimer *pParent)
	{
		m_strName = strName;
		m_pParent = pParent;
		m_nStartTime = (unsigned long)-1;
		m_nTotalTime = 0;
		m_nCallCount = 0;
	}
	~CProfileTimer()
	{
		// Clean up the child timers to avoid memory leaks
		std::map<std::string, CProfileTimer*>::iterator i;
		for(i = m_mapChildren.begin(); i != m_mapChildren.end(); i++)
		{
			CProfileTimer *pChild = i->second;
			delete pChild;
		}
	}

	std::string GetName()		{ return m_strName; }
	std::string GetProfilerStats(int nLevel=0)
	{
		static char szBuffer[1024];
		sprintf(szBuffer, "%8u : %8u : %5.1f : %*.*s%s\n", (unsigned int)m_nCallCount, (unsigned int)m_nTotalTime, m_pParent ? (m_nTotalTime*100 / (float)m_pParent->m_nTotalTime) : 100.0f, nLevel, nLevel, " ", m_strName.c_str());
		std::string strBuild = szBuffer;
		if(nLevel == 0)
			strBuild = "   Count : Time(ms) :     % : Profile Name\n-------------------------------------------------------------\n" + strBuild;
		for(std::map<std::string, CProfileTimer*>::iterator i = m_mapChildren.begin(); i != m_mapChildren.end(); i++)
			strBuild += i->second->GetProfilerStats(nLevel+1);
		return strBuild;
	}

	void Reset()
	{
		m_nTotalTime = 0;
		m_nCallCount = 0;
	}

	CProfileTimer *GetTimer(std::string strName)
	{
		CProfileTimer *&pTimer = m_mapChildren[strName];
		if(pTimer == NULL)
			pTimer = new CProfileTimer(strName, this);
		return pTimer;
	}

	void StartTimer()
	{
		LogAssert(m_nStartTime == (unsigned long)-1);
		m_nStartTime = SDL_GetTicks();
	}

	void StopTimer()
	{
		LogAssert(m_nStartTime != (unsigned long)-1);
		unsigned long nEndTime = SDL_GetTicks();
		m_nTotalTime += nEndTime - m_nStartTime;
		m_nStartTime = (unsigned long)-1;
		m_nCallCount++;
	}
};

class CProfileSample;
class CProfiler : public TSingleton<CProfiler>
{
protected:
	LoggingLevel m_nLoggingLevel;
	int m_nProfilingLevel;
	CProfileSample *m_pCurrentSample;
	CProfileTimer *m_pRootTimer;

public:
	CProfiler(std::string strName, int nProfilingLevel, LoggingLevel nLoggingLevel=Debug)
	{
		m_nLoggingLevel = nLoggingLevel;
		m_nProfilingLevel = nProfilingLevel;
		m_pCurrentSample = NULL;
		m_pRootTimer = new CProfileTimer(strName, NULL);
		m_pRootTimer->StartTimer();
	}
	~CProfiler()
	{
		m_pRootTimer->StopTimer();
		Log(m_nLoggingLevel, m_pRootTimer->GetProfilerStats().c_str());
		delete m_pRootTimer;
	}

	bool IsProfiled(int nLevel)		{ return (m_nProfilingLevel != None && nLevel <= m_nProfilingLevel); }
	friend class CProfileSample;
};

class CProfileSample
{
protected:
	CProfileSample *m_pParentSample;
	CProfileTimer *m_pTimer;

public:
	CProfileSample(std::string strName, int nProfilingLevel)
	{
		m_pTimer = NULL;
		CProfiler *pProfiler = CProfiler::GetPtr();
		if(pProfiler->IsProfiled(nProfilingLevel))
		{
			LogAssert(pProfiler->m_pCurrentSample != this);
			m_pParentSample = pProfiler->m_pCurrentSample;
			pProfiler->m_pCurrentSample = this;
			m_pTimer = (m_pParentSample ? m_pParentSample->m_pTimer : pProfiler->m_pRootTimer)->GetTimer(strName);
			m_pTimer->StartTimer();
		}
	}
	~CProfileSample()
	{
		if(m_pTimer)
		{
			CProfiler *pProfiler = CProfiler::GetPtr();
			LogAssert(pProfiler->m_pCurrentSample == this);
			m_pTimer->StopTimer();
			pProfiler->m_pCurrentSample = m_pParentSample;
		}
	}
};

#define PROFILE(name, level) CProfileSample _sample(name, level);
#else
#define PROFILE(name, level)
#endif

