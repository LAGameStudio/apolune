// *************************************************************************************
//	TTFMath.h
//
//		A small snippet from a larger math library.
//		Here in case I want to share TTF without needing to include everything.
//																																		Ryan Boghean
//																																		April 2012
//																																		January 2014
//
// *************************************************************************************

#pragma once

#include <exception>
#include <vector>
#include <map>
#include <cstdint>

#include "TTFExceptions.h"



// ---------------------------------------------------------------------------------------------------------------------------
//	TTFCore namespace
// ---------------------------------------------------------------------------------------------------------------------------

namespace TTFCore {



// ---------------------------------------------------------------------------------------------------------------------------
//	Simple math 'library'
//		- bare minimum needed to handle TTF math
//		- nothing special
//
//		implemented in: TTFMath.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec2f -----
struct vec2f {
	float x, y;

	vec2f ();
	vec2f (float x, float y);

	bool operator== (vec2f) const;
	bool operator!= (vec2f) const;

	vec2f& operator+= (vec2f);
	vec2f& operator-= (vec2f);
	vec2f& operator*= (vec2f);
	vec2f& operator/= (vec2f);

	vec2f& operator+= (float);
	vec2f& operator-= (float);
	vec2f& operator*= (float);
	vec2f& operator/= (float);
	};

vec2f operator+ (vec2f, vec2f);
vec2f operator- (vec2f, vec2f);
vec2f operator* (vec2f, vec2f);
vec2f operator/ (vec2f, vec2f);

vec2f operator+ (float, vec2f);
vec2f operator- (float, vec2f);
vec2f operator* (float, vec2f);
vec2f operator/ (float, vec2f);

vec2f operator+ (vec2f, float);
vec2f operator- (vec2f, float);
vec2f operator* (vec2f, float);
vec2f operator/ (vec2f, float);

float dot (vec2f, vec2f);				// 2d dot product
float cross (vec2f, vec2f);				// 2d cross product


// ----- matrix3x2f -----
struct matrix3x2f {
	float a, b, c, d, e, f;
	matrix3x2f ();		// intiailized to the identity
	};

vec2f mul (vec2f, const matrix3x2f&);
vec2f normal (vec2f);
vec2f lerp (vec2f, vec2f, float);							// linear interpolate between the two points
vec2f quad_lerp (vec2f, vec2f, vec2f, float);		// quadratic interpolation


// ----- vec3f -----
struct vec3f {
	float x, y, z;
	vec3f ();
	vec3f (float x, float y, float z);
	};


// ----- vec4f -----
struct vec4f {
	float x, y, z, w;
	vec4f ();
	vec4f (float x, float y, float z, float w);
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	integer versions
//
//		implemented in: TTFMath.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec2s -----
struct vec2s {
	int16_t x, y;

	vec2s ();
	vec2s (int16_t x, int16_t y);

	bool operator== (vec2s) const;
	bool operator!= (vec2s) const;

	vec2s& operator+= (vec2s);
	vec2s& operator-= (vec2s);
	vec2s& operator*= (vec2s);
	vec2s& operator/= (vec2s);

	vec2s& operator+= (int16_t);
	vec2s& operator-= (int16_t);
	vec2s& operator*= (int16_t);
	vec2s& operator/= (int16_t);
	};

vec2s operator+ (vec2s, vec2s);
vec2s operator- (vec2s, vec2s);
vec2s operator* (vec2s, vec2s);
vec2s operator/ (vec2s, vec2s);

vec2s operator+ (int16_t, vec2s);
vec2s operator- (int16_t, vec2s);
vec2s operator* (int16_t, vec2s);
vec2s operator/ (int16_t, vec2s);

vec2s operator+ (vec2s, int16_t);
vec2s operator- (vec2s, int16_t);
vec2s operator* (vec2s, int16_t);
vec2s operator/ (vec2s, int16_t);

int32_t dot (vec2s, vec2s);					// 2d dot product
int32_t cross (vec2s, vec2s);				// 2d cross product


// ----- matrix3x2s -----
struct matrix3x2s {
	int32_t a, b, c, d, e, f;			// in 16.16 fixed point format
	matrix3x2s ();						// intialized to the identity
	};

vec2s mul (vec2s, const matrix3x2s&);


// ----- vec3s -----
struct vec3s {
	int16_t x, y, z;
	vec3s ();
	vec3s (int16_t x, int16_t y, int16_t z);
	vec3s (vec2s);

	operator vec3f () const;
	};

vec3s mul (vec3s, const matrix3x2s&);

// ----- vec4s -----
struct vec4s {
	int16_t x, y, z, w;
	vec4s ();
	vec4s (int16_t x, int16_t y, int16_t z, int16_t w);
	};


// ----- various functions -----
int16_t RoundBy16 (int32_t);				// divides by 16 bits and rounds
int32_t RoundBy16Large (int32_t);		// divides by 16 bits and rounds
int16_t RoundBy2 (int16_t);					// divides by 2 and rounds
vec2s RoundBy2 (vec2s);						// divides by 2 and rounds

vec2s lerp (vec2s, vec2s, uint16_t);							// linear interpolate between the two points
vec2s quad_lerp (vec2s, vec2s, vec2s, uint16_t);		// quadratic interpolation



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
