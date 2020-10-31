#include "CTexture.h"
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

#include "CTexture.h"


void CTexture::Init(CPixelBuffer *pBuffer, bool bClamp, bool bMipmap)
{
	Cleanup();
	m_nType = (pBuffer->GetDepth() > 1) ? GL_TEXTURE_3D : (pBuffer->GetHeight() > 1) ? GL_TEXTURE_2D : GL_TEXTURE_1D;
	if(m_nType == GL_TEXTURE_2D && pBuffer->GetWidth() != pBuffer->GetHeight())
		m_nType = GL_TEXTURE_RECTANGLE_ARB;

	glGenTextures(1, &m_nID);
	Bind();
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_R, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, bMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	switch(m_nType)
	{
		case GL_TEXTURE_1D:
			if(bMipmap)
				gluBuild1DMipmaps(m_nType, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			else
				glTexImage1D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
			if(bMipmap)
				gluBuild2DMipmaps(m_nType, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			else
				glTexImage2D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		case GL_TEXTURE_3D:
			glTexImage3D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetDepth(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
	}
	glReportError( glGetError() );
}

void CTexture::Update(CPixelBuffer *pBuffer, int nLevel)
{
	Bind();
	switch(m_nType)
	{
		case GL_TEXTURE_1D:
			glTexSubImage1D(m_nType, nLevel, 0, pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
			glTexSubImage2D(m_nType, nLevel, 0, 0, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		case GL_TEXTURE_3D:
			glTexSubImage3D(m_nType, nLevel, 0, 0, 0, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetDepth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
	}
	glReportError( glGetError() );
}

void CTexture::InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp)
{
	Cleanup();
	m_nType = (nHeight == 1) ? GL_TEXTURE_1D : GL_TEXTURE_2D;
	glGenTextures(1, &m_nID);
	Bind();
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	switch(m_nType)
	{
		case GL_TEXTURE_1D:
			glCopyTexImage1D(m_nType, 0, GL_RGBA, x, y, nWidth, 0);
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
			glCopyTexImage2D(m_nType, 0, GL_RGBA, x, y, nWidth, nHeight, 0);
			break;
		case GL_TEXTURE_3D:
			//glCopyTexImage3D(m_nType, 0, GL_RGBA, x, y, nWidth, nHeight, 0);
			break;
	}
	glReportError( glGetError() );
}

void CTexture::UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx, int nOffy, int nOffz)
{
	Bind();
	switch(m_nType)
	{
		case GL_TEXTURE_1D:
			glCopyTexSubImage1D(m_nType, 0, nOffx, x, y, nWidth);
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
			glCopyTexSubImage2D(m_nType, 0, nOffx, nOffy, x, y, nWidth, nHeight);
			break;
		case GL_TEXTURE_3D:
			glCopyTexSubImage3D(m_nType, 0, nOffx, nOffy, nOffz, x, y, nWidth, nHeight);
			break;
	}
	glReportError( glGetError() );
}
