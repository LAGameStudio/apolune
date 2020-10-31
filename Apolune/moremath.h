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
#include <limits>
#include <type_traits>

// Forward declared from ZeroTypes.h
class Zdouble;
class Zfloat;
class Zint;

#define NOOPd 98765432.10

#define SQRTOF2 1.414213562373095

#define MOD(x,y)  (sqrt((x)*(x)+(y)*(y))) /* modulus of a 2d vector */
#define ANGLE(x,y) (atan2(y,x))           /* angle of a 2d vector */

float norm_deg( float angle );
double norm_deg( double angle );
float norm_rad( float angle );
double norm_rad( double angle );
float norm_deg( Zfloat angle );
float norm_rad( Zfloat angle );
double norm_deg( Zdouble angle );
double norm_rad( Zdouble angle );

#define LINE_ANGLE(x,y,x2,y2)  atan2( (float) (y2-y), (float) (x2-x) )

// Prototypes
#if defined(NEVER)
unsigned long getTime(void);
unsigned long getMillis(void);
struct _timeb GetTime(void);
#endif
void init_seeder(void);
void reseed(void);
double uniform (void);
double uniform ( bool reseed );
#define PSEUDORAND_MAX 0x7fff
int pseudorandom( unsigned int s, unsigned int x );
double random(double x);
int iround ( float f );
int iround ( double d );
int ilerp( int start, int end, float percent );
int ilerpw( int start, int end, float percent );
int ilerp( int start, int end, double percent );
int ilerpw( int start, int end, double percent );
double double_range( double L, double H );
float float_range( float L, float H );
int number_range( int L, int H );
int upto( int M );
int upto( int M, int seed );
#define DEG2RAD(aD) (aD*PI/180.0)
#define RAD2DEG(aR) (aR*180.0/PI)
double deg2rad( double deg );
double rad2deg( double rad );
float deg2rad( float deg );
float rad2deg( float rad );
void init_isqrt(void);
float isqrt(int i);
float iratiof( int i, int y );
double iratiod( int i, int y );
double uiratiod( unsigned int i, unsigned int y );
int fiaddi( int a, float f );
int faddi( float a, float f );
float wrap( float c, float a, float b );
double wrap( double c, double a, double b );
int iwrap( int c, int a, int b );
double ddistance( double x, double y, double xx, double yy );
double ddistance( double x, double y, double z, double xx, double yy, double zz );
float fdistance( float x, float y, float xx, float yy );
float fdistance( float x, float y, float z, float xx, float yy, float zz );
float idistance( int x, int y, int xx, int yy );
float idistance( float x, float y, float xx, float yy );
float idistance( double x, double y, double xx, double yy );
float idistance( int x, int y, int z, int xx, int yy, int zz );
float idistance( float x, float y, float z, float xx, float yy, float zz );
float idistance( double x, double y, double z, double xx, double yy, double zz );
float idistance( int x, int y, Zint xx, Zint yy );
float idistance( Zint x, Zint y, int xx, int yy );
float idistance( Zint x, Zint y, Zint xx, Zint yy );
float idistance( Zfloat x, Zfloat y, Zfloat xx, Zfloat yy );
float idistance( Zdouble x, Zdouble y, Zdouble xx, Zdouble yy );
float idistance( Zint x, Zint y, Zint z, Zint xx, Zint yy, Zint zz );
float idistance( Zfloat x, Zfloat y, Zfloat z, Zfloat xx, Zfloat yy, Zfloat zz );
float idistance( Zdouble x, Zdouble y, Zdouble z, Zdouble xx, Zdouble yy, Zdouble zz );
float rsqrtf(const float v);
float sqrf(const float x);
float sincf(const float x);
#define roundf(x) floorf((x) + 0.5f)
// Not recommended (can store b-a) :
#define INTERPf(a,b,c) (complete*(b-a)+a)
inline float intAdjustf(const float x, const float diff);
inline float lerpf(const float a, const float b, const float x);
double lerpd( double a, double b, double x );
inline float cerpf(const float a, const float b, const float c, const float d, const float x);
unsigned int getClosestPowerOfTwo(const unsigned int x);
int even_dilate(const int val);
#define odd_dilate(val) ((even_dilate(val) << 1))
#define mIndex2D(row, col) (even_dilate(row) | odd_dilate(col))
#define mIndex2DPadded(row, col, size) ((row < size)? ((col < size)? mIndex2D(row, col) : (size) * (size) + row) : (size) * (size + 1) + col)
int floatToIntBits(float f);
int NVfloatToIntBits(float f);
//void test_is_same();
double mapRanged(double inputValue, double inputMin, double inputMax, double outputMin, double outputMax);
float mapRangef(float inputValue, float inputMin, float inputMax, float outputMin, float outputMax);
float percf(float inputValue, float inputMin, float inputMax); 
float RandomSign(void);
float frequency(float v, float one);
float absf( float f );
int absi( int i );
double absd( double d );

