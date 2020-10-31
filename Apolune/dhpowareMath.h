//-----------------------------------------------------------------------------
// Copyright (c) 2005-2010 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#if !defined(dhpowareMATHLIB_H)
#define dhpowareMATHLIB_H

#include <cmath>
#include <cstdlib>
#include <string>
using namespace std;

#include "macros.h"
#include "Strings.h"

//-----------------------------------------------------------------------------
// Classes.

class dhpowareMath
{
public:

    template <typename T>
    static T bilerp(const T &a, const T &b, const T &c, const T &d, float u, float v)
    {
        // Performs a bilinear interpolation.
        //  P(u,v) = e + v(f - e)
        //  
        //  where
        //  e = a + u(b - a)
        //  f = c + u(d - c)
        //  u in range [0,1]
        //  v in range [0,1]

        return a * ((1.0f - u) * (1.0f - v))
            + b * (u * (1.0f - v))
            + c * (v * (1.0f - u))
            + d * (u * v);
    }

    static void cartesianToSpherical(float x, float y, float z,
                                     float &rho, float &phi, float &theta)
    {
        // All angles are in radians.
        // rho = distance from origin O to point P (i.e., length of OP)
        // phi = angle between OP and the XZ plane
        // theta = angle between X-axis and OP projected onto XZ plane

        rho = sqrtf((x * x) + (y * y) + (z * z));
        phi = asinf(y / rho);
        theta = atan2f(z, x);
    }

    static bool closeEnough(float f1, float f2)
    {
        // Determines whether the two floating-point values f1 and f2 are
        // close enough together that they can be considered equal.

        return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
    }

    static float degreesToRadians(float degrees)
    {
        return (degrees * (float) PI) / 180.0f;
    }

    static long floatToLong(float f)
    {
        // Converts a floating point number into an integer.
        // Fractional values are truncated as in ANSI C.
        // About 5 to 6 times faster than a standard typecast to an integer.

        long fpBits = *reinterpret_cast<const long*>(&f);
        long shift = 23 - (((fpBits & 0x7fffffff) >> 23) - 127);
        long result = ((fpBits & 0x7fffff) | (1 << 23)) >> shift;

        result = (result ^ (fpBits >> 31)) - (fpBits >> 31);
        result &= ~((((fpBits & 0x7fffffff) >> 23) - 127) >> 31);

        return result;
    }

    static bool isPower2(int x)
    {
        return ((x > 0) && ((x & (x - 1)) == 0));
    }

    template <typename T>
    static T lerp(const T &a, const T &b, float t)
    {
        // Performs a linear interpolation.
        //  P(t) = (1 - t)a + tb
        //       = a + t(b - a)
        //
        //  where
        //  t in range [0,1]

        return a + (b - a) * t;
    }

    static int nextMultipleOf(int multiple, int value)
    {
        // Returns the closest multiple of value that isn't less than value.

        return multiple * ((value + (multiple - 1)) / multiple);
    }

    static int nextPower2(int x);
    
    static float radiansToDegrees(float radians)
    {
        return (radians * 180.0f) / (float) PI;
    }

    static float random(float min, float max)
    {
        // Returns a random number in range [min,max].

        return min + (max - min)
            * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }

    static float smoothstep(float a, float b, float x)
    {
        // Returns a gradual transition of 'x' from 0 to 1 beginning at
        // threshold 'a' and ending at threshold 'b'.
        //
        // References:
        //  [1] http://www.rendermanacademy.com/docs/smoothstep.htm
        //  [2] http://www.brlcad.org/doxygen/d8/d33/noise_8c-source.html
        //  [3] Natalya Tatarchuk, "Efficient High-Level Shader Development",
        //      Game Developers Conference Europe, August 2003.

        if (x < a)
        {
            return 0.0f;
        }
        else if (x >= b)
        {
            return 1.0f;
        }
        else
        {
            x = (x - a) / (b - a);
            return x * x * (3.0f - 2.0f * x);
        }
    }

    static void sphericalToCartesian(float rho, float phi, float theta,
                                     float &x, float &y, float &z)
    {
        // All angles are in radians.
        // rho = distance from origin O to point P (i.e., length of OP)
        // phi = angle between OP and the XZ plane
        // theta = angle between X-axis and OP projected onto XZ plane

        x = rho * cosf(phi) * cosf(theta);
        y = rho * sinf(phi);
        z = rho * cosf(phi) * sinf(theta);
    }
};

//-----------------------------------------------------------------------------
// A 2-component vector class that represents a row vector.

class Vector2
{
    friend Vector2 operator*(float lhs, const Vector2 &rhs);
    friend Vector2 operator-(const Vector2 &v);

public:
    float x, y;

    static float distance(const Vector2 &pt1, const Vector2 &pt2);
    static float distanceSq(const Vector2 &pt1, const Vector2 &pt2);  
    static float dot(const Vector2 &p, const Vector2 &q);
    static Vector2 lerp(const Vector2 &p, const Vector2 &q, float t);
    static void orthogonalize(Vector2 &v1, Vector2 &v2);
    static Vector2 proj(const Vector2 &p, const Vector2 &q);
    static Vector2 perp(const Vector2 &p, const Vector2 &q);
    static Vector2 reflect(const Vector2 &i, const Vector2 &n);

    Vector2() { x=y=0.0f; }
    Vector2(float x_, float y_);
    ~Vector2() {}

    bool operator==(const Vector2 &rhs) const;
    bool operator!=(const Vector2 &rhs) const;

    Vector2 &operator+=(const Vector2 &rhs);
    Vector2 &operator-=(const Vector2 &rhs);
    Vector2 &operator*=(float scalar);
    Vector2 &operator/=(float scalar);

    Vector2 operator+(const Vector2 &rhs) const;
    Vector2 operator-(const Vector2 &rhs) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    float magnitude() const;
    float magnitudeSq() const;
    Vector2 inverse() const;
    void normalize();
    void set(float x_, float y_);
};

inline Vector2 operator*(float lhs, const Vector2 &rhs)
{
    return Vector2(lhs * rhs.x, lhs * rhs.y);
}

inline Vector2 operator-(const Vector2 &v)
{
    return Vector2(-v.x, -v.y);
}

inline float Vector2::distance(const Vector2 &pt1, const Vector2 &pt2)
{
    // Calculates the distance between 2 points.
    return sqrtf(distanceSq(pt1, pt2));
}

inline float Vector2::distanceSq(const Vector2 &pt1, const Vector2 &pt2)
{
    // Calculates the squared distance between 2 points.
    return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
        + ((pt1.y - pt2.y) * (pt1.y - pt2.y));
}

inline float Vector2::dot(const Vector2 &p, const Vector2 &q)
{
    return (p.x * q.x) + (p.y * q.y);
}

inline Vector2 Vector2::lerp(const Vector2 &p, const Vector2 &q, float t)
{
    // Linearly interpolates from 'p' to 'q' as t varies from 0 to 1.
    return p + t * (q - p);
}

inline void Vector2::orthogonalize(Vector2 &v1, Vector2 &v2)
{
    // Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
    // turn them into orthonormal basis vectors.
    v2 = v2 - proj(v2, v1);
    v2.normalize();
}

inline Vector2 Vector2::proj(const Vector2 &p, const Vector2 &q)
{
    // Calculates the projection of 'p' onto 'q'.
    float length =  q.magnitude();
    return (Vector2::dot(p, q) / (length * length)) * q;
}

inline Vector2 Vector2::perp(const Vector2 &p, const Vector2 &q)
{
    // Calculates the component of 'p' perpendicular to 'q'.
    float length = q.magnitude();
    return p - ((Vector2::dot(p, q) / (length * length)) * q);
}

