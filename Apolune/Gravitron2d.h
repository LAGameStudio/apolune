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

#include <Windows.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"
#include "Particle2d.h"
#include "TextureLibrary.h"
#include "Tweens.h"

class Gravitronf2d : public Particlef2d
{
public:
 Crayon color;
 Zint ox,oy;
 Zfloat x, y, px, py, magnitude, angle, mass, intensity;
 Zint index,length;
 Zdouble lifespan,totalLifespan;
 Zp<GLImage> particle;
 Zp<Tween> t;
 
 Gravitronf2d(void) : Particlef2d() { Init(); }
 ~Gravitronf2d(void) {}

 void Init() {
  ox=oy=0;
  x=y=px=py=magnitude=angle=intensity=0.0f;
  mass=0.1f;
  index=length=0;
  dead=false;
  particle=library.Load("data/images/flares/purple.png");
  t=tweens.find("Humanized Fast Rise Sustain Slow Fade");
 }
  
 void Set( float dx, float dy, float V, float A, float M ) {
  x = dx;
  y = dy;
  px = dx;
  py = dy;
  magnitude = V;
  angle = A;
  mass = M;
 }

 float findAngle( float x, float y ) {
  float theta;
  if (x == 0) {
   if (y > 0) theta = HALF_PIf;
   else if (y < 0) theta = 3*HALF_PIf;
   else theta = 0;
  }
  else {
   theta = atan( y / x );
   if ( (x<0) && (y>=0) ) theta += PIf;
   if ( (x<0) && (y< 0) ) theta -= PIf;
  }
  return theta;
 }
 
 void Gravitate( Gravitronf2d *Z ) {
  float F, mX, mY, A;
  if ( SQ( x - Z->x ) + SQ( y - Z->y ) != 0 ) {
   F = mass * Z->mass;
   mX = ( mass * x + Z->mass * Z->x ) / ( mass + Z->mass );
   mY = ( mass * y + Z->mass * Z->y ) / ( mass + Z->mass );
   A = findAngle( mX - x, mY - y );
   mX = F * cosf(A);
   mY = F * sinf(A);  
   mX += magnitude * cosf(angle);
   mY += magnitude * sinf(angle);    
   magnitude = sqrtf( SQ(mX) + SQ(mY) );
   angle = findAngle( mX, mY );
  }
 }
 
 void Repel( Gravitronf2d *Z ) {
  float F, mX, mY, A;
  if ( SQ( x - Z->x ) + SQ( y - Z->y ) != 0 ) {
   F = mass * Z->mass;
   mX = ( mass * x + Z->mass * Z->x ) / ( mass + Z->mass );
   mY = ( mass * y + Z->mass * Z->y ) / ( mass + Z->mass );
   A = findAngle( x - mX, y - mY );    
   mX = F * cosf(A);
   mY = F * sinf(A);  
   mX += magnitude * cosf(angle);
   mY += magnitude * sinf(angle);
   magnitude = sqrtf( SQ(mX) + SQ(mY) );
   angle = findAngle( mX, mY );
  }
 }
  
 inline void Deteriorate() {
  magnitude *= 0.925f;
 }
  
 void Between() {
  x += magnitude * cosf(angle);
  y += magnitude * sinf(angle);    
  intensity+=float_range(0.0f,0.01f);
  if( intensity > 1.0f ) intensity=0.0f;
  float r = iratiof(index,length);
  color.Float(intensity, pow(r,0.1f), 1.0f-r, 0.015f);
  lifespan-=FRAMETIME;
 }
  
 virtual void Render( GLWindow *surface ) {
  DrawLine2Color(surface,crayons.Pick(clear),color,ox+(int) px,oy+(int) py,ox+(int) x,oy+(int) y);
  px = x;
  py = y;
 }

 virtual void Render() {
  Cartesiand c;
  c.SetCorners(ox+(int) px,oy+(int) py,ox+(int) x,oy+(int) y);
  Crayon cray=crayons.Pick(alabaster);
  cray.ScaleByAlphaf(t->tweenf(lifespan/totalLifespan));
  TextureLine(particle,cray,additive,&c,8.0);
  if ( next ){
   Gravitronf2d *pn=(Gravitronf2d *)next;
   Cartesiand c;
   c.SetCorners(ox+(int) x,oy+(int) y,ox+(int)pn->x,oy+(int)pn->y);
   Crayon cray=crayons.Pick(alabaster);
   cray.ScaleByAlphaf(t->tweenf(lifespan/totalLifespan));
   TextureLine(particle,cray,additive,&c,2.0);
  }
  px = x;
  py = y;
 }

};

class Gravitronf2ds : public LinkedList {
public:
 CLEARLISTRESET(Gravitronf2d);
 ~Gravitronf2ds() { Clear(); }
};

class GravitronEmitterf2d : public Emitter2d {
public:
 Zbool repel;
 Gravitronf2d gravitant;
 Gravitronf2d repellent;
 Gravitronf2ds particles;
 GravitronEmitterf2d() : Emitter2d() {}
 ~GravitronEmitterf2d() { }
 virtual void Create( int size, int w, int h ) {
  Cartesian c;
  int w2=w/2,h2=h/2;
  float radius=(float) (w2+h2/2)/2;
  float w2f=(float) (w2), h2f=(float) (h2);
  for(int i = 0; i < size; i++) {
   Gravitronf2d *Z=new Gravitronf2d;
   c.PointDenseCentered(radius);
   Z->Set( c.fx, c.fy, 0.0f, 0.0f, 1.0f );
   Z->ox=w2;
   Z->oy=h2;
   Z->index=i;
   Z->length=size;
   Z->totalLifespan=3.0*uniform()+2.0;
   Z->lifespan=Z->totalLifespan;
   particles.Append(Z);
  }
 }
 void Between() {
  if ( repel )
  EACHN(particles.first,Gravitronf2d,f, {
   f->Repel( &repellent );
   f->Between();
   if ( f->lifespan < 0.0 ) { particles.Remove(f); delete f; }
  }) else EACHN(particles.first,Gravitronf2d,f, {
   f->Gravitate( &gravitant );
   f->Between();
   if ( f->lifespan < 0.0 ) { particles.Remove(f); delete f; }
  });
 }
 void Render( GLWindow *surface ) {
  EACH(particles.first,Gravitronf2d,f) f->Render(surface);
 }
 void Render() {
  EACH(particles.first,Gravitronf2d,f) f->Render();
 }
};

/*
class GravitronEmittersf2d : LinkedList {
public:
 CLEARLIST(GravitronEmitterf2d);
 void Render( GLWindow *surface ) {
  FOREACH(GravitronEmitterf2d,g) g->Render(surface);
 }
 ~GravitronEmittersf2d() { Clear(); }
};
 Now appearing as
 */

RENDERLIST(GravitronEmittersf2d,GravitronEmitterf2d);