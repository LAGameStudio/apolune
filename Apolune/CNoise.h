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

#include <stdlib.h>
#include "CMath.h"

/*******************************************************************************
* Class: CRandom
********************************************************************************
* This class wraps a random number generator. I plan to implement my own random
* number generator so I can keep the seeds as member variables (which is more
* flexible than using statics or globals). I was using one I found on the
* Internet implemented in assembler, but I was having problems with it so I
* removed it for this demo.
*******************************************************************************/
#define BITS		32
#define MSB			0x80000000L
#define ALL_BITS	0xffffffffL
#define HALF_RANGE	0x40000000L
#define STEP		7
class CRandom
{
protected:
	long m_nSeed;
	long m_nQuotient;
	long m_nRemainder;

public:
	CRandom()						{}
	CRandom(unsigned int nSeed)		{ Init(nSeed); }
	void Init(unsigned int nSeed)	{ srand(nSeed); }
	double Random()					{ return (double)rand()/(double)RAND_MAX; }
	double RandomD(double dMin, double dMax)
	{
		double dInterval = dMax - dMin;
		double d = dInterval * Random();
		return dMin + CMath::Min(d, dInterval);
	}
	unsigned int RandomI(unsigned int nMin, unsigned int nMax)
	{
		unsigned int nInterval = nMax - nMin;
		unsigned int i = (unsigned int)((nInterval+1.0) * Random());
		return nMin + CMath::Min(i, nInterval);
	}
};

class CRandomLCG
{
protected:
	unsigned int m_nSeed;
	unsigned int m_nQuotient;
	unsigned int m_nRemainder;

public:
	CRandomLCG(unsigned int nSeed)
	{
		m_nSeed = nSeed;
		m_nQuotient = LONG_MAX / 16807L;
		m_nRemainder = LONG_MAX % 16807L;
	}

	unsigned int rand()
	{
		if(m_nSeed <= m_nQuotient)
			m_nSeed = (m_nSeed * 16807L) % LONG_MAX;
		else
		{
			int nHigh = m_nSeed / m_nQuotient;
			int nLow  = m_nSeed % m_nQuotient;

			int test = 16807L * nLow - m_nRemainder * nHigh;

			if(test > 0)
				m_nSeed = test;
			else
				m_nSeed = test + LONG_MAX;
		}

		return m_nSeed;
	}
};

class CRandomR250
{
protected:
	unsigned int m_nR250buffer[250];
	int m_nR250index;

public:
	CRandomR250(unsigned int nSeed)
	{
		CRandomLCG lcg(nSeed);

		int j, k;
		unsigned int mask, msb;
		m_nR250index = 0;
		for(j = 0; j < 250; j++)
			m_nR250buffer[j] = rand();
		for(j = 0; j < 250; j++)	// set some MSBs to 1
			if(lcg.rand() > HALF_RANGE)
				m_nR250buffer[j] |= MSB;

		msb = MSB;	        // turn on diagonal bit
		mask = ALL_BITS;	// turn off the leftmost bits

		for (j = 0; j < BITS; j++)
		{
			k = STEP * j + 3;	// select a word to operate on
			m_nR250buffer[k] &= mask; // turn off bits left of the diagonal
			m_nR250buffer[k] |= msb;	// turn on the diagonal bit
			mask >>= 1;
			msb  >>= 1;
		}
	}

	unsigned int r250()
	{
		register int j;
		register unsigned int new_rand;

		if ( m_nR250index >= 147 )
			j = m_nR250index - 147;
		else
			j = m_nR250index + 103;

		new_rand = m_nR250buffer[ m_nR250index ] ^ m_nR250buffer[ j ];
		m_nR250buffer[ m_nR250index ] = new_rand;

		if ( m_nR250index >= 249 )
			m_nR250index = 0;
		else
			m_nR250index++;

		return new_rand;

	}

	double dr250()
	{
		register int j;
		register unsigned int new_rand;

		if ( m_nR250index >= 147 )
			j = m_nR250index - 147;
		else
			j = m_nR250index + 103;

		new_rand = m_nR250buffer[ m_nR250index ] ^ m_nR250buffer[ j ];
		m_nR250buffer[ m_nR250index ] = new_rand;

		if ( m_nR250index >= 249 )	/* increment pointer for next time */
			m_nR250index = 0;
		else
			m_nR250index++;

		return (double)new_rand / ALL_BITS;
	}
};

