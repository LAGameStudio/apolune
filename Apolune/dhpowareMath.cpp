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

#include "macros.h"
#include "dhpowareMath.h"

//-----------------------------------------------------------------------------
// Math.

int  dhpowareMath::nextPower2(int x)
{
    int i = x & (~x + 1);

    while (i < x)
        i <<= 1;	

    return i;
}

//-----------------------------------------------------------------------------
// Matrix3.

const Matrix3 Matrix3::IDENTITY(1.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 1.0f);

Matrix3 Matrix3::createMirror(const Vector3 &planeNormal)
{
    // Constructs a reflection (or mirror) matrix given an arbitrary plane
    // that passes through the origin.
    //
    // Ronald Goldman, "Matrices and Transformation," Graphics Gems, 1990.

    float x = planeNormal.x;
    float y = planeNormal.y;
    float z = planeNormal.z;

    return Matrix3( 1.0f - 2.0f * x * x, -2.0f * y * x,        -2.0f * z * x,
                   -2.0f * x * y,         1.0f - 2.0f * y * y, -2.0f * z * y,
                   -2.0f * x * z,        -2.0f * y * z,         1.0f - 2.0f * z * z);
}

void Matrix3::fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    // Constructs a rotation matrix based on a Euler Transform.
    // We use the popular NASA standard airplane convention of 
    // heading-pitch-roll (i.e., RzRxRy).

    headDegrees =  dhpowareMath::degreesToRadians(headDegrees);
    pitchDegrees =  dhpowareMath::degreesToRadians(pitchDegrees);
    rollDegrees =  dhpowareMath::degreesToRadians(rollDegrees);

    float cosH = cosf(headDegrees);
    float cosP = cosf(pitchDegrees);
    float cosR = cosf(rollDegrees);
    float sinH = sinf(headDegrees);
    float sinP = sinf(pitchDegrees);
    float sinR = sinf(rollDegrees);

    mtx[0][0] = cosR * cosH - sinR * sinP * sinH;
    mtx[0][1] = sinR * cosH + cosR * sinP * sinH;
    mtx[0][2] = -cosP * sinH;

    mtx[1][0] = -sinR * cosP;
    mtx[1][1] = cosR * cosP;
    mtx[1][2] = sinP;

    mtx[2][0] = cosR * sinH + sinR * sinP * cosH;
    mtx[2][1] = sinR * sinH - cosR * sinP * cosH;
    mtx[2][2] = cosP * cosH;
}

Matrix3 Matrix3::inverse() const
{
    // If the inverse doesn't exist for this matrix, then the identity
    // matrix will be returned.

    Matrix3 tmp;
    float d = determinant();

    if ( dhpowareMath::closeEnough(d, 0.0f))
    {
        tmp.identity();
    }
    else
    {
        d = 1.0f / d;

        tmp.mtx[0][0] = d * (mtx[1][1] * mtx[2][2] - mtx[1][2] * mtx[2][1]);
        tmp.mtx[0][1] = d * (mtx[0][2] * mtx[2][1] - mtx[0][1] * mtx[2][2]);
        tmp.mtx[0][2] = d * (mtx[0][1] * mtx[1][2] - mtx[0][2] * mtx[1][1]);

        tmp.mtx[1][0] = d * (mtx[1][2] * mtx[2][0] - mtx[1][0] * mtx[2][2]);
        tmp.mtx[1][1] = d * (mtx[0][0] * mtx[2][2] - mtx[0][2] * mtx[2][0]);
        tmp.mtx[1][2] = d * (mtx[0][2] * mtx[1][0] - mtx[0][0] * mtx[1][2]);

        tmp.mtx[2][0] = d * (mtx[1][0] * mtx[2][1] - mtx[1][1] * mtx[2][0]);
        tmp.mtx[2][1] = d * (mtx[0][1] * mtx[2][0] - mtx[0][0] * mtx[2][1]);
        tmp.mtx[2][2] = d * (mtx[0][0] * mtx[1][1] - mtx[0][1] * mtx[1][0]);
    }

    return tmp;
}

