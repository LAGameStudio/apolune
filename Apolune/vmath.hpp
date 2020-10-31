/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <math.h>

#ifdef _VECTORMATH_DEBUG
#include <stdio.h>
#endif

namespace Vectormath {

namespace Aos {

//-----------------------------------------------------------------------------
// Forward Declarations
//

class VVector3;
class VVector4;
class VPoint3;
class VQuat;
class VMatrix3;
class VMatrix4;
class VTransform3;

// A 3-D vector in array-of-structures format
//
class VVector3
{
    float mX;
    float mY;
    float mZ;
#ifndef __GNUC__
    float d;
#endif

public:
    // Default constructor; does no initialization
    // 
    inline VVector3( ) { };

    // Copy a 3-D vector
    // 
    inline VVector3( const VVector3 & vec );

    // Construct a 3-D vector from x, y, and z elements
    // 
    inline VVector3( float x, float y, float z );

    // Copy elements from a 3-D point into a 3-D vector
    // 
    explicit inline VVector3( const VPoint3 & pnt );

    // Set all elements of a 3-D vector to the same scalar value
    // 
    explicit inline VVector3( float scalar );

    // Assign one 3-D vector to another
    // 
    inline VVector3 & operator =( const VVector3 & vec );

    // Set the x element of a 3-D vector
    // 
    inline VVector3 & setX( float x );

    // Set the y element of a 3-D vector
    // 
    inline VVector3 & setY( float y );

    // Set the z element of a 3-D vector
    // 
    inline VVector3 & setZ( float z );

    // Get the x element of a 3-D vector
    // 
    inline float getX( ) const;

    // Get the y element of a 3-D vector
    // 
    inline float getY( ) const;

    // Get the z element of a 3-D vector
    // 
    inline float getZ( ) const;

    // Set an x, y, or z element of a 3-D vector by index
    // 
    inline VVector3 & setElem( int idx, float value );

    // Get an x, y, or z element of a 3-D vector by index
    // 
    inline float getElem( int idx ) const;

    // Subscripting operator to set or get an element
    // 
    inline float & operator []( int idx );

    // Subscripting operator to get an element
    // 
    inline float operator []( int idx ) const;

    // Add two 3-D vectors
    // 
    inline const VVector3 operator +( const VVector3 & vec ) const;

    // Subtract a 3-D vector from another 3-D vector
    // 
    inline const VVector3 operator -( const VVector3 & vec ) const;

    // Add a 3-D vector to a 3-D point
    // 
    inline const VPoint3 operator +( const VPoint3 & pnt ) const;

    // Multiply a 3-D vector by a scalar
    // 
    inline const VVector3 operator *( float scalar ) const;

    // Divide a 3-D vector by a scalar
    // 
    inline const VVector3 operator /( float scalar ) const;

    // Perform compound assignment and addition with a 3-D vector
    // 
    inline VVector3 & operator +=( const VVector3 & vec );

    // Perform compound assignment and subtraction by a 3-D vector
    // 
    inline VVector3 & operator -=( const VVector3 & vec );

    // Perform compound assignment and multiplication by a scalar
    // 
    inline VVector3 & operator *=( float scalar );

    // Perform compound assignment and division by a scalar
    // 
    inline VVector3 & operator /=( float scalar );

    // Negate all elements of a 3-D vector
    // 
    inline const VVector3 operator -( ) const;

    // Construct x axis
    // 
    static inline const VVector3 xAxis( );

    // Construct y axis
    // 
    static inline const VVector3 yAxis( );

    // Construct z axis
    // 
    static inline const VVector3 zAxis( );

}
#ifdef __GNUC__
__attribute__ ((aligned(16)))
#endif
;

// Multiply a 3-D vector by a scalar
// 
inline const VVector3 operator *( float scalar, const VVector3 & vec );

// Multiply two 3-D vectors per element
// 
inline const VVector3 mulPerElem( const VVector3 & vec0, const VVector3 & vec1 );

// Divide two 3-D vectors per element
// NOTE: 
// Floating-point behavior matches standard library function divf4.
// 
inline const VVector3 divPerElem( const VVector3 & vec0, const VVector3 & vec1 );

// Compute the reciprocal of a 3-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function recipf4.
// 
inline const VVector3 recipPerElem( const VVector3 & vec );

// Compute the square root of a 3-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function sqrtf4.
// 
inline const VVector3 sqrtPerElem( const VVector3 & vec );

// Compute the reciprocal square root of a 3-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function rsqrtf4.
// 
inline const VVector3 rsqrtPerElem( const VVector3 & vec );

// Compute the absolute value of a 3-D vector per element
// 
inline const VVector3 absPerElem( const VVector3 & vec );

// Copy sign from one 3-D vector to another, per element
// 
inline const VVector3 copySignPerElem( const VVector3 & vec0, const VVector3 & vec1 );

// Maximum of two 3-D vectors per element
// 
inline const VVector3 maxPerElem( const VVector3 & vec0, const VVector3 & vec1 );

// Minimum of two 3-D vectors per element
// 
inline const VVector3 minPerElem( const VVector3 & vec0, const VVector3 & vec1 );

// Maximum element of a 3-D vector
// 
inline float maxElem( const VVector3 & vec );

// Minimum element of a 3-D vector
// 
inline float minElem( const VVector3 & vec );

// Compute the sum of all elements of a 3-D vector
// 
inline float sum( const VVector3 & vec );

// Compute the dot product of two 3-D vectors
// 
inline float dot( const VVector3 & vec0, const VVector3 & vec1 );

// Compute the square of the length of a 3-D vector
// 
inline float lengthSqr( const VVector3 & vec );

// Compute the length of a 3-D vector
// 
inline float length( const VVector3 & vec );

// Normalize a 3-D vector
// NOTE: 
// The result is unpredictable when all elements of vec are at or near zero.
// 
inline const VVector3 normalize( const VVector3 & vec );

// Compute cross product of two 3-D vectors
// 
inline const VVector3 cross( const VVector3 & vec0, const VVector3 & vec1 );

// Outer product of two 3-D vectors
// 
inline const VMatrix3 outer( const VVector3 & vec0, const VVector3 & vec1 );

// Pre-multiply a row vector by a 3x3 matrix
// 
inline const VVector3 rowMul( const VVector3 & vec, const VMatrix3 & mat );

// Cross-product matrix of a 3-D vector
// 
inline const VMatrix3 crossMatrix( const VVector3 & vec );

// Create cross-product matrix and multiply
// NOTE: 
// Faster than separately creating a cross-product matrix and multiplying.
// 
inline const VMatrix3 crossMatrixMul( const VVector3 & vec, const VMatrix3 & mat );

// Linear interpolation between two 3-D vectors
// NOTE: 
// Does not clamp t between 0 and 1.
// 
inline const VVector3 lerp( float t, const VVector3 & vec0, const VVector3 & vec1 );

// Spherical linear interpolation between two 3-D vectors
// NOTE: 
// The result is unpredictable if the vectors point in opposite directions.
// Does not clamp t between 0 and 1.
// 
inline const VVector3 slerp( float t, const VVector3 & unitVec0, const VVector3 & unitVec1 );

// Conditionally select between two 3-D vectors
// 
inline const VVector3 select( const VVector3 & vec0, const VVector3 & vec1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 3-D vector
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VVector3 & vec );

// Print a 3-D vector and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VVector3 & vec, const char * name );

#endif

// A 4-D vector in array-of-structures format
//
class VVector4
{
    float mX;
    float mY;
    float mZ;
    float mW;

public:
    // Default constructor; does no initialization
    // 
    inline VVector4( ) { };

    // Copy a 4-D vector
    // 
    inline VVector4( const VVector4 & vec );

    // Construct a 4-D vector from x, y, z, and w elements
    // 
    inline VVector4( float x, float y, float z, float w );

    // Construct a 4-D vector from a 3-D vector and a scalar
    // 
    inline VVector4( const VVector3 & xyz, float w );

    // Copy x, y, and z from a 3-D vector into a 4-D vector, and set w to 0
    // 
    explicit inline VVector4( const VVector3 & vec );

    // Copy x, y, and z from a 3-D point into a 4-D vector, and set w to 1
    // 
    explicit inline VVector4( const VPoint3 & pnt );

    // Copy elements from a VQuaternion into a 4-D vector
    // 
    explicit inline VVector4( const VQuat & VQuat );

    // Set all elements of a 4-D vector to the same scalar value
    // 
    explicit inline VVector4( float scalar );

    // Assign one 4-D vector to another
    // 
    inline VVector4 & operator =( const VVector4 & vec );

    // Set the x, y, and z elements of a 4-D vector
    // NOTE: 
    // This function does not change the w element.
    // 
    inline VVector4 & setXYZ( const VVector3 & vec );

    // Get the x, y, and z elements of a 4-D vector
    // 
    inline const VVector3 getXYZ( ) const;

    // Set the x element of a 4-D vector
    // 
    inline VVector4 & setX( float x );

    // Set the y element of a 4-D vector
    // 
    inline VVector4 & setY( float y );

    // Set the z element of a 4-D vector
    // 
    inline VVector4 & setZ( float z );

    // Set the w element of a 4-D vector
    // 
    inline VVector4 & setW( float w );

    // Get the x element of a 4-D vector
    // 
    inline float getX( ) const;

    // Get the y element of a 4-D vector
    // 
    inline float getY( ) const;

    // Get the z element of a 4-D vector
    // 
    inline float getZ( ) const;

    // Get the w element of a 4-D vector
    // 
    inline float getW( ) const;

    // Set an x, y, z, or w element of a 4-D vector by index
    // 
    inline VVector4 & setElem( int idx, float value );

    // Get an x, y, z, or w element of a 4-D vector by index
    // 
    inline float getElem( int idx ) const;

    // Subscripting operator to set or get an element
    // 
    inline float & operator []( int idx );

    // Subscripting operator to get an element
    // 
    inline float operator []( int idx ) const;

    // Add two 4-D vectors
    // 
    inline const VVector4 operator +( const VVector4 & vec ) const;

    // Subtract a 4-D vector from another 4-D vector
    // 
    inline const VVector4 operator -( const VVector4 & vec ) const;

    // Multiply a 4-D vector by a scalar
    // 
    inline const VVector4 operator *( float scalar ) const;

    // Divide a 4-D vector by a scalar
    // 
    inline const VVector4 operator /( float scalar ) const;

    // Perform compound assignment and addition with a 4-D vector
    // 
    inline VVector4 & operator +=( const VVector4 & vec );

    // Perform compound assignment and subtraction by a 4-D vector
    // 
    inline VVector4 & operator -=( const VVector4 & vec );

    // Perform compound assignment and multiplication by a scalar
    // 
    inline VVector4 & operator *=( float scalar );

    // Perform compound assignment and division by a scalar
    // 
    inline VVector4 & operator /=( float scalar );

    // Negate all elements of a 4-D vector
    // 
    inline const VVector4 operator -( ) const;

    // Construct x axis
    // 
    static inline const VVector4 xAxis( );

    // Construct y axis
    // 
    static inline const VVector4 yAxis( );

    // Construct z axis
    // 
    static inline const VVector4 zAxis( );

    // Construct w axis
    // 
    static inline const VVector4 wAxis( );

}
#ifdef __GNUC__
__attribute__ ((aligned(16)))
#endif
;

// Multiply a 4-D vector by a scalar
// 
inline const VVector4 operator *( float scalar, const VVector4 & vec );

// Multiply two 4-D vectors per element
// 
inline const VVector4 mulPerElem( const VVector4 & vec0, const VVector4 & vec1 );

// Divide two 4-D vectors per element
// NOTE: 
// Floating-point behavior matches standard library function divf4.
// 
inline const VVector4 divPerElem( const VVector4 & vec0, const VVector4 & vec1 );

// Compute the reciprocal of a 4-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function recipf4.
// 
inline const VVector4 recipPerElem( const VVector4 & vec );

// Compute the square root of a 4-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function sqrtf4.
// 
inline const VVector4 sqrtPerElem( const VVector4 & vec );

// Compute the reciprocal square root of a 4-D vector per element
// NOTE: 
// Floating-point behavior matches standard library function rsqrtf4.
// 
inline const VVector4 rsqrtPerElem( const VVector4 & vec );

// Compute the absolute value of a 4-D vector per element
// 
inline const VVector4 absPerElem( const VVector4 & vec );

// Copy sign from one 4-D vector to another, per element
// 
inline const VVector4 copySignPerElem( const VVector4 & vec0, const VVector4 & vec1 );

// Maximum of two 4-D vectors per element
// 
inline const VVector4 maxPerElem( const VVector4 & vec0, const VVector4 & vec1 );

// Minimum of two 4-D vectors per element
// 
inline const VVector4 minPerElem( const VVector4 & vec0, const VVector4 & vec1 );

// Maximum element of a 4-D vector
// 
inline float maxElem( const VVector4 & vec );

// Minimum element of a 4-D vector
// 
inline float minElem( const VVector4 & vec );

// Compute the sum of all elements of a 4-D vector
// 
inline float sum( const VVector4 & vec );

// Compute the dot product of two 4-D vectors
// 
inline float dot( const VVector4 & vec0, const VVector4 & vec1 );

// Compute the square of the length of a 4-D vector
// 
inline float lengthSqr( const VVector4 & vec );

// Compute the length of a 4-D vector
// 
inline float length( const VVector4 & vec );

// Normalize a 4-D vector
// NOTE: 
// The result is unpredictable when all elements of vec are at or near zero.
// 
inline const VVector4 normalize( const VVector4 & vec );

// Outer product of two 4-D vectors
// 
inline const VMatrix4 outer( const VVector4 & vec0, const VVector4 & vec1 );

// Linear interpolation between two 4-D vectors
// NOTE: 
// Does not clamp t between 0 and 1.
// 
inline const VVector4 lerp( float t, const VVector4 & vec0, const VVector4 & vec1 );

// Spherical linear interpolation between two 4-D vectors
// NOTE: 
// The result is unpredictable if the vectors point in opposite directions.
// Does not clamp t between 0 and 1.
// 
inline const VVector4 slerp( float t, const VVector4 & unitVec0, const VVector4 & unitVec1 );

// Conditionally select between two 4-D vectors
// 
inline const VVector4 select( const VVector4 & vec0, const VVector4 & vec1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 4-D vector
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VVector4 & vec );

// Print a 4-D vector and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VVector4 & vec, const char * name );

#endif

// A 3-D point in array-of-structures format
//
class VPoint3
{
    float mX;
    float mY;
    float mZ;
#ifndef __GNUC__
    float d;
#endif

public:
    // Default constructor; does no initialization
    // 
    inline VPoint3( ) { };

    // Copy a 3-D point
    // 
    inline VPoint3( const VPoint3 & pnt );

    // Construct a 3-D point from x, y, and z elements
    // 
    inline VPoint3( float x, float y, float z );

    // Copy elements from a 3-D vector into a 3-D point
    // 
    explicit inline VPoint3( const VVector3 & vec );

    // Set all elements of a 3-D point to the same scalar value
    // 
    explicit inline VPoint3( float scalar );

    // Assign one 3-D point to another
    // 
    inline VPoint3 & operator =( const VPoint3 & pnt );

    // Set the x element of a 3-D point
    // 
    inline VPoint3 & setX( float x );

    // Set the y element of a 3-D point
    // 
    inline VPoint3 & setY( float y );

    // Set the z element of a 3-D point
    // 
    inline VPoint3 & setZ( float z );

    // Get the x element of a 3-D point
    // 
    inline float getX( ) const;

    // Get the y element of a 3-D point
    // 
    inline float getY( ) const;

    // Get the z element of a 3-D point
    // 
    inline float getZ( ) const;

    // Set an x, y, or z element of a 3-D point by index
    // 
    inline VPoint3 & setElem( int idx, float value );

    // Get an x, y, or z element of a 3-D point by index
    // 
    inline float getElem( int idx ) const;

    // Subscripting operator to set or get an element
    // 
    inline float & operator []( int idx );

    // Subscripting operator to get an element
    // 
    inline float operator []( int idx ) const;

    // Subtract a 3-D point from another 3-D point
    // 
    inline const VVector3 operator -( const VPoint3 & pnt ) const;

    // Add a 3-D point to a 3-D vector
    // 
    inline const VPoint3 operator +( const VVector3 & vec ) const;

    // Subtract a 3-D vector from a 3-D point
    // 
    inline const VPoint3 operator -( const VVector3 & vec ) const;

    // Perform compound assignment and addition with a 3-D vector
    // 
    inline VPoint3 & operator +=( const VVector3 & vec );

