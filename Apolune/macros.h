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
#include "GarbageCollector.h"
#include <float.h>
#include <vector>
using namespace std;

#include "string_functions.h"

#define and &&
#define or ||
#define not !
#define equals ==

#define XOR(a,b)  ( ((a) && !(b)) || ((b) && !(a)) )
#define LOOP(v,limit,c)  for ( int v=0; v<limit; v++ ) {c};

#define NEW(v,t) try { v= new t; } catch (bad_alloc& ba) { OUTPUT("NEW: bad_alloc caught: %s\n", ba.what()); v=null; }
#define CREATE(v,t) t *v=null; try { v= new t; } catch (bad_alloc& ba) { OUTPUT("NEW: bad_alloc caught: %s\n", ba.what()); v=null; }

#define AS(t,val,v) t*v=(t*)(val);
#define WITH(t,v,val,code) { t *v = val; {code} }

#define WHEN(conditional,code) if ( (conditional) ) {code}

#define UNLESS(A) ( A )
#define NEITHER(A,B) ( !A && !B )
#define BOTH(A,B)  ( A && B )

#define ABS(A)  ((A) < 0 ? -(A) : (A))

#define TOJSON string toJSON() 
#define TOJSON_(code) TOJSON {code}
// Used as first param of FORMAT, can be stacked.
#define JSONTREE(k) "\"" k "\":{\n%s},\n"
// Lacks last comma
#define JSONTREE_(k) "\"" k "\":{%s}\n"
// Used as first param of FORMAT, can be stacked.
#define JSONARRAY(k) "\"" k "\":[\n%s],\n"
// Lacks last comma
#define JSONARRAY_(k) "\"" k "\":[\n%s]\n"
// Used as first param of FORMAT, can be stacked.
#define JSONSTR(k) "\"" k "\":\"%s\",\n"
// Lacks last comma
#define JSONSTR_(k) "\"" k "\":\"%s\"\n"
// Used as first param of FORMAT, can be stacked.
#define JSONINT(k) "\"" k "\":\"%d\",\n"
// Lacks last comma
#define JSONINT_(k) "\"" k "\":\"%d\"\n"
// Used as first param of FORMAT, can be stacked.
#define JSONCHAR(k) "\"" k "\":\"%c\",\n"
// Lacks last comma
#define JSONCHAR_(k) "\"" k "\":\"%c\"\n"
// Used as first param of FORMAT, can be stacked.
#define JSONDECIMAL(k) "\"" k "\":\"%1.8f\",\n"
// Lacks last comma
#define JSONDECIMAL_(k) "\"" k "\":\"%1.8f\"\n"
// Used as first param of FORMAT, can be stacked.
#define JSONBOOL(k) "\"" k "\":%s,\n"
// Lacks last comma
#define JSONBOOL_(k) "\"" k "\":%s\n"
// Used in values list (varargs of FORMAT)
#define JSONTF(v) (v?"true":"false")
// JSON array item, used in MANY(..) toJSON()
#define JSONITEMS(single) { FOREACH(single,s) out+=string("{\n")+s->toJSON()+(s->next?string("},\n"):string("}\n")); }
// MANYTOJSON used in MANY() ... DONE() clauses
#define MANYTOJSON(single) string toJSON() { Zstring out; JSONITEMS(single) return out.value; }
// Used in ZIndexed
#define JSONINDEXED(target) string toJSON() { Zstring out; for ( int i=0; i<(int)target.length; i++ ) out+=string("[")+target[i].toJSON()+string("]")+((i!=target.length-1)?string(","):string("")); return out.value; }
// Used in ZIndexed
#define JSONINDEXEDFUN(fun_name,target) string fun_name() { Zstring out; for ( int i=0; i<(int)target.length; i++ ) out+=string("[")+target[i].toJSON()+string("]")+((i!=target.length-1)?string(","):string("")); return out.value; }
// Used in ZIndexed (of pointers or disposables)
#define JSONINDEXEDP(target) string toJSON() { Zstring out; for ( int i=0; i<(int)target.length; i++ ) if ( target[i] ) out+=string("[")+target[i]->toJSON()+string("]")+((i!=target.length-1)?string(","):string("")); return out.value; }
// Used in ZIndexed (of pointers or disposables)
#define JSONINDEXEDFUNP(fun_name,target) string fun_name() { Zstring out; for ( int i=0; i<(int)target.length; i++ ) if ( target[i] ) out+=string("[")+target[i]->toJSON()+string("]")+((i!=target.length-1)?string(","):string("")); return out.value; }
// Used in ZIndexed
#define JSONINDEXEDNUMBER(fun_name,target) string fun_name() { Zstring out; for ( int i=0; i<(int)target.length; i++ ) out+=string("[")+std::to_string(target[i])+string("]")+((i!=target.length-1)?string(","):string("")); return out.value; }

