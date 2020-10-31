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
#include "macros.h"
#include "LinkedList.h"
#include "Indexed.h"
#include "ZeroTypes.h"
#include "CDateTime.h"

float pseudo( int seed );

/* From Jim Bumgardner, author of "Pixel Magic": A note on pow( random(1), N );
This is a useful way to get a random number with an unequal distribution, in which you want the lower part of the range to receive more emphasis than the upper part of the range. In this case, for example, I used it to get a lot of reds and greens, but not too many blues.
With plain old random(1), the colors would have been more evenly distributed, with fewer reds, and more blues.
By changing the value of N, you can control the amount of "weight" in the lower part of the curve.
*/

/*
BEGIN: MT19937 Seeded Random Number Generator
A C-program for MT19937, with initialization improved 2002/1/26.
Coded by Takuji Nishimura and Makoto Matsumoto.

Before using, initialize the state by using init_genrand(seed) or init_by_array(init_key, key_length).

Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura, All rights reserved.                          

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. The names of its contributors may not be used to endorse or promote products derived from this software without specific prior written 
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Any feedback is very welcome.
http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */  
#define mtN 624
#define mtM 397
#define mtMATRIX_A 0x9908b0dfUL   /* constant vector a */
#define mtUPPER_MASK 0x80000000UL /* most significant w-r bits */
#define mtLOWER_MASK 0x7fffffffUL /* least significant r bits */

class MT19937intRand {
public:
 Indexed<unsigned long> mt;
 int mti; /* mti==N+1 means mt[N] is not initialized */

 MT19937intRand() {
  mt.Size(mtN);
  mti=mtN+1;
 }

/* initializes mt[N] with a seed */
 void Seed(unsigned long s) {
  mt[0]= s & 0xffffffffUL;
  for (mti=1; mti<mtN; mti++) {
   mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
   /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
   /* In the previous versions, MSBs of the seed affect   */
   /* only MSBs of the array mt[].                        */
   /* 2002/01/09 modified by Makoto Matsumoto             */
   mt[mti] &= 0xffffffffUL;
   /* for >32 bit machines */
  }
 }

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
/* adapted to a class, 2012/5/20 H. Elwood */

 void Seed(unsigned long init_key[], int key_length) {
  int i, j, k;
  Seed(19650218UL);
  i=1; j=0;
  k = (mtN>key_length ? mtN : key_length);
  for (; k; k--) {
   mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + init_key[j] + j; /* non linear */
   mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
   i++; j++;
   if (i>=mtN) { mt[0] = mt[mtN-1]; i=1; }
   if (j>=key_length) j=0;
  }
  for (k=mtN-1; k; k--) {
   mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i; /* non linear */
   mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
   i++;
   if (i>=mtN) { mt[0] = mt[mtN-1]; i=1; }
  }
  mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
 }

/* generates a random number on [0,0xffffffff]-interval */
 unsigned long int32(void) {
  unsigned long y;
  static unsigned long mag01[2]={0x0UL, mtMATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */  
  if (mti >= mtN) { /* generate N words at one time */
  	int kk;  
  	if (mti == mtN+1)   /* if init_genrand() has not been called, */
  		Seed(5489UL); /* a default initial seed is used */  
  	for (kk=0;kk<mtN-mtM;kk++) {
  		y = (mt[kk]&mtUPPER_MASK)|(mt[kk+1]&mtLOWER_MASK);
  		mt[kk] = mt[kk+mtM] ^ (y >> 1) ^ mag01[y & 0x1UL];
  	}
  	for (;kk<mtN-1;kk++) {
  		y = (mt[kk]&mtUPPER_MASK)|(mt[kk+1]&mtLOWER_MASK);
  		mt[kk] = mt[kk+(mtM-mtN)] ^ (y >> 1) ^ mag01[y & 0x1UL];
  	}
  	y = (mt[mtN-1]&mtUPPER_MASK)|(mt[0]&mtLOWER_MASK);
  	mt[mtN-1] = mt[mtM-1] ^ (y >> 1) ^ mag01[y & 0x1UL];  
  	mti = 0;
  }  
  y = mt[mti++];  
  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);  
  return y;
 }

/* generates a random number on [0,0x7fffffff]-interval */
 long int31(void) {
	return (long)(int32()>>1);
 }

/* generates a random number on [0,1]-real-interval */
 double real1(void) {
	return int32()*(1.0/4294967295.0); 
	/* divided by 2^32-1 */ 
 }

/* generates a random number on [0,1)-real-interval */
 double real2(void) {
	return int32()*(1.0/4294967296.0); 
	/* divided by 2^32 */
 }

/* generates a random number on (0,1)-real-interval */
 double real3(void) {
  return (((double)int32()) + 0.5)*(1.0/4294967296.0); 
	/* divided by 2^32 */
 }

/* generates a random number on [0,1) with 53-bit resolution*/
 double res53(void) { 
  unsigned long a=int32()>>5, b=int32()>>6; 
  return(a*67108864.0+b)*(1.0/9007199254740992.0); 
 } 
};


