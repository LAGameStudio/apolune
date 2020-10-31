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
#include <iostream>
#include <string>
using namespace std;

#include "moremath.h"
#include "ListItem.h"
#include "Crayon.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "DisplayList.h"
#include "Indexed.h"

class Tween : public ListItem
{
public:
 Indexed<Zfloat> tween;
 string name;
 Zfloat tindex;
 Zbool Constant,Generative;

 Tween(void);
 Tween( const char *name, int length );
 Tween( string name, int length );
 Tween( const char *name, float *tween, size_t length ); // Takes a constant as input
 Tween( const char *name, float *tween, size_t length, bool reverse ); // Reverses a constant as input
 Tween( Tween *t ) {
  name=t->name;
  tween.Size(t->tween.length);
  for ( unsigned int k=0; k<tween.length; k++ ) tween[k]=t->tween[k];
 }

 // Create a display list for this tween
 DisplayList render;
 void PreRender() {
  render.StartRecording();
  glBegin(GL_LINES);
  glVertex2f(0.0f,0.5f);
  for ( int i=(int)tween.length-1; i>=0; i-=2 ) {
   GLfloat fx=(GLfloat) ((double) i/1024.0);
   glVertex2f(fx, 0.5f);
   glVertex2f(fx, (-tween[i]*0.5f)+0.5f);
   glVertex2f(fx, 0.5f);
  }
  glEnd();
  render.StopRecording();
 }
 virtual void Generate(void);
 virtual float uncertainty( unsigned int p, float f );
 float Time() { return tweenf(iratiof((int)(TICK%1000),1000)); }
 float Time( int offset ) { return tweenf(iratiof((int)(TICK%1000),1000)); }
 float Time( having modulo ) { return tweenf(iratiof((int)(TICK%1000),1000),true); }
 float Time( int offset, having modulo ) { return tweenf(iratiof((int)(TICK%1000),1000),true); }
 void generate( unsigned int L );
 void Render( int x, int y, int w, int h, Crayon color );
 // return a value if a is within 0.0f-1.0f
 inline float tweenf( float a ) {
  float index = a*(float) tween.length;
  //if ( !tween ) return 1.0f;//a; // figure out a way around this?
 // OUTPUT( "%f:%f, ", index, tween[(int) index] );
  int idx=(int) index;
  while (idx < 0) idx += (int)tween.length;
  while (idx >= (int)tween.length) idx -= (int)tween.length;
  return tween[(unsigned int) idx];
 }
 inline float tweenf( double a ) { return tweenf((float)a); }
 inline float tweend( double a ) {
  double index = a*(double) tween.length;
  //if ( !tween ) return 1.0f;//a; // figure out a way around this?
 // OUTPUT( "%f:%f, ", index, tween[(int) index] );
  int idx=(int) index;
  while (idx < 0) idx += (int)tween.length;
  while (idx >= (int)tween.length) idx -= (int)tween.length;
  return tween[(unsigned int) idx];
 }
 inline float tweenf( float a, bool modulo ) {
  float index = a*(float) tween.length;
  //if ( !tween ) return 1.0f;//a; // figure out a way around this?
 // OUTPUT( "%f:%f, ", index, tween[(int) index] );
  int idx=(int) index % tween.length;
  if ( idx < 0 ) idx=abs(idx);
  return tween[(unsigned int) idx];
 }
 inline float tweend( double a, bool modulo ) {
  double index = a*(double) tween.length;
  //if ( !tween ) return 1.0f;//a; // figure out a way around this?
 // OUTPUT( "%f:%f, ", index, tween[(int) index] );
  int idx=(int) index % tween.length;
  if ( idx < 0 ) idx=abs(idx);
  return tween[(unsigned int) idx];
 }
 // Must be of the same length!
 // Merges to create new tween size of i
 void Multiply( string *newname, Tween *i, Tween *j ) {
  name=newname->c_str();
  tween.Size(i->tween.length);
  for (unsigned int k = 0; k<tween.length; k++) tween[k] = i->tween[k] * j->tween[(int)(iratiof(k, (int)(tween.length))*(int)tween.length)];
 }
 // Merges to create new tween size of i
 void Add( string *newname, Tween *i, Tween *j ) {
  name=newname->c_str();
  tween.Size(i->tween.length);
  for (unsigned int k = 0; k<tween.length; k++) tween[k] = i->tween[k] + (float) j->tween[(int)(iratiof(k, (int)(tween.length))*(int)tween.length)];
 }
 // Merges to create new tween size of i
 void Subtract( string *newname, Tween *i, Tween *j ) {
  name=newname->c_str();
  tween.Size(i->tween.length);
  for (unsigned int k = 0; k<tween.length; k++) tween[k] = i->tween[k] - j->tween[(int)(iratiof(k, (int)(tween.length))*(int)tween.length)];
 }
 // Merges to create new tween size of i
 void Average( string *newname, Tween *i, Tween *j ) {
  name=newname->c_str();
  tween.Size(i->tween.length);
  for (unsigned int k = 0; k<tween.length; k++) tween[k] = (i->tween[k] + (float) j->tween[(int)(iratiof(k, (int)(tween.length))*(int)tween.length)]) / 2.0f;
 }
 void Normalize( float ceiling ) {
  for ( unsigned int k=0; k<tween.length; k++ ) tween[k]/=ceiling;
 }
 /*
 Source: http://iphonedevelopment.blogspot.com/2010/12/more-animation-curves-than-you-can.html
 #include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <math.h>

#define BoundsCheck(t, start, end) \
if (t <= 0.f) return start;        \
else if (t >= 1.f) return end;

GLfloat LinearInterpolation(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return t * end + (1.f - t) * start;
}
#pragma mark -
#pragma mark Quadratic
GLfloat QuadraticEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return   -end * t * (t - 2.f) -1.f;
}
GLfloat QuadraticEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * t * t + start - 1.f;
}
GLfloat QuadraticEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.f;
    if (t < 1.f) return end/2.f * t * t + start - 1.f;
    t--;
    return -end/2.f * (t*(t-2) - 1) + start - 1.f;
}
#pragma mark -
#pragma mark Cubic
GLfloat CubicEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t--;
    return end*(t * t * t + 1.f) + start - 1.f;
}
GLfloat CubicEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * t * t * t+ start - 1.f;
}
GLfloat CubicEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.;
    if (t < 1.) return end/2 * t * t * t + start - 1.f;
    t -= 2;
    return end/2*(t * t * t + 2) + start - 1.f;
}
#pragma mark -
#pragma mark Quintic
GLfloat QuarticEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t--;
    return -end * (t * t * t * t - 1) + start - 1.f;
}
GLfloat QuarticEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * t * t * t * t + start;
}
GLfloat QuarticEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.f;
    if (t < 1.f) 
        return end/2.f * t * t * t * t + start - 1.f;
    t -= 2.f;
    return -end/2.f * (t * t * t * t - 2.f) + start - 1.f;
}
#pragma mark -
#pragma mark Quintic
GLfloat QuinticEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t--;
    return end * (t * t * t * t * t + 1) + start - 1.f;
}
GLfloat QuinticEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * t * t * t * t * t + start - 1.f;
}
GLfloat QuinticEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.f;
    if (t < 1.f) 
        return end/2 * t * t * t * t * t + start - 1.f;
    t -= 2;
    return end/2 * ( t * t * t * t * t + 2) + start - 1.f;
}
#pragma mark -
#pragma mark Sinusoidal
GLfloat SinusoidalEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * sinf(t * (M_PI/2)) + start - 1.f;
}
GLfloat SinusoidalEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return -end * cosf(t * (M_PI/2)) + end + start - 1.f;
}
GLfloat SinusoidalEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return -end/2.f * (cosf(M_PI*t) - 1.f) + start - 1.f;
}
#pragma mark -
#pragma mark Exponential
GLfloat ExponentialEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * (-powf(2.f, -10.f * t) + 1.f ) + start - 1.f;
}
GLfloat ExponentialEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return end * powf(2.f, 10.f * (t - 1.f) ) + start - 1.f;
}
GLfloat ExponentialEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.f;
    if (t < 1.f) 
        return end/2.f * powf(2.f, 10.f * (t - 1.f) ) + start - 1.f;
    t--;
    return end/2.f * ( -powf(2.f, -10.f * t) + 2.f ) + start - 1.f;
}
#pragma mark -
#pragma mark Circular
GLfloat CircularEaseOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t--;
    return end * sqrtf(1.f - t * t) + start - 1.f;
}
GLfloat CircularEaseIn(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return -end * (sqrtf(1.f - t * t) - 1.f) + start - 1.f;
}
GLfloat CircularEaseInOut(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    t *= 2.f;
    if (t < 1.f) 
        return -end/2.f * (sqrtf(1.f - t * t) - 1.f) + start - 1.f;
    t -= 2.f;
    return end/2.f * (sqrtf(1.f - t * t) + 1.f) + start - 1.f;
}
*/
};