inline Vector2 Vector2::reflect(const Vector2 &i, const Vector2 &n)
{
    // Calculates reflection vector from entering ray direction 'i'
    // and surface normal 'n'.
    return i - 2.0f * Vector2::proj(i, n);
}

inline Vector2::Vector2(float x_, float y_) : x(x_), y(y_) {}

inline bool Vector2::operator==(const Vector2 &rhs) const
{
    return dhpowareMath::closeEnough(x, rhs.x) && dhpowareMath::closeEnough(y, rhs.y);
}

inline bool Vector2::operator!=(const Vector2 &rhs) const
{
    return !(*this == rhs);
}

inline Vector2 &Vector2::operator+=(const Vector2 &rhs)
{
    x += rhs.x, y += rhs.y;
    return *this;
}

inline Vector2 &Vector2::operator-=(const Vector2 &rhs)
{
    x -= rhs.x, y -= rhs.y;
    return *this;
}

inline Vector2 &Vector2::operator*=(float scalar)
{
    x *= scalar, y *= scalar;
    return *this;
}

inline Vector2 &Vector2::operator/=(float scalar)
{
    x /= scalar, y /= scalar;
    return *this;
}

inline Vector2 Vector2::operator+(const Vector2 &rhs) const
{
    Vector2 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Vector2 Vector2::operator-(const Vector2 &rhs) const
{
    Vector2 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

inline Vector2 Vector2::operator/(float scalar) const
{
    return Vector2(x / scalar, y / scalar);
}

inline float Vector2::magnitude() const
{
    return sqrtf((x * x) + (y * y));
}

inline float Vector2::magnitudeSq() const
{
    return (x * x) + (y * y);
}

inline Vector2 Vector2::inverse() const
{
    return Vector2(-x, -y);
}

inline void Vector2::normalize()
{
    float invMag = 1.0f / magnitude();
    x *= invMag, y *= invMag;
}

inline void Vector2::set(float x_, float y_)
{
    x = x_, y = y_;
}

//-----------------------------------------------------------------------------
// A 3-component vector class that represents a row vector.

class Vector3
{
    friend Vector3 operator*(float lhs, const Vector3 &rhs);
    friend Vector3 operator-(const Vector3 &v);

public:
    float x, y, z;

    static Vector3 cross(const Vector3 &p, const Vector3 &q);
    static float distance(const Vector3 &pt1, const Vector3 &pt2);
    static float distanceSq(const Vector3 &pt1, const Vector3 &pt2);  
    static float dot(const Vector3 &p, const Vector3 &q);
    static Vector3 lerp(const Vector3 &p, const Vector3 &q, float t);
    static void orthogonalize(Vector3 &v1, Vector3 &v2);
    static void orthogonalize(Vector3 &v1, Vector3 &v2, Vector3 &v3);
    static Vector3 proj(const Vector3 &p, const Vector3 &q);
    static Vector3 perp(const Vector3 &p, const Vector3 &q);
    static Vector3 reflect(const Vector3 &i, const Vector3 &n);

    Vector3() { x=y=z=0.0f; }
    Vector3(float x_, float y_, float z_);
    ~Vector3() {}

    bool operator==(const Vector3 &rhs) const;
    bool operator!=(const Vector3 &rhs) const;

    Vector3 &operator+=(const Vector3 &rhs);
    Vector3 &operator-=(const Vector3 &rhs);
    Vector3 &operator*=(float scalar);
    Vector3 &operator/=(float scalar);

    Vector3 operator+(const Vector3 &rhs) const;
    Vector3 operator-(const Vector3 &rhs) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    float magnitude() const;
    float magnitudeSq() const;
    Vector3 inverse() const;
    void normalize();
    void set(float x_, float y_, float z_);
};

inline Vector3 operator*(float lhs, const Vector3 &rhs)
{
    return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

inline Vector3 operator-(const Vector3 &v)
{
    return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3 Vector3::cross(const Vector3 &p, const Vector3 &q)
{
    return Vector3((p.y * q.z) - (p.z * q.y),
        (p.z * q.x) - (p.x * q.z),
        (p.x * q.y) - (p.y * q.x));
}

inline float Vector3::distance(const Vector3 &pt1, const Vector3 &pt2)
{
    // Calculates the distance between 2 points.
    return sqrtf(distanceSq(pt1, pt2));
}

inline float Vector3::distanceSq(const Vector3 &pt1, const Vector3 &pt2)
{
    // Calculates the squared distance between 2 points.
    return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
        + ((pt1.y - pt2.y) * (pt1.y - pt2.y))
        + ((pt1.z - pt2.z) * (pt1.z - pt2.z));
}

inline float Vector3::dot(const Vector3 &p, const Vector3 &q)
{
    return (p.x * q.x) + (p.y * q.y) + (p.z * q.z);
}

inline Vector3 Vector3::lerp(const Vector3 &p, const Vector3 &q, float t)
{
    // Linearly interpolates from 'p' to 'q' as t varies from 0 to 1.
    return p + t * (q - p);
}

inline void Vector3::orthogonalize(Vector3 &v1, Vector3 &v2)
{
    // Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
    // turn them into orthonormal basis vectors.
    v2 = v2 - proj(v2, v1);
    v2.normalize();
}

inline void Vector3::orthogonalize(Vector3 &v1, Vector3 &v2, Vector3 &v3)
{
    // Performs Gram-Schmidt Orthogonalization on the 3 basis vectors to
    // turn them into orthonormal basis vectors.

    v2 = v2 - proj(v2, v1);
    v2.normalize();

    v3 = v3 - proj(v3, v1) - proj(v3, v2);
    v3.normalize();
}

inline Vector3 Vector3::proj(const Vector3 &p, const Vector3 &q)
{
    // Calculates the projection of 'p' onto 'q'.
    float length =  q.magnitude();
    return (Vector3::dot(p, q) / (length * length)) * q;
}

inline Vector3 Vector3::perp(const Vector3 &p, const Vector3 &q)
{
    // Calculates the component of 'p' perpendicular to 'q'.
    float length = q.magnitude();
    return p - ((Vector3::dot(p, q) / (length * length)) * q);
}

inline Vector3 Vector3::reflect(const Vector3 &i, const Vector3 &n)
{
    // Calculates reflection vector from entering ray direction 'i'
    // and surface normal 'n'.
    return i - 2.0f * Vector3::proj(i, n);
}

inline Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

inline Vector3 &Vector3::operator+=(const Vector3 &rhs)
{
    x += rhs.x, y += rhs.y, z += rhs.z;
    return *this;
}

inline bool Vector3::operator==(const Vector3 &rhs) const
{
    return dhpowareMath::closeEnough(x, rhs.x) && dhpowareMath::closeEnough(y, rhs.y)
        && dhpowareMath::closeEnough(z, rhs.z);
}

inline bool Vector3::operator!=(const Vector3 &rhs) const
{
    return !(*this == rhs);
}

inline Vector3 &Vector3::operator-=(const Vector3 &rhs)
{
    x -= rhs.x, y -= rhs.y, z -= rhs.z;
    return *this;
}

inline Vector3 &Vector3::operator*=(float scalar)
{
    x *= scalar, y *= scalar, z *= scalar;
    return *this;
}

inline Vector3 &Vector3::operator/=(float scalar)
{
    x /= scalar, y /= scalar, z /= scalar;
    return *this;
}

inline Vector3 Vector3::operator+(const Vector3 &rhs) const
{
    Vector3 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Vector3 Vector3::operator-(const Vector3 &rhs) const
{
    Vector3 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);    
}

inline Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

inline float Vector3::magnitude() const
{
    return sqrtf((x * x) + (y * y) + (z * z));
}

inline float Vector3::magnitudeSq() const
{
    return (x * x) + (y * y) + (z * z);
}

inline Vector3 Vector3::inverse() const
{
    return Vector3(-x, -y, -z);
}

inline void Vector3::normalize()
{
    float invMag = 1.0f / magnitude();
    x *= invMag, y *= invMag, z *= invMag;
}

inline void Vector3::set(float x_, float y_, float z_)
{
    x = x_, y = y_, z = z_;
}

//-----------------------------------------------------------------------------
// A 4-component row vector class that represents a point or vector in 
// homogeneous coordinates.

class Vector4
{
    friend Vector4 operator*(float lhs, const Vector4 &rhs);
    friend Vector4 operator-(const Vector4 &v);

public:
    float x, y, z, w;

    static float distance(const Vector4 &pt1, const Vector4 &pt2);
    static float distanceSq(const Vector4 &pt1, const Vector4 &pt2);
    static float dot(const Vector4 &p, const Vector4 &q);
    static Vector4 lerp(const Vector4 &p, const Vector4 &q, float t);

    Vector4() { x=y=z=w=0.0f; }
    Vector4(float x_, float y_, float z_, float w_);
    Vector4(const Vector3 &v, float w_);
    ~Vector4() {}

    bool operator==(const Vector4 &rhs) const;
    bool operator!=(const Vector4 &rhs) const;

    Vector4 &operator+=(const Vector4 &rhs);
    Vector4 &operator-=(const Vector4 &rhs);
    Vector4 &operator*=(float scalar);
    Vector4 &operator/=(float scalar);

    Vector4 operator+(const Vector4 &rhs) const;
    Vector4 operator-(const Vector4 &rhs) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;

    float magnitude() const;
    float magnitudeSq() const;
    Vector4 inverse() const;
    void normalize();
    void set(float x_, float y_, float z_, float w_);
    Vector3 toVector3() const;
};

inline Vector4 operator*(float lhs, const Vector4 &rhs)
{
    return Vector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

inline Vector4 operator-(const Vector4 &v)
{
    return Vector4(-v.x, -v.y, -v.z, -v.w);
}

inline float Vector4::distance(const Vector4 &pt1, const Vector4 &pt2)
{
    // Calculates the distance between 2 points.
    return sqrtf(distanceSq(pt1, pt2));
}

inline float Vector4::distanceSq(const Vector4 &pt1, const Vector4 &pt2)
{
    // Calculates the squared distance between 2 points.
    return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
        + ((pt1.y - pt2.y) * (pt1.y - pt2.y))
        + ((pt1.z - pt2.z) * (pt1.z - pt2.z))
        + ((pt1.w - pt2.w) * (pt1.w - pt2.w));
}

inline float Vector4::dot(const Vector4 &p, const Vector4 &q)
{
    return (p.x * q.x) + (p.y * q.y) + (p.z * q.z) + (p.w * q.w);
}

inline Vector4 Vector4::lerp(const Vector4 &p, const Vector4 &q, float t)
{
    // Linearly interpolates from 'p' to 'q' as t varies from 0 to 1.
    return p + t * (q - p);
}

inline Vector4::Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

inline Vector4::Vector4(const Vector3 &v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}

inline Vector4 &Vector4::operator+=(const Vector4 &rhs)
{
    x += rhs.x, y += rhs.y, z += rhs.z, w += rhs.w;
    return *this;
}

inline bool Vector4::operator==(const Vector4 &rhs) const
{
    return dhpowareMath::closeEnough(x, rhs.x) && dhpowareMath::closeEnough(y, rhs.y)
        && dhpowareMath::closeEnough(z, rhs.z) && dhpowareMath::closeEnough(w, rhs.w);
}

inline bool Vector4::operator!=(const Vector4 &rhs) const
{
    return !(*this == rhs);
}

inline Vector4 &Vector4::operator-=(const Vector4 &rhs)
{
    x -= rhs.x, y -= rhs.y, z -= rhs.z, w -= rhs.w;
    return *this;
}

inline Vector4 &Vector4::operator*=(float scalar)
{
    x *= scalar, y *= scalar, z *= scalar, w *= scalar;
    return *this;
}

inline Vector4 &Vector4::operator/=(float scalar)
{
    x /= scalar, y /= scalar, z /= scalar, w /= scalar;
    return *this;
}

inline Vector4 Vector4::operator+(const Vector4 &rhs) const
{
    Vector4 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Vector4 Vector4::operator-(const Vector4 &rhs) const
{
    Vector4 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Vector4 Vector4::operator*(float scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

inline Vector4 Vector4::operator/(float scalar) const
{
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

inline float Vector4::magnitude() const
{
    return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}

inline float Vector4::magnitudeSq() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

inline Vector4 Vector4::inverse() const
{
    return Vector4(-x, -y, -z, -w);
}

inline void Vector4::normalize()
{
    float invMag = 1.0f / magnitude();
    x *= invMag, y *= invMag, z *= invMag, w *= invMag;
}

inline void Vector4::set(float x_, float y_, float z_, float w_)
{
    x = x_, y = y_, z = z_, w = w_;
}

inline Vector3 Vector4::toVector3() const
{
    return (w != 0.0f) ? Vector3(x / w, y / w, z / w) : Vector3(x, y, z);
}

//-----------------------------------------------------------------------------
// A row-major 3x3 matrix class.
//
// Matrices are concatenated in a left to right order.
// Multiplies vectors to the left of the matrix.

class Matrix3
{
    friend Vector3 operator*(const Vector3 &lhs, const Matrix3 &rhs);
    friend Matrix3 operator*(float scalar, const Matrix3 &rhs);

public:
    static const Matrix3 IDENTITY;
    static Matrix3 createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    static Matrix3 createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    static Matrix3 createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    static Matrix3 createMirror(const Vector3 &planeNormal);
    static Matrix3 createOrient(const Vector3 &from, const Vector3 &to);
    static Matrix3 createRotate(const Vector3 &axis, float degrees);
    static Matrix3 createScale(float sx, float sy, float sz);

    Matrix3() {
     for ( int i=0; i<3; i++ )
      for ( int j=0; j<3; j++ ) mtx[i][j]=0.0f;
    }
    Matrix3(float m11, float m12, float m13,
            float m21, float m22, float m23,
            float m31, float m32, float m33);
    ~Matrix3() {}

    float *operator[](int row);
    const float *operator[](int row) const;

    bool operator==(const Matrix3 &rhs) const;
    bool operator!=(const Matrix3 &rhs) const;

    Matrix3 &operator+=(const Matrix3 &rhs);
    Matrix3 &operator-=(const Matrix3 &rhs);
    Matrix3 &operator*=(const Matrix3 &rhs);
    Matrix3 &operator*=(float scalar);
    Matrix3 &operator/=(float scalar);

    Matrix3 operator+(const Matrix3 &rhs) const;
    Matrix3 operator-(const Matrix3 &rhs) const;
    Matrix3 operator*(const Matrix3 &rhs) const;
    Matrix3 operator*(float scalar) const;
    Matrix3 operator/(float scalar) const;

    float determinant() const;
    void fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    void fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    void identity();
    Matrix3 inverse() const;
    void orient(const Vector3 &from, const Vector3 &to);
    void rotate(const Vector3 &axis, float degrees);
    void scale(float sx, float sy, float sz);
    void toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const;
    void toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const;
    void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
    Matrix3 transpose() const;

private:
    float mtx[3][3];
};

inline Vector3 operator*(const Vector3 &lhs, const Matrix3 &rhs)
{
    return Vector3(
        (lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]),
        (lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]),
        (lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]));
}

inline Matrix3 operator*(float scalar, const Matrix3 &rhs)
{
    return rhs * scalar;
}

inline Matrix3 Matrix3::createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    Matrix3 tmp;
    tmp.createFromAxes(x, y, z);
    return tmp;
}

inline Matrix3 Matrix3::createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    Matrix3 tmp;
    tmp.fromAxesTransposed(x, y, z);
    return tmp;
}

inline Matrix3 Matrix3::createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    Matrix3 tmp;
    tmp.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
    return tmp;
}

inline Matrix3 Matrix3::createOrient(const Vector3 &from, const Vector3 &to)
{
    Matrix3 tmp;
    tmp.orient(from, to);
    return tmp;
}

inline Matrix3 Matrix3::createRotate(const Vector3 &axis, float degrees)
{
    Matrix3 tmp;
    tmp.rotate(axis, degrees);
    return tmp;
}

inline Matrix3 Matrix3::createScale(float sx, float sy, float sz)
{
    Matrix3 tmp;
    tmp.scale(sx, sy, sz);
    return tmp;
}

inline Matrix3::Matrix3(float m11, float m12, float m13,
                        float m21, float m22, float m23,
                        float m31, float m32, float m33)
{
    mtx[0][0] = m11, mtx[0][1] = m12, mtx[0][2] = m13;
    mtx[1][0] = m21, mtx[1][1] = m22, mtx[1][2] = m23;
    mtx[2][0] = m31, mtx[2][1] = m32, mtx[2][2] = m33;
}

inline float *Matrix3::operator[](int row)
{
    return mtx[row];
}

inline const float *Matrix3::operator[](int row) const
{
    return mtx[row];
}

inline bool Matrix3::operator==(const Matrix3 &rhs) const
{
    return dhpowareMath::closeEnough(mtx[0][0], rhs.mtx[0][0])
        && dhpowareMath::closeEnough(mtx[0][1], rhs.mtx[0][1])
        && dhpowareMath::closeEnough(mtx[0][2], rhs.mtx[0][2])
        && dhpowareMath::closeEnough(mtx[1][0], rhs.mtx[1][0])
        && dhpowareMath::closeEnough(mtx[1][1], rhs.mtx[1][1])
        && dhpowareMath::closeEnough(mtx[1][2], rhs.mtx[1][2])
        && dhpowareMath::closeEnough(mtx[2][0], rhs.mtx[2][0])
        && dhpowareMath::closeEnough(mtx[2][1], rhs.mtx[2][1])
        && dhpowareMath::closeEnough(mtx[2][2], rhs.mtx[2][2]);
}

inline bool Matrix3::operator!=(const Matrix3 &rhs) const
{
    return !(*this == rhs);
}

inline Matrix3 &Matrix3::operator+=(const Matrix3 &rhs)
{
    mtx[0][0] += rhs.mtx[0][0], mtx[0][1] += rhs.mtx[0][1], mtx[0][2] += rhs.mtx[0][2];
    mtx[1][0] += rhs.mtx[1][0], mtx[1][1] += rhs.mtx[1][1], mtx[1][2] += rhs.mtx[1][2];
    mtx[2][0] += rhs.mtx[2][0], mtx[2][1] += rhs.mtx[2][1], mtx[2][2] += rhs.mtx[2][2];
    return *this;
}

inline Matrix3 &Matrix3::operator-=(const Matrix3 &rhs)
{
    mtx[0][0] -= rhs.mtx[0][0], mtx[0][1] -= rhs.mtx[0][1], mtx[0][2] -= rhs.mtx[0][2];
    mtx[1][0] -= rhs.mtx[1][0], mtx[1][1] -= rhs.mtx[1][1], mtx[1][2] -= rhs.mtx[1][2];
    mtx[2][0] -= rhs.mtx[2][0], mtx[2][1] -= rhs.mtx[2][1], mtx[2][2] -= rhs.mtx[2][2];
    return *this;
}

inline Matrix3 &Matrix3::operator*=(const Matrix3 &rhs)
{
    Matrix3 tmp;

    // Row 1.
    tmp.mtx[0][0] = (mtx[0][0] * rhs.mtx[0][0]) + (mtx[0][1] * rhs.mtx[1][0]) + (mtx[0][2] * rhs.mtx[2][0]);
    tmp.mtx[0][1] = (mtx[0][0] * rhs.mtx[0][1]) + (mtx[0][1] * rhs.mtx[1][1]) + (mtx[0][2] * rhs.mtx[2][1]);
    tmp.mtx[0][2] = (mtx[0][0] * rhs.mtx[0][2]) + (mtx[0][1] * rhs.mtx[1][2]) + (mtx[0][2] * rhs.mtx[2][2]);

    // Row 2.
    tmp.mtx[1][0] = (mtx[1][0] * rhs.mtx[0][0]) + (mtx[1][1] * rhs.mtx[1][0]) + (mtx[1][2] * rhs.mtx[2][0]);
    tmp.mtx[1][1] = (mtx[1][0] * rhs.mtx[0][1]) + (mtx[1][1] * rhs.mtx[1][1]) + (mtx[1][2] * rhs.mtx[2][1]);
    tmp.mtx[1][2] = (mtx[1][0] * rhs.mtx[0][2]) + (mtx[1][1] * rhs.mtx[1][2]) + (mtx[1][2] * rhs.mtx[2][2]);

    // Row 3.
    tmp.mtx[2][0] = (mtx[2][0] * rhs.mtx[0][0]) + (mtx[2][1] * rhs.mtx[1][0]) + (mtx[2][2] * rhs.mtx[2][0]);
    tmp.mtx[2][1] = (mtx[2][0] * rhs.mtx[0][1]) + (mtx[2][1] * rhs.mtx[1][1]) + (mtx[2][2] * rhs.mtx[2][1]);
    tmp.mtx[2][2] = (mtx[2][0] * rhs.mtx[0][2]) + (mtx[2][1] * rhs.mtx[1][2]) + (mtx[2][2] * rhs.mtx[2][2]);

    *this = tmp;
    return *this;
}

inline Matrix3 &Matrix3::operator*=(float scalar)
{
    mtx[0][0] *= scalar, mtx[0][1] *= scalar, mtx[0][2] *= scalar;
    mtx[1][0] *= scalar, mtx[1][1] *= scalar, mtx[1][2] *= scalar;
    mtx[2][0] *= scalar, mtx[2][1] *= scalar, mtx[2][2] *= scalar;
    return *this;
}

inline Matrix3 &Matrix3::operator/=(float scalar)
{
    mtx[0][0] /= scalar, mtx[0][1] /= scalar, mtx[0][2] /= scalar;
    mtx[1][0] /= scalar, mtx[1][1] /= scalar, mtx[1][2] /= scalar;
    mtx[2][0] /= scalar, mtx[2][1] /= scalar, mtx[2][2] /= scalar;
    return *this;
}

inline Matrix3 Matrix3::operator+(const Matrix3 &rhs) const
{
    Matrix3 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Matrix3 Matrix3::operator-(const Matrix3 &rhs) const
{
    Matrix3 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Matrix3 Matrix3::operator*(const Matrix3 &rhs) const
{
    Matrix3 tmp(*this);
    tmp *= rhs;
    return tmp;
}

inline Matrix3 Matrix3::operator*(float scalar) const
{
    Matrix3 tmp(*this);
    tmp *= scalar;
    return tmp;
}

inline Matrix3 Matrix3::operator/(float scalar) const
{
    Matrix3 tmp(*this);
    tmp /= scalar;
    return tmp;
}

inline float Matrix3::determinant() const
{
    return (mtx[0][0] * (mtx[1][1] * mtx[2][2] - mtx[1][2] * mtx[2][1]))
        - (mtx[0][1] * (mtx[1][0] * mtx[2][2] - mtx[1][2] * mtx[2][0]))
        + (mtx[0][2] * (mtx[1][0] * mtx[2][1] - mtx[1][1] * mtx[2][0]));
}

inline void Matrix3::fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = x.y,  mtx[0][2] = x.z;
    mtx[1][0] = y.x,  mtx[1][1] = y.y,  mtx[1][2] = y.z;
    mtx[2][0] = z.x,  mtx[2][1] = z.y,  mtx[2][2] = z.z;
}

inline void Matrix3::fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = y.x,  mtx[0][2] = z.x;
    mtx[1][0] = x.y,  mtx[1][1] = y.y,  mtx[1][2] = z.y;
    mtx[2][0] = x.z,  mtx[2][1] = y.z,  mtx[2][2] = z.z;
}

inline void Matrix3::identity()
{
    mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f;
}

inline void Matrix3::toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const
{
    x.set(mtx[0][0], mtx[0][1], mtx[0][2]);
    y.set(mtx[1][0], mtx[1][1], mtx[1][2]);
    z.set(mtx[2][0], mtx[2][1], mtx[2][2]);
}

inline void Matrix3::toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const
{
    x.set(mtx[0][0], mtx[1][0], mtx[2][0]);
    y.set(mtx[0][1], mtx[1][1], mtx[2][1]);
    z.set(mtx[0][2], mtx[1][2], mtx[2][2]);
}

inline Matrix3 Matrix3::transpose() const
{
    Matrix3 tmp;

    tmp[0][0] = mtx[0][0], tmp[0][1] = mtx[1][0], tmp[0][2] = mtx[2][0];
    tmp[1][0] = mtx[0][1], tmp[1][1] = mtx[1][1], tmp[1][2] = mtx[2][1];
    tmp[2][0] = mtx[0][2], tmp[2][1] = mtx[1][2], tmp[2][2] = mtx[2][2];

    return tmp;
}

//-----------------------------------------------------------------------------
// A homogeneous row-major 4x4 matrix class.
//
// Matrices are concatenated in a left to right order.
// Multiplies vectors to the left of the matrix.

class Matrix4
{
    friend Vector4 operator*(const Vector4 &lhs, const Matrix4 &rhs);
    friend Vector3 operator*(const Vector3 &lhs, const Matrix4 &rhs);
    friend Matrix4 operator*(float scalar, const Matrix4 &rhs);

public:
    static const Matrix4 IDENTITY;
    static Matrix4 createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    static Matrix4 createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    static Matrix4 createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    static Matrix4 createMirror(const Vector3 &planeNormal, const Vector3 &pointOnPlane);
    static Matrix4 createOrient(const Vector3 &from, const Vector3 &to);
    static Matrix4 createRotate(const Vector3 &axis, float degrees);
    static Matrix4 createScale(float sx, float sy, float sz);
    static Matrix4 createTranslate(float tx, float ty, float tz);

    Matrix4() {
     for ( int i=0; i<4; i++ )
      for ( int j=0; j<4; j++ ) mtx[i][j]=0.0f;
    }
    Matrix4(float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44);
    ~Matrix4() {}

    float *operator[](int row);
    const float *operator[](int row) const;

    bool operator==(const Matrix4 &rhs) const;
    bool operator!=(const Matrix4 &rhs) const;

    Matrix4 &operator+=(const Matrix4 &rhs);
    Matrix4 &operator-=(const Matrix4 &rhs);
    Matrix4 &operator*=(const Matrix4 &rhs);
    Matrix4 &operator*=(float scalar);
    Matrix4 &operator/=(float scalar);

    Matrix4 operator+(const Matrix4 &rhs) const;
    Matrix4 operator-(const Matrix4 &rhs) const;
    Matrix4 operator*(const Matrix4 &rhs) const;
    Matrix4 operator*(float scalar) const;
    Matrix4 operator/(float scalar) const;

    float determinant() const;
    void fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    void fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
    void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    void identity();
    Matrix4 inverse() const;
    void orient(const Vector3 &from, const Vector3 &to);
    void rotate(const Vector3 &axis, float degrees);
    void scale(float sx, float sy, float sz);
    void toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const;
    void toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const;
    void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
    void translate(float tx, float ty, float tz);
    Matrix4 transpose() const;
  
    string toString( const char *name ) {
     static char buf[8192];
     FORMAT( buf,8192, "%s:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
      name,
      (float) mtx[0][0], (float) mtx[0][1], (float) mtx[0][2], (float) mtx[0][3],
      (float) mtx[1][0], (float) mtx[1][1], (float) mtx[1][2], (float) mtx[1][3],
      (float) mtx[2][0], (float) mtx[2][1], (float) mtx[2][2], (float) mtx[2][3],
      (float) mtx[3][0], (float) mtx[3][1], (float) mtx[3][2], (float) mtx[3][3]
      );   
     return string(buf);
    }

//private:
    float mtx[4][4];
};

inline Vector4 operator*(const Vector4 &lhs, const Matrix4 &rhs)
{
    return Vector4(
        (lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]) + (lhs.w * rhs.mtx[3][0]),
        (lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]) + (lhs.w * rhs.mtx[3][1]),
        (lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]) + (lhs.w * rhs.mtx[3][2]),
        (lhs.x * rhs.mtx[0][3]) + (lhs.y * rhs.mtx[1][3]) + (lhs.z * rhs.mtx[2][3]) + (lhs.w * rhs.mtx[3][3]));
}