void Matrix3::orient(const Vector3 &from, const Vector3 &to)
{
    // Creates an orientation matrix that will rotate the vector 'from' 
    // into the vector 'to'. For this method to work correctly, vector
    // 'from' and vector 'to' must both be unit length vectors.
    //
    // The algorithm used is from:
    //   Tomas Moller and John F. Hughes, "Efficiently building a matrix
    //   to rotate one vector to another," Journal of Graphics Tools,
    //   4(4):1-4, 1999.

    float e = Vector3::dot(from, to);

    if ( dhpowareMath::closeEnough(e, 1.0f))
    {
        // Special case where 'from' is equal to 'to'. In other words,
        // the angle between vector 'from' and vector 'to' is zero 
        // degrees. In this case just load the identity matrix.

        identity();
    }
    else if ( dhpowareMath::closeEnough(e, -1.0f))
    {
        // Special case where 'from' is directly opposite to 'to'. In
        // other words, the angle between vector 'from' and vector 'to'
        // is 180 degrees. In this case, the following matrix is used:
        //
        // Let:
        //   F = from
        //   S = vector perpendicular to F
        //   U = S X F
        //
        // We want to rotate from (F, U, S) to (-F, U, -S)
        //
        // | -FxFx+UxUx-SxSx  -FxFy+UxUy-SxSy  -FxFz+UxUz-SxSz |
        // | -FxFy+UxUy-SxSy  -FyFy+UyUy-SySy  -FyFz+UyUz-SySz |
        // | -FxFz+UxUz-SxSz  -FyFz+UyUz-SySz  -FzFz+UzUz-SzSz |
        
        Vector3 side(0.0f, from.z, -from.y);

        if ( dhpowareMath::closeEnough(Vector3::dot(side, side), 0.0f))
            side.set(-from.z, 0.0f, from.x);

        side.normalize();

        Vector3 up = Vector3::cross(side, from);
        up.normalize();

        mtx[0][0] = -(from.x * from.x) + (up.x * up.x) - (side.x * side.x);
        mtx[0][1] = -(from.x * from.y) + (up.x * up.y) - (side.x * side.y);
        mtx[0][2] = -(from.x * from.z) + (up.x * up.z) - (side.x * side.z);
        mtx[1][0] = -(from.x * from.y) + (up.x * up.y) - (side.x * side.y);
        mtx[1][1] = -(from.y * from.y) + (up.y * up.y) - (side.y * side.y);
        mtx[1][2] = -(from.y * from.z) + (up.y * up.z) - (side.y * side.z);
        mtx[2][0] = -(from.x * from.z) + (up.x * up.z) - (side.x * side.z);
        mtx[2][1] = -(from.y * from.z) + (up.y * up.z) - (side.y * side.z);
        mtx[2][2] = -(from.z * from.z) + (up.z * up.z) - (side.z * side.z);
    }
    else
    {
        // This is the most common case. Creates the rotation matrix:
        //
        //               | E + HVx^2   HVxVy + Vz  HVxVz - Vy |
        // R(from, to) = | HVxVy - Vz  E + HVy^2   HVxVz + Vx |
        //               | HVxVz + Vy  HVyVz - Vx  E + HVz^2  |
        //
        // where,
        //   V = from.cross(to)
        //   E = from.dot(to)
        //   H = (1 - E) / V.dot(V)

        Vector3 v = Vector3::cross(from, to);
        v.normalize();

        float h = (1.0f - e) / Vector3::dot(v, v);

        mtx[0][0] = e + h * v.x * v.x;
        mtx[0][1] = h * v.x * v.y + v.z;
        mtx[0][2] = h * v.x * v.z - v.y;

        mtx[1][0] = h * v.x * v.y - v.z;
        mtx[1][1] = e + h * v.y * v.y;
        mtx[1][2] = h * v.x * v.z + v.x;

        mtx[2][0] = h * v.x * v.z + v.y;
        mtx[2][1] = h * v.y * v.z - v.x;
        mtx[2][2] = e + h * v.z * v.z;
    }
}

void Matrix3::rotate(const Vector3 &axis, float degrees)
{
    // Creates a rotation matrix about the specified axis.
    // The axis must be a unit vector. The angle must be in degrees.
    //
    // Let u = axis of rotation = (x, y, z)
    //
    //             | x^2(1 - c) + c  xy(1 - c) + zs  xz(1 - c) - ys |
    // Ru(angle) = | yx(1 - c) - zs  y^2(1 - c) + c  yz(1 - c) + xs |
    //             | zx(1 - c) - ys  zy(1 - c) - xs  z^2(1 - c) + c |
    //
    // where,
    //	c = cos(angle)
    //  s = sin(angle)

    degrees =  dhpowareMath::degreesToRadians(degrees);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = cosf(degrees);
    float s = sinf(degrees);

    mtx[0][0] = (x * x) * (1.0f - c) + c;
    mtx[0][1] = (x * y) * (1.0f - c) + (z * s);
    mtx[0][2] = (x * z) * (1.0f - c) - (y * s);

    mtx[1][0] = (y * x) * (1.0f - c) - (z * s);
    mtx[1][1] = (y * y) * (1.0f - c) + c;
    mtx[1][2] = (y * z) * (1.0f - c) + (x * s);

    mtx[2][0] = (z * x) * (1.0f - c) + (y * s);
    mtx[2][1] = (z * y) * (1.0f - c) - (x * s);
    mtx[2][2] = (z * z) * (1.0f - c) + c;
}

