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

#include "CMixedHeightMapFactory.h"
#include "Profiler.h"

void CMixedHeightMapFactory::Init(CPropertySet &prop)
{
	CSimpleHeightMapFactory::Init(prop);
	m_nBufferSize = prop.GetIntProperty("buffer.size", 128);

	for(int nFace=0; nFace<6; nFace++)
	{
		m_pb[nFace].Init(m_nBufferSize, m_nBufferSize, 1, 1, GL_ALPHA, GL_FLOAT);
		float *pIndex = (float *)m_pb[nFace].GetBuffer();
		for(int y=0; y<m_nBufferSize; y++)
		{
			for(int x=0; x<m_nBufferSize; x++)
			{
				CPlanetaryMapCoord coord(nFace, (float)x / (m_nBufferSize-1), (float)y / (m_nBufferSize-1));
				CVector v = coord.GetDirectionVector();

				v *= 2;
				float fValue = Noise(v) + -0.1f;
				for(int i=1; i<5; i++)
				{
					v *= m_fLacunarity;
					fValue += (Noise(v) + -0.1f) * m_fExponent[i];
				}
				while(CMath::Abs(fValue) > 1.0f)
				{
					if(fValue > 0.0f)
						fValue = 2 - fValue;
					else
						fValue = -2 - fValue;
				}
				*pIndex = fValue;
				pIndex++;
			}
		}
	}
}

void CMixedHeightMapFactory::BuildNode(CPlanetaryMapNode *pNode)
{
	PROFILE("CMixedHeightMapFactory::BuildNode", 3);
	// Initialize the height map
	for(int nIndex=0; nIndex<BORDER_MAP_COUNT; nIndex++)
	{
		float fHeight = GetHeight(builder.coord[nIndex]);
		fHeight = pNode->GetPlanetaryMap()->GetRadius() + fHeight * pNode->GetPlanetaryMap()->GetMaxHeight();
		builder.coord[nIndex].SetHeight(fHeight);
	}
}

float CMixedHeightMapFactory::GetHeight(const CPlanetaryMapCoord &coord)
{
	// Get the initial height from the pre-generated maps
	float fBase, fExtra;
	m_pb[coord.GetFace()].Interpolate(&fBase, coord.x, coord.y);

	// Compute partial fractal to add detail
	if(fBase > 0.0f)
	{
		CVector v = coord.GetDirectionVector();
		v *= powf(m_fLacunarity, 6);
		fExtra = CMath::Abs(Noise(v)) * m_fExponent[6];
		for(int i=6; i<12; i++)
		{
			v *= m_fLacunarity;
			fExtra += CMath::Abs(Noise(v)) * m_fExponent[i];
		}
		// For numbers from 0..1, 1-Square(1-n) is similar to sqrt(n)
		fBase = fBase - (1-CMath::Square(1-fExtra)) * sqrtf(CMath::Abs(fBase));
	}

	// Return the final adjusted height
	return fBase;
}