//////////////// end of MTrand

// Ziggurat //////////////////////////////////////////

/* The ziggurat method for RNOR and REXP - Combine the code below with the main program in which you want
normal or exponential variates.   Then use of RNOR in any expression will provide a standard normal variate
with mean zero, variance 1, while use of REXP in any expression will provide an exponential variate
with density exp(-x),x>0.

Before using RNOR or REXP in your main, insert a command such as zigset(86947731 );
with your own choice of seed value>0, rather than 86947731.
(If you do not invoke zigset(...) you will get all zeros for RNOR and REXP.) 
For details of the method, see Marsaglia and Tsang, 
"The ziggurat method for generating random variables", Journ. Statistical Software.
*/

class Ziggurat {
public:
 unsigned long jz,jsr;
 long hz;
 unsigned long iz, kn[128], ke[256];
 float wn[128],fn[128], we[256],fe[256];
 Ziggurat() {
  jz=0;
  hz=0;
  iz=0;
  jsr=123456789;
  Seed(123456789);
 }
 Ziggurat( unsigned long seed ) { 
  jz=0;
  hz=0;
  iz=0;
  jsr=123456789;
  Seed(seed);
 }

#define SHR3 (jz=jsr, jsr^=(jsr<<13), jsr^=(jsr>>17), jsr^=(jsr<<5),jz+jsr)
#define UNI (.5f + (signed) SHR3*.2328306e-9)
#define IUNI SHR3


#define RNOR (hz=SHR3, iz=hz&127, (absf(hz)<kn[iz])? hz*wn[iz] : nfix())
#define REXP (jz=SHR3, iz=jz&255, (     jz <ke[iz])? jz*we[iz] : efix())

/* nfix() generates variates from the residue when rejection in RNOR occurs. */

 float nfix(void) {
  const float r = 3.442620f;     /* The start of the right tail */
  static float x, y;
  for(;;) {
   x=hz*wn[iz];      /* iz==0, handles the base strip */
   if(iz==0){
    do{
     x=(float)(-log(UNI)*0.2904764); 	/* .2904764 is 1/r */
     y=(float)-log(UNI);
    }
    while(y+y<x*x);
    return (hz>0)? r+x : -r-x;
   }
   /* iz>0, handle the wedges of other strips */
   if( fn[iz]+UNI*(fn[iz-1]-fn[iz]) < exp(-.5*x*x) ) return x;
   /* initiate, try to exit for(;;) for loop*/
   hz=SHR3;
   iz=hz&127;
   if(absf((float)hz)<kn[iz]) return (float)(hz*wn[iz]);
  } 
 }

/* efix() generates variates from the residue when rejection in REXP occurs. */
 float efix(void) {
  float x;
  for(;;){
   if(iz==0) return (float)(7.69711-log(UNI));
   /* iz==0 */
   x=jz*we[iz];
   if( fe[iz]+UNI*(fe[iz-1]-fe[iz]) < exp(-x) ) return (x);
   /* initiate, try to exit for(;;) loop */
   jz=SHR3;
   iz=(jz&255);
   if(jz<ke[iz]) return (jz*we[iz]);
  }
 }
 /*--------This procedure sets the seed and creates the tables------*/
 void Seed(unsigned long jsrseed) {
  const double m1 = 2147483648.0, m2 = 4294967296.;
  double dn=3.442619855899,tn=dn,vn=9.91256303526217e-3, q;
  double de=7.697117470131487, te=de, ve=3.949659822581572e-3;
  int i;
  jsr^=jsrseed;

/* Set up tables for RNOR */
  q=vn/exp(-.5*dn*dn);
  kn[0]=(unsigned long)((dn/q)*m1);
  kn[1]=0;

  wn[0]=(float)(q/m1);
  wn[127]=(float)(dn/m1);

  fn[0]=1.0f;
  fn[127]=(float)exp(-.5*dn*dn);

  for(i=126;i>=1;i--) {
   dn=sqrt(-2.*log(vn/dn+exp(-.5*dn*dn)));
   kn[i+1]=(unsigned long)((dn/tn)*m1);
   tn=dn;
   fn[i]=(float)exp(-.5*dn*dn);
   wn[i]=(float)(dn/m1);
  }

/* Set up tables for REXP */
  q = ve/exp(-de);
  ke[0]=(unsigned long) ((de/q)*m2);
  ke[1]=0;

  we[0]=(float)(q/m2);
  we[255]=(float)(de/m2);

  fe[0]=1.0f;
  fe[255]=(float)exp(-de);

  for(i=254;i>=1;i--) {
   de=-log(ve/de+exp(-de));
   ke[i+1]=(unsigned long) ((de/te)*m2);
   te=de;
   fe[i]=(float)exp(-de);
   we[i]=(float)(de/m2);
  }
 }
};


