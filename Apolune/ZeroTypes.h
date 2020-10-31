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
#pragma once
#pragma warning (disable:4100)

#include "macros.h"
#include "moremath.h"
#include "string_functions.h"

// POD-type replacements

#define Zdis_manual Zpointer           // Means you gotta dispose yourself.  Used in places where the class wants to use forward declared stuff and can't use Zdis
#define Zp Zpointer
#define Zb Zbool
#define Zi Zint
#define Zf Zfloat
#define Zd Zdouble
#define Zui Zuint
#define Zus Zushort

// Divide by zero protection and report

#define DIV_BY_ZERO_PROTECTION 1

#if !defined(DIV_BY_ZERO_REPORTING) && ( defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_OUTPUT) )
//#define DIV_BY_ZERO_REPORTING 1
  //  Use /D compiler option to set
#endif

// Unsigned subtraction "wrap" protection and report

#define UNSIGNED_SUBTRACTION_WRAP_PROTECTION 1

#if !defined(UNSIGNED_SUBTRACTION_WRAP_REPORTING) && ( defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_OUTPUT) )
#define UNSIGNED_SUBTRACTION_WRAP_REPORTING 1
#endif

// Unsigned addition "wrap" protection and report

#define UNSIGNED_ADDITION_WRAP_PROTECTION 1

#if !defined(UNSIGNED_ADDITION_WRAP_REPORTING) && ( defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_OUTPUT) )
#define UNSIGNED_ADDITION_WRAP_REPORTING 1
#endif

// Unsigned multiplication "wrap" protection and report

#define UNSIGNED_MULTIPLY_WRAP_PROTECTION 1

#if !defined(UNSIGNED_MULTIPLY_WRAP_REPORTING) && ( defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_OUTPUT) )
#define UNSIGNED_MULTIPLY_WRAP_REPORTING 1
#endif

/// Zbool /////////////////////////////////////////////////////////////////////////////////////////// start

class Zbool {
public:
 bool value;
 Zbool() { value=false; }
 Zbool( bool b ) { this->value=b; }
 operator bool *() { return &value; }
 Zbool *operator()() { return this; }
 Zbool *self() { return this; }
 Zbool& operator= ( const bool b ) { value=b; return *this; }
 Zbool& operator|= ( const bool b ) { value|=b; return *this; }
 Zbool& operator&= ( const bool b ) { value&=b; return *this; }
 const bool operator-() { return !value; }
 operator bool() { return value; }
};

/// Zbool /////////////////////////////////////////////////////////////////////////////////////////// end

/// Zwas //////////////////////////////////////////////////////////////////////////////////////////// start

class Zwas {
public:
 bool value,was;
 Zwas() { value=was=false; }
 Zwas( bool b ) { this->value=b; was=false; }
 operator bool *() { return &value; }
 Zwas *operator()() { return this; }
 Zwas *self() { return this; }
 Zwas& operator= ( const bool b ) { was=value; value=b; return *this; }
 Zwas& operator|= ( const bool b ) { value|=b; return *this; }
 Zwas& operator&= ( const bool b ) { value&=b; return *this; }
 void Store() {
  was=value;
 }
 const bool operator-() { return !value; }
 operator bool() { return value; }
};

/// Zwas //////////////////////////////////////////////////////////////////////////////////////////// end

// Forward declarations
class Zbyte;
class Zushort;
class Zuint;
class Zint;
class Zdouble;
class Zstring;

/// Zfloat ////////////////////////////////////////////////////////////////////////////////////////// start

class Zfloat {
public:
 float value;
 Zfloat() { value=0.0f; }
 Zfloat( float value ) { this->value=value; }
 string toString() {static char b[120]; return remove_trailing_zeroes(FORMAT(b, 64, "%f", value).c_str()); }
 float abs() { return absf(value); }
 operator string() { return toString(); }
 operator float() { return value; }
 operator float *() { return &value; }
 Zfloat *operator()() { return this; }
 Zfloat *self() { return this; }
 float operator()( int i ) { return value=(float)i; }
 float operator()( float f ) { return value=f; }
 float operator()( double d ) { return value=(float)d; }
 float operator()( Zbyte &b );
 float operator()( Zuint &i );
 float operator()( Zint &i );
 float operator()( Zfloat &f );
 float operator()( Zdouble &d );
 Zfloat& operator= ( const unsigned char b ) { value=(float) b; return *this; }
 Zfloat& operator= ( const unsigned int i ) { value=(float) i; return *this; }
 Zfloat& operator= ( const int i ) { value=(float) i; return *this; }
 Zfloat& operator= ( const float f ) { value=f; return *this; }
 Zfloat& operator= ( const double d ) { value=(float) d; return *this; }
 Zfloat& operator= ( const Zbyte& b );
 Zfloat& operator= ( const Zuint& i );
 Zfloat& operator= ( const Zint& i );
 Zfloat& operator= ( const Zfloat& f ) { value=f.value; return *this; }
 Zfloat& operator= ( const Zdouble& d );
 Zfloat& operator= ( const Zstring& s );
 Zfloat& operator+= ( const Zfloat& f ) { value+=f.value; return *this; }
 Zfloat& operator-= ( const Zfloat& f ) { value-=f.value; return *this; }
 Zfloat& operator*= ( const Zfloat& f ) { value*=f.value; return *this; }
 Zfloat& operator/= ( const Zfloat& f ) { value/=f.value; return *this; }
 Zfloat& operator+= ( const Zint& i );
 Zfloat& operator-= ( const Zint& i );
 Zfloat& operator*= ( const Zint& i );
 Zfloat& operator/= ( const Zint& i );
 Zfloat& operator+= ( const unsigned char f ) { value+=(float)f; return *this; }
 Zfloat& operator-= ( const unsigned char f ) { value-=(float)f; return *this; }
 Zfloat& operator*= ( const unsigned char f ) { value*=(float)f; return *this; }
 Zfloat& operator/= ( const unsigned char f ) { value/=(float)f; return *this; }
 Zfloat& operator+= ( const unsigned short f ) { value+=(float)f; return *this; }
 Zfloat& operator-= ( const unsigned short f ) { value-=(float)f; return *this; }
 Zfloat& operator*= ( const unsigned short f ) { value*=(float)f; return *this; }
 Zfloat& operator/= ( const unsigned short f ) { value/=(float)f; return *this; }
 Zfloat& operator+= ( const int f ) { value+=(float)f; return *this; }
 Zfloat& operator-= ( const int f ) { value-=(float)f; return *this; }
 Zfloat& operator*= ( const int f ) { value*=(float)f; return *this; }
 Zfloat& operator/= ( const int f ) { value/=(float)f; return *this; }
 Zfloat& operator+= ( const unsigned int f ) { value+=(float)f; return *this; }
 Zfloat& operator-= ( const unsigned int f ) { value-=(float)f; return *this; }
 Zfloat& operator*= ( const unsigned int f ) { value*=(float)f; return *this; }
 Zfloat& operator/= ( const unsigned int f ) { value/=(float)f; return *this; }
 Zfloat& operator+= ( const float f ) { value+=f; return *this; }
 Zfloat& operator-= ( const float f ) { value-=f; return *this; }
 Zfloat& operator*= ( const float f ) { value*=f; return *this; }
 Zfloat& operator/= ( const float f ) { value/=f; return *this; }
 Zfloat& operator+= ( const double f ) { value+=(float)f; return *this; }
 Zfloat& operator-= ( const double f ) { value-=(float)f; return *this; }
 Zfloat& operator*= ( const double f ) { value*=(float)f; return *this; }
 Zfloat& operator/= ( const double f ) { value/=(float)f; return *this; }
 Zfloat& operator++ (int) { value+=1.0f; return *this; }
 Zfloat& operator-- (int) { value-=1.0f; return *this; }
 const float operator-() { return -value; }
 bool operator !() { return ( value == 0.0f ); }
// Not used intentionally to create warning:
// operator double() { return (double) value; }
// operator int() { return (int) value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator double() { return (double) value; }
// operator int() { return (int) value; }
};

