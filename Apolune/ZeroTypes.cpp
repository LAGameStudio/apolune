#include "ZeroTypes.h"

/// Temporary buffer for int->string conversions
char tbuf[128];

/// Forward declared type assocations

/// Zfloat operator= overloads for forward declared types

Zfloat& Zfloat::operator= ( const Zbyte& b   ) { value=(float)b.value; return *this; }
Zfloat& Zfloat::operator= ( const Zuint& i   ) { value=(float)i.value; return *this; }
Zfloat& Zfloat::operator= ( const Zint& i    ) { value=(float)i.value; return *this; }
Zfloat& Zfloat::operator= ( const Zdouble& d ) { value=(float)d.value; return *this; }
Zfloat& Zfloat::operator= ( const Zstring& s ) { value=(float)atof(s.value.c_str()); return *this; }
float Zfloat::operator()( Zbyte &b )   { return value=(float)b.value; }
float Zfloat::operator()( Zuint &i )   { return value=(float)i.value; }
float Zfloat::operator()( Zint &i )    { return value=(float)i.value; }
float Zfloat::operator()( Zfloat &f )  { return value; }
float Zfloat::operator()( Zdouble &d ) { return value=(float)d.value; }
Zfloat& Zfloat::operator+= ( const Zint& i ) { value+=(float)i.value; return *this; }
Zfloat& Zfloat::operator-= ( const Zint& i ) { value-=(float)i.value; return *this; }
Zfloat& Zfloat::operator*= ( const Zint& i ) { value*=(float)i.value; return *this; }
Zfloat& Zfloat::operator/= ( const Zint& i ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( (float) i.value == 0.0f ) {  
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in /= operation!\n");
#endif
  value=0.0f;
  return *this;
 }
#endif
 value/=(float)i.value;
 return *this;
}

/// Zbyte operator= overloads for forward declared types

Zbyte& Zbyte::operator= ( const Zuint& i )   { value=(unsigned char)i.value; return *this; }
Zbyte& Zbyte::operator= ( const Zint& i )    { value=(unsigned char)i.value; return *this; }
Zbyte& Zbyte::operator= ( const Zfloat& f )  { value=(unsigned char)f.value; return *this; }
Zbyte& Zbyte::operator= ( const Zdouble& d ) { value=(unsigned char)d.value; return *this; }
Zbyte& Zbyte::operator= ( const Zstring& s ) { value=(unsigned char)atof(s.value.c_str()); return *this; }
unsigned char Zbyte::operator()( Zuint &i )   { return value=(unsigned char)i.value; }
unsigned char Zbyte::operator()( Zint &i )    { return value=(unsigned char)i.value; }
unsigned char Zbyte::operator()( Zfloat &f )  { return value=(unsigned char)f.value; }
unsigned char Zbyte::operator()( Zdouble &d ) { return value=(unsigned char)d.value; }

/// Zushort operator= overloads for forward declared types

Zushort& Zushort::operator= ( const Zuint& i )   { value=(unsigned short)i.value; return *this; }
Zushort& Zushort::operator= ( const Zint& i )    { value=(unsigned short)i.value; return *this; }
Zushort& Zushort::operator= ( const Zfloat& f )  { value=(unsigned short)f.value; return *this; }
Zushort& Zushort::operator= ( const Zdouble& d ) { value=(unsigned short)d.value; return *this; }
Zushort& Zushort::operator= ( const Zstring& s ) { value=(unsigned short)atof(s.value.c_str()); return *this; }
unsigned short Zushort::operator()( Zuint &i )   { return value=(unsigned short)i.value; }
unsigned short Zushort::operator()( Zint &i )    { return value=(unsigned short)i.value; }
unsigned short Zushort::operator()( Zfloat &f )  { return value=(unsigned short)f.value; }
unsigned short Zushort::operator()( Zdouble &d ) { return value=(unsigned short)d.value; }

/// Zuint operator= overloads for forward declared types

Zuint& Zuint::operator= ( const Zbyte& b )   { value=(unsigned int)b.value; return *this; }
Zuint& Zuint::operator= ( const Zint& i )    { value=(unsigned int)i.value; return *this; }
Zuint& Zuint::operator= ( const Zfloat& f )  { value=(unsigned int)f.value; return *this; }
Zuint& Zuint::operator= ( const Zdouble& d ) { value=(unsigned int)d.value; return *this; }
Zuint& Zuint::operator= ( const Zstring& s ) { value=(unsigned int)atof(s.value.c_str()); return *this; }
unsigned int Zuint::operator()( Zbyte &b )   { return value=(unsigned int)b.value; }
unsigned int Zuint::operator()( Zint &i )    { return value=(unsigned int)i.value; }
unsigned int Zuint::operator()( Zfloat &f )  { return value=(unsigned int)f.value; }
unsigned int Zuint::operator()( Zdouble &d ) { return value=(unsigned int)d.value; }

/// Zint operator= overloads for forward declared types

Zint& Zint::operator= ( const Zbyte& b )   { value=(int)b.value; return *this; }
Zint& Zint::operator= ( const Zuint& i )   { value=(int)i.value; return *this; }
Zint& Zint::operator= ( const Zfloat& f )  { value=(int)f.value; return *this; }
Zint& Zint::operator= ( const Zdouble& d ) { value=(int)d.value; return *this; }
Zint& Zint::operator= ( const Zstring& s ) { value=(int)atof(s.value.c_str()); return *this; }
int Zint::operator()( Zbyte &b )   { return (value=(int)b.value); }
int Zint::operator()( Zuint &i )   { return (value=(int)i.value); }
int Zint::operator()( Zfloat &f )  { return (value=(int)f.value); }
int Zint::operator()( Zdouble &d ) { return (value=(int)d.value); }
Zint& Zint::operator+= ( const Zfloat& f ) { value+=(int)f.value; return *this; }
Zint& Zint::operator-= ( const Zfloat& f ) { value-=(int)f.value; return *this; }
Zint& Zint::operator*= ( const Zfloat& f ) { value*=(int)f.value; return *this; }
Zint& Zint::operator/= ( const Zfloat& f ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( (int) f.value == 0.0f ) {  
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in /= operation!\n");
#endif
  value=0;
  return *this;
 }
#endif
 value/=(int)f.value;
 return *this;
}

/// Zdouble operator= overloads for forward declared types

Zdouble& Zdouble::operator= ( const Zbyte& b )   { value=(double)b.value; return *this; }
Zdouble& Zdouble::operator= ( const Zuint& i )   { value=(double)i.value; return *this; }
Zdouble& Zdouble::operator= ( const Zint& i )    { value=(double)i.value; return *this; }
Zdouble& Zdouble::operator= ( const Zfloat& f )  { value=(double)f.value; return *this; }
Zdouble& Zdouble::operator= ( const Zstring& s ) { value=(double)atof(s.value.c_str()); return *this; }
double Zdouble::operator()( Zbyte &b )  { return value=(double)b.value; }
double Zdouble::operator()( Zuint &i )  { return value=(double)i.value; }
double Zdouble::operator()( Zint &i )   { return value=(double)i.value; }
double Zdouble::operator()( Zfloat &f ) { return value=(double)f.value; }

/// Zfloat ////////////////////////////////////////////////////////////////////////////////////////// start

// Mixed type operations
float operator+ ( const Zfloat& a, const Zint& b ) { return (float) b.value + a.value; }
float operator+ ( const Zint& a, const Zfloat& b ) { return (float) a.value + b.value; }
float operator* ( const Zfloat& a, const Zint& b ) { return (float) b.value * a.value; }
float operator* ( const Zint& a, const Zfloat& b ) { return (float) a.value * b.value; }

// Zfloat and itself
float operator+ ( const Zfloat& a, const Zfloat& b ) { return a.value+b.value; }
float operator- ( const Zfloat& a, const Zfloat& b ) { return a.value-b.value; }
float operator* ( const Zfloat& a, const Zfloat& b ) { return a.value*b.value; }
float operator/ ( const Zfloat& a, const Zfloat& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/b.value;
}
/* Creates C2593 for some reason...
float operator+ ( Zfloat a, Zfloat b ) { return a.value+b.value; }
float operator- ( Zfloat a, Zfloat b ) { return a.value-b.value; }
float operator* ( Zfloat a, Zfloat b ) { return a.value*b.value; }
float operator/ ( Zfloat a, Zfloat b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/b.value;
}
*/

/// Zfloat and int

// Casted int operations
float operator+ ( const Zfloat& a, int b ) { return a.value+(float)b; }
float operator- ( const Zfloat& a, int b ) { return a.value-(float)b; }
float operator* ( const Zfloat& a, int b ) { return a.value*(float)b; }
float operator/ ( const Zfloat& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/(float)b;
}
// Casted int operations reversed
float operator+ ( int b, const Zfloat& a ) { return a.value+(float)b; }
float operator- ( int b, const Zfloat& a ) { return (float)b-a.value; }
float operator* ( int b, const Zfloat& a ) { return a.value*(float)b; }
float operator/ ( int b, const Zfloat& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return (float)b/a.value;
}

/// Zfloat and float

