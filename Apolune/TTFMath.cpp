// *************************************************************************************
//	TTFMath.cpp
//
//		Implementation of the TTF math library.
//
//																															Ryan Bogean
//																															April 2012
//																															January 2014
//
// *************************************************************************************

#include <exception>
#include <vector>
#include <map>
#include <cstdint>
#include <cmath>

#include "TTFMath.h"


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	floating point 'library'
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec2f -----
vec2f::vec2f () {}
vec2f::vec2f (float x_, float y_) : x(x_), y(y_) {}

bool vec2f::operator== (vec2f v) const { return x == v.x && y == v.y; }
bool vec2f::operator!= (vec2f v) const { return x != v.x || y != v.y; }

vec2f& vec2f::operator+= (vec2f v) {
	x += v.x;
	y += v.y;
	return *this;
	}

vec2f& vec2f::operator-= (vec2f v) {
	x -= v.x;
	y -= v.y;
	return *this;
	}

vec2f& vec2f::operator*= (vec2f v) {
	x *= v.x;
	y *= v.y;
	return *this;
	}

vec2f& vec2f::operator/= (vec2f v) {
	x /= v.x;
	y /= v.y;
	return *this;
	}

vec2f& vec2f::operator+= (float f) {
	x += f;
	y += f;
	return *this;
	}

vec2f& vec2f::operator-= (float f) {
	x -= f;
	y -= f;
	return *this;
	}

vec2f& vec2f::operator*= (float f) {
	x *= f;
	y *= f;
	return *this;
	}

vec2f& vec2f::operator/= (float f) {
	x /= f;
	y /= f;
	return *this;
	}

vec2f TTFCore::operator+ (vec2f v0, vec2f v1) {
	return vec2f(v0.x + v1.x, v0.y + v1.y);
	}

vec2f TTFCore::operator- (vec2f v0, vec2f v1) {
	return vec2f(v0.x - v1.x, v0.y - v1.y);
	}

vec2f TTFCore::operator* (vec2f v0, vec2f v1) {
	return vec2f(v0.x * v1.x, v0.y * v1.y);
	}

vec2f TTFCore::operator/ (vec2f v0, vec2f v1) {
	return vec2f(v0.x / v1.x, v0.y / v1.y);
	}

vec2f TTFCore::operator+ (float f, vec2f v1) {
	return vec2f(f + v1.x, f + v1.y);
	}

vec2f TTFCore::operator- (float f, vec2f v1) {
	return vec2f(f - v1.x, f - v1.y);
	}

vec2f TTFCore::operator* (float f, vec2f v1) {
	return vec2f(f * v1.x, f * v1.y);
	}

vec2f TTFCore::operator/ (float f, vec2f v1) {
	return vec2f(f / v1.x, f / v1.y);
	}

vec2f TTFCore::operator+ (vec2f v0, float f) {
	return vec2f(v0.x + f, v0.y + f);
	}

vec2f TTFCore::operator- (vec2f v0, float f) {
	return vec2f(v0.x - f, v0.y - f);
	}

vec2f TTFCore::operator* (vec2f v0, float f) {
	return vec2f(v0.x * f, v0.y * f);
	}

vec2f TTFCore::operator/ (vec2f v0, float f) {
	return vec2f(v0.x / f, v0.y / f);
	}

float TTFCore::dot (vec2f v0, vec2f v1) {
	return (v0.x*v1.x) + (v0.y*v1.y);
	}

float TTFCore::cross (vec2f v0, vec2f v1) {
	return (v0.x*v1.y) - (v0.y*v1.x);
	}


// ----- matrix3x2f -----
matrix3x2f::matrix3x2f () : a(1.0f), b(0.0f), c(0.0f), d(1.0f), e(0.0f), f(0.0f) {}

vec2f TTFCore::mul (vec2f v, const matrix3x2f& m) {
	// obviously you can't multiply a 2d vector with a 3x2 matrix
	// in this case we 'extend' the 2d vector to a 3d vector by appending 1.0f
	// course this need'nt be done explicitly

	return vec2f( v.x*m.a + v.y*m.c + m.e , v.x*m.b + v.y*m.d + m.f );
	}