inline Vector3 operator*(const Vector3 &lhs, const Matrix4 &rhs)
{
    return Vector3(
        (lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]),
        (lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]),
        (lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]));
}

inline Matrix4 operator*(float scalar, const Matrix4 &rhs)
{
    return rhs * scalar;
}

inline Matrix4 Matrix4::createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    Matrix4 tmp;
    tmp.fromAxes(x, y, z);
    return tmp;
}

inline Matrix4 Matrix4::createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    Matrix4 tmp;
    tmp.fromAxesTransposed(x, y, z);
    return tmp;
}

inline Matrix4 Matrix4::createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    Matrix4 tmp;
    tmp.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
    return tmp;
}

inline Matrix4 Matrix4::createOrient(const Vector3 &from, const Vector3 &to)
{
    Matrix4 tmp;
    tmp.orient(from, to);
    return tmp;
}

inline Matrix4 Matrix4::createRotate(const Vector3 &axis, float degrees)
{
    Matrix4 tmp;
    tmp.rotate(axis, degrees);
    return tmp;
}

inline Matrix4 Matrix4::createScale(float sx, float sy, float sz)
{
    Matrix4 tmp;
    tmp.scale(sx, sy, sz);
    return tmp;
}

inline Matrix4 Matrix4::createTranslate(float tx, float ty, float tz)
{
    Matrix4 tmp;
    tmp.translate(tx, ty, tz);
    return tmp;
}