/*******************************************************************************
* Class: CNoise
********************************************************************************
* This class implements the Perlin noise function. Initialize it with the number
* of dimensions (1 to 4) and a random seed. I got the source for the first 3
* dimensions from "Texturing & Modeling: A Procedural Approach". I added the
* extra dimension because it may be desirable to use 3 spatial dimensions and
* one time dimension. The noise buffers are set up as member variables so that
* there may be several instances of this class in use at the same time, each
* initialized with different parameters.
*******************************************************************************/
class CNoise
{
protected:
	int m_nDimensions;						// Number of dimensions used by this object
	unsigned char m_nMap[256];				// Randomized map of indexes into buffer
	float m_nBuffer[256][MAX_DIMENSIONS];	// Random n-dimensional buffer

	float Lattice(int ix, float fx, int iy=0, float fy=0, int iz=0, float fz=0, int iw=0, float fw=0)
	{
		int n[4] = {ix, iy, iz, iw};
		float f[4] = {fx, fy, fz, fw};
		int i, nIndex = 0;
		for(i=0; i<m_nDimensions; i++)
			nIndex = m_nMap[(nIndex + n[i]) & 0xFF];
		float fValue = 0;
		for(i=0; i<m_nDimensions; i++)
			fValue += m_nBuffer[nIndex][i] * f[i];
		return fValue;
	}

public:
	CNoise()	{}
	CNoise(int nDimensions, unsigned int nSeed)	{ Init(nDimensions, nSeed); }
	void Init(int nDimensions, unsigned int nSeed);
	float Noise(float *f);
};

/*******************************************************************************
* Class: CSeededNoise
********************************************************************************
*******************************************************************************/
class CSeededNoise
{
protected:
	float m_nBuffer[64][64];

	float Lattice(int ix, float fx, int iy=0, float fy=0, int iz=0, float fz=0)
	{
		float fValue = m_nBuffer[ix][iy];
		return fValue;
	}

public:
	CSeededNoise()	{}
	CSeededNoise(unsigned int nSeed)	{ Init(nSeed); }
	void Init(unsigned int nSeed);
	float Noise(float *f);
};

/*******************************************************************************
* Class: CFractal
********************************************************************************
* This class implements fBm, or fractal Brownian motion. Since fBm uses Perlin
* noise, this class is derived from CNoise. Initialize it with the number of
* dimensions (1 to 4), a random seed, H (roughness ranging from 0 to 1), and
* the lacunarity (2.0 is often used). Many of the fractal routines came from
* "Texturing & Modeling: A Procedural Approach". fBmTest() is my own creation,
* and I created it to generate my first planet.
*******************************************************************************/
class CFractal : public CNoise
{
protected:
	float m_fH;
	float m_fLacunarity;
	float m_fExponent[MAX_OCTAVES];

public:
	CFractal()	{}
	CFractal(int nDimensions, unsigned int nSeed, float fH, float fLacunarity)
	{
		Init(nDimensions, nSeed, fH, fLacunarity);
	}
	void Init(int nDimensions, unsigned int nSeed, float fH, float fLacunarity)
	{
		CNoise::Init(nDimensions, nSeed);
		m_fH = fH;
		m_fLacunarity = fLacunarity;
		float f = 1;
		for(int i=0; i<MAX_OCTAVES; i++) 
		{
			m_fExponent[i] = powf(f, -m_fH);
			f *= m_fLacunarity;
		}
	}
	float fBm(float *f, float fOctaves);
	float Turbulence(float *f, float fOctaves);
	float Multifractal(float *f, float fOctaves, float fOffset);
	float Heterofractal(float *f, float fOctaves, float fOffset);
	float HybridMultifractal(float *f, float fOctaves, float fOffset, float fGain);
	float RidgedMultifractal(float *f, float fOctaves, float fOffset, float fThreshold);
	float fBmTest(float *f, float fOctaves, float fOffset=-0.1f);
	float fBmTest2(float *f, float fOctaves, float fGain=2.0f, float fOffset=-0.1f);
	float fBmTest3(float *f, float fOctaves, float fGain=2.0f, float fOffset=-0.1f);
};