void Matrix3::scale(float sx, float sy, float sz)
{
    // Creates a scaling matrix.
    //
    //                 | sx   0    0  |
    // S(sx, sy, sz) = | 0    sy   0  |
    //                 | 0    0    sz |
    
    mtx[0][0] = sx,   mtx[0][1] = 0.0f, mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = sy,   mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = sz;
}

void Matrix3::toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
{
    // Extracts the Euler angles from a rotation matrix. The returned
    // angles are in degrees. This method might suffer from numerical
    // imprecision for ill defined rotation matrices.
    //
    // This function only works for rotation matrices constructed using
    // the popular NASA standard airplane convention of heading-pitch-roll 
    // (i.e., RzRxRy).
    //
    // The algorithm used is from:
    //  David Eberly, "Euler Angle Formulas", Geometric Tools web site,
    //  http://www.geometrictools.com/Documentation/EulerAngles.pdf.

    float thetaX = asinf(mtx[1][2]);
    float thetaY = 0.0f;
    float thetaZ = 0.0f;

    if (thetaX <  HALF_PIf)
    {
        if (thetaX > - HALF_PIf)
        {
            thetaZ = atan2f(-mtx[1][0], mtx[1][1]);
            thetaY = atan2f(-mtx[0][2], mtx[2][2]);
        }
        else
        {
            // Not a unique solution.
            thetaZ = -atan2f(mtx[2][0], mtx[0][0]);
            thetaY = 0.0f;
        }
    }
    else
    {
        // Not a unique solution.
        thetaZ = atan2f(mtx[2][0], mtx[0][0]);
        thetaY = 0.0f;
    }

    headDegrees =  dhpowareMath::radiansToDegrees(thetaY);
    pitchDegrees =  dhpowareMath::radiansToDegrees(thetaX);
    rollDegrees =  dhpowareMath::radiansToDegrees(thetaZ);
}

//-----------------------------------------------------------------------------
// Matrix4.

const Matrix4 Matrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);

Matrix4 Matrix4::createMirror(const Vector3 &planeNormal, const Vector3 &pointOnPlane)
{
    // Constructs a reflection (or mirror) matrix given an arbitrary plane
    // that passes through the specified position.
    //
    // Ronald Goldman, "Matrices and Transformation," Graphics Gems, 1990.

    float x = planeNormal.x;
    float y = planeNormal.y;
    float z = planeNormal.z;
    float dot = Vector3::dot(planeNormal, pointOnPlane);

    return Matrix4( 1.0f - 2.0f * x * x, -2.0f * y * x,        -2.0f * z * x,        0.0f,    
                   -2.0f * x * y,         1.0f - 2.0f * y * y, -2.0f * z * y,        0.0f,
                   -2.0f * x * z,        -2.0f * y * z,         1.0f - 2.0f * z * z, 0.0f,
                    2.0f * dot * x,       2.0f * dot * y,       2.0f * dot * z,      1.0f);
}

void Matrix4::fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    // Constructs a rotation matrix based on a Euler Transform.
    // We use the popular NASA standard airplane convention of 
    // heading-pitch-roll (i.e., RzRxRy).

    headDegrees =  dhpowareMath::degreesToRadians(headDegrees);
    pitchDegrees =  dhpowareMath::degreesToRadians(pitchDegrees);
    rollDegrees =  dhpowareMath::degreesToRadians(rollDegrees);

    float cosH = cosf(headDegrees);
    float cosP = cosf(pitchDegrees);
    float cosR = cosf(rollDegrees);
    float sinH = sinf(headDegrees);
    float sinP = sinf(pitchDegrees);
    float sinR = sinf(rollDegrees);

    mtx[0][0] = cosR * cosH - sinR * sinP * sinH;
    mtx[0][1] = sinR * cosH + cosR * sinP * sinH;
    mtx[0][2] = -cosP * sinH;
    mtx[0][3] = 0.0f;

    mtx[1][0] = -sinR * cosP;
    mtx[1][1] = cosR * cosP;
    mtx[1][2] = sinP;
    mtx[1][3] = 0.0f;

    mtx[2][0] = cosR * sinH + sinR * sinP * cosH;
    mtx[2][1] = sinR * sinH - cosR * sinP * cosH;
    mtx[2][2] = cosP * cosH;
    mtx[2][3] = 0.0f;

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
}

