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
#include <math.h>
#include "macros.h"
#include "moremath.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"
#include "BinaryFile.h"

class Vertex;
class Cartesiand;
class Box;
class Line;
class Vertices;

class VariableStore;

ONE(Cartesian,{Zero();})
 int x, y, z, w, h, x2, y2;
 float fx, fy, fz, fw, fh,a;
 double Xd,Yd; // Used to speed up casting-related code when double values are needed
 void Zero() {
  x=y=z=w=h=x2=y2=0;
  fx=fy=fz=fw=fh=a=0.0f;
  Xd=0.0; Yd=0.0;
 }
 Cartesian( float ffx, float ffy );
 Cartesian( const Zfloat& ix, const Zfloat& iy );
 Cartesian( int ix, int iy );
 Cartesian( int ix, int iy, int iw, int ih );
 Cartesian( const Zint& ix, const Zint& iy );
 Cartesian( Cartesian *c ) { Set(c); }
 BINARY_IN({
  B_IN_TXT(name)
  B_IN_POD(a)  B_IN_POD(x)  B_IN_POD(y)  B_IN_POD(z)
  B_IN_POD(w)  B_IN_POD(h)  B_IN_POD(Xd) B_IN_POD(x2)
  B_IN_POD(y2) B_IN_POD(Yd) B_IN_POD(fx) B_IN_POD(fy)
  B_IN_POD(fz) B_IN_POD(fw) B_IN_POD(fh)
 })
 BINARY_OUT({
  B_OUT_TXT(name)
  B_OUT_POD(a)  B_OUT_POD(x)  B_OUT_POD(y)  B_OUT_POD(z)
  B_OUT_POD(w)  B_OUT_POD(h)  B_OUT_POD(Xd) B_OUT_POD(x2)
  B_OUT_POD(y2) B_OUT_POD(Yd) B_OUT_POD(fx) B_OUT_POD(fy)
  B_OUT_POD(fz) B_OUT_POD(fw) B_OUT_POD(fh)
 })
 // Extra stuff for smart fromString
 Zint interp_value; // Lets us load x,y,z as 0 1 2 instead of x 0 y 1 z 2
 void SetInterpi( const char *s ) {
  switch ( interp_value.value ) {
   case 0: this->x=atoi(s); break;
   case 1: this->y=atoi(s); this->Set(this->x,this->y); break;
   case 2: this->w=atoi(s); break;
   case 3: this->h=atoi(s); this->SetRectf(this->x,this->y,this->w,this->h); break;
   case 4: this->a=(float)atof(s); break;
   default: break;
  }
 }
 void SetInterpf( const char *s ) {
  switch ( interp_value.value ) {
   case 0: this->fx=(float)atof(s); break;
   case 1: this->fy=(float)atof(s); this->Set(this->fx,this->fy); break;
   case 2: this->fw=(float)atof(s); break;
   case 3: this->fh=(float)atof(s); this->SetRectf(this->fx,this->fy,this->fw,this->fh); break;
   case 4: this->a=(float)atof(s); break;
   default: break;
  }
 }
 void fromString( const char *s, VariableStore *vs );
 SKEYSTART
  if ( is_integer(key.c_str()) ) {
   SetInterpi(key.c_str());
   interp_value+=1;
  } else if ( is_decimal(key.c_str()) ) {
   SetInterpf(key.c_str());
   interp_value+=1;
  } else
  SKEY("x",  { SPARAM(x, int,atoi); })
  SKEY("y",  { SPARAM(y, int,atoi); })
  SKEY("z",  { SPARAM(z, int,atoi); })
  SKEY("w",  { SPARAM(w, int,atoi); })
  SKEY("h",  { SPARAM(h, int,atoi); })
  SKEY("x2", { SPARAM(x2,int,atoi); })
  SKEY("y2", { SPARAM(y2,int,atoi); })
  SKEY("fx", { SPARAM(fx,float,atof); })
  SKEY("fy", { SPARAM(fy,float,atof); })
  SKEY("fz", { SPARAM(fz,float,atof); })
  SKEY("fw", { SPARAM(fw,float,atof); })
  SKEY("fh", { SPARAM(fh,float,atof); })
  SKEY("a",  { SPARAM(a, float,atof); })
 SKEYENDING("Cartesian")
  interp_value=0;
 SKEYENDER;
 string toString() {
  string out=string("");
  char buf[80];
  if ( x  != 0 )    { FORMAT(buf,80,"%d ",(int) x);  out+=string("x ")+string(buf); }
  if ( y  != 0 )    { FORMAT(buf,80,"%d ",(int) y);  out+=string("y ")+string(buf); }
  if ( z  != 0 )    { FORMAT(buf,80,"%d ",(int) z);  out+=string("z ")+string(buf); }
  if ( w  != 0 )    { FORMAT(buf,80,"%d ",(int) w);  out+=string("w ")+string(buf); }
  if ( h  != 0 )    { FORMAT(buf,80,"%d ",(int) h);  out+=string("h ")+string(buf); }
  if ( x2 != 0 )    { FORMAT(buf,80,"%d ",(int) x2); out+=string("x2 ")+string(buf); }
  if ( y2 != 0 )    { FORMAT(buf,80,"%d ",(int) y2); out+=string("y2 ")+string(buf); }
  if ( fx != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) fx); out+=string("fx ")+string(buf); }
  if ( fy != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) fy); out+=string("fy ")+string(buf); }
  if ( fz != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) fz); out+=string("fz ")+string(buf); }
  if ( fw != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) fw); out+=string("fw ")+string(buf); }
  if ( fh != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) fh); out+=string("fh ")+string(buf); }
  if ( a  != 0.0f ) { FORMAT(buf,80,"%1.5f ",(float) a);  out+=string("a ")+string(buf); }
  return out;
 }
 Cartesian *Copy() {
  Cartesian *c=new Cartesian;
  c->x=x; c->y=y; c->z=z; c->w=w; c->h=h; c->x2=x2; c->y2=y2; 
  c->fx=fx; c->fy=fy; c->fz=fz; c->fw=fw; c->fh=fh; c->a=a;
  return c;
 }
 Cartesian *Duplicate() {
  Cartesian *c=new Cartesian;
  c->x=x; c->y=y; c->z=z; c->w=w; c->h=h; c->x2=x2; c->y2=y2; 
  c->fx=fx; c->fy=fy; c->fz=fz; c->fw=fw; c->fh=fh; c->a=a;
  return c;
 }
 void Duplicate(Cartesian *c) {
  x=c->x;
  y=c->y;
  z=c->z;
  w=c->w;
  h=c->h;
  x2=c->x2;
  y2=c->y2;
  fx=c->fx;
  fy=c->fy;
  fz=c->fz;
  fw=c->fw;
  fh=c->fh;
  a=c->a;
 }
 void Set(Cartesian *c ) {
  x=c->x;
  y=c->y;
  z=c->z;
  w=c->w;
  h=c->h;
  x2=c->x2;
  y2=c->y2;
  fx=c->fx;
  fy=c->fy;
  fz=c->fz;
  fw=c->fw;
  fh=c->fh;
  a=c->a; 
 }
 void Set( int x, Zint y ) { Set(x,(int)y); }
 void Set( Zint x, int y ) { Set((int)x,y); }
 void Set( Zint x, Zint y ) { Set((int)x,(int)y); }
 void Set( int x, int y ) {
  this->x=x;
  this->fx=(float) x;
  this->y=y;
  this->fy=(float) y;
 }
 void Set( Zfloat x, float y ) { Set((float)x,y); }
 void Set( float x, Zfloat y ) { Set(x,(float)y); }
 void Set( Zfloat x, Zfloat y ) { Set((float)x,(float)y); }
 void Set( float x, float y ) {
  this->x=(int) x;
  this->fx=x;
  this->y=(int) y;
  this->fy=y;
 }
 void Seti( Zint x, int y ) { Seti((int)x,y); }
 void Seti( int x, Zint y ) { Seti(x,(int)y); }
 void Seti( Zint x, Zint y ) { Seti((int)x,(int)y); }
 void Seti( int x, int y ) {
  this->x=x;
  this->y=y;
 }
 void Setf( Zfloat x, float y ) { Setf((float)x,y); }
 void Setf( float x, Zfloat y ) { Setf(x,(float)y); }
 void Setf( Zfloat x, Zfloat y ) { Setf((float)x,(float)y); }
 void Setf( float x, float y ) {
  fx=x;
  fy=y;
 }
 void SetRectf( Zfloat X, Zfloat Y, Zfloat W, Zfloat H ) { SetRectf((float)X,(float)Y,(float)W,(float)H); }
 void SetRectf( float X, Zfloat Y, Zfloat W, Zfloat H ) { SetRectf(X,(float)Y,(float)W,(float)H); }
 void SetRectf( float X, float Y, Zfloat W, Zfloat H ) { SetRectf(X,Y,(float)W,(float)H); }
 void SetRectf( float X, float Y, float W, Zfloat H ) { SetRectf(X,Y,W,(float)H); }
 void SetRectf( float X, float Y, float W, float H ) {
  fx=X; fy=Y; fw=W; fh=H;
 }
 void SetRectf( int X, int Y, int W, int H ) {
  SetRect(X,Y,W,H);
  SetRectf((float)X,(float)Y,(float)W,(float)H);
 }
 void SetRect( int X, Zint Y, int W, Zint H ) { SetRect(X,(int)Y,W,(int)H); }
 void SetRect( Zint X, int Y, Zint W, int H ) { SetRect((int)X,Y,(int)W,H); }
 void SetRect( int X, Zint Y, int W, int H ) { SetRect(X,(int)Y,W,H); }
 void SetRect( Zint X, Zint Y, int W, Zint H ) { SetRect((int)X,(int)Y,W,(int)H); }
 void SetRect( Zint X, int Y, Zint W, Zint H ) { SetRect((int)X,Y,(int)W,(int)H); }
 void SetRect( Zint X, Zint Y, Zint W, Zint H ) { SetRect((int)X,(int)Y,(int)W,(int)H); }
 void SetRect( int X, Zint Y, Zint W, Zint H ) { SetRect(X,(int)Y,(int)W,(int)H); }
 void SetRect( int X, int Y, Zint W, Zint H ) { SetRect(X,Y,(int)W,(int)H); }
 void SetRect( int X, int Y, int W, Zint H ) { SetRect(X,Y,W,(int)H); }
 void SetRect( int X, int Y, int W, int H ) {
  x=X; y=Y; w=W; h=H; x2=X+W; y2=Y+H;
 }
 void SetRect( double X, double Y, double W, double H ) { SetRect((float)X,(float)Y,(float)W,(float)H); }
 void SetRect( float X, float Y, float W, float H ) {
  SetRectf(X,Y,W,H);
  SetRect((int)X,(int)Y,(int)W,(int)H);
 }
 void SetLine( int X, int Y, int X2, int Y2 ) { Corners(X,Y,X2,Y2); }
 // Compute the line angle in radians
 float LineAngle() {
  return (float) atan2( (float) (y2-y), (float) (x2-x) );
 }
 // Use fw,fh for x2,y2
 float LineAnglef() {
  return (float) atan2( fh-fy,fw-fx );
 }
 void Corners( int X, int Y, int X2, int Y2 ) {
  x=X; y=Y; x2=X2; y2=Y2; w=abs(X2-X); h=abs(Y2-Y); SetRectf(X,Y,this->w,this->h);
 }
 void Corners( int X, int Y, int X2, int Y2, int W, int H ) {
  x=X; y=Y; x2=X2; y2=Y2; w=W; h=H;
 }
 bool within( int X, int Y ) { return WITHIN(X,Y,x,y,x2,y2); }
 bool withinclusive( int X, int Y ) { return WITHINCLUSIVE(X,Y,x,y,x2,y2); }
 bool within( Cartesian *box ) {
  return (
     WITHIN(box->x,box->y,x,y,x2,y2)
  || WITHIN(box->x,box->y2,x,y,x2,y2)
  || WITHIN(box->x2,box->y2,x,y,x2,y2)
  || WITHIN(box->x2,box->y,x,y,x2,y2)
  );
 }