    // Perform compound assignment and subtraction by a 3-D vector
    // 
    inline VPoint3 & operator -=( const VVector3 & vec );

}
#ifdef __GNUC__
__attribute__ ((aligned(16)))
#endif
;

// Multiply two 3-D points per element
// 
inline const VPoint3 mulPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Divide two 3-D points per element
// NOTE: 
// Floating-point behavior matches standard library function divf4.
// 
inline const VPoint3 divPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Compute the reciprocal of a 3-D point per element
// NOTE: 
// Floating-point behavior matches standard library function recipf4.
// 
inline const VPoint3 recipPerElem( const VPoint3 & pnt );

// Compute the square root of a 3-D point per element
// NOTE: 
// Floating-point behavior matches standard library function sqrtf4.
// 
inline const VPoint3 sqrtPerElem( const VPoint3 & pnt );

// Compute the reciprocal square root of a 3-D point per element
// NOTE: 
// Floating-point behavior matches standard library function rsqrtf4.
// 
inline const VPoint3 rsqrtPerElem( const VPoint3 & pnt );

// Compute the absolute value of a 3-D point per element
// 
inline const VPoint3 absPerElem( const VPoint3 & pnt );

// Copy sign from one 3-D point to another, per element
// 
inline const VPoint3 copySignPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Maximum of two 3-D points per element
// 
inline const VPoint3 maxPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Minimum of two 3-D points per element
// 
inline const VPoint3 minPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Maximum element of a 3-D point
// 
inline float maxElem( const VPoint3 & pnt );

// Minimum element of a 3-D point
// 
inline float minElem( const VPoint3 & pnt );

// Compute the sum of all elements of a 3-D point
// 
inline float sum( const VPoint3 & pnt );

// Apply uniform scale to a 3-D point
// 
inline const VPoint3 scale( const VPoint3 & pnt, float scaleVal );

// Apply non-uniform scale to a 3-D point
// 
inline const VPoint3 scale( const VPoint3 & pnt, const VVector3 & scaleVec );

// Scalar projection of a 3-D point on a unit-length 3-D vector
// 
inline float projection( const VPoint3 & pnt, const VVector3 & unitVec );

// Compute the square of the distance of a 3-D point from the coordinate-system origin
// 
inline float distSqrFromOrigin( const VPoint3 & pnt );

// Compute the distance of a 3-D point from the coordinate-system origin
// 
inline float distFromOrigin( const VPoint3 & pnt );

// Compute the square of the distance between two 3-D points
// 
inline float distSqr( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Compute the distance between two 3-D points
// 
inline float dist( const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Linear interpolation between two 3-D points
// NOTE: 
// Does not clamp t between 0 and 1.
// 
inline const VPoint3 lerp( float t, const VPoint3 & pnt0, const VPoint3 & pnt1 );

// Conditionally select between two 3-D points
// 
inline const VPoint3 select( const VPoint3 & pnt0, const VPoint3 & pnt1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 3-D point
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VPoint3 & pnt );

// Print a 3-D point and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VPoint3 & pnt, const char * name );

#endif

// A VQuaternion in array-of-structures format
//
class VQuat
{
    float mX;
    float mY;
    float mZ;
    float mW;

public:
    // Default constructor; does no initialization
    // 
    inline VQuat( ) { };

    // Copy a VQuaternion
    // 
    inline VQuat( const VQuat & VQuat );

    // Construct a VQuaternion from x, y, z, and w elements
    // 
    inline VQuat( float x, float y, float z, float w );

    // Construct a VQuaternion from a 3-D vector and a scalar
    // 
    inline VQuat( const VVector3 & xyz, float w );

    // Copy elements from a 4-D vector into a VQuaternion
    // 
    explicit inline VQuat( const VVector4 & vec );

    // Convert a rotation matrix to a unit-length VQuaternion
    // 
    explicit inline VQuat( const VMatrix3 & rotMat );

    // Set all elements of a VQuaternion to the same scalar value
    // 
    explicit inline VQuat( float scalar );

    // Assign one VQuaternion to another
    // 
    inline VQuat & operator =( const VQuat & VQuat );

    // Set the x, y, and z elements of a VQuaternion
    // NOTE: 
    // This function does not change the w element.
    // 
    inline VQuat & setXYZ( const VVector3 & vec );

    // Get the x, y, and z elements of a VQuaternion
    // 
    inline const VVector3 getXYZ( ) const;

    // Set the x element of a VQuaternion
    // 
    inline VQuat & setX( float x );

    // Set the y element of a VQuaternion
    // 
    inline VQuat & setY( float y );

    // Set the z element of a VQuaternion
    // 
    inline VQuat & setZ( float z );

    // Set the w element of a VQuaternion
    // 
    inline VQuat & setW( float w );

    // Get the x element of a VQuaternion
    // 
    inline float getX( ) const;

    // Get the y element of a VQuaternion
    // 
    inline float getY( ) const;

    // Get the z element of a VQuaternion
    // 
    inline float getZ( ) const;

    // Get the w element of a VQuaternion
    // 
    inline float getW( ) const;

    // Set an x, y, z, or w element of a VQuaternion by index
    // 
    inline VQuat & setElem( int idx, float value );

    // Get an x, y, z, or w element of a VQuaternion by index
    // 
    inline float getElem( int idx ) const;

    // Subscripting operator to set or get an element
    // 
    inline float & operator []( int idx );

    // Subscripting operator to get an element
    // 
    inline float operator []( int idx ) const;

    // Add two VQuaternions
    // 
    inline const VQuat operator +( const VQuat & VQuat ) const;

    // Subtract a VQuaternion from another VQuaternion
    // 
    inline const VQuat operator -( const VQuat & VQuat ) const;

    // Multiply two VQuaternions
    // 
    inline const VQuat operator *( const VQuat & VQuat ) const;

    // Multiply a VQuaternion by a scalar
    // 
    inline const VQuat operator *( float scalar ) const;

    // Divide a VQuaternion by a scalar
    // 
    inline const VQuat operator /( float scalar ) const;

    // Perform compound assignment and addition with a VQuaternion
    // 
    inline VQuat & operator +=( const VQuat & VQuat );

    // Perform compound assignment and subtraction by a VQuaternion
    // 
    inline VQuat & operator -=( const VQuat & VQuat );

    // Perform compound assignment and multiplication by a VQuaternion
    // 
    inline VQuat & operator *=( const VQuat & VQuat );

    // Perform compound assignment and multiplication by a scalar
    // 
    inline VQuat & operator *=( float scalar );

    // Perform compound assignment and division by a scalar
    // 
    inline VQuat & operator /=( float scalar );

    // Negate all elements of a VQuaternion
    // 
    inline const VQuat operator -( ) const;

    // Construct an identity VQuaternion
    // 
    static inline const VQuat identity( );

    // Construct a VQuaternion to rotate between two unit-length 3-D vectors
    // NOTE: 
    // The result is unpredictable if unitVec0 and unitVec1 point in opposite directions.
    // 
    static inline const VQuat rotation( const VVector3 & unitVec0, const VVector3 & unitVec1 );

    // Construct a VQuaternion to rotate around a unit-length 3-D vector
    // 
    static inline const VQuat rotation( float radians, const VVector3 & unitVec );

    // Construct a VQuaternion to rotate around the x axis
    // 
    static inline const VQuat rotationX( float radians );

    // Construct a VQuaternion to rotate around the y axis
    // 
    static inline const VQuat rotationY( float radians );

    // Construct a VQuaternion to rotate around the z axis
    // 
    static inline const VQuat rotationZ( float radians );

}
#ifdef __GNUC__
__attribute__ ((aligned(16)))
#endif
;

// Multiply a VQuaternion by a scalar
// 
inline const VQuat operator *( float scalar, const VQuat & VQuat );

// Compute the conjugate of a VQuaternion
// 
inline const VQuat conj( const VQuat & VQuat );

// Use a unit-length VQuaternion to rotate a 3-D vector
// 
inline const VVector3 rotate( const VQuat & unitVQuat, const VVector3 & vec );

// Compute the dot product of two VQuaternions
// 
inline float dot( const VQuat & VQuat0, const VQuat & VQuat1 );

// Compute the norm of a VQuaternion
// 
inline float norm( const VQuat & VQuat );

// Compute the length of a VQuaternion
// 
inline float length( const VQuat & VQuat );

// Normalize a VQuaternion
// NOTE: 
// The result is unpredictable when all elements of VQuat are at or near zero.
// 
inline const VQuat normalize( const VQuat & VQuat );

// Linear interpolation between two VQuaternions
// NOTE: 
// Does not clamp t between 0 and 1.
// 
inline const VQuat lerp( float t, const VQuat & VQuat0, const VQuat & VQuat1 );

// Spherical linear interpolation between two VQuaternions
// NOTE: 
// Interpolates along the shortest path between orientations.
// Does not clamp t between 0 and 1.
// 
inline const VQuat slerp( float t, const VQuat & unitVQuat0, const VQuat & unitVQuat1 );

// Spherical quadrangle interpolation
// 
inline const VQuat squad( float t, const VQuat & unitVQuat0, const VQuat & unitVQuat1, const VQuat & unitVQuat2, const VQuat & unitVQuat3 );

// Conditionally select between two VQuaternions
// 
inline const VQuat select( const VQuat & VQuat0, const VQuat & VQuat1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a VQuaternion
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VQuat & VQuat );

// Print a VQuaternion and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VQuat & VQuat, const char * name );

#endif

// A 3x3 matrix in array-of-structures format
//
class VMatrix3
{
    VVector3 mCol0;
    VVector3 mCol1;
    VVector3 mCol2;

public:
    // Default constructor; does no initialization
    // 
    inline VMatrix3( ) { };

    // Copy a 3x3 matrix
    // 
    inline VMatrix3( const VMatrix3 & mat );

    // Construct a 3x3 matrix containing the specified columns
    // 
    inline VMatrix3( const VVector3 & col0, const VVector3 & col1, const VVector3 & col2 );

    // Construct a 3x3 rotation matrix from a unit-length VQuaternion
    // 
    explicit inline VMatrix3( const VQuat & unitVQuat );

    // Set all elements of a 3x3 matrix to the same scalar value
    // 
    explicit inline VMatrix3( float scalar );

    // Assign one 3x3 matrix to another
    // 
    inline VMatrix3 & operator =( const VMatrix3 & mat );

    // Set column 0 of a 3x3 matrix
    // 
    inline VMatrix3 & setCol0( const VVector3 & col0 );

    // Set column 1 of a 3x3 matrix
    // 
    inline VMatrix3 & setCol1( const VVector3 & col1 );

    // Set column 2 of a 3x3 matrix
    // 
    inline VMatrix3 & setCol2( const VVector3 & col2 );

    // Get column 0 of a 3x3 matrix
    // 
    inline const VVector3 getCol0( ) const;

    // Get column 1 of a 3x3 matrix
    // 
    inline const VVector3 getCol1( ) const;

    // Get column 2 of a 3x3 matrix
    // 
    inline const VVector3 getCol2( ) const;

    // Set the column of a 3x3 matrix referred to by the specified index
    // 
    inline VMatrix3 & setCol( int col, const VVector3 & vec );

    // Set the row of a 3x3 matrix referred to by the specified index
    // 
    inline VMatrix3 & setRow( int row, const VVector3 & vec );

    // Get the column of a 3x3 matrix referred to by the specified index
    // 
    inline const VVector3 getCol( int col ) const;

    // Get the row of a 3x3 matrix referred to by the specified index
    // 
    inline const VVector3 getRow( int row ) const;

    // Subscripting operator to set or get a column
    // 
    inline VVector3 & operator []( int col );

    // Subscripting operator to get a column
    // 
    inline const VVector3 operator []( int col ) const;

    // Set the element of a 3x3 matrix referred to by column and row indices
    // 
    inline VMatrix3 & setElem( int col, int row, float val );

    // Get the element of a 3x3 matrix referred to by column and row indices
    // 
    inline float getElem( int col, int row ) const;

    // Add two 3x3 matrices
    // 
    inline const VMatrix3 operator +( const VMatrix3 & mat ) const;

    // Subtract a 3x3 matrix from another 3x3 matrix
    // 
    inline const VMatrix3 operator -( const VMatrix3 & mat ) const;

    // Negate all elements of a 3x3 matrix
    // 
    inline const VMatrix3 operator -( ) const;

    // Multiply a 3x3 matrix by a scalar
    // 
    inline const VMatrix3 operator *( float scalar ) const;

    // Multiply a 3x3 matrix by a 3-D vector
    // 
    inline const VVector3 operator *( const VVector3 & vec ) const;

    // Multiply two 3x3 matrices
    // 
    inline const VMatrix3 operator *( const VMatrix3 & mat ) const;

    // Perform compound assignment and addition with a 3x3 matrix
    // 
    inline VMatrix3 & operator +=( const VMatrix3 & mat );

    // Perform compound assignment and subtraction by a 3x3 matrix
    // 
    inline VMatrix3 & operator -=( const VMatrix3 & mat );

    // Perform compound assignment and multiplication by a scalar
    // 
    inline VMatrix3 & operator *=( float scalar );

    // Perform compound assignment and multiplication by a 3x3 matrix
    // 
    inline VMatrix3 & operator *=( const VMatrix3 & mat );

    // Construct an identity 3x3 matrix
    // 
    static inline const VMatrix3 identity( );

    // Construct a 3x3 matrix to rotate around the x axis
    // 
    static inline const VMatrix3 rotationX( float radians );

    // Construct a 3x3 matrix to rotate around the y axis
    // 
    static inline const VMatrix3 rotationY( float radians );

    // Construct a 3x3 matrix to rotate around the z axis
    // 
    static inline const VMatrix3 rotationZ( float radians );

    // Construct a 3x3 matrix to rotate around the x, y, and z axes
    // 
    static inline const VMatrix3 rotationZYX( const VVector3 & radiansXYZ );

    // Construct a 3x3 matrix to rotate around a unit-length 3-D vector
    // 
    static inline const VMatrix3 rotation( float radians, const VVector3 & unitVec );

    // Construct a rotation matrix from a unit-length VQuaternion
    // 
    static inline const VMatrix3 rotation( const VQuat & unitVQuat );

    // Construct a 3x3 matrix to perform scaling
    // 
    static inline const VMatrix3 scale( const VVector3 & scaleVec );

};
// Multiply a 3x3 matrix by a scalar
// 
inline const VMatrix3 operator *( float scalar, const VMatrix3 & mat );

// Append (post-multiply) a scale transformation to a 3x3 matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VMatrix3 appendScale( const VMatrix3 & mat, const VVector3 & scaleVec );

// Prepend (pre-multiply) a scale transformation to a 3x3 matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VMatrix3 prependScale( const VVector3 & scaleVec, const VMatrix3 & mat );

// Multiply two 3x3 matrices per element
// 
inline const VMatrix3 mulPerElem( const VMatrix3 & mat0, const VMatrix3 & mat1 );

// Compute the absolute value of a 3x3 matrix per element
// 
inline const VMatrix3 absPerElem( const VMatrix3 & mat );

// Transpose of a 3x3 matrix
// 
inline const VMatrix3 transpose( const VMatrix3 & mat );

// Compute the inverse of a 3x3 matrix
// NOTE: 
// Result is unpredictable when the determinant of mat is equal to or near 0.
// 
inline const VMatrix3 inverse( const VMatrix3 & mat );

// Determinant of a 3x3 matrix
// 
inline float determinant( const VMatrix3 & mat );

// Conditionally select between two 3x3 matrices
// 
inline const VMatrix3 select( const VMatrix3 & mat0, const VMatrix3 & mat1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 3x3 matrix
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VMatrix3 & mat );

// Print a 3x3 matrix and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VMatrix3 & mat, const char * name );

#endif

// A 4x4 matrix in array-of-structures format
//
class VMatrix4
{
    VVector4 mCol0;
    VVector4 mCol1;
    VVector4 mCol2;
    VVector4 mCol3;

public:
    // Default constructor; does no initialization
    // 
    inline VMatrix4( ) { };

    // Copy a 4x4 matrix
    // 
    inline VMatrix4( const VMatrix4 & mat );

    // Construct a 4x4 matrix containing the specified columns
    // 
    inline VMatrix4( const VVector4 & col0, const VVector4 & col1, const VVector4 & col2, const VVector4 & col3 );

    // Construct a 4x4 matrix from a 3x4 transformation matrix
    // 
    explicit inline VMatrix4( const VTransform3 & mat );

    // Construct a 4x4 matrix from a 3x3 matrix and a 3-D vector
    // 
    inline VMatrix4( const VMatrix3 & mat, const VVector3 & translateVec );

    // Construct a 4x4 matrix from a unit-length VQuaternion and a 3-D vector
    // 
    inline VMatrix4( const VQuat & unitVQuat, const VVector3 & translateVec );

    // Set all elements of a 4x4 matrix to the same scalar value
    // 
    explicit inline VMatrix4( float scalar );

    // Assign one 4x4 matrix to another
    // 
    inline VMatrix4 & operator =( const VMatrix4 & mat );

    // Set the upper-left 3x3 submatrix
    // NOTE: 
    // This function does not change the bottom row elements.
    // 
    inline VMatrix4 & setUpper3x3( const VMatrix3 & mat3 );

    // Get the upper-left 3x3 submatrix of a 4x4 matrix
    // 
    inline const VMatrix3 getUpper3x3( ) const;

    // Set translation component
    // NOTE: 
    // This function does not change the bottom row elements.
    // 
    inline VMatrix4 & setTranslation( const VVector3 & translateVec );

    // Get the translation component of a 4x4 matrix
    // 
    inline const VVector3 getTranslation( ) const;

    // Set column 0 of a 4x4 matrix
    // 
    inline VMatrix4 & setCol0( const VVector4 & col0 );

    // Set column 1 of a 4x4 matrix
    // 
    inline VMatrix4 & setCol1( const VVector4 & col1 );

    // Set column 2 of a 4x4 matrix
    // 
    inline VMatrix4 & setCol2( const VVector4 & col2 );

    // Set column 3 of a 4x4 matrix
    // 
    inline VMatrix4 & setCol3( const VVector4 & col3 );

    // Get column 0 of a 4x4 matrix
    // 
    inline const VVector4 getCol0( ) const;

    // Get column 1 of a 4x4 matrix
    // 
    inline const VVector4 getCol1( ) const;

    // Get column 2 of a 4x4 matrix
    // 
    inline const VVector4 getCol2( ) const;

    // Get column 3 of a 4x4 matrix
    // 
    inline const VVector4 getCol3( ) const;

    // Set the column of a 4x4 matrix referred to by the specified index
    // 
    inline VMatrix4 & setCol( int col, const VVector4 & vec );

    // Set the row of a 4x4 matrix referred to by the specified index
    // 
    inline VMatrix4 & setRow( int row, const VVector4 & vec );

    // Get the column of a 4x4 matrix referred to by the specified index
    // 
    inline const VVector4 getCol( int col ) const;

    // Get the row of a 4x4 matrix referred to by the specified index
    // 
    inline const VVector4 getRow( int row ) const;

    // Subscripting operator to set or get a column
    // 
    inline VVector4 & operator []( int col );

    // Subscripting operator to get a column
    // 
    inline const VVector4 operator []( int col ) const;

    // Set the element of a 4x4 matrix referred to by column and row indices
    // 
    inline VMatrix4 & setElem( int col, int row, float val );

    // Get the element of a 4x4 matrix referred to by column and row indices
    // 
    inline float getElem( int col, int row ) const;

    // Add two 4x4 matrices
    // 
    inline const VMatrix4 operator +( const VMatrix4 & mat ) const;

    // Subtract a 4x4 matrix from another 4x4 matrix
    // 
    inline const VMatrix4 operator -( const VMatrix4 & mat ) const;

    // Negate all elements of a 4x4 matrix
    // 
    inline const VMatrix4 operator -( ) const;

    // Multiply a 4x4 matrix by a scalar
    // 
    inline const VMatrix4 operator *( float scalar ) const;

    // Multiply a 4x4 matrix by a 4-D vector
    // 
    inline const VVector4 operator *( const VVector4 & vec ) const;

    // Multiply a 4x4 matrix by a 3-D vector
    // 
    inline const VVector4 operator *( const VVector3 & vec ) const;

    // Multiply a 4x4 matrix by a 3-D point
    // 
    inline const VVector4 operator *( const VPoint3 & pnt ) const;

    // Multiply two 4x4 matrices
    // 
    inline const VMatrix4 operator *( const VMatrix4 & mat ) const;

    // Multiply a 4x4 matrix by a 3x4 transformation matrix
    // 
    inline const VMatrix4 operator *( const VTransform3 & tfrm ) const;

    // Perform compound assignment and addition with a 4x4 matrix
    // 
    inline VMatrix4 & operator +=( const VMatrix4 & mat );

    // Perform compound assignment and subtraction by a 4x4 matrix
    // 
    inline VMatrix4 & operator -=( const VMatrix4 & mat );

    // Perform compound assignment and multiplication by a scalar
    // 
    inline VMatrix4 & operator *=( float scalar );

    // Perform compound assignment and multiplication by a 4x4 matrix
    // 
    inline VMatrix4 & operator *=( const VMatrix4 & mat );

    // Perform compound assignment and multiplication by a 3x4 transformation matrix
    // 
    inline VMatrix4 & operator *=( const VTransform3 & tfrm );

    // Construct an identity 4x4 matrix
    // 
    static inline const VMatrix4 identity( );

    // Construct a 4x4 matrix to rotate around the x axis
    // 
    static inline const VMatrix4 rotationX( float radians );

    // Construct a 4x4 matrix to rotate around the y axis
    // 
    static inline const VMatrix4 rotationY( float radians );

    // Construct a 4x4 matrix to rotate around the z axis
    // 
    static inline const VMatrix4 rotationZ( float radians );

    // Construct a 4x4 matrix to rotate around the x, y, and z axes
    // 
    static inline const VMatrix4 rotationZYX( const VVector3 & radiansXYZ );

    // Construct a 4x4 matrix to rotate around a unit-length 3-D vector
    // 
    static inline const VMatrix4 rotation( float radians, const VVector3 & unitVec );

    // Construct a rotation matrix from a unit-length VQuaternion
    // 
    static inline const VMatrix4 rotation( const VQuat & unitVQuat );

    // Construct a 4x4 matrix to perform scaling
    // 
    static inline const VMatrix4 scale( const VVector3 & scaleVec );

    // Construct a 4x4 matrix to perform translation
    // 
    static inline const VMatrix4 translation( const VVector3 & translateVec );

    // Construct viewing matrix based on eye position, position looked at, and up direction
    // 
    static inline const VMatrix4 lookAt( const VPoint3 & eyePos, const VPoint3 & lookAtPos, const VVector3 & upVec );

    // Construct a perspective projection matrix
    // 
    static inline const VMatrix4 perspective( float fovyRadians, float aspect, float zNear, float zFar );

    // Construct a perspective projection matrix based on frustum
    // 
    static inline const VMatrix4 frustum( float left, float right, float bottom, float top, float zNear, float zFar );

    // Construct an orthographic projection matrix
    // 
    static inline const VMatrix4 orthographic( float left, float right, float bottom, float top, float zNear, float zFar );

};
// Multiply a 4x4 matrix by a scalar
// 
inline const VMatrix4 operator *( float scalar, const VMatrix4 & mat );

// Append (post-multiply) a scale transformation to a 4x4 matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VMatrix4 appendScale( const VMatrix4 & mat, const VVector3 & scaleVec );

// Prepend (pre-multiply) a scale transformation to a 4x4 matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VMatrix4 prependScale( const VVector3 & scaleVec, const VMatrix4 & mat );

// Multiply two 4x4 matrices per element
// 
inline const VMatrix4 mulPerElem( const VMatrix4 & mat0, const VMatrix4 & mat1 );

// Compute the absolute value of a 4x4 matrix per element
// 
inline const VMatrix4 absPerElem( const VMatrix4 & mat );

// Transpose of a 4x4 matrix
// 
inline const VMatrix4 transpose( const VMatrix4 & mat );

// Compute the inverse of a 4x4 matrix
// NOTE: 
// Result is unpredictable when the determinant of mat is equal to or near 0.
// 
inline const VMatrix4 inverse( const VMatrix4 & mat );

// Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix
// NOTE: 
// This can be used to achieve better performance than a general inverse when the specified 4x4 matrix meets the given restrictions.  The result is unpredictable when the determinant of mat is equal to or near 0.
// 
inline const VMatrix4 affineInverse( const VMatrix4 & mat );

// Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix with an orthogonal upper-left 3x3 submatrix
// NOTE: 
// This can be used to achieve better performance than a general inverse when the specified 4x4 matrix meets the given restrictions.
// 
inline const VMatrix4 orthoInverse( const VMatrix4 & mat );

// Determinant of a 4x4 matrix
// 
inline float determinant( const VMatrix4 & mat );

// Conditionally select between two 4x4 matrices
// 
inline const VMatrix4 select( const VMatrix4 & mat0, const VMatrix4 & mat1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 4x4 matrix
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VMatrix4 & mat );

// Print a 4x4 matrix and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VMatrix4 & mat, const char * name );

#endif

// A 3x4 transformation matrix in array-of-structures format
//
class VTransform3
{
    VVector3 mCol0;
    VVector3 mCol1;
    VVector3 mCol2;
    VVector3 mCol3;

public:
    // Default constructor; does no initialization
    // 
    inline VTransform3( ) { };

    // Copy a 3x4 transformation matrix
    // 
    inline VTransform3( const VTransform3 & tfrm );

    // Construct a 3x4 transformation matrix containing the specified columns
    // 
    inline VTransform3( const VVector3 & col0, const VVector3 & col1, const VVector3 & col2, const VVector3 & col3 );

    // Construct a 3x4 transformation matrix from a 3x3 matrix and a 3-D vector
    // 
    inline VTransform3( const VMatrix3 & tfrm, const VVector3 & translateVec );

    // Construct a 3x4 transformation matrix from a unit-length VQuaternion and a 3-D vector
    // 
    inline VTransform3( const VQuat & unitVQuat, const VVector3 & translateVec );

    // Set all elements of a 3x4 transformation matrix to the same scalar value
    // 
    explicit inline VTransform3( float scalar );

    // Assign one 3x4 transformation matrix to another
    // 
    inline VTransform3 & operator =( const VTransform3 & tfrm );

    // Set the upper-left 3x3 submatrix
    // 
    inline VTransform3 & setUpper3x3( const VMatrix3 & mat3 );

    // Get the upper-left 3x3 submatrix of a 3x4 transformation matrix
    // 
    inline const VMatrix3 getUpper3x3( ) const;

    // Set translation component
    // 
    inline VTransform3 & setTranslation( const VVector3 & translateVec );

    // Get the translation component of a 3x4 transformation matrix
    // 
    inline const VVector3 getTranslation( ) const;

    // Set column 0 of a 3x4 transformation matrix
    // 
    inline VTransform3 & setCol0( const VVector3 & col0 );

    // Set column 1 of a 3x4 transformation matrix
    // 
    inline VTransform3 & setCol1( const VVector3 & col1 );

    // Set column 2 of a 3x4 transformation matrix
    // 
    inline VTransform3 & setCol2( const VVector3 & col2 );

    // Set column 3 of a 3x4 transformation matrix
    // 
    inline VTransform3 & setCol3( const VVector3 & col3 );

    // Get column 0 of a 3x4 transformation matrix
    // 
    inline const VVector3 getCol0( ) const;

    // Get column 1 of a 3x4 transformation matrix
    // 
    inline const VVector3 getCol1( ) const;

    // Get column 2 of a 3x4 transformation matrix
    // 
    inline const VVector3 getCol2( ) const;

    // Get column 3 of a 3x4 transformation matrix
    // 
    inline const VVector3 getCol3( ) const;

    // Set the column of a 3x4 transformation matrix referred to by the specified index
    // 
    inline VTransform3 & setCol( int col, const VVector3 & vec );

    // Set the row of a 3x4 transformation matrix referred to by the specified index
    // 
    inline VTransform3 & setRow( int row, const VVector4 & vec );

    // Get the column of a 3x4 transformation matrix referred to by the specified index
    // 
    inline const VVector3 getCol( int col ) const;

    // Get the row of a 3x4 transformation matrix referred to by the specified index
    // 
    inline const VVector4 getRow( int row ) const;

    // Subscripting operator to set or get a column
    // 
    inline VVector3 & operator []( int col );

    // Subscripting operator to get a column
    // 
    inline const VVector3 operator []( int col ) const;

    // Set the element of a 3x4 transformation matrix referred to by column and row indices
    // 
    inline VTransform3 & setElem( int col, int row, float val );

    // Get the element of a 3x4 transformation matrix referred to by column and row indices
    // 
    inline float getElem( int col, int row ) const;

    // Multiply a 3x4 transformation matrix by a 3-D vector
    // 
    inline const VVector3 operator *( const VVector3 & vec ) const;

    // Multiply a 3x4 transformation matrix by a 3-D point
    // 
    inline const VPoint3 operator *( const VPoint3 & pnt ) const;

    // Multiply two 3x4 transformation matrices
    // 
    inline const VTransform3 operator *( const VTransform3 & tfrm ) const;

    // Perform compound assignment and multiplication by a 3x4 transformation matrix
    // 
    inline VTransform3 & operator *=( const VTransform3 & tfrm );

    // Construct an identity 3x4 transformation matrix
    // 
    static inline const VTransform3 identity( );

    // Construct a 3x4 transformation matrix to rotate around the x axis
    // 
    static inline const VTransform3 rotationX( float radians );

    // Construct a 3x4 transformation matrix to rotate around the y axis
    // 
    static inline const VTransform3 rotationY( float radians );

    // Construct a 3x4 transformation matrix to rotate around the z axis
    // 
    static inline const VTransform3 rotationZ( float radians );

    // Construct a 3x4 transformation matrix to rotate around the x, y, and z axes
    // 
    static inline const VTransform3 rotationZYX( const VVector3 & radiansXYZ );

    // Construct a 3x4 transformation matrix to rotate around a unit-length 3-D vector
    // 
    static inline const VTransform3 rotation( float radians, const VVector3 & unitVec );

    // Construct a rotation matrix from a unit-length VQuaternion
    // 
    static inline const VTransform3 rotation( const VQuat & unitVQuat );

    // Construct a 3x4 transformation matrix to perform scaling
    // 
    static inline const VTransform3 scale( const VVector3 & scaleVec );

    // Construct a 3x4 transformation matrix to perform translation
    // 
    static inline const VTransform3 translation( const VVector3 & translateVec );

};
// Append (post-multiply) a scale transformation to a 3x4 transformation matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VTransform3 appendScale( const VTransform3 & tfrm, const VVector3 & scaleVec );

// Prepend (pre-multiply) a scale transformation to a 3x4 transformation matrix
// NOTE: 
// Faster than creating and multiplying a scale transformation matrix.
// 
inline const VTransform3 prependScale( const VVector3 & scaleVec, const VTransform3 & tfrm );

// Multiply two 3x4 transformation matrices per element
// 
inline const VTransform3 mulPerElem( const VTransform3 & tfrm0, const VTransform3 & tfrm1 );

// Compute the absolute value of a 3x4 transformation matrix per element
// 
inline const VTransform3 absPerElem( const VTransform3 & tfrm );

// Inverse of a 3x4 transformation matrix
// NOTE: 
// Result is unpredictable when the determinant of the left 3x3 submatrix is equal to or near 0.
// 
inline const VTransform3 inverse( const VTransform3 & tfrm );

// Compute the inverse of a 3x4 transformation matrix, expected to have an orthogonal upper-left 3x3 submatrix
// NOTE: 
// This can be used to achieve better performance than a general inverse when the specified 3x4 transformation matrix meets the given restrictions.
// 
inline const VTransform3 orthoInverse( const VTransform3 & tfrm );

// Conditionally select between two 3x4 transformation matrices
// 
inline const VTransform3 select( const VTransform3 & tfrm0, const VTransform3 & tfrm1, bool select1 );

#ifdef _VECTORMATH_DEBUG

// Print a 3x4 transformation matrix
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VTransform3 & tfrm );

// Print a 3x4 transformation matrix and an associated string identifier
// NOTE: 
// Function is only defined when _VECTORMATH_DEBUG is defined.
// 
inline void print( const VTransform3 & tfrm, const char * name );

#endif

} // namespace Aos
} // namespace Vectormath

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _VECTORMATH_VEC_AOS_CPP_H
#define _VECTORMATH_VEC_AOS_CPP_H
//-----------------------------------------------------------------------------
// Constants

#define _VECTORMATH_SLERP_TOL 0.999f

//-----------------------------------------------------------------------------
// Definitions

#ifndef _VECTORMATH_INTERNAL_FUNCTIONS
#define _VECTORMATH_INTERNAL_FUNCTIONS

#endif

namespace Vectormath {
namespace Aos {

inline VVector3::VVector3( const VVector3 & vec )
{
    mX = vec.mX;
    mY = vec.mY;
    mZ = vec.mZ;
}

inline VVector3::VVector3( float _x, float _y, float _z )
{
    mX = _x;
    mY = _y;
    mZ = _z;
}

inline VVector3::VVector3( const VPoint3 & pnt )
{
    mX = pnt.getX();
    mY = pnt.getY();
    mZ = pnt.getZ();
}

inline VVector3::VVector3( float scalar )
{
    mX = scalar;
    mY = scalar;
    mZ = scalar;
}

inline const VVector3 VVector3::xAxis( )
{
    return VVector3( 1.0f, 0.0f, 0.0f );
}

inline const VVector3 VVector3::yAxis( )
{
    return VVector3( 0.0f, 1.0f, 0.0f );
}

inline const VVector3 VVector3::zAxis( )
{
    return VVector3( 0.0f, 0.0f, 1.0f );
}

inline const VVector3 lerp( float t, const VVector3 & vec0, const VVector3 & vec1 )
{
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const VVector3 slerp( float t, const VVector3 & unitVec0, const VVector3 & unitVec1 )
{
    float recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dot( unitVec0, unitVec1 );
    if ( cosAngle < _VECTORMATH_SLERP_TOL ) {
        angle = acosf( cosAngle );
        recipSinAngle = ( 1.0f / sinf( angle ) );
        scale0 = ( sinf( ( ( 1.0f - t ) * angle ) ) * recipSinAngle );
        scale1 = ( sinf( ( t * angle ) ) * recipSinAngle );
    } else {
        scale0 = ( 1.0f - t );
        scale1 = t;
    }
    return ( ( unitVec0 * scale0 ) + ( unitVec1 * scale1 ) );
}

inline VVector3 & VVector3::operator =( const VVector3 & vec )
{
    mX = vec.mX;
    mY = vec.mY;
    mZ = vec.mZ;
    return *this;
}

inline VVector3 & VVector3::setX( float _x )
{
    mX = _x;
    return *this;
}

inline float VVector3::getX( ) const
{
    return mX;
}

inline VVector3 & VVector3::setY( float _y )
{
    mY = _y;
    return *this;
}

inline float VVector3::getY( ) const
{
    return mY;
}

inline VVector3 & VVector3::setZ( float _z )
{
    mZ = _z;
    return *this;
}

inline float VVector3::getZ( ) const
{
    return mZ;
}

inline VVector3 & VVector3::setElem( int idx, float value )
{
    *(&mX + idx) = value;
    return *this;
}

inline float VVector3::getElem( int idx ) const
{
    return *(&mX + idx);
}

inline float & VVector3::operator []( int idx )
{
    return *(&mX + idx);
}

inline float VVector3::operator []( int idx ) const
{
    return *(&mX + idx);
}

inline const VVector3 VVector3::operator +( const VVector3 & vec ) const
{
    return VVector3(
        ( mX + vec.mX ),
        ( mY + vec.mY ),
        ( mZ + vec.mZ )
    );
}

inline const VVector3 VVector3::operator -( const VVector3 & vec ) const
{
    return VVector3(
        ( mX - vec.mX ),
        ( mY - vec.mY ),
        ( mZ - vec.mZ )
    );
}

inline const VPoint3 VVector3::operator +( const VPoint3 & pnt ) const
{
    return VPoint3(
        ( mX + pnt.getX() ),
        ( mY + pnt.getY() ),
        ( mZ + pnt.getZ() )
    );
}

inline const VVector3 VVector3::operator *( float scalar ) const
{
    return VVector3(
        ( mX * scalar ),
        ( mY * scalar ),
        ( mZ * scalar )
    );
}

inline VVector3 & VVector3::operator +=( const VVector3 & vec )
{
    *this = *this + vec;
    return *this;
}

inline VVector3 & VVector3::operator -=( const VVector3 & vec )
{
    *this = *this - vec;
    return *this;
}

inline VVector3 & VVector3::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const VVector3 VVector3::operator /( float scalar ) const
{
    return VVector3(
        ( mX / scalar ),
        ( mY / scalar ),
        ( mZ / scalar )
    );
}

inline VVector3 & VVector3::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const VVector3 VVector3::operator -( ) const
{
    return VVector3(
        -mX,
        -mY,
        -mZ
    );
}

inline const VVector3 operator *( float scalar, const VVector3 & vec )
{
    return vec * scalar;
}

inline const VVector3 mulPerElem( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        ( vec0.getX() * vec1.getX() ),
        ( vec0.getY() * vec1.getY() ),
        ( vec0.getZ() * vec1.getZ() )
    );
}

inline const VVector3 divPerElem( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        ( vec0.getX() / vec1.getX() ),
        ( vec0.getY() / vec1.getY() ),
        ( vec0.getZ() / vec1.getZ() )
    );
}

inline const VVector3 recipPerElem( const VVector3 & vec )
{
    return VVector3(
        ( 1.0f / vec.getX() ),
        ( 1.0f / vec.getY() ),
        ( 1.0f / vec.getZ() )
    );
}

inline const VVector3 sqrtPerElem( const VVector3 & vec )
{
    return VVector3(
        sqrtf( vec.getX() ),
        sqrtf( vec.getY() ),
        sqrtf( vec.getZ() )
    );
}

inline const VVector3 rsqrtPerElem( const VVector3 & vec )
{
    return VVector3(
        ( 1.0f / sqrtf( vec.getX() ) ),
        ( 1.0f / sqrtf( vec.getY() ) ),
        ( 1.0f / sqrtf( vec.getZ() ) )
    );
}

inline const VVector3 absPerElem( const VVector3 & vec )
{
    return VVector3(
        fabsf( vec.getX() ),
        fabsf( vec.getY() ),
        fabsf( vec.getZ() )
    );
}

inline const VVector3 copySignPerElem( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        ( vec1.getX() < 0.0f )? -fabsf( vec0.getX() ) : fabsf( vec0.getX() ),
        ( vec1.getY() < 0.0f )? -fabsf( vec0.getY() ) : fabsf( vec0.getY() ),
        ( vec1.getZ() < 0.0f )? -fabsf( vec0.getZ() ) : fabsf( vec0.getZ() )
    );
}

inline const VVector3 maxPerElem( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        (vec0.getX() > vec1.getX())? vec0.getX() : vec1.getX(),
        (vec0.getY() > vec1.getY())? vec0.getY() : vec1.getY(),
        (vec0.getZ() > vec1.getZ())? vec0.getZ() : vec1.getZ()
    );
}

