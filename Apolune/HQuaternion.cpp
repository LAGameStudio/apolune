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

#include "HQuaternion.h"
#include "HMath.h"

HQuaternion::HQuaternion(const float Wx, const float Wy){
	float cx = cosf(Wx * 0.5f),
	      sx = sinf(Wx * 0.5f),
	      cy = cosf(Wy * 0.5f),
	      sy = sinf(Wy * 0.5f);


	s =  cx * cy;
	x = -cy * sx;
	y =  cx * sy;
	z =  sx * sy;
}

HQuaternion::HQuaternion(const float Wx, const float Wy, const float Wz){
	float cx = cosf(Wx * 0.5f),
	      sx = sinf(Wx * 0.5f),
	      cy = cosf(Wy * 0.5f),
	      sy = sinf(Wy * 0.5f),
	      cz = cosf(Wz * 0.5f),
	      sz = sinf(Wz * 0.5f);

	s =  cx * cy * cz + sx * sy * sz;
	x = -sx * cy * cz - cx * sy * sz;
	y =  cx * sy * cz - sx * cy * sz;
	z =  sx * sy * cz - cx * cy * sz;
}

void HQuaternion::operator += (const HQuaternion &v){
	s += v.s;
	x += v.x;
	y += v.y;
	z += v.z;
}

void HQuaternion::operator -= (const HQuaternion &v){
	s -= v.s;
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void HQuaternion::operator *= (const float scalar){
	s *= scalar;
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void HQuaternion::operator /= (const float dividend){
	s /= dividend;
	x /= dividend;
	y /= dividend;
	z /= dividend;
}

void HQuaternion::normalize(){
	float invLen = 1.0f / sqrtf(s * s + x * x + y * y + z * z);
	s *= invLen;
	x *= invLen;
	y *= invLen;
	z *= invLen;
}

void HQuaternion::fastNormalize(){
	float invLen = rsqrtf(s * s + x * x + y * y + z * z);
	s *= invLen;
	x *= invLen;
	y *= invLen;
	z *= invLen;
}


HQuaternion operator + (const HQuaternion &u, const HQuaternion &v){
	return HQuaternion(u.s + v.s, u.x + v.x, u.y + v.y, u.z + v.z);
}

HQuaternion operator - (const HQuaternion &u, const HQuaternion &v){
	return HQuaternion(u.s - v.s, u.x - v.x, u.y - v.y, u.z - v.z);
}

HQuaternion operator - (const HQuaternion &v){
	return HQuaternion(-v.s, -v.x, -v.y, -v.z);
}

HQuaternion operator * (const HQuaternion &u, const HQuaternion &v){
	return HQuaternion(
		u.s * v.s - u.x * v.x - u.y * v.y - u.z * v.z,
		u.s * v.x + u.x * v.s + u.y * v.z - u.z * v.y,
		u.s * v.y + u.y * v.s + u.z * v.x - u.x * v.z,
		u.s * v.z + u.z * v.s + u.x * v.y - u.y * v.x);
}

HQuaternion operator * (const float scalar, const HQuaternion &v){
	return HQuaternion(v.s * scalar, v.x * scalar, v.y * scalar, v.z * scalar);
}

HQuaternion operator * (const HQuaternion &v, const float scalar){
	return HQuaternion(v.s * scalar, v.x * scalar, v.y * scalar, v.z * scalar);
}

HQuaternion operator / (const HQuaternion &v, const float dividend){
	return HQuaternion(v.s / dividend, v.x / dividend, v.y / dividend, v.z / dividend);
}


HQuaternion slerp(const HQuaternion &q0, const HQuaternion &q1, const float t){
	float cosTheta = q0.s * q1.s + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

	if (fabsf(1 - cosTheta) < 0.001f){
		return q0 * (1 - t) + q1 * t;
	} else {
		float theta = acosf(cosTheta);
		return (q0 * sinf((1 - t) * theta) + q1 * sinf(t * theta)) / sinf(theta);
	}
}


HQuaternion scerp(const HQuaternion &q0, const HQuaternion &q1, const HQuaternion &q2, const HQuaternion &q3, const float t){
	return slerp(slerp(q1, q2, t), slerp(q0, q3, t), 2 * t * (1 - t));
}