// bool within( float X, float Y ) { return (WITHIN(X,Y,fx,fy,(float)x2,(float)y2)); }
 void MoveBy( Zint X, Zint Y ) { MoveBy((int)X,(int)Y); }
 void MoveBy( int X, int Y ) {
  x+=X;
  y+=Y;
  x2+=X;
  y2+=Y;
 }
 void MoveByf( Zfloat X, Zfloat Y ) { MoveByf((float)X,(float)Y); }
 void MoveByf( float X, float Y ) {
  fx+=X;
  fy+=Y;
 }
 void MoveByd( double X, double Y ) {
  float fx=(float) (X+(double) Xd), fy=(float) (Y+(double) Yd);
  int ix=(int) fx, iy=(int) fy;
  x+=ix;
  y+=iy;
  x2+=ix;
  y2+=iy;
  Xd=(double) (fx-(float)ix);
  Yd=(double) (fy-(float)iy);
 }
 float Magnitude ()				// Calculate The Length Of The Vector
 {
  return sqrtf ((float) SQ(x)+(float) SQ(y)+(float) SQ(z));
 }
 void normalize()	{ // Creates A Vector With A Unit Length Of 1
  float M = Magnitude(); // Calculate The Length Of The Vector 
  if (M != 0.0f)			// Make Sure We Don't Divide By 0 
  {
   x = (int) ((float)x/M);		// Normalize The 3 Components 
   y = (int) ((float)y/M);
  }
 }
 void normalizef()	{ // Creates A Vector With A Unit Length Of 1
  float M = Magnitude(); // Calculate The Length Of The Vector 
  if (M != 0.0f)			// Make Sure We Don't Divide By 0 
  {
   fx /= M;		// Normalize The 3 Components 
   fy /= M;
  }
 }
 float Aspect() {
  return fw/fh;
 }
 float AspectInverse() {
  return fh/fw;
 }
 Cartesian RotateXYi  (Cartesian const &sourcePoint, float rotationAngleDegrees );
 Cartesian RotateXYf  (Cartesian const &sourcePoint, float rotationAngleDegrees );
 Cartesian RotateZYf  (Cartesian const &sourcePoint, float rotationAngleDegrees );
 Cartesian RotateXYZYf(Cartesian const &sourcePoint, float rotationAngleDegreesXY, float rotationAngleDegreesZY );
 float Distancef( Zfloat xx, Zfloat yy ) { return Distancef((float)xx,(float)yy); }
 float Distancef( float xx, float yy );
 float Distancei();
 float Distancei( Zint xx, Zint yy ) { return Distancei((int)xx,(int)yy); }
 float Distancei( int xx, int yy );
 float Distance( float xx, float yy );
 float Distance( int xx, int yy );
 void Lerp2d( Cartesian *a, Cartesian *b, double time );
 void Lerp( Cartesian *a, Cartesian *b, float L ) {
  x=(int)((float)a->x+((float)b->x-(float)a->x)*L);
  y=(int)((float)a->y+((float)b->y-(float)a->y)*L);
  z=(int)((float)a->z+((float)b->z-(float)a->z)*L);
 }
 bool Intersects( Cartesian *a );
 bool Intersects( Cartesiand *a );
 inline bool Intersects( Vertex *offset, Cartesian *a );
 inline bool TranslateIntersects( Vertex *offset1, Vertex *offset2, Cartesian *a );
 void PointInCircle( float radius ) {
  float q = (float) (uniform() * (PI * 2));
  float r = (float) sqrt(uniform());
  fx = (radius * r) * cosf(q);
  fy = (radius * r) * sinf(q);
  x=(int) fx;
  y=(int) fy;
 }
 void PointDenseCentered( float radius ) {
  float r = (float) uniform();
  float q = (float) (uniform() * (PI * 2));
  fx = (radius * r) * cosf(q);
  fy = (radius * r) * sinf(q);
  x=(int) fx;
  y=(int) fy;
 }
 void Print( const char *name ) {
  OUTPUT( "Cartesian `%s`:\n - integers: x,y,z: %d,%d,%d wxh: %dx%d x2,y2:%d,%d\n - floats: fx,fy,fz: %f,%f,%f fw,fh: %f,%f a: %f",
   name,
   x, y, z, w, h, x2, y2,
   fx, fy, fz, fw, fh,a
  );
 }
 bool OnScreen();
 
 // Code below - copied and put into C++ - original from Franklin Antonio.. thanks!
 // http://www.2d-cubed.com/2010/01/some-really-useful-collision-detection.html
 /* Faster Line Segment Intersection                                     */
 /* Franklin Antonio                                                     */
 /* The SAME_SIGNS macro assumes arithmetic where the exclusive-or    */
 /* operation will work on sign bits.  This works for twos-complement,*/
 /* and most other machine arithmetic.                                */