inline float maxElem( const VVector3 & vec )
{
    float result;
    result = (vec.getX() > vec.getY())? vec.getX() : vec.getY();
    result = (vec.getZ() > result)? vec.getZ() : result;
    return result;
}

inline const VVector3 minPerElem( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        (vec0.getX() < vec1.getX())? vec0.getX() : vec1.getX(),
        (vec0.getY() < vec1.getY())? vec0.getY() : vec1.getY(),
        (vec0.getZ() < vec1.getZ())? vec0.getZ() : vec1.getZ()
    );
}

inline float minElem( const VVector3 & vec )
{
    float result;
    result = (vec.getX() < vec.getY())? vec.getX() : vec.getY();
    result = (vec.getZ() < result)? vec.getZ() : result;
    return result;
}

inline float sum( const VVector3 & vec )
{
    float result;
    result = ( vec.getX() + vec.getY() );
    result = ( result + vec.getZ() );
    return result;
}

inline float dot( const VVector3 & vec0, const VVector3 & vec1 )
{
    float result;
    result = ( vec0.getX() * vec1.getX() );
    result = ( result + ( vec0.getY() * vec1.getY() ) );
    result = ( result + ( vec0.getZ() * vec1.getZ() ) );
    return result;
}

inline float lengthSqr( const VVector3 & vec )
{
    float result;
    result = ( vec.getX() * vec.getX() );
    result = ( result + ( vec.getY() * vec.getY() ) );
    result = ( result + ( vec.getZ() * vec.getZ() ) );
    return result;
}