Matrix4 Matrix4::inverse() const
{
    // This method of computing the inverse of a 4x4 matrix is based
    // on a similar function found in Paul Nettle's matrix template
    // class (http://www.fluidstudios.com).
    //
    // If the inverse doesn't exist for this matrix, then the identity
    // matrix will be returned.

    Matrix4 tmp;
    float d = determinant();

    if ( dhpowareMath::closeEnough(d, 0.0f))
    {
        tmp.identity();
    }
    else
    {
        d = 1.0f / d;

        tmp.mtx[0][0] = d * (mtx[1][1] * (mtx[2][2] * mtx[3][3] - mtx[3][2] * mtx[2][3]) + mtx[2][1] * (mtx[3][2] * mtx[1][3] - mtx[1][2] * mtx[3][3]) + mtx[3][1] * (mtx[1][2] * mtx[2][3] - mtx[2][2] * mtx[1][3]));
        tmp.mtx[1][0] = d * (mtx[1][2] * (mtx[2][0] * mtx[3][3] - mtx[3][0] * mtx[2][3]) + mtx[2][2] * (mtx[3][0] * mtx[1][3] - mtx[1][0] * mtx[3][3]) + mtx[3][2] * (mtx[1][0] * mtx[2][3] - mtx[2][0] * mtx[1][3]));
        tmp.mtx[2][0] = d * (mtx[1][3] * (mtx[2][0] * mtx[3][1] - mtx[3][0] * mtx[2][1]) + mtx[2][3] * (mtx[3][0] * mtx[1][1] - mtx[1][0] * mtx[3][1]) + mtx[3][3] * (mtx[1][0] * mtx[2][1] - mtx[2][0] * mtx[1][1]));
        tmp.mtx[3][0] = d * (mtx[1][0] * (mtx[3][1] * mtx[2][2] - mtx[2][1] * mtx[3][2]) + mtx[2][0] * (mtx[1][1] * mtx[3][2] - mtx[3][1] * mtx[1][2]) + mtx[3][0] * (mtx[2][1] * mtx[1][2] - mtx[1][1] * mtx[2][2]));
        
        tmp.mtx[0][1] = d * (mtx[2][1] * (mtx[0][2] * mtx[3][3] - mtx[3][2] * mtx[0][3]) + mtx[3][1] * (mtx[2][2] * mtx[0][3] - mtx[0][2] * mtx[2][3]) + mtx[0][1] * (mtx[3][2] * mtx[2][3] - mtx[2][2] * mtx[3][3]));
        tmp.mtx[1][1] = d * (mtx[2][2] * (mtx[0][0] * mtx[3][3] - mtx[3][0] * mtx[0][3]) + mtx[3][2] * (mtx[2][0] * mtx[0][3] - mtx[0][0] * mtx[2][3]) + mtx[0][2] * (mtx[3][0] * mtx[2][3] - mtx[2][0] * mtx[3][3]));
        tmp.mtx[2][1] = d * (mtx[2][3] * (mtx[0][0] * mtx[3][1] - mtx[3][0] * mtx[0][1]) + mtx[3][3] * (mtx[2][0] * mtx[0][1] - mtx[0][0] * mtx[2][1]) + mtx[0][3] * (mtx[3][0] * mtx[2][1] - mtx[2][0] * mtx[3][1]));
        tmp.mtx[3][1] = d * (mtx[2][0] * (mtx[3][1] * mtx[0][2] - mtx[0][1] * mtx[3][2]) + mtx[3][0] * (mtx[0][1] * mtx[2][2] - mtx[2][1] * mtx[0][2]) + mtx[0][0] * (mtx[2][1] * mtx[3][2] - mtx[3][1] * mtx[2][2]));
        
        tmp.mtx[0][2] = d * (mtx[3][1] * (mtx[0][2] * mtx[1][3] - mtx[1][2] * mtx[0][3]) + mtx[0][1] * (mtx[1][2] * mtx[3][3] - mtx[3][2] * mtx[1][3]) + mtx[1][1] * (mtx[3][2] * mtx[0][3] - mtx[0][2] * mtx[3][3]));
        tmp.mtx[1][2] = d * (mtx[3][2] * (mtx[0][0] * mtx[1][3] - mtx[1][0] * mtx[0][3]) + mtx[0][2] * (mtx[1][0] * mtx[3][3] - mtx[3][0] * mtx[1][3]) + mtx[1][2] * (mtx[3][0] * mtx[0][3] - mtx[0][0] * mtx[3][3]));
        tmp.mtx[2][2] = d * (mtx[3][3] * (mtx[0][0] * mtx[1][1] - mtx[1][0] * mtx[0][1]) + mtx[0][3] * (mtx[1][0] * mtx[3][1] - mtx[3][0] * mtx[1][1]) + mtx[1][3] * (mtx[3][0] * mtx[0][1] - mtx[0][0] * mtx[3][1]));
        tmp.mtx[3][2] = d * (mtx[3][0] * (mtx[1][1] * mtx[0][2] - mtx[0][1] * mtx[1][2]) + mtx[0][0] * (mtx[3][1] * mtx[1][2] - mtx[1][1] * mtx[3][2]) + mtx[1][0] * (mtx[0][1] * mtx[3][2] - mtx[3][1] * mtx[0][2]));
        
        tmp.mtx[0][3] = d * (mtx[0][1] * (mtx[2][2] * mtx[1][3] - mtx[1][2] * mtx[2][3]) + mtx[1][1] * (mtx[0][2] * mtx[2][3] - mtx[2][2] * mtx[0][3]) + mtx[2][1] * (mtx[1][2] * mtx[0][3] - mtx[0][2] * mtx[1][3]));
        tmp.mtx[1][3] = d * (mtx[0][2] * (mtx[2][0] * mtx[1][3] - mtx[1][0] * mtx[2][3]) + mtx[1][2] * (mtx[0][0] * mtx[2][3] - mtx[2][0] * mtx[0][3]) + mtx[2][2] * (mtx[1][0] * mtx[0][3] - mtx[0][0] * mtx[1][3]));
        tmp.mtx[2][3] = d * (mtx[0][3] * (mtx[2][0] * mtx[1][1] - mtx[1][0] * mtx[2][1]) + mtx[1][3] * (mtx[0][0] * mtx[2][1] - mtx[2][0] * mtx[0][1]) + mtx[2][3] * (mtx[1][0] * mtx[0][1] - mtx[0][0] * mtx[1][1]));
        tmp.mtx[3][3] = d * (mtx[0][0] * (mtx[1][1] * mtx[2][2] - mtx[2][1] * mtx[1][2]) + mtx[1][0] * (mtx[2][1] * mtx[0][2] - mtx[0][1] * mtx[2][2]) + mtx[2][0] * (mtx[0][1] * mtx[1][2] - mtx[1][1] * mtx[0][2]));
    }

    return tmp;
}