inline Matrix4::Matrix4(float m11, float m12, float m13, float m14,
                      float m21, float m22, float m23, float m24,
                      float m31, float m32, float m33, float m34,
                      float m41, float m42, float m43, float m44)
{
    mtx[0][0] = m11, mtx[0][1] = m12, mtx[0][2] = m13, mtx[0][3] = m14;
    mtx[1][0] = m21, mtx[1][1] = m22, mtx[1][2] = m23, mtx[1][3] = m24;
    mtx[2][0] = m31, mtx[2][1] = m32, mtx[2][2] = m33, mtx[2][3] = m34;
    mtx[3][0] = m41, mtx[3][1] = m42, mtx[3][2] = m43, mtx[3][3] = m44;
}

inline float *Matrix4::operator[](int row)
{
    return mtx[row];
}

inline const float *Matrix4::operator[](int row) const
{
    return mtx[row];
}

inline bool Matrix4::operator==(const Matrix4 &rhs) const
{
    return dhpowareMath::closeEnough(mtx[0][0], rhs.mtx[0][0])
        && dhpowareMath::closeEnough(mtx[0][1], rhs.mtx[0][1])
        && dhpowareMath::closeEnough(mtx[0][2], rhs.mtx[0][2])
        && dhpowareMath::closeEnough(mtx[0][3], rhs.mtx[0][3])
        && dhpowareMath::closeEnough(mtx[1][0], rhs.mtx[1][0])
        && dhpowareMath::closeEnough(mtx[1][1], rhs.mtx[1][1])
        && dhpowareMath::closeEnough(mtx[1][2], rhs.mtx[1][2])
        && dhpowareMath::closeEnough(mtx[1][3], rhs.mtx[1][3])
        && dhpowareMath::closeEnough(mtx[2][0], rhs.mtx[2][0])
        && dhpowareMath::closeEnough(mtx[2][1], rhs.mtx[2][1])
        && dhpowareMath::closeEnough(mtx[2][2], rhs.mtx[2][2])
        && dhpowareMath::closeEnough(mtx[2][3], rhs.mtx[2][3])
        && dhpowareMath::closeEnough(mtx[3][0], rhs.mtx[3][0])
        && dhpowareMath::closeEnough(mtx[3][1], rhs.mtx[3][1])
        && dhpowareMath::closeEnough(mtx[3][2], rhs.mtx[3][2])
        && dhpowareMath::closeEnough(mtx[3][3], rhs.mtx[3][3]);
}