#define Cartesian_SAME_SIGNS( a, b ) (((long) ((unsigned long) a ^ (unsigned long) b)) >= 0 )
 /* The use of some short working variables allows this code to run   */
 /* faster on 16-bit computers, but is not essential.  It should not  */
 /* affect operation on 32-bit computers.  The short working variables*/
 /* to not restrict the range of valid input values, as these were    */
 /* constrained in any case, due to algorithm restrictions.           */
 bool LineLine( Cartesian *lineB ) { 
    int Ax,Bx,Cx,Ay,By,Cy,d,e,f;
    int x1lo,x1hi,y1lo,y1hi;
    Ax = x2-x;
    Bx = lineB->x-lineB->x2;
    // X bound box test 
    if(Ax<0) { x1lo=2; x1hi=x; }
    else     { x1hi=2; x1lo=x; }
    if(Bx>0) { if(x1hi < lineB->x2 || lineB->x < x1lo) return false; }
    else     { if(x1hi < lineB->x || lineB->x2 < x1lo) return false; }
    Ay = y2-y;    
    By = lineB->y-lineB->y2;    
    // Y bound box test
    if(Ay<0) { y1lo=y2; y1hi=y; } 
    else     { y1hi=y2; y1lo=y; }    
    if(By>0) { if(y1hi < lineB->y2 || lineB->y < y1lo) return false; } 
    else     { if(y1hi < lineB->y || lineB->y2 < y1lo) return false; }
    Cx = x-lineB->x;
    Cy = y-lineB->y;
    d = By*Cx - Bx*Cy;                    // alpha numerator
    f = Ay*Bx - Ax*By;                    // both denominator
    // alpha tests    
    if(f>0) { if(d < 0 || d > f) return false; } 
    else    { if(d > 0 || d < f) return false; }
    e = Ax*Cy - Ay*Cx;                    // beta numerator
    // beta tests
    if(f > 0) { if(e < 0 || e > f) return false; } 
    else      { if(e > 0 || e < f) return false; }
    if(f==0) return false; // CO-LINEAR!    - could return something other than false..
    return true; // if got here, then the lines do intersect.
 }
 bool LineLine( Cartesian *lineB, Cartesian *out ) { 
    int Ax,Bx,Cx,Ay,By,Cy,d,e,f;
    int x1lo,x1hi,y1lo,y1hi;
    Ax = x2-x;
    Bx = lineB->x-lineB->x2;
    // X bound box test 
    if(Ax<0) { x1lo=2; x1hi=x; }
    else     { x1hi=2; x1lo=x; }
    if(Bx>0) { if(x1hi < lineB->x2 || lineB->x < x1lo) return false; }
    else     { if(x1hi < lineB->x || lineB->x2 < x1lo) return false; }
    Ay = y2-y;    
    By = lineB->y-lineB->y2;    
    // Y bound box test
    if(Ay<0) { y1lo=y2; y1hi=y; } 
    else     { y1hi=y2; y1lo=y; }    
    if(By>0) { if(y1hi < lineB->y2 || lineB->y < y1lo) return false; } 
    else     { if(y1hi < lineB->y || lineB->y2 < y1lo) return false; }
    Cx = x-lineB->x;
    Cy = y-lineB->y;
    d = By*Cx - Bx*Cy;                    // alpha numerator
    f = Ay*Bx - Ax*By;                    // both denominator
    // alpha tests    
    if(f>0) { if(d < 0 || d > f) return false; } 
    else    { if(d > 0 || d < f) return false; }
    e = Ax*Cy - Ay*Cx;                    // beta numerator
    // beta tests
    if(f > 0) { if(e < 0 || e > f) return false; } 
    else      { if(e > 0 || e < f) return false; }
    if(f==0) return false; // CO-LINEAR!    - could return something other than false..
    // compute intersection coordinates   
    // - not necessary for pure collision detection...    
    int num = d*Ax;                                        // numerator     
    int offset = (Cartesian_SAME_SIGNS(num,f) ? f/2 : -f/2);        // round direction
    out->x = x + (num+offset) / f;                        // intersection x 
    num = d*Ay;                                        // numerator 
    offset = Cartesian_SAME_SIGNS(num,f) ? f/2 : -f/2;        // round direction
    out->y = y + (num+offset) / f;                        // intersection y 
    return true; // if got here, then the lines do intersect.
 }
 // LineBox collision (Tim Gift http://www.garagegames.com/community/blogs/view/309)
 bool LineBox( Cartesian *box, float *collisionRatio ) {
   float st,et,fst = 0.0f,fet = 1.0f;
   float bmin = (float) (box->x<box->x2?box->x:box->x2);
   float bmax = (float) (box->x>box->x2?box->x:box->x2);
   float si = (float) x;
   float ei = (float) x2;
   for (int i = 0; i < 3; i++) {
   if (si < ei) {
    if (si > bmax || ei < bmin) return false;
    float di = ei - si;
    st = (si < bmin)? (bmin - si) / di: 0.0f;
    et = (ei > bmax)? (bmax - si) / di: 1.0f;
   } else {
    if (ei > bmax || si < bmin) return false;
    float di = ei - si;
    st = (si > bmax)? (bmax - si) / di: 0.0f;
    et = (ei < bmin)? (bmin - si) / di: 1.0f;
   }
   if (st > fst) fst = st;
   if (et < fet) fet = et;
   if (fet < fst) return false;
   bmin++; bmax++;
   si++; ei++;
  }
  *collisionRatio = fst;
  return true;
 }
 bool BoxLine( Cartesian *line, float *collisionRatio ) {
   float st,et,fst = 0.0f,fet = 1.0f;
   float bmin = (float) (x<x2?x:x2);
   float bmax = (float) (x>x2?x:x2);
   float si = (float) line->x;
   float ei = (float) line->x2;
   for (int i = 0; i < 3; i++) {
   if (si < ei) {
    if (si > bmax || ei < bmin) return false;
    float di = ei - si;
    st = (si < bmin)? (bmin - si) / di: 0.0f;
    et = (ei > bmax)? (bmax - si) / di: 1.0f;
   } else {
    if (ei > bmax || si < bmin) return false;
    float di = ei - si;
    st = (si > bmax)? (bmax - si) / di: 0.0f;
    et = (ei < bmin)? (bmin - si) / di: 1.0f;
   }
   if (st > fst) fst = st;
   if (et < fet) fet = et;
   if (fet < fst) return false;
   bmin++; bmax++;
   si++; ei++;
  }
  *collisionRatio = fst;
  return true;
 }
