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
#include "Display.h"
#include "Cartesian.h"
#include "BinaryFile.h"
// Holds all information relevent to an object's screen position in the 2.5D engine

ONE(ScreenPosition,{})
 Zint x,y, w,h, x2,y2, cx,cy, h2,w2;
 Zfloat fx,fy, fw,fh, fx2,fy2, fcx,fcy, fh2,fw2;
 Zdouble dx,dy, dw,dh, dx2,dy2, dcx,dcy, dh2,dw2;
 Zdouble angle; // rotation
 KEYWORDS({
  NUMWORD("x",x)
  else NUMWORD("y",y)
  else NUMWORD("w",w)
  else NUMWORD("h",h)
  else NUMWORD("x2",x2)
  else NUMWORD("y2",y2)
  else NUMWORD("cx",cx)
  else NUMWORD("cx",cy)
  else NUMWORD("h2",h2)
  else NUMWORD("w2",w2)
  else NUMWORD("fx",fx)
  else NUMWORD("fx",fy)
  else NUMWORD("fw",fw)
  else NUMWORD("fh",fh)
  else NUMWORD("fx2",fx2)
  else NUMWORD("fy2",fy2)
  else NUMWORD("fcx",fcx)
  else NUMWORD("fcy",fcy)
  else NUMWORD("fh2",fh2)
  else NUMWORD("fw2",fw2)
  else NUMWORD("dx",dx)
  else NUMWORD("dx",dy)
  else NUMWORD("dw",dw)
  else NUMWORD("dh",dh)
  else NUMWORD("dx2",dx2)
  else NUMWORD("dy2",dy2)
  else NUMWORD("dcx",dcx)
  else NUMWORD("dcy",dcy)
  else NUMWORD("dh2",dh2)
  else NUMWORD("dw2",dw2)
  else NUMWORD("a",angle)
  else BADKEY("ScreenPosition")
 })
 WORDKEYS({
  NUMBKEY("x",x)
  NUMBKEY("y",y)
  NUMBKEY("w",w)
  NUMBKEY("h",h)
  NUMBKEY("x2",x2)
  NUMBKEY("y2",y2)
  NUMBKEY("cx",cx)
  NUMBKEY("cx",cy)
  NUMBKEY("h2",h2)
  NUMBKEY("w2",w2)
  NUMBKEY("fx",fx)
  NUMBKEY("fx",fy)
  NUMBKEY("fw",fw)
  NUMBKEY("fh",fh)
  NUMBKEY("fx2",fx2)
  NUMBKEY("fy2",fy2)
  NUMBKEY("fcx",fcx)
  NUMBKEY("fcy",fcy)
  NUMBKEY("fh2",fh2)
  NUMBKEY("fw2",fw2)
  NUMBKEY("dx",dx)
  NUMBKEY("dx",dy)
  NUMBKEY("dw",dw)
  NUMBKEY("dh",dh)
  NUMBKEY("dx2",dx2)
  NUMBKEY("dy2",dy2)
  NUMBKEY("dcx",dcx)
  NUMBKEY("dcy",dcy)
  NUMBKEY("dh2",dh2)
  NUMBKEY("dw2",dw2)
  NUMBKEY("a",angle)
 })
 CLONE(ScreenPosition,{
  DUPE(x);
  DUPE(y);
  DUPE(x2);
  DUPE(y2);
  DUPE(w);
  DUPE(h);
  DUPE(cx);
  DUPE(cy);
  DUPE(w2);
  DUPE(h2);
  DUPE(fx);
  DUPE(fy);
  DUPE(fx2);
  DUPE(fy2);
  DUPE(fw);
  DUPE(fh);
  DUPE(fcx);
  DUPE(fcy);
  DUPE(fw2);
  DUPE(fh2);
  DUPE(dx);
  DUPE(dy);
  DUPE(dx2);
  DUPE(dy2);
  DUPE(dw);
  DUPE(dh);
  DUPE(dcx);
  DUPE(dcy);
  DUPE(dw2);
  DUPE(dh2);
  DUPE(angle);
 })
 void BinaryRead( BinaryFile *open ) {
  open->read(&x);
  open->read(&y);
  open->read(&w);
  open->read(&h);
  open->read(&x2);
  open->read(&y2);
  open->read(&cx);
  open->read(&cy);
  open->read(&h2);
  open->read(&w2);
  open->read(&fx);
  open->read(&fy);
  open->read(&fw);
  open->read(&fh);
  open->read(&fx2);
  open->read(&fy2);
  open->read(&fcx);
  open->read(&fcy);
  open->read(&fh2);
  open->read(&fw2);
  open->read(&dx);
  open->read(&dy);
  open->read(&dw);
  open->read(&dh);
  open->read(&dx2);
  open->read(&dy2);
  open->read(&dcx);
  open->read(&dcy);
  open->read(&dh2);
  open->read(&dw2);
  open->read(&angle);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->write(&x);
  open->write(&y);
  open->write(&w);
  open->write(&h);
  open->write(&x2);
  open->write(&y2);
  open->write(&cx);
  open->write(&cy);
  open->write(&h2);
  open->write(&w2);
  open->write(&fx);
  open->write(&fy);
  open->write(&fw);
  open->write(&fh);
  open->write(&fx2);
  open->write(&fy2);
  open->write(&fcx);
  open->write(&fcy);
  open->write(&fh2);
  open->write(&fw2);
  open->write(&dx);
  open->write(&dy);
  open->write(&dw);
  open->write(&dh);
  open->write(&dx2);
  open->write(&dy2);
  open->write(&dcx);
  open->write(&dcy);
  open->write(&dh2);
  open->write(&dw2);
  open->write(&angle);
 }
 void Set( ScreenPosition *s ) {
  x=    s->x;
  y=    s->y;
  w=    s->w;
  h=    s->h;
  x2=   s->x2;
  y2=   s->y2;
  cx=   s->cx;
  cy=   s->cy;
  h2=   s->h2;
  w2=   s->w2;
  fx=   s->fx;
  fy=   s->fy;
  fw=   s->fw;
  fh=   s->fh;
  fx2=  s->fx2;
  fy2=  s->fy2;
  fcx=  s->fcx;
  fcy=  s->fcy;
  fh2=  s->fh2;
  fw2=  s->fw2;
  dx=   s->dx;
  dy=   s->dy;
  dw=   s->dw;
  dh=   s->dh;
  dx2=  s->dx2;
  dy2=  s->dy2;
  dcx=  s->dcx;
  dcy=  s->dcy;
  dh2=  s->dh2;
  dw2=  s->dw2;
  angle=s->angle;
 }
 int RandomX() { return x+upto(w); }
 int RandomY() { return y+upto(h); }
 float RandomXf() { return float_range(fx,fx+fw); }
 float RandomYf() { return float_range(fy,fy+fh); }
 double RandomXd() { return double_range(dx,dx+dw); }
 double RandomYd() { return double_range(dy,dy+dh); }
 int RandomW() { return upto(w); }
 int RandomH() { return upto(h); }
 float RandomWf() { return float_range(0.0f,fw); }
 float RandomHf() { return float_range(0.0f,fh); }
 double RandomWd() { return double_range(0.0,dw); }
 double RandomHd() { return double_range(0.0,dh); }
 void X( int x ) { PositionX(x); }
 void X( float x ) { PositionX(x); }
 void X( double x ) { PositionX(x); }
 void PositionX( int X ) {
  x=X;
  fx=(float)X;
  dx=(double)X;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void PositionX( float X ) {
  x=(int)X;
  fx=X;
  dx=(double)X;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void PositionX( double X ) {
  x=(int)X;
  fx=(float)X;
  dx=X;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void Y( int y ) { PositionY(y); }
 void Y( float y ) { PositionY(y); }
 void Y( double y ) { PositionY(y); }
 void PositionY( int Y ) {
  y=Y;
  fy=(float)Y;
  dy=(double)Y;
  y2=y+h;
  fy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void PositionY( float Y ) {
  y=(int)Y;
  fy=Y;
  dy=(double)Y;
  y2=y+h;
  fy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void PositionY( double Y ) {
  y=(int)Y;
  fy=(float)Y;
  dy=Y;
  y2=y+h;
  fy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void W( int w ) { Width(w); }
 void W( float w  ) { Width(w); }
 void W( double w ) { Width(w); }
 void Width( int W ) {
  w=W;
  fw=(float)W;
  dw=(double)W;
  w2=w/2;
  fw2=fw/2.0f;
  dw2=dw/2.0;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void Width( float W ) {
  w=(int)W;
  fw=W;
  dw=(double)W;
  w2=w/2;
  fw2=fw/2.0f;
  dw2=dw/2.0;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void Width( double W ) {
  w=(int)W;
  fw=(float)W;
  dw=W;
  w2=w/2;
  fw2=fw/2.0f;
  dw2=dw/2.0;
  x2=x+w;
  fx2=fx+fw;
  dx2=dx+dw;
  cx=x+w/2;
  fcx=fx+fw/2.0f;
  dcx=dx+dw/2.0;
 }
 void H( int h ) { Height(h); }
 void H( float h ) { Height(h); }
 void H( double h ) { Height(h); }
 void Height( int H ) {
  h=H;
  fh=(float)H;
  dh=(double)H;
  h2=h/2;
  fh2=fh/2.0f;
  dh2=dh/2.0;
  y2=y+h;
  fy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void Height( float H ) {
  h=(int)H;
  fh=H;
  dh=(double)H;
  h2=h/2;
  fh2=fh/2.0f;
  dh2=dh/2.0;
  y2=y+h;
  fy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void Height( double H ) {
  h=(int)H;
  fh=(float)H;
  dh=H;
  h2=h/2;
  fh2=fh/2.0f;
  dh2=dh/2.0;
  y2=y+h;
  dy2=fy+fh;
  dy2=dy+dh;
  cy=y+h/2;
  fcy=fy+fh/2.0f;
  dcy=dy+dh/2.0;
 }
 void Size( int w, int h ) {
  Height(h);
  Width(w);
 }
 void Size( float w, float h ) {
  Height(h);
  Width(w);
 }
 void Size( double w, double h ) {
  Height(h); 
  Width(w);
 }
 void Size( int wh ) {
  Height(wh);
  Width(wh);
 }
 void Size( float wh ) {
  Height(wh);
  Width(wh);
 }
 void Size( double wh ) {
  Height(wh);
  Width(wh);
 }
 void MoveTo( int X, int Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void MoveTo( float X, float Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void MoveTo( double X, double Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void MoveToCenter( int CX, int CY ) {
  CenterX(CX);
  CenterY(CY);
 }
 void MoveToCenter( float CX, float CY ) {
  CenterX(CX);
  CenterY(CY);
 }
 void MoveToCenter( double CX, double CY ) {
  CenterX(CX);
  CenterY(CY);
 }
 void MoveBy( int X, int Y ) {
  PositionX(this->x+X);
  PositionY(this->y+Y);
 }
 void MoveBy( float X, float Y ) {
  PositionX(this->fx+X);
  PositionY(this->fy+Y);
 }
 void MoveBy( double X, double Y ) {
  PositionX(this->dx+X);
  PositionY(this->dy+Y);
 }
 void Set( int X, int Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void Set( float X, float Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void Set( double X, double Y ) {
  PositionX(X);
  PositionY(Y);
 }
 void Set( int X, int Y, int W, int H ) {
  Width(W);
  Height(H);
  CenterX(X+W/2);
  CenterY(Y+H/2);
 }
 void Set( float X, float Y, float W, float H ) {
  Width(W);
  Height(H);
  CenterX(X+W/2.0f);
  CenterY(Y+H/2.0f);
 }
 void Set( double X, double Y, double W, double H ) {
  Width(W);
  Height(H);  
  CenterX(X+W/2.0);
  CenterY(Y+H/2.0);
 }
 void Corners( int X, int Y, int X2, int Y2 ) {
  int x=X<X2?X:X2;
  int y=Y<Y2?Y:Y2;
  int w=abs(X2-X);
  int h=abs(Y2-Y);
  Set(x,y,w,h);
 }
 void Corners( float X, float Y, float X2, float Y2 ) {
  float x=X<X2?X:X2;
  float y=Y<Y2?Y:Y2;
  float w=ABS(X2-X);
  float h=ABS(Y2-Y);
  Set(x,y,w,h);
 }
 void Corners( double X, double Y, double X2, double Y2 ) {
  double x=X<X2?X:X2;
  double y=Y<Y2?Y:Y2;
  double w=ABS(X2-X);
  double h=ABS(Y2-Y);
  Set(x,y,w,h);
 }
 void Center( int cx, int cy ) {
  CenterX(cx);
  CenterY(cy);
 }
 void Center( float cx, float cy ) {
  CenterX(cx);
  CenterY(cy);
 }
 void Center( double cx, double cy ) {
  CenterX(cx);
  CenterY(cy);
 }
 void CenterX( int CX ) { PositionX(CX-w/2); }
 void CenterY( int CY ) { PositionY(CY-h/2); }
 void CenterX( float CX ) { PositionX(CX-fw/2.0f); }
 void CenterY( float CY ) { PositionY(CY-fh/2.0f); }
 void CenterX( double CX ) { PositionX(CX-dw/2.0); }
 void CenterY( double CY ) { PositionY(CY-dh/2.0); }
 void Set( Cartesian *in ) { Set(in->x,in->y,in->w,in->h); }
 void Get( Cartesian *out ) { out->Corners(x,y,x2,y2,w,h); }
 void Setf( Cartesian *in ) { Set(in->fx,in->fy,(float)in->w,(float)in->h); }
 void Set( Cartesiand *in ) { Set(in->x,in->y,in->w,in->h); }
 void Get( Cartesiand *out ) { out->SetRect(dx,dy,dw,dh); }
 void GetNormalizedPosition( double inx, double iny, double *outx, double *outy ) {
  if ( dx < 0 ) {
   if ( inx < dx ) (*outx)=-((inx-dx)/dw);
   else (*outx)=(inx+abs(dx))/dw;
  } else {
   if ( inx < dx ) {
    if ( inx < 0 ) {
     (*outx)=-((abs(inx)+dx)/dh);
    } else (*outx)=-((dx-inx)/dw);
   } else (*outx)=inx/dw;
  }
  if ( dy < 0 ) {
   if ( iny < dy ) (*outy)=-((iny-dy)/dh);
   else (*outy)=(iny+abs(dy))/dh;
  } else {
   if ( iny < dy ) {
    if ( iny < 0 ) {
     (*outy)=-((abs(iny)+dy)/dh);
    } else (*outy)=-((dy-iny)/dh);
   }
   else (*outy)=iny/dh;
  }
 }
 bool OnScreen()  {
  return (dx > -dw && dy > -dh && dx2 < display.wd+dw && dy2 < display.hd+dh );
 }
 bool OnScreeni() {
  return (x > -w && y > -h && x2 < display.w+w && y2 < display.h+h );
 }
 bool OnScreenf() {
  return (fx > -fw && fy > -fh && fx2 < display.wf+fw && fy2 < display.hf+fh );
 }
 void DrawDebug();
 bool within( int mx, int my ) {
  return ( mx >= x && mx <= x2 && my >= y && my <= y2 );
 }
 bool within( float mx, float my ) {
  return ( mx >= fx && mx <= fx2 && my >= fy && my <= fy2 );
 }
 bool within( double mx, double my ) {
  return ( mx >= dx && mx <= dx2 && my >= dy && my <= dy2 );
 }
 bool within( const Zint& mx, const Zint& my ) {
  return ( mx >= x && mx <= x2 && my >= y && my <= y2 );
 }
 bool within( const Zfloat& mx, const Zfloat& my ) {
  return ( mx >= fx && mx <= fx2 && my >= fy && my <= fy2 );
 }
 bool within( const Zdouble& mx, const Zdouble& my ) {
  return ( mx >= dx && mx <= dx2 && my >= dy && my <= dy2 );
 }
 bool within( ScreenPosition *box ) {
  return (
     WITHIN(box->x,box->y,x,y,x2,y2)
  || WITHIN(box->x,box->y2,x,y,x2,y2)
  || WITHIN(box->x2,box->y2,x,y,x2,y2)
  || WITHIN(box->x2,box->y,x,y,x2,y2)
  );
 }
 double Aspect() {
  return dw/dh;
 }
 double AspectInverse() {
  return dh/dw;
 }
MANY(ScreenPosition,ScreenPositionHandle,ScreenPositionHandles,"ScreenPosition",ScreenPositions,{})
 KEYSWORDSGroups("ScreenPosition",ScreenPosition)
 CLONES(ScreenPosition,ScreenPositions)
DONE(ScreenPosition)