inline bool Matrix4::operator!=(const Matrix4 &rhs) const
{
    return !(*this == rhs);
}

inline Matrix4 &Matrix4::operator+=(const Matrix4 &rhs)
{
    mtx[0][0] += rhs.mtx[0][0], mtx[0][1] += rhs.mtx[0][1], mtx[0][2] += rhs.mtx[0][2], mtx[0][3] += rhs.mtx[0][3];
    mtx[1][0] += rhs.mtx[1][0], mtx[1][1] += rhs.mtx[1][1], mtx[1][2] += rhs.mtx[1][2], mtx[1][3] += rhs.mtx[1][3];
    mtx[2][0] += rhs.mtx[2][0], mtx[2][1] += rhs.mtx[2][1], mtx[2][2] += rhs.mtx[2][2], mtx[2][3] += rhs.mtx[2][3];
    mtx[3][0] += rhs.mtx[3][0], mtx[3][1] += rhs.mtx[3][1], mtx[3][2] += rhs.mtx[3][2], mtx[3][3] += rhs.mtx[3][3];
    return *this;
}

inline Matrix4 &Matrix4::operator-=(const Matrix4 &rhs)
{
    mtx[0][0] -= rhs.mtx[0][0], mtx[0][1] -= rhs.mtx[0][1], mtx[0][2] -= rhs.mtx[0][2], mtx[0][3] -= rhs.mtx[0][3];
    mtx[1][0] -= rhs.mtx[1][0], mtx[1][1] -= rhs.mtx[1][1], mtx[1][2] -= rhs.mtx[1][2], mtx[1][3] -= rhs.mtx[1][3];
    mtx[2][0] -= rhs.mtx[2][0], mtx[2][1] -= rhs.mtx[2][1], mtx[2][2] -= rhs.mtx[2][2], mtx[2][3] -= rhs.mtx[2][3];
    mtx[3][0] -= rhs.mtx[3][0], mtx[3][1] -= rhs.mtx[3][1], mtx[3][2] -= rhs.mtx[3][2], mtx[3][3] -= rhs.mtx[3][3];
    return *this;
}