#define Cartesian_clamp(x, l, h)    (  ((x) > (h)) ? (h) : ((x) < (l)) ? (l) : (x)   )   
 // original code suggested on 'stackoverflow.com' - Juozas Kontvainis
 bool LineCircle( int cx, int cy, double R ) {
  double dx = (double) (x2 - x), dy = (double) (y2 - y);
  double l,t;
  double nearestX,nearestY;
  int dist;
  int px,py;
  if((dx == 0) && (dy == 0)) // A and B are the same - cannot test
   return false;
  l = (dx*dx + dy*dy);
  t = (double) ( (cx-x)*dx + (cy-y)*dy ) / l;
  //point on line nearest to circle centre
  nearestX = (x + t * dx);
  nearestY = (y + t * dy);
  //CLAMP THE POINT TO THE ACTUAL LINE... - rather than an infinite line...
  if(x == x2) { nearestX = x; }
  else if(x < x2) { nearestX = Cartesian_clamp(nearestX, x, x2); }
  else { nearestX = Cartesian_clamp(nearestX, x2, x); }
  if(y == y2) { nearestY = y; }
  else if(y < y2) { nearestY = Cartesian_clamp(nearestY, y, y2); }
  else { nearestY = Cartesian_clamp(nearestY, y2, y); }
  px = (int)nearestX;
  py = (int)nearestY;
  dist = (int) idistance(px,py,cx,cy);
  if(dist == R) return true;
  else if(dist < R) return true;    
  return false;
 }
 bool LineCircle( Zint cx, Zint cy, Zdouble R, Cartesian *out1, Cartesian *out2 ) { return LineCircle((int)cx,(int)cy,(double)R,out1,out2); }
 bool LineCircle( int cx, int cy, double R, Cartesian *out1, Cartesian *out2 ) {
   double dx = (double) (x2 - x), dy = (double) (y2 - y);
   double l,t;
   double nearestX,nearestY;
   int dist;
   int px,py;
   if((dx == 0) && (dy == 0)) // A and B are the same - cannot test
    return false;
   l = (dx*dx + dy*dy);
   t = (double) ( (cx-x)*dx + (cy-y)*dy ) / l;
   //point on line nearest to circle centre
   nearestX = (x + t * dx);
   nearestY = (y + t * dy);
   //CLAMP THE POINT TO THE ACTUAL LINE... - rather than an infinite line...
   if(x == x2) { nearestX = x; }
   else if(x < x2) { nearestX = Cartesian_clamp(nearestX, x, x2); }
   else { nearestX = Cartesian_clamp(nearestX, x2, x); }
   if(y == y2) { nearestY = y; }
   else if(y < y2) { nearestY = Cartesian_clamp(nearestY, y, y2); }
   else { nearestY = Cartesian_clamp(nearestY, y2, y); }
   px = (int)nearestX;
   py = (int)nearestY;
   dist = (int) idistance(px,py,cx,cy);
   if(dist == R) {
    out1->x=out2->x=(int)nearestX;
    out1->y=out2->y=(int)nearestY;
    return true;
   } else if(dist < R) { //there are two interection points...
    double dt = sqrt(R*R-dist*dist)/sqrt((double)dist);
    //interection of point nearest to A
    out1->x = (int)(x + (t - dt) * dx);
    out1->y = (int)(y + (t - dt) * dy);
    //interection of point farthest from A
    out2->x = (int)(x + (t + dt) * dx);
    out2->y = (int)(y + (t + dt) * dy);    
    return true;    
   }
  return false;
 }
 bool BoxLine( Cartesian *line ) {
  return false;
 }
 void Multiply( double ratio ) {
  x=(int)((double)x*ratio);
  y=(int)((double)y*ratio);
 }
 void Multiply( Cartesiand *ratio );
 int CX() { return x+w/2; }
 int CY() { return y+h/2; }
MANY(Cartesian,CartesianHandle,CartesianHandles,"caxy",Cartesians,{})
 KEYWORDSGroup("caxy",Cartesian)
 WORDKEYSGroup("caxy",Cartesian)
 BINARY_INOUT(Cartesian)
 SORTING(Cartesian,SortCounterClockwise,{},{ if ( A->a < B->a ) result=-1; },{})
 Cartesian *Find( int x, int y ) {
  FOREACH(Cartesian,c) {
   if ( c->x == x && c->y == y ) return c;
  }
  return null;
 }
 void VectorRectangle( int x, int y, int x2, int y2 ) {
  int w=abs(x2-x);
  int h=abs(y2-y); 
  Add(x,y,w,0);
  Add(x,y,0,h);
  Add(x2,y2,0,-h);
  Add(x2,y2,-w,0);
 }
 void VectorRectanglef( float x, float y, float x2, float y2 ) {
  float w=abs(x2-x);
  float h=abs(y2-y);
  Add(x,y,w,0.0f);
  Add(x,y,0.0f,h);
  Add(x2,y2,-w,0.0f);
  Add(x2,y2,0.0f,-h);
 }
 Cartesian *Add( int x, int y ) {
  Cartesian *c=new Cartesian;
  c->Set(x,y);
  this->Append(c);
  return c;
 }
 void ScaleBy( int factor ) { FOREACH(Cartesian,c) { c->x *=factor; c->y *=factor; } }
 void Remove( ListItem *L ) { _Remove(L); }
 void Remove( Cartesian *c ) { _Remove(c); }
 void Remove( int x, int y ) {
  Cartesian *found=Find(x,y);
  if ( found ) {
   Remove(found);
   delete found;
  }
 }
 void AddIfUnique( int x, int y ) {
  FOREACH(Cartesian,c) if ( c->x==x && c->y==y ) return;
  Cartesian *c=new Cartesian;
  c->Set(x,y);
  this->Prepend(c); // Prepending here increases duplicate item detection efficiency
 }
 void Add( int x, int y, int w, int h ) {
  Cartesian *c=new Cartesian;
  c->SetRect(x,y,w,h);
  this->Append(c);
 }
 void Add( float x, float y, float w, float h ) {
  Cartesian *c=new Cartesian;
  c->SetRectf(x,y,w,h);
  this->Append(c);
 }
 bool Intersects( Cartesiand *a );
 bool Intersects( Cartesian *a ) {
  FOREACH(Cartesian,c) if ( c->Intersects(a) ) return true;
  return false;
 }
 bool Intersects( Cartesiand *a, Cartesiand *ofs );
 bool Intersects( Cartesian *a, Cartesian *ofs ) {
  FOREACH(Cartesian,c) {
   Cartesian d(c);
   d.MoveBy(ofs->x,ofs->y);
   if ( d.Intersects(a) ) return true;
  }
  return false;
 }
 bool Intersects( Cartesiand *a, double ox, double oy );
 bool Intersects( Cartesiand *a, int ox, int oy );
 bool Intersects( Cartesian *a, int ox, int oy ) {
  FOREACH(Cartesian,c) {
   Cartesian d(c);
   d.MoveBy(ox,oy);
   if ( d.Intersects(a) ) return true;
  }
  return false;
 }
 bool Within( int tx, int ty ) {
  FOREACH(Cartesian,c) if ( c->within(tx,ty) ) return true;
  return false;
 }
 bool Within( having inclusiveEdges, int tx, int ty ) {
  FOREACH(Cartesian,c) if ( c->withinclusive(tx,ty) ) return true;
  return false;
 }
 bool Within( int tx, int ty, double dx, double dy ) {
  FOREACH(Cartesian,c) if ( c->within(tx-(int)dx,ty-(int)dy) ) return true;
  return false;
 }
 bool Within( having inclusiveEdges, int tx, int ty, double dx, double dy ) {
  FOREACH(Cartesian,c) if ( c->withinclusive(tx-(int)dx,ty-(int)dy) ) return true;
  return false;
 }
 bool Intersects( Cartesian *a, Cartesiand *ofs );
 bool Intersects( Cartesian *a, double dx, double dy );
 bool Intersects( Cartesian *a, double dx, double dy, Cartesian *excepting );
 bool Intersects( Cartesiand *a, double dx, double dy, Cartesian *excepting );
 bool Intersects( Cartesians *many ) {
  FOREACH(Cartesian,c) EACH(many->first,Cartesian,d) if ( c->Intersects(d) ) return true;
  return false;
 }
 void GetGridPoints( int sx, int sy, int ex, int ey, bool rounding=true ) {
  double m=iratiod((ey-sy),(ex-sx));    
  double b=(double)sy-m/(double)sx;
  double step=1.0/ceil(abs(m))+1.0;        //Calculate a step value to find all grid squares
  double startX=(double)UMIN(sx,ex);
  double endX=(double)UMAX(sx,ex);
  if ( rounding ) for ( double x=startX; x<=endX; x+=step ) {
   double y = m * x + b;
   int iy=(int) ( y+0.5 );
   int ix=(int) ( x+0.5 );
   AddIfUnique(ix,iy);
  } else for ( double x=startX; x<=endX; x+=step ) {
   double y = m * x + b;
   int iy=(int) ( y );
   int ix=(int) ( x );
   AddIfUnique(ix,iy);
  }
 }
 inline bool Intersects( Vertex *offset, Cartesians *many );
 bool TranslateIntersects( Vertex *offset1, Vertex *offset2, Cartesians *many );
 void MoveBy( int x, int y ) {
  FOREACH(Cartesian,c) c->MoveBy(x,y);
 }
 void Duplicate( Cartesians *many ) {
  FOREACH(Cartesian,c) many->Append(c->Copy()); 
 }
 void GetLinePoints( int x, int y, int x2, int y2, int accuracy );