// having, true-or-false (diminished meaning of bool) parameter specifiers...
// Used to mark a parameter being presented a bool as a signal rather than a parameter-as-tested-value signal.  (See implementation...)
typedef bool trueorfalse;
typedef bool having; // symbol name idea came after reading about /using/ C++ keyword on http://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11
// Sometimes the implementation uses the presence of a bool typed parameter to allow for a quick variant of a function/method of same name
// Example can be seen in GridSplitter where presence of "columns_instead_of_rows" parameter activates the feature whether presented with true or false.
// This is a slight brainly optimization that adds functionality where it was already present, and the typedef(s) above simplify programmer interface by
// embedding the nuance right in the code.  Most compilers would treat this as an alias, so bool and Zbool can still be provided when cast,
// but if you see /having/, passing it a bool variable isn't meaningful, you must test its value if it is to mean something.  The alternative
// would be to collude the code with cryptic numbers like "Concatenate2..." ... which is also done in some places, but is avoided where-ever possible.
// Prior to creating these typedefs, the implementation did not specify when true or false would lead to the feature being activated at the parameter level.

template<typename T>
struct matrix2d {
public:
 T outOfBounds;
 matrix2d(size_t m=0, size_t n=0) : m(m), n(n), vs(m*n) {}
 T& operator ()(unsigned i, unsigned j, bool boundaryCheck ) {
  if ( boundaryCheck && i<m && j<n ) return vs[i + m * j];
  OUTPUT("matrix2d::(%d,%d) failed boundary check\n",(int)i,(int)j);
  return outOfBounds;
 }
 T& operator ()(unsigned k) { return vs[k]; }
 T& operator ()(unsigned i, unsigned j) { return vs[i + m * j]; }
 T& index( unsigned k ) { return vs.at(k); }
 T& index( unsigned i, unsigned j ) { return vs.at(i+m*j); }
 T& index( unsigned i, unsigned j, bool boundaryCheck ) {
  if ( boundaryCheck && i<m && j<n ) return vs.at(i + m * j);
  return outOfBounds;
 }
 bool withinBounds(int i, int j) { return ( i >= 0 && i < (int) m && j >= 0 && j < (int) n ); }
 void resize(size_t x, size_t y) {
  m=x;
  n=y;
  vs.resize(m*n);
  OnResize();
 }
 int M() { int r=(int) m; return r; }
 int N() { int r=(int) n; return r; }
 int size() { return (int) (m*n); }
 int length() { return size(); }
 virtual void OnResize() {}
 virtual ~matrix2d() {}
private:
 size_t m;
 size_t n;
 std::vector<T> vs;
};

template<typename T>
struct matrix1d {
public:
    matrix1d(size_t m=0) : m(m), vs(m) {}
    T& operator ()(unsigned i) {
        return vs[i + m];
    }
    void resize(size_t s) {
        m=s;
        vs.resize(m);
    }
private:
    size_t m;
    std::vector<T> vs;
};

#if !defined(null)
#define null nullptr
#endif

#define FPS 60
#define MSPF (1000/60)
#define FRAMETIME (1.0f/60.0f)
#define FRAMETIMEd (1.0/60.0)

// Pluggable class functions (member function pointers).
// PROGRAMMABLE establishes a member function pointer that can be set with pointer=&Something::Method;
// PROGRAMMABLES establishes a member function pointer array that is initialized to a default empty function
// PROGRAMMABLES requires "member_Init()" to be called in class constructor
// Legend: t=owner's class, p=pointer type symbol, v=storage variable symbol, f=member function symbol, n=depth, i=integer variable symbol
#define PROGRAMMABLE(t,p,v,f)   typedef void (t::*p)(); t::p v; void f() { (((t *)this)->*(t::v))(); }
#define PROGRAMMABLE_PLUG(t,v,f)  v=&t::f;
#define PROGRAMMABLES(t,p,v,f,n)  typedef void (t::*p)(); t::p v[n]; void f() { for ( int i=0; i<n; i++ ) v[i](); } void f_Empty() {} void f_Init() { for ( int t_iter=0; t_iter<n; t_iter++ ) { v[t_iter]=&t::f_Empty; } }
#define PROGRAMMABLES_PLUG(t,v,n,f)  v[n]=&t::f;
//#define EXECUTE_PROGRAMMABLES_IN_ORDER(p,v,i,n)  { for ( int i=0; i<n; i++ ) v[i](); }
#define PROGRAMMABLET(t,p,v,r,f)   typedef r (t::*p)(); t::p v; r f() { return (((t *)this)->*(t::v))(); }