void Matrix4::orient(const Vector3 &from, const Vector3 &to)
{
    // Creates an orientation matrix that will rotate the vector 'from' 
    // into the vector 'to'. For this method to work correctly, vector
    // 'from' and vector 'to' must both be unit length vectors.
    //
    // The algorithm used is from:
    //   Tomas Moller and John F. Hughes, "Efficiently building a matrix
    //   to rotate one vector to another," Journal of Graphics Tools,
    //   4(4):1-4, 1999.

    float e = Vector3::dot(from, to);

    if ( dhpowareMath::closeEnough(e, 1.0f))
    {
        // Special case where 'from' is equal to 'to'. In other words,
        // the angle between vector 'from' and vector 'to' is zero 
        // degrees. In this case just load the identity matrix.

        identity();
    }
    else if ( dhpowareMath::closeEnough(e, -1.0f))
    {
        // Special case where 'from' is directly opposite to 'to'. In
        // other words, the angle between vector 'from' and vector 'to'
        // is 180 degrees. In this case, the following matrix is used:
        //
        // Let:
        //   F = from
        //   S = vector perpendicular to F
        //   U = S X F
        //
        // We want to rotate from (F, U, S) to (-F, U, -S)
        //
        // | -FxFx+UxUx-SxSx  -FxFy+UxUy-SxSy  -FxFz+UxUz-SxSz  0 |
        // | -FxFy+UxUy-SxSy  -FyFy+UyUy-SySy  -FyFz+UyUz-SySz  0 |
        // | -FxFz+UxUz-SxSz  -FyFz+UyUz-SySz  -FzFz+UzUz-SzSz  0 |
        // |       0                 0                0         1 |

        Vector3 side(0.0f, from.z, -from.y);

        if ( dhpowareMath::closeEnough(Vector3::dot(side, side), 0.0f))
            side.set(-from.z, 0.0f, from.x);

        side.normalize();

        Vector3 up = Vector3::cross(side, from);
        up.normalize();

        mtx[0][0] = -(from.x * from.x) + (up.x * up.x) - (side.x * side.x);
        mtx[0][1] = -(from.x * from.y) + (up.x * up.y) - (side.x * side.y);
        mtx[0][2] = -(from.x * from.z) + (up.x * up.z) - (side.x * side.z);
        mtx[0][3] = 0.0f;
        mtx[1][0] = -(from.x * from.y) + (up.x * up.y) - (side.x * side.y);
        mtx[1][1] = -(from.y * from.y) + (up.y * up.y) - (side.y * side.y);
        mtx[1][2] = -(from.y * from.z) + (up.y * up.z) - (side.y * side.z);
        mtx[1][3] = 0.0f;
        mtx[2][0] = -(from.x * from.z) + (up.x * up.z) - (side.x * side.z);
        mtx[2][1] = -(from.y * from.z) + (up.y * up.z) - (side.y * side.z);
        mtx[2][2] = -(from.z * from.z) + (up.z * up.z) - (side.z * side.z);
        mtx[2][3] = 0.0f;
        mtx[3][0] = 0.0f;
        mtx[3][1] = 0.0f;
        mtx[3][2] = 0.0f;
        mtx[3][3] = 1.0f;
    }
    else
    {
        // This is the most common case. Creates the rotation matrix:
        //
        //               | E + HVx^2   HVxVy + Vz  HVxVz - Vy  0 |
        // R(from, to) = | HVxVy - Vz  E + HVy^2   HVxVz + Vx  0 |
        //               | HVxVz + Vy  HVyVz - Vx  E + HVz^2   0 |
        //               |     0           0           0       1 |
        //
        // where,
        //   V = from.cross(to)
        //   E = from.dot(to)
        //   H = (1 - E) / V.dot(V)

        Vector3 v = Vector3::cross(from, to);
        v.normalize();

        float h = (1.0f - e) / Vector3::dot(v, v);

        mtx[0][0] = e + h * v.x * v.x;
        mtx[0][1] = h * v.x * v.y + v.z;
        mtx[0][2] = h * v.x * v.z - v.y;
        mtx[0][3] = 0.0f;

        mtx[1][0] = h * v.x * v.y - v.z;
        mtx[1][1] = e + h * v.y * v.y;
        mtx[1][2] = h * v.x * v.z + v.x;
        mtx[1][3] = 0.0f;

        mtx[2][0] = h * v.x * v.z + v.y;
        mtx[2][1] = h * v.y * v.z - v.x;
        mtx[2][2] = e + h * v.z * v.z;
        mtx[2][3] = 0.0f;

        mtx[3][0] = 0.0f;
        mtx[3][1] = 0.0f;
        mtx[3][2] = 0.0f;
        mtx[3][3] = 1.0f;
    }
}

