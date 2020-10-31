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
#include "moremath.h"
#include "Display.h"
#include "Vertexd.h"
#include "Set.h"
#include "HMatrix.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "MapPosition.h"
#include "LookAtPerspective.h"
#include "Interface.h"
#include "UglyFont.h"
#include "HeightBrush.h"
#include "pThread.h"

struct VBlock;

struct TerrVertex {
 HVertex vertex,normal;
 float s, t;
};

#include "Tweens.h"

enum InfiniteTerrainEquationOperations {
 ite_add=0,
 ite_sub=1,
 ite_mul=2,
 ite_avg=3
};

class InfiniteTerrainEquation : public ListItem {
public:
 InfiniteTerrainEquationOperations opa,opb,postop; // a is the operation between pre and comp, b is the operation between comp and recomp
 Zbool modify,compute;
 InfiniteTerrainEquation() : ListItem() {
  opa=ite_add;
  opb=ite_add;
  postop=ite_avg;
  compute=true;
 }
 void Mode( bool modify, bool compute ) {
  this->modify=modify;
  this->compute=compute;
 }
 virtual void Premodify( const double x, const double z, double *out ) {}
 virtual void Modify(    const double x, const double z, double *out ) {}
 virtual void Remodify(  const double x, const double z, double *out ) {}
 virtual double Precompute( const double x, const double z ) { return 0.1*256.0; }
 virtual double Compute(    const double x, const double z ) { return 0.2*256.0; }
 virtual double Recompute(  const double x, const double z ) { return 0.3*256.0; }
};

class InfiniteTerrainEquations : public LinkedList {
protected:
 Zbool precomputations,recomputations;
public:
 CLEARLISTRESET(InfiniteTerrainEquation);
 PROGRAMMABLE(InfiniteTerrainEquations,CalculationPtr,calculation,Calculation);
 PROGRAMMABLE(InfiniteTerrainEquations,CalculatePtr,calculate,Calculate);
 double x,z,out;
 InfiniteTerrainEquations() : LinkedList() {
  x=z=out=0.0;
  calculate=&InfiniteTerrainEquations::Compute;
  calculation=&InfiniteTerrainEquations::_Calc;
 }
 void Precompute( bool toggle ) {
  if ( toggle ) {
   precomputations=true;
   if ( precomputations && recomputations ) calculate=&InfiniteTerrainEquations::PrecomputeComputeRecompute;
   else calculate=&InfiniteTerrainEquations::PrecomputeCompute;
  } else {
   precomputations=false;
   if ( recomputations ) calculate=&InfiniteTerrainEquations::ComputeRecompute;
   else calculate=&InfiniteTerrainEquations::Compute;
  }
 }
 void Recompute( bool toggle ) {
  if ( toggle ) {
   recomputations=true;
   if ( precomputations && recomputations ) calculate=&InfiniteTerrainEquations::PrecomputeComputeRecompute;
   else calculate=&InfiniteTerrainEquations::ComputeRecompute;
  } else {
   recomputations=false;
   if ( precomputations ) calculate=&InfiniteTerrainEquations::PrecomputeCompute;
   else calculate=&InfiniteTerrainEquations::Compute;
  }
 }
 void Modify( bool toggle ) {
  if ( toggle ) calculation=&InfiniteTerrainEquations::_CalcMod;
  else calculation=&InfiniteTerrainEquations::_Calc;
 }
 double Height( double X, double Z ) {
  x=X;
  z=Z;
  Calculation();
  return out;
 }
protected:
 void _Calc() { Calculate(); }
 void _CalcMod() { Calculate(); Modify(); }
 void Compute() {
  bool computed=false;
  out=0.0;
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->compute ) {
   computed=true;
   double temp=eq->Compute(x,z);
   if ( computed ) {
    switch ( eq->postop ) {
    case ite_add: out+=temp; break;
    case ite_sub: out-=temp; break;
    case ite_mul: out*=temp; break;
    case ite_avg: out+=temp; out/=2.0; break;
    }
   } else out=temp;
  }
 }
 void PrecomputeCompute() {
  bool computed=false;
  out=0.0;
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->compute ) {
   computed=true;
   double temp=eq->Precompute(x,z);
   switch ( eq->opa ) {
   case ite_add: temp+=eq->Compute(x,z); break;
   case ite_sub: temp-=eq->Compute(x,z); break;
   case ite_mul: temp*=eq->Compute(x,z); break;
   case ite_avg: temp+=eq->Compute(x,z); temp/=2.0; break;
   }
   if ( computed ) {
    switch ( eq->postop ) {
    case ite_add: out+=temp; break;
    case ite_sub: out-=temp; break;
    case ite_mul: out*=temp; break;
    case ite_avg: out+=temp; out/=2.0; break;
    }
   } else out=temp;
  }
 }
 void PrecomputeComputeRecompute() {
  bool computed=false;
  out=0.0;
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->compute ) {
   computed=true;
   double temp=eq->Precompute(x,z);
   switch ( eq->opa ) {
   case ite_add: temp+=eq->Compute(x,z); break;
   case ite_sub: temp-=eq->Compute(x,z); break;
   case ite_mul: temp*=eq->Compute(x,z); break;
   case ite_avg: temp+=eq->Compute(x,z); temp/=2.0; break;
   }
   switch ( eq->opb ) {
   case ite_add: temp+=eq->Recompute(x,z); break;
   case ite_sub: temp-=eq->Recompute(x,z); break;
   case ite_mul: temp*=eq->Recompute(x,z); break;
   case ite_avg: temp+=eq->Recompute(x,z); temp/=2.0; break;
   }
   if ( computed ) {
    switch ( eq->postop ) {
    case ite_add: out+=temp; break;
    case ite_sub: out-=temp; break;
    case ite_mul: out*=temp; break;
    case ite_avg: out+=temp; out/=2.0; break;
    }
   } else out=temp;
  }
 }
 void ComputeRecompute() {
  bool computed=false;
  out=0.0;
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->compute ) {
   computed=true;
   double temp=eq->Compute(x,z);
   switch ( eq->opb ) {
   case ite_add: temp+=eq->Recompute(x,z); break;
   case ite_sub: temp-=eq->Recompute(x,z); break;
   case ite_mul: temp*=eq->Recompute(x,z); break;
   case ite_avg: temp+=eq->Recompute(x,z); temp/=2.0; break;
   }
   if ( computed ) {
    switch ( eq->postop ) {
    case ite_add: out+=temp; break;
    case ite_sub: out-=temp; break;
    case ite_mul: out*=temp; break;
    case ite_avg: out+=temp; out/=2.0; break;
    }
   } else out=temp;
  }
 }
 void Modify() {
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->modify ) eq->Modify(x,z,&out);
 }
 void PremodifyModify() {
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->modify ) {
   eq->Premodify(x,z,&out);
   eq->Modify(x,z,&out);
  }
 }
 void PremodifyModifyRemodify() {
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->modify ) {
   eq->Premodify(x,z,&out);
   eq->Modify(x,z,&out);
   eq->Remodify(x,z,&out);
  }
 }
 void ModifyRemodify() {
  FOREACH(InfiniteTerrainEquation,eq) if ( eq->modify ) {
   eq->Modify(x,z,&out);
   eq->Remodify(x,z,&out);
  }
 }
};

