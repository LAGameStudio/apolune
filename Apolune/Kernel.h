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

#ifndef __Kernel_h__
#define __Kernel_h__

using namespace std;
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <list>
#include <iostream>

#include "Reference.h"

class CKernelTask : public CAutoRefCounter
{
protected:
	unsigned short m_nPriority;
	bool m_bKillFlag;

	CKernelTask()
	{
		m_nPriority = 0;
		m_bKillFlag = false;
	}

public:
	typedef TReference<CKernelTask> Ref;
	void SetKillFlag()				{ m_bKillFlag = true; }
	bool GetKillFlag()				{ return m_bKillFlag; }
	unsigned short GetPriority()	{ return m_nPriority; }

	virtual bool Start()=0;
	virtual void Stop()=0;
	virtual void Update()=0;
	virtual void OnSuspend() {}
	virtual void OnResume() {}
};

#define DEFAULT_TASK_CREATOR(class_name)\
public:\
	typedef TReference<class_name> Ref;\
	static class_name *Create(int nPriority)\
	{\
  OUTPUT("Creating kernel task %s\n", #class_name);\
		class_name *pTask = new class_name();\
		pTask->m_nPriority = nPriority;\
		return pTask;\
	}


class CKernel : public TSingleton<CKernel>
{
protected:
	std::list<CKernelTask::Ref> m_lTasks;
	std::list<CKernelTask::Ref> m_lPaused;

	void Insert(CKernelTask::Ref task)
	{
		// Insert the new task in priority order
		std::list<CKernelTask::Ref>::iterator it;
		for(it=m_lTasks.begin(); it!=m_lTasks.end(); it++)
		{
			if((*it)->GetPriority() > task->GetPriority())
				break;
		}
		m_lTasks.insert(it, task);
	}

	CKernel()
	{
	}
	~CKernel()
	{
	}

public:
	static CKernel *Create()
	{
		return new CKernel();
	}
	static void Destroy()
	{
		delete CKernel::GetPtr();
	}

	bool IsTaskRunning(CKernelTask::Ref task)
	{
		for(std::list<CKernelTask::Ref>::iterator i=m_lTasks.begin(); i!=m_lTasks.end(); i++)
		{
			if(*i == task)
				return true;
		}
		return false;
	}

	bool AddTask(CKernelTask::Ref task)
	{
		if(!task->Start())
			return false;
		Insert(task);
		return true;
	}

	void RemoveTask(CKernelTask::Ref task)
	{
		for(std::list<CKernelTask::Ref>::iterator i=m_lTasks.begin(); i!=m_lTasks.end(); i++)
		{
			if(*i == task)
				task->SetKillFlag();
		}
	}

	void SuspendTask(CKernelTask::Ref task)
	{
		std::list<CKernelTask::Ref>::iterator i = std::find(m_lTasks.begin(), m_lTasks.end(), task);
		if(i != m_lTasks.end())
		{
			m_lTasks.erase(i);
			m_lPaused.push_back(task);
		}
	}

	void ResumeTask(CKernelTask::Ref task)
	{
		std::list<CKernelTask::Ref>::iterator i = std::find(m_lPaused.begin(), m_lPaused.end(), task);
		if(i != m_lPaused.end())
		{
			m_lPaused.erase(i);
			Insert(task);
		}
	}

	void KillAllTasks()
	{
		for(std::list<CKernelTask::Ref>::iterator i=m_lTasks.begin(); i!=m_lTasks.end(); i++)
			(*i)->SetKillFlag();
	}

	int Execute()
	{
		while(!m_lTasks.empty())
		{
			// Loop to execute tasks, removing any dead ones
			std::list<CKernelTask::Ref>::iterator i = m_lTasks.begin();
			while(i != m_lTasks.end())
			{
				if((*i)->GetKillFlag())
				{
					(*i)->Stop();
					i = m_lTasks.erase(i);
				}
				else
				{
					(*i)->Update();
					i++;
				}
			}
		}
		return 0;
	}
};

#endif // __Kernel_h__