inline float length( const VVector3 & vec )
{
    return sqrtf( lengthSqr( vec ) );
}

inline const VVector3 normalize( const VVector3 & vec )
{
    float lenSqr, lenInv;
    lenSqr = lengthSqr( vec );
    lenInv = ( 1.0f / sqrtf( lenSqr ) );
    return VVector3(
        ( vec.getX() * lenInv ),
        ( vec.getY() * lenInv ),
        ( vec.getZ() * lenInv )
    );
}

inline const VVector3 cross( const VVector3 & vec0, const VVector3 & vec1 )
{
    return VVector3(
        ( ( vec0.getY() * vec1.getZ() ) - ( vec0.getZ() * vec1.getY() ) ),
        ( ( vec0.getZ() * vec1.getX() ) - ( vec0.getX() * vec1.getZ() ) ),
        ( ( vec0.getX() * vec1.getY() ) - ( vec0.getY() * vec1.getX() ) )
    );
}

inline const VVector3 select( const VVector3 & vec0, const VVector3 & vec1, bool select1 )
{
    return VVector3(
        ( select1 )? vec1.getX() : vec0.getX(),
        ( select1 )? vec1.getY() : vec0.getY(),
        ( select1 )? vec1.getZ() : vec0.getZ()
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VVector3 & vec )
{
    printf( "( %f %f %f )\n", vec.getX(), vec.getY(), vec.getZ() );
}

inline void print( const VVector3 & vec, const char * name )
{
    printf( "%s: ( %f %f %f )\n", name, vec.getX(), vec.getY(), vec.getZ() );
}

#endif

inline VVector4::VVector4( const VVector4 & vec )
{
    mX = vec.mX;
    mY = vec.mY;
    mZ = vec.mZ;
    mW = vec.mW;
}

inline VVector4::VVector4( float _x, float _y, float _z, float _w )
{
    mX = _x;
    mY = _y;
    mZ = _z;
    mW = _w;
}

inline VVector4::VVector4( const VVector3 & xyz, float _w )
{
    this->setXYZ( xyz );
    this->setW( _w );
}

inline VVector4::VVector4( const VVector3 & vec )
{
    mX = vec.getX();
    mY = vec.getY();
    mZ = vec.getZ();
    mW = 0.0f;
}

inline VVector4::VVector4( const VPoint3 & pnt )
{
    mX = pnt.getX();
    mY = pnt.getY();
    mZ = pnt.getZ();
    mW = 1.0f;
}

inline VVector4::VVector4( const VQuat & VQuat )
{
    mX = VQuat.getX();
    mY = VQuat.getY();
    mZ = VQuat.getZ();
    mW = VQuat.getW();
}

inline VVector4::VVector4( float scalar )
{
    mX = scalar;
    mY = scalar;
    mZ = scalar;
    mW = scalar;
}

inline const VVector4 VVector4::xAxis( )
{
    return VVector4( 1.0f, 0.0f, 0.0f, 0.0f );
}

inline const VVector4 VVector4::yAxis( )
{
    return VVector4( 0.0f, 1.0f, 0.0f, 0.0f );
}

inline const VVector4 VVector4::zAxis( )
{
    return VVector4( 0.0f, 0.0f, 1.0f, 0.0f );
}

inline const VVector4 VVector4::wAxis( )
{
    return VVector4( 0.0f, 0.0f, 0.0f, 1.0f );
}

inline const VVector4 lerp( float t, const VVector4 & vec0, const VVector4 & vec1 )
{
    return ( vec0 + ( ( vec1 - vec0 ) * t ) );
}

inline const VVector4 slerp( float t, const VVector4 & unitVec0, const VVector4 & unitVec1 )
{
    float recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dot( unitVec0, unitVec1 );
    if ( cosAngle < _VECTORMATH_SLERP_TOL ) {
        angle = acosf( cosAngle );
        recipSinAngle = ( 1.0f / sinf( angle ) );
        scale0 = ( sinf( ( ( 1.0f - t ) * angle ) ) * recipSinAngle );
        scale1 = ( sinf( ( t * angle ) ) * recipSinAngle );
    } else {
        scale0 = ( 1.0f - t );
        scale1 = t;
    }
    return ( ( unitVec0 * scale0 ) + ( unitVec1 * scale1 ) );
}

inline VVector4 & VVector4::operator =( const VVector4 & vec )
{
    mX = vec.mX;
    mY = vec.mY;
    mZ = vec.mZ;
    mW = vec.mW;
    return *this;
}

inline VVector4 & VVector4::setXYZ( const VVector3 & vec )
{
    mX = vec.getX();
    mY = vec.getY();
    mZ = vec.getZ();
    return *this;
}

inline const VVector3 VVector4::getXYZ( ) const
{
    return VVector3( mX, mY, mZ );
}

inline VVector4 & VVector4::setX( float _x )
{
    mX = _x;
    return *this;
}

inline float VVector4::getX( ) const
{
    return mX;
}

inline VVector4 & VVector4::setY( float _y )
{
    mY = _y;
    return *this;
}

inline float VVector4::getY( ) const
{
    return mY;
}

inline VVector4 & VVector4::setZ( float _z )
{
    mZ = _z;
    return *this;
}

inline float VVector4::getZ( ) const
{
    return mZ;
}

inline VVector4 & VVector4::setW( float _w )
{
    mW = _w;
    return *this;
}

inline float VVector4::getW( ) const
{
    return mW;
}

inline VVector4 & VVector4::setElem( int idx, float value )
{
    *(&mX + idx) = value;
    return *this;
}

inline float VVector4::getElem( int idx ) const
{
    return *(&mX + idx);
}

inline float & VVector4::operator []( int idx )
{
    return *(&mX + idx);
}

inline float VVector4::operator []( int idx ) const
{
    return *(&mX + idx);
}

inline const VVector4 VVector4::operator +( const VVector4 & vec ) const
{
    return VVector4(
        ( mX + vec.mX ),
        ( mY + vec.mY ),
        ( mZ + vec.mZ ),
        ( mW + vec.mW )
    );
}

inline const VVector4 VVector4::operator -( const VVector4 & vec ) const
{
    return VVector4(
        ( mX - vec.mX ),
        ( mY - vec.mY ),
        ( mZ - vec.mZ ),
        ( mW - vec.mW )
    );
}

inline const VVector4 VVector4::operator *( float scalar ) const
{
    return VVector4(
        ( mX * scalar ),
        ( mY * scalar ),
        ( mZ * scalar ),
        ( mW * scalar )
    );
}

inline VVector4 & VVector4::operator +=( const VVector4 & vec )
{
    *this = *this + vec;
    return *this;
}

inline VVector4 & VVector4::operator -=( const VVector4 & vec )
{
    *this = *this - vec;
    return *this;
}

inline VVector4 & VVector4::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const VVector4 VVector4::operator /( float scalar ) const
{
    return VVector4(
        ( mX / scalar ),
        ( mY / scalar ),
        ( mZ / scalar ),
        ( mW / scalar )
    );
}

inline VVector4 & VVector4::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const VVector4 VVector4::operator -( ) const
{
    return VVector4(
        -mX,
        -mY,
        -mZ,
        -mW
    );
}

inline const VVector4 operator *( float scalar, const VVector4 & vec )
{
    return vec * scalar;
}

inline const VVector4 mulPerElem( const VVector4 & vec0, const VVector4 & vec1 )
{
    return VVector4(
        ( vec0.getX() * vec1.getX() ),
        ( vec0.getY() * vec1.getY() ),
        ( vec0.getZ() * vec1.getZ() ),
        ( vec0.getW() * vec1.getW() )
    );
}

inline const VVector4 divPerElem( const VVector4 & vec0, const VVector4 & vec1 )
{
    return VVector4(
        ( vec0.getX() / vec1.getX() ),
        ( vec0.getY() / vec1.getY() ),
        ( vec0.getZ() / vec1.getZ() ),
        ( vec0.getW() / vec1.getW() )
    );
}

inline const VVector4 recipPerElem( const VVector4 & vec )
{
    return VVector4(
        ( 1.0f / vec.getX() ),
        ( 1.0f / vec.getY() ),
        ( 1.0f / vec.getZ() ),
        ( 1.0f / vec.getW() )
    );
}

inline const VVector4 sqrtPerElem( const VVector4 & vec )
{
    return VVector4(
        sqrtf( vec.getX() ),
        sqrtf( vec.getY() ),
        sqrtf( vec.getZ() ),
        sqrtf( vec.getW() )
    );
}

inline const VVector4 rsqrtPerElem( const VVector4 & vec )
{
    return VVector4(
        ( 1.0f / sqrtf( vec.getX() ) ),
        ( 1.0f / sqrtf( vec.getY() ) ),
        ( 1.0f / sqrtf( vec.getZ() ) ),
        ( 1.0f / sqrtf( vec.getW() ) )
    );
}

inline const VVector4 absPerElem( const VVector4 & vec )
{
    return VVector4(
        fabsf( vec.getX() ),
        fabsf( vec.getY() ),
        fabsf( vec.getZ() ),
        fabsf( vec.getW() )
    );
}

inline const VVector4 copySignPerElem( const VVector4 & vec0, const VVector4 & vec1 )
{
    return VVector4(
        ( vec1.getX() < 0.0f )? -fabsf( vec0.getX() ) : fabsf( vec0.getX() ),
        ( vec1.getY() < 0.0f )? -fabsf( vec0.getY() ) : fabsf( vec0.getY() ),
        ( vec1.getZ() < 0.0f )? -fabsf( vec0.getZ() ) : fabsf( vec0.getZ() ),
        ( vec1.getW() < 0.0f )? -fabsf( vec0.getW() ) : fabsf( vec0.getW() )
    );
}

inline const VVector4 maxPerElem( const VVector4 & vec0, const VVector4 & vec1 )
{
    return VVector4(
        (vec0.getX() > vec1.getX())? vec0.getX() : vec1.getX(),
        (vec0.getY() > vec1.getY())? vec0.getY() : vec1.getY(),
        (vec0.getZ() > vec1.getZ())? vec0.getZ() : vec1.getZ(),
        (vec0.getW() > vec1.getW())? vec0.getW() : vec1.getW()
    );
}

inline float maxElem( const VVector4 & vec )
{
    float result;
    result = (vec.getX() > vec.getY())? vec.getX() : vec.getY();
    result = (vec.getZ() > result)? vec.getZ() : result;
    result = (vec.getW() > result)? vec.getW() : result;
    return result;
}

inline const VVector4 minPerElem( const VVector4 & vec0, const VVector4 & vec1 )
{
    return VVector4(
        (vec0.getX() < vec1.getX())? vec0.getX() : vec1.getX(),
        (vec0.getY() < vec1.getY())? vec0.getY() : vec1.getY(),
        (vec0.getZ() < vec1.getZ())? vec0.getZ() : vec1.getZ(),
        (vec0.getW() < vec1.getW())? vec0.getW() : vec1.getW()
    );
}

inline float minElem( const VVector4 & vec )
{
    float result;
    result = (vec.getX() < vec.getY())? vec.getX() : vec.getY();
    result = (vec.getZ() < result)? vec.getZ() : result;
    result = (vec.getW() < result)? vec.getW() : result;
    return result;
}

inline float sum( const VVector4 & vec )
{
    float result;
    result = ( vec.getX() + vec.getY() );
    result = ( result + vec.getZ() );
    result = ( result + vec.getW() );
    return result;
}

inline float dot( const VVector4 & vec0, const VVector4 & vec1 )
{
    float result;
    result = ( vec0.getX() * vec1.getX() );
    result = ( result + ( vec0.getY() * vec1.getY() ) );
    result = ( result + ( vec0.getZ() * vec1.getZ() ) );
    result = ( result + ( vec0.getW() * vec1.getW() ) );
    return result;
}

inline float lengthSqr( const VVector4 & vec )
{
    float result;
    result = ( vec.getX() * vec.getX() );
    result = ( result + ( vec.getY() * vec.getY() ) );
    result = ( result + ( vec.getZ() * vec.getZ() ) );
    result = ( result + ( vec.getW() * vec.getW() ) );
    return result;
}

inline float length( const VVector4 & vec )
{
    return sqrtf( lengthSqr( vec ) );
}

inline const VVector4 normalize( const VVector4 & vec )
{
    float lenSqr, lenInv;
    lenSqr = lengthSqr( vec );
    lenInv = ( 1.0f / sqrtf( lenSqr ) );
    return VVector4(
        ( vec.getX() * lenInv ),
        ( vec.getY() * lenInv ),
        ( vec.getZ() * lenInv ),
        ( vec.getW() * lenInv )
    );
}

inline const VVector4 select( const VVector4 & vec0, const VVector4 & vec1, bool select1 )
{
    return VVector4(
        ( select1 )? vec1.getX() : vec0.getX(),
        ( select1 )? vec1.getY() : vec0.getY(),
        ( select1 )? vec1.getZ() : vec0.getZ(),
        ( select1 )? vec1.getW() : vec0.getW()
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VVector4 & vec )
{
    printf( "( %f %f %f %f )\n", vec.getX(), vec.getY(), vec.getZ(), vec.getW() );
}

inline void print( const VVector4 & vec, const char * name )
{
    printf( "%s: ( %f %f %f %f )\n", name, vec.getX(), vec.getY(), vec.getZ(), vec.getW() );
}

#endif

inline VPoint3::VPoint3( const VPoint3 & pnt )
{
    mX = pnt.mX;
    mY = pnt.mY;
    mZ = pnt.mZ;
}

inline VPoint3::VPoint3( float _x, float _y, float _z )
{
    mX = _x;
    mY = _y;
    mZ = _z;
}

inline VPoint3::VPoint3( const VVector3 & vec )
{
    mX = vec.getX();
    mY = vec.getY();
    mZ = vec.getZ();
}

inline VPoint3::VPoint3( float scalar )
{
    mX = scalar;
    mY = scalar;
    mZ = scalar;
}

inline const VPoint3 lerp( float t, const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return ( pnt0 + ( ( pnt1 - pnt0 ) * t ) );
}

inline VPoint3 & VPoint3::operator =( const VPoint3 & pnt )
{
    mX = pnt.mX;
    mY = pnt.mY;
    mZ = pnt.mZ;
    return *this;
}

inline VPoint3 & VPoint3::setX( float _x )
{
    mX = _x;
    return *this;
}

inline float VPoint3::getX( ) const
{
    return mX;
}

inline VPoint3 & VPoint3::setY( float _y )
{
    mY = _y;
    return *this;
}

inline float VPoint3::getY( ) const
{
    return mY;
}

inline VPoint3 & VPoint3::setZ( float _z )
{
    mZ = _z;
    return *this;
}

inline float VPoint3::getZ( ) const
{
    return mZ;
}

inline VPoint3 & VPoint3::setElem( int idx, float value )
{
    *(&mX + idx) = value;
    return *this;
}

inline float VPoint3::getElem( int idx ) const
{
    return *(&mX + idx);
}

inline float & VPoint3::operator []( int idx )
{
    return *(&mX + idx);
}

inline float VPoint3::operator []( int idx ) const
{
    return *(&mX + idx);
}

inline const VVector3 VPoint3::operator -( const VPoint3 & pnt ) const
{
    return VVector3(
        ( mX - pnt.mX ),
        ( mY - pnt.mY ),
        ( mZ - pnt.mZ )
    );
}

inline const VPoint3 VPoint3::operator +( const VVector3 & vec ) const
{
    return VPoint3(
        ( mX + vec.getX() ),
        ( mY + vec.getY() ),
        ( mZ + vec.getZ() )
    );
}

inline const VPoint3 VPoint3::operator -( const VVector3 & vec ) const
{
    return VPoint3(
        ( mX - vec.getX() ),
        ( mY - vec.getY() ),
        ( mZ - vec.getZ() )
    );
}

inline VPoint3 & VPoint3::operator +=( const VVector3 & vec )
{
    *this = *this + vec;
    return *this;
}

inline VPoint3 & VPoint3::operator -=( const VVector3 & vec )
{
    *this = *this - vec;
    return *this;
}

inline const VPoint3 mulPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return VPoint3(
        ( pnt0.getX() * pnt1.getX() ),
        ( pnt0.getY() * pnt1.getY() ),
        ( pnt0.getZ() * pnt1.getZ() )
    );
}

inline const VPoint3 divPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return VPoint3(
        ( pnt0.getX() / pnt1.getX() ),
        ( pnt0.getY() / pnt1.getY() ),
        ( pnt0.getZ() / pnt1.getZ() )
    );
}

inline const VPoint3 recipPerElem( const VPoint3 & pnt )
{
    return VPoint3(
        ( 1.0f / pnt.getX() ),
        ( 1.0f / pnt.getY() ),
        ( 1.0f / pnt.getZ() )
    );
}