inline Matrix4 &Matrix4::operator*=(const Matrix4 &rhs)
{
    Matrix4 tmp;

    // Row 1.
    tmp.mtx[0][0] = (mtx[0][0] * rhs.mtx[0][0]) + (mtx[0][1] * rhs.mtx[1][0]) + (mtx[0][2] * rhs.mtx[2][0]) + (mtx[0][3] * rhs.mtx[3][0]);
    tmp.mtx[0][1] = (mtx[0][0] * rhs.mtx[0][1]) + (mtx[0][1] * rhs.mtx[1][1]) + (mtx[0][2] * rhs.mtx[2][1]) + (mtx[0][3] * rhs.mtx[3][1]);
    tmp.mtx[0][2] = (mtx[0][0] * rhs.mtx[0][2]) + (mtx[0][1] * rhs.mtx[1][2]) + (mtx[0][2] * rhs.mtx[2][2]) + (mtx[0][3] * rhs.mtx[3][2]);
    tmp.mtx[0][3] = (mtx[0][0] * rhs.mtx[0][3]) + (mtx[0][1] * rhs.mtx[1][3]) + (mtx[0][2] * rhs.mtx[2][3]) + (mtx[0][3] * rhs.mtx[3][3]);

    // Row 2.
    tmp.mtx[1][0] = (mtx[1][0] * rhs.mtx[0][0]) + (mtx[1][1] * rhs.mtx[1][0]) + (mtx[1][2] * rhs.mtx[2][0]) + (mtx[1][3] * rhs.mtx[3][0]);
    tmp.mtx[1][1] = (mtx[1][0] * rhs.mtx[0][1]) + (mtx[1][1] * rhs.mtx[1][1]) + (mtx[1][2] * rhs.mtx[2][1]) + (mtx[1][3] * rhs.mtx[3][1]);
    tmp.mtx[1][2] = (mtx[1][0] * rhs.mtx[0][2]) + (mtx[1][1] * rhs.mtx[1][2]) + (mtx[1][2] * rhs.mtx[2][2]) + (mtx[1][3] * rhs.mtx[3][2]);
    tmp.mtx[1][3] = (mtx[1][0] * rhs.mtx[0][3]) + (mtx[1][1] * rhs.mtx[1][3]) + (mtx[1][2] * rhs.mtx[2][3]) + (mtx[1][3] * rhs.mtx[3][3]);

    // Row 3.
    tmp.mtx[2][0] = (mtx[2][0] * rhs.mtx[0][0]) + (mtx[2][1] * rhs.mtx[1][0]) + (mtx[2][2] * rhs.mtx[2][0]) + (mtx[2][3] * rhs.mtx[3][0]);
    tmp.mtx[2][1] = (mtx[2][0] * rhs.mtx[0][1]) + (mtx[2][1] * rhs.mtx[1][1]) + (mtx[2][2] * rhs.mtx[2][1]) + (mtx[2][3] * rhs.mtx[3][1]);
    tmp.mtx[2][2] = (mtx[2][0] * rhs.mtx[0][2]) + (mtx[2][1] * rhs.mtx[1][2]) + (mtx[2][2] * rhs.mtx[2][2]) + (mtx[2][3] * rhs.mtx[3][2]);
    tmp.mtx[2][3] = (mtx[2][0] * rhs.mtx[0][3]) + (mtx[2][1] * rhs.mtx[1][3]) + (mtx[2][2] * rhs.mtx[2][3]) + (mtx[2][3] * rhs.mtx[3][3]);

    // Row 4.
    tmp.mtx[3][0] = (mtx[3][0] * rhs.mtx[0][0]) + (mtx[3][1] * rhs.mtx[1][0]) + (mtx[3][2] * rhs.mtx[2][0]) + (mtx[3][3] * rhs.mtx[3][0]);
    tmp.mtx[3][1] = (mtx[3][0] * rhs.mtx[0][1]) + (mtx[3][1] * rhs.mtx[1][1]) + (mtx[3][2] * rhs.mtx[2][1]) + (mtx[3][3] * rhs.mtx[3][1]);
    tmp.mtx[3][2] = (mtx[3][0] * rhs.mtx[0][2]) + (mtx[3][1] * rhs.mtx[1][2]) + (mtx[3][2] * rhs.mtx[2][2]) + (mtx[3][3] * rhs.mtx[3][2]);
    tmp.mtx[3][3] = (mtx[3][0] * rhs.mtx[0][3]) + (mtx[3][1] * rhs.mtx[1][3]) + (mtx[3][2] * rhs.mtx[2][3]) + (mtx[3][3] * rhs.mtx[3][3]);

    *this = tmp;
    return *this;
}

