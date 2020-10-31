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

#ifndef _HQUATERNION_H_
#define _HQUATERNION_H_

//#include "Matrix.h"

struct HQuaternion {
	float s,x,y,z;

	HQuaternion(){}
	HQuaternion(const float Wx, const float Wy);
	HQuaternion(const float Wx, const float Wy, const float Wz);
	HQuaternion(const float is, const float ix, const float iy, const float iz){
		s = is;
		x = ix;
		y = iy;
		z = iz;
	}

	operator const float *() const {
		return &s;
	}

	void operator += (const HQuaternion &v);
	void operator -= (const HQuaternion &v);
	void operator *= (const float scalar);
	void operator /= (const float dividend);
	void normalize();
	void fastNormalize();
};

HQuaternion operator + (const HQuaternion &u, const HQuaternion &v);
HQuaternion operator - (const HQuaternion &u, const HQuaternion &v);
HQuaternion operator - (const HQuaternion &v);
HQuaternion operator * (const HQuaternion &u, const HQuaternion &v);
HQuaternion operator * (const float scalar, const HQuaternion &v);
HQuaternion operator * (const HQuaternion &v, const float scalar);
HQuaternion operator / (const HQuaternion &v, const float dividend);

HQuaternion slerp(const HQuaternion &q0, const HQuaternion &q1, const float t);
HQuaternion scerp(const HQuaternion &q0, const HQuaternion &q1, const HQuaternion &q2, const HQuaternion &q3, const float t);


#endif // _QUATERNION_H_