inline const VPoint3 sqrtPerElem( const VPoint3 & pnt )
{
    return VPoint3(
        sqrtf( pnt.getX() ),
        sqrtf( pnt.getY() ),
        sqrtf( pnt.getZ() )
    );
}

inline const VPoint3 rsqrtPerElem( const VPoint3 & pnt )
{
    return VPoint3(
        ( 1.0f / sqrtf( pnt.getX() ) ),
        ( 1.0f / sqrtf( pnt.getY() ) ),
        ( 1.0f / sqrtf( pnt.getZ() ) )
    );
}

inline const VPoint3 absPerElem( const VPoint3 & pnt )
{
    return VPoint3(
        fabsf( pnt.getX() ),
        fabsf( pnt.getY() ),
        fabsf( pnt.getZ() )
    );
}

inline const VPoint3 copySignPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return VPoint3(
        ( pnt1.getX() < 0.0f )? -fabsf( pnt0.getX() ) : fabsf( pnt0.getX() ),
        ( pnt1.getY() < 0.0f )? -fabsf( pnt0.getY() ) : fabsf( pnt0.getY() ),
        ( pnt1.getZ() < 0.0f )? -fabsf( pnt0.getZ() ) : fabsf( pnt0.getZ() )
    );
}

inline const VPoint3 maxPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return VPoint3(
        (pnt0.getX() > pnt1.getX())? pnt0.getX() : pnt1.getX(),
        (pnt0.getY() > pnt1.getY())? pnt0.getY() : pnt1.getY(),
        (pnt0.getZ() > pnt1.getZ())? pnt0.getZ() : pnt1.getZ()
    );
}

inline float maxElem( const VPoint3 & pnt )
{
    float result;
    result = (pnt.getX() > pnt.getY())? pnt.getX() : pnt.getY();
    result = (pnt.getZ() > result)? pnt.getZ() : result;
    return result;
}

inline const VPoint3 minPerElem( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return VPoint3(
        (pnt0.getX() < pnt1.getX())? pnt0.getX() : pnt1.getX(),
        (pnt0.getY() < pnt1.getY())? pnt0.getY() : pnt1.getY(),
        (pnt0.getZ() < pnt1.getZ())? pnt0.getZ() : pnt1.getZ()
    );
}

inline float minElem( const VPoint3 & pnt )
{
    float result;
    result = (pnt.getX() < pnt.getY())? pnt.getX() : pnt.getY();
    result = (pnt.getZ() < result)? pnt.getZ() : result;
    return result;
}

inline float sum( const VPoint3 & pnt )
{
    float result;
    result = ( pnt.getX() + pnt.getY() );
    result = ( result + pnt.getZ() );
    return result;
}

inline const VPoint3 scale( const VPoint3 & pnt, float scaleVal )
{
    return mulPerElem( pnt, VPoint3( scaleVal ) );
}

inline const VPoint3 scale( const VPoint3 & pnt, const VVector3 & scaleVec )
{
    return mulPerElem( pnt, VPoint3( scaleVec ) );
}

inline float projection( const VPoint3 & pnt, const VVector3 & unitVec )
{
    float result;
    result = ( pnt.getX() * unitVec.getX() );
    result = ( result + ( pnt.getY() * unitVec.getY() ) );
    result = ( result + ( pnt.getZ() * unitVec.getZ() ) );
    return result;
}

inline float distSqrFromOrigin( const VPoint3 & pnt )
{
    return lengthSqr( VVector3( pnt ) );
}

inline float distFromOrigin( const VPoint3 & pnt )
{
    return length( VVector3( pnt ) );
}

inline float distSqr( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return lengthSqr( ( pnt1 - pnt0 ) );
}

inline float dist( const VPoint3 & pnt0, const VPoint3 & pnt1 )
{
    return length( ( pnt1 - pnt0 ) );
}

inline const VPoint3 select( const VPoint3 & pnt0, const VPoint3 & pnt1, bool select1 )
{
    return VPoint3(
        ( select1 )? pnt1.getX() : pnt0.getX(),
        ( select1 )? pnt1.getY() : pnt0.getY(),
        ( select1 )? pnt1.getZ() : pnt0.getZ()
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VPoint3 & pnt )
{
    printf( "( %f %f %f )\n", pnt.getX(), pnt.getY(), pnt.getZ() );
}

inline void print( const VPoint3 & pnt, const char * name )
{
    printf( "%s: ( %f %f %f )\n", name, pnt.getX(), pnt.getY(), pnt.getZ() );
}

#endif

} // namespace Aos
} // namespace Vectormath

#endif

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _VECTORMATH_VQuat_AOS_CPP_H
#define _VECTORMATH_VQuat_AOS_CPP_H
//-----------------------------------------------------------------------------
// Definitions

#ifndef _VECTORMATH_INTERNAL_FUNCTIONS
#define _VECTORMATH_INTERNAL_FUNCTIONS

#endif