// Float operations
float operator+ ( float b, const Zfloat& a ) { return a.value+b; }
float operator- ( float b, const Zfloat& a ) { return b-a.value; }
float operator* ( float b, const Zfloat& a ) { return a.value*b; }
float operator/ ( float b, const Zfloat& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return b/a.value;
}
// Float operations reversed
float operator+ ( const Zfloat& a, float b ) { return a.value+b; }
float operator- ( const Zfloat& a, float b ) { return a.value-b; }
float operator* ( const Zfloat& a, float b ) { return a.value*b; }
float operator/ ( const Zfloat& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/b;
}

/// Zfloat and double

// Casted double operations (leans toward double precision)
float operator+ ( double b, const Zfloat& a ) { return (float)((double)a.value+b); }
float operator- ( double b, const Zfloat& a ) { return (float)(b-(double)a.value); }
float operator* ( double b, const Zfloat& a ) { return (float)((double)a.value*b); }
float operator/ ( double b, const Zfloat& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0f) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return (float)(b/(double)a.value);
}
// Casted double operations reversed (leans toward double precision)
float operator+ ( const Zfloat& a, double b ) { return (float)((double)a.value+b); }
float operator- ( const Zfloat& a, double b ) { return (float)((double)a.value-b); }
float operator* ( const Zfloat& a, double b ) { return (float)((double)a.value*b); }
float operator/ ( const Zfloat& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zfloat: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return (float)((double)a.value/b);
}

// Zfloat boolean operations
bool operator>= ( const Zfloat& a, const Zfloat& b ) { return ( a.value >= b.value );    }
bool operator>= ( const Zfloat& a, const Zbyte& b ) { return ( a.value >= (float) b.value );    }
bool operator>= ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value >= b.value ); }
bool operator>= ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value >= b.value ); }
bool operator>= ( const Zfloat& a, const Zint& b )    { return ((int) a.value >= b.value ); }
bool operator>= ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value >= b.value ); }
bool operator>= ( const Zbyte& a, const Zfloat& b )   { return (a.value >= (unsigned char) b.value ); }
bool operator>= ( const Zushort& a, const Zfloat& b ) { return (a.value >= (unsigned short) b.value ); }
bool operator>= ( const Zuint& a, const Zfloat& b )   { return (a.value >= (unsigned int) b.value ); }
bool operator>= ( const Zint& a, const Zfloat& b )    { return (a.value >= (int) b.value ); }
bool operator>= ( const Zdouble& a, const Zfloat& b ) { return (a.value >= (double) b.value ); }
bool operator>= ( const Zfloat& a, int b )           { return ( (int) a.value >= b );    }
bool operator>= ( const Zfloat& a, float b )         { return ( a.value >= b );          }
bool operator>= ( const Zfloat& a, double b )        { return ( (double) a.value >= b ); }
bool operator>= ( int a, const Zfloat& b )           { return ( a >= (int) b.value );    }
bool operator>= ( float a, const Zfloat& b )         { return ( a >= b.value );          }
bool operator>= ( double a, const Zfloat& b )        { return ( a >= (double) b.value ); }
bool operator== ( const Zfloat& a, const Zbyte& b )   { return ((unsigned char) a.value == b.value ); }
bool operator== ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value == b.value ); }
bool operator== ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value == b.value ); }
bool operator== ( const Zfloat& a, const Zint& b )    { return ((int) a.value == b.value ); }
bool operator== ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value == b.value ); }
bool operator== ( const Zbyte& a, const Zfloat& b )   { return (a.value == (unsigned char) b.value ); }
bool operator== ( const Zushort& a, const Zfloat& b ) { return (a.value == (unsigned short) b.value ); }
bool operator== ( const Zuint& a, const Zfloat& b )   { return (a.value == (unsigned int) b.value ); }
bool operator== ( const Zint& a, const Zfloat& b )    { return (a.value == (int) b.value ); }
bool operator== ( const Zdouble& a, const Zfloat& b ) { return (a.value == (double) b.value ); }
bool operator== ( const Zfloat& a, const Zfloat& b ) { return ( a.value == b.value );    }
bool operator== ( const Zfloat& a, int b )           { return ( (int) a.value == b );    }
bool operator== ( const Zfloat& a, float b )         { return ( a.value == b );          }
bool operator== ( const Zfloat& a, double b )        { return ( (double) a.value == b ); }
bool operator== ( int a, const Zfloat& b )           { return ( a == (int) b.value );    }
bool operator== ( float a, const Zfloat& b )         { return ( a == b.value );          }
bool operator== ( double a, const Zfloat& b )        { return ( a == (double) b.value ); }
bool operator!= ( const Zfloat& a, const Zfloat& b ) { return ( a.value != b.value );    }
bool operator!= ( const Zfloat& a, const Zbyte& b ) { return ( a.value != (float)b.value );    }
bool operator!= ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value != b.value ); }
bool operator!= ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value != b.value ); }
bool operator!= ( const Zfloat& a, const Zint& b )    { return ((int) a.value != b.value ); }
bool operator!= ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value != b.value ); }
bool operator!= ( const Zbyte& a, const Zfloat& b )   { return (a.value != (unsigned char) b.value ); }
bool operator!= ( const Zushort& a, const Zfloat& b ) { return (a.value != (unsigned short) b.value ); }
bool operator!= ( const Zuint& a, const unsigned int b )  { return (a.value != b ); }
bool operator!= ( const Zuint& a, const int b )       { return (a.value != (unsigned int) b ); }
bool operator!= ( const Zuint& a, const Zfloat& b )   { return (a.value != (unsigned int) b.value ); }
bool operator!= ( const Zint& a, const Zfloat& b )    { return (a.value != (int) b.value ); }
bool operator!= ( const Zdouble& a, const Zfloat& b ) { return (a.value != (double) b.value ); }
bool operator!= ( const Zfloat& a, int b )           { return ( (int) a.value != b );    }
bool operator!= ( const Zfloat& a, float b )         { return ( a.value != b );          }
bool operator!= ( const Zfloat& a, double b )        { return ( (double) a.value != b ); }
bool operator!= ( int a, const Zfloat& b )           { return ( a != (int) b.value );    }
bool operator!= ( float a, const Zfloat& b )         { return ( a != b.value );          }
bool operator!= ( double a, const Zfloat& b )        { return ( a != (double) b.value ); }
bool operator<= ( const Zfloat& a, const Zfloat& b ) { return ( a.value <= b.value );    }
bool operator<= ( const Zfloat& a, const Zbyte& b )  { return ( a.value <= (float) b.value ); }
bool operator<= ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value <= b.value ); }
bool operator<= ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value <= b.value ); }
bool operator<= ( const Zfloat& a, const Zint& b )    { return ((int) a.value <= b.value ); }
bool operator<= ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value <= b.value ); }
bool operator<= ( const Zbyte& a, const Zfloat& b )   { return (a.value <= (unsigned char) b.value ); }
bool operator<= ( const Zushort& a, const Zfloat& b ) { return (a.value <= (unsigned short) b.value ); }
bool operator<= ( const Zuint& a, const Zfloat& b )   { return (a.value <= (unsigned int) b.value ); }
bool operator<= ( const Zint& a, const Zfloat& b )    { return (a.value <= (int) b.value ); }
bool operator<= ( const Zdouble& a, const Zfloat& b ) { return (a.value <= (double) b.value ); }
bool operator<= ( const Zfloat& a, int b )           { return ( (int) a.value <= b );    }
bool operator<= ( const Zfloat& a, float b )         { return ( a.value <= b );          }
bool operator<= ( const Zfloat& a, double b )        { return ( (double) a.value <= b ); }
bool operator<= ( int a, const Zfloat& b )           { return ( a <= (int) b.value );    }
bool operator<= ( float a, const Zfloat& b )         { return ( a <= b.value );          }
bool operator<= ( double a, const Zfloat& b )        { return ( a <= (double) b.value ); }
bool operator> ( const Zfloat& a, const Zfloat& b )  { return ( a.value > b.value );    }
bool operator> ( const Zfloat& a, const Zbyte& b )   { return ( a.value > (float) b.value ); }
bool operator> ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value > b.value ); }
bool operator> ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value > b.value ); }
bool operator> ( const Zfloat& a, const Zint& b )    { return ((int) a.value > b.value ); }
bool operator> ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value > b.value ); }
bool operator> ( const Zbyte& a, const Zfloat& b )   { return (a.value > (unsigned char) b.value ); }
bool operator> ( const Zushort& a, const Zfloat& b ) { return (a.value > (unsigned short) b.value ); }
bool operator> ( const Zuint& a, const Zfloat& b )   { return (a.value > (unsigned int) b.value ); }
bool operator> ( const Zint& a, const Zfloat& b )    { return (a.value > (int) b.value ); }
bool operator> ( const Zdouble& a, const Zfloat& b ) { return (a.value > (double) b.value ); }
bool operator> ( const Zfloat& a, int b )            { return ( (int) a.value > b );    }
bool operator> ( const Zfloat& a, float b )          { return ( a.value > b );          }
bool operator> ( const Zfloat& a, double b )         { return ( (double) a.value > b ); }
bool operator> ( int a, const Zfloat& b )            { return ( a > (int) b.value );    }
bool operator> ( float a, const Zfloat& b )          { return ( a > b.value );          }
bool operator> ( double a, const Zfloat& b )         { return ( a > (double) b.value ); }
bool operator< ( const Zfloat& a, const Zfloat& b )  { return ( a.value < b.value );    }
bool operator< ( const Zfloat& a, const Zbyte& b )   { return ( a.value < (float) b.value ); }
bool operator< ( const Zfloat& a, const Zushort& b ) { return ((unsigned short) a.value < b.value ); }
bool operator< ( const Zfloat& a, const Zuint& b )   { return ((unsigned int) a.value < b.value ); }
bool operator< ( const Zfloat& a, const Zint& b )    { return ((int) a.value < b.value ); }
bool operator< ( const Zfloat& a, const Zdouble& b ) { return ((double) a.value < b.value ); }
bool operator< ( const Zbyte& a, const Zfloat& b )   { return (a.value < (unsigned char) b.value ); }
bool operator< ( const Zushort& a, const Zfloat& b ) { return (a.value < (unsigned short) b.value ); }
bool operator< ( const Zuint& a, const Zfloat& b )   { return (a.value < (unsigned int) b.value ); }
bool operator< ( const Zint& a, const Zfloat& b )    { return (a.value < (int) b.value ); }
bool operator< ( const Zdouble& a, const Zfloat& b ) { return (a.value < (double) b.value ); }
bool operator< ( const Zfloat& a, int b )            { return ( (int) a.value < b );    }
bool operator< ( const Zfloat& a, float b )          { return ( a.value < b );          }
bool operator< ( const Zfloat& a, double b )         { return ( (double) a.value < b ); }
bool operator< ( int a, const Zfloat& b )            { return ( a < (int) b.value );    }
bool operator< ( float a, const Zfloat& b )          { return ( a < b.value );          }
bool operator< ( double a, const Zfloat& b )         { return ( a < (double) b.value ); }

