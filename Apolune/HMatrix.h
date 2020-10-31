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

#ifndef _HMATRIX_H_
#define _HMATRIX_H_
#include <math.h>
#include "HVertex.h"
#include "HPlane.h"
#include "HQuaternion.h"

// The HMatrix structure defines a 4x4 Matrix

#define POSITIVE_X 0
#define NEGATIVE_X 1
#define POSITIVE_Y 2
#define NEGATIVE_Y 3
#define POSITIVE_Z 4
#define NEGATIVE_Z 5

struct HMatrix {
	float m[16];

	HMatrix(){}
	HMatrix(float initVal){
		for (int i = 0; i < 16; i++) m[i] = initVal;
	}
	HMatrix(float im[]){
		for (int i = 0; i < 16; i++) m[i] = im[i];
	}
	HMatrix(const HQuaternion &q);

	void loadIdentity();
	
	void loadRotateX(const float angle);
	void loadRotateY(const float angle);
	void loadRotateZ(const float angle);
	void loadRotateXY(const float angleX, const float angleY);
	void loadRotateZXY(const float angleX, const float angleY, const float angleZ);
	void loadRotate(const HVertex &v, const float angle);

	void loadProjectionMatrix(float fov, float aspect, float zNear, float zFar);
	void loadShadowMatrix(const HPlane &plane, const HVertex &lightPos);
	void loadCubemapMatrix(const unsigned int side);
	void loadCubemapMatrixOGL(const unsigned int side);


	void translate(const HVertex &v);
	void scale(const float sx, const float sy, const float sz);

	void rotateX(const float angle);
	void rotateY(const float angle);
	void rotateZ(const float angle);
	void rotateXY(const float angleX, const float angleY);
	void rotateZXY(const float angleX, const float angleY, const float angleZ);
	void rotate(const HVertex &v, const float angle);

	void transpose();

	operator float *(){
		return m;
	}
	const float &operator () (const int row, const int column) const {
		return m[row + (column << 2)];
	}
	const float &operator [] (const int index) const {
		return m[index];
	}
	void operator += (const HMatrix &v);
	void operator -= (const HMatrix &v);
	void operator *= (const HMatrix &v);

	void operator += (const float term);
	void operator -= (const float term);
	void operator *= (const float scalar);
	void operator /= (const float dividend);
};

HMatrix operator + (const HMatrix &u, const HMatrix &v);
HMatrix operator - (const HMatrix &u, const HMatrix &v);
HMatrix operator - (const HMatrix &v);
HMatrix operator * (const HMatrix &u, const HMatrix &v);
HVertex operator * (const HMatrix &mat, const HVertex &v);
HMatrix operator + (const HMatrix &v, const float term);
HMatrix operator - (const HMatrix &v, const float term);
HMatrix operator * (const float scalar, const HMatrix &v);
HMatrix operator * (const HMatrix &v, const float scalar);
HMatrix operator / (const HMatrix &v, const float dividend);
HMatrix operator ! (const HMatrix &v);

HMatrix transpose(const HMatrix &u);

#endif
