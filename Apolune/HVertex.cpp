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

#include "HVertex.h"

float &HVertex::operator [] (const int index){
	return *(&x + index);
}

void HVertex::operator += (const HVertex &v){
	x += v.x;
	y += v.y;
	z += v.z;
}

void HVertex::operator -= (const HVertex &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void HVertex::operator *= (const float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
}
void HVertex::operator /= (const float dividend){
    x /= dividend;
    y /= dividend;
    z /= dividend;
}

void HVertex::operator *= (const double scalar){
	x = (float)((double)x*scalar);
	y = (float)((double)y*scalar);
	z = (float)((double)z*scalar);
}
void HVertex::operator /= (const double dividend){
    x= (float)((double) x / dividend);
    y= (float)((double) y / dividend);
    z= (float)((double) z / dividend);
}

bool operator == (const HVertex &v0, const HVertex &v1){
	return (v0.x == v1.x && v0.y == v1.y && v0.z == v1.z);
}

bool operator != (const HVertex &v0, const HVertex &v1){
	return (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z);
}

void HVertex::normalize(){
	float invLen = 1.0f / sqrtf(x * x + y * y + z * z);
	x *= invLen;
	y *= invLen;
	z *= invLen;
}

void HVertex::fastNormalize(){
	float invLen = rsqrtf(x * x + y * y + z * z);
	x *= invLen;
	y *= invLen;
	z *= invLen;
}

/* ------------------------------------------------------------ */

HVertex operator + (const HVertex &u, const HVertex &v){
	return HVertex(u.x + v.x,u.y + v.y,u.z + v.z);
}

HVertex operator - (const HVertex &u, const HVertex &v){
	return HVertex(u.x - v.x,u.y - v.y,u.z - v.z);
}

HVertex operator - (const HVertex &v){
	return HVertex(-v.x,-v.y,-v.z);
}

float operator * (const HVertex &u, const HVertex &v){
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

HVertex operator * (const float scalar, const HVertex &v){
	return HVertex(scalar * v.x,scalar * v.y,scalar * v.z);
}

HVertex operator * (const double scalar, const HVertex &v){
	return HVertex(scalar * (double) v.x,scalar * (double) v.y,scalar * (double) v.z);
}

HVertex operator * (const HVertex &v, const float scalar){
	return HVertex(scalar * v.x,scalar * v.y,scalar * v.z);
}

HVertex operator / (const HVertex &v, const float dividend){
	return HVertex(v.x / dividend,v.y / dividend,v.z / dividend);
}

HVertex operator * (const HVertex &v, const double scalar){
	return HVertex(scalar * (double)v.x,scalar * (double)v.y,scalar * (double) v.z);
}

HVertex operator / (const HVertex &v, const double dividend){
	return HVertex((double) v.x / dividend, (double) v.y / dividend, (double) v.z / dividend);
}



HVertex cross(const HVertex &u, const HVertex &v){
	return HVertex(u.y * v.z - v.y * u.z, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float dist(const HVertex &u, const HVertex &v){
	return sqrtf(sqrf(u.x - v.x) + sqrf(u.y - v.y) + sqrf(u.z - v.z));
}

float length(const HVertex &v){
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float fastLength(const HVertex &v){
	float lsqr = v.x * v.x + v.y * v.y + v.z * v.z;
	return lsqr * rsqrtf(lsqr);
}

float lengthSqr(const HVertex &v){
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

HVertex normalize(const HVertex &v){
	float invLen = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return v * invLen;
}

HVertex fastNormalize(const HVertex &v){
	float invLen = rsqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return v * invLen;
}

unsigned int getRGBANormal(HVertex v){
	unsigned int norm;
	int tmp;
	float lenSqr = lengthSqr(v);
	if (lenSqr > 1) v *= rsqrtf(lenSqr);
	
	tmp = int((v.x+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm = tmp;
	tmp = int((v.y+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm |= tmp << 8;
	tmp = int((v.z+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm |= tmp << 16;
	return norm;
}

unsigned int getBGRANormal(HVertex v){
	unsigned int norm;
	int tmp;
	float lenSqr = lengthSqr(v);
	if (lenSqr > 1) v *= rsqrtf(lenSqr);
	
	tmp = int((v.z+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm = tmp;
	tmp = int((v.y+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm |= tmp << 8;
	tmp = int((v.x+1) * 127.5f);
	if (tmp < 0  ) tmp = 0;
	if (tmp > 255) tmp = 255;
	norm |= tmp << 16;
	return norm;
}