/// Zfloat ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zbyte ////////////////////////////////////////////////////////////////////////////////////////// start

unsigned char operator+ ( const Zbyte& a, const Zbyte& b ) { return a.value+b.value; }
unsigned char operator- ( const Zbyte& a, const Zbyte& b ) { return a.value-b.value; }
unsigned char operator* ( const Zbyte& a, const Zbyte& b ) { return a.value*b.value; }
unsigned char operator/ ( const Zbyte& a, const Zbyte& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}
unsigned char operator+ ( Zbyte a, Zbyte b ) { return a.value+b.value; }
unsigned char operator- ( Zbyte a, Zbyte b ) { return a.value-b.value; }
unsigned char operator* ( Zbyte a, Zbyte b ) { return a.value*b.value; }
unsigned char operator/ ( Zbyte a, Zbyte b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}

/// Zbyte and uint

// Casted int operations
unsigned char operator+ ( const Zbyte& a, unsigned char b ) {
 return a.value+b;
}
unsigned char operator- ( const Zbyte& a, unsigned char b ) {
 return a.value-b;
}
unsigned char operator* ( const Zbyte& a, unsigned char b ) {
 return a.value*b;
}
unsigned char operator/ ( const Zbyte& a, unsigned char b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b;
}
// Casted int operations reversed
unsigned char operator+ ( unsigned char b, const Zbyte& a ) {
 return a.value+b;
}
unsigned char operator- ( unsigned char b, const Zbyte& a ) {
 return b-a.value;
}
unsigned char operator* ( unsigned char b, const Zbyte& a ) {
 return a.value*b;
}
unsigned char operator/ ( unsigned char b, const Zbyte& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return b/a.value;
}

/// Zuint and int

// Casted int operations
unsigned char operator+ ( const Zbyte& a, int b ) {
 return a.value+(unsigned char) b;
}
unsigned char operator- ( const Zbyte& a, int b ) {
 return a.value-(unsigned char) b;
}
unsigned char operator* ( const Zbyte& a, int b ) {
 return a.value*(unsigned char) b;
}
unsigned char operator/ ( const Zbyte& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/(unsigned char) b;
}
// Casted int operations reversed
unsigned char operator+ ( int b, const Zbyte& a ) {
 return a.value+(unsigned char) b;
}
unsigned char operator- ( int b, const Zbyte& a ) {
 return (unsigned char) b-a.value;
}
unsigned char operator* ( int b, const Zbyte& a ) {
 return a.value*(unsigned char) b;
}
unsigned char operator/ ( int b, const Zbyte& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned char) b/a.value;
}

/// Zbyte and float

// Float operations (leans toward float precision)
unsigned char operator+ ( float b, const Zbyte& a ) {
 return (unsigned char)((float)a.value+b);
}
unsigned char operator- ( float b, const Zbyte& a ) {
 return (unsigned char)(b-(float)a.value);
}
unsigned char operator* ( float b, const Zbyte& a ) {
 return (unsigned char)((float)a.value*b);
}
unsigned char operator/ ( float b, const Zbyte& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned char)(b/(float)a.value);
}
// Float operations reversed (leans toward float precision)
unsigned char operator+ ( const Zbyte& a, float b ) {
 return (unsigned char)((float)a.value+b);
}
unsigned char operator- ( const Zbyte& a, float b ) {
 return (unsigned char)((float)a.value-b);
}
unsigned char operator* ( const Zbyte& a, float b ) {
 return (unsigned char)((float)a.value*b);
}
unsigned char operator/ ( const Zbyte& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif

 return (unsigned char)((float)a.value/b);
}

/// Zbyte and double

// Casted double operations (leans toward double precision)
unsigned char operator+ ( double b, const Zbyte& a ) {
 return (unsigned char)((double)a.value+b);
}
unsigned char operator- ( double b, const Zbyte& a ) {
 return (unsigned char)(b-(double)a.value);
}
unsigned char operator* ( double b, const Zbyte& a ) {
 return (unsigned char)((double)a.value*b);
}
unsigned char operator/ ( double b, const Zbyte& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned char)(b/(double)a.value);
}
// Casted double operations reversed (leans toward double precision)
unsigned char operator+ ( const Zbyte& a, double b ) {
 return (unsigned char)((double)a.value+b);
}
unsigned char operator- ( const Zbyte& a, double b ) {
 return (unsigned char)((double)a.value-b);
}
unsigned char operator* ( const Zbyte& a, double b ) {
 return (unsigned char)((double)a.value*b);
}
unsigned char operator/ ( const Zbyte& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned char)((double)a.value/b);
}


// Boolean operations
bool operator>= ( const Zbyte& a, const Zbyte& b ) { return ( a.value >= b.value );    }
bool operator>= ( const Zbyte& a, int b )            { return ( a.value >= (unsigned char) b ); }
bool operator>= ( const Zbyte& a, float b )          { return ( a.value >= (unsigned char) b ); }
bool operator>= ( const Zbyte& a, double b )         { return ( a.value >= (unsigned char) b ); }
bool operator>= ( int a, const Zbyte& b )            { return ( (unsigned char) a >= b.value ); }
bool operator>= ( float a, const Zbyte& b )          { return ( (unsigned char) a >= b.value ); }
bool operator>= ( double a, const Zbyte& b )         { return ( (unsigned char) a >= b.value ); }
bool operator== ( const Zbyte& a, const Zbyte& b ) { return ( a.value == b.value );    }
bool operator== ( const Zbyte& a, int b )            { return ( a.value == (unsigned char) b ); }
bool operator== ( const Zbyte& a, float b )          { return ( a.value == (unsigned char) b ); }
bool operator== ( const Zbyte& a, double b )         { return ( a.value == (unsigned char) b ); }
bool operator== ( int a, const Zbyte& b )            { return ( (unsigned char) a == b.value ); }
bool operator== ( float a, const Zbyte& b )          { return ( (unsigned char) a == b.value ); }
bool operator== ( double a, const Zbyte& b )         { return ( (unsigned char) a == b.value ); }
bool operator!= ( const Zbyte& a, const Zbyte& b ) { return ( a.value != b.value );    }
bool operator!= ( const Zbyte& a, int b )            { return ( a.value != (unsigned char) b ); }
bool operator!= ( const Zbyte& a, float b )          { return ( a.value != (unsigned char) b ); }
bool operator!= ( const Zbyte& a, double b )         { return ( a.value != (unsigned char) b ); }
bool operator!= ( int a, const Zbyte& b )            { return ( (unsigned char) a != b.value ); }
bool operator!= ( float a, const Zbyte& b )          { return ( (unsigned char) a != b.value ); }
bool operator!= ( double a, const Zbyte& b )         { return ( (unsigned char) a != b.value ); }
bool operator<= ( const Zbyte& a, const Zbyte& b ) { return ( a.value <= b.value );    }
bool operator<= ( const Zbyte& a, int b )            { return ( a.value <= (unsigned char) b ); }
bool operator<= ( const Zbyte& a, float b )          { return ( a.value <= (unsigned char) b ); }
bool operator<= ( const Zbyte& a, double b )         { return ( a.value <= (unsigned char) b ); }
bool operator<= ( int a, const Zbyte& b )            { return ( (unsigned char) a <= b.value ); }
bool operator<= ( float a, const Zbyte& b )          { return ( (unsigned char) a <= b.value ); }
bool operator<= ( double a, const Zbyte& b )         { return ( (unsigned char) a <= b.value ); }
bool operator> ( const Zbyte& a, const Zbyte& b )  { return ( a.value > b.value );    }
bool operator> ( const Zbyte& a, int b )             { return ( a.value > (unsigned char) b ); }
bool operator> ( const Zbyte& a, float b )           { return ( a.value > (unsigned char) b ); }
bool operator> ( const Zbyte& a, double b )          { return ( a.value > (unsigned char) b ); }
bool operator> ( int a, const Zbyte& b )             { return ( (unsigned char) a > b.value ); }
bool operator> ( float a, const Zbyte& b )           { return ( (unsigned char) a > b.value ); }
bool operator> ( double a, const Zbyte& b )          { return ( (unsigned char) a > b.value ); }
bool operator< ( const Zbyte& a, const Zbyte& b )  { return ( a.value < b.value );    }
bool operator< ( const Zbyte& a, int b )             { return ( a.value < (unsigned char) b ); }
bool operator< ( const Zbyte& a, float b )           { return ( a.value < (unsigned char) b ); }
bool operator< ( const Zbyte& a, double b )          { return ( a.value < (unsigned char) b ); }
bool operator< ( int a, const Zbyte& b )             { return ( (unsigned char) a < b.value ); }
bool operator< ( float a, const Zbyte& b )           { return ( (unsigned char) a < b.value ); }
bool operator< ( double a, const Zbyte& b )          { return ( (unsigned char) a < b.value ); }