inline Matrix4 &Matrix4::operator*=(float scalar)
{
    mtx[0][0] *= scalar, mtx[0][1] *= scalar, mtx[0][2] *= scalar, mtx[0][3] *= scalar;
    mtx[1][0] *= scalar, mtx[1][1] *= scalar, mtx[1][2] *= scalar, mtx[1][3] *= scalar;
    mtx[2][0] *= scalar, mtx[2][1] *= scalar, mtx[2][2] *= scalar, mtx[2][3] *= scalar;
    mtx[3][0] *= scalar, mtx[3][1] *= scalar, mtx[3][2] *= scalar, mtx[3][3] *= scalar;
    return *this;
}

inline Matrix4 &Matrix4::operator/=(float scalar)
{
    mtx[0][0] /= scalar, mtx[0][1] /= scalar, mtx[0][2] /= scalar, mtx[0][3] /= scalar;
    mtx[1][0] /= scalar, mtx[1][1] /= scalar, mtx[1][2] /= scalar, mtx[1][3] /= scalar;
    mtx[2][0] /= scalar, mtx[2][1] /= scalar, mtx[2][2] /= scalar, mtx[2][3] /= scalar;
    mtx[3][0] /= scalar, mtx[3][1] /= scalar, mtx[3][2] /= scalar, mtx[3][3] /= scalar;
    return *this;
}

inline Matrix4 Matrix4::operator+(const Matrix4 &rhs) const
{
    Matrix4 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Matrix4 Matrix4::operator-(const Matrix4 &rhs) const
{
    Matrix4 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
{
    Matrix4 tmp(*this);
    tmp *= rhs;
    return tmp;
}

inline Matrix4 Matrix4::operator*(float scalar) const
{
    Matrix4 tmp(*this);
    tmp *= scalar;
    return tmp;
}

inline Matrix4 Matrix4::operator/(float scalar) const
{
    Matrix4 tmp(*this);
    tmp /= scalar;
    return tmp;
}

inline float Matrix4::determinant() const
{
    return (mtx[0][0] * mtx[1][1] - mtx[1][0] * mtx[0][1])
        * (mtx[2][2] * mtx[3][3] - mtx[3][2] * mtx[2][3])
        - (mtx[0][0] * mtx[2][1] - mtx[2][0] * mtx[0][1])
        * (mtx[1][2] * mtx[3][3] - mtx[3][2] * mtx[1][3])
        + (mtx[0][0] * mtx[3][1] - mtx[3][0] * mtx[0][1])
        * (mtx[1][2] * mtx[2][3] - mtx[2][2] * mtx[1][3])
        + (mtx[1][0] * mtx[2][1] - mtx[2][0] * mtx[1][1])
        * (mtx[0][2] * mtx[3][3] - mtx[3][2] * mtx[0][3])
        - (mtx[1][0] * mtx[3][1] - mtx[3][0] * mtx[1][1])
        * (mtx[0][2] * mtx[2][3] - mtx[2][2] * mtx[0][3])
        + (mtx[2][0] * mtx[3][1] - mtx[3][0] * mtx[2][1])
        * (mtx[0][2] * mtx[1][3] - mtx[1][2] * mtx[0][3]);
}

inline void Matrix4::fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = x.y,  mtx[0][2] = x.z,  mtx[0][3] = 0.0f;
    mtx[1][0] = y.x,  mtx[1][1] = y.y,  mtx[1][2] = y.z,  mtx[1][3] = 0.0f;
    mtx[2][0] = z.x,  mtx[2][1] = z.y,  mtx[2][2] = z.z,  mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f, mtx[3][1] = 0.0f, mtx[3][2] = 0.0f, mtx[3][3] = 1.0f;
}

inline void Matrix4::fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = y.x,  mtx[0][2] = z.x,  mtx[0][3] = 0.0f;
    mtx[1][0] = x.y,  mtx[1][1] = y.y,  mtx[1][2] = z.y,  mtx[1][3] = 0.0f;
    mtx[2][0] = x.z,  mtx[2][1] = y.z,  mtx[2][2] = z.z,  mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f, mtx[3][1] = 0.0f, mtx[3][2] = 0.0f, mtx[3][3] = 1.0f;
}

inline void Matrix4::identity()
{
    mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f, mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f, mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f, mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f, mtx[3][1] = 0.0f, mtx[3][2] = 0.0f, mtx[3][3] = 1.0f;
}

inline void Matrix4::toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const
{
    x.set(mtx[0][0], mtx[0][1], mtx[0][2]);
    y.set(mtx[1][0], mtx[1][1], mtx[1][2]);
    z.set(mtx[2][0], mtx[2][1], mtx[2][2]);
}

inline void Matrix4::toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const
{
    x.set(mtx[0][0], mtx[1][0], mtx[2][0]);
    y.set(mtx[0][1], mtx[1][1], mtx[2][1]);
    z.set(mtx[0][2], mtx[1][2], mtx[2][2]);
}

inline Matrix4 Matrix4::transpose() const
{
    Matrix4 tmp;

    tmp[0][0] = mtx[0][0], tmp[0][1] = mtx[1][0], tmp[0][2] = mtx[2][0], tmp[0][3] = mtx[3][0];
    tmp[1][0] = mtx[0][1], tmp[1][1] = mtx[1][1], tmp[1][2] = mtx[2][1], tmp[1][3] = mtx[3][1];
    tmp[2][0] = mtx[0][2], tmp[2][1] = mtx[1][2], tmp[2][2] = mtx[2][2], tmp[2][3] = mtx[3][2];
    tmp[3][0] = mtx[0][3], tmp[3][1] = mtx[1][3], tmp[3][2] = mtx[2][3], tmp[3][3] = mtx[3][3];

    return tmp;
}

//-----------------------------------------------------------------------------
// This Quaternionf class will concatenate Quaternionfs in a left to right order.
// The reason for this is to maintain the same multiplication semantics as the
// Matrix3 and Matrix4 classes.

class Quaternionf {
    friend Quaternionf operator*(float lhs, const Quaternionf &rhs);
public:
    static const Quaternionf IDENTITY;
    float w, x, y, z;

    Quaternionf() {
     w=x=y=z=0.0f;
    }
    Quaternionf(float w_, float x_, float y_, float z_);
    Quaternionf(float headDegrees, float pitchDegrees, float rollDegrees);
    Quaternionf(const Vector3 &axis, float degrees);
    explicit Quaternionf(const Matrix3 &m);
    explicit Quaternionf(const Matrix4 &m);
    ~Quaternionf() {}

    SKEYSTART
     SKEY("w",{ SPARAM(w,float,atof); })
     SKEY("x",{ SPARAM(x,float,atof); })
     SKEY("y",{ SPARAM(y,float,atof); })
     SKEY("z",{ SPARAM(z,float,atof); })
    SKEYEND("Quaternionf");
    string toString() {
     char buf[512];
     FORMAT(buf,512,"w %1.5f x %1.5f y %1.5f z %1.5f", (float) w, (float) x, (float) y, (float) z);
     return string(buf);
    }