//////////////////////////////////////////////////////

extern const float xyzs[60000];
extern int randomer;

// Holds psuedo-random numbers generated by rand()
struct randSet {
public:
 Indexed<Zushort> rands;
 randSet() {
  rands.Size(PSEUDORAND_MAX);
 }
};

// Holds atmospheric noise and related psuedo-random number values based on binary Random.org daily dumps
struct Random : public ListItem {
public:
 MT19937intRand mt;
 Indexed<Zbyte> randoms;
 Indexed<Zuint> randomsui;
 Indexed<Zint> randomsi;
 Indexed<Zfloat> randomsf;
 Indexed<Zfloat> resequenced;
 Indexed<randSet> rands;
 Random() {
  randoms.Size(1000000);
  randomsui.Size(250000);
  randomsi.Size(250000);
  resequenced.Size(100000);
  randomsf.Size(250000);
  rands.Size(100);
 }
 // Returns abs random integer using a wrapping index from the randomsi table
 int arnd(int s)                   { return absi(randomsi.Wrap(s));  }
 // Returns abs random float using a wrapping index from the random floats table
 float arndf(int s)                { return absf(randomsf.Wrap(s));  }
 // Returns abs random float using a wrapping index from the random float table and casts to double
 double arndd(int s)               { return (double) abs(randf(s));  }
 // Returns abs random integer by calling a pseudo-generated rand() using the 3d coordinate (x,y,z)
 int arnd(int x, int y, int z )    { return abs(rand(x,y,z)); }
 // Returns abs random integer by calling a pseudo-generated rand() using the 2d coordinate (x,y)
 int arnd(int x,int y)             { return abs(rand(x,y));   }
 // Returns abs random float by calling a pseudo-generated rand() using the 2d coordinate (x,y)
 float arndf(int x,int y)          { return abs(randf(x,y));  }
 // Returns abs random float by calling a pseudo-generated rand() using the 2d coordinate (x,y) and casts to float
 double rndd(int x,int y)          { return abs(randd(x,y));  }
 // Returns abs random float by acquire data from the Randoms.org resequenced random table
 float aunique(int x)               { return abs(unique(x));     }
 // Returns abs random float by acquire data from the Randoms.org resequenced random table
 float aunique(int x, int y)        { return abs(unique(x,y));   }
 // Returns abs random float by acquire data from the Randoms.org resequenced random table
 float aunique(int x, int y, int z) { return abs(unique(x,y,z)); }
 // Returns abs random float by acquire data from the Randoms.org resequenced random table and casts to double
 double auniqued(int x)             { return abs(uniqued(x));    }
 // + upto hi-lo by seed s using resequenced random.org float data and casts to int adding lo
 int arange(int s, int l, int h)          { return abs(l+(int)(resequenced.Wrap(s)*(float)(h-l))); }
 // + upto hi-lo by seed s using resequenced random.org float data and casts to float adding lo
 float arange(int s, float l, float h )   { return abs(l+(resequenced.Wrap(s)*(h-l)));             }
 // + upto hi-lo by seed s using resequenced random.org float data and casts to double adding lo
 double arange(int s, double l, double h) { return abs(l+((double)resequenced.Wrap(s)*(h-l)));     }
 // Accesses the rand() pseudo-random integer database
 int rand(int s)                   { return randomsi.Wrap(s);    }
 // Accesses the rand() pseudo-random float database
 float randf(int s)                { return randomsf.Wrap(s);    }
 // Accesses the rand() pseudo-random float database and casts to float
 double randd(int s)               { return (double) randf(s);   }
 // Accesses the rand() pseudo-random integer database and multiplies the resulting numbers to produce an int
 int rand(int x, int y, int z )    { return (int)rands.Wrap(x).rands.Wrap(y)*(int)rands.Wrap(y).rands.Wrap(z); }
 // Accesses the rand() pseudo-random integer database and multiplies the resulting numbers to produce an int
 int rand(int x,int y)             { return (int)rands.Wrap(x).rands.Wrap(y);              }
 // Accesses the rand() pseudo-random float database by doubly calling Wrap() on the seeds and divides by PSEUDORAND_MAX
 float randf(int x,int y)          { return iratiof((int)rands.Wrap(x).rands.Wrap(y),PSEUDORAND_MAX); }
 // Accesses the rand() pseudo-random float database by doubly calling Wrap() on the seeds and divides by PSEUDORAND_MAX and casts to double
 double randd(int x,int y)         { return iratiod((int)rands.Wrap(x).rands.Wrap(y),PSEUDORAND_MAX); }
 // Returns data acquired by seed from the resequenced random.org data
 float unique(int x)               { return resequenced.Wrap(x);          }
 // Returns data acquired by x+y from the resequenced random.org data
 float unique(int x, int y)        { return resequenced.Wrap(x+y);        }
 // Returns data acquired by x+y+z from the resequenced random.org data
 float unique(int x, int y, int z) { return resequenced.Wrap(x+y+z);      }
 // Returns data acquired by seed from the resequenced random.org data and casts to float
 double uniqued(int x)             { return (double) resequenced.Wrap(x); }
 // Questions arnd()
 bool Query(int x)                 { return arnd(x)%2==1;   }
 // Questions arnd()
 bool Query(int x, int y)          { return arnd(x,y)%2==1; }
 // returns upto h by seed s using resequenced random.org float data
 int upto(int s, int h)                  { return (int)(resequenced.Wrap(s)*(float)h);       }
 // returns upto h by seed s using resequenced random.org float data
 float upto(int s, float h)              { return resequenced.Wrap(s)*h;                     }
 // returns upto h by seed s using resequenced random.org float data and casts to double
 double upto(int s, double h)            { return (double)resequenced.Wrap(s)*h;             }
 // +/- returns upto hi-lo by seed s using resequenced random.org float data and casts to int adding lo
 int range(int s, int l, int h)          { return l+(int)(resequenced.Wrap(s)*(float)(h-l)); }
 // +/- returns upto hi-lo by seed s using resequenced random.org float data and casts to float adding lo
 float range(int s, float l, float h )   { return l+(resequenced.Wrap(s)*(h-l));             }
 // +/- upto hi-lo by seed s using resequenced random.org float data and casts to double adding lo
 double range(int s, double l, double h) { return l+((double)resequenced.Wrap(s)*(h-l));     }
 // +/- splits the difference alternating 0,1 to return random numbers from the rand() number tables
 int dutch(int x, int y, int z)    { return x%2==1 ? rand(x,y) : rand(y,z);   }
 // +/- splits the difference alternating 0,1 to return random numbers from the rand() number tables
 float dutch(int x, int y)         { return x%2==1 ? randf(y,x) : randf(x,y); } 
 // +/- splits the difference alternating 0,1 to return random numbers from the rand() number tables
 float cuban(int x, int y, int z)  { return x%2==1 ? randf(x,y) : randf(y,z); }
 // +/- splits the difference alternating 0,1 to return random numbers from the rand() number tables
 int cuban(int x, int y)           { return x%2==1 ? rand(y,x) : rand(x,y);   }
 // +/- splits the difference alternating 0,1 to return calls to unique()
 float xyzzyx(int x, int y, int z ) {
  switch ( (x+y+z)%6 ) {
   case 0: return unique(x,y,z); case 1: return unique(y,z,x);
   case 2: return unique(z,x,y); case 3: return unique(z,y,x);
   case 4: return unique(y,x,z); default: case 5: return unique(x,z,y);
  }
 }
 // +/- executes unique(?,?,?) in a variety of ways sometimes adding seeds together
 float rubix(int x, int y, int z ) {
  switch ( (x+y+z)%(9*6) ) {
   default:
    case 0: return unique(x,y,z);        case 1: return unique(y,z,x);
    case 2: return unique(z,x,y);        case 3: return unique(z,y,x);
    case 4: return unique(y,x,z);        case 5: return unique(x,z,y);
    case 6: return unique(x+x,y,z);      case 7: return unique(y+x,z,x);
    case 8: return unique(z,x+x,y);      case 9: return unique(z,y,x+x);
   case 10: return unique(y,x,z+x);     case 11: return unique(x,z,y+x);
   case 12: return unique(x+y,y,z);     case 13: return unique(y,z+y,x);
   case 14: return unique(z,x,y+y);     case 15: return unique(z+y,y,x);
   case 16: return unique(y,x+y,z);     case 17: return unique(x,z,y+y);
   case 18: return unique(x+z,y,z);     case 19: return unique(y,z+z,x);
   case 20: return unique(z,x,y+z);     case 21: return unique(z,y,x+z);
   case 22: return unique(y,x+y,z);     case 23: return unique(x,z+y,y);
   case 24: return unique(x,y,z+y);     case 25: return unique(y+y,z,x);
   case 26: return unique(z+z,x,y);     case 27: return unique(z,y+z,x);
   case 28: return unique(y,x,z+z);     case 29: return unique(x+z,z,y);
   case 30: return unique(x+x,y+y,z);   case 31: return unique(y+x,z+y,x);
   case 32: return unique(z+x,x+x,y);   case 33: return unique(z+z,y+x,x+y);
   case 34: return unique(y+z,x+x,z+x); case 35: return unique(x+z,z+z,y+z);
   case 36: return unique(x+y,y+z,z+x); case 37: return unique(y+z,z+y,x+z);
   case 38: return unique(z+x,x+x,y+x); case 39: return unique(z+z,y+z,x+z);
   case 40: return unique(y+y,x+y,z+y); case 41: return unique(x+z,z+y,y+x);
   case 42: return unique(x+z,y+y,z+x); case 43: return unique(y+x,z+y,x+z);
   case 44: return unique(z+x,x+y,y+z); case 45: return unique(z+y,y+x,x+y);
   case 46: return unique(y+z,x+z,z+x); case 47: return unique(x+x,z+z,y+x);
   case 48: return unique(x+y,y+z,z+x); case 49: return unique(y+x,z+x,x+x);
   case 50: return unique(z+z,x+x,y+z); case 51: return unique(z+z,y+y,x+x);
   case 52: return unique(y+z,x+x,z);   case 53: return unique(x+z,z,y+z);
  }
 }
 // returns a range using three seeds tied to xyzzyx
 int range(int sa, int sb, int sc, int l, int h)       { return l+(int)(xyzzyx(sa,sb,sc)*(float)(h-l)); }
 // returns a range using three seeds tied to xyzzyx
 float range(int sa, int sb, int sc, float l, float h) { return l+xyzzyx(sa,sb,sc)*(h-l); }
};