/// Zbyte ////////////////////////////////////////////////////////////////////////////////////////// end


/// Zushort ////////////////////////////////////////////////////////////////////////////////////////// start

unsigned short operator+ ( const Zushort& a, const Zushort& b ) { return a.value+b.value; }
unsigned short operator- ( const Zushort& a, const Zushort& b ) { return a.value-b.value; }
unsigned short operator* ( const Zushort& a, const Zushort& b ) { return a.value*b.value; }
unsigned short operator/ ( const Zushort& a, const Zushort& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}
unsigned short operator+ ( Zushort a, Zushort b ) { return a.value+b.value; }
unsigned short operator- ( Zushort a, Zushort b ) { return a.value-b.value; }
unsigned short operator* ( Zushort a, Zushort b ) { return a.value*b.value; }
unsigned short operator/ ( Zushort a, Zushort b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}

unsigned short operator+ ( const Zushort& a, unsigned char b ) { return a.value+b; }
unsigned short operator- ( const Zushort& a, unsigned char b ) { return a.value-b; }
unsigned short operator* ( const Zushort& a, unsigned char b ) { return a.value*b; }
unsigned short operator/ ( const Zushort& a, unsigned char b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b;
}
unsigned short operator+ ( unsigned char b, const Zushort& a ) { return b+a.value; }
unsigned short operator- ( unsigned char b, const Zushort& a ) { return b-a.value; }
unsigned short operator* ( unsigned char b, const Zushort& a ) { return b*a.value; }
unsigned short operator/ ( unsigned char b, const Zushort& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return b/a.value;
}

/// Zbyte and uint

// Casted int operations
unsigned short operator+ ( const Zushort& a, unsigned short b ) {
 return a.value+b;
}
unsigned short operator- ( const Zushort& a, unsigned short b ) {
 return a.value-b;
}
unsigned short operator* ( const Zushort& a, unsigned short b ) {
 return a.value*b;
}
unsigned short operator/ ( const Zushort& a, unsigned short b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b;
}
// Casted int operations reversed
unsigned short operator+ ( unsigned short b, const Zushort& a ) {
 return a.value+b;
}
unsigned short operator- ( unsigned short b, const Zushort& a ) {
 return b-a.value;
}
unsigned short operator* ( unsigned short b, const Zushort& a ) {
 return a.value*b;
}
unsigned short operator/ ( unsigned short b, const Zushort& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return b/a.value;
}

/// Zushort and int

// Casted int operations
unsigned short operator+ ( const Zushort& a, int b ) {
 return a.value+(unsigned short) b;
}
unsigned short operator- ( const Zushort& a, int b ) {
 return a.value-(unsigned short) b;
}
unsigned short operator* ( const Zushort& a, int b ) {
 return a.value*(unsigned short) b;
}
unsigned short operator/ ( const Zushort& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/(unsigned short) b;
}
// Casted int operations reversed
unsigned short operator+ ( int b, const Zushort& a ) {
 return a.value+(unsigned short) b;
}
unsigned short operator- ( int b, const Zushort& a ) {
 return (unsigned short) b-a.value;
}
unsigned short operator* ( int b, const Zushort& a ) {
 return a.value*(unsigned short) b;
}
unsigned short operator/ ( int b, const Zushort& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned short) b/a.value;
}

/// Zbyte and float

// Float operations (leans toward float precision)
unsigned short operator+ ( float b, const Zushort& a ) {
 return (unsigned short)((float)a.value+b);
}
unsigned short operator- ( float b, const Zushort& a ) {
 return (unsigned short)(b-(float)a.value);
}
unsigned short operator* ( float b, const Zushort& a ) {
 return (unsigned short)((float)a.value*b);
}
unsigned short operator/ ( float b, const Zushort& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned char)(b/(float)a.value);
}
// Float operations reversed (leans toward float precision)
unsigned short operator+ ( const Zushort& a, float b ) {
 return (unsigned short)((float)a.value+b);
}
unsigned short operator- ( const Zushort& a, float b ) {
 return (unsigned short)((float)a.value-b);
}
unsigned short operator* ( const Zushort& a, float b ) {
 return (unsigned short)((float)a.value*b);
}
unsigned short operator/ ( const Zushort& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif

 return (unsigned short)((float)a.value/b);
}

/// Zbyte and double

// Casted double operations (leans toward double precision)
unsigned short operator+ ( double b, const Zushort& a ) {
 return (unsigned short)((double)a.value+b);
}
unsigned short operator- ( double b, const Zushort& a ) {
 return (unsigned short)(b-(double)a.value);
}
unsigned short operator* ( double b, const Zushort& a ) {
 return (unsigned short)((double)a.value*b);
}
unsigned short operator/ ( double b, const Zushort& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zushort: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned short)(b/(double)a.value);
}
// Casted double operations reversed (leans toward double precision)
unsigned short operator+ ( const Zushort& a, double b ) {
 return (unsigned short)((double)a.value+b);
}
unsigned short operator- ( const Zushort& a, double b ) {
 return (unsigned short)((double)a.value-b);
}
unsigned short operator* ( const Zushort& a, double b ) {
 return (unsigned short)((double)a.value*b);
}
unsigned short operator/ ( const Zushort& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zbyte: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned short)((double)a.value/b);
}

/// Zushort ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zint ////////////////////////////////////////////////////////////////////////////////////////// start

int operator+ ( const Zint& a, const Zint& b ) { return a.value+b.value; }
int operator- ( const Zint& a, const Zint& b ) { return a.value-b.value; }
int operator* ( const Zint& a, const Zint& b ) { return a.value*b.value; }
int operator/ ( const Zint& a, const Zint& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}
/* Creates C2593 for some reason...
int operator+ ( Zint a, Zint b ) { return a.value+b.value; }
int operator- ( Zint a, Zint b ) { return a.value-b.value; }
int operator* ( Zint a, Zint b ) { return a.value*b.value; }
int operator/ ( Zint a, Zint b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}
*/

/// Zint and int

// Casted int operations
int operator+ ( const Zint& a, int b ) { return a.value+b; }
int operator- ( const Zint& a, int b ) { return a.value-b; }
int operator* ( const Zint& a, int b ) { return a.value*b; }
int operator/ ( const Zint& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b;
}
int operator% ( const Zint& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: modulo by zero in %% operation!\n");
#endif
  return 0;
 }
#endif
 return a.value%b;
}
// Casted int operations reversed
int operator+ ( int b, const Zint& a ) { return a.value+b; }
int operator- ( int b, const Zint& a ) { return b-a.value; }
int operator* ( int b, const Zint& a ) { return a.value*b; }
int operator/ ( int b, const Zint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return b/a.value;
}
int operator% ( int b, const Zint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: modulo by zero in %% operation!\n");
#endif
  return 0;
 }
#endif
 return b%a.value;
}

/// Zint and float

// Float operations (leans toward float precision)
int operator+ ( float b, const Zint& a ) { return (int)((float)a.value+b); }
int operator- ( float b, const Zint& a ) { return (int)(b-(float)a.value); }
int operator* ( float b, const Zint& a ) { return (int)((float)a.value*b); }
int operator/ ( float b, const Zint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (int)(b/(float)a.value);
}
// Float operations reversed (leans toward float precision)
int operator+ ( const Zint& a, float b ) { return (int)((float)a.value+b); }
int operator- ( const Zint& a, float b ) { return (int)((float)a.value-b); }
int operator* ( const Zint& a, float b ) { return (int)((float)a.value*b); }
int operator/ ( const Zint& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif

 return (int)((float)a.value/b);
}

/// Zint and double

// Casted double operations (leans toward double precision)
int operator+ ( double b, const Zint& a ) { return (int)((double)a.value+b); }
int operator- ( double b, const Zint& a ) { return (int)(b-(double)a.value); }
int operator* ( double b, const Zint& a ) { return (int)((double)a.value*b); }
int operator/ ( double b, const Zint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (int)(b/(double)a.value);
}
// Casted double operations reversed (leans toward double precision)
int operator+ ( const Zint& a, double b ) { return (int)((double)a.value+b); }
int operator- ( const Zint& a, double b ) { return (int)((double)a.value-b); }
int operator* ( const Zint& a, double b ) { return (int)((double)a.value*b); }
int operator/ ( const Zint& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (int)((double)a.value/b);
}