DONE(Cartesian);

class Vertexd;
ONE(Cartesiand,{Zero();})
 double x, y, z, w, h, d, x2, y2, z2;
 Zdouble a;
 void Zero() { x=y=z=w=h=d=x2=y2=z2=0.0; }
 Cartesiand( Cartesiand *c ) { Set(c); }
 Cartesiand( double X, double Y ) : ListItem() { x=X; y=Y; z=w=h=d=x2=y2=z2=0.0; }
 Cartesiand( double X, double Y, double W, double H ) : ListItem() { x=X; y=Y; w=W; h=H; x2=X+W; y2=Y+H; z=d=z2=0.0; }
 Cartesiand( double X, double Y, double Z, double W, double H, double D ) : ListItem() { x=X; y=Y; z=Z; w=W; h=H; d=D; x2=X+W; y2=Y+H; z2=Z+D; } 
 KEYWORDS({
  KEYWORD("x",{x=(double)atof(w);})
  else KEYWORD("y",{y=(double)atof(w);})
  else KEYWORD("z",{z=(double)atof(w);})
  else KEYWORD("w",{this->w=(double)atof(w);})
  else KEYWORD("h",{h=(double)atof(w);})
  else KEYWORD("d",{d=(double)atof(w);})
  else KEYWORD("x2",{x2=(double)atof(w);})
  else KEYWORD("y2",{y2=(double)atof(w);})
  else KEYWORD("z2",{z2=(double)atof(w);})
 })
 WORDKEYS({
  if ( x  != 0.0 ) WORDKEY("x",D2S(x))
  if ( y  != 0.0 ) WORDKEY("y",D2S(y))
  if ( z  != 0.0 ) WORDKEY("z",D2S(z))
  if ( w  != 0.0 ) WORDKEY("w",D2S(w))
  if ( h  != 0.0 ) WORDKEY("h",D2S(h))
  if ( d  != 0.0 ) WORDKEY("d",D2S(d))
  if ( x2 != 0.0 ) WORDKEY("x2",D2S(x2))
  if ( y2 != 0.0 ) WORDKEY("y2",D2S(y2))
  if ( z2 != 0.0 ) WORDKEY("z2",D2S(z2))
 })
 BINARY_IN({
  B_IN_TXT(name)  B_IN_POD(x)  B_IN_POD(y)  B_IN_POD(z)
  B_IN_POD(w)     B_IN_POD(h)  B_IN_POD(d)  B_IN_POD(x2)
  B_IN_POD(y2)    B_IN_POD(z2)
 })
 BINARY_OUT({
  B_OUT_TXT(name) B_OUT_POD(x) B_OUT_POD(y) B_OUT_POD(z)
  B_OUT_POD(w)    B_OUT_POD(h) B_OUT_POD(d) B_OUT_POD(x2)
  B_OUT_POD(y2)   B_OUT_POD(z2)
 })
 Cartesiand *Duplicate() {
  Cartesiand *c=new Cartesiand;
  c->x=x;    c->y=y;    c->z=z;
  c->w=w;    c->h=h;    c->d=d;
  c->x2=x2;  c->y2=y2;  c->z2=z2;
  return c;
 }
 void Duplicate( Cartesiand *c ) {
  c->x=x;    c->y=y;    c->z=z;
  c->w=w;    c->h=h;    c->d=d;
  c->x2=x2;  c->y2=y2;  c->z2=z2;
 }
 void Set(Cartesiand *c) {
  x=c->x;    y=c->y;    z=c->z;
  w=c->w;    h=c->h;    d=c->d;
  x2=c->x2;  y2=c->y2;  z2=c->z2;
 }
 void Set(Vertexd *in);
 void Set( Zdouble x, Zdouble y ) { Set((double)x,(double)y); }
 void Set( double x, double y ) { this->x=x; this->y=y; }
 void Set( double x, double y, double z ) { Set(x,y); this->z=z; }
 void Set( double x, double y, double z, double w, double h, double d ) { SetRect(x,y,w,h); this->z=z; this->d=d; this->z2=z+d; }
 void Vector( double angle, double speed ) { Zero(); x=speed; Rotate( 0.0, 0.0, angle ); }
 void VectorX( double angle, double speed ) { Zero(); x=speed; Rotate( 0.0, 0.0, angle ); }
 void VectorY( double angle, double speed ) { Zero(); y=speed; Rotate( 0.0, 0.0, angle ); }
 void VectorZ( double angle, double speed ) { Zero(); z=speed; Rotate( 0.0, 0.0, angle ); }
 void VectorXY( double angle, double speed ) { Zero(); x=speed; Rotate( 0.0, angle, 0.0 ); }
 void VectorYY( double angle, double speed ) { Zero(); y=speed; Rotate( 0.0, angle, 0.0 ); }
 void VectorZY( double angle, double speed ) { Zero(); z=speed; Rotate( 0.0, angle, 0.0 ); }
 void VectorXX( double angle, double speed ) { Zero(); x=speed; Rotate( angle, 0.0, 0.0 ); }
 void VectorYX( double angle, double speed ) { Zero(); y=speed; Rotate( angle, 0.0, 0.0 ); }
 void VectorZX( double angle, double speed ) { Zero(); z=speed; Rotate( angle, 0.0, 0.0 ); }
 void Lerp2d( Cartesiand *a, Cartesiand *b, double time );
 void Rotate( double angleDeg ) {
  Rotate( 0.0, 0.0, (angleDeg) );
 }
 void Rotate( double ax, double ay, double az ) {
  static Cartesiand temp;
  ax=deg2rad(ax);
  ay=deg2rad(ay);
  az=deg2rad(az);     
  // Rotate by az around Z axis
  temp.Set(
   x*cos(az) - y*sin(az),
   x*sin(az) + y*cos(az),
   z
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
  // Rotate by ay around Y axis
  temp.Set(
   z*sin(ay) + x*cos(ay),
   y,
   z*cos(ay) - x*sin(ay)
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
  // Rotate by ax around X axis
  temp.Set(
   x,
   y*cos(ax) - z*sin(ax),
   y*sin(ax) + z*cos(ax)
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
 }
 void SetRect( Zdouble X, Zdouble Y, Zdouble W, Zdouble H ) { SetRect((double)X,(double)Y,(double)W,(double)H); }
 void SetRect( double X, Zdouble Y, Zdouble W, Zdouble H ) { SetRect(X,(double)Y,(double)W,(double)H); }
 void SetRect( double X, double Y, Zdouble W, Zdouble H ) { SetRect(X,Y,(double)W,(double)H); }
 void SetRect( double X, double Y, double W, Zdouble H ) { SetRect(X,Y,W,(double)H); }
 void SetRect( double X, double Y, double W, double H ) {
  x=X; y=Y; w=W; h=H; x2=X+W; y2=Y+H;
 }
 void SetCube( double X, double Y, double Z, double W, double H, double D ) {
  x=X; y=Y; z=Z; w=W; h=H; d=D; x2=X+W; y2=Y+H; z2=Z+D;
 }
 // Treat x,y,z and x2,y2,z2 as boundaries of a volume, where x,y,z is min and x2,y2,z2 is max, calculate W/H/D
 void Volume() {
  w= ( x < 0 && x2 < 0 ) ? ( abs(x)-abs(x2) ) : ( x < 0 && x2 > 0 ) ? ( abs(x)+abs(x2) ) : ( x2-x );
  h= ( y < 0 && y2 < 0 ) ? ( abs(y)-abs(y2) ) : ( y < 0 && y2 > 0 ) ? ( abs(y)+abs(y2) ) : ( y2-y );
  d= ( z < 0 && z2 < 0 ) ? ( abs(z)-abs(z2) ) : ( z < 0 && z2 > 0 ) ? ( abs(z)+abs(z2) ) : ( z2-z );
 }
 void SetCorners( double X, double Y, double X2, double Y2 ) {
  x=X; y=Y; w=X2-X; h=Y2-Y; x2=X2; y2=Y2;
 }
 void SetCorners( double X, double Y, double X2, double Y2, double W, double H ) {
  x=X; y=Y; x2=X2; y2=Y2; w=W; h=H;
 }
 bool Equals( Cartesiand *B ) {
  return ( x == B->x && y == B->y && z == B->z
        && w == B->w && h == B->h && d == B->d );
 }
 void Multiply( double factor ) {
  x*=factor;
  y*=factor;
  z*=factor;
  w*=factor;
  h*=factor;
  d*=factor;
  x2*=factor;
  y2*=factor;
  z2*=factor;
 }
 bool within( double X, double Y ) {
  return (WITHIN(X,Y,x,y,x2,y2));
 }
// bool within( float X, float Y ) { return (WITHIN(X,Y,fx,fy,(float)x2,(float)y2)); }
 void MoveBy( double X, double Y ) {
  x+=X;
  y+=Y;
  x2+=X;
  y2+=Y;
 }
 void MoveTo( double X, double Y ) {
  SetRect(X,Y,w,h);
 }
 void Add( double x, double y, double z ) {
  this->x+=x; x2+=x;
  this->y+=y; y2+=y;
  this->z+=z;
 }
 double NormalizedMagnitude() {
  double M = Magnitude(); // Calculate The Length Of The Vector 
  if (M != 0.0)			// Make Sure We Don't Divide By 0 
  {
   double tx = (x/M);		// Normalize The 3 Components 
   double ty = (y/M);
   double tz = (z/M);

   return sqrt( SQ(tx)+SQ(ty)+SQ(tz) );
  }
  return 0.0;
 }
 double Magnitude ()				// Calculate The Length Of The Vector
 {
  return sqrt (SQ(x)+SQ(y)+SQ(z));
 }
 void normalize()	{ // Creates A Vector With A Unit Length Of 1
  double M = Magnitude(); // Calculate The Length Of The Vector 
  if (M != 0.0)			// Make Sure We Don't Divide By 0 
  {
   x = (x/M);		// Normalize The 3 Components 
   x2=x+w;
   y = (y/M);
   y2=y+h;
   z = (z/M);
   z2=z+d;
  }
 }
 bool Intersects( Cartesiand *a );
 bool Intersects( Cartesian *a );
 void PointInCircle( double radius ) {
  double q = (uniform() * (PI * 2));
  double r = sqrt(uniform());
  x = (radius * r) * cos(q);
  y = (radius * r) * sin(q);
 }
 void PointDenseCentered( double radius ) {
  double r = uniform();
  double q = (uniform() * (PI * 2));
  x = (radius * r) * cos(q);
  y = (radius * r) * sin(q);
 }
 void Print( const char *name ) {
  OUTPUT( "Cartesiand `%s`:\n - integers: x,y,z: %e,%e,%e whd: %ex%ex%e x2,y2,z2:%e,%e,%e\n",
   name,
   x, y, z, w, h, d, x2, y2, z2
  );
 }
 // Compute the line angle in radians
 double LineAngle() {
  return atan2( (y2-y), (x2-x) );
 }
 void Lerp( Cartesiand *a, Cartesiand *b, double L ) {
  x=a->x+(b->x-a->x)*L;
  y=a->y+(b->y-a->y)*L;
  z=a->z+(b->z-a->z)*L;
 }
 void Lerp( Cartesian *a, Cartesian *b, double L ) {
  x=(double)a->x+((double)b->x-(double)a->x)*L;
  y=(double)a->y+((double)b->y-(double)a->y)*L;
  z=(double)a->z+((double)b->z-(double)a->z)*L;
 }
 double LineMagnitude() {
  Cartesiand vector;
  vector.x = x2 - x;
  vector.y = y2 - y;
  vector.z = z2 - z;
  return (float)sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
 }
 double Magnitude( Cartesiand *Point2 ) {
  Cartesiand vector;
  vector.x = Point2->x - x;
  vector.y = Point2->y - y;
  vector.z = Point2->z - z;
  return sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
 }
 bool DistancePointLine( Cartesiand *line, double *Distance ) { // P. Bourke et al
  double LineMag;
  double U;
  Cartesiand intersection;
  LineMag = line->LineMagnitude();
  U = ( ( ( x - line->x ) * ( line->x2 - line->x ) ) + ( ( y - line->y ) * ( line->y2 - line->y ) ) + ( ( z - line->z ) * ( line->z2 - line->z ) ) ) / ( LineMag * LineMag );
  if ( U < 0.0f || U > 1.0f ) return false;   // closest point does not fall within the line segment
  intersection.x = line->x + U * ( line->x2 - line->x );
  intersection.y = line->y + U * ( line->y2 - line->y );
  intersection.z = line->z + U * ( line->z2 - line->z );
  *Distance = Magnitude( &intersection );
  return true;
 }
 double Aspect() {
  return w/h;
 }
 double AspectInverse() {
  return h/w;
 }
 double CenterX() { return x+w/2.0; }
 double CenterY() { return y+h/2.0; }
 TOJSON {
  return FORMAT(
   JSONDECIMAL("x")
   JSONDECIMAL("y")
   JSONDECIMAL("z")
   JSONDECIMAL("w")
   JSONDECIMAL("h")
   JSONDECIMAL("d")
   JSONDECIMAL("x2")
   JSONDECIMAL("y2")
   JSONDECIMAL_("z2"),
   (double) x, (double) y, (double) z, 
   (double) w, (double) h, (double) d, 
   (double) x2, (double) y2, (double) z2
  );
 }
MANY(Cartesiand,CartesiandHandle,CartesiandHandles,"cadxy",Cartesiands,{})
 MANYTOJSON(Cartesiand)
Cartesiand sortByNearest;
void SortPointsNearestTo( double x, double y, double z ) { sortByNearest.Set(x,y,z); SortPointNearestFirst();}
 SORTING(Cartesiand,SortPointNearestFirst,{
  FOREACH(Cartesiand,cd) cd->d=ddistance(cd->x,cd->y,cd->z,sortByNearest.x,sortByNearest.y,sortByNearest.z);
 },{ result = (A->d < B->d ? -1 : A->d == B->d ? 0 : 1); },{})
 KEYWORDSGroup("cadxy",Cartesiand)
 WORDKEYSGroup("cadxy",Cartesiand)
 BINARY_INOUT(Cartesiand)
 CLONES(Cartesiand,Cartesiands);
 void fromVertices( Vertices *in );
 void MoveBy( double dx, double dy ) {
  FOREACH(Cartesiand,c) c->MoveBy(dx,dy);
 }
 void WrapBy( double dx, double dy, double x1, double x2, double y1, double y2 ) {
  FOREACH(Cartesiand,c) {
   c->x=wrap(c->x+dx,x1,x2);
   c->y=wrap(c->y+dy,y1,y2);
  }
 }
 double Distance2d() {
  double d=0.0;
  if ( count < 2 ) return d;
  Cartesiand *p=(Cartesiand*)(first->next);
  Cartesiand *p_prev=(Cartesiand*)p->prev;
  while ( p ) {
   d+=ddistance(p->x,p->y,p_prev->x,p_prev->y);
   p_prev=p;
   p=(Cartesiand *)p->next;
  }
  return d;
 }
 Cartesiand *LeastX() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->x < candidate->x ) candidate=c;
  return candidate;
 }
 Cartesiand *LeastY() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->y < candidate->y ) candidate=c;
  return candidate;
 }
 Cartesiand *LeastZ() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->z < candidate->z ) candidate=c;
  return candidate;
 }
 Cartesiand *GreatestX() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->x > candidate->x ) candidate=c;
  return candidate;
 }
 Cartesiand *GreatestY() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->y > candidate->y ) candidate=c;
  return candidate;
 }
 Cartesiand *GreatestZ() {
  Cartesiand *candidate=(Cartesiand *)first;
  FOREACH(Cartesiand,c) if ( c->z > candidate->z ) candidate=c;
  return candidate;
 }
 // Boundaries of x,y
 bool Bounds2d( Cartesiand *out ) {
  if ( count == 0 ) { OUTPUT("Cartesiand::Bounds on empty list\n"); return false; }
  Cartesiand *p=(Cartesiand *) first;
  double Lx=p->x;
  double Ly=p->y;
  double Gx=p->x;
  double Gy=p->y;
  p=(Cartesiand *) p->next;
  while ( p ) {
   if ( p->x < Lx ) Lx=p->x;
   if ( p->y < Ly ) Ly=p->y;
   if ( p->x > Gx ) Gx=p->x;
   if ( p->y > Gy ) Gy=p->y;
   p=(Cartesiand *) p->next;
  }
  out->SetRect(Lx,Ly,Gx,Gy);
  return true;
 }
 // Boundaries of x,y,x2,y2
 bool Bounds2dRect( Cartesiand *out ) {
  if ( count == 0 ) { OUTPUT("Cartesiand::Bounds on empty list\n"); return false; }
  Cartesiand *p=(Cartesiand *) first;
  double Lx=p->x;
  double Ly=p->y;
  double Gx=p->x;
  double Gy=p->y;
  p=(Cartesiand *) p->next;
  while ( p ) {
   if ( p->x < Lx ) Lx=p->x;
   if ( p->y < Ly ) Ly=p->y;
   if ( p->x > Gx ) Gx=p->x;
   if ( p->y > Gy ) Gy=p->y;
   if ( p->x2 < Lx ) Lx=p->x2;
   if ( p->y2 < Ly ) Ly=p->y2;
   if ( p->x2 > Gx ) Gx=p->x2;
   if ( p->y2 > Gy ) Gy=p->y2;
   p=(Cartesiand *) p->next;
  }
  out->SetRect(Lx,Ly,Gx,Gy);
  return true;
 }
 void Bounds3d( Box *out );
 // Treating the list as a series of end-to-end line segments,
 // outputs the two closest points connected, returning true 
 // if more than one point was in the list.  Otherwise, values for
 // a and b are unchanged.  Point A is always the closest point,
 // and point B is either the previous point in the list, or the next,
 // depending which is closer.
 bool NearestLine( double x, double y, Cartesiand **a, Cartesiand **b, double farValue=1000000000.0 ) {
  if ( count < 2 ) return false;
  double dA=farValue;
  Cartesiand *candidateA=null;
  Cartesiand *candidateB=null;
  FOREACH(Cartesiand,c) {
   double d=ddistance(c->x,c->y,x,y);
   if ( d < dA ) {
    dA=d;
    candidateA=c;
    if ( !c->prev ) {
     Cartesiand *cp=(Cartesiand *)c->prev;
     candidateB=cp;
    } else if ( !c->next ) {
     Cartesiand *cp=(Cartesiand *)c->next;
     candidateB=cp;
    } else {
     Cartesiand *c1=(Cartesiand *)c->prev;
     Cartesiand *c2=(Cartesiand *)c->next;
     double d1=ddistance(c1->x,c1->y,x,y);
     double d2=ddistance(c2->x,c2->y,x,y);
     if ( d1 < d2 ) {
      candidateB=c1;
     } else {
      candidateB=c2;
     }
    }
   }
  }
  (*a)=candidateA;
  (*b)=candidateB;
  return true;
 }
 // Treating the list as a series of end-to-end line segments,
 // outputs the two closest points connected, returning true 
 // if more than one point was in the list.  Otherwise, values for
 // a and b are unchanged.  Point A is always the closest point,
 // and point B is always the next point in the list (or null).
 bool NearestSegmentTowardEnd( double x, double y, Cartesiand **a, Cartesiand **b, double farValue=1000000000.0 ) {
  if ( count < 2 ) return false;
  double dA=farValue;
  Cartesiand *candidateA=null;
  Cartesiand *candidateB=null;
  FOREACH(Cartesiand,c) {
   double d=ddistance(c->x,c->y,x,y);
   if ( d < dA ) {
    dA=d;
    candidateA=c;
    Cartesiand *cp=(Cartesiand *)c->next;
    candidateB=cp;
   }
  }
  (*a)=candidateA;
  (*b)=candidateB;
  return true;
 }
 // Treating the list as a series of end-to-end line segments,
 // outputs the one closest point, returning it.
 Cartesiand *NearestPoint( double x, double y, double farValue=1000000000.0 ) {
  double dA=farValue;
  Cartesiand *candidateA=null;
  FOREACH(Cartesiand,c) {
   double d=ddistance(c->x,c->y,x,y);
   if ( d < dA ) {
    dA=d;
    candidateA=c;
   }
  }
  return candidateA;
 }
 // Give me the closest point (out) and return the "next point" in the segment,
 // by using a combination of line testing (when perpendicular)
 // or the closest endpoints.  Returns null
 // Returns the nearest point on the line segments.
 // More thorough test, with fallback.
 Cartesiand *NearestPoint( Cartesiand *out, double x, double y, double farValue=10000000.0 );
 // This variant returns the nearest point on the segments and its distance
 Cartesiand *NearestPoint( Cartesiand *out, double *distOut, double x, double y, double farValue=10000000.0 );
 // having shortestBias variation means we look for the nearest point overall,
 // whereas the other tests all lines and returns the one farthest along in the list
 Cartesiand *NearestPoint( having shortestBias, Cartesiand *out, double x, double y, double farValue=10000000.0 );
 // These two functions work together to test paths in a rectangle world.
 // Obstacles are rectangles, path is a line we intend to travel.
 bool WillCollide( Cartesiands *obstacles, Line *path );
 // These two functions work together to test paths in a rectangle world.       ___b
 // Obstacles are rectangles, path is a line we intend to travel and           /  /|
 // footprint is the rectangle that will pass along it, where the rectangle   +-+a/   would be line A-B,   
 // center is the point which will move along the path line.                  +-+/     but as a rectangle. 
 bool WillCollide( Cartesiands *obstacles, Cartesiand *footprint, Line *path );
 void FitTo1() {
  Cartesiand bounds;
  Bounds2d(&bounds);
  double aspect=bounds.w/bounds.h;
  double invAspect=bounds.h/bounds.w;
  double scale = UMIN( 1.0/(double)bounds.w, 1.0/(double)bounds.h );
  FOREACH(Cartesiand,c) {
   c->x =c->x  * scale;   c->y =c->y  * scale;
   c->x2=c->x2 * scale;   c->y2=c->y2 * scale;
   c->w =c->w  * scale;   c->h =c->h  * scale;
  }
 }
 void FitTo1( having threed );
