//////////////////////////////////////////////////////////////////////////////////////////
//	MATRIX4X4.h
//	Class declaration for a 4x4 matrix
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Updated:	19th August 2002	-	Corrected 2nd SetPerspective for n!=1.0f
//				26th September 2002	-	Added nudge to prevent artifacts with infinite far plane
//									-	Improved speed
//				7th November 2002	-	Added Affine Inverse functions
//									-	Changed constructors
//									-	Added special cases for row3 = (0, 0, 0, 1)
//				17th December 2002	-	Converted from radians to degrees for consistency
//										with OpenGL. Should have been done a long time ago...
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <math.h>

class VECTOR3D
{
public:
	//constructors
	VECTOR3D(void)	:	x(0.0f), y(0.0f), z(0.0f)
	{}

	VECTOR3D(float newX, float newY, float newZ)	:	x(newX), y(newY), z(newZ)
	{}

	VECTOR3D(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2))
	{}

	VECTOR3D(const VECTOR3D & rhs)	:	x(rhs.x), y(rhs.y), z(rhs.z)
	{}

	~VECTOR3D() {}	//empty

	void Set(float newX, float newY, float newZ)
	{	x=newX;	y=newY;	z=newZ;	}
	
	//Accessors kept for compatibility
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}

	float GetX() const {return x;}	//public accessor functions
	float GetY() const {return y;}	//inline, const
	float GetZ() const {return z;}

	void LoadZero(void)
	{	x=y=z=0.0f;	}
	void LoadOne(void)
	{	x=y=z=1.0f;	}
	
	//vector algebra
	VECTOR3D CrossProduct(const VECTOR3D & rhs) const
	{	return VECTOR3D(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);	}

	float DotProduct(const VECTOR3D & rhs) const
	{	return x*rhs.x + y*rhs.y + z*rhs.z;	}
	
	void Normalize();
	VECTOR3D GetNormalized() const;
	
	float GetLength() const
	{	return (float)sqrt((x*x)+(y*y)+(z*z));	}
	
	float GetSquaredLength() const
	{	return (x*x)+(y*y)+(z*z);	}

	//rotations
	void RotateX(double angle);
	VECTOR3D GetRotatedX(double angle) const;
	void RotateY(double angle);
	VECTOR3D GetRotatedY(double angle) const;
	void RotateZ(double angle);
	VECTOR3D GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const VECTOR3D & axis);
	VECTOR3D GetRotatedAxis(double angle, const VECTOR3D & axis) const;

	//pack to [0,1] for color
	void PackTo01();
	VECTOR3D GetPackedTo01() const;

	//linear interpolate
	VECTOR3D lerp(const VECTOR3D & v2, float factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

	VECTOR3D QuadraticInterpolate(const VECTOR3D & v2, const VECTOR3D & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}


	//overloaded operators
	//binary operators
	VECTOR3D operator+(const VECTOR3D & rhs) const
	{	return VECTOR3D(x + rhs.x, y + rhs.y, z + rhs.z);	}
	
	VECTOR3D operator-(const VECTOR3D & rhs) const
	{	return VECTOR3D(x - rhs.x, y - rhs.y, z - rhs.z);	}

	VECTOR3D operator*(const float rhs) const
	{	return VECTOR3D(x*rhs, y*rhs, z*rhs);	}
	
	VECTOR3D operator/(const float rhs) const
	{	return (rhs==0.0f) ? VECTOR3D(0.0f, 0.0f, 0.0f) : VECTOR3D(x / rhs, y / rhs, z / rhs);	}

	//multiply by a float, eg 3*v
	friend VECTOR3D operator*(float scaleFactor, const VECTOR3D & rhs);

	//Add, subtract etc, saving the construction of a temporary
	void Add(const VECTOR3D & v2, VECTOR3D & result)
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	void Subtract(const VECTOR3D & v2, VECTOR3D & result)
	{
		result.x=x-v2.x;
		result.y=y-v2.y;
		result.z=z-v2.z;
	}

	bool operator==(const VECTOR3D & rhs) const;
	bool operator!=(const VECTOR3D & rhs) const
	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const VECTOR3D & rhs)
	{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	}

	void operator-=(const VECTOR3D & rhs)
	{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;	}

	void operator*=(const float rhs)
	{	x*=rhs;	y*=rhs;	z*=rhs;	}
	
	void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs;	}
	}


	//unary operators
	VECTOR3D operator-(void) const {return VECTOR3D(-x, -y, -z);}
	VECTOR3D operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	//member variables
	float x;
	float y;
	float z;
};