// Modulo

int operator% ( const Zint& b, const Zint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zint: modulo by zero in %% operation!\n");
#endif
  return 0;
 }
#endif
 return b.value%a.value;
}

// Zint boolean operations
bool operator>= ( const Zint& a, const Zint& b ) { return ( a.value >= b.value ); }
bool operator>= ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value >= b ); }
bool operator>= ( const Zint& a, int b )         { return ( a.value >= b );       }
bool operator>= ( const Zint& a, float b )       { return ( a.value >= (int) b ); }
bool operator>= ( const Zint& a, double b )      { return ( a.value >= (int) b ); }
bool operator>= ( unsigned int a, const Zint& b ) { return ( a >= (unsigned int) b.value ); }
bool operator>= ( int a, const Zint& b )         { return ( a >= b.value );       }
bool operator>= ( float a, const Zint& b )       { return ( (int) a >= b.value ); }
bool operator>= ( double a, const Zint& b )      { return ( (int) a >= b.value ); }
bool operator== ( const Zint& a, const Zbyte& b )   { return ((unsigned char) a.value == b.value ); }
bool operator== ( const Zint& a, const Zushort& b ) { return ((unsigned short) a.value == b.value ); }
bool operator== ( const Zint& a, const Zuint& b )   { return ((unsigned int) a.value == b.value ); }
bool operator== ( const Zint& a, const Zdouble& b ) { return ((double) a.value == b.value ); }
bool operator== ( const Zbyte& a, const Zint& b )   { return (a.value == (unsigned char) b.value ); }
bool operator== ( const Zushort& a, const Zint& b ) { return (a.value == (unsigned short) b.value ); }
bool operator== ( const Zuint& a, const Zint& b )   { return (a.value == (unsigned int) b.value ); }
bool operator== ( const Zdouble& a, const Zint& b ) { return (a.value == (double) b.value ); }
bool operator== ( const Zint& a, const Zint& b ) { return ( a.value == b.value ); }
bool operator== ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value >= b ); }
bool operator== ( const Zint& a, int b )         { return ( a.value == b );       }
bool operator== ( const Zint& a, float b )       { return ( a.value == (int) b ); }
bool operator== ( const Zint& a, double b )      { return ( a.value == (int) b ); }
bool operator== ( unsigned int a, const Zint& b ) { return ( a == (unsigned int) b.value ); }
bool operator== ( int a, const Zint& b )         { return ( a == b.value );       }
bool operator== ( float a, const Zint& b )       { return ( (int) a == b.value ); }
bool operator== ( double a, const Zint& b )      { return ( (int) a == b.value ); }
bool operator!= ( const Zint& a, const Zint& b ) { return ( a.value != b.value ); }
bool operator!= ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value != b ); }
bool operator!= ( const Zint& a, int b )         { return ( a.value != b );       }
bool operator!= ( const Zint& a, float b )       { return ( a.value != (int) b ); }
bool operator!= ( const Zint& a, double b )      { return ( a.value != (int) b ); }
bool operator!= ( unsigned int a, const Zint& b ) { return ( a != (unsigned int) b.value ); }
bool operator!= ( int a, const Zint& b )         { return ( a != b.value );       }
bool operator!= ( float a, const Zint& b )       { return ( (int) a != b.value ); }
bool operator!= ( double a, const Zint& b )      { return ( (int) a != b.value ); }
bool operator<= ( const Zint& a, const Zint& b ) { return ( a.value <= b.value ); }
bool operator<= ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value <= b ); }
bool operator<= ( const Zint& a, int b )         { return ( a.value <= b );       }
bool operator<= ( const Zint& a, float b )       { return ( a.value <= (int) b ); }
bool operator<= ( const Zint& a, double b )      { return ( a.value <= (int) b ); }
bool operator<= ( unsigned int a, const Zint& b ) { return ( a <= (unsigned int) b.value ); }
bool operator<= ( int a, const Zint& b )         { return ( a <= b.value );       }
bool operator<= ( float a, const Zint& b )       { return ( (int) a <= b.value ); }
bool operator<= ( double a, const Zint& b )      { return ( (int) a <= b.value ); }
bool operator> ( const Zint& a, const Zint& b )  { return ( a.value > b.value ); }
bool operator> ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value > b ); }
bool operator> ( const Zint& a, int b )          { return ( a.value > b );       }
bool operator> ( const Zint& a, float b )        { return ( a.value > (int) b ); }
bool operator> ( const Zint& a, double b )       { return ( a.value > (int) b ); }
bool operator> ( unsigned int a, const Zint& b ) { return ( a > (unsigned int) b.value ); }
bool operator> ( int a, const Zint& b )          { return ( a > b.value );       }
bool operator> ( float a, const Zint& b )        { return ( (int) a > b.value ); }
bool operator> ( double a, const Zint& b )       { return ( (int) a > b.value ); }
bool operator< ( const Zint& a, const Zint& b )  { return ( a.value < b.value ); }
bool operator< ( const Zint& a, unsigned int b ) { return ( (unsigned int) a.value < b ); }
bool operator< ( const Zint& a, int b )          { return ( a.value < b );       }
bool operator< ( const Zint& a, float b )        { return ( a.value < (int) b ); }
bool operator< ( const Zint& a, double b )       { return ( a.value < (int) b ); }
bool operator< ( unsigned int a, const Zint& b ) { return ( a < (unsigned int) b.value ); }
bool operator< ( int a, const Zint& b )          { return ( a < b.value );       }
bool operator< ( float a, const Zint& b )        { return ( (int) a < b.value ); }
bool operator< ( double a, const Zint& b )       { return ( (int) a < b.value ); }

/// Zint ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zuint ////////////////////////////////////////////////////////////////////////////////////////// start

unsigned int operator+ ( const Zuint& a, const Zuint& b ) {
 return a.value+b.value;
}
unsigned int operator- ( const Zuint& a, const Zuint& b ) {
 return a.value-b.value;
}
unsigned int operator* ( const Zuint& a, const Zuint& b ) {
 return a.value*b.value;
}
unsigned int operator/ ( const Zuint& a, const Zuint& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b.value;
}

/// Zuint and uint

// Casted int operations
unsigned int operator+ ( const Zuint& a, unsigned int b ) {
 return a.value+b;
}
unsigned int operator- ( const Zuint& a, unsigned int b ) {
 return a.value-b;
}
unsigned int operator* ( const Zuint& a, unsigned int b ) {
 return a.value*b;
}
unsigned int operator/ ( const Zuint& a, unsigned int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/b;
}
bool operator< ( unsigned int a, const Zuint& b ) { return ( a < b.value ); }
// Casted int operations reversed
unsigned int operator+ ( unsigned int b, const Zuint& a ) {
 return a.value+b;
}
unsigned int operator- ( unsigned int b, const Zuint& a ) {
 return b-a.value;
}
unsigned int operator* ( unsigned int b, const Zuint& a ) {
 return a.value*b;
}
unsigned int operator/ ( unsigned int b, const Zuint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return b/a.value;
}

/// Zuint and int

// Casted int operations
unsigned int operator+ ( const Zuint& a, int b ) {
 return a.value+(unsigned int) b;
}
unsigned int operator- ( const Zuint& a, int b ) {
 return a.value-(unsigned int) b;
}
unsigned int operator* ( const Zuint& a, int b ) {
 return a.value*(unsigned int) b;
}
unsigned int operator/ ( const Zuint& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return a.value/(unsigned int) b;
}
// Casted int operations reversed
unsigned int operator+ ( int b, const Zuint& a ) {
 return a.value+(unsigned int) b;
}
unsigned int operator- ( int b, const Zuint& a ) {
 return (unsigned int) b-a.value;
}
unsigned int operator* ( int b, const Zuint& a ) {
 return a.value*(unsigned int) b;
}
unsigned int operator/ ( int b, const Zuint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned int) b/a.value;
}

/// Zuint and float

// Float operations (leans toward float precision)
unsigned int operator+ ( float b, const Zuint& a ) {
 return (unsigned int)((float)a.value+b);
}
unsigned int operator- ( float b, const Zuint& a ) {
 return (unsigned int)(b-(float)a.value);
}
unsigned int operator* ( float b, const Zuint& a ) {
 return (unsigned int)((float)a.value*b);
}
unsigned int operator/ ( float b, const Zuint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned int)(b/(float)a.value);
}
// Float operations reversed (leans toward float precision)
unsigned int operator+ ( const Zuint& a, float b ) {
 return (unsigned int)((float)a.value+b);
}
unsigned int operator- ( const Zuint& a, float b ) {
 return (unsigned int)((float)a.value-b);
}
unsigned int operator* ( const Zuint& a, float b ) {
 return (unsigned int)((float)a.value*b);
}
unsigned int operator/ ( const Zuint& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif

 return (unsigned int)((float)a.value/b);
}

/// Zuint and double