void Matrix4::rotate(const Vector3 &axis, float degrees)
{
    // Creates a rotation matrix about the specified axis.
    // The axis must be a unit vector. The angle must be in degrees.
    //
    // Let u = axis of rotation = (x, y, z)
    //
    //             | x^2(1 - c) + c  xy(1 - c) + zs  xz(1 - c) - ys   0 |
    // Ru(angle) = | yx(1 - c) - zs  y^2(1 - c) + c  yz(1 - c) + xs   0 |
    //             | zx(1 - c) - ys  zy(1 - c) - xs  z^2(1 - c) + c   0 |
    //             |      0              0                0           1 |
    //
    // where,
    //	c = cos(angle)
    //  s = sin(angle)

    degrees =  dhpowareMath::degreesToRadians(degrees);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = cosf(degrees);
    float s = sinf(degrees);

    mtx[0][0] = (x * x) * (1.0f - c) + c;
    mtx[0][1] = (x * y) * (1.0f - c) + (z * s);
    mtx[0][2] = (x * z) * (1.0f - c) - (y * s);
    mtx[0][3] = 0.0f;

    mtx[1][0] = (y * x) * (1.0f - c) - (z * s);
    mtx[1][1] = (y * y) * (1.0f - c) + c;
    mtx[1][2] = (y * z) * (1.0f - c) + (x * s);
    mtx[1][3] = 0.0f;

    mtx[2][0] = (z * x) * (1.0f - c) + (y * s);
    mtx[2][1] = (z * y) * (1.0f - c) - (x * s);
    mtx[2][2] = (z * z) * (1.0f - c) + c;
    mtx[2][3] = 0.0f;

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
}

void Matrix4::scale(float sx, float sy, float sz)
{
    // Creates a scaling matrix.
    //
    //                 | sx   0    0    0 |
    // S(sx, sy, sz) = | 0    sy   0    0 |
    //                 | 0    0    sz   0 |
    //                 | 0    0    0    1 |

    mtx[0][0] = sx,   mtx[0][1] = 0.0f, mtx[0][2] = 0.0f, mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = sy,   mtx[1][2] = 0.0f, mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = sz,   mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f, mtx[3][1] = 0.0f, mtx[3][2] = 0.0f, mtx[3][3] = 1.0f;
}

void Matrix4::toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
{
    // Extracts the Euler angles from a rotation matrix. The returned
    // angles are in degrees. This method might suffer from numerical
    // imprecision for ill defined rotation matrices.
    //
    // This function only works for rotation matrices constructed using
    // the popular NASA standard airplane convention of heading-pitch-roll 
    // (i.e., RzRxRy).
    //
    // The algorithm used is from:
    //  David Eberly, "Euler Angle Formulas", Geometric Tools web site,
    //  http://www.geometrictools.com/Documentation/EulerAngles.pdf.

    float thetaX = asinf(mtx[1][2]);
    float thetaY = 0.0f;
    float thetaZ = 0.0f;

    if (thetaX <  HALF_PIf)
    {
        if (thetaX > - HALF_PIf)
        {
            thetaZ = atan2f(-mtx[1][0], mtx[1][1]);
            thetaY = atan2f(-mtx[0][2], mtx[2][2]);
        }
        else
        {
            // Not a unique solution.
            thetaZ = -atan2f(mtx[2][0], mtx[0][0]);
            thetaY = 0.0f;
        }
    }
    else
    {
        // Not a unique solution.
        thetaZ = atan2f(mtx[2][0], mtx[0][0]);
        thetaY = 0.0f;
    }

    headDegrees =  dhpowareMath::radiansToDegrees(thetaY);
    pitchDegrees =  dhpowareMath::radiansToDegrees(thetaX);
    rollDegrees =  dhpowareMath::radiansToDegrees(thetaZ);
}

