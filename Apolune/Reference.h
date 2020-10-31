#pragma once

#ifndef __Reference_h__
#define __Reference_h__

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

#include "CLog.h"

template <class T>
class TReference
{
protected:
	T *m_pObject;

public:
	// Constructor/destructor methods
	TReference(T *p=NULL)
	{
		if(m_pObject = p)
			m_pObject->AddRef();
	}
	TReference(const TReference &ref)
	{
		if(m_pObject = ref.m_pObject)
			m_pObject->AddRef();
	}
	~TReference()
	{
		if(m_pObject)
			m_pObject->Release();
	}

	// Manual manipulation methods
	void AddRef()			{ m_pObject->AddRef(); }
	void Release()			{ m_pObject->Release(); m_pObject = NULL; }

	// Implicit conversion operators
	operator T*() const
	{
		return m_pObject;
	}
	T& operator*() const
	{
		LogAssert(m_pObject);
		return *m_pObject;
	}
	T* operator->() const
	{
		LogAssert(m_pObject);
		return m_pObject;
	}

	// Assignment operators
	TReference &operator=(const TReference &ref)
	{
		if(m_pObject != ref.m_pObject)
		{
			if(m_pObject)
				m_pObject->Release();
			if(m_pObject = ref.m_pObject)
				m_pObject->AddRef();
		}
		return *this;
	}
	TReference &operator=(T* p)
	{
		if(m_pObject != p)
		{
			if(m_pObject)
				m_pObject->Release();
			if(m_pObject = p)
				m_pObject->AddRef();
		}
		return *this;
	}

	// Comparison operators (compare addresses)
	bool operator==(T* p) const						{ return m_pObject == p; }
	bool operator!=(T* p) const						{ return m_pObject != p; }
	bool operator==(const TReference &ref) const	{ return m_pObject == ref.m_pObject; }
	bool operator!=(const TReference &ref) const	{ return m_pObject != ref.m_pObject; }

	// Comparison methods (compare values, relies on operator overloading in class being referenced)
	bool eq(const TReference &ref) const			{ return (*m_pObject) == (*ref.m_pObject); }
	bool ne(const TReference &ref) const			{ return (*m_pObject) != (*ref.m_pObject); }
	bool gt(const TReference &ref) const			{ return (*m_pObject) > (*ref.m_pObject); }
	bool lt(const TReference &ref) const			{ return (*m_pObject) < (*ref.m_pObject); }
	bool ge(const TReference &ref) const			{ return (*m_pObject) >= (*ref.m_pObject); }
	bool le(const TReference &ref) const			{ return (*m_pObject) <= (*ref.m_pObject); }
};

// A simple reference counting base class. It is called manual because it does not perform
// automatic cleanup of the object. It has no virtual methods and all methods are inline.
// If performance is critical, you can override the Release() method and keep the cleanup
// code entirely inline. (Warning: As with pointers, if you're keeping a reference to a
// base class of an object, the derived class's cleanup code won't be called unless it
// is properly casted or relies on virtual methods.)
class CManualRefCounter
{
protected:
	unsigned short m_nReferenceCount;
	CManualRefCounter()			{ m_nReferenceCount = 0; }
	~CManualRefCounter()		{ LogAssert(m_nReferenceCount == 0); }

public:
	unsigned short AddRef()
	{
		LogAssert(GetReferenceCount() < 0x7FFF);
		m_nReferenceCount++;
		//m_nReferenceCount = InterlockedIncrement((LPLONG)&m_nReferenceCount);
		return GetReferenceCount();
	}
	unsigned short Release()
	{
		LogAssert(GetReferenceCount() > 0);
		m_nReferenceCount--;
		//m_nReferenceCount = InterlockedDecrement((LPLONG)&m_nReferenceCount);
		return GetReferenceCount();
	}
	unsigned short GetReferenceCount()	{ return (m_nReferenceCount & 0x7FFF); }
	bool IsDead()						{ return (m_nReferenceCount & 0x8000) == 0; }
	void Kill()							{ m_nReferenceCount |= 0x8000; }
};

// This class automatically deletes the object when its reference counter reaches 0.
// Because the object's class is unknown, it relies on virtual destructors. Use this when
// you want the object to be deleted when its reference count reaches 0 and you're not
// concerned about performance (or you're keeping references to a base class).
class CAutoRefCounter : public CManualRefCounter
{
protected:
	virtual ~CAutoRefCounter()		{}
public:
	void Release()					{ if(!CManualRefCounter::Release()) delete this; }
};

// This templatized version of CAutoRefCounter can execute the correct destructor without
// it being virtual. Unfortunately, this can't be used for any class that has other classes
// derive from it.
template <class T>
class TAutoRefCounter : public CManualRefCounter
{
public:
	void Release()					{ if(!CManualRefCounter::Release()) delete (T *)this; }
	typedef TReference<T> Ref;
};

// If you want automatic cleanup but do not want an object to be deleted when the reference
// counter reaches 0 (i.e. when it is part of an object pool), derive from this class and
// override the pure virtual Destroy() method.
class CAbstractRefCounter : public CManualRefCounter
{
protected:
	virtual void Destroy() = 0;
	virtual ~CAbstractRefCounter()	{}
public:
	void Release()					{ if(!CManualRefCounter::Release()) Destroy(); }
};

#endif