// Mixed type interactions
float operator+ ( const Zfloat& a, const Zint& b );
float operator+ ( const Zint& a, const Zfloat& b );
float operator* ( const Zfloat& a, const Zint& b );
float operator* ( const Zint& a, const Zfloat& b );
// Float and itself
float operator+ ( const Zfloat& a, const Zfloat& b );
float operator- ( const Zfloat& a, const Zfloat& b );
float operator* ( const Zfloat& a, const Zfloat& b );
float operator/ ( const Zfloat& a, const Zfloat& b );
/* Creates C2593 for some reason...
float operator+ ( Zfloat a, Zfloat b );
float operator- ( Zfloat a, Zfloat b );
float operator* ( Zfloat a, Zfloat b );
float operator/ ( Zfloat a, Zfloat b );
*/
// Casted int operations
float operator+ ( const Zfloat& a, int b );
float operator- ( const Zfloat& a, int b );
float operator* ( const Zfloat& a, int b );
float operator/ ( const Zfloat& a, int b );
float operator+ ( int b, const Zfloat& a );
float operator- ( int b, const Zfloat& a );
float operator* ( int b, const Zfloat& a );
float operator/ ( int b, const Zfloat& a );
// Float operations
float operator+ ( const Zfloat& a, float b );
float operator- ( const Zfloat& a, float b );
float operator* ( const Zfloat& a, float b );
float operator/ ( const Zfloat& a, float b );
float operator+ ( float b, const Zfloat& a );
float operator- ( float b, const Zfloat& a );
float operator* ( float b, const Zfloat& a );
float operator/ ( float b, const Zfloat& a );
// Casted double operations (leans toward double precision)
float operator+ ( const Zfloat& a, double b );
float operator- ( const Zfloat& a, double b );
float operator* ( const Zfloat& a, double b );
float operator/ ( const Zfloat& a, double b );
float operator+ ( double b, const Zfloat& a );
float operator- ( double b, const Zfloat& a );
float operator* ( double b, const Zfloat& a );
float operator/ ( double b, const Zfloat& a );
// Boolean operations
bool operator>= ( const Zfloat& a, const Zfloat& b );
bool operator>= ( const Zfloat& a, const Zbyte& b );
bool operator>= ( const Zfloat& a, const Zushort& b );
bool operator>= ( const Zfloat& a, const Zuint& b );
bool operator>= ( const Zfloat& a, const Zint& b );
bool operator>= ( const Zfloat& a, const Zdouble& b );
bool operator>= ( const Zbyte& a, const Zfloat& b );
bool operator>= ( const Zushort& a, const Zfloat& b );
bool operator>= ( const Zuint& a, const Zfloat& b );
bool operator>= ( const Zint& a, const Zfloat& b );
bool operator>= ( const Zdouble& a, const Zfloat& b );
bool operator>= ( const Zfloat& a, int b );
bool operator>= ( const Zfloat& a, float b );
bool operator>= ( const Zfloat& a, double b );
bool operator>= ( int a, const Zfloat& b );
bool operator>= ( float a, const Zfloat& b );
bool operator>= ( double a, const Zfloat& b );
bool operator== ( const Zfloat& a, const Zfloat& b );
bool operator== ( const Zfloat& a, const Zbyte& b );
bool operator== ( const Zfloat& a, const Zushort& b );
bool operator== ( const Zfloat& a, const Zuint& b );
bool operator== ( const Zfloat& a, const Zint& b );
bool operator== ( const Zfloat& a, const Zdouble& b );
bool operator== ( const Zbyte& a, const Zfloat& b );
bool operator== ( const Zushort& a, const Zfloat& b );
bool operator== ( const Zuint& a, const Zfloat& b );
bool operator== ( const Zint& a, const Zfloat& b );
bool operator== ( const Zdouble& a, const Zfloat& b );
bool operator== ( const Zfloat& a, int b );
bool operator== ( const Zfloat& a, float b );
bool operator== ( const Zfloat& a, double b );
bool operator== ( int a, const Zfloat& b );
bool operator== ( float a, const Zfloat& b );
bool operator== ( double a, const Zfloat& b );
bool operator!= ( const Zfloat& a, const Zfloat& b );
bool operator!= ( const Zfloat& a, const Zbyte& b );
bool operator!= ( const Zfloat& a, const Zushort& b );
bool operator!= ( const Zfloat& a, const Zuint& b );
bool operator!= ( const Zfloat& a, const Zint& b );
bool operator!= ( const Zfloat& a, const Zdouble& b );
bool operator!= ( const Zbyte& a, const Zfloat& b );
bool operator!= ( const Zushort& a, const Zfloat& b );
bool operator!= ( const Zuint& a, const unsigned int b );
bool operator!= ( const Zuint& a, const int b );
bool operator!= ( const Zuint& a, const Zfloat& b );
bool operator!= ( const Zint& a, const Zfloat& b );
bool operator!= ( const Zdouble& a, const Zfloat& b );
bool operator!= ( const Zfloat& a, int b );
bool operator!= ( const Zfloat& a, float b );
bool operator!= ( const Zfloat& a, double b );
bool operator!= ( int a, const Zfloat& b );
bool operator!= ( float a, const Zfloat& b );
bool operator!= ( double a, const Zfloat& b );
bool operator<= ( const Zfloat& a, const Zfloat& b );
bool operator<= ( const Zfloat& a, const Zbyte& b );
bool operator<= ( const Zfloat& a, const Zushort& b );
bool operator<= ( const Zfloat& a, const Zuint& b );
bool operator<= ( const Zfloat& a, const Zint& b );
bool operator<= ( const Zfloat& a, const Zdouble& b );
bool operator<= ( const Zbyte& a, const Zfloat& b );
bool operator<= ( const Zushort& a, const Zfloat& b );
bool operator<= ( const Zuint& a, const Zfloat& b );
bool operator<= ( const Zint& a, const Zfloat& b );
bool operator<= ( const Zdouble& a, const Zfloat& b );
bool operator<= ( const Zfloat& a, int b );
bool operator<= ( const Zfloat& a, float b );
bool operator<= ( const Zfloat& a, double b );
bool operator<= ( int a, const Zfloat& b );
bool operator<= ( float a, const Zfloat& b );
bool operator<= ( double a, const Zfloat& b );
bool operator> ( const Zfloat& a, const Zfloat& b );
bool operator> ( const Zfloat& a, const Zbyte& b );
bool operator> ( const Zfloat& a, const Zushort& b );
bool operator> ( const Zfloat& a, const Zuint& b );
bool operator> ( const Zfloat& a, const Zint& b );
bool operator> ( const Zfloat& a, const Zdouble& b );
bool operator> ( const Zbyte& a, const Zfloat& b );
bool operator> ( const Zushort& a, const Zfloat& b );
bool operator> ( const Zuint& a, const Zfloat& b );
bool operator> ( const Zint& a, const Zfloat& b );
bool operator> ( const Zdouble& a, const Zfloat& b );
bool operator> ( const Zfloat& a, int b );
bool operator> ( const Zfloat& a, float b );
bool operator> ( const Zfloat& a, double b );
bool operator> ( int a, const Zfloat& b );
bool operator> ( float a, const Zfloat& b );
bool operator> ( double a, const Zfloat& b );
bool operator< ( const Zfloat& a, const Zfloat& b );
bool operator< ( const Zfloat& a, const Zbyte& b );
bool operator< ( const Zfloat& a, const Zushort& b );
bool operator< ( const Zfloat& a, const Zuint& b );
bool operator< ( const Zfloat& a, const Zint& b );
bool operator< ( const Zfloat& a, const Zdouble& b );
bool operator< ( const Zbyte& a, const Zfloat& b );
bool operator< ( const Zushort& a, const Zfloat& b );
bool operator< ( const Zuint& a, const Zfloat& b );
bool operator< ( const Zint& a, const Zfloat& b );
bool operator< ( const Zdouble& a, const Zfloat& b );
bool operator< ( const Zfloat& a, int b );
bool operator< ( const Zfloat& a, float b );
bool operator< ( const Zfloat& a, double b );
bool operator< ( int a, const Zfloat& b );
bool operator< ( float a, const Zfloat& b );
bool operator< ( double a, const Zfloat& b );

/// Zfloat ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zbyte ////////////////////////////////////////////////////////////////////////////////////////// start

class Zbyte {
public:
 unsigned char value;
 Zbyte() { value=0; }
 Zbyte( unsigned char value ) { this->value=value; }
 operator unsigned char *() { return &value; }
 Zbyte *operator()() { return this; }
 Zbyte *self() { return this; }
 bool Get(const unsigned char bit)          { return ((value) & (1<<(bit))) ? true : false; }
 void Set(const unsigned char bit, const bool on) { value |= (1<<(on?1:0));        }
 unsigned char operator()( unsigned char b ) { return value=b; }
 unsigned char operator()( unsigned int i ) { return value=(unsigned char)i; }
 unsigned char operator()( int i ) { return value=(unsigned char) i; }
 unsigned char operator()( float f ) { return value=(unsigned char) f; }
 unsigned char operator()( double d ) { return value=(unsigned char) d; }
 unsigned char operator()( Zuint &i );
 unsigned char operator()( Zint &i );
 unsigned char operator()( Zfloat &f );
 unsigned char operator()( Zdouble &d );
 Zbyte& operator= ( const unsigned char b ) { value=b; return *this; }
 Zbyte& operator= ( const int i ) { value=(unsigned char)i; return *this; }
 Zbyte& operator= ( const float f ) { value=(unsigned char)f; return *this; }
 Zbyte& operator= ( const double d ) { value=(unsigned char) d; return *this; }
 Zbyte& operator= ( const Zbyte& i ) { value=i.value; return *this; }
 Zbyte& operator= ( const Zuint& i );
 Zbyte& operator= ( const Zint& i );
 Zbyte& operator= ( const Zfloat& f );
 Zbyte& operator= ( const Zdouble& d );
 Zbyte& operator= ( const Zstring& s );
 Zbyte& operator+= ( const Zbyte& i ) { value+=i.value; return *this; }
 Zbyte& operator-= ( const Zbyte& i ) { value-=i.value; return *this; }
 Zbyte& operator*= ( const Zbyte& i ) { value*=i.value; return *this; }
 Zbyte& operator/= ( const Zbyte& i ) { value/=i.value; return *this; }
 Zbyte& operator+= ( const unsigned char i ) { value+=i; return *this; }
 Zbyte& operator-= ( const unsigned char i ) { value-=i; return *this; }
 Zbyte& operator*= ( const unsigned char i ) { value*=i; return *this; }
 Zbyte& operator/= ( const unsigned char i ) { value/=i; return *this; }
 Zbyte& operator+= ( const unsigned short i ) { value+=(unsigned char)i; return *this; }
 Zbyte& operator-= ( const unsigned short i ) { value-=(unsigned char)i; return *this; }
 Zbyte& operator*= ( const unsigned short i ) { value*=(unsigned char)i; return *this; }
 Zbyte& operator/= ( const unsigned short i ) { value/=(unsigned char)i; return *this; }
 Zbyte& operator+= ( const int i ) { value+=(unsigned char)i; return *this; }
 Zbyte& operator-= ( const int i ) { value-=(unsigned char)i; return *this; }
 Zbyte& operator*= ( const int i ) { value*=(unsigned char)i; return *this; }
 Zbyte& operator/= ( const int i ) { value/=(unsigned char)i; return *this; }
 Zbyte& operator+= ( const unsigned int i ) { value+=(unsigned char)i; return *this; }
 Zbyte& operator-= ( const unsigned int i ) { value-=(unsigned char)i; return *this; }
 Zbyte& operator*= ( const unsigned int i ) { value*=(unsigned char)i; return *this; }
 Zbyte& operator/= ( const unsigned int i ) { value/=(unsigned char)i; return *this; }
 Zbyte& operator+= ( const float i ) { value+=(unsigned char)i; return *this; }
 Zbyte& operator-= ( const float i ) { value-=(unsigned char)i; return *this; }
 Zbyte& operator*= ( const float i ) { value*=(unsigned char)i; return *this; }
 Zbyte& operator/= ( const float i ) { value/=(unsigned char)i; return *this; }
 Zbyte& operator+= ( const double i ) { value+=(unsigned char)i; return *this; }
 Zbyte& operator-= ( const double i ) { value-=(unsigned char)i; return *this; }
 Zbyte& operator*= ( const double i ) { value*=(unsigned char)i; return *this; }
 Zbyte& operator/= ( const double i ) { value/=(unsigned char)i; return *this; }
 Zbyte& operator++ (int) { value+=1; return *this; }
 Zbyte& operator-- (int) { value-=1; return *this; }
 operator unsigned char() { return value; }
 bool operator !() { return ( value == 0 ); }
// Not used intentionally to create warning:
// operator float() { return (float)value; }
// operator double() { return (double)value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator float() { return (float) value; }
// operator float() { return (double) value; }
};


