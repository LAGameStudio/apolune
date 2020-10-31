#pragma once

#ifndef __CTimerTask_h__
#define __CTimerTask_h__

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

#ifdef _WIN32

// In Windows SDL_GetTicks causes the camera to "jump".
// Use QueryPerformanceCounter instead.
class CTimerTask : public CKernelTask, public TSingleton<CTimerTask>
{
protected:
	double m_dFactor;
	LARGE_INTEGER m_nGameStart, m_nFrameStart;

	// If set, locks the frame rate (for video capture)
	unsigned long m_nLockedRate;

	// Some clients will work with game time
	unsigned long m_nGameTicks;
	float m_fGameSeconds;

	// Some clients will work with frame time
	unsigned long m_nFrameTicks;
	float m_fFrameSeconds;

public:
	void LockFrameRate(unsigned long nTicks)	{ m_nLockedRate = nTicks; }
	void UnlockFrameRate()						{ m_nLockedRate = 0; }
	unsigned long GetGameTicks()	{ return m_nGameTicks; }
	float GetGameSeconds()			{ return m_fGameSeconds; }
	unsigned long GetFrameTicks()	{ return m_nFrameTicks; }
	float GetFrameSeconds()			{ return m_fFrameSeconds; }

	DEFAULT_TASK_CREATOR(CTimerTask);

	virtual bool Start()
	{
		m_nLockedRate = 0;
		LARGE_INTEGER nFrequency;
		::QueryPerformanceFrequency(&nFrequency);
		m_dFactor = 1.0 / nFrequency.QuadPart;

		::QueryPerformanceCounter(&m_nGameStart);
		m_nFrameStart.QuadPart = m_nGameStart.QuadPart;
		m_nGameTicks = m_nFrameTicks = 0;
		m_fGameSeconds = m_fFrameSeconds = 0.0f;
		return true;
	}

	virtual void Update()
	{
		if(m_nLockedRate)
		{
			m_nFrameTicks = m_nLockedRate;
			m_fFrameSeconds = m_nFrameTicks * 0.001f;
			m_nGameTicks += m_nLockedRate;
			m_fGameSeconds = m_nGameTicks * 0.001f;
			return;
		}

		LARGE_INTEGER nPreviousFrame;
		nPreviousFrame.QuadPart = m_nFrameStart.QuadPart;
		::QueryPerformanceCounter(&m_nFrameStart);
		m_fFrameSeconds = (float)((m_nFrameStart.QuadPart - nPreviousFrame.QuadPart) * m_dFactor);
		m_nFrameTicks = (unsigned long)(m_fFrameSeconds * 1000.0f + 0.5f);

		m_fGameSeconds = (float)((m_nFrameStart.QuadPart - m_nGameStart.QuadPart) * m_dFactor);
		m_nGameTicks = (unsigned long)(m_fGameSeconds * 1000.0f + 0.5f);
	}

	virtual void Stop()
	{
	}
};

#else

class CTimerTask : public CKernelTask, public TSingleton<CTimerTask>
{
protected:
	// Some clients will work with game time
	unsigned long m_nGameStart;
	unsigned long m_nGameTicks;
	float m_fGameSeconds;

	// Some clients will work with frame time
	unsigned long m_nFrameStart;
	unsigned long m_nFrameTicks;
	float m_fFrameSeconds;

public:
	unsigned long GetGameTicks()	{ return m_nGameTicks; }
	float GetGameSeconds()			{ return m_fGameSeconds; }
	unsigned long GetFrameTicks()	{ return m_nFrameTicks; }
	float GetFrameSeconds()			{ return m_fFrameSeconds; }

	DEFAULT_TASK_CREATOR(CTimerTask);

	virtual bool Start()
	{
		m_nGameStart = m_nFrameStart = SDL_GetTicks();
		m_nGameTicks = m_nFrameTicks = 0;
		m_fGameSeconds = m_fFrameSeconds = 0.0f;
		return true;
	}

	virtual void Update()
	{
		PROFILE("CTimerTask::Update()", 1);
		unsigned long nPreviousFrame = m_nFrameStart;
		m_nFrameStart = SDL_GetTicks();

		m_nFrameTicks = m_nFrameStart - nPreviousFrame;
		m_fFrameSeconds = m_nFrameTicks * 0.001f;

		m_nGameTicks = m_nFrameStart - m_nGameStart;
		m_fGameSeconds = m_nGameTicks * 0.001f;
	}

	virtual void Stop()
	{
	}
};

#endif

#endif // __TimerTask_h__