DONE(Cartesiand);

ONE(CartesiandsListItem,{})
 Cartesiands cartesians;
MANY(CartesiandsListItem,CartesiandsListItemHandle,CartesiandsListItemHandles,"CartesiandsList",CartesiandsList,{})
 void Dump(Cartesiands *in) {
  CartesiandsListItem *cli=new CartesiandsListItem;
  cli->cartesians.Concat(in);
  Append(cli);
 }
 void DumpCopy(Cartesiands *in) {
  CartesiandsListItem *cli=new CartesiandsListItem;
  EACH(in->first,Cartesiand,p) cli->cartesians.Append(p->Duplicate());
  Append(cli);
 }
 int TotalPoints() {
  int total=0;
  FOREACH(CartesiandsListItem,L) total+=L->cartesians.count.value;
  return total;
 }
 // Boundaries of x,y
 bool Bounds2d( Cartesiand *out ) {
  if ( count == 0 ) { OUTPUT("CartesiandsList::Bounds on empty list\n"); return false; }
  CartesiandsListItem *L=(CartesiandsListItem *) first;
  L->cartesians.Bounds2d(out);
  L=(CartesiandsListItem *)L->next;
  while ( L ) {
   Cartesiand temp;
   L->cartesians.Bounds2d(&temp);
   if ( out->x > temp.x ) out->x=temp.x;
   if ( out->y > temp.y ) out->y=temp.y;
   if ( out->x2 < temp.x2 ) out->x2=temp.x2;
   if ( out->y2 < temp.y2 ) out->y2=temp.y2;
  }
  return true;
 }
 // Boundaries of x,y,x2,y2
 bool Bounds2dRect( Cartesiand *out ) {
  if ( count == 0 ) { OUTPUT("CartesiandsList::Bounds on empty list\n"); return false; }
  CartesiandsListItem *L=(CartesiandsListItem *) first;
  L->cartesians.Bounds2dRect(out);
  L=(CartesiandsListItem *)L->next;
  while ( L ) {
   Cartesiand temp;
   L->cartesians.Bounds2dRect(&temp);
   if ( out->x > temp.x ) out->x=temp.x;
   if ( out->y > temp.y ) out->y=temp.y;
   if ( out->x2 < temp.x2 ) out->x2=temp.x2;
   if ( out->y2 < temp.y2 ) out->y2=temp.y2;
  }
  return true;
 }
 double Distance2d() {
  double total=0.0;
  FOREACH(CartesiandsListItem,L) total+=L->cartesians.Distance2d();
  return total;
 }