namespace Vectormath {
namespace Aos {

inline VQuat::VQuat( const VQuat & VQuat )
{
    mX = VQuat.mX;
    mY = VQuat.mY;
    mZ = VQuat.mZ;
    mW = VQuat.mW;
}

inline VQuat::VQuat( float _x, float _y, float _z, float _w )
{
    mX = _x;
    mY = _y;
    mZ = _z;
    mW = _w;
}

inline VQuat::VQuat( const VVector3 & xyz, float _w )
{
    this->setXYZ( xyz );
    this->setW( _w );
}

inline VQuat::VQuat( const VVector4 & vec )
{
    mX = vec.getX();
    mY = vec.getY();
    mZ = vec.getZ();
    mW = vec.getW();
}

inline VQuat::VQuat( float scalar )
{
    mX = scalar;
    mY = scalar;
    mZ = scalar;
    mW = scalar;
}

inline const VQuat VQuat::identity( )
{
    return VQuat( 0.0f, 0.0f, 0.0f, 1.0f );
}

inline const VQuat lerp( float t, const VQuat & VQuat0, const VQuat & VQuat1 )
{
    return ( VQuat0 + ( ( VQuat1 - VQuat0 ) * t ) );
}

inline const VQuat slerp( float t, const VQuat & unitVQuat0, const VQuat & unitVQuat1 )
{
    VQuat start;
    float recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dot( unitVQuat0, unitVQuat1 );
    if ( cosAngle < 0.0f ) {
        cosAngle = -cosAngle;
        start = ( -unitVQuat0 );
    } else {
        start = unitVQuat0;
    }
    if ( cosAngle < _VECTORMATH_SLERP_TOL ) {
        angle = acosf( cosAngle );
        recipSinAngle = ( 1.0f / sinf( angle ) );
        scale0 = ( sinf( ( ( 1.0f - t ) * angle ) ) * recipSinAngle );
        scale1 = ( sinf( ( t * angle ) ) * recipSinAngle );
    } else {
        scale0 = ( 1.0f - t );
        scale1 = t;
    }
    return ( ( start * scale0 ) + ( unitVQuat1 * scale1 ) );
}

inline const VQuat squad( float t, const VQuat & unitVQuat0, const VQuat & unitVQuat1, const VQuat & unitVQuat2, const VQuat & unitVQuat3 )
{
    VQuat tmp0, tmp1;
    tmp0 = slerp( t, unitVQuat0, unitVQuat3 );
    tmp1 = slerp( t, unitVQuat1, unitVQuat2 );
    return slerp( ( ( 2.0f * t ) * ( 1.0f - t ) ), tmp0, tmp1 );
}

inline VQuat & VQuat::operator =( const VQuat & VQuat )
{
    mX = VQuat.mX;
    mY = VQuat.mY;
    mZ = VQuat.mZ;
    mW = VQuat.mW;
    return *this;
}

inline VQuat & VQuat::setXYZ( const VVector3 & vec )
{
    mX = vec.getX();
    mY = vec.getY();
    mZ = vec.getZ();
    return *this;
}

inline const VVector3 VQuat::getXYZ( ) const
{
    return VVector3( mX, mY, mZ );
}

inline VQuat & VQuat::setX( float _x )
{
    mX = _x;
    return *this;
}

inline float VQuat::getX( ) const
{
    return mX;
}

inline VQuat & VQuat::setY( float _y )
{
    mY = _y;
    return *this;
}

inline float VQuat::getY( ) const
{
    return mY;
}

inline VQuat & VQuat::setZ( float _z )
{
    mZ = _z;
    return *this;
}

inline float VQuat::getZ( ) const
{
    return mZ;
}

inline VQuat & VQuat::setW( float _w )
{
    mW = _w;
    return *this;
}

inline float VQuat::getW( ) const
{
    return mW;
}

inline VQuat & VQuat::setElem( int idx, float value )
{
    *(&mX + idx) = value;
    return *this;
}

inline float VQuat::getElem( int idx ) const
{
    return *(&mX + idx);
}

inline float & VQuat::operator []( int idx )
{
    return *(&mX + idx);
}

inline float VQuat::operator []( int idx ) const
{
    return *(&mX + idx);
}

inline const VQuat VQuat::operator +( const VQuat & q ) const
{
    return VQuat(
        ( mX + q.mX ),
        ( mY + q.mY ),
        ( mZ + q.mZ ),
        ( mW + q.mW )
    );
}

inline const VQuat VQuat::operator -( const VQuat & q ) const
{
    return VQuat(
        ( mX - q.mX ),
        ( mY - q.mY ),
        ( mZ - q.mZ ),
        ( mW - q.mW )
    );
}

inline const VQuat VQuat::operator *( float scalar ) const
{
    return VQuat(
        ( mX * scalar ),
        ( mY * scalar ),
        ( mZ * scalar ),
        ( mW * scalar )
    );
}

inline VQuat & VQuat::operator +=( const VQuat & VQuat )
{
    *this = *this + VQuat;
    return *this;
}

inline VQuat & VQuat::operator -=( const VQuat & VQuat )
{
    *this = *this - VQuat;
    return *this;
}

inline VQuat & VQuat::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const VQuat VQuat::operator /( float scalar ) const
{
    return VQuat(
        ( mX / scalar ),
        ( mY / scalar ),
        ( mZ / scalar ),
        ( mW / scalar )
    );
}

inline VQuat & VQuat::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const VQuat VQuat::operator -( ) const
{
    return VQuat(
        -mX,
        -mY,
        -mZ,
        -mW
    );
}

inline const VQuat operator *( float scalar, const VQuat & VQuat )
{
    return VQuat * scalar;
}

inline float dot( const VQuat & VQuat0, const VQuat & VQuat1 )
{
    float result;
    result = ( VQuat0.getX() * VQuat1.getX() );
    result = ( result + ( VQuat0.getY() * VQuat1.getY() ) );
    result = ( result + ( VQuat0.getZ() * VQuat1.getZ() ) );
    result = ( result + ( VQuat0.getW() * VQuat1.getW() ) );
    return result;
}

inline float norm( const VQuat & q )
{
    float result;
    result = ( q.getX() * q.getX() );
    result = ( result + ( q.getY() * q.getY() ) );
    result = ( result + ( q.getZ() * q.getZ() ) );
    result = ( result + ( q.getW() * q.getW() ) );
    return result;
}

inline float length( const VQuat & VQuat )
{
    return sqrtf( norm( VQuat ) );
}

inline const VQuat normalize( const VQuat & q )
{
    float lenSqr, lenInv;
    lenSqr = norm( q );
    lenInv = ( 1.0f / sqrtf( lenSqr ) );
    return VQuat(
        ( q.getX() * lenInv ),
        ( q.getY() * lenInv ),
        ( q.getZ() * lenInv ),
        ( q.getW() * lenInv )
    );
}

inline const VQuat VQuat::rotation( const VVector3 & unitVec0, const VVector3 & unitVec1 )
{
    float cosHalfAngleX2, recipCosHalfAngleX2;
    cosHalfAngleX2 = sqrtf( ( 2.0f * ( 1.0f + dot( unitVec0, unitVec1 ) ) ) );
    recipCosHalfAngleX2 = ( 1.0f / cosHalfAngleX2 );
    return VQuat( ( cross( unitVec0, unitVec1 ) * recipCosHalfAngleX2 ), ( cosHalfAngleX2 * 0.5f ) );
}

inline const VQuat VQuat::rotation( float radians, const VVector3 & unitVec )
{
    float s, c, angle;
    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return VQuat( ( unitVec * s ), c );
}

inline const VQuat VQuat::rotationX( float radians )
{
    float s, c, angle;
    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return VQuat( s, 0.0f, 0.0f, c );
}

inline const VQuat VQuat::rotationY( float radians )
{
    float s, c, angle;
    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return VQuat( 0.0f, s, 0.0f, c );
}

inline const VQuat VQuat::rotationZ( float radians )
{
    float s, c, angle;
    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return VQuat( 0.0f, 0.0f, s, c );
}

inline const VQuat VQuat::operator *( const VQuat & q ) const
{
    return VQuat(
        ( ( ( ( mW * q.mX ) + ( mX * q.mW ) ) + ( mY * q.mZ ) ) - ( mZ * q.mY ) ),
        ( ( ( ( mW * q.mY ) + ( mY * q.mW ) ) + ( mZ * q.mX ) ) - ( mX * q.mZ ) ),
        ( ( ( ( mW * q.mZ ) + ( mZ * q.mW ) ) + ( mX * q.mY ) ) - ( mY * q.mX ) ),
        ( ( ( ( mW * q.mW ) - ( mX * q.mX ) ) - ( mY * q.mY ) ) - ( mZ * q.mZ ) )
    );
}

inline VQuat & VQuat::operator *=( const VQuat & VQuat )
{
    *this = *this * VQuat;
    return *this;
}

inline const VVector3 rotate( const VQuat & VQuat, const VVector3 & vec )
{
    float tmpX, tmpY, tmpZ, tmpW;
    tmpX = ( ( ( VQuat.getW() * vec.getX() ) + ( VQuat.getY() * vec.getZ() ) ) - ( VQuat.getZ() * vec.getY() ) );
    tmpY = ( ( ( VQuat.getW() * vec.getY() ) + ( VQuat.getZ() * vec.getX() ) ) - ( VQuat.getX() * vec.getZ() ) );
    tmpZ = ( ( ( VQuat.getW() * vec.getZ() ) + ( VQuat.getX() * vec.getY() ) ) - ( VQuat.getY() * vec.getX() ) );
    tmpW = ( ( ( VQuat.getX() * vec.getX() ) + ( VQuat.getY() * vec.getY() ) ) + ( VQuat.getZ() * vec.getZ() ) );
    return VVector3(
        ( ( ( ( tmpW * VQuat.getX() ) + ( tmpX * VQuat.getW() ) ) - ( tmpY * VQuat.getZ() ) ) + ( tmpZ * VQuat.getY() ) ),
        ( ( ( ( tmpW * VQuat.getY() ) + ( tmpY * VQuat.getW() ) ) - ( tmpZ * VQuat.getX() ) ) + ( tmpX * VQuat.getZ() ) ),
        ( ( ( ( tmpW * VQuat.getZ() ) + ( tmpZ * VQuat.getW() ) ) - ( tmpX * VQuat.getY() ) ) + ( tmpY * VQuat.getX() ) )
    );
}

inline const VQuat conj( const VQuat & q )
{
    return VQuat( -q.getX(), -q.getY(), -q.getZ(), q.getW() );
}

inline const VQuat select( const VQuat & VQuat0, const VQuat & VQuat1, bool select1 )
{
    return VQuat(
        ( select1 )? VQuat1.getX() : VQuat0.getX(),
        ( select1 )? VQuat1.getY() : VQuat0.getY(),
        ( select1 )? VQuat1.getZ() : VQuat0.getZ(),
        ( select1 )? VQuat1.getW() : VQuat0.getW()
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VQuat & VQuat )
{
    printf( "( %f %f %f %f )\n", VQuat.getX(), VQuat.getY(), VQuat.getZ(), VQuat.getW() );
}

inline void print( const VQuat & VQuat, const char * name )
{
    printf( "%s: ( %f %f %f %f )\n", name, VQuat.getX(), VQuat.getY(), VQuat.getZ(), VQuat.getW() );
}

#endif

} // namespace Aos
} // namespace Vectormath

#endif

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

namespace Vectormath {
namespace Aos {

//-----------------------------------------------------------------------------
// Constants

#define _VECTORMATH_PI_OVER_2 1.570796327f

//-----------------------------------------------------------------------------
// Definitions

inline VMatrix3::VMatrix3( const VMatrix3 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
}

inline VMatrix3::VMatrix3( float scalar )
{
    mCol0 = VVector3( scalar );
    mCol1 = VVector3( scalar );
    mCol2 = VVector3( scalar );
}

inline VMatrix3::VMatrix3( const VQuat & unitVQuat )
{
    float qx, qy, qz, qw, qx2, qy2, qz2, qxqx2, qyqy2, qzqz2, qxqy2, qyqz2, qzqw2, qxqz2, qyqw2, qxqw2;
    qx = unitVQuat.getX();
    qy = unitVQuat.getY();
    qz = unitVQuat.getZ();
    qw = unitVQuat.getW();
    qx2 = ( qx + qx );
    qy2 = ( qy + qy );
    qz2 = ( qz + qz );
    qxqx2 = ( qx * qx2 );
    qxqy2 = ( qx * qy2 );
    qxqz2 = ( qx * qz2 );
    qxqw2 = ( qw * qx2 );
    qyqy2 = ( qy * qy2 );
    qyqz2 = ( qy * qz2 );
    qyqw2 = ( qw * qy2 );
    qzqz2 = ( qz * qz2 );
    qzqw2 = ( qw * qz2 );
    mCol0 = VVector3( ( ( 1.0f - qyqy2 ) - qzqz2 ), ( qxqy2 + qzqw2 ), ( qxqz2 - qyqw2 ) );
    mCol1 = VVector3( ( qxqy2 - qzqw2 ), ( ( 1.0f - qxqx2 ) - qzqz2 ), ( qyqz2 + qxqw2 ) );
    mCol2 = VVector3( ( qxqz2 + qyqw2 ), ( qyqz2 - qxqw2 ), ( ( 1.0f - qxqx2 ) - qyqy2 ) );
}

inline VMatrix3::VMatrix3( const VVector3 & _col0, const VVector3 & _col1, const VVector3 & _col2 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
}

inline VMatrix3 & VMatrix3::setCol0( const VVector3 & _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline VMatrix3 & VMatrix3::setCol1( const VVector3 & _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline VMatrix3 & VMatrix3::setCol2( const VVector3 & _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline VMatrix3 & VMatrix3::setCol( int col, const VVector3 & vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline VMatrix3 & VMatrix3::setRow( int row, const VVector3 & vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    return *this;
}

inline VMatrix3 & VMatrix3::setElem( int col, int row, float val )
{
    VVector3 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline float VMatrix3::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const VVector3 VMatrix3::getCol0( ) const
{
    return mCol0;
}

inline const VVector3 VMatrix3::getCol1( ) const
{
    return mCol1;
}

inline const VVector3 VMatrix3::getCol2( ) const
{
    return mCol2;
}

inline const VVector3 VMatrix3::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const VVector3 VMatrix3::getRow( int row ) const
{
    return VVector3( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ) );
}

inline VVector3 & VMatrix3::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const VVector3 VMatrix3::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline VMatrix3 & VMatrix3::operator =( const VMatrix3 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    return *this;
}

inline const VMatrix3 transpose( const VMatrix3 & mat )
{
    return VMatrix3(
        VVector3( mat.getCol0().getX(), mat.getCol1().getX(), mat.getCol2().getX() ),
        VVector3( mat.getCol0().getY(), mat.getCol1().getY(), mat.getCol2().getY() ),
        VVector3( mat.getCol0().getZ(), mat.getCol1().getZ(), mat.getCol2().getZ() )
    );
}

inline const VMatrix3 inverse( const VMatrix3 & mat )
{
    VVector3 tmp0, tmp1, tmp2;
    float detinv;
    tmp0 = cross( mat.getCol1(), mat.getCol2() );
    tmp1 = cross( mat.getCol2(), mat.getCol0() );
    tmp2 = cross( mat.getCol0(), mat.getCol1() );
    detinv = ( 1.0f / dot( mat.getCol2(), tmp2 ) );
    return VMatrix3(
        VVector3( ( tmp0.getX() * detinv ), ( tmp1.getX() * detinv ), ( tmp2.getX() * detinv ) ),
        VVector3( ( tmp0.getY() * detinv ), ( tmp1.getY() * detinv ), ( tmp2.getY() * detinv ) ),
        VVector3( ( tmp0.getZ() * detinv ), ( tmp1.getZ() * detinv ), ( tmp2.getZ() * detinv ) )
    );
}

inline float determinant( const VMatrix3 & mat )
{
    return dot( mat.getCol2(), cross( mat.getCol0(), mat.getCol1() ) );
}

inline const VMatrix3 VMatrix3::operator +( const VMatrix3 & mat ) const
{
    return VMatrix3(
        ( mCol0 + mat.mCol0 ),
        ( mCol1 + mat.mCol1 ),
        ( mCol2 + mat.mCol2 )
    );
}

inline const VMatrix3 VMatrix3::operator -( const VMatrix3 & mat ) const
{
    return VMatrix3(
        ( mCol0 - mat.mCol0 ),
        ( mCol1 - mat.mCol1 ),
        ( mCol2 - mat.mCol2 )
    );
}

inline VMatrix3 & VMatrix3::operator +=( const VMatrix3 & mat )
{
    *this = *this + mat;
    return *this;
}

inline VMatrix3 & VMatrix3::operator -=( const VMatrix3 & mat )
{
    *this = *this - mat;
    return *this;
}

inline const VMatrix3 VMatrix3::operator -( ) const
{
    return VMatrix3(
        ( -mCol0 ),
        ( -mCol1 ),
        ( -mCol2 )
    );
}

inline const VMatrix3 absPerElem( const VMatrix3 & mat )
{
    return VMatrix3(
        absPerElem( mat.getCol0() ),
        absPerElem( mat.getCol1() ),
        absPerElem( mat.getCol2() )
    );
}

inline const VMatrix3 VMatrix3::operator *( float scalar ) const
{
    return VMatrix3(
        ( mCol0 * scalar ),
        ( mCol1 * scalar ),
        ( mCol2 * scalar )
    );
}

inline VMatrix3 & VMatrix3::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const VMatrix3 operator *( float scalar, const VMatrix3 & mat )
{
    return mat * scalar;
}

inline const VVector3 VMatrix3::operator *( const VVector3 & vec ) const
{
    return VVector3(
        ( ( ( mCol0.getX() * vec.getX() ) + ( mCol1.getX() * vec.getY() ) ) + ( mCol2.getX() * vec.getZ() ) ),
        ( ( ( mCol0.getY() * vec.getX() ) + ( mCol1.getY() * vec.getY() ) ) + ( mCol2.getY() * vec.getZ() ) ),
        ( ( ( mCol0.getZ() * vec.getX() ) + ( mCol1.getZ() * vec.getY() ) ) + ( mCol2.getZ() * vec.getZ() ) )
    );
}

inline const VMatrix3 VMatrix3::operator *( const VMatrix3 & mat ) const
{
    return VMatrix3(
        ( *this * mat.mCol0 ),
        ( *this * mat.mCol1 ),
        ( *this * mat.mCol2 )
    );
}

inline VMatrix3 & VMatrix3::operator *=( const VMatrix3 & mat )
{
    *this = *this * mat;
    return *this;
}

inline const VMatrix3 mulPerElem( const VMatrix3 & mat0, const VMatrix3 & mat1 )
{
    return VMatrix3(
        mulPerElem( mat0.getCol0(), mat1.getCol0() ),
        mulPerElem( mat0.getCol1(), mat1.getCol1() ),
        mulPerElem( mat0.getCol2(), mat1.getCol2() )
    );
}

inline const VMatrix3 VMatrix3::identity( )
{
    return VMatrix3(
        VVector3::xAxis( ),
        VVector3::yAxis( ),
        VVector3::zAxis( )
    );
}

inline const VMatrix3 VMatrix3::rotationX( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix3(
        VVector3::xAxis( ),
        VVector3( 0.0f, c, s ),
        VVector3( 0.0f, -s, c )
    );
}

inline const VMatrix3 VMatrix3::rotationY( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix3(
        VVector3( c, 0.0f, -s ),
        VVector3::yAxis( ),
        VVector3( s, 0.0f, c )
    );
}

inline const VMatrix3 VMatrix3::rotationZ( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix3(
        VVector3( c, s, 0.0f ),
        VVector3( -s, c, 0.0f ),
        VVector3::zAxis( )
    );
}

inline const VMatrix3 VMatrix3::rotationZYX( const VVector3 & radiansXYZ )
{
    float sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf( radiansXYZ.getX() );
    cX = cosf( radiansXYZ.getX() );
    sY = sinf( radiansXYZ.getY() );
    cY = cosf( radiansXYZ.getY() );
    sZ = sinf( radiansXYZ.getZ() );
    cZ = cosf( radiansXYZ.getZ() );
    tmp0 = ( cZ * sY );
    tmp1 = ( sZ * sY );
    return VMatrix3(
        VVector3( ( cZ * cY ), ( sZ * cY ), -sY ),
        VVector3( ( ( tmp0 * sX ) - ( sZ * cX ) ), ( ( tmp1 * sX ) + ( cZ * cX ) ), ( cY * sX ) ),
        VVector3( ( ( tmp0 * cX ) + ( sZ * sX ) ), ( ( tmp1 * cX ) - ( cZ * sX ) ), ( cY * cX ) )
    );
}

inline const VMatrix3 VMatrix3::rotation( float radians, const VVector3 & unitVec )
{
    float x, y, z, s, c, oneMinusC, xy, yz, zx;
    s = sinf( radians );
    c = cosf( radians );
    x = unitVec.getX();
    y = unitVec.getY();
    z = unitVec.getZ();
    xy = ( x * y );
    yz = ( y * z );
    zx = ( z * x );
    oneMinusC = ( 1.0f - c );
    return VMatrix3(
        VVector3( ( ( ( x * x ) * oneMinusC ) + c ), ( ( xy * oneMinusC ) + ( z * s ) ), ( ( zx * oneMinusC ) - ( y * s ) ) ),
        VVector3( ( ( xy * oneMinusC ) - ( z * s ) ), ( ( ( y * y ) * oneMinusC ) + c ), ( ( yz * oneMinusC ) + ( x * s ) ) ),
        VVector3( ( ( zx * oneMinusC ) + ( y * s ) ), ( ( yz * oneMinusC ) - ( x * s ) ), ( ( ( z * z ) * oneMinusC ) + c ) )
    );
}

inline const VMatrix3 VMatrix3::rotation( const VQuat & unitVQuat )
{
    return VMatrix3( unitVQuat );
}

inline const VMatrix3 VMatrix3::scale( const VVector3 & scaleVec )
{
    return VMatrix3(
        VVector3( scaleVec.getX(), 0.0f, 0.0f ),
        VVector3( 0.0f, scaleVec.getY(), 0.0f ),
        VVector3( 0.0f, 0.0f, scaleVec.getZ() )
    );
}

inline const VMatrix3 appendScale( const VMatrix3 & mat, const VVector3 & scaleVec )
{
    return VMatrix3(
        ( mat.getCol0() * scaleVec.getX( ) ),
        ( mat.getCol1() * scaleVec.getY( ) ),
        ( mat.getCol2() * scaleVec.getZ( ) )
    );
}

inline const VMatrix3 prependScale( const VVector3 & scaleVec, const VMatrix3 & mat )
{
    return VMatrix3(
        mulPerElem( mat.getCol0(), scaleVec ),
        mulPerElem( mat.getCol1(), scaleVec ),
        mulPerElem( mat.getCol2(), scaleVec )
    );
}

inline const VMatrix3 select( const VMatrix3 & mat0, const VMatrix3 & mat1, bool select1 )
{
    return VMatrix3(
        select( mat0.getCol0(), mat1.getCol0(), select1 ),
        select( mat0.getCol1(), mat1.getCol1(), select1 ),
        select( mat0.getCol2(), mat1.getCol2(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VMatrix3 & mat )
{
    print( mat.getRow( 0 ) );
    print( mat.getRow( 1 ) );
    print( mat.getRow( 2 ) );
}

inline void print( const VMatrix3 & mat, const char * name )
{
    printf("%s:\n", name);
    print( mat );
}

#endif

inline VMatrix4::VMatrix4( const VMatrix4 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    mCol3 = mat.mCol3;
}

inline VMatrix4::VMatrix4( float scalar )
{
    mCol0 = VVector4( scalar );
    mCol1 = VVector4( scalar );
    mCol2 = VVector4( scalar );
    mCol3 = VVector4( scalar );
}

inline VMatrix4::VMatrix4( const VTransform3 & mat )
{
    mCol0 = VVector4( mat.getCol0(), 0.0f );
    mCol1 = VVector4( mat.getCol1(), 0.0f );
    mCol2 = VVector4( mat.getCol2(), 0.0f );
    mCol3 = VVector4( mat.getCol3(), 1.0f );
}

inline VMatrix4::VMatrix4( const VVector4 & _col0, const VVector4 & _col1, const VVector4 & _col2, const VVector4 & _col3 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
    mCol3 = _col3;
}

inline VMatrix4::VMatrix4( const VMatrix3 & mat, const VVector3 & translateVec )
{
    mCol0 = VVector4( mat.getCol0(), 0.0f );
    mCol1 = VVector4( mat.getCol1(), 0.0f );
    mCol2 = VVector4( mat.getCol2(), 0.0f );
    mCol3 = VVector4( translateVec, 1.0f );
}

inline VMatrix4::VMatrix4( const VQuat & unitVQuat, const VVector3 & translateVec )
{
    VMatrix3 mat;
    mat = VMatrix3( unitVQuat );
    mCol0 = VVector4( mat.getCol0(), 0.0f );
    mCol1 = VVector4( mat.getCol1(), 0.0f );
    mCol2 = VVector4( mat.getCol2(), 0.0f );
    mCol3 = VVector4( translateVec, 1.0f );
}

inline VMatrix4 & VMatrix4::setCol0( const VVector4 & _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline VMatrix4 & VMatrix4::setCol1( const VVector4 & _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline VMatrix4 & VMatrix4::setCol2( const VVector4 & _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline VMatrix4 & VMatrix4::setCol3( const VVector4 & _col3 )
{
    mCol3 = _col3;
    return *this;
}

inline VMatrix4 & VMatrix4::setCol( int col, const VVector4 & vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline VMatrix4 & VMatrix4::setRow( int row, const VVector4 & vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    mCol3.setElem( row, vec.getElem( 3 ) );
    return *this;
}

inline VMatrix4 & VMatrix4::setElem( int col, int row, float val )
{
    VVector4 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline float VMatrix4::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const VVector4 VMatrix4::getCol0( ) const
{
    return mCol0;
}

inline const VVector4 VMatrix4::getCol1( ) const
{
    return mCol1;
}

inline const VVector4 VMatrix4::getCol2( ) const
{
    return mCol2;
}

inline const VVector4 VMatrix4::getCol3( ) const
{
    return mCol3;
}

inline const VVector4 VMatrix4::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const VVector4 VMatrix4::getRow( int row ) const
{
    return VVector4( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ), mCol3.getElem( row ) );
}

inline VVector4 & VMatrix4::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const VVector4 VMatrix4::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline VMatrix4 & VMatrix4::operator =( const VMatrix4 & mat )
{
    mCol0 = mat.mCol0;
    mCol1 = mat.mCol1;
    mCol2 = mat.mCol2;
    mCol3 = mat.mCol3;
    return *this;
}

inline const VMatrix4 transpose( const VMatrix4 & mat )
{
    return VMatrix4(
        VVector4( mat.getCol0().getX(), mat.getCol1().getX(), mat.getCol2().getX(), mat.getCol3().getX() ),
        VVector4( mat.getCol0().getY(), mat.getCol1().getY(), mat.getCol2().getY(), mat.getCol3().getY() ),
        VVector4( mat.getCol0().getZ(), mat.getCol1().getZ(), mat.getCol2().getZ(), mat.getCol3().getZ() ),
        VVector4( mat.getCol0().getW(), mat.getCol1().getW(), mat.getCol2().getW(), mat.getCol3().getW() )
    );
}

inline const VMatrix4 inverse( const VMatrix4 & mat )
{
    VVector4 res0, res1, res2, res3;
    float mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, detInv;
    mA = mat.getCol0().getX();
    mB = mat.getCol0().getY();
    mC = mat.getCol0().getZ();
    mD = mat.getCol0().getW();
    mE = mat.getCol1().getX();
    mF = mat.getCol1().getY();
    mG = mat.getCol1().getZ();
    mH = mat.getCol1().getW();
    mI = mat.getCol2().getX();
    mJ = mat.getCol2().getY();
    mK = mat.getCol2().getZ();
    mL = mat.getCol2().getW();
    mM = mat.getCol3().getX();
    mN = mat.getCol3().getY();
    mO = mat.getCol3().getZ();
    mP = mat.getCol3().getW();
    tmp0 = ( ( mK * mD ) - ( mC * mL ) );
    tmp1 = ( ( mO * mH ) - ( mG * mP ) );
    tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
    tmp3 = ( ( mF * mO ) - ( mN * mG ) );
    tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
    tmp5 = ( ( mN * mH ) - ( mF * mP ) );
    res0.setX( ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) ) );
    res0.setY( ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) ) );
    res0.setZ( ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) ) );
    res0.setW( ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) ) );
    detInv = ( 1.0f / ( ( ( ( mA * res0.getX() ) + ( mE * res0.getY() ) ) + ( mI * res0.getZ() ) ) + ( mM * res0.getW() ) ) );
    res1.setX( ( mI * tmp1 ) );
    res1.setY( ( mM * tmp0 ) );
    res1.setZ( ( mA * tmp1 ) );
    res1.setW( ( mE * tmp0 ) );
    res3.setX( ( mI * tmp3 ) );
    res3.setY( ( mM * tmp2 ) );
    res3.setZ( ( mA * tmp3 ) );
    res3.setW( ( mE * tmp2 ) );
    res2.setX( ( mI * tmp5 ) );
    res2.setY( ( mM * tmp4 ) );
    res2.setZ( ( mA * tmp5 ) );
    res2.setW( ( mE * tmp4 ) );
    tmp0 = ( ( mI * mB ) - ( mA * mJ ) );
    tmp1 = ( ( mM * mF ) - ( mE * mN ) );
    tmp2 = ( ( mI * mD ) - ( mA * mL ) );
    tmp3 = ( ( mM * mH ) - ( mE * mP ) );
    tmp4 = ( ( mI * mC ) - ( mA * mK ) );
    tmp5 = ( ( mM * mG ) - ( mE * mO ) );
    res2.setX( ( ( ( mL * tmp1 ) - ( mJ * tmp3 ) ) + res2.getX() ) );
    res2.setY( ( ( ( mP * tmp0 ) - ( mN * tmp2 ) ) + res2.getY() ) );
    res2.setZ( ( ( ( mB * tmp3 ) - ( mD * tmp1 ) ) - res2.getZ() ) );
    res2.setW( ( ( ( mF * tmp2 ) - ( mH * tmp0 ) ) - res2.getW() ) );
    res3.setX( ( ( ( mJ * tmp5 ) - ( mK * tmp1 ) ) + res3.getX() ) );
    res3.setY( ( ( ( mN * tmp4 ) - ( mO * tmp0 ) ) + res3.getY() ) );
    res3.setZ( ( ( ( mC * tmp1 ) - ( mB * tmp5 ) ) - res3.getZ() ) );
    res3.setW( ( ( ( mG * tmp0 ) - ( mF * tmp4 ) ) - res3.getW() ) );
    res1.setX( ( ( ( mK * tmp3 ) - ( mL * tmp5 ) ) - res1.getX() ) );
    res1.setY( ( ( ( mO * tmp2 ) - ( mP * tmp4 ) ) - res1.getY() ) );
    res1.setZ( ( ( ( mD * tmp5 ) - ( mC * tmp3 ) ) + res1.getZ() ) );
    res1.setW( ( ( ( mH * tmp4 ) - ( mG * tmp2 ) ) + res1.getW() ) );
    return VMatrix4(
        ( res0 * detInv ),
        ( res1 * detInv ),
        ( res2 * detInv ),
        ( res3 * detInv )
    );
}

inline const VMatrix4 affineInverse( const VMatrix4 & mat )
{
    VTransform3 affineMat;
    affineMat.setCol0( mat.getCol0().getXYZ( ) );
    affineMat.setCol1( mat.getCol1().getXYZ( ) );
    affineMat.setCol2( mat.getCol2().getXYZ( ) );
    affineMat.setCol3( mat.getCol3().getXYZ( ) );
    return VMatrix4( inverse( affineMat ) );
}

inline const VMatrix4 orthoInverse( const VMatrix4 & mat )
{
    VTransform3 affineMat;
    affineMat.setCol0( mat.getCol0().getXYZ( ) );
    affineMat.setCol1( mat.getCol1().getXYZ( ) );
    affineMat.setCol2( mat.getCol2().getXYZ( ) );
    affineMat.setCol3( mat.getCol3().getXYZ( ) );
    return VMatrix4( orthoInverse( affineMat ) );
}

inline float determinant( const VMatrix4 & mat )
{
    float dx, dy, dz, dw, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
    mA = mat.getCol0().getX();
    mB = mat.getCol0().getY();
    mC = mat.getCol0().getZ();
    mD = mat.getCol0().getW();
    mE = mat.getCol1().getX();
    mF = mat.getCol1().getY();
    mG = mat.getCol1().getZ();
    mH = mat.getCol1().getW();
    mI = mat.getCol2().getX();
    mJ = mat.getCol2().getY();
    mK = mat.getCol2().getZ();
    mL = mat.getCol2().getW();
    mM = mat.getCol3().getX();
    mN = mat.getCol3().getY();
    mO = mat.getCol3().getZ();
    mP = mat.getCol3().getW();
    tmp0 = ( ( mK * mD ) - ( mC * mL ) );
    tmp1 = ( ( mO * mH ) - ( mG * mP ) );
    tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
    tmp3 = ( ( mF * mO ) - ( mN * mG ) );
    tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
    tmp5 = ( ( mN * mH ) - ( mF * mP ) );
    dx = ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) );
    dy = ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) );
    dz = ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) );
    dw = ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) );
    return ( ( ( ( mA * dx ) + ( mE * dy ) ) + ( mI * dz ) ) + ( mM * dw ) );
}

