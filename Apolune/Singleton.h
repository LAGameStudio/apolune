#pragma once

#ifndef __Singleton_h__
#define __Singleton_h__

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

// This is a useful templatized base class for making a class into a singleton. The
// singleton can be created and destroyed dynamically, but there can only be one.
// Static methods will return a pointer or reference to the singleton.

#include "macros.h"

template <class T>
class TSingleton
{
private:
	static T *m_pSingleton;

protected:
	TSingleton()
	{
//		LogAssert(!m_pSingleton);
		m_pSingleton = (T *)this;	// Not sure about other compilers, but MSVC++ is smart enough to cast this correctly

		// Manual way to do the pointer arithmetic
		//int nOffset = (int)(T*)1 - (int)(TSingleton <T>*)(T*)1;
		//m_pSingleton = (T*)((int)this + nOffset);
	}
	~TSingleton()
	{
	//	LogAssert(m_pSingleton);
		m_pSingleton = NULL;
	}

public:
	static bool IsValid()	{ return m_pSingleton != NULL; }
	static T *GetPtr()
	{
	//	LogAssert(m_pSingleton);
		return m_pSingleton;
	}
	static T &GetRef()
	{
	//	LogAssert(m_pSingleton);
		return *m_pSingleton;
	}
};

// Because it's templatized, this static member doesn't need to be declared in a CPP file
template <class T> T *TSingleton<T>::m_pSingleton = NULL;


// This is an instance of the singleton class that creates itself if it doesn't already exist
template <class T>
class TAutoSingleton : public TSingleton<T>
{
public:
	static T *GetPtr()
	{
		if(!TSingleton<T>::IsValid())
			new T();
		return TSingleton<T>::GetPtr();
	}
	static T &GetRef()
	{
		if(!TSingleton<T>::IsValid())
			new T();
		return TSingleton<T>::GetRef();
	}
};

#endif // __Singleton_h__