DONE(CartesiandsList);

class Angle : public ListItem {
public:
 float deg, rad;
 float Degrees() { return deg; }
 float Radians() { return rad; }
 void Degrees( float d ) {
  deg=d;
  rad=deg2rad(d);
 }
 void Radians( float r ) {
  rad=r;
  deg=rad2deg(r);
 }
 void AddDegrees( float d ) { Degrees( deg+d ); }
 void AddRadians( float r ) { Radians( rad+r ); } 
};

class Angles : public LinkedList {
public:
 CLEARLISTRESET(Angle);
 ~Angles() { Clear(); }
};

struct Range : public ListItem {
public:
 string name;
 float min,max;
 float total;
 Range() { min=0.0f; max=1.0f; total=1.0f; name=string(""); }
 Range( float L, float H ) { Set(L,H); name=string(""); }
 Range( string name, float L, float H ) { Set(L,H); this->name=name; }
 void Set( float mi, float ma ) {
  if ( mi>ma ) {
   min=ma;
   max=mi;
  } else {
   min=mi;
   max=ma;
  }
  if ( min < 0.0f && max < 0.0f ) total=-(abs(max)-abs(min));
  else if ( min < 0.0f ) total=abs(min)+max;
  else total=max-min;
 }
 float random() {
  return min+(total*(float) uniform());
 }
};

