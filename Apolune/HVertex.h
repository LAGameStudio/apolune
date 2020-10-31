#pragma once
/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#ifndef _HVERTEX_H_
#define _HVERTEX_H_

#include "HMath.h"

#define HVector HVertex

// The Vertex structure defines a single point in space or a vector
struct HVertex {
	float x,y,z;
	HVertex(){}
	HVertex(const double ix, const double iy, const double iz){
		x = (float) ix;
		y = (float) iy;
		z = (float) iz;
	}
	HVertex(const float ix, const float iy, const float iz){
		x = ix;
		y = iy;
		z = iz;
	}
	operator const float *() const {
		return &x;
	}
	float &operator [] (const int index);
	void operator += (const HVertex &v);
	void operator -= (const HVertex &v);
	void operator *= (const float scalar);
	void operator /= (const float dividend);
	void operator *= (const double scalar);
	void operator /= (const double dividend);
	void normalize();
	void fastNormalize();
};

bool operator == (const HVertex &v0, const HVertex &v1);
bool operator != (const HVertex &v0, const HVertex &v1);

HVertex operator + (const HVertex &u, const HVertex &v);
HVertex operator - (const HVertex &u, const HVertex &v);
HVertex operator - (const HVertex &v);
float  operator * (const HVertex &u, const HVertex &v);
HVertex operator * (const float scalar, const HVertex &v);
HVertex operator * (const double scalar, const HVertex &v);
HVertex operator * (const HVertex &v, const float scalar);
HVertex operator / (const HVertex &v, const float dividend);
HVertex operator * (const HVertex &v, const double scalar);
HVertex operator / (const HVertex &v, const double dividend);

HVertex cross(const HVertex &u, const HVertex &v);
float dist(const HVertex &u, const HVertex &v);
float length(const HVertex &v);
float fastLength(const HVertex &v);
float lengthSqr(const HVertex &v);
HVertex normalize(const HVertex &v);
HVertex fastNormalize(const HVertex &v);

unsigned int getRGBANormal(HVertex v);
unsigned int getBGRANormal(HVertex v);

#endif