class VECTOR4D
{
public:
	//constructors
	VECTOR4D(void)	:	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	VECTOR4D(float newX, float newY, float newZ, float newW)	
		:	x(newX), y(newY), z(newZ), w(newW)
	{}

	VECTOR4D(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2)), w(*(rhs+3))
	{}

	VECTOR4D(const VECTOR4D & rhs):	x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
	{}

	//convert v3d to v4d
	VECTOR4D(const VECTOR3D & rhs):	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{}

	~VECTOR4D() {}	//empty

	void Set(float newX, float newY, float newZ, float newW)
	{	x=newX;	y=newY;	z=newZ; w=newW;	}
	
	//accessors kept for compatability
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}
	void SetW(float newW) {w = newW;}
	
	float GetX() const {return x;}	//public accessor functions
	float GetY() const {return y;}	//inline, const
	float GetZ() const {return z;}
	float GetW() const {return w;}

	void LoadZero(void)
	{	x=0.0f; y=0.0f; z=0.0f; w=0.0f;	}

	void LoadOne(void)
	{	x=1.0f; y=1.0f; z=1.0f; w=1.0f;	}

	//vector algebra
	float DotProduct(const VECTOR4D & rhs)
	{	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;	}

	//rotations
	void RotateX(double angle);
	VECTOR4D GetRotatedX(double angle) const;
	void RotateY(double angle);
	VECTOR4D GetRotatedY(double angle) const;
	void RotateZ(double angle);
	VECTOR4D GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const VECTOR3D & axis);
	VECTOR4D GetRotatedAxis(double angle, const VECTOR3D & axis) const;
	
	VECTOR4D lerp(const VECTOR4D & v2, float factor) const
	{	return (*this)*(1.0f-factor)+v2*factor;	}

	VECTOR4D QuadraticInterpolate(const VECTOR4D & v2, const VECTOR4D & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}

	//binary operators
	VECTOR4D operator+(const VECTOR4D & rhs) const
	{	return VECTOR4D(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w);	}

	VECTOR4D operator-(const VECTOR4D & rhs) const
	{	return VECTOR4D(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w);	}

	VECTOR4D operator*(const float rhs) const
	{	return VECTOR4D(x*rhs, y*rhs, z*rhs, w*rhs);	}

	VECTOR4D operator/(const float rhs) const
	{	return rhs==0.0f	?	VECTOR4D(0.0f, 0.0f, 0.0f, 0.0f)
							:	VECTOR4D(x/rhs, y/rhs, z/rhs, w/rhs);	}

	//multiply by a float, eg 3*v
	friend VECTOR4D operator*(float scaleFactor, const VECTOR4D & rhs);

	bool operator==(const VECTOR4D & rhs) const;
	bool operator!=(const VECTOR4D & rhs) const
	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const VECTOR4D & rhs)
	{	x+=rhs.x; y+=rhs.y; z+=rhs.z; w+=rhs.w;	}

	void operator-=(const VECTOR4D & rhs)
	{	x-=rhs.x; y-=rhs.y; z-=rhs.z; w-=rhs.w;	}

	void operator*=(const float rhs)
	{	x*=rhs; y*=rhs; z*=rhs; w*=rhs;	}

	void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs; w/=rhs;	}
	}

	//unary operators
	VECTOR4D operator-(void) const {return VECTOR4D(-x, -y, -z, -w);}
	VECTOR4D operator+(void) const {return (*this);}

	//cast to pointer to float for glVertex4fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	operator VECTOR3D();							//convert v4d to v3d

	//member variables
	float x;
	float y;
	float z;
	float w;
};