vec2f TTFCore::normal (vec2f v) {
	return v / std::sqrt(dot(v,v));
	}

vec2f TTFCore::lerp (vec2f p0, vec2f p1, float f) {
	if (f <= 0.0f) return p0;
	if (f >= 1.0f) return p1;
	return p0 + ((p1 - p0) * f);
	}

vec2f TTFCore::quad_lerp (vec2f p0, vec2f p1, vec2f p2, float f) {
	if (f <= 0.0f) return p0;
	if (f >= 1.0f) return p1;
	vec2f t0 = p0 + ((p1 - p0) * f);
	vec2f t1 = p1 + ((p2 - p1) * f);
	return t0 + ((t1 - t0) * f);
	}



// ---------------------------------------------------------------------------------------------------------------------------
//	vec3f / vec4f
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec3f -----
vec3f::vec3f () {}
vec3f::vec3f (float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}


// ----- vec4f -----
vec4f::vec4f () {}
vec4f::vec4f (float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}



// ---------------------------------------------------------------------------------------------------------------------------
//	interger functions/types
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec2s -----
vec2s::vec2s () {}
vec2s::vec2s (int16_t x_, int16_t y_) : x(x_), y(y_) {}

bool vec2s::operator== (vec2s v) const { return x == v.x && y == v.y; }
bool vec2s::operator!= (vec2s v) const { return x != v.x || y != v.y; }

vec2s& vec2s::operator+= (vec2s v) {
	x += v.x;
	y += v.y;
	return *this;
	}

vec2s& vec2s::operator-= (vec2s v) {
	x -= v.x;
	y -= v.y;
	return *this;
	}

vec2s& vec2s::operator*= (vec2s v) {
	x *= v.x;
	y *= v.y;
	return *this;
	}

vec2s& vec2s::operator/= (vec2s v) {
	x /= v.x;
	y /= v.y;
	return *this;
	}

vec2s& vec2s::operator+= (int16_t v) {
	x += v;
	y += v;
	return *this;
	}

vec2s& vec2s::operator-= (int16_t v) {
	x -= v;
	y -= v;
	return *this;
	}

vec2s& vec2s::operator*= (int16_t v) {
	x *= v;
	y *= v;
	return *this;
	}

vec2s& vec2s::operator/= (int16_t v) {
	x /= v;
	y /= v;
	return *this;
	}

vec2s TTFCore::operator+ (vec2s v0, vec2s v1) {
	return vec2s(v0.x + v1.x, v0.y + v1.y);
	}

vec2s TTFCore::operator- (vec2s v0, vec2s v1) {
	return vec2s(v0.x - v1.x, v0.y - v1.y);
	}

vec2s TTFCore::operator* (vec2s v0, vec2s v1) {
	return vec2s(v0.x * v1.x, v0.y * v1.y);
	}

vec2s TTFCore::operator/ (vec2s v0, vec2s v1) {
	return vec2s(v0.x / v1.x, v0.y / v1.y);
	}

vec2s TTFCore::operator+ (int16_t v, vec2s v1) {
	return vec2s(v + v1.x, v + v1.y);
	}

vec2s TTFCore::operator- (int16_t v, vec2s v1) {
	return vec2s(v - v1.x, v - v1.y);
	}

vec2s TTFCore::operator* (int16_t v, vec2s v1) {
	return vec2s(v * v1.x, v * v1.y);
	}

vec2s TTFCore::operator/ (int16_t v, vec2s v1) {
	return vec2s(v / v1.x, v / v1.y);
	}

vec2s TTFCore::operator+ (vec2s v0, int16_t v) {
	return vec2s(v0.x + v, v0.y + v);
	}

vec2s TTFCore::operator- (vec2s v0, int16_t v) {
	return vec2s(v0.x - v, v0.y - v);
	}