inline const VMatrix4 VMatrix4::operator +( const VMatrix4 & mat ) const
{
    return VMatrix4(
        ( mCol0 + mat.mCol0 ),
        ( mCol1 + mat.mCol1 ),
        ( mCol2 + mat.mCol2 ),
        ( mCol3 + mat.mCol3 )
    );
}

inline const VMatrix4 VMatrix4::operator -( const VMatrix4 & mat ) const
{
    return VMatrix4(
        ( mCol0 - mat.mCol0 ),
        ( mCol1 - mat.mCol1 ),
        ( mCol2 - mat.mCol2 ),
        ( mCol3 - mat.mCol3 )
    );
}

inline VMatrix4 & VMatrix4::operator +=( const VMatrix4 & mat )
{
    *this = *this + mat;
    return *this;
}

inline VMatrix4 & VMatrix4::operator -=( const VMatrix4 & mat )
{
    *this = *this - mat;
    return *this;
}

inline const VMatrix4 VMatrix4::operator -( ) const
{
    return VMatrix4(
        ( -mCol0 ),
        ( -mCol1 ),
        ( -mCol2 ),
        ( -mCol3 )
    );
}

inline const VMatrix4 absPerElem( const VMatrix4 & mat )
{
    return VMatrix4(
        absPerElem( mat.getCol0() ),
        absPerElem( mat.getCol1() ),
        absPerElem( mat.getCol2() ),
        absPerElem( mat.getCol3() )
    );
}

inline const VMatrix4 VMatrix4::operator *( float scalar ) const
{
    return VMatrix4(
        ( mCol0 * scalar ),
        ( mCol1 * scalar ),
        ( mCol2 * scalar ),
        ( mCol3 * scalar )
    );
}

inline VMatrix4 & VMatrix4::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const VMatrix4 operator *( float scalar, const VMatrix4 & mat )
{
    return mat * scalar;
}

inline const VVector4 VMatrix4::operator *( const VVector4 & vec ) const
{
    return VVector4(
        ( ( ( ( mCol0.getX() * vec.getX() ) + ( mCol1.getX() * vec.getY() ) ) + ( mCol2.getX() * vec.getZ() ) ) + ( mCol3.getX() * vec.getW() ) ),
        ( ( ( ( mCol0.getY() * vec.getX() ) + ( mCol1.getY() * vec.getY() ) ) + ( mCol2.getY() * vec.getZ() ) ) + ( mCol3.getY() * vec.getW() ) ),
        ( ( ( ( mCol0.getZ() * vec.getX() ) + ( mCol1.getZ() * vec.getY() ) ) + ( mCol2.getZ() * vec.getZ() ) ) + ( mCol3.getZ() * vec.getW() ) ),
        ( ( ( ( mCol0.getW() * vec.getX() ) + ( mCol1.getW() * vec.getY() ) ) + ( mCol2.getW() * vec.getZ() ) ) + ( mCol3.getW() * vec.getW() ) )
    );
}

inline const VVector4 VMatrix4::operator *( const VVector3 & vec ) const
{
    return VVector4(
        ( ( ( mCol0.getX() * vec.getX() ) + ( mCol1.getX() * vec.getY() ) ) + ( mCol2.getX() * vec.getZ() ) ),
        ( ( ( mCol0.getY() * vec.getX() ) + ( mCol1.getY() * vec.getY() ) ) + ( mCol2.getY() * vec.getZ() ) ),
        ( ( ( mCol0.getZ() * vec.getX() ) + ( mCol1.getZ() * vec.getY() ) ) + ( mCol2.getZ() * vec.getZ() ) ),
        ( ( ( mCol0.getW() * vec.getX() ) + ( mCol1.getW() * vec.getY() ) ) + ( mCol2.getW() * vec.getZ() ) )
    );
}

inline const VVector4 VMatrix4::operator *( const VPoint3 & pnt ) const
{
    return VVector4(
        ( ( ( ( mCol0.getX() * pnt.getX() ) + ( mCol1.getX() * pnt.getY() ) ) + ( mCol2.getX() * pnt.getZ() ) ) + mCol3.getX() ),
        ( ( ( ( mCol0.getY() * pnt.getX() ) + ( mCol1.getY() * pnt.getY() ) ) + ( mCol2.getY() * pnt.getZ() ) ) + mCol3.getY() ),
        ( ( ( ( mCol0.getZ() * pnt.getX() ) + ( mCol1.getZ() * pnt.getY() ) ) + ( mCol2.getZ() * pnt.getZ() ) ) + mCol3.getZ() ),
        ( ( ( ( mCol0.getW() * pnt.getX() ) + ( mCol1.getW() * pnt.getY() ) ) + ( mCol2.getW() * pnt.getZ() ) ) + mCol3.getW() )
    );
}

inline const VMatrix4 VMatrix4::operator *( const VMatrix4 & mat ) const
{
    return VMatrix4(
        ( *this * mat.mCol0 ),
        ( *this * mat.mCol1 ),
        ( *this * mat.mCol2 ),
        ( *this * mat.mCol3 )
    );
}

inline VMatrix4 & VMatrix4::operator *=( const VMatrix4 & mat )
{
    *this = *this * mat;
    return *this;
}

inline const VMatrix4 VMatrix4::operator *( const VTransform3 & tfrm ) const
{
    return VMatrix4(
        ( *this * tfrm.getCol0() ),
        ( *this * tfrm.getCol1() ),
        ( *this * tfrm.getCol2() ),
        ( *this * VPoint3( tfrm.getCol3() ) )
    );
}

inline VMatrix4 & VMatrix4::operator *=( const VTransform3 & tfrm )
{
    *this = *this * tfrm;
    return *this;
}

inline const VMatrix4 mulPerElem( const VMatrix4 & mat0, const VMatrix4 & mat1 )
{
    return VMatrix4(
        mulPerElem( mat0.getCol0(), mat1.getCol0() ),
        mulPerElem( mat0.getCol1(), mat1.getCol1() ),
        mulPerElem( mat0.getCol2(), mat1.getCol2() ),
        mulPerElem( mat0.getCol3(), mat1.getCol3() )
    );
}

inline const VMatrix4 VMatrix4::identity( )
{
    return VMatrix4(
        VVector4::xAxis( ),
        VVector4::yAxis( ),
        VVector4::zAxis( ),
        VVector4::wAxis( )
    );
}

inline VMatrix4 & VMatrix4::setUpper3x3( const VMatrix3 & mat3 )
{
    mCol0.setXYZ( mat3.getCol0() );
    mCol1.setXYZ( mat3.getCol1() );
    mCol2.setXYZ( mat3.getCol2() );
    return *this;
}

inline const VMatrix3 VMatrix4::getUpper3x3( ) const
{
    return VMatrix3(
        mCol0.getXYZ( ),
        mCol1.getXYZ( ),
        mCol2.getXYZ( )
    );
}

inline VMatrix4 & VMatrix4::setTranslation( const VVector3 & translateVec )
{
    mCol3.setXYZ( translateVec );
    return *this;
}

inline const VVector3 VMatrix4::getTranslation( ) const
{
    return mCol3.getXYZ( );
}