#define PCLONES(t) Zp<t> cloned; PROGRAMMABLE(t,_ClonePtr,_clone,Clone);
#define PCLONEABLE(child,code) void Cloneable() { PROGRAMMABLE_PLUG(child,_clone,CloneMe); } void CloneMe() {code}
#define PCLONE(o,base,v) { o->Clone(); v=(base *)cloned; }

// Alternative naming for above
//#define PLUG PROGRAMMABLE
//#define PLUG_IN PROGRAMMABLE_PLUG
//#define PLUGS PROGRAMMABLES
//#define PLUGS_IN PROGRAMMABLES_PLUG
//#define PLUGIT EXECUTE_PROGRAMMABLES_IN_ORDER

static const double PIDiv180 = 0.017453292519943295769236907684886;

//#define byte (unsigned short)
//typedef unsigned short byte;

/*
 * Ecere symbol modifications
 */

#define COLOR_RGBA pixelFormatRGBA

//#define BLIT(x,y,sx,sy,ex,ey)  ( for ( (x)=

/*
 * 64 byte vertex
 * Source: http://www.opengl.org/wiki/VBO#Sample_Code
 */
#define VERTEX_BUFFER_SIZE          4194304
#define VERTEX_BUFFER_SIZE_ENGINE   4194304

#define VBUFFER_OFFSET(i)  ((char *)null + (i))

/*
 * GLSL bindings, used by glsl.c et al
 */

#define UNIFORM_SIZE 4
#define ATTRIB_SIZE  2

/*******************************************************************
 * Utility macros.
 */
#define PI          3.1415926535897932384626433832795
#define TWO_PI     (3.1415926535897932384626433832795*2.0)
#define HALF_PI    (3.1415926535897932384626433832795/2.0)
#define QUARTER_PI (3.1415926535897932384626433832795/4.0)
#define THREE_QUARTERS_PI HALF_PI+QUARTER_PI

#define PIf          3.1415926535897932384626433832795f
#define TWO_PIf     (3.1415926535897932384626433832795f*2.0f)
#define HALF_PIf    (3.1415926535897932384626433832795f/2.0f)
#define QUARTER_PIf (3.1415926535897932384626433832795f/4.0f)
#define THREE_QUARTERS_PIf HALF_PIf+QUARTER_PIf

#define PI_Simple 3.1415926
#define TWO_PI_Simple 3.1415926*2

#define EPSILON 1e-6f
#define MACHINE_EPSILON FLT_EPSILON

//#define EPSILON   FLT_EPSILON// 0.000000000000000111022302463

// math
#define BETWEEN(v,A,B)             ( (A)<(B) ? (v) > (A) && (v) < (B) : (v) < (A) && (v) > (B) )
#define BETWEENInclusive(v,A,B)    ( (A)<(B) ? (v) >= (A) && (v) <= (B) : (v) <= (A) && (v) >= (B) )
#define WITHIN(x,y,x1,y1,x2,y2)    ( (x) >= (x1) && (x) <= (x2) && (y) >= (y1) && (y) <= (y2) )
#define WITHIN3D(x,y,z,x1,y1,z1,x2,y2,z2) ( (x) >= (x1) && (x) <= (x2) && (y) >= (y1) && (y) <= (y2) && (z) >= (z1) && (z) <= (z2) )
#define WITHINCLUSIVE(x,y,x1,y1,x2,y2) ( (x) > (x1) && (x) < (x2) && (y) > (y1) && (y) < (y2) )
#define WITHIN75(ar,m)             ( abs( (ar) - (m) ) < (ar)*0.75 )
#define interpolate(l, v0, v32)    ( (v0) + (l) * ((v32)-(v0)) / 32 )
#define INTERPOLATE(min, max, percent)   ( (min) + ((max) - (min) / (percent) == 0 ? 1 : (percent) ) )
#define LERP1(a,b,r)               ( UMIN(a,b) + (UMAX(a,b) - UMIN(a,b)) * r )
#define bLERP1(from,to,r)          ( (to-from) * r )
#define UMIN(a, b)                 ((a) < (b) ? (a) : (b))
#define UMAX(a, b)                 ((a) > (b) ? (a) : (b))
#define URANGE(less, b, more)            ((b) < (less) ? (less) : ((b) > (more) ? (more) : (b))) // clamp B between A and C
#define INRANGE(x,ra,rb,yes,no)    ((x) >= (ra) && (x) <= (rb) ? (yes) : (no))
#define CLAMP(a,b)                 ( (a) < 0 ? 0 : (a) > (b) ? (b) : (a) )
#define CLAMPTO(v,a,b)             ( (v) < (a) ? (a) : ( (v) > (b) ? (b) : (v) ) )
#define UWRAP(a,b,c)               ((b) > (c) ? (b)-(c) : (b) < (a) ? (b)+(c))
#define SQ(a)                      ((a)*(a))
#define SIGN(a)                    ( (a) >= 0 ? 1 : -1 )
#define LINEATE(x,y,x_size)        ( ((x)+(y)*(x_size)) )
#define FINDDESC(x,y,tnum)         ( ((x)*(y)%(tnum)) )
#define NEARBY(a,b,nearness)       ( ((a)-(nearness)) < (b) && ((a)+(nearness)) > b )