unsigned char operator+ ( const Zbyte& a, const Zbyte& b );
unsigned char operator- ( const Zbyte& a, const Zbyte& b );
unsigned char operator* ( const Zbyte& a, const Zbyte& b );
unsigned char operator/ ( const Zbyte& a, const Zbyte& b );
unsigned char operator+ ( Zbyte a, Zbyte b );
unsigned char operator- ( Zbyte a, Zbyte b );
unsigned char operator* ( Zbyte a, Zbyte b );
unsigned char operator/ ( Zbyte a, Zbyte b );
// Casted unsigned int operations
unsigned char operator+ ( const Zbyte& a, unsigned char b );
unsigned char operator- ( const Zbyte& a, unsigned char b );
unsigned char operator* ( const Zbyte& a, unsigned char b );
unsigned char operator/ ( const Zbyte& a, unsigned char b );
unsigned char operator+ ( unsigned char b, const Zbyte& a );
unsigned char operator- ( unsigned char b, const Zbyte& a );
unsigned char operator* ( unsigned char b, const Zbyte& a );
unsigned char operator/ ( unsigned char b, const Zbyte& a );
// Casted int operations
unsigned char operator+ ( const Zbyte& a, int b );
unsigned char operator- ( const Zbyte& a, int b );
unsigned char operator* ( const Zbyte& a, int b );
unsigned char operator/ ( const Zbyte& a, int b );
unsigned char operator+ ( int b, const Zbyte& a );
unsigned char operator- ( int b, const Zbyte& a );
unsigned char operator* ( int b, const Zbyte& a );
unsigned char operator/ ( int b, const Zbyte& a );
// Float operations (leans toward float precision)
unsigned char operator+ ( const Zbyte& a, float b );
unsigned char operator- ( const Zbyte& a, float b );
unsigned char operator* ( const Zbyte& a, float b );
unsigned char operator/ ( const Zbyte& a, float b );
unsigned char operator+ ( float b, const Zbyte& a );
unsigned char operator- ( float b, const Zbyte& a );
unsigned char operator* ( float b, const Zbyte& a );
unsigned char operator/ ( float b, const Zbyte& a );
// Casted double operations (leans toward double precision)
unsigned char operator+ ( const Zbyte& a, double b );
unsigned char operator- ( const Zbyte& a, double b );
unsigned char operator* ( const Zbyte& a, double b );
unsigned char operator/ ( const Zbyte& a, double b );
unsigned char operator+ ( double b, const Zbyte& a );
unsigned char operator- ( double b, const Zbyte& a );
unsigned char operator* ( double b, const Zbyte& a );
unsigned char operator/ ( double b, const Zbyte& a );
// Boolean operations
bool operator>= ( const Zbyte& a, const Zbyte& b );
bool operator>= ( const Zbyte& a, int b );
bool operator>= ( const Zbyte& a, float b );
bool operator>= ( const Zbyte& a, double b );
bool operator>= ( int a, const Zbyte& b );
bool operator>= ( float a, const Zbyte& b );
bool operator>= ( double a, const Zbyte& b );
bool operator== ( const Zbyte& a, const Zbyte& b );
bool operator== ( const Zbyte& a, int b );
bool operator== ( const Zbyte& a, float b );
bool operator== ( const Zbyte& a, double b );
bool operator== ( int a, const Zbyte& b );
bool operator== ( float a, const Zbyte& b );
bool operator== ( double a, const Zbyte& b );
bool operator!= ( const Zbyte& a, const Zbyte& b );
bool operator!= ( const Zbyte& a, int b );
bool operator!= ( const Zbyte& a, float b );
bool operator!= ( const Zbyte& a, double b );
bool operator!= ( int a, const Zbyte& b );
bool operator!= ( float a, const Zbyte& b );
bool operator!= ( double a, const Zbyte& b );
bool operator<= ( const Zbyte& a, const Zbyte& b );
bool operator<= ( const Zbyte& a, int b );
bool operator<= ( const Zbyte& a, float b );
bool operator<= ( const Zbyte& a, double b );
bool operator<= ( int a, const Zbyte& b );
bool operator<= ( float a, const Zbyte& b );
bool operator<= ( double a, const Zbyte& b );
bool operator> ( const Zbyte& a, const Zbyte& b );
bool operator> ( const Zbyte& a, int b );
bool operator> ( const Zbyte& a, float b );
bool operator> ( const Zbyte& a, double b );
bool operator> ( int a, const Zbyte& b );
bool operator> ( float a, const Zbyte& b );
bool operator> ( double a, const Zbyte& b );
bool operator< ( const Zbyte& a, const Zbyte& b );
bool operator< ( const Zbyte& a, int b );
bool operator< ( const Zbyte& a, float b );
bool operator< ( const Zbyte& a, double b );
bool operator< ( int a, const Zbyte& b );
bool operator< ( float a, const Zbyte& b );
bool operator< ( double a, const Zbyte& b );


/// Zbyte ///////////////////////////////////////////////////////////////////////////////////////// end

/// Zushort ////////////////////////////////////////////////////////////////////////////////////////// start