inline const VMatrix4 VMatrix4::rotationX( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix4(
        VVector4::xAxis( ),
        VVector4( 0.0f, c, s, 0.0f ),
        VVector4( 0.0f, -s, c, 0.0f ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 VMatrix4::rotationY( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix4(
        VVector4( c, 0.0f, -s, 0.0f ),
        VVector4::yAxis( ),
        VVector4( s, 0.0f, c, 0.0f ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 VMatrix4::rotationZ( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VMatrix4(
        VVector4( c, s, 0.0f, 0.0f ),
        VVector4( -s, c, 0.0f, 0.0f ),
        VVector4::zAxis( ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 VMatrix4::rotationZYX( const VVector3 & radiansXYZ )
{
    float sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf( radiansXYZ.getX() );
    cX = cosf( radiansXYZ.getX() );
    sY = sinf( radiansXYZ.getY() );
    cY = cosf( radiansXYZ.getY() );
    sZ = sinf( radiansXYZ.getZ() );
    cZ = cosf( radiansXYZ.getZ() );
    tmp0 = ( cZ * sY );
    tmp1 = ( sZ * sY );
    return VMatrix4(
        VVector4( ( cZ * cY ), ( sZ * cY ), -sY, 0.0f ),
        VVector4( ( ( tmp0 * sX ) - ( sZ * cX ) ), ( ( tmp1 * sX ) + ( cZ * cX ) ), ( cY * sX ), 0.0f ),
        VVector4( ( ( tmp0 * cX ) + ( sZ * sX ) ), ( ( tmp1 * cX ) - ( cZ * sX ) ), ( cY * cX ), 0.0f ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 VMatrix4::rotation( float radians, const VVector3 & unitVec )
{
    float x, y, z, s, c, oneMinusC, xy, yz, zx;
    s = sinf( radians );
    c = cosf( radians );
    x = unitVec.getX();
    y = unitVec.getY();
    z = unitVec.getZ();
    xy = ( x * y );
    yz = ( y * z );
    zx = ( z * x );
    oneMinusC = ( 1.0f - c );
    return VMatrix4(
        VVector4( ( ( ( x * x ) * oneMinusC ) + c ), ( ( xy * oneMinusC ) + ( z * s ) ), ( ( zx * oneMinusC ) - ( y * s ) ), 0.0f ),
        VVector4( ( ( xy * oneMinusC ) - ( z * s ) ), ( ( ( y * y ) * oneMinusC ) + c ), ( ( yz * oneMinusC ) + ( x * s ) ), 0.0f ),
        VVector4( ( ( zx * oneMinusC ) + ( y * s ) ), ( ( yz * oneMinusC ) - ( x * s ) ), ( ( ( z * z ) * oneMinusC ) + c ), 0.0f ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 VMatrix4::rotation( const VQuat & unitVQuat )
{
    return VMatrix4( VTransform3::rotation( unitVQuat ) );
}

inline const VMatrix4 VMatrix4::scale( const VVector3 & scaleVec )
{
    return VMatrix4(
        VVector4( scaleVec.getX(), 0.0f, 0.0f, 0.0f ),
        VVector4( 0.0f, scaleVec.getY(), 0.0f, 0.0f ),
        VVector4( 0.0f, 0.0f, scaleVec.getZ(), 0.0f ),
        VVector4::wAxis( )
    );
}

inline const VMatrix4 appendScale( const VMatrix4 & mat, const VVector3 & scaleVec )
{
    return VMatrix4(
        ( mat.getCol0() * scaleVec.getX( ) ),
        ( mat.getCol1() * scaleVec.getY( ) ),
        ( mat.getCol2() * scaleVec.getZ( ) ),
        mat.getCol3()
    );
}

inline const VMatrix4 prependScale( const VVector3 & scaleVec, const VMatrix4 & mat )
{
    VVector4 scale4;
    scale4 = VVector4( scaleVec, 1.0f );
    return VMatrix4(
        mulPerElem( mat.getCol0(), scale4 ),
        mulPerElem( mat.getCol1(), scale4 ),
        mulPerElem( mat.getCol2(), scale4 ),
        mulPerElem( mat.getCol3(), scale4 )
    );
}

inline const VMatrix4 VMatrix4::translation( const VVector3 & translateVec )
{
    return VMatrix4(
        VVector4::xAxis( ),
        VVector4::yAxis( ),
        VVector4::zAxis( ),
        VVector4( translateVec, 1.0f )
    );
}

inline const VMatrix4 VMatrix4::lookAt( const VPoint3 & eyePos, const VPoint3 & lookAtPos, const VVector3 & upVec )
{
    VMatrix4 m4EyeFrame;
    VVector3 v3X, v3Y, v3Z;
    v3Y = normalize( upVec );
    v3Z = normalize( ( eyePos - lookAtPos ) );
    v3X = normalize( cross( v3Y, v3Z ) );
    v3Y = cross( v3Z, v3X );
    m4EyeFrame = VMatrix4( VVector4( v3X ), VVector4( v3Y ), VVector4( v3Z ), VVector4( eyePos ) );
    return orthoInverse( m4EyeFrame );
}

inline const VMatrix4 VMatrix4::perspective( float fovyRadians, float aspect, float zNear, float zFar )
{
    float f, rangeInv;
    f = tanf( ( (float)( _VECTORMATH_PI_OVER_2 ) - ( 0.5f * fovyRadians ) ) );
    rangeInv = ( 1.0f / ( zNear - zFar ) );
    return VMatrix4(
        VVector4( ( f / aspect ), 0.0f, 0.0f, 0.0f ),
        VVector4( 0.0f, f, 0.0f, 0.0f ),
        VVector4( 0.0f, 0.0f, ( ( zNear + zFar ) * rangeInv ), -1.0f ),
        VVector4( 0.0f, 0.0f, ( ( ( zNear * zFar ) * rangeInv ) * 2.0f ), 0.0f )
    );
}

inline const VMatrix4 VMatrix4::frustum( float left, float right, float bottom, float top, float zNear, float zFar )
{
    float sum_rl, sum_tb, sum_nf, inv_rl, inv_tb, inv_nf, n2;
    sum_rl = ( right + left );
    sum_tb = ( top + bottom );
    sum_nf = ( zNear + zFar );
    inv_rl = ( 1.0f / ( right - left ) );
    inv_tb = ( 1.0f / ( top - bottom ) );
    inv_nf = ( 1.0f / ( zNear - zFar ) );
    n2 = ( zNear + zNear );
    return VMatrix4(
        VVector4( ( n2 * inv_rl ), 0.0f, 0.0f, 0.0f ),
        VVector4( 0.0f, ( n2 * inv_tb ), 0.0f, 0.0f ),
        VVector4( ( sum_rl * inv_rl ), ( sum_tb * inv_tb ), ( sum_nf * inv_nf ), -1.0f ),
        VVector4( 0.0f, 0.0f, ( ( n2 * inv_nf ) * zFar ), 0.0f )
    );
}

inline const VMatrix4 VMatrix4::orthographic( float left, float right, float bottom, float top, float zNear, float zFar )
{
    float sum_rl, sum_tb, sum_nf, inv_rl, inv_tb, inv_nf;
    sum_rl = ( right + left );
    sum_tb = ( top + bottom );
    sum_nf = ( zNear + zFar );
    inv_rl = ( 1.0f / ( right - left ) );
    inv_tb = ( 1.0f / ( top - bottom ) );
    inv_nf = ( 1.0f / ( zNear - zFar ) );
    return VMatrix4(
        VVector4( ( inv_rl + inv_rl ), 0.0f, 0.0f, 0.0f ),
        VVector4( 0.0f, ( inv_tb + inv_tb ), 0.0f, 0.0f ),
        VVector4( 0.0f, 0.0f, ( inv_nf + inv_nf ), 0.0f ),
        VVector4( ( -sum_rl * inv_rl ), ( -sum_tb * inv_tb ), ( sum_nf * inv_nf ), 1.0f )
    );
}

inline const VMatrix4 select( const VMatrix4 & mat0, const VMatrix4 & mat1, bool select1 )
{
    return VMatrix4(
        select( mat0.getCol0(), mat1.getCol0(), select1 ),
        select( mat0.getCol1(), mat1.getCol1(), select1 ),
        select( mat0.getCol2(), mat1.getCol2(), select1 ),
        select( mat0.getCol3(), mat1.getCol3(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VMatrix4 & mat )
{
    print( mat.getRow( 0 ) );
    print( mat.getRow( 1 ) );
    print( mat.getRow( 2 ) );
    print( mat.getRow( 3 ) );
}

inline void print( const VMatrix4 & mat, const char * name )
{
    printf("%s:\n", name);
    print( mat );
}

#endif

inline VTransform3::VTransform3( const VTransform3 & tfrm )
{
    mCol0 = tfrm.mCol0;
    mCol1 = tfrm.mCol1;
    mCol2 = tfrm.mCol2;
    mCol3 = tfrm.mCol3;
}

inline VTransform3::VTransform3( float scalar )
{
    mCol0 = VVector3( scalar );
    mCol1 = VVector3( scalar );
    mCol2 = VVector3( scalar );
    mCol3 = VVector3( scalar );
}

inline VTransform3::VTransform3( const VVector3 & _col0, const VVector3 & _col1, const VVector3 & _col2, const VVector3 & _col3 )
{
    mCol0 = _col0;
    mCol1 = _col1;
    mCol2 = _col2;
    mCol3 = _col3;
}

inline VTransform3::VTransform3( const VMatrix3 & tfrm, const VVector3 & translateVec )
{
    this->setUpper3x3( tfrm );
    this->setTranslation( translateVec );
}

inline VTransform3::VTransform3( const VQuat & unitVQuat, const VVector3 & translateVec )
{
    this->setUpper3x3( VMatrix3( unitVQuat ) );
    this->setTranslation( translateVec );
}

inline VTransform3 & VTransform3::setCol0( const VVector3 & _col0 )
{
    mCol0 = _col0;
    return *this;
}

inline VTransform3 & VTransform3::setCol1( const VVector3 & _col1 )
{
    mCol1 = _col1;
    return *this;
}

inline VTransform3 & VTransform3::setCol2( const VVector3 & _col2 )
{
    mCol2 = _col2;
    return *this;
}

inline VTransform3 & VTransform3::setCol3( const VVector3 & _col3 )
{
    mCol3 = _col3;
    return *this;
}

inline VTransform3 & VTransform3::setCol( int col, const VVector3 & vec )
{
    *(&mCol0 + col) = vec;
    return *this;
}

inline VTransform3 & VTransform3::setRow( int row, const VVector4 & vec )
{
    mCol0.setElem( row, vec.getElem( 0 ) );
    mCol1.setElem( row, vec.getElem( 1 ) );
    mCol2.setElem( row, vec.getElem( 2 ) );
    mCol3.setElem( row, vec.getElem( 3 ) );
    return *this;
}

inline VTransform3 & VTransform3::setElem( int col, int row, float val )
{
    VVector3 tmpV3_0;
    tmpV3_0 = this->getCol( col );
    tmpV3_0.setElem( row, val );
    this->setCol( col, tmpV3_0 );
    return *this;
}

inline float VTransform3::getElem( int col, int row ) const
{
    return this->getCol( col ).getElem( row );
}

inline const VVector3 VTransform3::getCol0( ) const
{
    return mCol0;
}

inline const VVector3 VTransform3::getCol1( ) const
{
    return mCol1;
}

inline const VVector3 VTransform3::getCol2( ) const
{
    return mCol2;
}

inline const VVector3 VTransform3::getCol3( ) const
{
    return mCol3;
}

inline const VVector3 VTransform3::getCol( int col ) const
{
    return *(&mCol0 + col);
}

inline const VVector4 VTransform3::getRow( int row ) const
{
    return VVector4( mCol0.getElem( row ), mCol1.getElem( row ), mCol2.getElem( row ), mCol3.getElem( row ) );
}

inline VVector3 & VTransform3::operator []( int col )
{
    return *(&mCol0 + col);
}

inline const VVector3 VTransform3::operator []( int col ) const
{
    return *(&mCol0 + col);
}

inline VTransform3 & VTransform3::operator =( const VTransform3 & tfrm )
{
    mCol0 = tfrm.mCol0;
    mCol1 = tfrm.mCol1;
    mCol2 = tfrm.mCol2;
    mCol3 = tfrm.mCol3;
    return *this;
}

inline const VTransform3 inverse( const VTransform3 & tfrm )
{
    VVector3 tmp0, tmp1, tmp2, inv0, inv1, inv2;
    float detinv;
    tmp0 = cross( tfrm.getCol1(), tfrm.getCol2() );
    tmp1 = cross( tfrm.getCol2(), tfrm.getCol0() );
    tmp2 = cross( tfrm.getCol0(), tfrm.getCol1() );
    detinv = ( 1.0f / dot( tfrm.getCol2(), tmp2 ) );
    inv0 = VVector3( ( tmp0.getX() * detinv ), ( tmp1.getX() * detinv ), ( tmp2.getX() * detinv ) );
    inv1 = VVector3( ( tmp0.getY() * detinv ), ( tmp1.getY() * detinv ), ( tmp2.getY() * detinv ) );
    inv2 = VVector3( ( tmp0.getZ() * detinv ), ( tmp1.getZ() * detinv ), ( tmp2.getZ() * detinv ) );
    return VTransform3(
        inv0,
        inv1,
        inv2,
        VVector3( ( -( ( inv0 * tfrm.getCol3().getX() ) + ( ( inv1 * tfrm.getCol3().getY() ) + ( inv2 * tfrm.getCol3().getZ() ) ) ) ) )
    );
}

inline const VTransform3 orthoInverse( const VTransform3 & tfrm )
{
    VVector3 inv0, inv1, inv2;
    inv0 = VVector3( tfrm.getCol0().getX(), tfrm.getCol1().getX(), tfrm.getCol2().getX() );
    inv1 = VVector3( tfrm.getCol0().getY(), tfrm.getCol1().getY(), tfrm.getCol2().getY() );
    inv2 = VVector3( tfrm.getCol0().getZ(), tfrm.getCol1().getZ(), tfrm.getCol2().getZ() );
    return VTransform3(
        inv0,
        inv1,
        inv2,
        VVector3( ( -( ( inv0 * tfrm.getCol3().getX() ) + ( ( inv1 * tfrm.getCol3().getY() ) + ( inv2 * tfrm.getCol3().getZ() ) ) ) ) )
    );
}

inline const VTransform3 absPerElem( const VTransform3 & tfrm )
{
    return VTransform3(
        absPerElem( tfrm.getCol0() ),
        absPerElem( tfrm.getCol1() ),
        absPerElem( tfrm.getCol2() ),
        absPerElem( tfrm.getCol3() )
    );
}

inline const VVector3 VTransform3::operator *( const VVector3 & vec ) const
{
    return VVector3(
        ( ( ( mCol0.getX() * vec.getX() ) + ( mCol1.getX() * vec.getY() ) ) + ( mCol2.getX() * vec.getZ() ) ),
        ( ( ( mCol0.getY() * vec.getX() ) + ( mCol1.getY() * vec.getY() ) ) + ( mCol2.getY() * vec.getZ() ) ),
        ( ( ( mCol0.getZ() * vec.getX() ) + ( mCol1.getZ() * vec.getY() ) ) + ( mCol2.getZ() * vec.getZ() ) )
    );
}

inline const VPoint3 VTransform3::operator *( const VPoint3 & pnt ) const
{
    return VPoint3(
        ( ( ( ( mCol0.getX() * pnt.getX() ) + ( mCol1.getX() * pnt.getY() ) ) + ( mCol2.getX() * pnt.getZ() ) ) + mCol3.getX() ),
        ( ( ( ( mCol0.getY() * pnt.getX() ) + ( mCol1.getY() * pnt.getY() ) ) + ( mCol2.getY() * pnt.getZ() ) ) + mCol3.getY() ),
        ( ( ( ( mCol0.getZ() * pnt.getX() ) + ( mCol1.getZ() * pnt.getY() ) ) + ( mCol2.getZ() * pnt.getZ() ) ) + mCol3.getZ() )
    );
}

inline const VTransform3 VTransform3::operator *( const VTransform3 & tfrm ) const
{
    return VTransform3(
        ( *this * tfrm.mCol0 ),
        ( *this * tfrm.mCol1 ),
        ( *this * tfrm.mCol2 ),
        VVector3( ( *this * VPoint3( tfrm.mCol3 ) ) )
    );
}

inline VTransform3 & VTransform3::operator *=( const VTransform3 & tfrm )
{
    *this = *this * tfrm;
    return *this;
}

inline const VTransform3 mulPerElem( const VTransform3 & tfrm0, const VTransform3 & tfrm1 )
{
    return VTransform3(
        mulPerElem( tfrm0.getCol0(), tfrm1.getCol0() ),
        mulPerElem( tfrm0.getCol1(), tfrm1.getCol1() ),
        mulPerElem( tfrm0.getCol2(), tfrm1.getCol2() ),
        mulPerElem( tfrm0.getCol3(), tfrm1.getCol3() )
    );
}

inline const VTransform3 VTransform3::identity( )
{
    return VTransform3(
        VVector3::xAxis( ),
        VVector3::yAxis( ),
        VVector3::zAxis( ),
        VVector3( 0.0f )
    );
}

inline VTransform3 & VTransform3::setUpper3x3( const VMatrix3 & tfrm )
{
    mCol0 = tfrm.getCol0();
    mCol1 = tfrm.getCol1();
    mCol2 = tfrm.getCol2();
    return *this;
}

inline const VMatrix3 VTransform3::getUpper3x3( ) const
{
    return VMatrix3( mCol0, mCol1, mCol2 );
}

inline VTransform3 & VTransform3::setTranslation( const VVector3 & translateVec )
{
    mCol3 = translateVec;
    return *this;
}

inline const VVector3 VTransform3::getTranslation( ) const
{
    return mCol3;
}

inline const VTransform3 VTransform3::rotationX( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VTransform3(
        VVector3::xAxis( ),
        VVector3( 0.0f, c, s ),
        VVector3( 0.0f, -s, c ),
        VVector3( 0.0f )
    );
}

inline const VTransform3 VTransform3::rotationY( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VTransform3(
        VVector3( c, 0.0f, -s ),
        VVector3::yAxis( ),
        VVector3( s, 0.0f, c ),
        VVector3( 0.0f )
    );
}

inline const VTransform3 VTransform3::rotationZ( float radians )
{
    float s, c;
    s = sinf( radians );
    c = cosf( radians );
    return VTransform3(
        VVector3( c, s, 0.0f ),
        VVector3( -s, c, 0.0f ),
        VVector3::zAxis( ),
        VVector3( 0.0f )
    );
}

inline const VTransform3 VTransform3::rotationZYX( const VVector3 & radiansXYZ )
{
    float sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf( radiansXYZ.getX() );
    cX = cosf( radiansXYZ.getX() );
    sY = sinf( radiansXYZ.getY() );
    cY = cosf( radiansXYZ.getY() );
    sZ = sinf( radiansXYZ.getZ() );
    cZ = cosf( radiansXYZ.getZ() );
    tmp0 = ( cZ * sY );
    tmp1 = ( sZ * sY );
    return VTransform3(
        VVector3( ( cZ * cY ), ( sZ * cY ), -sY ),
        VVector3( ( ( tmp0 * sX ) - ( sZ * cX ) ), ( ( tmp1 * sX ) + ( cZ * cX ) ), ( cY * sX ) ),
        VVector3( ( ( tmp0 * cX ) + ( sZ * sX ) ), ( ( tmp1 * cX ) - ( cZ * sX ) ), ( cY * cX ) ),
        VVector3( 0.0f )
    );
}

inline const VTransform3 VTransform3::rotation( float radians, const VVector3 & unitVec )
{
    return VTransform3( VMatrix3::rotation( radians, unitVec ), VVector3( 0.0f ) );
}

inline const VTransform3 VTransform3::rotation( const VQuat & unitVQuat )
{
    return VTransform3( VMatrix3( unitVQuat ), VVector3( 0.0f ) );
}

inline const VTransform3 VTransform3::scale( const VVector3 & scaleVec )
{
    return VTransform3(
        VVector3( scaleVec.getX(), 0.0f, 0.0f ),
        VVector3( 0.0f, scaleVec.getY(), 0.0f ),
        VVector3( 0.0f, 0.0f, scaleVec.getZ() ),
        VVector3( 0.0f )
    );
}

inline const VTransform3 appendScale( const VTransform3 & tfrm, const VVector3 & scaleVec )
{
    return VTransform3(
        ( tfrm.getCol0() * scaleVec.getX( ) ),
        ( tfrm.getCol1() * scaleVec.getY( ) ),
        ( tfrm.getCol2() * scaleVec.getZ( ) ),
        tfrm.getCol3()
    );
}

inline const VTransform3 prependScale( const VVector3 & scaleVec, const VTransform3 & tfrm )
{
    return VTransform3(
        mulPerElem( tfrm.getCol0(), scaleVec ),
        mulPerElem( tfrm.getCol1(), scaleVec ),
        mulPerElem( tfrm.getCol2(), scaleVec ),
        mulPerElem( tfrm.getCol3(), scaleVec )
    );
}

inline const VTransform3 VTransform3::translation( const VVector3 & translateVec )
{
    return VTransform3(
        VVector3::xAxis( ),
        VVector3::yAxis( ),
        VVector3::zAxis( ),
        translateVec
    );
}

inline const VTransform3 select( const VTransform3 & tfrm0, const VTransform3 & tfrm1, bool select1 )
{
    return VTransform3(
        select( tfrm0.getCol0(), tfrm1.getCol0(), select1 ),
        select( tfrm0.getCol1(), tfrm1.getCol1(), select1 ),
        select( tfrm0.getCol2(), tfrm1.getCol2(), select1 ),
        select( tfrm0.getCol3(), tfrm1.getCol3(), select1 )
    );
}

#ifdef _VECTORMATH_DEBUG

inline void print( const VTransform3 & tfrm )
{
    print( tfrm.getRow( 0 ) );
    print( tfrm.getRow( 1 ) );
    print( tfrm.getRow( 2 ) );
}

inline void print( const VTransform3 & tfrm, const char * name )
{
    printf("%s:\n", name);
    print( tfrm );
}

#endif

inline VQuat::VQuat( const VMatrix3 & tfrm )
{
    float trace, radicand, scale, xx, yx, zx, xy, yy, zy, xz, yz, zz, tmpx, tmpy, tmpz, tmpw, qx, qy, qz, qw;
    int negTrace, ZgtX, ZgtY, YgtX;
    int largestXorY, largestYorZ, largestZorX;

    xx = tfrm.getCol0().getX();
    yx = tfrm.getCol0().getY();
    zx = tfrm.getCol0().getZ();
    xy = tfrm.getCol1().getX();
    yy = tfrm.getCol1().getY();
    zy = tfrm.getCol1().getZ();
    xz = tfrm.getCol2().getX();
    yz = tfrm.getCol2().getY();
    zz = tfrm.getCol2().getZ();

    trace = ( ( xx + yy ) + zz );

    negTrace = ( trace < 0.0f );
    ZgtX = zz > xx;
    ZgtY = zz > yy;
    YgtX = yy > xx;
    largestXorY = ( !ZgtX || !ZgtY ) && negTrace;
    largestYorZ = ( YgtX || ZgtX ) && negTrace;
    largestZorX = ( ZgtY || !YgtX ) && negTrace;
    
    if ( largestXorY )
    {
        zz = -zz;
        xy = -xy;
    }
    if ( largestYorZ )
    {
        xx = -xx;
        yz = -yz;
    }
    if ( largestZorX )
    {
        yy = -yy;
        zx = -zx;
    }

    radicand = ( ( ( xx + yy ) + zz ) + 1.0f );
    scale = ( 0.5f * ( 1.0f / sqrtf( radicand ) ) );

    tmpx = ( ( zy - yz ) * scale );
    tmpy = ( ( xz - zx ) * scale );
    tmpz = ( ( yx - xy ) * scale );
    tmpw = ( radicand * scale );
    qx = tmpx;
    qy = tmpy;
    qz = tmpz;
    qw = tmpw;

    if ( largestXorY )
    {
        qx = tmpw;
        qy = tmpz;
        qz = tmpy;
        qw = tmpx;
    }
    if ( largestYorZ )
    {
        tmpx = qx;
        tmpz = qz;
        qx = qy;
        qy = tmpx;
        qz = qw;
        qw = tmpz;
    }

    mX = qx;
    mY = qy;
    mZ = qz;
    mW = qw;
}

inline const VMatrix3 outer( const VVector3 & tfrm0, const VVector3 & tfrm1 )
{
    return VMatrix3(
        ( tfrm0 * tfrm1.getX( ) ),
        ( tfrm0 * tfrm1.getY( ) ),
        ( tfrm0 * tfrm1.getZ( ) )
    );
}

inline const VMatrix4 outer( const VVector4 & tfrm0, const VVector4 & tfrm1 )
{
    return VMatrix4(
        ( tfrm0 * tfrm1.getX( ) ),
        ( tfrm0 * tfrm1.getY( ) ),
        ( tfrm0 * tfrm1.getZ( ) ),
        ( tfrm0 * tfrm1.getW( ) )
    );
}

inline const VVector3 rowMul( const VVector3 & vec, const VMatrix3 & mat )
{
    return VVector3(
        ( ( ( vec.getX() * mat.getCol0().getX() ) + ( vec.getY() * mat.getCol0().getY() ) ) + ( vec.getZ() * mat.getCol0().getZ() ) ),
        ( ( ( vec.getX() * mat.getCol1().getX() ) + ( vec.getY() * mat.getCol1().getY() ) ) + ( vec.getZ() * mat.getCol1().getZ() ) ),
        ( ( ( vec.getX() * mat.getCol2().getX() ) + ( vec.getY() * mat.getCol2().getY() ) ) + ( vec.getZ() * mat.getCol2().getZ() ) )
    );
}

inline const VMatrix3 crossMatrix( const VVector3 & vec )
{
    return VMatrix3(
        VVector3( 0.0f, vec.getZ(), -vec.getY() ),
        VVector3( -vec.getZ(), 0.0f, vec.getX() ),
        VVector3( vec.getY(), -vec.getX(), 0.0f )
    );
}

inline const VMatrix3 crossMatrixMul( const VVector3 & vec, const VMatrix3 & mat )
{
    return VMatrix3( cross( vec, mat.getCol0() ), cross( vec, mat.getCol1() ), cross( vec, mat.getCol2() ) );
}

} // namespace Aos
} // namespace Vectormath

namespace vmath
{
    using namespace Vectormath::Aos;

    inline VPoint3 project(VPoint3 p, VPoint3 a, VPoint3 b)
    {
        float t = dot(b - a, p - a) / distSqr(a, b);
        return a + t * (b - a);
    }

    inline VMatrix4 pick_box(float centerX, float centerY, float width, float height, int viewport[4])
    {
        float sx = viewport[2] / width;
        float sy = viewport[3] / height;
        float tx = (viewport[2] + 2.0f * (viewport[0] - centerX)) / width;
        float ty = (viewport[3] + 2.0f * (viewport[1] - centerY)) / height;

        VVector4 c0(sx, 0, 0, tx);
        VVector4 c1(0, sy, 0, ty);
        VVector4 c2(0, 0, 1, 0);
        VVector4 c3(0, 0, 0, 1);

        return transpose(VMatrix4(c0, c1, c2, c3));
    }

    inline VPoint3 perspective(VVector4 v)
    {
        return VPoint3(v.getX() / v.getW(), v.getY() / v.getW(), v.getZ() / v.getW());
    }

    inline VVector3 perp(VVector3 a)
    {
        VVector3 c = VVector3(1, 0, 0);
        VVector3 b = cross(a, c);
        if (lengthSqr(b) < 0.01f)
        {
            c = VVector3(0, 1, 0);
            b = cross(a, c);
        }
        return b;
    }

    inline VQuat rotate(VQuat a, VQuat b)
    {
        float w = a.getW() * b.getW() - a.getX() * b.getX() - a.getY() * b.getY() - a.getZ() * b.getZ();
        float x = a.getW() * b.getX() + a.getX() * b.getW() + a.getY() * b.getZ() - a.getZ() * b.getY();
        float y = a.getW() * b.getY() + a.getY() * b.getW() + a.getZ() * b.getX() - a.getX() * b.getZ();
        float z = a.getW() * b.getZ() + a.getZ() * b.getW() + a.getX() * b.getY() - a.getY() * b.getX();
        VQuat q(x, y, z, w);
        return normalize(q);
    }
}
