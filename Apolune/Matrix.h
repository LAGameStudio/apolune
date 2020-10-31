/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/

//
// matrix.h
//
// matrix header file
//

#ifndef __MATRIX_H__
#define __MATRIX_H__

// includes

#include <math.h>

class Matrix;
class Vector;

// defs
enum {_XX = 0, _XY, _XZ, _YX, _YY, _YZ, _ZX, _ZY, _ZZ};

// structs

// vars

// funcs/classes

class Matrix
{
public:
	float	p[9];

	// constructors
	Matrix() { // nullify matrix
  for ( int i=0; i<9; i++ ) p[i]=0.0f;
 }
	Matrix(	float xx, float xy, float xz,	float yx, float yy, float yz,	float zx, float zy, float zz)	{ // construct matrix from 9 floats
  Set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz);
 }
	Matrix(float ax, float ay, float az)	{ // construct matrix form 3 angles
  for ( int i=0; i<9; i++ ) p[i]=0.0f;
  SetAng(ax, ay, az);
 }
	Matrix(Matrix &m)	{ // construct matrix from another matrix
  for ( int i=0; i<9; i++ ) p[i]=m.p[i];
 }

	// operators
	Matrix operator *(Matrix &m);	// return MatrixMulMatrix(this, m)
	Vector operator *(Vector &v);	// return MatrixMulVector(this, v)
	Matrix operator *(float f);		// return MatrixScale(this, f)
	void operator *=(Matrix &m);	// this = MatrixMulMatrix(this, m)
	void operator *=(float f);		// this = MatrixScale(this, f)

	Matrix operator !();			// return MatrixInverse(this)

	// functions
	void Set(	float xx, float xy, float xz,
				float yx, float yy, float yz,
				float zx, float zy, float zz);	// set teh matrix's values directly
	void SetAng(float ax, float ay, float az);	// specify a xyz rotation matrix
	void Identity();							// set this matrix to identity
	void Invert();								// invert this matrix
};



#endif //__MATRIX_H__