// Casted double operations (leans toward double precision)
unsigned int operator+ ( double b, const Zuint& a ) {
 return (unsigned int)((double)a.value+b);
}
unsigned int operator- ( double b, const Zuint& a ) {
 return (unsigned int)(b-(double)a.value);
}
unsigned int operator* ( double b, const Zuint& a ) {
 return (unsigned int)((double)a.value*b);
}
unsigned int operator/ ( double b, const Zuint& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned int)(b/(double)a.value);
}
// Casted double operations reversed (leans toward double precision)
unsigned int operator+ ( const Zuint& a, double b ) {
 return (unsigned int)((double)a.value+b);
}
unsigned int operator- ( const Zuint& a, double b ) {
 return (unsigned int)((double)a.value-b);
}
unsigned int operator* ( const Zuint& a, double b ) {
 return (unsigned int)((double)a.value*b);
}
unsigned int operator/ ( const Zuint& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zuint: divide by zero in / operation!\n");
#endif
  return 0;
 }
#endif
 return (unsigned int)((double)a.value/b);
}

/// Zuint ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zdouble ////////////////////////////////////////////////////////////////////////////////////////// start

// Mixed type operations
double operator+ ( const Zdouble& a, const Zfloat& b ) { return a.value + (double) b.value; }
double operator+ ( const Zfloat& a, const Zdouble& b ) { return (double) a.value + b.value; }
double operator+ ( const Zdouble& a, const Zint& b )   { return a.value + (double) b.value; }
double operator+ ( const Zint& a, const Zdouble& b )   { return (double) a.value + b.value; }
double operator* ( const Zdouble& a, const Zfloat& b ) { return a.value * (double) b.value; }
double operator* ( const Zfloat& a, const Zdouble& b ) { return (double) a.value * b.value; }
double operator* ( const Zdouble& a, const Zint& b )   { return a.value * (double) b.value; }
double operator* ( const Zint& a, const Zdouble& b )   { return (double) a.value * b.value; }
// Zdouble and itself
double operator+ ( const Zdouble& a, const Zdouble& b ) { return a.value+b.value; }
double operator- ( const Zdouble& a, const Zdouble& b ) { return a.value-b.value; }
double operator* ( const Zdouble& a, const Zdouble& b ) { return a.value*b.value; }
double operator/ ( const Zdouble& a, const Zdouble& b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/b.value;
}
/* Creates C2593 for some reason...
double operator+ ( Zdouble a, Zdouble b ) { return a.value+b.value; }
double operator- ( Zdouble a, Zdouble b ) { return a.value-b.value; }
double operator* ( Zdouble a, Zdouble b ) { return a.value*b.value; }
double operator/ ( Zdouble a, Zdouble b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b.value == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0f;
 }
#endif
 return a.value/b.value;
}
*/

/// Zdouble and int

// Casted int operations (leans toward double precision)
double operator+ ( const Zdouble& a, int b ) { return a.value+(double)b; }
double operator- ( const Zdouble& a, int b ) { return a.value-(double)b; }
double operator* ( const Zdouble& a, int b ) { return a.value*(double)b; }
double operator/ ( const Zdouble& a, int b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 return a.value/(double)b;
}
// Casted int operations reversed (leans toward double precision)
double operator+ ( int b, const Zdouble& a ) { return a.value+(double)b; }
double operator- ( int b, const Zdouble& a ) { return (double)b-a.value; }
double operator* ( int b, const Zdouble& a ) { return a.value*(double)b; }
double operator/ ( int b, const Zdouble& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 return (double)b/a.value;
}

/// Zdouble and float

// Float operations (leans toward float precision)
double operator+ ( float b, const Zdouble& a ) { return a.value+(double)b; }
double operator- ( float b, const Zdouble& a ) { return (double)b-a.value; }
double operator* ( float b, const Zdouble& a ) { return a.value*(double)b; }
double operator/ ( float b, const Zdouble& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 return (double)b/a.value;
}
// Float operations reversed (leans toward float precision)
double operator+ ( const Zdouble& a, float b ) { return a.value+(double)b; }
double operator- ( const Zdouble& a, float b ) { return a.value-(double)b; }
double operator* ( const Zdouble& a, float b ) { return a.value*(double)b; }
double operator/ ( const Zdouble& a, float b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0f ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 
 return a.value/(double)b;
}

/// Zdouble and double

// Casted double operations
double operator+ ( double b, const Zdouble& a ) { return a.value+b; }
double operator- ( double b, const Zdouble& a ) { return b-a.value; }
double operator* ( double b, const Zdouble& a ) { return a.value*b; }
double operator/ ( double b, const Zdouble& a ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( a.value == 0.0) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 return b/a.value;
}
// Casted double operations reversed
double operator+ ( const Zdouble& a, double b ) { return a.value+b; }
double operator- ( const Zdouble& a, double b ) { return a.value-b; }
double operator* ( const Zdouble& a, double b ) { return a.value*b; }
double operator/ ( const Zdouble& a, double b ) {
#if defined(DIV_BY_ZERO_PROTECTION)
 if ( b == 0.0 ) {
#if defined(DIV_BY_ZERO_REPORTING)
  OUTPUT("Zdouble: divide by zero in / operation!\n");
#endif
  return 0.0;
 }
#endif
 return a.value/b;
}

// Boolean operations
bool operator>= ( const Zdouble& a, const Zdouble& b ) { return ( a.value >= b.value );    }
bool operator>= ( const Zdouble& a, int b )            { return ( a.value >= (double) b ); }
bool operator>= ( const Zdouble& a, float b )          { return ( a.value >= (double) b ); }
bool operator>= ( const Zdouble& a, double b )         { return ( a.value >= b );          }
bool operator>= ( int a, const Zdouble& b )            { return ( (double) a >= b.value ); }
bool operator>= ( float a, const Zdouble& b )          { return ( (double) a >= b.value ); }
bool operator>= ( double a, const Zdouble& b )         { return ( a >= b.value );          }
bool operator== ( const Zdouble& a, const Zdouble& b ) { return ( a.value == b.value );    }
bool operator== ( const Zdouble& a, int b )            { return ( a.value == (double) b ); }
bool operator== ( const Zdouble& a, float b )          { return ( a.value == (double) b ); }
bool operator== ( const Zdouble& a, double b )         { return ( a.value == b );          }
bool operator== ( int a, const Zdouble& b )            { return ( (double) a == b.value ); }
bool operator== ( float a, const Zdouble& b )          { return ( (double) a == b.value ); }
bool operator== ( double a, const Zdouble& b )         { return ( a == b.value );          }
bool operator!= ( const Zdouble& a, const Zdouble& b ) { return ( a.value != b.value );    }
bool operator!= ( const Zdouble& a, int b )            { return ( a.value != (double) b ); }
bool operator!= ( const Zdouble& a, float b )          { return ( a.value != (double) b ); }
bool operator!= ( const Zdouble& a, double b )         { return ( a.value != b );          }
bool operator!= ( int a, const Zdouble& b )            { return ( (double) a != b.value ); }
bool operator!= ( float a, const Zdouble& b )          { return ( (double) a != b.value ); }
bool operator!= ( double a, const Zdouble& b )         { return ( a != b.value );          }
bool operator<= ( const Zdouble& a, const Zdouble& b ) { return ( a.value <= b.value );    }
bool operator<= ( const Zdouble& a, int b )            { return ( a.value <= (double) b ); }
bool operator<= ( const Zdouble& a, float b )          { return ( a.value <= (double) b ); }
bool operator<= ( const Zdouble& a, double b )         { return ( a.value <= b );          }
bool operator<= ( int a, const Zdouble& b )            { return ( (double) a <= b.value ); }
bool operator<= ( float a, const Zdouble& b )          { return ( (double) a <= b.value ); }
bool operator<= ( double a, const Zdouble& b )         { return ( a <= b.value );          }
bool operator> ( const Zdouble& a, const Zdouble& b )  { return ( a.value > b.value );    }
bool operator> ( const Zdouble& a, int b )             { return ( a.value > (double) b ); }
bool operator> ( const Zdouble& a, float b )           { return ( a.value > (double) b ); }
bool operator> ( const Zdouble& a, double b )          { return ( a.value > b );          }
bool operator> ( int a, const Zdouble& b )             { return ( (double) a > b.value ); }
bool operator> ( float a, const Zdouble& b )           { return ( (double) a > b.value ); }
bool operator> ( double a, const Zdouble& b )          { return ( a > b.value );          }
bool operator< ( const Zdouble& a, const Zdouble& b )  { return ( a.value < b.value );    }
bool operator< ( const Zdouble& a, int b )             { return ( a.value < (double) b ); }
bool operator< ( const Zdouble& a, float b )           { return ( a.value < (double) b ); }
bool operator< ( const Zdouble& a, double b )          { return ( a.value < b );          }
bool operator< ( int a, const Zdouble& b )             { return ( (double) a < b.value ); }
bool operator< ( float a, const Zdouble& b )           { return ( (double) a < b.value ); }
bool operator< ( double a, const Zdouble& b )          { return ( a < b.value );          }

/// Zdouble ////////////////////////////////////////////////////////////////////////////////////////// end

/// Zstring ////////////////////////////////////////////////////////////////////////////////////////// end