void Matrix4::translate(float tx, float ty, float tz)
{
    // Creates a translation matrix.
    //
    //                 | 1    0    0    0 |
    // T(tx, ty, tz) = | 0    1    0    0 |
    //                 | 0    0    1    0 |
    //                 | tx   ty   tz   1 |

    mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f, mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f, mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f, mtx[2][3] = 0.0f;
    mtx[3][0] = tx,   mtx[3][1] = ty,   mtx[3][2] = tz,   mtx[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Quaternionf.

const Quaternionf Quaternionf::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

Quaternionf Quaternionf::slerp(const Quaternionf &a, const Quaternionf &b, float t)
{
    // Smoothly interpolates from Quaternionf 'a' to Quaternionf 'b' using
    // spherical linear interpolation.
    // 
    // Both Quaternionfs must be unit length and represent absolute rotations.
    // In particular Quaternionf 'b' must not be relative to Quaternionf 'a'.
    // If 'b' is relative to 'a' make 'b' an absolute rotation by: b = a * b.
    // 
    // The interpolation parameter 't' is in the range [0,1]. When t = 0 the
    // resulting Quaternionf will be 'a'. When t = 1 the resulting Quaternionf
    // will be 'b'.
    //
    // The algorithm used is adapted from Allan and Mark Watt's "Advanced
    // Animation and Rendering Techniques" (ACM Press 1992).

    Quaternionf result;
    float omega = 0.0f;
    float cosom = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    float sinom = 0.0f;
    float scale0 = 0.0f;
    float scale1 = 0.0f;
        
    if ((1.0f + cosom) >  EPSILON)
    {
        // 'a' and 'b' Quaternionfs are not opposite each other.

        if ((1.0f - cosom) >  EPSILON)
        {
            // Standard case - slerp.
            omega = acosf(cosom);
            sinom = sinf(omega);
            scale0 = sinf((1.0f - t) * omega) / sinom;
            scale1 = sinf(t * omega) / sinom;
        }
        else
        {
            // 'a' and 'b' Quaternionfs are very close so lerp instead.
            scale0 = 1.0f - t;
            scale1 = t;
        }

        result.x = scale0 * a.x + scale1 * b.x;
        result.y = scale0 * a.y + scale1 * b.y;
        result.z = scale0 * a.z + scale1 * b.z;
        result.w = scale0 * a.w + scale1 * b.w;
    }
    else
    {
        // 'a' and 'b' Quaternionfs are opposite each other.

        result.x = -b.y;
        result.y = b.x;
        result.z = -b.w;
        result.w = b.z;
        
        scale0 = sinf((1.0f - t) -  HALF_PIf);
        scale1 = sinf(t *  HALF_PIf);

        result.x = scale0 * a.x + scale1 * result.x;
        result.y = scale0 * a.y + scale1 * result.y;
        result.z = scale0 * a.z + scale1 * result.z;
        result.w = scale0 * a.w + scale1 * result.w;
    }

    return result;
}

void Quaternionf::fromMatrix(const Matrix3 &m)
{
    // Creates a Quaternionf from a rotation matrix. 
    // The algorithm used is from Allan and Mark Watt's "Advanced 
    // Animation and Rendering Techniques" (ACM Press 1992).

    float s = 0.0f;
    float q[4] = {0.0f};
    float trace = m[0][0] + m[1][1] + m[2][2];

    if (trace > 0.0f)
    {
        s = sqrtf(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m[1][2] - m[2][1]) * s;
        q[1] = (m[2][0] - m[0][2]) * s;
        q[2] = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        int nxt[3] = {1, 2, 0};
        int i = 0, j = 0, k = 0;

        if (m[1][1] > m[0][0])
            i = 1;

        if (m[2][2] > m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

        q[i] = s * 0.5f;
        s = 0.5f / s;
        q[3] = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;
    }

    x = q[0], y = q[1], z = q[2], w = q[3];
}

void Quaternionf::fromMatrix(const Matrix4 &m)
{
    // Creates a Quaternionf from a rotation matrix. 
    // The algorithm used is from Allan and Mark Watt's "Advanced 
    // Animation and Rendering Techniques" (ACM Press 1992).

    float s = 0.0f;
    float q[4] = {0.0f};
    float trace = m[0][0] + m[1][1] + m[2][2];

    if (trace > 0.0f)
    {
        s = sqrtf(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m[1][2] - m[2][1]) * s;
        q[1] = (m[2][0] - m[0][2]) * s;
        q[2] = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        int nxt[3] = {1, 2, 0};
        int i = 0, j = 0, k = 0;

        if (m[1][1] > m[0][0])
            i = 1;

        if (m[2][2] > m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

        q[i] = s * 0.5f;
        s = 0.5f / s;
        q[3] = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;
    }

    x = q[0], y = q[1], z = q[2], w = q[3];
}

void Quaternionf::toAxisAngle(Vector3 &axis, float &degrees) const
{
    // Converts this Quaternionf to an axis and an angle.

    float sinHalfThetaSq = 1.0f - w * w;

    // Guard against numerical imprecision and identity Quaternionfs.
    if (sinHalfThetaSq <= 0.0f)
    {
        axis.x = 1.0f, axis.y = axis.z = 0.0f;
        degrees = 0.0f;
    }
    else
    {
        float invSinHalfTheta = 1.0f / sqrtf(sinHalfThetaSq);

        axis.x = x * invSinHalfTheta;
        axis.y = y * invSinHalfTheta;
        axis.z = z * invSinHalfTheta;
        degrees =  dhpowareMath::radiansToDegrees(2.0f * acosf(w));
    }
}

Matrix3 Quaternionf::toMatrix3() const
{
    // Converts this Quaternionf to a rotation matrix.
    //
    //  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)		 |
    //  | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)		 |
    //  | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2) |

    float x2 = x + x;
    float y2 = y + y;
    float z2 = z + z;
    float xx = x * x2;
    float xy = x * y2;
    float xz = x * z2;
    float yy = y * y2;
    float yz = y * z2;
    float zz = z * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

    Matrix3 m;

    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy + wz;
    m[0][2] = xz - wy;

    m[1][0] = xy - wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz + wx;

    m[2][0] = xz + wy;
    m[2][1] = yz - wx;
    m[2][2] = 1.0f - (xx + yy);

    return m;
}

Matrix4 Quaternionf::toMatrix4() const
{
    // Converts this Quaternionf to a rotation matrix.
    //
    //  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
    //  | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)			0  |
    //  | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2)	0  |
    //  | 0					0					0					1  |

    float x2 = x + x; 
    float y2 = y + y; 
    float z2 = z + z;
    float xx = x * x2;
    float xy = x * y2;
    float xz = x * z2;
    float yy = y * y2;
    float yz = y * z2;
    float zz = z * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

    Matrix4 m;

    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy + wz;
    m[0][2] = xz - wy;
    m[0][3] = 0.0f;

    m[1][0] = xy - wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz + wx;
    m[1][3] = 0.0f;

    m[2][0] = xz + wy;
    m[2][1] = yz - wx;
    m[2][2] = 1.0f - (xx + yy);
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// MatrixStack.


MatrixStack::MatrixStack()
{
	init(DEFAULT_MAX_STACK_DEPTH);
}

MatrixStack::MatrixStack(unsigned int maxDepth)
{
	init(maxDepth);
}

MatrixStack::~MatrixStack()
{
	delete [] m_pStack;
	m_pStack = 0;
}

unsigned int MatrixStack::currentDepth() const
{
	return m_depth;
}

const Matrix4 &MatrixStack::currentMatrix() const
{
	return m_pStack[m_depth];
}

MatrixStack::Error MatrixStack::lastError() const
{
	return m_lastError;
}

void MatrixStack::loadIdentity()
{
	m_pStack[m_depth].identity();
}

void MatrixStack::loadMatrix(const Matrix4 &m)
{
	m_pStack[m_depth] = m;
}

unsigned int MatrixStack::maxDepth() const
{
	return m_maxDepth;
}

void MatrixStack::multMatrix(const Matrix4 &m)
{
	m_pStack[m_depth] *= m;
}

void MatrixStack::popMatrix()
{
	if (m_depth == 0)
	{
		m_lastError = ERROR_MATRIX_STACK_UNDERFLOW;
	}
	else
	{
		--m_depth;
		m_lastError = ERROR_OK;
	}
}

void MatrixStack::pushMatrix()
{
	if (m_depth + 1 >= m_maxDepth)
	{
		m_lastError = ERROR_MATRIX_STACK_OVERFLOW;
	}
	else
	{
		m_pStack[m_depth + 1] = m_pStack[m_depth];
		++m_depth;
		m_lastError = ERROR_OK;
	}
}

void MatrixStack::init(unsigned int maxDepth)
{	
	m_pStack = 0;
	m_depth = 0;
	m_maxDepth = 0;
	m_lastError = ERROR_OK;

	if (maxDepth > 0)
	{
		m_maxDepth = maxDepth;
		m_pStack = new Matrix4[m_maxDepth];
		m_lastError = ERROR_OK;
	}
	else
	{
		m_lastError = ERROR_INVALID_VALUE;
	}
}