    static Quaternionf slerp(const Quaternionf &a, const Quaternionf &b, float t);

    bool operator==(const Quaternionf &rhs) const;
    bool operator!=(const Quaternionf &rhs) const;

    Quaternionf &operator+=(const Quaternionf &rhs);
    Quaternionf &operator-=(const Quaternionf &rhs);
    Quaternionf &operator*=(const Quaternionf &rhs);
    Quaternionf &operator*=(float scalar);
    Quaternionf &operator/=(float scalar);

    Quaternionf operator+(const Quaternionf &rhs) const;
    Quaternionf operator-(const Quaternionf &rhs) const;
    Quaternionf operator*(const Quaternionf &rhs) const;
    Quaternionf operator*(float scalar) const;
    Quaternionf operator/(float scalar) const;

    Quaternionf conjugate() const;
    void fromAxisAngle(const Vector3 &axis, float degrees);
    void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    void fromMatrix(const Matrix3 &m);
    void fromMatrix(const Matrix4 &m);
    void identity();
    Quaternionf inverse() const;
    float magnitude() const;
    void normalize();
    void set(float w_, float x_, float y_, float z_);
    void toAxisAngle(Vector3 &axis, float &degrees) const;
    void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
    Matrix3 toMatrix3() const;
    Matrix4 toMatrix4() const;
};

inline Quaternionf operator*(float lhs, const Quaternionf &rhs)
{
    return rhs * lhs;
}

inline Quaternionf::Quaternionf(float w_, float x_, float y_, float z_)
    : w(w_), x(x_), y(y_), z(z_) {}

inline Quaternionf::Quaternionf(float headDegrees, float pitchDegrees, float rollDegrees)
{
    fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
}

inline Quaternionf::Quaternionf(const Vector3 &axis, float degrees)
{
    x=y=z=0;
    fromAxisAngle(axis, degrees);
}

inline Quaternionf::Quaternionf(const Matrix3 &m)
{
    fromMatrix(m);
}

inline Quaternionf::Quaternionf(const Matrix4 &m)
{
    fromMatrix(m);
}

inline bool Quaternionf::operator==(const Quaternionf &rhs) const
{
    return dhpowareMath::closeEnough(w, rhs.w) && dhpowareMath::closeEnough(x, rhs.x)
        && dhpowareMath::closeEnough(y, rhs.y) && dhpowareMath::closeEnough(z, rhs.z);
}

inline bool Quaternionf::operator!=(const Quaternionf &rhs) const
{
    return !(*this == rhs);
}

inline Quaternionf &Quaternionf::operator+=(const Quaternionf &rhs)
{
    w += rhs.w, x += rhs.x, y += rhs.y, z += rhs.z;
    return *this;
}

inline Quaternionf &Quaternionf::operator-=(const Quaternionf &rhs)
{
    w -= rhs.w, x -= rhs.x, y -= rhs.y, z -= rhs.z;
    return *this;
}

inline Quaternionf &Quaternionf::operator*=(const Quaternionf &rhs)
{
    // Multiply so that rotations are applied in a left to right order.
    Quaternionf tmp(
        (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
        (w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
        (w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
        (w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));

    /*
    // Multiply so that rotations are applied in a right to left order.
    Quaternionf tmp(
    (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
    (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
    (w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
    (w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w));
    */

    *this = tmp;
    return *this;
}

inline Quaternionf &Quaternionf::operator*=(float scalar)
{
    w *= scalar, x *= scalar, y *= scalar, z *= scalar;
    return *this;
}

inline Quaternionf &Quaternionf::operator/=(float scalar)
{
    w /= scalar, x /= scalar, y /= scalar, z /= scalar;
    return *this;
}

inline Quaternionf Quaternionf::operator+(const Quaternionf &rhs) const
{
    Quaternionf tmp(*this);
    tmp += rhs;
    return tmp;
}

inline Quaternionf Quaternionf::operator-(const Quaternionf &rhs) const
{
    Quaternionf tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline Quaternionf Quaternionf::operator*(const Quaternionf &rhs) const
{
    Quaternionf tmp(*this);
    tmp *= rhs;
    return tmp;
}

inline Quaternionf Quaternionf::operator*(float scalar) const
{
    Quaternionf tmp(*this);
    tmp *= scalar;
    return tmp;
}

inline Quaternionf Quaternionf::operator/(float scalar) const
{
    Quaternionf tmp(*this);
    tmp /= scalar;
    return tmp;
}

inline Quaternionf Quaternionf::conjugate() const
{
    Quaternionf tmp(w, -x, -y, -z);
    return tmp;
}

inline void Quaternionf::fromAxisAngle(const Vector3 &axis, float degrees)
{
    float halfTheta = dhpowareMath::degreesToRadians(degrees) * 0.5f;
    float s = sinf(halfTheta);
    w = cosf(halfTheta), x = axis.x * s, y = axis.y * s, z = axis.z * s;
}

inline void Quaternionf::fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    Matrix3 m;
    m.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
    fromMatrix(m);
}

inline void Quaternionf::identity()
{
    w = 1.0f, x = y = z = 0.0f;
}

inline Quaternionf Quaternionf::inverse() const
{
    float invMag = 1.0f / magnitude();
    return conjugate() * invMag;
}

inline float Quaternionf::magnitude() const
{
    return sqrtf(w * w + x * x + y * y + z * z);
}

inline void Quaternionf::normalize()
{
    float invMag = 1.0f / magnitude();
    w *= invMag, x *= invMag, y *= invMag, z *= invMag;
}

inline void Quaternionf::set(float w_, float x_, float y_, float z_)
{
    w = w_, x = x_, y = y_, z = z_;
}

inline void Quaternionf::toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
{
    Matrix3 m = toMatrix3();
    m.toHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
}

//-----------------------------------------------------------------------------
// The MatrixStack utility class is used to maintain a stack of Matrix objects.
// pushMatrix() copies the current matrix and adds the copy to the top of
// the matrix stack. popMatrix() removes the top most matrix from the stack.
// currentMatrix() returns a read only copy of the matrix at the top of the
// stack. loadIdentity() replaces the top most matrix on the stack with the
// identity matrix. loadMatrix() replaces the top most matrix with the
// specified matrix. multMatrix() multiplies the top most matrix with the
// specified matrix and replaces the top most matrix with the result.
// lastError() returns the error code from the last operation performed.
// currentDepth() returns the current depth of the matrix stack.
// maxDepth() returns the maximum depth of the matrix stack.

class MatrixStack
{
public:
	enum Error
	{
		ERROR_OK,
		ERROR_INVALID_VALUE,
		ERROR_MATRIX_STACK_OVERFLOW,
		ERROR_MATRIX_STACK_UNDERFLOW
	};

	static const int DEFAULT_MAX_STACK_DEPTH = 32;

	MatrixStack();
	MatrixStack(unsigned int maxDepth);
	~MatrixStack();

	unsigned int currentDepth() const;
	const Matrix4 &currentMatrix() const;
	unsigned int maxDepth() const;
	Error lastError() const;
	void loadIdentity();
	void loadMatrix(const Matrix4 &m);
	void multMatrix(const Matrix4 &m);
	void popMatrix();
	void pushMatrix();

private:
	void init(unsigned int maxDepth);

	Matrix4 *m_pStack;
	unsigned int m_depth;
	unsigned int m_maxDepth;
	Error m_lastError;
};

//-----------------------------------------------------------------------------

#endif