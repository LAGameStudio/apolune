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

#include "GLSetup.h"
#include "GLWindow.h"
#include "CLog.h"

#include "CPixelBuffer.h"


/*******************************************************************************
* Class: CTexture
********************************************************************************
* This class encapsulates OpenGL texture objects. You initialize it with a
* CPixelBuffer instance and a flag indicating whether you want mipmaps to be
* generated.
*******************************************************************************/
class CTexture
{
protected:
	int m_nType;					// GL_TEXTURE_1D or GL_TEXTURE_2D
	unsigned int m_nID;				// OpenGL-generated texture ID

public:

	CTexture()		{ m_nID = (unsigned int)-1; }
	CTexture(CPixelBuffer *pBuffer, bool bMipmap=true)
	{
		m_nID = (unsigned int)-1;
		Init(pBuffer, bMipmap);
	}
	~CTexture()		{ Cleanup(); }
	void Cleanup()
	{
		if(m_nID != (unsigned int)-1)
		{
			glDeleteTextures(1, &m_nID);
			m_nID = (unsigned int)-1;
		}
	}

	static void Enable(int nType)			{ glEnable(nType); }
	static void Disable(int nType)			{ glDisable(nType); }
	
	DWORD GetID()						{ return m_nID; }
	int GetType()						{ return m_nType; }
	void Bind()							{ if(m_nID != (unsigned int)-1) glBindTexture(m_nType, m_nID); }
	void Enable()						{ if(m_nID != (unsigned int)-1) { Bind(); glEnable(m_nType); } }
	void Disable()						{ if(m_nID != (unsigned int)-1) glDisable(m_nType); }

	void Init(CPixelBuffer *pBuffer, bool bClamp=true, bool bMipmap=true);
	void Update(CPixelBuffer *pBuffer, int nLevel=0);

	// Use when rendering to texture (either in the back buffer or a CPBuffer)
	void InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp=true);
	void UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx=0, int nOffy=0, int nOffz=0);
};


class CTextureArray : public CTexture
{
protected:
	int m_nTextureSize;
	int m_nPartitionSize;
	int m_nChannels;
	int m_nFormat;
	int m_nDataType;
	int m_nArrayWidth;
	int m_nStackSize;
	int m_nStackIndex;
	int *m_pStack;
	CPixelBuffer m_pb;

public:
	CTextureArray()
	{
		m_pStack = NULL;
	}
	~CTextureArray()
	{
		Cleanup();
	}

	void Init(int nTextureSize, int nPartitionSize, int nChannels, int nFormat, int nDataType)
	{
		Cleanup();

		m_nTextureSize = nTextureSize;
		m_nPartitionSize = nPartitionSize;
		m_nChannels = nChannels;
		m_nFormat = nFormat;
		m_nDataType = nDataType;
		m_nArrayWidth = m_nTextureSize / m_nPartitionSize;

		m_nStackIndex = 0;
		m_nStackSize = m_nArrayWidth * m_nArrayWidth;
		m_pStack = new int[m_nStackSize];
		for(int n = 0; n < m_nStackSize; n++)
			m_pStack[n] = n;

		m_pb.Init(nTextureSize, nTextureSize, 1, nChannels, nFormat, nDataType);
		memset(m_pb.GetBuffer(), 0xFF, m_pb.GetBufferSize());
		CTexture::Init(&m_pb, true, false);
	}

	void Cleanup()
	{
		if(m_pStack)
		{
			delete m_pStack;
			m_pStack = NULL;
		}
		CTexture::Cleanup();
	}

	// Call to lock one of the sub-textures
	int LockTexture()
	{
		LogAssert(m_nStackIndex < m_nStackSize);
		if(m_nStackIndex >= m_nStackSize)
			return m_nStackSize;
		return m_pStack[m_nStackIndex++];
	}

	// Call to release one of the sub-textures
	void ReleaseTexture(int nTexture)
	{
		LogAssert(m_nStackIndex > 0 && nTexture >= 0 && nTexture < m_nStackSize);
		if(m_nStackIndex <= 0 || nTexture < 0 || nTexture >= m_nStackSize)
			return;
		m_pStack[--m_nStackIndex] = nTexture;
	}

	// Call to update one of the sub-textures
	void Update(int nTexture, CPixelBuffer *pBuffer)
	{
		if(nTexture < 0 || nTexture >= m_nStackSize)
			return;

		Bind();
		int x = nTexture % m_nArrayWidth;
		int y = nTexture / m_nArrayWidth;
		LogAssert(pBuffer->GetWidth() == m_nPartitionSize);
		LogAssert(pBuffer->GetHeight() == m_nPartitionSize);
		LogAssert(pBuffer->GetFormat() == m_nFormat);
		LogAssert(pBuffer->GetDataType() == m_nDataType);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x*m_nPartitionSize, y*m_nPartitionSize, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glReportError( glGetError() );
		/*unsigned char *pSrc = (unsigned char *)pBuffer->GetBuffer();
		for(int i=0; i<m_nPartitionSize; i++)
		{
			unsigned char *pDest = (unsigned char *)m_pb(x*m_nPartitionSize, y*m_nPartitionSize+i, 0);
			for(int j=0; j<m_nPartitionSize; j++)
			{
				*pDest++ = *pSrc++;
				*pDest++ = *pSrc++;
				*pDest++ = *pSrc++;
			}
		}*/
	}
	void Flush()
	{
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pb.GetWidth(), m_pb.GetHeight(), m_pb.GetFormat(), m_pb.GetDataType(), m_pb.GetBuffer());
	}

	// Call to generate a texture matrix to map 0..1 to the corners of a sub-texture
	void MapCorners(int nTexture, CMatrix4 &mTexture)
	{
		LogAssert(nTexture >= 0 && nTexture < m_nStackSize);
		if(nTexture < 0 || nTexture >= m_nStackSize)
			return;

		// Sets the corners of the texture coordinates (0-1 in x and y dimensions) to the centers
		// of the texels in the array element being accessed
		float fXScale = (float)(m_nPartitionSize-1) / (float)m_nTextureSize;
		float fYScale = (float)(m_nPartitionSize-1) / (float)m_nTextureSize;
		float fXOffset = ((nTexture % m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
		float fYOffset = ((nTexture / m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
		mTexture.STMatrix(CVector(fXScale, fYScale, 1.0f), CVector(fXOffset, fYOffset, 0.0f));
	}

	// Call to generate a texture matrix to map something other than 0..1 to the corners of a sub-texture
	void MapCorners(int nTexture, CMatrix4 &mTexture, float fXMin, float fYMin, float fXMax, float fYMax)
	{
		LogAssert(nTexture >= 0 && nTexture < m_nStackSize);
		if(nTexture < 0 || nTexture >= m_nStackSize)
			return;

		// Sets the corners of the texture coordinates (0-1 in x and y dimensions) to the centers
		// of the texels in the array element being accessed
		float fXScale = (float)(m_nPartitionSize-1) / ((float)m_nTextureSize * (fXMax-fXMin));
		float fYScale = (float)(m_nPartitionSize-1) / ((float)m_nTextureSize * (fYMax-fYMin));
		float fXOffset = ((nTexture % m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
		float fYOffset = ((nTexture / m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
		mTexture.STMatrix(CVector(fXScale, fYScale, 1.0f), CVector(fXOffset, fYOffset, 0.0f));
		mTexture.Translate(CVector(-fXMin, -fYMin, 0.0f));
	}
};