#define MIN2MAX(t,a,b) if ( a > b ) { t temp; temp=a; a=b; b=temp; } 
#define MIN2MAXT(t,s,e) if ( s.x > e.x ) { t temp; temp=s.x; s.x=e.x; e.x=temp; } if ( s.y > e.y ) { t temp; temp=s.y; s.y=e.y; e.y=temp; }

#define RANDRANGE(func,max,max2,division) ( ((max)-func(0.0,(max2)))/(func(1.0,division)) )

// Moves numbers toward a value by a delta if not near
#define TOWARDS(v,t,d)         if(BETWEEN(v,(t)-(d),(t)+(d))){}else if((t)>v)v+=(d);else if((t)<(v))v-=(d);
#define TOWARDSTOP(v,t,d)      if(BETWEEN(v,(t)-(d),(t)+(d))){v=t;}else if((t)>v)v+=(d);else if((t)<(v))v-=(d);

/*
 * Definitions brought in from somewhere when building in Ecere, now here because they worked.
 */

//  clip b to a-c
#define RANGE(a, b, c)            ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
///#define NEARZERO(b)               (RANGE(-EPSILON,b,EPSILON)) /// Does not work!

#define ADIFF(a,b)                 ( (a) < (b) ? ((b)-(a)) : ((a)-(b)) )

// Lazy programmer marcos
#define LOOP0(i,m) for ( int i=0; i<m; m++ )

// strings
#define LOWER(c)                ((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)                ((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_ALPHA(c)             ((c) >= 'A' && (c) <= 'Z' || (c) >= 'a' && (c) <= 'z')
#define IS_VOWEL(c)             ((c) == 'A' || (c) == 'a' || \
                                 (c) == 'E' || (c) == 'e' || \
                                 (c) == 'I' || (c) == 'i' || \
                                 (c) == 'O' || (c) == 'o' || \
                                 (c) == 'U' || (c) == 'u'    )
// flags
#define IS_SET(flag, bit)       ((flag) & (bit))
#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))

// pointers and string assignment
#define NST(pointer)            (pointer == NULL ? "" : pointer)
#define MTD(str)                ((str == NULL || str[0] == '\0' ))
#define MTP(str)                ((str == NULL || *str   == '\0' ))
#define STRASN(dat1, dat2)      (free_string( dat1 );   dat1 = str_dup( dat2 ))


// Used by alpha surfaces
#define ISNEAR(a,b)  ( a > b-5 && b+5 < a )

// Blending mode macaroni
#define SOFT(A,B)    ( (B)<128 ? 2*(((A)>>1)+64)*(B)/255 : 255-(2*(255-(((A)>>1)+64))*(255-(B))/255) )
#define HARD(A,B)    ( (A)<128 ? (2*(A)*(B)/255) : 255-2*(255-(A))*(255-(B))/255 )
#define SCREEN(A,B)  ( 255 - ( (255-(A)) * (255-(B)) / 255 ) )
#define OVERLAY(A,B) ( (B)<128 ? (2*(A)*(B)/255) : 255-2*(255-(A))*(255-(B))/255 )

typedef unsigned  char	byte;
typedef unsigned  short	word;
typedef unsigned  long  dword;
typedef signed    char  sbyte;
typedef signed    short sword;
typedef	signed    long  sdword;

// An often typo
#define breka break