vec2s TTFCore::operator* (vec2s v0, int16_t v) {
	return vec2s(v0.x * v, v0.y * v);
	}

vec2s TTFCore::operator/ (vec2s v0, int16_t v) {
	return vec2s(v0.x / v, v0.y / v);
	}

int32_t TTFCore::dot (vec2s v0, vec2s v1) {

	int32_t a = v0.x;
	int32_t b = v0.y;
	int32_t c = v1.x;
	int32_t d = v1.y;

	return (a*c) + (b*d);
	}

int32_t TTFCore::cross (vec2s v0, vec2s v1) {

	int32_t a = v0.x;
	int32_t b = v0.y;
	int32_t c = v1.x;
	int32_t d = v1.y;

	return (a*d) - (b*c);
	}


// ----- matrix3x2s -----
matrix3x2s::matrix3x2s () {
	a = 1 << 16;
	b = 0;
	c = 0;
	d = 1 << 16;
	e = 0;
	f = 0;	
	}

vec2s TTFCore::mul (vec2s v, const matrix3x2s& m) {
	
	const int32_t round = 1 << 15;

	int32_t x = v.x;
	int32_t y = v.y;

	vec2s result;
 result.x=static_cast<int16_t>((x*m.a + y*m.c + m.e + round) >> 16);
	result.y=static_cast<int16_t>((x*m.b + y*m.d + m.f + round) >> 16);
 return result;
	}


// ----- vec3s -----
vec3s::vec3s () {}

vec3s::vec3s (int16_t x_, int16_t y_, int16_t z_) {
	x = x_;
	y = y_;
	z = z_;
	}

vec3s::vec3s (vec2s v) {
	x = v.x;
	y = v.y;
	z = 0;
	}

vec3s::operator vec3f () const {
	 vec3f result;
  result.x=static_cast<float>(x);
  result.y=static_cast<float>(y);
  result.z=static_cast<float>(z);	
  return result;
	}

vec3s TTFCore::mul (vec3s v, const matrix3x2s& m) {
	// not really a true multiply

	const int32_t round = 1 << 15;

	int32_t x = v.x;
	int32_t y = v.y;

	vec3s result;
 result.x=static_cast<int16_t>((x*m.a + y*m.c + m.e + round) >> 16);
 result.y=static_cast<int16_t>((x*m.b + y*m.d + m.f + round) >> 16);
 result.z=v.z;
 return result;
	}


// ----- vec4s -----
vec4s::vec4s () {}

vec4s::vec4s (int16_t x_, int16_t y_, int16_t z_, int16_t w_) {
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	}


// ----- various functions -----
int16_t TTFCore::RoundBy16 (int32_t x) {
	const int32_t round = 1 << 15;
	if (x >= 0) return static_cast<int16_t>((x + round) >> 16);
	else return static_cast<int16_t>((x - round) >> 16);
	}

int32_t TTFCore::RoundBy16Large (int32_t x) {
	const int32_t round = 1 << 15;
	if (x >= 0) return (x + round) >> 16;
	else return (x - round) >> 16;
	}

int16_t TTFCore::RoundBy2 (int16_t x) {
	if (x >= 0) return (x + 1) >> 1;
	else return (x - 1) >> 1;
	}

vec2s TTFCore::RoundBy2 (vec2s v) {
	return vec2s(RoundBy2(v.x), RoundBy2(v.y));
	}

vec2s TTFCore::lerp (vec2s p0, vec2s p1, uint16_t f) {
	int32_t dx = static_cast<int32_t>(p1.x - p0.x) * static_cast<int32_t>(f);
	int32_t dy = static_cast<int32_t>(p1.y - p0.y) * static_cast<int32_t>(f);
	return vec2s(p0.x + RoundBy16(dx), p0.y + RoundBy16(dy));
	}

vec2s TTFCore::quad_lerp (vec2s p0, vec2s p1, vec2s p2, uint16_t f) {
	return lerp(lerp(p0,p1,f),lerp(p1,p2,f),f);
	}