class ITE_Flat : public InfiniteTerrainEquations {
public:
 double height;
 ITE_Flat() {
  height=0.0;
 }
 ITE_Flat( double scale ) {
  height=scale;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) { return height; }
 double Recompute(  const double x, const double z ) { return 0.0; }

};

class ITE_AbsSinCos : public InfiniteTerrainEquation {
public:
 double scale;
 ITE_AbsSinCos() : InfiniteTerrainEquation() {  
  scale=256.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return abs(sin(x)*cos(z))*scale;
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_AbsSinCosScaled : public InfiniteTerrainEquation {
public:
 double scale,a,b;
 ITE_AbsSinCosScaled() : InfiniteTerrainEquation() {
  scale=256.0;
  a=0.00734;
  b=0.00234;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) { return abs(sin(x*a)*cos(z*b))*scale; }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_Noise2 : public InfiniteTerrainEquation {
public:
 double scale,factor,a,b,c,d;
 ITE_Noise2() : InfiniteTerrainEquation() {
  a=0.0031212312;
  b=0.004613212;
  c=0.017412412;
  d=0.01752341;
  scale=256.0;
  factor=4.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * Noise2( a*x,  b*z ) + (scale/factor) * Noise2( c*x,  d*z );
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_Turbulence2 : public InfiniteTerrainEquation {
public:
 double scale,factor,a,b,c,d,frequency,frequency2;
 ITE_Turbulence2() : InfiniteTerrainEquation() {
  frequency=0.5f;
  frequency2=0.25f;
  a=0.0031212312;
  b=0.004613212;
  c=0.017412412;
  d=0.01752341;
  scale=256.0;
  factor=4.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * Turbulence2( a*x,  b*z, frequency ) + (scale/factor) * Turbulence2( c*x,  d*z, frequency2 );
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TileableNoise2 : public InfiniteTerrainEquation {
public:
 double scale,factor,a,b,c,d,w,h;
 ITE_TileableNoise2() : InfiniteTerrainEquation() {
  a=0.0031212312;
  b=0.004613212;
  c=0.017412412;
  d=0.01752341;
  scale=256.0;
  factor=4.0;
  w=128.0;
  h=128.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * TileableNoise2( a*x,  b*z, w, h ) + (scale/factor) * TileableNoise2( c*x,  d*z, w, h );
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TileableTurbulence2 : public InfiniteTerrainEquation {
public:
 double scale,factor,a,b,c,d,frequency,frequency2,w,h;
 ITE_TileableTurbulence2() : InfiniteTerrainEquation() {
  frequency=0.5f;
  frequency2=0.25f;
  a=0.0031212312;
  b=0.004613212;
  c=0.017412412;
  d=0.01752341;
  scale=256.0;
  factor=4.0;
  w=128.0;
  h=128.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * TileableTurbulence2( a*x,  b*z,  w, h, frequency ) + (scale/factor) * TileableTurbulence2( c*x,  d*z, w, h, frequency2 );
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_AbsRandomsUnique : public InfiniteTerrainEquation {
public:
 double scale,Xscale,Zscale;
 ITE_AbsRandomsUnique() : InfiniteTerrainEquation() {
  scale=256.0;
  Xscale=30000.0;
  Zscale=30000.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) { return scale*abs(randoms.Unique((int)(Xscale*x),(int)(Zscale*z))); }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_AbsRandomSetRubix : public InfiniteTerrainEquation {
public:
 Random *set;
 double scale,Xscale,Yscale,Zscale;
 ITE_AbsRandomSetRubix( int seed ) : InfiniteTerrainEquation() {
  set=randoms.GetSet(seed);
  scale=256.0;
  Xscale=30000.0;
  Yscale=30000.0;
  Zscale=30000.0;
 }
 ITE_AbsRandomSetRubix() : InfiniteTerrainEquation() {
  set=randoms.GetSet(4);
  scale=256.0;
  Xscale=30000.0;
  Yscale=30000.0;
  Zscale=30000.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale*abs(set->rubix((int)(Xscale*x),(int)(Zscale*z),(int)(Yscale*(x+z))));
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_AbsRandomSetRand : public InfiniteTerrainEquation {
public:
 Random *set;
 double scale,Xscale,Zscale;
 ITE_AbsRandomSetRand( int seed ) : InfiniteTerrainEquation() {
  set=randoms.GetSet(seed);
  scale=256.0;
  Xscale=30000.0;
  Zscale=30000.0;
 }
 ITE_AbsRandomSetRand() : InfiniteTerrainEquation() {
  set=randoms.GetSet(4);
  scale=256.0;
  Xscale=30000.0;
  Zscale=30000.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale*set->arndf((int)(Xscale*x),(int)(Zscale*z));
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenX : public InfiniteTerrainEquation {
public:
 double scale;
 Zpointer<Tween> tween;
 ITE_TweenX() : InfiniteTerrainEquation() {
  tween=(Tween *) tweens.first;
  scale=256.0;
 }
 ITE_TweenX( Tween *t ) : InfiniteTerrainEquation() {
  tween=t;
  scale=256.0;
 }
 ITE_TweenX( const char *t ) : InfiniteTerrainEquation() {
  tween=tweens.find(t);
  if ( !tween ) tween=(Tween *) tweens.first;
  scale=256.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * (double) tween->tween[ (unsigned int) (x) % tween->tween.length ];
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenZ : public InfiniteTerrainEquation {
public:
 double scale;
 Zpointer<Tween> tween;
 ITE_TweenZ() : InfiniteTerrainEquation() {
  tween=(Tween *) tweens.first;
  scale=256.0;
 }
 ITE_TweenZ( Tween *t ) : InfiniteTerrainEquation() {
  tween=t;
  scale=256.0;
 }
 ITE_TweenZ( const char *t ) : InfiniteTerrainEquation() {
  tween=tweens.find(t);
  if ( !tween ) tween=(Tween *) tweens.first;
  scale=256.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * (double) tween->tween[ (unsigned int) (z) % tween->tween.length ];
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenXpZ : public InfiniteTerrainEquation {
public:
 double scale,xScale,zScale;
 Zpointer<Tween> tween;
 ITE_TweenXpZ() : InfiniteTerrainEquation() {
  tween=(Tween *) tweens.first;
  scale=256.0;
  xScale=1.0;
  zScale=1.0;
 }
 ITE_TweenXpZ( Tween *t ) : InfiniteTerrainEquation() {
  tween=t;
  scale=256.0;
 }
 ITE_TweenXpZ( const char *t ) : InfiniteTerrainEquation() {
  tween=tweens.find(t);
  if ( !tween ) tween=(Tween *) tweens.first;
  scale=256.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return scale * (double) tween->tween[ (unsigned int) (xScale*x+zScale*z) % tween->tween.length ];
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenXZ : public InfiniteTerrainEquation {
public:
 double scale;
 Zpointer<Tween> tweenX,tweenZ;
 ITE_TweenXZ() : InfiniteTerrainEquation() {
  tweenX=(Tween *) tweens.first;
  tweenZ=(Tween *) tweens.last;
  scale=256.0;
 }
 ITE_TweenXZ( Tween *X, Tween *Z ) : InfiniteTerrainEquation() {
  tweenX=X;
  tweenZ=Z;
  scale=256.0;
 }
 ITE_TweenXZ( const char *X, const char *Z ) : InfiniteTerrainEquation() {
  tweenX=tweens.find(X);
  if ( !tweenX ) tweenX=(Tween *) tweens.first;
  tweenZ=tweens.find(Z);
  if ( !tweenX ) tweenX=(Tween *) tweens.last;
  scale=256.0;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {  
  return ( ( (double) tweenX->tween[ (unsigned int) x % tweenX->tween.length ]
           + (double) tweenZ->tween[ (unsigned int) z % tweenZ->tween.length ] )
           / 2.0
         ) * scale;
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenABCD : public InfiniteTerrainEquation {
public:
 double scale, factor;
 Zpointer<Tween> A,B,C,D;
 ITE_TweenABCD() : InfiniteTerrainEquation() {
  A=C=(Tween *) tweens.first;
  B=D=(Tween *) tweens.last;
  scale=256.0;
  factor=0.25;
 }
 ITE_TweenABCD( Tween *A, Tween *B, Tween *C, Tween *D ) : InfiniteTerrainEquation() {
  this->A=A;
  this->B=B;
  this->C=C;
  this->D=D;
  scale=256.0;
  factor=0.25;
 }
 ITE_TweenABCD( const char *_A, const char *_B, const char *_C, const char *_D ) : InfiniteTerrainEquation() {
  A=tweens.find(_A);  if ( !A ) A=(Tween *) tweens.first;
  B=tweens.find(_B);  if ( !B ) B=(Tween *) tweens.last;
  C=tweens.find(_C);  if ( !C ) C=(Tween *) tweens.first;
  D=tweens.find(_D);  if ( !D ) D=(Tween *) tweens.last;
  scale=256.0;
  factor=0.25;
 }
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {  
  return (
           ( (double) A->tween[ (unsigned int) x % A->tween.length ]
           + (double) B->tween[ (unsigned int) z % B->tween.length ]
           + (double) C->tween[ (unsigned int) (x * factor) % C->tween.length ]
           + (double) D->tween[ (unsigned int) (z * factor) % D->tween.length ]
           ) * 0.25
         ) * scale;
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_TweenList : public InfiniteTerrainEquation {
public:
 double scale;
 TweenHandles selected;
 InfiniteTerrainEquationOperations op;
 ITE_TweenList() : InfiniteTerrainEquation() {
  op=ite_avg;
  scale=256.0;
 }
 void AddTween( Tween *t ) {
  selected.Add(t);
 }
 void AddTween( const char *add ) {
  Tween *t=tweens.find(add);
  if ( t ) selected.Add(t);
  else OUTPUT("InfiniteTerrain::AddTween(`%s`): invalid keyword (tween not found).\n",add);
 } 
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  double out=0.0;
  switch ( op ) {
  case ite_add:
    EACH(selected.first,TweenHandle,t) {
     out+= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
    }
   break;
  case ite_sub:
    EACH(selected.first,TweenHandle,t) {
     if ( !t->prev ) out= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
     else out-= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
    }
   break;
  case ite_mul:
    out=1.0;
    EACH(selected.first,TweenHandle,t) {
     out*= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
    }
   break;
  case ite_avg:
    EACH(selected.first,TweenHandle,t) {
     if ( !t->prev ) out= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
     else {
      out-= (double) t->p->tween[ (unsigned int) (x+z) % t->p->tween.length ];
      out/=2.0;
     }
    }
   break;
  }
  return out*scale;
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

class ITE_HeightBrush : public InfiniteTerrainEquation {
public:
 Zpointer<HeightBrush> brush;
 double scale,xScale,zScale;
 ITE_HeightBrush() : InfiniteTerrainEquation() {
  brush=(HeightBrush *) heightbrushes.first;
  xScale=128.0;
  zScale=128.0;
  scale=512.0;
 } 
 ITE_HeightBrush( HeightBrush *hb ) : InfiniteTerrainEquation() {
  brush=hb;
  xScale=0.1;
  zScale=0.1;
  scale=512.0;
 } 
 void Premodify( const double x, const double z, double *out ) {}
 void Modify(    const double x, const double z, double *out ) {}
 void Remodify(  const double x, const double z, double *out ) {}
 double Precompute( const double x, const double z ) { return 0.0; }
 double Compute(    const double x, const double z ) {
  return brush->brush((unsigned int)(xScale*x)%brush->width,(unsigned int)(zScale*z)%brush->height)*scale;
 }
 double Recompute(  const double x, const double z ) { return 0.0; }
};

struct VBlock : public ListItem {
 TerrVertex *vertices;
 size_t element_size;
 Zint density, width, size, x, z, pDensity, pDensitySize, tRepeat;
 Zdouble minX,minZ,w;
 GLuint vbo;
 Zp<InfiniteTerrainEquations> generator;
 Zbool uploaded;
 VBlock( int X, int Z, int blockDensity, double blockSize, int texRepeat, InfiniteTerrainEquations *eq ) {
  element_size=sizeof(TerrVertex);
  vertices=null;
  x=X;
  z=Z;
  tRepeat=texRepeat;
  density=blockDensity;
  pDensity = density + 2;
  pDensitySize = SQ(pDensity);
  size=SQ(blockDensity);
  width=blockSize;
  w = iratiod(width,blockDensity-1);
  minX = ((double)x - 0.5) * width;
 //float maxX = (block->x + 0.5f) * blockSize;
  minZ = ((double)z - 0.5) * width;
  vertices = new TerrVertex[(unsigned int)size];
  generator=eq;
  Empty();
  Upload();
 }
 VBlock( int X, int Z, int blockDensity, double blockSize, int texRepeat, InfiniteTerrainEquations *eq, bool calculate ) {
  element_size=sizeof(TerrVertex);
  vertices=null;
  x=X;
  z=Z;
  tRepeat=texRepeat;
  density=blockDensity;
  pDensity = density + 2;
  pDensitySize = SQ(pDensity);
  size=SQ(blockDensity);
  width=blockSize;
  w = iratiod(width,blockDensity-1);
  minX = ((double)x - 0.5) * width;
 //float maxX = (block->x + 0.5f) * blockSize;
  minZ = ((double)z - 0.5) * width;
  vertices = new TerrVertex[(unsigned int)size];
  generator=eq;
  Calculate();
  Upload();
 }
 void Calculate() {
  if ( !generator ) { Empty(); return; }
  float *paddedHeights = new float[(unsigned int) pDensitySize]; 
  float *pDest = paddedHeights; 
  TerrVertex *dest = vertices;
  for (int z = -1; z <= density; z++) {
  	generator->z = (double) minZ + z * w;
  	for (int x = -1; x <= density; x++) {
  		generator->x = (double) minX + x * w;
    generator->Calculation();
  		float h = (float) generator->out; 
  		*pDest++ = h; 
  		if (x >= 0 && x < density && z >= 0 && z < density){
  			dest->vertex.x=(float) generator->x;
     dest->vertex.y=h;
     dest->vertex.z=(float) generator->z;
  			dest->s = (float) tRepeat * iratiof(x,density - 1);
  			dest->t = (float) tRepeat * iratiof(z,density - 1);
  			dest++;
  		}
  	}
  }
  FindNormals(paddedHeights);
  delete[] paddedHeights;
  Pad();
 }
 void Empty() {
  Cartesiand generator;
  float *paddedHeights = new float[(unsigned int) pDensitySize]; 
  float *pDest = paddedHeights; 
  TerrVertex *dest = vertices;
  for (int z = -1; z <= density; z++) {
  	generator.z = (double) minZ + z * w;
  	for (int x = -1; x <= density; x++) {
  		generator.x = (double) minX + x * w;
    generator.y = 0.0;
  		float h = (float) generator.y; 
  		*pDest++ = h; 
  		if (x >= 0 && x < density && z >= 0 && z < density){
  			dest->vertex.x=(float) generator.x;
     dest->vertex.y=h;
     dest->vertex.z=(float) generator.z;
  			dest->s = (float) tRepeat * iratiof(x,density - 1);
  			dest->t = (float) tRepeat * iratiof(z,density - 1);
  			dest++;
  		}
  	}
  }
  FindNormals(paddedHeights);
  delete[] paddedHeights;
  Pad();
 }
 // Apply Sobel filter to find mesh normals
 void FindNormals( float *paddedHeights ) {
  TerrVertex *dest = vertices;
  w *= w;
  int i, j;
  for (j = 1; j < pDensity-1; j++){
  	for (i = 1; i < pDensity-1; i++){
  		dest->normal.x =
  			     paddedHeights[(j - 1) * pDensity + (i - 1)]  -
  			     paddedHeights[(j - 1) * pDensity + (i + 1)]  +
  			2 * (paddedHeights[ j      * pDensity + (i - 1)]  -
  			     paddedHeights[ j      * pDensity + (i + 1)]) +
  				 paddedHeights[(j + 1) * pDensity + (i - 1)]  -
  				 paddedHeights[(j + 1) * pDensity + (i + 1)];
  		dest->normal.y = (float) w;
  		dest->normal.z =
  			     paddedHeights[(j - 1) * pDensity + (i - 1)]  -
  			     paddedHeights[(j + 1) * pDensity + (i - 1)]  +
  			2 * (paddedHeights[(j - 1) * pDensity +  i     ]  -
  			     paddedHeights[(j + 1) * pDensity +  i     ]) +
  				 paddedHeights[(j - 1) * pDensity + (i + 1)]  -
  				 paddedHeights[(j + 1) * pDensity + (i + 1)];
  		dest->normal.fastNormalize();
  		dest++;
  	}
  }
 }
 void Pad() { 
  TerrVertex *newVertices = new TerrVertex[(unsigned int) size];
  for (int j = 0; j < density; j++)	for (int i = 0; i < density; i++) {
 		int index = mIndex2DPadded(j, i, density - 1);
 		newVertices[index] = vertices[j * density + i];
  }
  delete vertices;
  vertices = newVertices; 
 }
 void Upload() {
  if ( uploaded ) {
   OUTPUT("Replacing VBO %d with ", (int) vbo);
   glDeleteBuffers(1, &vbo);
  }
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, ((unsigned int) size) * element_size, vertices, GL_STATIC_DRAW);
//  OUTPUT("VBlock::Upload() VBO %d for %d,%d\n", (int) vbo, (int) x, (int) z );
  uploaded=true;
 }
 void Clear() {
  if ( vertices ) {
   delete vertices;
   vertices=null;
  }
  if ( uploaded ) {
//   OUTPUT("VBlock::Clear() disposing VBO %d for %d,%d\n", (int) vbo, (int) x, (int) z );
   glDeleteBuffers(1,&vbo);
   uploaded=false;
  }
 }
 ~VBlock() {
  if ( vertices ) delete vertices;
  if ( uploaded ) {
//   OUTPUT("~VBlock() disposing VBO %d for %d,%d\n", (int) vbo, (int) x, (int) z );
   glDeleteBuffers(1, &vbo);
  }
 }
};

class VBlockRequest : public ListItem {
public:
 Zp<VBlock> block;
 VBlockRequest( VBlock *block ) { this->block=block; }
};

class VBlockRequests : public LinkedList {
public:
 void Add( VBlock *block ) {
  Append(new VBlockRequest(block));
 }
 VBlockRequest *Find(int x, int z) {
  FOREACH(VBlockRequest,r) if ( x == r->block->x && z == r->block->z ) return r;
  return null;
 }
};

class VBlocks : public LinkedList {
public:
 Zdouble blockSize;
 Zuint blockDensity;
 Zint texRepeat;
 Zuint maxBlocks;
 Vertexd position;
 Zdouble viewDistance;
 Zdouble lodFactor;
 Set <unsigned int *> indices;
 Set <unsigned int> indexBuffers;
 VBlock *New( int x, int z, InfiniteTerrainEquations *eq, bool calculate ) {
  VBlock *b=new VBlock(x,z,blockDensity,blockSize,texRepeat,eq,calculate);
  Append(b);
  return b;
 }
 VBlock *New( int x, int z, InfiniteTerrainEquations *eq ) {
  VBlock *b=new VBlock(x,z,blockDensity,blockSize,texRepeat,eq);
  Append(b);
  return b;
 }
 void Add( VBlock *v ) {
  Append(v);
 }
 VBlock *FindBlock(int x, int z) {
  FOREACH(VBlock,v) if ( v->x == x && v->z == z ) return v;
  return null;
 } 
 double blockDistance(const int x, const int z, const double px, const double pz ) {
  // Find the square of the distance from the sphere to the box
  double d = 0;
  double minX = ((double)x - 0.5) * blockSize;
  double maxX = ((double)x + 0.5) * blockSize;
  double minZ = ((double)z - 0.5) * blockSize;
  double maxZ = ((double)z + 0.5) * blockSize;
  if (px < minX) d += SQ(px - minX);
  else if (px > maxX) d += SQ(px - maxX);
  if (pz < minZ) d += SQ(pz - minZ);
  else if (pz > maxZ) d += SQ(pz - maxZ);
  return sqrt(d);
 }
 int levelSize(int size, int level) { return ((size - 1) >> level) + 1; }
 int getIndex(unsigned int index, unsigned int size) {
  unsigned int r = index / size;
  unsigned int c = index % size;
  return mIndex2DPadded(r, c, size - 1);
 } 
 unsigned int *generateIndexArray(const int size, const int level){
  int lSize = levelSize(size, level);
  unsigned int *dest, *indexArray = new unsigned int[(lSize - 1) * (lSize * 2 + 1)];
  dest = indexArray;
  int base = 0;
  int step = 1 << level;
  for (int j = 1; j < lSize; j++) {
   if (j & 1) {
    for (int i = 0; i < size; i += step){
     *dest++ = getIndex(base + i, size);
     *dest++ = getIndex(base + i + size * step, size);
    }
    *dest++ = getIndex(base + size - 1 + size * step, size);
   } else {
    for (int i = size - 1; i >= 0; i -= step) {
     *dest++ = getIndex(base + i, size);
     *dest++ = getIndex(base + i + size * step, size);
    }
    *dest++ = getIndex(base + size * step, size);
   }
   base += size * step;
  }
  return indexArray;
 }
 void GenerateIndexArrays() {
  int level = 0;
  do {
   unsigned int *indexArray = generateIndexArray(blockDensity, level);
   indices.add(indexArray);
   GLuint indexBuffer=0;
   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   int lSize = levelSize(blockDensity, level);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * (lSize - 1) * (lSize * 2 + 1), indexArray, GL_STATIC_DRAW);
   indexBuffers.add(indexBuffer);
   level++;
  } while (levelSize(blockDensity, level) > 8);
 }
 void Render( VBlock *block, double d ) {
  glBindBuffer(GL_ARRAY_BUFFER, block->vbo);
  glVertexPointer(3, GL_FLOAT, sizeof(TerrVertex), VBUFFER_OFFSET(0));
  glNormalPointer( GL_FLOAT, sizeof(TerrVertex), VBUFFER_OFFSET(sizeof(HVertex)));
  glTexCoordPointer(2, GL_FLOAT, sizeof(TerrVertex), VBUFFER_OFFSET(sizeof(HVertex)*2));
  unsigned int level = (int) (lodFactor * d);
  if (level >= indices.getSize()) level = indices.getSize() - 1;
  int lSize = levelSize(blockDensity, level);
  int nIndices = (lSize - 1) * (lSize * 2 + 1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[level]);
  glDrawElements(GL_TRIANGLE_STRIP, nIndices, GL_UNSIGNED_INT, VBUFFER_OFFSET(0));
 }
 void PopFarthest( HVertex *position ) {
  // Toss yesterday's culling
  while (count > (int)maxBlocks){
   Zdouble maxDist;
   VBlock *candidate=(VBlock *)first;
   FOREACH(VBlock,block) {
    double d = SQ(position->x - blockSize * block->x) + SQ(position->z - blockSize * block->z);
    if (d > maxDist) {
     maxDist = d;
     candidate=block;
    }
   }
   Remove(candidate);
   delete candidate;
  }
 }
 void PopFarthest() {
  // Toss yesterday's culling
  while (count > (int)maxBlocks){
   Zdouble maxDist;
   VBlock *candidate=(VBlock *)first;
   FOREACH(VBlock,block) {
    double d = SQ(position.x - blockSize * block->x) + SQ(position.z - blockSize * block->z);
    if (d > maxDist) {
     maxDist = d;
     candidate=block;
    }
   }
   Remove(candidate);
   delete candidate;
  }
 }
 CLEARLISTRESET(VBlock);
 ~VBlocks() {
  Clear();
  unsigned int i, len = indices.getSize();
  for (i = 0; i < len; i++) delete indices[i];
 }
};

class InfiniteTerrain;
class InfiniteTerrainThread : public pThread {
public:
 Zpointer<InfiniteTerrain> it;
 VBlockRequests requests;
 InfiniteTerrainThread( InfiniteTerrain *it );
 bool Unlocked();
 bool Locked();
 void Main();
};

class InfiniteTerrain {
public:
 VBlocks blocks;
 VBlock *GenerateBlock(int x, int z);
 VBlock *RequestBlock(int x, int z);
 Zbool threaded,threadComplete;
 InfiniteTerrainEquations generator;
 Zp<InfiniteTerrainThread> thread;
 InfiniteTerrain(bool threaded, double bSize, int bDensity, int blockTexRepeat, int mBlocks);
~InfiniteTerrain();
 void Render();
 void Render( HVertex *position );
};

class SimpleHCamera {
public:
 HMatrix projection,modelView;
 HVertex position;
 double Wx,Wy,Wz;
 double speed;
 SimpleHCamera() {
  position = HVertex(0.0,100.0,0.0);
  Wx=0;
  Wy=1.0;
  Wz=0;
  speed=256.0;
  projection.loadProjectionMatrix(90 * PIf / 180, 0.75f, 1, 4000);
 }
 void moveView(float dx, float dy, float dz ) {
  position+=HVertex(dx,dy,dz);
 }
 void rotateView(float wx, float wy, float wz) {
  Wx += wx;
  Wy += wy;
  Wz += wz;
 }
 virtual void Reset(){
  position = HVertex(0.0,0.0,0.0);
  Wx = 0;
  Wy = 0;
  Wz = 0;
 }
 void Controls() {
  float sqrLen;
  HVertex dir(0.0,0.0,0.0);
  HVertex dx(modelView[0], modelView[4], modelView[8]);
  HVertex dy(modelView[1], modelView[5], modelView[9]);
  HVertex dz(modelView[2], modelView[6], modelView[10]);
  if ( input.KeyDown(DX_LEFT) ) dir -= dx;
  if ( input.KeyDown(DX_RIGHT) ) dir += dx;
  if ( input.KeyDown(DX_DOWN) ) dir -= dz;
  if ( input.KeyDown(DX_UP) ) dir += dz;
  if ( input.KeyDown(DX_Q) ) dir -= dy;
  if ( input.KeyDown(DX_Z) ) dir += dy;
  if ( input.KeyDown(DX_BACKSPACE) ) Reset();
  if ((sqrLen = lengthSqr(dir)) != 0){
  	dir *= rsqrtf(sqrLen);
  }
  processMovement(position + FRAMETIME * speed * dir);
 }
 void processMovement( const HVertex &newPosition ) {
  position=newPosition;
 }
};

class InfiniteTerrainTest : public GLWindow {
public:
 Zp<InfiniteTerrain> terrain;
 Zp<GLImage> ground;
 ImmediateLighting lights;
 SimpleHCamera hcamera;
 Crayon fog;
 Zbool foggy;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  terrain = new InfiniteTerrain( false, 2048.0f, 128, 2, 512);
  ground=library.Load("data/images/textures/odd/copperball.png",tllto_Tiling);
  lights.light[7].position.Set( -1.0f, 0.5f, 0.2f, 0.0f );
  lights.light[7].on=true;
  foggy=true;
  fog.Int(58, 68, 184,255);
 }
 void Between() {
  if ( input->KeyDown(DX_T) ) {
   hcamera.position.x=-1000;
   hcamera.position.z=-1000;
  }
  if ( input->KeyDown(DX_SPACE) ) foggy=!foggy;
  hcamera.Controls();
  hcamera.Wy=iratiof(input->mxi,display->w)*TWO_PIf;
  hcamera.Wx=iratiof(input->myi,display->h)*TWO_PIf;
 }
 void WheelUp() {
  hcamera.speed+=128;
 }
 void WheelDown() {
  hcamera.speed-=128;
 }
 void Render() {
  glPush();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(hcamera.projection);
  hcamera.modelView.loadRotateZXY(-(float)hcamera.Wx, -(float)hcamera.Wy, -(float)hcamera.Wz);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(hcamera.modelView);
  glClearColor(fog.rf,fog.gf,fog.bf,fog.af);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
 	glDepthFunc(GL_LEQUAL);
  glDisable(GL_DEPTH_TEST);
  hcamera.modelView.translate(-hcamera.position);
  glLoadMatrixf(hcamera.modelView);
  // <- Draw Skybox...
  glEnable(GL_DEPTH_TEST);
  Blending(transparency);
  if ( foggy ) {
   glFogfv(GL_FOG_COLOR, fog.floats);
   glFogi(GL_FOG_MODE, GL_EXP2);
   glFogf(GL_FOG_DENSITY, 0.0005f);
//   glFogf(GL_FOG_START, 1);
    //glFogf(GL_FOG_END, viewDistance);
   glEnable(GL_FOG);
  }
  Blending(none);
  lights.On();
//  float lightDir[] = {  };
//  glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,ground->texture);
	/*renderer->changeTextureUnit(0);
	glMatrixMode(GL_TEXTURE);
	float mat[16] = {
		0.005f, 0,      0, 0,
		0,      0,      0, 0,
		0,      0.005f, 0, 0,
		0,      0,      0, 1
	};
	glLoadMatrixf(mat);
	*/
  terrain->Render( &hcamera.position ); 
  //glLoadIdentity();
//  glDisable(GL_FOG);
  glDisable(GL_TEXTURE_2D);
  lights.Off();
  glPop();
  FORMAT(buf,64,"Pos: %1.2f  %1.2f  %1.2f\nRot: %1.2f  %1.2f  %1.2f",
   (float) hcamera.position.x,
   (float) hcamera.position.y,
   (float) hcamera.position.z,
   (float) hcamera.Wx,
   (float) hcamera.Wy,
   (float) hcamera.Wz
  );
  Text(buf,crayons.Pick(black),10,20,8,8,2,1);
  Text(buf,crayons.jami(0,255,255,0,255),10+1,20-1,8,8,2,1);
 }

};

