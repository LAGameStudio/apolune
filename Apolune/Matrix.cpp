//
// matrix.cpp
//
//
//

#include "Matrix.h"

#include "Vertex.h"

// vars

// functions

Matrix Matrix::operator *(Matrix &m)	// return MatrixMulMatrix(this, m)
{
	Matrix temp;

	temp.p[_XX] = p[_XX]*m.p[_XX]+p[_XY]*m.p[_YX]+p[_XZ]*m.p[_ZX];
	temp.p[_YX] = p[_YX]*m.p[_XX]+p[_YY]*m.p[_YX]+p[_YZ]*m.p[_ZX];
	temp.p[_ZX] = p[_ZX]*m.p[_XX]+p[_ZY]*m.p[_YX]+p[_ZZ]*m.p[_ZX];

	temp.p[_XY] = p[_XX]*m.p[_XY]+p[_XY]*m.p[_YY]+p[_XZ]*m.p[_ZY];
	temp.p[_YY] = p[_YX]*m.p[_XY]+p[_YY]*m.p[_YY]+p[_YZ]*m.p[_ZY];
	temp.p[_ZY] = p[_ZX]*m.p[_XY]+p[_ZY]*m.p[_YY]+p[_ZZ]*m.p[_ZY];

	temp.p[_XZ] = p[_XX]*m.p[_XZ]+p[_XY]*m.p[_YZ]+p[_XZ]*m.p[_ZZ];
	temp.p[_YZ] = p[_YX]*m.p[_XZ]+p[_YY]*m.p[_YZ]+p[_YZ]*m.p[_ZZ];
	temp.p[_ZZ] = p[_ZX]*m.p[_XZ]+p[_ZY]*m.p[_YZ]+p[_ZZ]*m.p[_ZZ];

	return temp;
}

Vector Matrix::operator *(Vector &v)	// return MatrixMulVector(this, v)
{
	Vector	temp;

	temp.x = p[_XX] * v.x + p[_XY] * v.y + p[_XZ] * v.z;
	temp.y = p[_YX] * v.x + p[_YY] * v.y + p[_YZ] * v.z;
	temp.z = p[_ZX] * v.x + p[_ZY] * v.y + p[_ZZ] * v.z;

	return temp;
}

Matrix Matrix::operator *(float f)		// return MatrixScale(this, f)
{
	Matrix temp;

	for (int i=0; i<9; i++)
		temp.p[i] = p[i] * f;

	return temp;
}


void Matrix::operator *=(Matrix &m)		// this = MatrixMulMatrix(this, m)
{
	*this = *this * m;
}


void Matrix::operator *=(float f)		// this = MatrixScale(this, f)
{
	*this = *this * f;
}

Matrix Matrix::operator !()				// return MatrixInverse(this)
{
	Matrix temp;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			temp.p[i+j*3] = p[j+i*3];
		
	return temp;
}




void Matrix::Set(	float xx, float xy, float xz,
					float yx, float yy, float yz,
					float zx, float zy, float zz)
{
	p[_XX] = xx; p[_XY] = xy; p[_XZ] = xz;
	p[_YX] = yx; p[_YY] = yy; p[_YZ] = yz;
	p[_ZX] = zx; p[_ZY] = zy; p[_ZZ] = zz;
}

void Matrix::SetAng(float ax, float ay, float az)
{
	float sinx, siny, sinz, cosx, cosy, cosz, syz, cxz, sxcz;

	sinx = sinf(ax); cosx = cosf(ax);
	siny = sinf(ay); cosy = cosf(ay);
	sinz = sinf(az); cosz = cosf(az);
	
	syz = siny*sinz;
	cxz = cosx*cosz;
	sxcz = sinx*cosz;

	p[_XX] = sinx * syz + cxz;
	p[_XY] = cosy * sinz;
	p[_XZ] = sxcz - cosx * syz;
	p[_YX] = sxcz * siny - cosx * sinz;
	p[_YY] = cosy * cosz;
	p[_YZ] = -cxz * siny - sinx * sinz;
	p[_ZX] = -sinx * cosy;
	p[_ZY] = siny;
	p[_ZZ] = cosx * cosy;
}

void Matrix::Identity()
{
	p[_XX] = 1.f; p[_XY] = 0;   p[_XZ] = 0;
	p[_YX] = 0;   p[_YY] = 1.f; p[_YZ] = 0;
	p[_ZX] = 0;   p[_ZY] = 0;   p[_ZZ] = 1.f;
}

void Matrix::Invert()
{
	*this = !(*this);
}