struct Ranges : public LinkedList {
public:
 CLEARLISTRESET(Range);
 ~Ranges() { Clear(); }
 Range *Add( string name, float L, float H ) {
  Range *m;
  Append(m=new Range( name, L, H ));
  return m;
 }
 Range *Add( float L, float H ) {
  Range *m;
  Append(m=new Range( L, H ));
  return m;
 }
 Range *named( string name ) {
  FOREACH(Range,m) if ( !str_cmp(m->name.c_str(),name.c_str()) ) return m;
  return null;
 }
 SORTING(Range,SortTotal,{},{ if ( A->total < B->total ) result=-1; },{})
};

struct Probability : public ListItem {
 string name;
 int percent;
 double one;
 float onef;
 Probability() : ListItem() { Set("",0); }
 Probability( string name, int percent ) : ListItem() { Set(name,percent); }
 Probability( string name, float ratio ) : ListItem() { Set(name,ratio);   }
 Probability( string name, double ratio) : ListItem() { Set(name,ratio);   }
 void Set( string name, int percent  ) { this->name=name; Set(percent); }
 void Set( string name, double ratio ) { this->name=name; Set(ratio);   }
 void Set( string name, float ratio  ) { this->name=name; Set(ratio);   }
 void Set( int percent ) {
  this->percent=percent;
  this->one=iratiod(percent,100);
  this->onef=iratiof(percent,100);
 }
 void Set( float ratio ) {
  this->onef=ratio;
  this->one=(double) ratio;
  this->percent=(int) ( ratio * 100.0f );
 }
 void Set( double ratio ) {
  this->one=ratio;
  this->onef=(float) ratio;
  this->percent=(int) (ratio * 100.0);
 }
 bool Dice() {
  return ( uniform() <= one );
 }
};

struct Probabilities : public LinkedList {
public:
 CLEARLISTRESET(Probability);
 ~Probabilities() { Clear(); }
 Probability *Add( string name, int percent ) {
  Probability *p;
  Append(p=new Probability( name, percent ));
  return p;
 }
 Probability *Add( string name, float ratio ) {
  Probability *p;
  Append(p=new Probability( name, ratio ));
  return p;
 }
 Probability *Add( string name, double ratio  ) {
  Probability *p;
  Append(p=new Probability( name, ratio ));
  return p;
 }
 Probability *named( string name ) {
  FOREACH(Probability,p) if ( !str_cmp(p->name.c_str(),name.c_str()) ) return p;
  return null;
 }
 SORTING(Probability,SortAscending,{},{ if ( A->one < B->one ) result=-1; },{})
};