// Removes all newlines, limits all spaces to 1
string Zstring::Words() {
 Zstring out;
 out=value;
 out("\n"," ");
 out("\t"," ");
 while ( out.Contains("  ") ) out("  "," ");
 const char *p=value.c_str();
 while ( *p != '\0' ) {
  p++;
  string word;
  p=string_argument_case(p,&word);
  if ( word.length() > 0 ) {
   if ( out.length > 0 ) {
    result+=" ";
   }
   result+=word;
  }
 }
 return out.value;
}

// Limits to a length or less without breaking words ex: LimitTo(dog cat fish ~ 6) = dog
#include "Strings.h"
string Zstring::LimitTo( int len ) {
 Strings words;
 words.SplitPush(Words().c_str(),' ');
 Zstring out;
 EACH(words.first,String,s) {
  if ( (int)out.length + (int)s->s.length() > len ) break;
  out+=s->s;
 }
 return out.value;
}

string Zstring::WordWrap( int linelen ) {
 Strings words;
 words.SplitPush(Words().c_str(),' ');
 Zstring out;
 Zint thislinelen;
 EACH(words.first,String,s) {
  int wordlen=(int)s->s.length();
  if ( (int)thislinelen + 1 + wordlen > linelen ) {
   out+="\n";
   out+=s->s+string(" ");
   thislinelen=(int)s->s.length();
  } else {
   out+=s->s+string(" ");
   thislinelen+=wordlen+1;
  }
 }
 return out.value;
}

string Zstring::WordWrapNewlines( int linelen, const char *forcednewline ) {
 Strings words;
 words.SplitPush(Words().c_str(),' ');
 Zstring out;
 Zint thislinelen;
 EACH(words.first,String,s) {
  int wordlen=(int)s->s.length();
  if ( (int)thislinelen + 1 + wordlen > linelen ) {
   out+="\n";
   out+=s->s+string(" ");
   thislinelen=(int)s->s.length();
  } else {
   out+=s->s+string(" ");
   thislinelen+=wordlen+1;
  }
 }
 out(forcednewline,"\n");
 return out.value;
}
bool Zstring:: is_inside( const char * cloud ) {
 Zstring c(cloud);
 Strings it;
 it.SplitPush(cloud);
 Strings us;
 us.SplitPush(this->c_str());
 return it.includes(&us);
}

bool operator== ( const Zstring& a, const Zstring& b ) {
 bool result= !str_cmp(a.value.c_str(),b.value.c_str());
 if ( !result ) {
  if ( is_decimal(a.value.c_str()) && is_decimal(b.value.c_str()) ) {
   double A=atof(a.value.c_str());
   double B=atof(b.value.c_str());
   return ( A == B );
  }
 }
 return result;
}

bool operator== ( const Zstring& a, const char * b ) {
 return !str_cmp(a.value.c_str(),b);
}

bool operator== ( const char * b, const Zstring& a ) {
 return !str_cmp(a.value.c_str(),b);
}

string operator+ ( const Zstring& a, const Zstring& b ) {
 return a.value+b.value;
}

string operator+ ( const Zstring& a, const string& b ) {
 return a.value+b;
}

string operator+ ( const string& a, const Zstring& b ) {
 return a+b.value;
}

string operator+ ( const Zstring& a, const int& b ) {
 return a.value+FORMAT(tbuf,128,"%d",(int)b);
}

string operator+ ( const Zstring& a, const unsigned int& b ) {
 return a.value+FORMAT(tbuf,128,"%d",(int)b);
}

string operator+ ( const Zstring& a, const float& b ) {
 return a.value+FORMAT(tbuf,128,"%f",(float)b);
}

string operator+ ( const Zstring& a, const double& b ) {
 return a.value+FORMAT(tbuf,128,"%f",(double)b);
}

string operator+ ( const int& a, const Zstring& b ) {
 return b.value+FORMAT(tbuf,128,"%d",(int)a);
}

string operator+ ( const unsigned int& a, const Zstring& b ) {
 return b.value+FORMAT(tbuf,128,"%d",(int)a);
}

string operator+ ( const float& a, const Zstring& b ) {
 return b.value+FORMAT(tbuf,128,"%f",(float)a);
}

string operator+ ( const double& a, const Zstring& b ) {
 return b.value+FORMAT(tbuf,128,"%f",(double)a);
}

string operator+ ( string a, Zint& b )    { return a+b.toString(); }
string operator+ ( string a, Zuint& b )   { return a+b.toString(); }
string operator+ ( string a, Zfloat& b )  { return a+b.toString(); }
string operator+ ( string a, Zdouble& b ) { return a+b.toString(); }
/* Creates C2593
string operator+ ( Zint& a, string b )    { return a.toString()+b; }
string operator+ ( Zuint& a, string b )   { return a.toString()+b; }
string operator+ ( Zfloat& a, string b )  { return a.toString()+b; }
string operator+ ( Zdouble& a, string b ) { return a.toString()+b; }
*/


int operator+ ( Zint& a, Zint& b ) { return a.value+b.value; }
float operator+ ( Zfloat& a, Zfloat& b ) { return a.value+b.value; }
double operator+ ( Zdouble& a, Zdouble& b ) { return a.value+b.value; }

string operator+ ( const Zstring& a, char c ) {
 return a.value+c;
}

string operator+ ( char c, const Zstring& a ) {
 return a.value+c;
}

string operator- ( const Zstring& a, const Zstring& b ) {
 string result=a.value;
 replaceOnce(result,b.value,string(""));
 return result;
}

string operator- ( const Zstring& a, const string& b ) {
 string result=a.value;
 replaceOnce(result,b,string(""));
 return result;
}

string operator- ( const string& b, const Zstring& a ) {
 string result=b;
 replaceOnce(result,a.value,string(""));
 return result;
}

string operator- ( const Zstring& a, const char *b ) {
 string result=a.value;
 replaceOnce(result,string(b),string(""));
 return result;
}

/*
string operator- ( const char *b, const Zstring& a ) {
 string result=string(b);
 replaceOnce(result,a.value,string(""));
 return result;
}*/

string operator/ ( const Zstring& a, const Zstring& b ) {
 string result=a.value;
 replaceAll(result,b.value,string(""));
 return result;
}

string operator/ ( const Zstring& a, const string& b ) {
 string result=a.value;
 replaceAll(result,b,string(""));
 return result;
}

string operator/ ( const string& b, const Zstring& a ) {
 string result=b;
 replaceAll(result,a.value,string(""));
 return result;
}

string operator/ ( const Zstring& a, const char *b ) {
 string result=a.value;
 replaceAll(result,string(b),string(""));
 return result;
}

string operator/ ( const char *b, const Zstring& a ) {
 string result=string(b);
 replaceAll(result,a.value,string(""));
 return result;
}

string operator* ( const Zstring& a, unsigned int i ) {
 string temp=string("");
 for ( unsigned int j=0; j<i; j++ ) {
  temp+=a.value;
 }
 return temp;
}

int operator* ( const Zstring& a, string b ) {
 if (a.value.length() == 0) return 0;
 int count = 0;
 for (
  size_t offset = a.value.find(b);
  offset != std::string::npos;
	 offset = a.value.find(b, offset + b.length())
   ) ++count;
 return count;
}

int operator* ( string a, const Zstring& b ) {
 if (a.length() == 0) return 0;
 int count = 0;
 for (
  size_t offset = a.find(b.value);
  offset != std::string::npos;
	 offset = a.find(b.value, offset + b.value.length())
   ) ++count;
 return count;
}

int operator* ( const Zstring& a, const char *B ) {
 string b=string(B);
 if (a.value.length() == 0) return 0;
 int count = 0;
 for (
  size_t offset = a.value.find(b);
  offset != std::string::npos;
	 offset = a.value.find(b, offset + b.length())
   ) ++count;
 return count;
}

int operator* ( const Zstring& a, const Zstring& b ) {
 if (a.value.length() == 0) return 0;
 int count = 0;
 for (
  size_t offset = a.value.find(b.value);
  offset != std::string::npos;
	 offset = a.value.find(b.value, offset + b.value.length())
   ) ++count;
 return count;
}

/// Zstring ////////////////////////////////////////////////////////////////////////////////////////// end

// Test cases