class Zushort {
public:
 unsigned short value;
 Zushort() { value=0; }
 Zushort( unsigned short value ) { this->value=value; }
 operator unsigned short *() { return &value; }
 Zushort *operator()() { return this; }
 Zushort *self() { return this; }
 unsigned short operator()( unsigned short z ) { return value=(unsigned short)z; }
 unsigned short operator()( unsigned char b ) { return value=(unsigned short)b; }
 unsigned short operator()( unsigned int i ) { return value=(unsigned short)i; }
 unsigned short operator()( int i ) { return value=(unsigned short) i; }
 unsigned short operator()( float f ) { return value=(unsigned short) f; }
 unsigned short operator()( double d ) { return value=(unsigned short) d; }
 unsigned short operator()( Zuint &i );
 unsigned short operator()( Zint &i );
 unsigned short operator()( Zfloat &f );
 unsigned short operator()( Zdouble &d );
 Zushort& operator= ( const unsigned short z ) { value=z; return *this; }
 Zushort& operator= ( const unsigned char b ) { value=(unsigned short)b; return *this; }
 Zushort& operator= ( const int i ) { value=(unsigned short)i; return *this; }
 Zushort& operator= ( const float f ) { value=(unsigned short)f; return *this; }
 Zushort& operator= ( const double d ) { value=(unsigned short) d; return *this; }
 Zushort& operator= ( const Zushort& i ) { value=i.value; return *this; }
 Zushort& operator= ( const Zbyte& i ) { value=(unsigned short)i.value; return *this; }
 Zushort& operator= ( const Zuint& i );
 Zushort& operator= ( const Zint& i );
 Zushort& operator= ( const Zfloat& f );
 Zushort& operator= ( const Zdouble& d );
 Zushort& operator= ( const Zstring& s );
 Zushort& operator+= ( const unsigned char i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const unsigned char i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const unsigned char i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const unsigned char i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator+= ( const unsigned short i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const unsigned short i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const unsigned short i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const unsigned short i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator+= ( const int i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const int i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const int i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const int i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator+= ( const unsigned int i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const unsigned int i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const unsigned int i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const unsigned int i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator+= ( const float i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const float i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const float i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const float i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator+= ( const double i ) { value+=(unsigned short)i; return *this; }
 Zushort& operator-= ( const double i ) { value-=(unsigned short)i; return *this; }
 Zushort& operator*= ( const double i ) { value*=(unsigned short)i; return *this; }
 Zushort& operator/= ( const double i ) { value/=(unsigned short)i; return *this; }
 Zushort& operator++ (int) { value+=1; return *this; }
 Zushort& operator-- (int) { value-=1; return *this; }
 operator unsigned short() { return value; }
 operator int() { return -(int)value; }
 bool operator !() { return ( value == 0 ); }
// Not used intentionally to create warning:
// operator float() { return (float)value; }
// operator double() { return (double)value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator float() { return (float) value; }
// operator float() { return (double) value; }
};


unsigned short operator+ ( const Zushort& a, const Zushort& b );
unsigned short operator- ( const Zushort& a, const Zushort& b );
unsigned short operator* ( const Zushort& a, const Zushort& b );
unsigned short operator/ ( const Zushort& a, const Zushort& b );
unsigned short operator+ ( Zushort a, Zushort b );
unsigned short operator- ( Zushort a, Zushort b );
unsigned short operator* ( Zushort a, Zushort b );
unsigned short operator/ ( Zushort a, Zushort b );
// Casted unsigned int operations
unsigned short operator+ ( const Zushort& a, unsigned char b );
unsigned short operator- ( const Zushort& a, unsigned char b );
unsigned short operator* ( const Zushort& a, unsigned char b );
unsigned short operator/ ( const Zushort& a, unsigned char b );
unsigned short operator+ ( unsigned char b, const Zushort& a );
unsigned short operator- ( unsigned char b, const Zushort& a );
unsigned short operator* ( unsigned char b, const Zushort& a );
unsigned short operator/ ( unsigned char b, const Zushort& a );
// Casted int operations
unsigned short operator+ ( const Zushort& a, int b );
unsigned short operator- ( const Zushort& a, int b );
unsigned short operator* ( const Zushort& a, int b );
unsigned short operator/ ( const Zushort& a, int b );
unsigned short operator+ ( int b, const Zushort& a );
unsigned short operator- ( int b, const Zushort& a );
unsigned short operator* ( int b, const Zushort& a );
unsigned short operator/ ( int b, const Zushort& a );
// Float operations (leans toward float precision)
unsigned short operator+ ( const Zushort& a, float b );
unsigned short operator- ( const Zushort& a, float b );
unsigned short operator* ( const Zushort& a, float b );
unsigned short operator/ ( const Zushort& a, float b );
unsigned short operator+ ( float b, const Zushort& a );
unsigned short operator- ( float b, const Zushort& a );
unsigned short operator* ( float b, const Zushort& a );
unsigned short operator/ ( float b, const Zushort& a );
// Casted double operations (leans toward double precision)
unsigned short operator+ ( const Zushort& a, double b );
unsigned short operator- ( const Zushort& a, double b );
unsigned short operator* ( const Zushort& a, double b );
unsigned short operator/ ( const Zushort& a, double b );
unsigned short operator+ ( double b, const Zushort& a );
unsigned short operator- ( double b, const Zushort& a );
unsigned short operator* ( double b, const Zushort& a );
unsigned short operator/ ( double b, const Zushort& a );

/// Zbyte ///////////////////////////////////////////////////////////////////////////////////////// end

/// Zint ////////////////////////////////////////////////////////////////////////////////////////// start

class Zint {
public:
 int value;
 Zint() { value=0; }
 Zint( int value ) { this->value=value; }
 int abs() { return absi(value); }
 string toString() {static char b[64]; return FORMAT(b, 64, "%d", value); }
 operator string() { return toString(); }
 operator int() { return value; }
 operator int *() { return &value; }
 Zint *operator()() { return this; }
 Zint *self() { return this; }
 int operator()( int i ) { return value=i; }
 int operator()( float f ) { return value=(int)f; }
 int operator()( double d ) { return value=(int)d; }
 int operator()( Zbyte &i );
 int operator()( Zuint &i );
 int operator()( Zint &i ) { return value=i.value; }
 int operator()( Zfloat &f );
 int operator()( Zdouble &d );
 Zint& operator= ( const int i ) { value=i; return *this; }
 Zint& operator= ( const float f ) { value=(int)f; return *this; }
 Zint& operator= ( const double d ) { value=(int) d; return *this; }
 Zint& operator= ( const Zint& i ) { value=i.value; return *this; }
 Zint& operator= ( const Zbyte& b );
 Zint& operator= ( const Zuint& i );
 Zint& operator= ( const Zfloat& f );
 Zint& operator= ( const Zdouble& d );
 Zint& operator= ( const Zstring& s );
 Zint& operator+= ( const Zint& i ) { value+=i.value; return *this; }
 Zint& operator-= ( const Zint& i ) { value-=i.value; return *this; }
 Zint& operator*= ( const Zint& i ) { value*=i.value; return *this; }
 Zint& operator/= ( const Zint& i ) { value/=i.value; return *this; }
 Zint& operator+= ( const Zfloat& f );
 Zint& operator-= ( const Zfloat& f );
 Zint& operator*= ( const Zfloat& f );
 Zint& operator/= ( const Zfloat& f );
 Zint& operator+= ( const unsigned char i ) { value+=(int)i; return *this; }
 Zint& operator-= ( const unsigned char i ) { value-=(int)i; return *this; }
 Zint& operator*= ( const unsigned char i ) { value*=(int)i; return *this; }
 Zint& operator/= ( const unsigned char i ) { value/=(int)i; return *this; }
 Zint& operator+= ( const unsigned short i ) { value+=(int)i; return *this; }
 Zint& operator-= ( const unsigned short i ) { value-=(int)i; return *this; }
 Zint& operator*= ( const unsigned short i ) { value*=(int)i; return *this; }
 Zint& operator/= ( const unsigned short i ) { value/=(int)i; return *this; }
 Zint& operator+= ( const int i ) { value+=i; return *this; }
 Zint& operator-= ( const int i ) { value-=i; return *this; }
 Zint& operator*= ( const int i ) { value*=i; return *this; }
 Zint& operator/= ( const int i ) { value/=i; return *this; }
 Zint& operator+= ( const unsigned int i ) { value+=(int)i; return *this; }
 Zint& operator-= ( const unsigned int i ) { value-=(int)i; return *this; }
 Zint& operator*= ( const unsigned int i ) { value*=(int)i; return *this; }
 Zint& operator/= ( const unsigned int i ) { value/=(int)i; return *this; }
 Zint& operator+= ( const float i ) { value+=(int)i; return *this; }
 Zint& operator-= ( const float i ) { value-=(int)i; return *this; }
 Zint& operator*= ( const float i ) { value*=(int)i; return *this; }
 Zint& operator/= ( const float i ) { value/=(int)i; return *this; }
 Zint& operator+= ( const double i ) { value+=(int)i; return *this; }
 Zint& operator-= ( const double i ) { value-=(int)i; return *this; }
 Zint& operator*= ( const double i ) { value*=(int)i; return *this; }
 Zint& operator/= ( const double i ) { value/=(int)i; return *this; }
 Zint& operator++ (int) { value+=1; return *this; }
 Zint& operator-- (int) { value-=1; return *this; }
 const int operator-() { return -value; }
// operator size_t() { return (size_t)value; }
 operator unsigned int() { return (unsigned int) value; }
 operator float() { return (float) value; }
 operator double() { return (double) value; }
 bool operator !() { return ( value == 0 ); }
// Not used intentionally to create warning:
// operator float() { return (float)value; }
// operator double() { return (double)value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator float() { return (float) value; }
// operator float() { return (double) value; }
};


int operator+ ( const Zint& a, const Zint& b );
int operator- ( const Zint& a, const Zint& b );
int operator* ( const Zint& a, const Zint& b );
int operator/ ( const Zint& a, const Zint& b );
/* Creates C2593 for some reason...
int operator+ ( Zint a, Zint b );
int operator- ( Zint a, Zint b );
int operator* ( Zint a, Zint b );
int operator/ ( Zint a, Zint b );
*/
// Casted int operations
int operator+ ( const Zint& a, int b );
int operator- ( const Zint& a, int b );
int operator* ( const Zint& a, int b );
int operator/ ( const Zint& a, int b );
int operator% ( const Zint& a, int b );
int operator+ ( int b, const Zint& a );
int operator- ( int b, const Zint& a );
int operator* ( int b, const Zint& a );
int operator/ ( int b, const Zint& a );
int operator% ( int b, const Zint& a );
// Float operations (leans toward float precision)
int operator+ ( const Zint& a, float b );
int operator- ( const Zint& a, float b );
int operator* ( const Zint& a, float b );
int operator/ ( const Zint& a, float b );
int operator+ ( float b, const Zint& a );
int operator- ( float b, const Zint& a );
int operator* ( float b, const Zint& a );
int operator/ ( float b, const Zint& a );
// Casted double operations (leans toward double precision)
int operator+ ( const Zint& a, double b );
int operator- ( const Zint& a, double b );
int operator* ( const Zint& a, double b );
int operator/ ( const Zint& a, double b );
int operator+ ( double b, const Zint& a );
int operator- ( double b, const Zint& a );
int operator* ( double b, const Zint& a );
int operator/ ( double b, const Zint& a );
// Modulo
int operator% ( const Zint &a, const Zint &b );
// Boolean operations
bool operator>= ( const Zint& a, const Zint& b );
bool operator>= ( const Zint& a, unsigned int b );
bool operator>= ( const Zint& a, int b );
bool operator>= ( const Zint& a, float b );
bool operator>= ( const Zint& a, double b );
bool operator>= ( unsigned int a, const Zint& b );
bool operator>= ( int a, const Zint& b );
bool operator>= ( float a, const Zint& b );
bool operator>= ( double a, const Zint& b );
bool operator== ( const Zint& a, const Zbyte& b );
bool operator== ( const Zint& a, const Zushort& b );
bool operator== ( const Zint& a, const Zuint& b );
bool operator== ( const Zint& a, const Zdouble& b );
bool operator== ( const Zbyte& a, const Zint& b );
bool operator== ( const Zushort& a, const Zint& b );
bool operator== ( const Zuint& a, const Zint& b );
bool operator== ( const Zdouble& a, const Zint& b );
bool operator== ( const Zint& a, const Zint& b );
bool operator== ( const Zint& a, unsigned int b );
bool operator== ( const Zint& a, int b );
bool operator== ( const Zint& a, float b );
bool operator== ( const Zint& a, double b );
bool operator== ( unsigned int a, const Zint& b );
bool operator== ( int a, const Zint& b );
bool operator== ( float a, const Zint& b );
bool operator== ( double a, const Zint& b );
bool operator!= ( const Zint& a, const Zint& b );
bool operator!= ( const Zint& a, unsigned int b );
bool operator!= ( const Zint& a, int b );
bool operator!= ( const Zint& a, float b );
bool operator!= ( const Zint& a, double b );
bool operator!= ( unsigned int a, const Zint& b );
bool operator!= ( int a, const Zint& b );
bool operator!= ( float a, const Zint& b );
bool operator!= ( double a, const Zint& b );
bool operator<= ( const Zint& a, const Zint& b );
bool operator<= ( const Zint& a, unsigned int b );
bool operator<= ( const Zint& a, int b );
bool operator<= ( const Zint& a, float b );
bool operator<= ( const Zint& a, double b );
bool operator<= ( unsigned int a, const Zint& b );
bool operator<= ( int a, const Zint& b );
bool operator<= ( float a, const Zint& b );
bool operator<= ( double a, const Zint& b );
bool operator> ( const Zint& a, const Zint& b );
bool operator> ( const Zint& a, unsigned int b );
bool operator> ( const Zint& a, int b );
bool operator> ( const Zint& a, float b );
bool operator> ( const Zint& a, double b );
bool operator> ( unsigned int a, const Zint& b );
bool operator> ( int a, const Zint& b );
bool operator> ( float a, const Zint& b );
bool operator> ( double a, const Zint& b );
bool operator< ( const Zint& a, const Zint& b );
bool operator< ( const Zint& a, unsigned int b );
bool operator< ( const Zint& a, int b );
bool operator< ( const Zint& a, float b );
bool operator< ( const Zint& a, double b );
bool operator< ( unsigned int a, const Zint& b );
bool operator< ( int a, const Zint& b );
bool operator< ( float a, const Zint& b );
bool operator< ( double a, const Zint& b );

/// Zint ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zuint ////////////////////////////////////////////////////////////////////////////////////////// start

enum Zuint_bits {
 Zuint_0 =0x1,
 Zuint_1 =0x2,
 Zuint_2 =0x4,
 Zuint_3 =0x8,
 Zuint_4 =0x10,
 Zuint_5 =0x20,
 Zuint_6 =0x40,
 Zuint_7 =0x80,
 Zuint_8 =0x100,
 Zuint_9 =0x200,
 Zuint_10=0x400,
 Zuint_11=0x800,
 Zuint_12=0x1000,
 Zuint_13=0x2000,
 Zuint_14=0x4000,
 Zuint_15=0x8000,
 Zuint_16=0x10000,
 Zuint_17=0x20000,
 Zuint_18=0x40000,
 Zuint_19=0x80000,
 Zuint_20=0x100000,
 Zuint_21=0x200000,
 Zuint_22=0x400000,
 Zuint_23=0x800000,
 Zuint_24=0x1000000,
 Zuint_25=0x2000000,
 Zuint_26=0x4000000,
 Zuint_27=0x8000000,
 Zuint_28=0x10000000,
 Zuint_29=0x20000000,
 Zuint_30=0x40000000,
 Zuint_31=0x80000000
};

class Zuint {
public:
 unsigned int value;
 Zuint() { value=0; }
 Zuint( unsigned int value ) { this->value=value; }
 string toString() {static char b[64]; return I2S(value); }
 operator string() { return toString(); }
 operator unsigned int() { return value; }
 operator unsigned int *() { return &value; }
 Zuint *operator()() { return this; }
 Zuint *self() { return this; }
 unsigned int operator()( unsigned int i ) { return value=i; }
 unsigned int operator()( int i ) { return value=(unsigned int) i; }
 unsigned int operator()( float f ) { return value=(unsigned int)f; }
 unsigned int operator()( double d ) { return value=(unsigned int)d; }
 unsigned int operator()( Zbyte &i );
 unsigned int operator()( Zuint &i ) { return i.value; }
 unsigned int operator()( Zint &i );
 unsigned int operator()( Zfloat &f );
 unsigned int operator()( Zdouble &d );
 bool isOn( unsigned int bitwise_flag ) {
  return ( value & bitwise_flag ) > 0;
 }
 bool isOff( unsigned int bitwise_flag ) { return !isOn(bitwise_flag); }
 void Toggle( unsigned int bitwise_flag, bool on ) {
  if ( on ) value = value | bitwise_flag;
  else value = value & ~bitwise_flag;
 }
 Zuint& operator= ( const unsigned int i ) { value=i; return *this; }
 Zuint& operator= ( const int i ) { value=i; return *this; }
 Zuint& operator= ( const float f ) { value=(int)f; return *this; }
 Zuint& operator= ( const double d ) { value=(int) d; return *this; }
 Zuint& operator= ( const Zuint& i ) { value=i.value; return *this; }
 Zuint& operator= ( const Zbyte& b );
 Zuint& operator= ( const Zint& i );
 Zuint& operator= ( const Zfloat& f );
 Zuint& operator= ( const Zdouble& d );
 Zuint& operator= ( const Zstring& s );
 Zuint& operator+= ( const Zuint& i ) { value+=i.value; return *this; }
 Zuint& operator-= ( const Zuint& i ) { value-=i.value; return *this; }
 Zuint& operator*= ( const Zuint& i ) { value*=i.value; return *this; }
 Zuint& operator/= ( const Zuint& i ) { value/=i.value; return *this; }
 Zuint& operator+= ( const Zint& i ) { value+=(unsigned int) i.value; return *this; }
 Zuint& operator-= ( const Zint& i ) { value-=(unsigned int) i.value; return *this; }
 Zuint& operator*= ( const Zint& i ) { value*=(unsigned int) i.value; return *this; }
 Zuint& operator/= ( const Zint& i ) { value/=(unsigned int) i.value; return *this; }
 Zuint& operator+= ( const unsigned char i ) { value+=(unsigned int) i; return *this; }
 Zuint& operator-= ( const unsigned char i ) { value-=(unsigned int) i; return *this; }
 Zuint& operator*= ( const unsigned char i ) { value*=(unsigned int) i; return *this; }
 Zuint& operator/= ( const unsigned char i ) { value/=(unsigned int) i; return *this; }
 Zuint& operator+= ( const unsigned short i ) { value+=(unsigned int) i; return *this; }
 Zuint& operator-= ( const unsigned short i ) { value-=(unsigned int) i; return *this; }
 Zuint& operator*= ( const unsigned short i ) { value*=(unsigned int) i; return *this; }
 Zuint& operator/= ( const unsigned short i ) { value/=(unsigned int) i; return *this; }
 Zuint& operator+= ( const int i ) { value+=(unsigned int) i; return *this; }
 Zuint& operator-= ( const int i ) { value-=(unsigned int) i; return *this; }
 Zuint& operator*= ( const int i ) { value*=(unsigned int) i; return *this; }
 Zuint& operator/= ( const int i ) { value/=(unsigned int) i; return *this; }
 Zuint& operator+= ( const unsigned int i ) { value+=i; return *this; }
 Zuint& operator-= ( const unsigned int i ) { value-=i; return *this; }
 Zuint& operator*= ( const unsigned int i ) { value*=i; return *this; }
 Zuint& operator/= ( const unsigned int i ) { value/=i; return *this; }
 Zuint& operator+= ( const float i ) { value+=(unsigned int) i; return *this; }
 Zuint& operator-= ( const float i ) { value-=(unsigned int) i; return *this; }
 Zuint& operator*= ( const float i ) { value*=(unsigned int) i; return *this; }
 Zuint& operator/= ( const float i ) { value/=(unsigned int) i; return *this; }
 Zuint& operator+= ( const double i ) { value+=(unsigned int) i; return *this; }
 Zuint& operator-= ( const double i ) { value-=(unsigned int) i; return *this; }
 Zuint& operator*= ( const double i ) { value*=(unsigned int) i; return *this; }
 Zuint& operator/= ( const double i ) { value/=(unsigned int) i; return *this; }
 Zuint& operator++ (int) { value+=1; return *this; }
 Zuint& operator-- (int) { value-=1; return *this; }
 operator int() { return (int) value; }
 bool operator !() { return ( value == 0 ); }
// Not used intentionally to create warning:
// operator float() { return (float)value; }
// operator double() { return (double)value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator float() { return (float) value; }
// operator float() { return (double) value; }
};

bool operator< ( unsigned int a, const Zuint& b );

unsigned int operator+ ( const Zuint& a, const Zuint& b );
unsigned int operator- ( const Zuint& a, const Zuint& b );
unsigned int operator* ( const Zuint& a, const Zuint& b );
unsigned int operator/ ( const Zuint& a, const Zuint& b );
// Casted unsigned int operations
unsigned int operator+ ( const Zuint& a, int b );
unsigned int operator- ( const Zuint& a, int b );
unsigned int operator* ( const Zuint& a, int b );
unsigned int operator/ ( const Zuint& a, int b );
unsigned int operator+ ( int b, const Zuint& a );
unsigned int operator- ( int b, const Zuint& a );
unsigned int operator* ( int b, const Zuint& a );
unsigned int operator/ ( int b, const Zuint& a );
// Casted int operations
unsigned int operator+ ( const Zuint& a, int b );
unsigned int operator- ( const Zuint& a, int b );
unsigned int operator* ( const Zuint& a, int b );
unsigned int operator/ ( const Zuint& a, int b );
unsigned int operator+ ( int b, const Zuint& a );
unsigned int operator- ( int b, const Zuint& a );
unsigned int operator* ( int b, const Zuint& a );
unsigned int operator/ ( int b, const Zuint& a );
// Float operations (leans toward float precision)
unsigned int operator+ ( const Zuint& a, float b );
unsigned int operator- ( const Zuint& a, float b );
unsigned int operator* ( const Zuint& a, float b );
unsigned int operator/ ( const Zuint& a, float b );
unsigned int operator+ ( float b, const Zuint& a );
unsigned int operator- ( float b, const Zuint& a );
unsigned int operator* ( float b, const Zuint& a );
unsigned int operator/ ( float b, const Zuint& a );
// Casted double operations (leans toward double precision)
unsigned int operator+ ( const Zuint& a, double b );
unsigned int operator- ( const Zuint& a, double b );
unsigned int operator* ( const Zuint& a, double b );
unsigned int operator/ ( const Zuint& a, double b );
unsigned int operator+ ( double b, const Zuint& a );
unsigned int operator- ( double b, const Zuint& a );
unsigned int operator* ( double b, const Zuint& a );
unsigned int operator/ ( double b, const Zuint& a );

/// Zuint ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zdouble ////////////////////////////////////////////////////////////////////////////////////////// start

class Zdouble {
public:
 double value;
 Zdouble() { value=0.0; }
 Zdouble( double value ) { this->value=value; }
 string toString() {static char b[120]; return remove_trailing_zeroes(FORMAT(b, 64, "%f", value).c_str()); }
 operator string() { return toString(); }
 double abs() { return absd(value); }
 operator double() { return value; }
 operator double *() { return &value; }
 Zdouble *operator()() { return this; }
 Zdouble *self() { return this; }
 double operator()( int i ) { return value=(double)i; }
 double operator()( float f ) { return value=(double)f; }
 double operator()( double d ) { return value=d; }
 double operator()( Zbyte &b );
 double operator()( Zuint &i );
 double operator()( Zint &i );
 double operator()( Zfloat &f );
 double operator()( Zdouble &d ) { return d.value; }
 Zdouble& operator= ( const int i ) { value=(double)i; return *this; }
 Zdouble& operator= ( const float f ) { value=(double)f; return *this; }
 Zdouble& operator= ( const double d ) { value=d; return *this; }
 Zdouble& operator= ( const Zbyte& b );
 Zdouble& operator= ( const Zuint& b );
 Zdouble& operator= ( const Zint& i );
 Zdouble& operator= ( const Zfloat& f );
 Zdouble& operator= ( const Zdouble& d ) { value=d.value; return *this; }
 Zdouble& operator= ( const Zstring& s );
 Zdouble& operator+= ( const Zbyte& b ) { value+=(double)b.value; return *this; }
 Zdouble& operator-= ( const Zbyte& b ) { value-=(double)b.value; return *this; }
 Zdouble& operator*= ( const Zbyte& b ) { value*=(double)b.value; return *this; }
 Zdouble& operator/= ( const Zbyte& b ) { value/=(double)b.value; return *this; }
 Zdouble& operator+= ( const Zint& i ) { value+=(double)i.value; return *this; }
 Zdouble& operator-= ( const Zint& i ) { value-=(double)i.value; return *this; }
 Zdouble& operator*= ( const Zint& i ) { value*=(double)i.value; return *this; }
 Zdouble& operator/= ( const Zint& i ) { value/=(double)i.value; return *this; }
 Zdouble& operator+= ( const Zuint& i ) { value+=(double)i.value; return *this; }
 Zdouble& operator-= ( const Zuint& i ) { value-=(double)i.value; return *this; }
 Zdouble& operator*= ( const Zuint& i ) { value*=(double)i.value; return *this; }
 Zdouble& operator/= ( const Zuint& i ) { value/=(double)i.value; return *this; }
 Zdouble& operator+= ( const Zfloat& f ) { value+=(double)f.value; return *this; }
 Zdouble& operator-= ( const Zfloat& f ) { value-=(double)f.value; return *this; }
 Zdouble& operator*= ( const Zfloat& f ) { value*=(double)f.value; return *this; }
 Zdouble& operator/= ( const Zfloat& f ) { value/=(double)f.value; return *this; }
 Zdouble& operator+= ( const Zdouble& d ) { value+=d.value; return *this; }
 Zdouble& operator-= ( const Zdouble& d ) { value-=d.value; return *this; }
 Zdouble& operator*= ( const Zdouble& d ) { value*=d.value; return *this; }
 Zdouble& operator/= ( const Zdouble& d ) { value/=d.value; return *this; }
 Zdouble& operator+= ( const unsigned char d ) { value+=(double) d; return *this; }
 Zdouble& operator-= ( const unsigned char d ) { value-=(double) d; return *this; }
 Zdouble& operator*= ( const unsigned char d ) { value*=(double) d; return *this; }
 Zdouble& operator/= ( const unsigned char d ) { value/=(double) d; return *this; }
 Zdouble& operator+= ( const unsigned short d ) { value+=(double) d; return *this; }
 Zdouble& operator-= ( const unsigned short d ) { value-=(double) d; return *this; }
 Zdouble& operator*= ( const unsigned short d ) { value*=(double) d; return *this; }
 Zdouble& operator/= ( const unsigned short d ) { value/=(double) d; return *this; }
 Zdouble& operator+= ( const int d ) { value+=(double) d; return *this; }
 Zdouble& operator-= ( const int d ) { value-=(double) d; return *this; }
 Zdouble& operator*= ( const int d ) { value*=(double) d; return *this; }
 Zdouble& operator/= ( const int d ) { value/=(double) d; return *this; }
 Zdouble& operator+= ( const unsigned int d ) { value+=(double) d; return *this; }
 Zdouble& operator-= ( const unsigned int d ) { value-=(double) d; return *this; }
 Zdouble& operator*= ( const unsigned int d ) { value*=(double) d; return *this; }
 Zdouble& operator/= ( const unsigned int d ) { value/=(double) d; return *this; }
 Zdouble& operator+= ( const float d ) { value+=(double) d; return *this; }
 Zdouble& operator-= ( const float d ) { value-=(double) d; return *this; }
 Zdouble& operator*= ( const float d ) { value*=(double) d; return *this; }
 Zdouble& operator/= ( const float d ) { value/=(double) d; return *this; }
 Zdouble& operator+= ( const double d ) { value+=d; return *this; }
 Zdouble& operator-= ( const double d ) { value-=d; return *this; }
 Zdouble& operator*= ( const double d ) { value*=d; return *this; }
 Zdouble& operator/= ( const double d ) { value/=d; return *this; }
 Zdouble& operator++ (int) { value+=1.0; return *this; }
 Zdouble& operator-- (int) { value-=1.0; return *this; }
 const double operator-() { return -value; }
 bool operator !() { return ( value == 0.0 ); }
// Not used intentionally to create warning:
// operator float() { return (float) value; }
// operator int() { return (int) value; }
// Causes ambiguity, because this is removed, the resulting issue (no better solution, see C2593)
// (mixed type comparisons require explicit casts) is described in the test:
// operator int() { return (int) value; }
// operator float() { return (float) value; }
};

// Mixed type operations
double operator+ ( const Zdouble& a, const Zfloat& b );
double operator+ ( const Zfloat& a, const Zdouble& b );
double operator+ ( const Zdouble& a, const Zint& b );
double operator+ ( const Zint& a, const Zdouble& b );
double operator* ( const Zdouble& a, const Zfloat& b );
double operator* ( const Zfloat& a, const Zdouble& b );
double operator* ( const Zdouble& a, const Zint& b );
double operator* ( const Zint& a, const Zdouble& b );
// Zdouble and itself
double operator+ ( const Zdouble& a, const Zdouble& b );
double operator- ( const Zdouble& a, const Zdouble& b );
double operator* ( const Zdouble& a, const Zdouble& b );
double operator/ ( const Zdouble& a, const Zdouble& b );
/* Creates C2593 for some reason...
double operator+ ( Zdouble a, Zdouble b );
double operator- ( Zdouble a, Zdouble b );
double operator* ( Zdouble a, Zdouble b );
double operator/ ( Zdouble a, Zdouble b );
*/
// Casted int operations
double operator+ ( const Zdouble& a, int b );
double operator- ( const Zdouble& a, int b );
double operator* ( const Zdouble& a, int b );
double operator/ ( const Zdouble& a, int b );
double operator+ ( int b, const Zdouble& a );
double operator- ( int b, const Zdouble& a );
double operator* ( int b, const Zdouble& a );
double operator/ ( int b, const Zdouble& a );
// Float operations
double operator+ ( const Zdouble& a, float b );
double operator- ( const Zdouble& a, float b );
double operator* ( const Zdouble& a, float b );
double operator/ ( const Zdouble& a, float b );
double operator+ ( float b, const Zdouble& a );
double operator- ( float b, const Zdouble& a );
double operator* ( float b, const Zdouble& a );
double operator/ ( float b, const Zdouble& a );
// Casted double operations (leans toward double precision)
double operator+ ( const Zdouble& a, double b );
double operator- ( const Zdouble& a, double b );
double operator* ( const Zdouble& a, double b );
double operator/ ( const Zdouble& a, double b );
double operator+ ( double b, const Zdouble& a );
double operator- ( double b, const Zdouble& a );
double operator* ( double b, const Zdouble& a );
double operator/ ( double b, const Zdouble& a );
// Boolean operations
bool operator>= ( const Zdouble& a, const Zdouble& b );
bool operator>= ( const Zdouble& a, int b );
bool operator>= ( const Zdouble& a, float b );
bool operator>= ( const Zdouble& a, double b );
bool operator>= ( int a, const Zdouble& b );
bool operator>= ( float a, const Zdouble& b );
bool operator>= ( double a, const Zdouble& b );
bool operator== ( const Zdouble& a, const Zdouble& b );
bool operator== ( const Zdouble& a, int b );
bool operator== ( const Zdouble& a, float b );
bool operator== ( const Zdouble& a, double b );
bool operator== ( int a, const Zdouble& b );
bool operator== ( float a, const Zdouble& b );
bool operator== ( double a, const Zdouble& b );
bool operator!= ( const Zdouble& a, const Zdouble& b );
bool operator!= ( const Zdouble& a, int b );
bool operator!= ( const Zdouble& a, float b );
bool operator!= ( const Zdouble& a, double b );
bool operator!= ( int a, const Zdouble& b );
bool operator!= ( float a, const Zdouble& b );
bool operator!= ( double a, const Zdouble& b );
bool operator<= ( const Zdouble& a, const Zdouble& b );
bool operator<= ( const Zdouble& a, int b );
bool operator<= ( const Zdouble& a, float b );
bool operator<= ( const Zdouble& a, double b );
bool operator<= ( int a, const Zdouble& b );
bool operator<= ( float a, const Zdouble& b );
bool operator<= ( double a, const Zdouble& b );
bool operator> ( const Zdouble& a, const Zdouble& b );
bool operator> ( const Zdouble& a, int b );
bool operator> ( const Zdouble& a, float b );
bool operator> ( const Zdouble& a, double b );
bool operator> ( int a, const Zdouble& b );
bool operator> ( float a, const Zdouble& b );
bool operator> ( double a, const Zdouble& b );
bool operator< ( const Zdouble& a, const Zdouble& b );
bool operator< ( const Zdouble& a, int b );
bool operator< ( const Zdouble& a, float b );
bool operator< ( const Zdouble& a, double b );
bool operator< ( int a, const Zdouble& b );
bool operator< ( float a, const Zdouble& b );
bool operator< ( double a, const Zdouble& b );


/// Zfloat ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zstring ///////////////////////////////////////////////////////////////////////////////////////// start
class Strings;
class Zstring {
public:
 string value, result;
 unsigned int length;
 const char *p;
 Zstring() {
  value=string("");
  result=string("");
  length=0;
  p=null;
 }
 Zstring( const char *value ) {
  this->value=string(value);
  result=string("");
  rewind();
 }
 Zstring( const string &value ) {
  this->value=value;
  result=string("");
  rewind();
 }
 Zstring *self() { return this; }
 bool inside( const char * key ) {
  return (value.find(string(key)) != string::npos);
 }
 bool prefix( const char * partial ) {
  return !str_prefix(partial,c_str());
 }
 bool insideCaseless( const char * key ) {
  return str_infix(key,c_str());
 }
 bool is_inside( const char * cloud );
 bool ending( const char *s ) {
  return endsWith(lower(value).c_str(),lower(s));
 }
 // Matching case
 bool Contains( const char *s ) {
  string key(s);
  string::const_iterator it = search(value.begin(), value.end(), key.begin(), key.end());
  return (it != value.end());
 }
 //Non-Matching case
 bool contains( const char *s ) {
  string key(s);
  string data=value;
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  string::const_iterator it = search(value.begin(), value.end(), key.begin(), key.end());
  return (it != value.end());
 }
 bool contains( char c ) {
  const char *p=value.c_str();
  while ( *p != '\0' ) {
   if ( *p == c ) return true;
   p++;
  }
  return false;
 }
 bool containsAny( const char *charList ) {
  const char *p=value.c_str();
  while ( *p != '\0' ) {
   if ( char_is_of(*p,charList) ) return true;
   p++;
  }
  return false;
 }
 int count( char c ) {
  int i=0;
  const char *p=value.c_str();
  while ( *p != '\0' ) {
   if ( *p == c ) i++;
   p++;
  }
  return i;
 }
 int count( const char *s ) {
  int t=0;
  for (
   size_t nPos = value.find(s, 0); // fist occurrence
   nPos != string::npos;
   nPos = value.find(s, nPos+1)
  ) t++;
  return t;
 }
 std::string CAPS() {
  Zstring temp;
  int i=0;
  const char *p=value.c_str();
  while ( *p != '\0' ) {
   temp += FORMAT("%c",UPPER(*p));
   p++;
  }
  return temp.value;
  
 }
 int toSeed() {
  const char *p=value.c_str();
  int s=0; while ( *p != '\0' ) { s+=(int)(*p); p++; } return s;
 }
 char At( unsigned int index ) {
  if ( index >= length ) return '\0';
  return value.at(index);
 }
 void Set( unsigned int index, char c ) {
  if ( index == 0 ) {
   value=value.substr(1,length-1);
  } else if ( index == length-1 ) {
   value=value.substr(0,length-2);
   value+=c;
  } else if ( index > length || index < 0 ) return;
  else value=value.substr(0,index-1)+c+value.substr(index+1,length-index-1);
  rewind();
 }
 void Load( string filename )  { Load(filename.c_str()); }
 void Load( const char *filename )  {
  value=file_as_string(filename);
  rewind();
 }
 void Save( string filename )  { Save(filename.c_str()); }
 void Save( const char *filename )  {
  string_as_file(value.c_str(),filename);
 }
 int first( char c ) {
  const char *p=value.c_str();
  int i=0;
  while ( *p != '\0' ) {
   if ( *p == c ) return i;
   i++;
  }
  return -1;
 }
 const char *first( const Zstring& in ) {
  const char *p=string_argument(in.value.c_str(),&value);
  rewind();
  return p;
 }
 const char *first( const string& in ) {
  const char *p=string_argument(in.c_str(),&value);
  rewind();
  return p;
 }
 const char *first( const char *in ) {
  const char *p=string_argument(in,&value);
  rewind();
  return p;
 }
 // At(length-1)
 char last() { return At(length-1); }
 // returns next word (string fragment), uses next(), and one_argument
 const char *word( int number, char *out=buf ) {
  rewind();
  for ( int i=0; i<number; i++ ) next(out);
  return out;
 }
 // checks for caseless match of a "word" using next
 bool word( const char *matches, int number, char *out=buf ) {
  rewind();
  for ( int i=0; i<number; i++ ) next(out);
  return !str_cmp(matches,out);
 }
 // one_argument
 char *next( char *out=buf ) {
  p=one_argument(p,out);
  return out;
 }
 // one_argument_case
 char *next_case( char *out=buf ) {
  p=one_argument_case(p,out);
  return out;
 }
 // Limits and stores from input string to a specific length
 void Limit( const char *in, size_t truncation ) {
  if ( strlen(in) < truncation ) (*this)=in;
  else {
   (*this)="";
   for ( size_t i=0; i<truncation; i++ ) {
    (*this)+=in[i];
   }
  }
 }
 void toUpper() {
  (*this).value=upper(value);
  rewind();
 }
 void toLower() {
  (*this).value=lower(value);
  rewind();
 }
 // string_argument
 const char *Next() {
  p=string_argument(p,&result);
  return result.c_str();
 }
 // string_argument_case
 const char *Next_case() {
  p=string_argument_case(p,&result);
  return result.c_str();
 }
 // returns next value from string_argument but no advancement
 const char *Peek() {
  string_argument(p,&result);
  return result.c_str();
 }
 const char *NextSOL() {
  while ( *p != '\0' && *p != '\n' ) p++;
  if ( *p != '\0' && *p == '\n' ) p++;
  if ( *p != '\0' && *p == '\r' ) p++;
 }
 void rewind() {
  p=value.c_str();
  length=(unsigned int) this->value.length();
 }
 // is_integer || is_decimal
 bool is_number() { return is_integer(value.c_str()) || is_decimal(value.c_str()); }
 // atoi
 int integer() { return atoi(value.c_str()); }
 // atof
 double decimal() { return atof(value.c_str()); }
 // string is empty or filled with spaces;
 bool Empty() {
  Zstring temp=*this;
  temp/=" ";
  return ( length == 0 || temp.length == 0 );
 }
 void Clear() { value=string(""); length=0; }
 string *operator &() { return &value; } // some debate here
 const char *c_str() { return value.c_str(); }
 operator const char *() { return value.c_str(); }
 operator char *() { return (char *) value.c_str(); }
 operator std::string() { return value; }
 operator std::string*() { return &value; }
 Zstring *operator ++() { return this; }
 void operator= (const char *s ) {
  value=s?string(s):"";
  rewind();
 }
 void operator= (string s) {
  value=s;
  rewind();
 }
 Zstring& operator/= ( string b ) {
  replaceAll(value,b,string(""));
  rewind();
  return *this;
 }
 Zstring& operator/= ( Zstring& b ) {
  replaceAll(value,b,string(""));
  rewind();
  return *this;
 }
 operator int() {
  if (is_decimal(value.c_str())) return (int)(atof(value.c_str()));  // rounding?
  else return (int)atoi(value.c_str());
 }
 // Precise equals
 bool Equals( const char *b ) {
  string me=string(b);
  if ( me == value ) return true;
  return false;
 }
 // Caseless alpha compare
 int Compare( const char *b ) {  
  const char *p,*q;
  p=value.c_str(); while ( *p != '\0' && !IS_ALPHA(*p) ) p++;
  q=b; while ( *q != '\0' && !IS_ALPHA(*q) ) q++;
  while ( *p!= '\0' && *q!='\0' ) {
   if ( (int) LOWER(*p) > (int) LOWER(*q) ) return 1;
   if ( (int) LOWER(*p) < (int) LOWER(*q) ) return -1;
   p++; while ( *p != '\0' && !IS_ALPHA(*p) ) p++;
   q++; while ( *q != '\0' && !IS_ALPHA(*q) ) q++;
  }
  if ( *p != '\0' ) return 1;
  if ( *q != '\0' ) return -1;
  return 0;
 }
 operator float() { return (float) atof(value.c_str()); }
 operator double() { Zstring result(value); result/="$"; result/=","; return atof(result.value.c_str()); }
 operator unsigned int() { return length; }
 Zstring& operator --(int) { if ( length > 0 ) { value=value.substr(0,value.size()-1); rewind(); } return *this; }
 Zstring& operator --() { if ( length > 0 ) { value=value.substr(1,value.size()); rewind(); } return *this; }
 Zstring& operator +=( unsigned char c ) { value+=c; rewind(); return *this; }
 Zstring& operator +=( char c ) { char buf[2]; buf[0]=c; buf[1]='\0'; value=value+string(buf); rewind(); return *this; }
 Zstring& operator +=( const string& b ) { value+=b; rewind(); return *this; }
 Zstring& operator +=( const Zstring& b ) { value+=b.value; rewind(); return *this; }
 Zstring& operator +=( const char *b ) { value+=string(b); rewind(); return *this; }
 Zstring& operator -=( const Zstring& b ) { replaceOnce(value,b.value,string("")); rewind(); return *this; }
 Zstring& operator -=( const char *b ) { replaceOnce(value,string(b),string("")); rewind(); return *this; }
 Zstring& operator -=( const string& b ) { replaceOnce(value,b,string("")); rewind(); return *this; }
 Zstring& operator -=( char c ) { char buf[2]; buf[0]=c; buf[1]='\0'; replaceAll(value,string(buf),string("")); rewind(); return *this; }
 Zstring& operator /=( char c ) { char buf[2]; buf[0]=c; buf[1]='\0'; replaceOnce(value,string(buf),string("")); rewind(); return *this; }
 Zstring& operator +=( float f ) { value+=FORMAT(" %1.2f",(float)f); rewind(); return *this; }
 Zstring& operator +=( double d ) { value+=FORMAT(" %1.4f",(double)d); rewind(); return *this; }
 Zstring& operator +=( int i ) { value+=FORMAT(" %d",(int)i); rewind(); return *this; }
 Zstring& operator -=( int i ) {
  rewind();
  const char *w=value.c_str();
  while ( i > 0 ) {
   i--;
   w=Next();
  }
  value=string(this->p);
  rewind();
  return *this;
 }
 string Words();
 string LimitTo( int len );
 string WordWrap( int linelen );
 string WordWrapNewlines( int linelen, const char * forcednewline="\\" );
 bool operator !() { return ( Empty() ); }
 bool operator () ( const char *partial ) { return !str_prefix(value.c_str(),partial); }
 bool operator () ( const string& partial ) { return !str_prefix(value.c_str(),partial.c_str()); }
 bool operator () ( const char *partial, bool anywhere ) { return !str_infix(value.c_str(),partial); }
 bool operator () ( const string& partial, bool anywhere ) { return !str_infix(value.c_str(),partial.c_str()); }
 char operator[] (unsigned int index) { return At(index); }
 char operator() (unsigned int index) { return At(index%length); }
 Zstring& operator() ( const char *search, const char *replace ) { replaceAll(value,string(search),string(replace)); rewind(); return *this; }
 string operator() ( int start, int count ) {
  return value.substr( start, count );
 }
 void shred() {
  for ( char i=32; i<127; i++ ) {
   Zstring num;
   num+=(char)(48+upto(10));
   Zstring search;
   search+=(char)i;
   (*this)(search.c_str(),num.c_str());
  }
 }
 void rTrim(const string& delimiters = " \f\n\r\t\v") {
  rtrim(value,delimiters);
  rewind();
 }
 void lTrim(const string& delimiters = " \f\n\r\t\v") {
  ltrim(value,delimiters);
  rewind();
 }
 void Trim(const string& delimiters = " \f\n\r\t\v") {
  trim(value,delimiters);
  rewind();
 }
 // Case insensitive
 bool begins(const char *s) {
  int len=(int)strlen(s);
  if ( len > (int) value.length() ) return false;
  const char *p=value.c_str();
  const char *q=s;
  for ( int i=0; i<len; i++,p++,q++ ) if ( LOWER(*p) != LOWER(*q) ) return false;
  return true;
 }
};

bool operator==( const Zstring& a, const Zstring& b );
bool operator==( const Zstring& a, const char *b );
bool operator==( const char *b , const Zstring& a );
string operator+ ( const Zstring& a, const Zstring& b );
string operator+ ( const Zstring& a, const string& b );
string operator+ ( const string& a, const Zstring& b );
string operator+ ( const Zstring& a, const int& b );
string operator+ ( const Zstring& a, const unsigned int& b );
string operator+ ( const Zstring& a, const float& b );
string operator+ ( const Zstring& a, const double& b );
string operator+ ( const int& a, const Zstring& b );
string operator+ ( const unsigned int& a, const Zstring& b );
string operator+ ( const float& a, const Zstring& b );
string operator+ ( const double& a, const Zstring& b );
string operator+ ( string a, Zint& b );
string operator+ ( string a, Zuint& b );
string operator+ ( string a, Zfloat& b );
string operator+ ( string a, Zdouble& b );
/* Creates C2593
string operator+ ( Zint& a, string b );
string operator+ ( Zuint& a, string b );
string operator+ ( Zfloat& a, string b );
string operator+ ( Zdouble& a, string b );
*/
int operator+ ( Zint& a, Zint& b );
float operator+ ( Zfloat& a, Zfloat& b );
double operator+ ( Zdouble& a, Zdouble& b );
string operator+ ( const Zstring& a, char c );
string operator+ ( char c, const Zstring& a );
string operator- ( const Zstring& a, const Zstring& b );
string operator- ( const Zstring& a, const string& b );
string operator- ( const string& b, const Zstring& a );
string operator- ( const Zstring& a, const char *b );
//string operator- ( const char *b, const Zstring& a );
string operator/ ( const Zstring& a, const Zstring& b );
string operator/ ( const Zstring& a, const string& b );
string operator/ ( const string& b, const Zstring& a );
string operator/ ( const Zstring& a, const char *b );
string operator/ ( const char *b, const Zstring& a );
string operator* ( const Zstring& a, unsigned int i );
int operator* ( const Zstring& a, string b );
int operator* ( string a, const Zstring& b );
int operator* ( const Zstring& a, const char *B );
int operator* ( const Zstring& a, const Zstring& b );

/// Zstring ///////////////////////////////////////////////////////////////////////////////////////// end

/// Zpointer //////////////////////////////////////////////////////////////////////////////////////// start

template<class T>
class Zpointer {
public:
 T *pointer;
 Zpointer() { pointer=null; }
 Zpointer( T *value ) { this->pointer=value; }
 ~Zpointer() {
  return;
 }
 Zpointer( Zpointer<T>& p ) { this->pointer=p.pointer; }
 void Delete() {
  if ( pointer ) {
   delete pointer;
   pointer=null;
  }
 }
 void Delete( T *replace ) {
  if ( pointer ) {
   delete pointer;
   pointer=replace;
  }
 }
 operator T*() { return pointer; }
 T operator* ( Zpointer<T>& t ) { return *pointer; }
// T **operator& ( Zpointer<T>& t ) { return &pointer; }
 Zpointer<T> *self() { return this; }
 Zpointer<T>& operator()( T *value ) { pointer=value; return *this; }
 Zpointer<T>& operator()( Zpointer<T>& t ) { pointer=t.pointer; return *this; }
 Zpointer<T>& operator= ( T *value ) { pointer=value; return *this; }
 Zpointer<T>& operator= ( Zpointer<T>& t ) { pointer=t.pointer; return *this; }
 T *operator->() {
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
  if ( !pointer ) {
   OUTPUT("Zpointer:->() referenced when null.\n");
  }
#endif
  return pointer;
 }
};

typedef Zpointer<void>   Pvoid;
typedef Zpointer<int>    Pint;
typedef Zpointer<float>  Pfloat;
typedef Zpointer<double> Pdouble;

/*
 * Protected zPointer which only allows itself to be set once,
 * used mainly for debugging.
 */
template<class T>
class pZpointer {
public:
 Zint set;
 T *pointer;
 pZpointer() { pointer=null; }
 pZpointer( T *value ) { this->pointer=value; }
 ~pZpointer() {
  return;
 }
 pZpointer( pZpointer<T>& p ) { this->pointer=p.pointer; }
 void Delete() {
  if ( pointer ) {
   delete pointer;
   pointer=null;
  }
 }
 operator T*() { return pointer; }
 T operator* ( pZpointer<T>& t ) { return *pointer; }
// T **operator& ( Zpointer<T>& t ) { return &pointer; }
 bool Protected() {
  if ( set == 1 ) {
   OUTPUT("pZpointer Warning: protected pointer assigned twice!\n");
   return false;
  } else set++;
  return true;
 }
 pZpointer<T> *self() { return this; }
 pZpointer<T>& operator()( T *value ) { if (Protected()) return *this; pointer=value; return *this; }
 pZpointer<T>& operator()( Zpointer<T>& t ) { if (Protected()) return *this; pointer=t.pointer; return *this; }
 pZpointer<T>& operator()( pZpointer<T>& t ) { if (Protected()) return *this; pointer=t.pointer; return *this; }
 pZpointer<T>& operator= ( T *value ) { if (Protected()) return *this; pointer=value; return *this; }
 pZpointer<T>& operator= ( Zpointer<T>& t ) { if (Protected()) return *this; pointer=t.pointer; return *this; }
 pZpointer<T>& operator= ( pZpointer<T>& t ) { if (Protected()) return *this; pointer=t.pointer; return *this; }
 T *operator->() {
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
  if ( !pointer ) {
   OUTPUT("pZpointer:->() referenced when null.\n");
  }
#endif
  return pointer;
 }
};
#define pZp pZpointer

// Notes from the underground
/*
class Zvoid : public Zpointer<void> {
public:
  Zvoid() { .. }
  Zvoid(void* value) : Zpointer<void>(value) { .. }
  Zvoid(Zvoid& p) : Zpointer<void>(p) { .. } 
  using Zpointer<void>::operator =;
};
 */

// On debugging pointers:  Zpointer could be attached to a historical pointer system
// that keeps track of all assignments, accesses, etc.

// This would reproduce the work of "$12000" programs

/// Zpointer //////////////////////////////////////////////////////////////////////////////////////// end

/// Zdisposable ///////////////////////////////////////////////////////////////////////////////////// start

template<class T>
class Zdisposable {
public:
 T *pointer;
 Zdisposable() {
  pointer=null;
 }
 void I() { Instantiate(); }
 void Instantiate() {
  if ( !pointer ) pointer=new T;
 }
 void Deinstantiate() {
  if ( pointer ) {
   delete pointer;
   pointer=null;
  }
 }
 void Recycle() {
  if ( pointer ) delete pointer;
  pointer=new T;
 }
 void Recycle( T *replacement ) {
  if ( pointer ) delete pointer;
  pointer=replacement;
 }
 operator T*() { return pointer; }
 T *operator= ( T *replacement ) {
  Recycle(replacement);
  return pointer;
 }
 Zdisposable<T> *self() { return this; }
 T *operator= ( Zdisposable<T>& t ) {
  if ( pointer ) {
   if ( pointer != t.pointer ) Deinstantiate();
  }
  pointer=t.pointer;
  t.pointer=null;
  return pointer;
 }
 T operator* ( Zdisposable<T>& t ) { return *pointer; }
 T operator& ( Zdisposable<T>& t ) { return &pointer; }
// T **operator& ( Zpointer<T>& t ) { return &pointer; }
 T *operator->() {
  if ( !pointer ) {
   Instantiate();
  }
  return pointer;
 }
 ~Zdisposable() {
  Deinstantiate();
 }
};
#define Zdis Zdisposable

/// Zdisposable ///////////////////////////////////////////////////////////////////////////////////// end

void ZeroTypesTest();