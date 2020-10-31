/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_MATH_H
#define B2_MATH_H

#include "b2Settings.h"

#include <cmath>
//#include <cb2decimal>
#include <cstddef>
#include <limits>

/// This function is used to ensure that a b2decimaling point number is
/// not a NaN or infinity.
inline bool b2IsValid(b2decimal x)
{
	if (x != x)
	{
		// NaN.
		return false;
	}

	b2decimal infinity = std::numeric_limits<b2decimal>::infinity();
	return -infinity < x && x < infinity;
}

/// This is a approximate yet fast inverse square-root.
inline b2decimal b2InvSqrt(b2decimal x)
{
	union
	{
		b2decimal x;
		int i;
	} convert;

	convert.x = x;
	b2decimal xhalf = 0.5f * x;
	convert.i = 0x5f3759df - (convert.i >> 1);
	x = convert.x;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

#define	b2Sqrt(x)	sqrt(x)
#define	b2Atan2(y, x)	atan2(y, x)
#define b2Cos(x) cos(x)
#define b2Sin(x) sin(x)
#define b2Floor(x) floor(x)

inline b2decimal b2Abs(b2decimal a)
{
	return a > 0.0 ? a : -a;
}

/// A 2D column vector.
struct b2Vec2 {
	b2decimal x, y;
	/// Default constructor does nothing (for performance).
	b2Vec2() {
  x=y=0.0;
 }

	/// Construct using coordinates.
	b2Vec2(b2decimal x, b2decimal y) : x(x), y(y) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0; y = 0.0; }

	/// Set this vector to some specified coordinates.
	void Set(b2decimal x_, b2decimal y_) { x = x_; y = y_; }

	/// Negate this vector.
	b2Vec2 operator -() const { b2Vec2 v; v.Set(-x, -y); return v; }
	
	/// Read from and indexed element.
	b2decimal operator () (int i) const
	{
		return (&x)[i];
	}

	/// Write to an indexed element.
	b2decimal& operator () (int i)
	{
		return (&x)[i];
	}

	/// Add a vector to this vector.
	void operator += (const b2Vec2& v)
	{
		x += v.x; y += v.y;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const b2Vec2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (b2decimal a)
	{
		x *= a; y *= a;
	}

	/// Get the length of this vector (the norm).
	b2decimal Length() const
	{
		return b2Sqrt(x * x + y * y);
	}

	/// Get the length squared. For performance, use this instead of
	/// b2Vec2::Length (if possible).
	b2decimal LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Convert this vector into a unit vector. Returns the length.
	b2decimal Normalize()
	{
		b2decimal length = Length();
		if (length < b2_epsilon)
		{
			return 0.0;
		}
		b2decimal invLength = 1.0 / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		return b2IsValid(x) && b2IsValid(y);
	}
};

/// A 2D column vector with 3 elements.
struct b2Vec3
{
	/// Default constructor does nothing (for performance).
	b2Vec3() {}

	/// Construct using coordinates.
	b2Vec3(b2decimal x, b2decimal y, b2decimal z) : x(x), y(y), z(z) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0; y = 0.0; z = 0.0; }

	/// Set this vector to some specified coordinates.
	void Set(b2decimal x_, b2decimal y_, b2decimal z_) { x = x_; y = y_; z = z_; }

	/// Negate this vector.
	b2Vec3 operator -() const { b2Vec3 v; v.Set(-x, -y, -z); return v; }

	/// Add a vector to this vector.
	void operator += (const b2Vec3& v)
	{
		x += v.x; y += v.y; z += v.z;
	}

	/// Subtract a vector from this vector.
	void operator -= (const b2Vec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	/// Multiply this vector by a scalar.
	void operator *= (b2decimal s)
	{
		x *= s; y *= s; z *= s;
	}

	b2decimal x, y, z;
};

/// A 2-by-2 matrix. Stored in column-major order.
struct b2Mat22
{
	/// The default constructor does nothing (for performance).
	b2Mat22() {}

	/// Construct this matrix using columns.
	b2Mat22(const b2Vec2& c1, const b2Vec2& c2)
	{
		col1 = c1;
		col2 = c2;
	}

	/// Construct this matrix using scalars.
	b2Mat22(b2decimal a11, b2decimal a12, b2decimal a21, b2decimal a22)
	{
		col1.x = a11; col1.y = a21;
		col2.x = a12; col2.y = a22;
	}

	/// Construct this matrix using an angle. This matrix becomes
	/// an orthonormal rotation matrix.
	explicit b2Mat22(b2decimal angle)
	{
		// TODO_ERIN compute sin+cos together.
		b2decimal c = b2Cos(angle), s = b2Sin(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	/// Initialize this matrix using columns.
	void Set(const b2Vec2& c1, const b2Vec2& c2)
	{
		col1 = c1;
		col2 = c2;
	}

	/// Initialize this matrix using an angle. This matrix becomes
	/// an orthonormal rotation matrix.
	void Set(b2decimal angle)
	{
		b2decimal c = b2Cos(angle), s = b2Sin(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	/// Set this to the identity matrix.
	void SetIdentity()
	{
		col1.x = 1.0; col2.x = 0.0;
		col1.y = 0.0; col2.y = 1.0;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		col1.x = 0.0; col2.x = 0.0;
		col1.y = 0.0; col2.y = 0.0;
	}

	/// Extract the angle from this matrix (assumed to be
	/// a rotation matrix).
	b2decimal GetAngle() const
	{
		return b2Atan2(col1.y, col1.x);
	}

	b2Mat22 GetInverse() const
	{
		b2decimal a = col1.x, b = col2.x, c = col1.y, d = col2.y;
		b2Mat22 B;
		b2decimal det = a * d - b * c;
		if (det != 0.0)
		{
			det = 1.0 / det;
		}
		B.col1.x =  det * d;	B.col2.x = -det * b;
		B.col1.y = -det * c;	B.col2.y =  det * a;
		return B;
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	b2Vec2 Solve(const b2Vec2& b) const
	{
		b2decimal a11 = col1.x, a12 = col2.x, a21 = col1.y, a22 = col2.y;
		b2decimal det = a11 * a22 - a12 * a21;
		if (det != 0.0)
		{
			det = 1.0 / det;
		}
		b2Vec2 x;
		x.x = det * (a22 * b.x - a12 * b.y);
		x.y = det * (a11 * b.y - a21 * b.x);
		return x;
	}

	b2Vec2 col1, col2;
};

/// A 3-by-3 matrix. Stored in column-major order.
struct b2Mat33 {
	b2Vec3 col1, col2, col3;
	/// The default constructor does nothing (for performance).
	b2Mat33() {}

	/// Construct this matrix using columns.
	b2Mat33(const b2Vec3& c1, const b2Vec3& c2, const b2Vec3& c3)
	{
		col1 = c1;
		col2 = c2;
		col3 = c3;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		col1.SetZero();
		col2.SetZero();
		col3.SetZero();
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	b2Vec3 Solve33(const b2Vec3& b) const;

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases. Solve only the upper
	/// 2-by-2 matrix equation.
	b2Vec2 Solve22(const b2Vec2& b) const;
};

/// A transform contains translation and rotation. It is used to represent
/// the position and orientation of rigid frames.
struct b2Transform
{
	/// The default constructor does nothing (for performance).
	b2Transform() {}

	/// Initialize using a position vector and a rotation matrix.
	b2Transform(const b2Vec2& position, const b2Mat22& R) : position(position), R(R) {}

	/// Set this to the identity transform.
	void SetIdentity()
	{
		position.SetZero();
		R.SetIdentity();
	}

	/// Set this based on the position and angle.
	void Set(const b2Vec2& p, b2decimal angle)
	{
		position = p;
		R.Set(angle);
	}

	/// Calculate the angle that the rotation matrix represents.
	b2decimal GetAngle() const
	{
		return b2Atan2(R.col1.y, R.col1.x);
	}

	b2Vec2 position;
	b2Mat22 R;
};

/// This describes the motion of a body/shape for TOI computation.
/// Shapes are defined with respect to the body origin, which may
/// no coincide with the center of mass. However, to support dynamics
/// we must interpolate the center of mass position.
struct b2Sweep
{
	/// Get the interpolated transform at a specific time.
	/// @param alpha is a factor in [0,1], where 0 indicates t0.
	void GetTransform(b2Transform* xf, b2decimal alpha) const;

	/// Advance the sweep forward, yielding a new initial state.
	/// @param t the new initial time.
	void Advance(b2decimal t);

	/// Normalize the angles.
	void Normalize();

	b2Vec2 localCenter;	///< local center of mass position
	b2Vec2 c0, c;		///< center world positions
	b2decimal a0, a;		///< world angles
};


extern const b2Vec2 b2Vec2_zero;
extern const b2Mat22 b2Mat22_identity;
extern const b2Transform b2Transform_identity;

/// Perform the dot product on two vectors.
inline b2decimal b2Dot(const b2Vec2& a, const b2Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline b2decimal b2Cross(const b2Vec2& a, const b2Vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a vector and a scalar. In 2D this produces
/// a vector.
inline b2Vec2 b2Cross(const b2Vec2& a, b2decimal s)
{
	return b2Vec2(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a vector. In 2D this produces
/// a vector.
inline b2Vec2 b2Cross(b2decimal s, const b2Vec2& a)
{
	return b2Vec2(-s * a.y, s * a.x);
}

/// Multiply a matrix times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another.
inline b2Vec2 b2Mul(const b2Mat22& A, const b2Vec2& v)
{
	return b2Vec2(A.col1.x * v.x + A.col2.x * v.y, A.col1.y * v.x + A.col2.y * v.y);
}

/// Multiply a matrix transpose times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another (inverse transform).
inline b2Vec2 b2MulT(const b2Mat22& A, const b2Vec2& v)
{
	return b2Vec2(b2Dot(v, A.col1), b2Dot(v, A.col2));
}

/// Add two vectors component-wise.
inline b2Vec2 operator + (const b2Vec2& a, const b2Vec2& b)
{
	return b2Vec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline b2Vec2 operator - (const b2Vec2& a, const b2Vec2& b)
{
	return b2Vec2(a.x - b.x, a.y - b.y);
}

inline b2Vec2 operator * (b2decimal s, const b2Vec2& a)
{
	return b2Vec2(s * a.x, s * a.y);
}

inline bool operator == (const b2Vec2& a, const b2Vec2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline b2decimal b2Distance(const b2Vec2& a, const b2Vec2& b)
{
	b2Vec2 c = a - b;
	return c.Length();
}

inline b2decimal b2DistanceSquared(const b2Vec2& a, const b2Vec2& b)
{
	b2Vec2 c = a - b;
	return b2Dot(c, c);
}

inline b2Vec3 operator * (b2decimal s, const b2Vec3& a)
{
	return b2Vec3(s * a.x, s * a.y, s * a.z);
}

/// Add two vectors component-wise.
inline b2Vec3 operator + (const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/// Subtract two vectors component-wise.
inline b2Vec3 operator - (const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/// Perform the dot product on two vectors.
inline b2decimal b2Dot(const b2Vec3& a, const b2Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// Perform the cross product on two vectors.
inline b2Vec3 b2Cross(const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline b2Mat22 operator + (const b2Mat22& A, const b2Mat22& B)
{
	return b2Mat22(A.col1 + B.col1, A.col2 + B.col2);
}

// A * B
inline b2Mat22 b2Mul(const b2Mat22& A, const b2Mat22& B)
{
	return b2Mat22(b2Mul(A, B.col1), b2Mul(A, B.col2));
}

// A^T * B
inline b2Mat22 b2MulT(const b2Mat22& A, const b2Mat22& B)
{
	b2Vec2 c1(b2Dot(A.col1, B.col1), b2Dot(A.col2, B.col1));
	b2Vec2 c2(b2Dot(A.col1, B.col2), b2Dot(A.col2, B.col2));
	return b2Mat22(c1, c2);
}

/// Multiply a matrix times a vector.
inline b2Vec3 b2Mul(const b2Mat33& A, const b2Vec3& v)
{
	return v.x * A.col1 + v.y * A.col2 + v.z * A.col3;
}

inline b2Vec2 b2Mul(const b2Transform& T, const b2Vec2& v)
{
	b2decimal x = T.position.x + T.R.col1.x * v.x + T.R.col2.x * v.y;
	b2decimal y = T.position.y + T.R.col1.y * v.x + T.R.col2.y * v.y;

	return b2Vec2(x, y);
}

inline b2Vec2 b2MulT(const b2Transform& T, const b2Vec2& v)
{
	return b2MulT(T.R, v - T.position);
}

inline b2Vec2 b2Abs(const b2Vec2& a)
{
	return b2Vec2(b2Abs(a.x), b2Abs(a.y));
}

inline b2Mat22 b2Abs(const b2Mat22& A)
{
	return b2Mat22(b2Abs(A.col1), b2Abs(A.col2));
}

template <typename T>
inline T b2Min(T a, T b)
{
	return a < b ? a : b;
}

inline b2Vec2 b2Min(const b2Vec2& a, const b2Vec2& b)
{
	return b2Vec2(b2Min(a.x, b.x), b2Min(a.y, b.y));
}

template <typename T>
inline T b2Max(T a, T b)
{
	return a > b ? a : b;
}

inline b2Vec2 b2Max(const b2Vec2& a, const b2Vec2& b)
{
	return b2Vec2(b2Max(a.x, b.x), b2Max(a.y, b.y));
}

template <typename T>
inline T b2Clamp(T a, T low, T high)
{
	return b2Max(low, b2Min(a, high));
}

inline b2Vec2 b2Clamp(const b2Vec2& a, const b2Vec2& low, const b2Vec2& high)
{
	return b2Max(low, b2Min(a, high));
}

template<typename T> inline void b2Swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

/// "Next Largest Power of 2
/// Given a binary integer value x, the next largest power of 2 can be computed by a SWAR algorithm
/// that recursively "folds" the upper bits into the lower bits. This process yields a bit vector with
/// the same most significant 1 as x, but all 1's below it. Adding 1 to that value yields the next
/// largest power of 2. For a 32-bit value:"
inline uint b2NextPowerOfTwo(uint x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return x + 1;
}

inline bool b2IsPowerOfTwo(uint x)
{
	bool result = x > 0 && (x & (x - 1)) == 0;
	return result;
}

inline void b2Sweep::GetTransform(b2Transform* xf, b2decimal alpha) const
{
	xf->position = (1.0 - alpha) * c0 + alpha * c;
	b2decimal angle = (1.0 - alpha) * a0 + alpha * a;
	xf->R.Set(angle);

	// Shift to origin
	xf->position -= b2Mul(xf->R, localCenter);
}

inline void b2Sweep::Advance(b2decimal t)
{
	c0 = (1.0 - t) * c0 + t * c;
	a0 = (1.0 - t) * a0 + t * a;
}

/// Normalize an angle in radians to be between -pi and pi
inline void b2Sweep::Normalize()
{
	b2decimal twoPi = 2.0 * b2_pi;
	b2decimal d =  twoPi * b2Floor(a0 / twoPi);
	a0 -= d;
	a -= d;
}

#endif