void ZeroTypesTest() {
 OUTPUT("Zfloat test (Ftest) :::::::::::::::::::::::::::::::::::::::::::::::: \n");
 Zfloat Ftest;
 OUTPUT("Ftest = %1.9f (initialized value)\n", Ftest);
 OUTPUT("Ftest*5 = %1.9f\n", Ftest*5);
 Ftest=12378;
 OUTPUT("Ftest := 12378 = %1.9f\n", Ftest*5);
 Ftest=12378.0f;
 OUTPUT("Ftest := 12378.0f = %1.9f\n", Ftest*5);
 Ftest=12378.0;
 OUTPUT("Ftest := 12378.0 = %1.9f\n", Ftest*5);
 Ftest=12345.0/12345.0f;
 OUTPUT("Ftest := 12345.0/12345.0f = %1.9f\n", Ftest*5);
 OUTPUT("Ftest*5+123 = %1.9f\n", Ftest*5+123);
 OUTPUT("0/Ftest = %1.9f\n", 0/Ftest);
 OUTPUT("12345+Ftest/0 = %1.9f\n", 12345+Ftest/0);
 OUTPUT("777+Ftest*66.0f = %1.9f\n", 777+Ftest*66.0f);
 OUTPUT("Ftest/0    = %1.9f\n", Ftest/0);
 OUTPUT("Ftest/0.0f = %1.9f\n", Ftest/0.0f);
 OUTPUT("Ftest/0.0  = %1.9f\n", Ftest/0.0);
// OUTPUT("(double)Ftest/0.0 = %1.9f\n", (double)Ftest/0.0);
 OUTPUT("Ftest*0.12345f = %1.9f\n", Ftest*0.12345f);
 OUTPUT("Ftest/0.12345f = %1.9f\n", Ftest/0.12345f);
 OUTPUT("Ftest/12345 = %1.9f\n", Ftest/12345);

 OUTPUT("Zint test (Itest) :::::::::::::::::::::::::::::::::::::::::::::::: \n");
 Zint Itest;
 OUTPUT("Itest = %d (initialized value)\n", Itest);
 OUTPUT("Itest*5 = %d\n", Itest*5);
 Itest=12378;
 OUTPUT("Itest := 12378 = %d\n", Itest*5);
 Itest=12378.0f;
 OUTPUT("Itest := 12378.0f = %d\n", Itest*5);
 Itest=12378.0;
 OUTPUT("Itest := 12378.0 = %d\n", Itest*5);
 Itest=12345.0/12345.0f;
 OUTPUT("Itest := 12345.0/12345.0f = %d\n", Itest*5);
 OUTPUT("Itest*5+123 = %d\n", Itest*5+123);
 OUTPUT("0/Itest = %d\n", 0/Itest);
 OUTPUT("12345+Itest/0 = %d\n", 12345+Itest/0);
 OUTPUT("777+Itest*66.0f = %d\n", 777+Itest*66.0f);
 OUTPUT("Itest/0    = %d\n", Itest/0);
 OUTPUT("Itest/0.0f = %d\n", Itest/0.0f);
 OUTPUT("Itest/0.0  = %d\n", Itest/0.0);
// OUTPUT("(double)Itest/0.0 = %d\n", (double)Itest/0.0);
 OUTPUT("Itest*0.12345f = %d\n", Itest*0.12345f);
 OUTPUT("Itest/0.12345f = %d\n", Itest/0.12345f);
 OUTPUT("Itest/12345 = %d\n", Itest/12345);
 
 OUTPUT("Zdouble test (Dtest) :::::::::::::::::::::::::::::::::::::::::::::::: \n");
 Zdouble Dtest;
 OUTPUT("Dtest = %1.9f (initialized value)\n", Dtest);
 OUTPUT("Dtest*5 = %1.9f\n", Dtest*5);
 Dtest=12378;
 OUTPUT("Dtest := 12378 = %1.9f\n", Dtest*5);
 Dtest=12378.0f;
 OUTPUT("Dtest := 12378.0f = %1.9f\n", Dtest*5);
 Dtest=12378.0;
 OUTPUT("Dtest := 12378.0 = %1.9f\n", Dtest*5);
 Dtest=12345.0/12345.0f;
 OUTPUT("Dtest := 12345.0/12345.0f = %1.9f\n", Dtest*5);
 OUTPUT("Dtest*5+123 = %1.9f\n", Dtest*5+123);
 OUTPUT("0/Dtest = %1.9f\n", 0/Dtest);
 OUTPUT("12345+Dtest/0 = %1.9f\n", 12345+Dtest/0);
 OUTPUT("777+Dtest*66.0f = %1.9f\n", 777+Dtest*66.0f);
 OUTPUT("Dtest/0    = %1.9f\n", Dtest/0);
 OUTPUT("Dtest/0.0f = %1.9f\n", Dtest/0.0f);
 OUTPUT("Dtest/0.0  = %1.9f\n", Dtest/0.0);
// OUTPUT("(double)Dtest/0.0 = %1.2f\n", (double)Dtest/0.0);
 OUTPUT("Dtest*0.12345f = %1.9f\n", Dtest*0.12345f);
 OUTPUT("Dtest/0.12345f = %1.9f\n", Dtest/0.12345f);
 OUTPUT("Dtest/12345 = %1.9f\n", Dtest/12345);

 OUTPUT("::::::::::::::::::::: Initialization, Assigning between Z-types:\n");
 Zint a;
 a=2;
 Zfloat b;
 b=2.49f;
 Zdouble c;
 c=2.2;
 Zint Itest2;
 Itest2=b;
 OUTPUT("Zint Itest2=b (a Zfloat of %1.9f) = %d\n", b, Itest2);
 b=7090.5f;
 Itest2=b;
 OUTPUT("Zint Itest2=b (a Zfloat of %1.9f) = %d\n", b, Itest2);
 Zint a2;
 a2(a);
 OUTPUT("Zint a2; a2(a); a=%d a2=%d\n", a,a2);
 Zfloat f2(751.0f);
 OUTPUT("Zfloat f2(751.0f); f2=%1.9f\n", f2);

 OUTPUT("::::::::::::::::::::: Comparsion (==) tests:\n");
 Zint i;
 i=5;
 Zfloat f;
 f=5.0f;
 Zdouble d;
 d=5.0;
 OUTPUT("Zint(%d) == Zfloat(%1.9f) is: %s\n",       i.value,f.value, i==f ? "equal" : "unequal" );
 OUTPUT("Zdouble(%1.9f) == Zfloat(%1.9f) is: %s\n", d.value,f.value, d==f ? "equal" : "unequal" );
 OUTPUT("Zint(%d) == Zdouble(%1.9f) is: %s\n",      i.value,d.value, i==d ? "equal" : "unequal" );
 f=5.1f;
 d=5.1;
 OUTPUT("Zint(%d) == Zfloat(%1.9f) is: %s\n",                     i.value,f.value, i==f ? "equal" : "unequal" );
 OUTPUT("Zint(%d) == Zdouble(%1.9f) is: %s\n",                    i.value,d.value, i==d ? "equal" : "unequal" );
 OUTPUT("Zfloat(%1.9f) == Zdouble(%1.9f) is: %s (C2593 issue)\n", f.value,d.value, f==d ? "equal" : "unequal" );
 OUTPUT("Zfloat(%1.9f) == (float)Zdouble(%1.9f)=%1.9f is: %s\n",  f.value,d.value,(float)d, f==(float)d ? "equal" : "unequal" );

 OUTPUT("::::::::::::::::::::::: Zstring tests:\n" );
 Zstring Stest;
 OUTPUT("Initial value: Stest contains `%s` (casted to const char *) (%d characters long)\n", (const char*) Stest, Stest.length );
 Stest=string("testing");
 OUTPUT("Assigned new std:string 'testing' results in `%s` (casted to const char *) length %d\n", (const char*)Stest, Stest.length );
 Stest=string("   32");
 OUTPUT("Assigned Stest=`%s`, casted to int: Stest=%d\n", (const char*)Stest, (int)Stest);
 Stest=string("   123.321      ");
 OUTPUT("Assigned Stest=`%s`, casted to float: Stest=%1.9f\n", (const char*)Stest, (float)Stest);
 Stest=string("   1234.4321");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Stest=string("   1234.4321x");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Stest=string("   $1234.21");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Stest=string("   1,234.21");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Stest=string("   $1,234,545.21");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Stest=string("   1,2,3,4");
 OUTPUT("Assigned Stest=`%s`, casted to double: Stest=%1.9f\n", (const char*)Stest, (double)Stest);
 Zstring money("$1234.21");
 money/="$";
 OUTPUT("Assigned Zstring money to `$1234.21`, then money/=`$`, result was `%s`, and cast to double %1.9f\n",
  (const char *)money, (double) money);
 Zstring aa_bb_cc_dd("aa_bb_cc_dd");
 OUTPUT("Assigned a Zstring to `%s`, subtracted 'bb' result was `%s`,\n divided by 'c', result was `%s'\n subtracted by '_d', result was `%s`\n",
  (const char *)aa_bb_cc_dd,
  (aa_bb_cc_dd-"bb").c_str(),
  (aa_bb_cc_dd/"c").c_str(),
  (aa_bb_cc_dd-"_d").c_str()
  );
 Zstring as("hello howdy hi hello hi howdy hello"),bs("hello");
 OUTPUT("Assigned Zstring A to `%s`, B to `%s`,\n divided A by B result was `%s`,\n A-B was `%s`\n",
  (const char *)as, (const char *)bs, (as/bs).c_str(), (as-bs).c_str() );
 Zstring spre("super"), spost("man");
 OUTPUT("Assigned Zstring A to `%s` and B to `%s`, concatenated with A+B and result was `%s`\n",
  (const char *)spre, (const char *)spost, (spre+spost).c_str() );

 OUTPUT("::::::::::::::::::::::::::::::::::: Zpointer (Pvoid) tests:\n");
 Pvoid v;
 int x;
 OUTPUT("%p initial value\n", v.pointer);
 v((void *) &x);
 OUTPUT("int x memory location: %p\nv((void *)&x) value is now %p\n", &x, v.pointer);
 v=((void *) &x);
 OUTPUT("int x memory location: %p\nv((void *)&x) value is now %p\n", &x, v.pointer);
 Pvoid w((void *) &x);
 OUTPUT("int x memory location: %p\nZvoid w((void *)&x) initial value is now %p\n", &x, v.pointer);
// v=((void *)&x);

 exit(1);
}