class MATRIX4X4
{
public:
	MATRIX4X4()
	{	LoadIdentity();	}
	MATRIX4X4(	float e0, float e1, float e2, float e3,
				float e4, float e5, float e6, float e7,
				float e8, float e9, float e10, float e11,
				float e12, float e13, float e14, float e15);
	MATRIX4X4(const float * rhs);
	MATRIX4X4(const MATRIX4X4 & rhs);
	~MATRIX4X4() {}	//empty

	void SetEntry(int position, float value);
	float GetEntry(int position) const;
	VECTOR4D GetRow(int position) const;
	VECTOR4D GetColumn(int position) const;
	
	void LoadIdentity(void);
	void LoadZero(void);
	
	//binary operators
	MATRIX4X4 operator+(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator-(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator*(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator*(const float rhs) const;
	MATRIX4X4 operator/(const float rhs) const;
	friend MATRIX4X4 operator*(float scaleFactor, const MATRIX4X4 & rhs);

	bool operator==(const MATRIX4X4 & rhs) const;
	bool operator!=(const MATRIX4X4 & rhs) const;

	//self-add etc
	void operator+=(const MATRIX4X4 & rhs);
	void operator-=(const MATRIX4X4 & rhs);
	void operator*=(const MATRIX4X4 & rhs);
	void operator*=(const float rhs);
	void operator/=(const float rhs);

	//unary operators
	MATRIX4X4 operator-(void) const;
	MATRIX4X4 operator+(void) const {return (*this);}
	
	//multiply a vector by this matrix
	VECTOR4D operator*(const VECTOR4D rhs) const;

	//rotate a 3d vector by rotation part
	void RotateVector3D(VECTOR3D & rhs) const
	{rhs=GetRotatedVector3D(rhs);}

	void InverseRotateVector3D(VECTOR3D & rhs) const
	{rhs=GetInverseRotatedVector3D(rhs);}

	VECTOR3D GetRotatedVector3D(const VECTOR3D & rhs) const;
	VECTOR3D GetInverseRotatedVector3D(const VECTOR3D & rhs) const;

	//translate a 3d vector by translation part
	void TranslateVector3D(VECTOR3D & rhs) const
	{rhs=GetTranslatedVector3D(rhs);}

	void InverseTranslateVector3D(VECTOR3D & rhs) const
	{rhs=GetInverseTranslatedVector3D(rhs);}
	
	VECTOR3D GetTranslatedVector3D(const VECTOR3D & rhs) const;
	VECTOR3D GetInverseTranslatedVector3D(const VECTOR3D & rhs) const;

	//Other methods
	void Invert(void);
	MATRIX4X4 GetInverse(void) const;
	void Transpose(void);
	MATRIX4X4 GetTranspose(void) const;
	void InvertTranspose(void);
	MATRIX4X4 GetInverseTranspose(void) const;

	//Inverse of a rotation/translation only matrix
	void AffineInvert(void);
	MATRIX4X4 GetAffineInverse(void) const;
	void AffineInvertTranspose(void);
	MATRIX4X4 GetAffineInverseTranspose(void) const;

	//set to perform an operation on space - removes other entries
	void SetTranslation(const VECTOR3D & translation);
	void SetScale(const VECTOR3D & scaleFactor);
	void SetUniformScale(const float scaleFactor);
	void SetRotationAxis(const double angle, const VECTOR3D & axis);
	void SetRotationX(const double angle);
	void SetRotationY(const double angle);
	void SetRotationZ(const double angle);
	void SetRotationEuler(const double angleX, const double angleY, const double angleZ);
	void SetPerspective(float left, float right, float bottom, float top, float n, float f);
	void SetPerspective(float fovy, float aspect, float n, float f);
	void SetOrtho(float left, float right, float bottom, float top, float n, float f);

	//set parts of the matrix
	void SetTranslationPart(const VECTOR3D & translation);
	void SetRotationPartEuler(const double angleX, const double angleY, const double angleZ);
	void SetRotationPartEuler(const VECTOR3D & rotations)
	{
		SetRotationPartEuler((double)rotations.x, (double)rotations.y, (double)rotations.z);
	}

	//cast to pointer to a (float *) for glGetFloatv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
	
	//member variables
	float entries[16];
};

#endif	//MATRIX4X4_H