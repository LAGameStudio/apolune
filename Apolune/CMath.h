#pragma once


#ifndef __CMath_h__
#define __CMath_h__

#include <math.h> 

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

// Defines

/*
#define PI					3.14159f			// PI
#define HALF_PI				1.57080f			// PI / 2
#define TWO_PI				6.28318f			// PI * 2
*/

#include "macros.h"

#define INV_PI				0.318310f			// 1 / PI
#define INV_TWO_PI			0.159155f			// 1 / (PI*2)
#define INV_HALF_PI			0.636618f			// 1 / (PI/2)

#define LOGHALF				-0.693147f			// log(0.5)
#define LOGHALFI			-1.442695f			// Inverse of log(0.5)
//#define DELTA				1e-6f				// Small number for comparing floating point numbers
#define MAX_DIMENSIONS		4					// Maximum number of dimensions in a noise object
#define MAX_OCTAVES			128					// Maximum # of octaves in an fBm object

#define HALF_RAND			(RAND_MAX/2)

#define LOG_GLERROR() {\
	GLenum err = glGetError();\
	if(err != GL_NO_ERROR)\
		OUTPUT("[%s line %d] GL Error: %s", __FILE__, __LINE__, gluErrorString(err));\
}


template <class T> void Swap(T &a, T &b)		{ T t = a; a = b; b = t; }


namespace CMath
{
	template <class T> T Abs(T a)					{ return (a < 0 ? -a : a); }
	template <class T> T Min(T a, T b)				{ return (a < b ? a : b); }
	template <class T> T Max(T a, T b)				{ return (a > b ? a : b); }
	template <class T> T Avg(T a, T b)				{ return (a + b) / (T)2; }
	template <class T> T Clamp(T a, T b, T x)		{ return (x < a ? a : (x > b ? b : x)); }
	template <class T> T Lerp(T a, T b, T x)		{ return (a + x * (b - a)); }
	template <class T> T Cubic(T a)					{ return a * a * (3 - 2*a); }
	template <class T> T Sign(T a)					{ return a < 0 ? (T)-1 : (T)1; }
	template <class T> T Square(T a)				{ return a * a; }
	template <class T> T SquareWithSign(T a)		{ return a < 0 ? -(a * a) : (a * a); }
	template <class T> T Step(T a, T x)				{ return (T)(x >= a); }
	template <class T> T Boxstep(T a, T b, T x)		{ return Clamp(0, 1, (x-a)/(b-a)); }
	template <class T> T Pulse(T a, T b, T x)		{ return (T)((x >= a) - (x >= b)); }

	inline float ToRadians(float fDegrees)			{ return fDegrees * 0.01745329f; }
	inline float ToDegrees(float fRadians)			{ return fRadians * 57.295779f; }
	inline float Sin(float a)						{ return sinf(a); }
	inline float Cos(float a)						{ return cosf(a); }
	inline float Tan(float a)						{ return tanf(a); }
	inline float Asin(float a)						{ return asinf(a); }
	inline float Acos(float a)						{ return acosf(a); }
	inline float Atan(float a)						{ return atanf(a); }
	inline float Sqrt(float a)						{ return sqrtf(a); }

	inline int Floor(float a)						{ return ((int)a - (a < 0 && a != (int)a)); }
	inline int Ceiling(float a)						{ return ((int)a + (a > 0 && a != (int)a)); }
	inline float SqrtWithSign(float a)				{ return a < 0 ? -sqrtf(-a) : sqrtf(a); }
	inline float Gamma(float a, float g)			{ return powf(a, 1/g); }
	inline float Bias(float a, float b)				{ return powf(a, logf(b) * LOGHALFI); }
	inline float Expose(float l, float k)			{ return (1 - expf(-l * k)); }

	inline float Gain(float a, float b)
	{
		if(a <= EPSILON)
			return 0;
		if(a >= 1-EPSILON)
			return 1;

		register float p = (logf(1 - b) * LOGHALFI);
		if(a < 0.5)
			return powf(2 * a, p) * 0.5f;
		else
			return 1 - powf(2 * (1 - a), p) * 0.5f;
	}

	inline float Smoothstep(float a, float b, float x)
	{
		if(x <= a)
			return 0;
		if(x >= b)
			return 1;
		return Cubic((x - a) / (b - a));
	}

	inline float Mod(float a, float b)
	{
		a -= ((int)(a / b)) * b;
		if(a < 0)
			a += b;
		return a;
	}

	inline void Normalize(float *f, int n)
	{
		int i;
		float fMagnitude = 0;
		for(i=0; i<n; i++)
			fMagnitude += f[i]*f[i];
		fMagnitude = 1 / sqrtf(fMagnitude);
		for(i=0; i<n; i++)
			f[i] *= fMagnitude;
	}
};

#ifndef WIN32
#define DWORD unsigned long
#define _MAX_PATH 512
inline char* itoa(int val, char *buf, int base) {
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}
#endif

#endif