extern double tickerMinute;
extern double tickerHour;
void ticker();

// Perlin
#define p_lerp(t, a, b) ( a + t * (b - a) )
double Noise(int x);
double Noise(double x);
double Noise(int x, int y);
double Noise(double x, double y);
double Interpolate( double a, double b, double x );
double CosInterpolate( double a, double b, double x );
double CubicInterpolate(double v0, double v1, double v2, double v3, double x);
double InterpolatedNoise(double x);
double InterpolatedNoise(double x, double y);
double PerlinNoise_1D(double x, int octaves, int persistence );
double PerlinNoise_2D(double x, double y, int octaves, int persistence );
double PerlinNoise1D(double x,double alpha,double beta,int n);
double PerlinNoise2D(double x,double y,double alpha,double beta,int n);
double PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n);

double Noise1(const double x);
double Noise2(const double x, const double y);
double Noise3(const double x, const double y, const double z);
double Turbulence2(const double x, const double y, double freq);
double Turbulence3(const double x, const double y, const double z, double freq);
double TileableNoise1(const double x, const double w);
double TileableNoise2(const double x, const double y, const double w, const double h);
double TileableNoise3(const double x, const double y, const double z, const double w, const double h, const double d);
double TileableTurbulence2(const double x, const double y, const double w, const double h, double freq);
double TileableTurbulence3(const double x, const double y, const double z, const double w, const double h, const double d, double freq);
void InitPerlin();

extern double cos90,sin90,cos180,sin180,cos270,sin270;
void InitMath();

#ifndef __REALTOINT_H
#define __REALTOINT_H
#pragma once


typedef double lreal;
typedef float  real;
#ifndef STEAMTYPES_H
#ifdef USE_LONG_AS_UINT
typedef unsigned long uint32;
typedef long int32;
#else
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef int64 lint64;
typedef uint64 ulint64;
#endif
#endif

const lreal _double2fixmagic = 68719476736.0*1.5;     //2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
const int32 _shiftamt        = 16;                    //16.16 fixed point representation,

#if BigEndian_
	#define iexp_				0
	#define iman_				1
#else
	#define iexp_				1
	#define iman_				0
#endif //BigEndian_

// ================================================================================================
// Real2Int
// ================================================================================================
inline int32 Real2Int(lreal val)
{
#if DEFAULT_CONVERSION
	return val;
#else
	val		= val + _double2fixmagic;
	return ((int32*)&val)[iman_] >> _shiftamt;    /// Cppcheck's Portability note: double * to integer * is not portable due to different binary data representations on different platforms
#endif
}

// ================================================================================================
// Real2Int
// ================================================================================================
inline int32 Real2Int(real val)
{
#if DEFAULT_CONVERSION
	return val;
#else
	return Real2Int ((lreal)val);
#endif
}

#endif //__REALTOINT_H

/*
#pragma once
template <class T>
T nearestpower2(T v)
{
        int k;
        if (v == 0)
                return 1;
        for (k = sizeof(T) * 8 - 1; ((static_cast<T>(1U) << k) & v) == 0; k--);
        if (((static_cast<T>(1U) << (k - 1)) & v) == 0)
                return static_cast<T>(1U) << k;
        return static_cast<T>(1U) << (k + 1);
}
template<typename T> inline bool isnan(T value) { return value != value; }
template<typename T> inline bool isinf(T value) { 
 return std::numeric_limits<T>::has_infinity &&
 value == std::numeric_limits<T>::infinity();
}
template <typename T, typename T2> T cast_dammit_cast(T2 what){ return *((T*) &what); }
template<class Ty1, class Ty2> struct is_same;
struct base { int val; };
struct derived : public base {}; 
*/

bool LiangBarsky (double edgeLeft, double edgeRight, double edgeBottom, double edgeTop, 
                  double x0src, double y0src, double x1src, double y1src,               
                  double *x0clip, double *y0clip, double *x1clip, double *y1clip);

int PntOnLine(int px, int py, int qx, int qy, int tx, int ty);

int isPowerOfTwo (unsigned int x);

double drint( double x );
int irint( double x );

// Per-Frame High Performance Timer Query
extern uint64 LAST_TICK,TICK,TICKSECS,FRAME;
void InitTICK();
void GameTICK();