class Randoms : public LinkedList {
public:
 Ziggurat zig;
 bool advances;
 int seeder;
 Randoms() : LinkedList() {
  advances=true;
  Reset();
 }
 void Reset() { seeder=0; }
 void Advance() { seeder++; }
 void LoadRandomDotOrg( const char *filename );
 void LoadRandom( const char *filename );
 void SaveRandom( const char *filename, Random *set );
 Random *GetSet() { return (Random *) Element(upto(count,seeder+upto(666))); }
 Random *GetSet( int pick ) { return (Random *) Element(pick%count); }
 int Range( int lo, int hi ) {
  Random *set=GetSet();
  if ( advances ) Advance();
  double f=set->auniqued(seeder);
  Zint result;
       if ( lo >= 0 && hi >= 0 ) result=lo+(int)(f*(hi-lo));
  else if ( lo < 0 && hi <= 0 ) result=-(abs(hi)+(int)((f*abs(hi-lo))));
  else if ( lo < 0 && hi > 0 ) result=lo+(int)(f*(hi+abs(lo)));
  return result;
 }
 int Range( int lo, int hi, int pick_a_set ) {
  Random *set=GetSet(pick_a_set);
  if ( advances ) Advance();
  double f=set->auniqued(seeder);
  Zint result;
       if ( lo >= 0 && hi >= 0 ) result=lo+(int)(f*(hi-lo));
  else if ( lo < 0 && hi <= 0 ) result=-(abs(hi)+(int)((f*abs(hi-lo))));
  else if ( lo < 0 && hi > 0 ) result=lo+(int)(f*(hi+abs(lo)));
  return result;
 }
 int Range( int lo, int hi, int pick_a_set, int provide_a_seed ) {
  Random *set=GetSet(pick_a_set);
  double f=set->auniqued(provide_a_seed);
  Zint result;
       if ( lo >= 0 && hi >= 0 ) result=lo+(int)(f*(hi-lo));
  else if ( lo < 0 && hi <= 0 ) result=-(abs(hi)+(int)((f*abs(hi-lo))));
  else if ( lo < 0 && hi > 0 ) result=lo+(int)(f*(hi+abs(lo)));
  return result;
 }
 int Integer() {
  Random *set=GetSet();
  int result=set->rand(seeder);
  if ( advances ) Advance();
  return result;
 }
 int Integer(int pick_a_set) {
  Random *set=GetSet(pick_a_set);
  int result=set->rand(seeder);
  if ( advances ) Advance();
  return result;
 }
 int Integer(int pick_a_set, int provide_a_seed) {
  Random *set=GetSet(pick_a_set);
  int result=set->rand(provide_a_seed);
  return result;
 }
 int Integer(int pick_a_set, int provide_a_seed, int provide_an_iterative) {
  Random *set=GetSet(pick_a_set);
  int result=set->rand(provide_a_seed,provide_an_iterative);
  return result;
 }
 float Float() {
  Random *set=GetSet();
  float result=set->randf(seeder);
  if ( advances ) Advance();
  return result;
 }
 float Float(int pick_a_set) {
  Random *set=GetSet(pick_a_set);
  float result=set->randf(seeder);
  if ( advances ) Advance();
  return result;
 }
 float Float(int pick_a_set, int provide_a_seed) {
  Random *set=GetSet(pick_a_set);
  float result=set->randf(provide_a_seed);
  return result;
 }
 float Float(int pick_a_set, int provide_a_seed, int provide_an_iterative) {
  Random *set=GetSet(pick_a_set);
  float result=set->randf(provide_a_seed,provide_an_iterative);
  return result;
 }
 float Unique() {
  Random *set=GetSet(seeder);
  float result=set->unique(seeder);
  if ( advances ) Advance();
  return result;
 }
 float Unique(int pick_a_set) {
  Random *set=GetSet(pick_a_set);
  float result=set->unique(seeder);
  if ( advances ) Advance();
  return result;
 }
 float Unique(int pick_a_set, int provide_a_seed) {
  Random *set=GetSet(pick_a_set);
  float result=set->unique(provide_a_seed);
  return result;
 }
 int Upto( int max, int pick_a_set, int provide_a_seed ) {
  return (int) (Unique(pick_a_set,provide_a_seed)*(float)max);
 }
 int Upto( int max, int pick_a_set ) {
  return (int) (Unique(pick_a_set)*(float)max);
 }
 int Upto( int max ) {
  return (int) (Unique()*(float)max);
 }
 bool Query() {
  Random *set=GetSet(seeder);
  float result=set->unique(seeder);
  if ( advances ) Advance();
  return result>=0.5f;
 }
 bool Query(int pick_a_set) {
  Random *set=GetSet(pick_a_set);
  float result=set->unique(seeder);
  if ( advances ) Advance();
  return result>=0.5f;
 }
 bool Query(int pick_a_set, int provide_a_seed) {
  Random *set=GetSet(pick_a_set);
  float result=set->unique(provide_a_seed);
  return result>=0.5f;
 }
 int TimeSeed() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  CDateTime clock(now);
  return ( clock.GetHour()+clock.GetDay()+clock.GetDayOfWeek()+clock.GetMinute()+clock.GetSecsSinceMillenium() );
 }
 int DaySeed() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  return (int) ( now.wHour + now.wMinute + now.wSecond );
 }
 int HourSeed() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  return (int) ( now.wMinute + now.wSecond );
 }
 int MinuteSeed() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  return (int) ( now.wSecond + now.wMilliseconds );
 }
 int MinutelySeed() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  return (int) now.wMinute;
 }
 int Sign(int number) { return ( number %2 == 1 ? -1 : 1 ); }
 CLEARLISTRESET(Random);
 ~Randoms() { Clear(); }
};

extern Randoms randoms;

int randomify( int seed, int l, int h );
float randomify2( int seed, int max );
int randify( int seed, int max );
float randify( int seed, float max );