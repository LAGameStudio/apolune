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
#include "Art.h"
#include "Proce55or.h"
#include "Vertex.h"
#include "Film.h"
#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"
#include "Film.h"

/*
 * These are all "fire and forget" proce55ors, and they are for use with the "FireAndForget" prefixed proce55or renderers
 * (branching from FireAndForgetUnlimitedWrappingProce55Viewer2d).  FireAndForget* proce55ors introduce a few new
 * coding standards to the Proce55or line: specifically cloning with possible mutation, 
 * required duration and expiration, "wrapped" sub-proce55ors.
 *
 * They define their respective "Done()" queries to tell the FAF proce55or to drop them (they die).
 */

class FAFProce55or : public Proce55or {
public:
 FAFProce55or() : Proce55or() {}
 virtual void MoveBy( int xx, int yy ) {
  x+=xx;
  x2+=xx;
  y+=yy;
  y2+=yy;
  position.MoveBy(x,y);
  WhenMoved( (double) xx, (double) yy );
 }
 Cartesiand excess;
 virtual void MoveByd( double xx, double yy ) {
  double dx=xx+excess.x;
  double dy=yy+excess.y;
  int ix=(int) (dx), iy=(int) (dy);
  excess.Set( dx - (double) ix, dy - (double) iy );
  MoveBy(ix,iy);
  position.MoveBy(x,y);
 }
 virtual void WhenMoved( double x, double y ) {}
 virtual void MoveTo( int xx, int yy ) {
  x=xx;
  y=yy;
  x2=xx+w;
  y2=yy+h;
  WhenMovedTo(x,y);
  position.Set((double)xx,(double)yy);
 }
 virtual void WhenMovedTo( double x, double y ) {}
 virtual void Between() { expired+=FRAMETIME; }
 // Creates a clone (used when the proce55or is being treated as a template)
 virtual void OnExpired() {
  dropMe=true;
 }
 void EmitPuff( const char *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy );
 void EmitPuff( GLImage *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy );
 void EmitLegacyPuff( GLImage *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy );
 void EmitCinder( GLImage *image, Crayon tint, Blends blend, float in, float out, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy );
 void EmitFilmTrap( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w1, float w2 );
 void EmitFilmRotatedRect( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w2 );
 void EmitSpinner( GLImage *image, Crayon tint, Blends blend, float duration, float in, float out, int emx, int emy, int w, int h, Tween *A, Tween *B, float spin, double dx, double dy );
};

class fx_SpinningExpandingFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Crayon tint;
 Blends blend;
 Zp<Tween> xin, xout;
 Zdouble in,endIn,startOut,out,instant;
 Crayon color;
 Zdouble wh,angle,spin;
 fx_SpinningExpandingFader();
 void WhenMoved( double x, double y ) {
 }
 void WhenMovedTo( double x, double y ) {
 }
 void Between() {
  expired+=FRAMETIME;
  if ( expired <= in ) {
   instant=xin->tweend(expired/(float)in);
   endIn=instant;
  } else if ( expired <= out ) {
   instant=((double)expired-in)/(out-in);
   startOut=xout->tweenf(0.0f);
   instant=instant*(startOut-endIn);
  } else {
   instant=xout->tweenf((float)(((double)expired.value-out)/(duration.value-out)));
  }
  color.fromCrayon(tint);
  if ( blend==transparency ) {
   color.Alpha((float)instant);
  } else {
   color.ScaleByAlphaf((float)instant);
  }
  wh=((float)(w+h)/2.0f)*(float)((double)expired.value/duration.value);
  angle+=spin;
  position.Add(velocity.x,velocity.y,velocity.z);
 }
 void Render( GLWindow *surface ) {
  PivotedRotatedRectf(image,color,blend,(float)position.x,(float)position.y,0.0f,0.0f,(float)wh,(float)wh,(float)angle);
 }
 void Render() {
  PivotedRotatedRectf(image,color,blend,(float)position.x,(float)position.y,0.0f,0.0f,(float)wh,(float)wh,(float)angle);
 }
};

#include "MapPosition.h"

class ZonedFAFProce55or : public FAFProce55or {
public:
 MapPosition map;
 ScreenPosition screen;
 Cartesiand center;
 Cartesiand centerBottom;
 ScreenPosition translated;
 ZonedFAFProce55or() : FAFProce55or() {
  unRezoned=true;
 }
 virtual void SetPosition( double x, double y ) {
  screen.MoveTo(x,y);
  center.x=centerBottom.x=screen.cx;
  center.y=screen.cy;
  collision2d.Set((int)center.x,(int)center.y);
  centerBottom.y=screen.y+screen.h;
  map.position.x=(double)x;
  map.position.y=(double)y;
  this->x=(int)x;
  this->y=(int)y;
  this->x2=(int)screen.x2;
  this->y2=(int)screen.y2;
  this->w=(int)w;
  this->h=(int)h;
  Rezone();
 }
 virtual void SetPosition( double x, double y, double w, double h ) {
  screen.Set(x,y,w,h);
  center.x=centerBottom.x=screen.cx;
  center.y=screen.cy;
  centerBottom.y=screen.y+screen.h;
  map.position.x=(double)x;
  map.position.y=(double)y;
  this->x=(int)x;
  this->y=(int)y;
  this->x2=(int)screen.x2;
  this->y2=(int)screen.y2;
  this->w=(int)w;
  this->h=(int)h;
  Rezone();
 }
 void MoveBy( int xx, int yy ) {
  SetPosition(map.position.x+(double)xx,map.position.y+(double)yy);
 }
 void MoveByd( double xx, double yy ) {
  SetPosition(map.position.x+xx,map.position.y+yy);
 }
 void MoveBy( double xx, double yy ) {
  SetPosition(map.position.x+xx,map.position.y+yy);
 }
 virtual void Translate( ScreenPosition *screen ) {
  translated.Set(
   this->screen.x-screen->x,
   this->screen.y-screen->y,
   this->screen.w,
   this->screen.h
   );
 }
 bool OnScreenTranslated() {
  return ( translated.x > -translated.w && translated.x2 < display.wd+translated.w )
      && ( translated.y > -translated.h && translated.y2 < display.hd+translated.h );
 }
 virtual int Y() {  return (int) ( translated.y+translated.h ); }
};

/*
 * Expands from a point at deltaWidth, simultaneously fading out with a tween.
 */
class fx_ExpandFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint;
 Zpointer<Tween> xin;
 Zdouble in,out;
 Zdouble alpha,dw,W,X,Y;
 Crayon color;
 fx_ExpandFader( GLImage *im, Blends b, Crayon t, Tween *i, double deltaWidth, double in, double out ) : FAFProce55or() {
  xin=i;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  tint.fromCrayon(t);
  dw=deltaWidth;
  W=0.001f;
 }
 void Between() {
  expired+=FRAMETIME;
  alpha=xin->tweend(expired/in);
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  W+=dw;
  X=(double)x-W/2.0f;
  Y=(double)y-W/2.0f;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,(int)X,(int)Y,(int)W,(int)W);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,(int)X,(int)Y,(int)W,(int)W);
 }
};

/*
 * Expands from a point at deltaWidth, simultaneously fading out with a tween.
 */
class fx_WanderingExpandFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint;
 Zpointer<Tween> xin;
 Zdouble in,out;
 Zdouble alpha,dw,W,X,Y;
 Cartesiand velocity;
 Crayon color;
 fx_WanderingExpandFader( GLImage *im, Blends b, Crayon t, Tween *i, double deltaWidth, double in, double out ) : FAFProce55or() {
  xin=i;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  tint.fromCrayon(t);
  dw=deltaWidth;
  W=0.001f;
 }
 void Between() {
  expired+=FRAMETIME;
  alpha=xin->tweend(expired/in);
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  W+=dw;
  x+=velocity.x;
  y+=velocity.y;
  X=(float)x-W/2.0f;
  Y=(float)y-W/2.0f;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,(int)X,(int)Y,(int)W,(int)W);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,(int)X,(int)Y,(int)W,(int)W);
 }
};

/*
 * Expands from a point, then contracts, for a duration,
 * using tweens both to control size and to fade in and out.
 */
class fx_ExpandContractFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint;
 Zp<Tween> xin, xout;
 Zdouble in,out;
 Zdouble alpha,W,H,W2,H2,X,Y,mina;
 Cartesiand direction;
 Crayon color;
 fx_ExpandContractFader( GLImage *im, Blends b, Crayon t, Tween *i, Tween *o, double in, double out ) : FAFProce55or() {
  xin=i;
  xout=o;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  tint.fromCrayon(t);
  mina=1.0f/255.0f;
 }
 void Between();
 void Render( GLWindow *surface );
 void Render();
};


/* LEGACY
 * Expands from a point, then contracts, for a duration,
 * using tweens both to control size and to fade in and out.
 */
class fx_LegacyExpandContractFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint;
 Zpointer<Tween> xin, xout;
 Zdouble in,out;
 Zdouble alpha,W,H,W2,H2,X,Y;
 Cartesiand direction;
 Crayon color;
 fx_LegacyExpandContractFader( GLImage *im, Blends b, Crayon t, Tween *i, Tween *o, double in, double out ) : FAFProce55or() {
  xin=i;
  xout=i;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  tint.fromCrayon(t);
 }
 void Between() {
  expired+=FRAMETIME;
  MoveByd(direction.x,direction.y);
  if ( expired < in ) alpha=xin->tweend(expired/in);
  else if ( expired < out ) alpha=1.0f;
  else alpha=(double)URANGE(0.1f,xout->tweend((expired-out)/(duration-out)),1.0f);
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)(1.0-alpha)*tint.af);
//  color.ScaleByAlphaf(expired/duration);
  W=(double)alpha*(double)w; W=CLAMPTO(0.0f,W,(double)w);
  H=(double)alpha*(double)h; H=CLAMPTO(0.0f,H,(double)h);
  X=(double)x-W/2.0f;
  Y=(double)y-H/2.0f;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
};


/*
 * Expands from a point, then contracts, burning, for a duration,
 * using tweens both to control size and to fade in and out.
 */
class fx_ExpandContractBurnFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint,burnColor;
 Zp<Tween> xin, xout;
 Zdouble in,out;
 Zdouble alpha,mina;
 ScreenPosition screen;
 Cartesiand direction;
 Crayon color;
 Zdouble angle,spin;
 Zdouble burnDelay;
 Zint burnSize;
 fx_ExpandContractBurnFader( GLImage *im, Blends b, Crayon t, Tween *i, Tween *o, double in, double out ) : FAFProce55or() {
  xin=i;
  xout=o;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  burnSize=16;
  burnColor.Brights(upto(12345));
  tint.fromCrayon(t);
  mina=1.0f/255.0f;
 }
 void WhenMoved( double x, double y ) {
  screen.MoveBy(x,y);
 }
 void Between();
 void Render( GLWindow *surface ) {
  if ( expired < duration ) {
   PivotedRotatedRectf(image,color,blend,screen.fx,screen.fy,0.0f,0.0f,screen.fh2,screen.fw2,(float)angle);
  }
 }
 void Render() {
  if ( expired < duration ) {
   PivotedRotatedRectf(image,color,blend,screen.fx,screen.fy,0.0f,0.0f,screen.fh2,screen.fw2,(float)angle);
  }
 }
};

/*
 * Expands from a point, then contracts, for a duration,
 * using tweens both to control size and to fade in and out.
 */
class fx_ExpandContractFaderFilm : public FAFProce55or {
public:
 Zp<Film> film;
 Blends blend;
 Crayon tint;
 Zpointer<Tween> xin, xout;
 Zdouble in,out;
 Zdouble alpha,W,H,W2,H2,X,Y,mina;
 Cartesiand direction;
 Crayon color;
 fx_ExpandContractFaderFilm( Film *f, Blends b, Crayon t, Tween *i, Tween *o, double in, double out ) : FAFProce55or() {
  xin=i;
  xout=i;
  this->in=in;
  this->out=out;
  this->duration=1.5f;
  film=f;
  blend=b;
  tint.fromCrayon(t);
  mina=0.01f;
 }
 bool Done() { return expired>=duration; }
 void Between() {
  expired+=FRAMETIME;
  MoveByd(direction.x,direction.y);
  if ( expired < in ) alpha=xin->tweend(expired/in);
  else if ( expired < out ) alpha=1.0;
  else alpha=URANGE(mina,xout->tweend((expired-out)/(duration-out)),1.0);
  color.fromCrayon(tint);
  color.ScaleByAlphaf(1.0f-(float)alpha);
//  color.ScaleByAlphaf(expired/duration);
  W=alpha*(double)w;
  H=alpha*(double)h;
  X=(double)x-W/2.0;
  Y=(double)y-H/2.0;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,film->frame(expired/duration),color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
 void Render() {
  StretchiNoAlpha(film->frame(expired/duration),color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
};

/*
 * Flickers two images (in, out) where intensity and size is controlled by tweens.
 */
class fx_ExpandContract2Images : public FAFProce55or {
public:
 Zp<GLImage> A,B,image;
 Crayon tint;
 Blends blend;
 Zpointer<Tween> xin, xout;
 Zdouble in;
 Zdouble alpha,W,H,W2,H2,X,Y;
 Crayon color;
 Cartesiand velocity;
 fx_ExpandContract2Images( GLImage *_a, GLImage *_b, Blends b, Crayon t, Tween *i, Tween *o, double zenith ) : FAFProce55or() {
  xin=i;
  xout=i;
  in=zenith;
  image=_a;
  A=_a;
  B=_b;
  blend=b;
  tint.fromCrayon(t);
 }
 void Between() {
  expired+=FRAMETIME;
  if ( expired < in ) {
   alpha=xin->tweend(expired/duration);
   image=A;
  } else {
   alpha=xout->tweend((expired-in)/duration);
   image=B;
  }
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  x+=(int)velocity.x;
  y+=(int)velocity.y;
  W=alpha*(double)w;
  H=alpha*(double)h;
  X=x-W/2.0;
  Y=y-H/2.0;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,x,y,w,h);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,x,y,w,h);
 }
};

/*
 * Expands from a point, then contracts, for a duration,
 * using tweens to fade in and out. (Not finished.)
 */
class fx_SpinningExpandContractFader : public FAFProce55or {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon tint;
 Zpointer<Tween> xin, xout;
 Zdouble in,out;
 Zdouble alpha,W,H,W2,H2,X,Y;
 Crayon color;
 Cartesiand velocity;
 fx_SpinningExpandContractFader( GLImage *im, Blends b, Crayon t, Tween *i, Tween *o, double in, double out ) : FAFProce55or() {
  xin=i;
  xout=i;
  this->in=in;
  this->out=out;
  image=im;
  blend=b;
  tint.fromCrayon(t);
 }
 void Between() {
  expired+=FRAMETIME;
  if ( expired < in ) alpha=xin->tweend(expired/in);
  else if ( expired < out ) alpha=1.0;
  else alpha=xout->tweend((expired-out)/in);
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  x+=velocity.x;
  y+=velocity.y;
  W=alpha*(double)w;
  H=alpha*(double)h;
  X=(double)x-W/2.0f;
  Y=(double)y-H/2.0f;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,(int)X,(int)Y,(int)W,(int)H);
 }
};

/*
 * Simply displays an image controlled by two tweens (in, out).  "Zenith" defines timeline point.
 */
class fx_Flicker : public FAFProce55or {
public:
 Zp<GLImage> image;
 Crayon tint;
 Blends blend;
 Zpointer<Tween> xin, xout;
 Zdouble in;
 Zdouble alpha;
 Crayon color;
 Cartesiand velocity;
 fx_Flicker( GLImage *im, Blends b, Crayon t, Tween *i, Tween *o, double zenith ) : FAFProce55or() {
  xin=i;
  xout=i;
  in=zenith;
  image=im;
  blend=b;
  tint.fromCrayon(t);
 }
 void Setup() {
 }
 void Between() {
  expired+=FRAMETIME;
  if ( expired < in ) {
   alpha=xin->tweend(expired/(duration-in));
  } else {
   alpha=xout->tweend((expired-in)/duration);
  }
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  x+=velocity.x;
  y+=velocity.y;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,x-w/2,y-h/2,w,h);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,x-w/2,y-h/2,w,h);
 }
};

/*
 * Flickers two images (in, out) where intensity is controlled by tweens.
 */
class fx_Flicker2Images : public FAFProce55or {
public:
 Zp<GLImage> A,B,image;
 Crayon tint;
 Blends blend;
 Zpointer<Tween> xin, xout;
 Zdouble in;
 Zdouble alpha;
 Crayon color;
 Cartesiand velocity;
 fx_Flicker2Images( GLImage *_a, GLImage *_b, Blends b, Crayon t, Tween *i, Tween *o, float zenith ) : FAFProce55or() {
  xin=i;
  xout=i;
  in=zenith;
  image=_a;
  A=_a;
  B=_b;
  blend=b;
  tint.fromCrayon(t);
 }
 void Between() {
  expired+=FRAMETIME;
  if ( expired < in ) {
   alpha=xin->tweend(expired/duration);
   image=A;
  } else {
   alpha=xout->tweend((expired-in)/duration);
   image=B;
  }
  color.fromCrayon(tint);
  color.ScaleByAlphaf((float)alpha);
  x+=velocity.x;
  y+=velocity.y;
 }
 void Render( GLWindow *surface ) {
  StretchiNoAlpha(surface,image,color,blend,x-w/2,y-h/2,w,h);
 }
 void Render() {
  StretchiNoAlpha(image,color,blend,x-w/2,y-h/2,w,h);
 }
};

/*
 * This Proce55or controls motion only of a sub-processor.
 * Uses brownian motion to control a FAF.
 */
class fx_Fizzle : public FAFProce55or {
public:
 Zdis<Proce55or> content;
 Zint X,X2,Y,Y2;
 fx_Fizzle( Proce55or *faffer, int xrange, int yrange ) : FAFProce55or() {
  content=faffer;
  X=xrange*2;
  X2=xrange;
  Y=yrange*2;
  Y2=yrange;
 }
 void Between() {
  expired+=FRAMETIME;
  content->MoveBy(number_range(0,X)-X2,number_range(0,Y)-Y2);
  content->Between();
 }
 void Render() { content->Render(); }
 void Init() { content->Init(); }
 void Setup() { content->Setup(); }
 void Clean() { content.Deinstantiate(); }
 bool Done() { return content->Done(); }
};

/*
 * This Proce55or controls motion only of a sub-processor.
 * Make a FAF fly off in a direction at a constant velocity.
 */
class fx_Shoot : public FAFProce55or {
public:
 Zdis<Proce55or> content;
 Zfloat expired;
 Zfloat dx,dy;
 fx_Shoot( Proce55or *faffer, float _cos, float _sin ) : FAFProce55or() {
  content=faffer;
  dx=_cos;
  dy=_sin;
 }
 Vertexd excess;
 void Between() {
  expired+=FRAMETIME;
  float fx=dx+(float) excess.x, fy=dy+(float) excess.y;
  int ix=(int) fx, iy=(int) fy;
  excess.Set( fx - (float) ix, fy - (float) iy );
  content->MoveBy(ix,iy);
  content->Between();
 }
 void Render() { content->Render(); }
 void Init() { content->Init(); }
 void Setup() { content->Setup(); }
 void Clean() { content.Deinstantiate(); }
 bool Done() { return content->Done(); }
};


class fx_FilmRotated : public FAFProce55or {
public:
 Zp<Film> film;
 Zfloat angle,spin;
 Vertexd direction;
 Crayon tint;
 Blends blend;
 fx_FilmRotated( Film *f, float duration, float angle ) : FAFProce55or() {
  expired=0.0f;
  film=f;
  blend=additive;
  tint.Pick(alabaster);
  this->angle=angle;
  this->duration=duration;
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
 }
 void Render( GLWindow *surface ) {
  RotatedRectf(film->frame(expired/duration),tint,blend,(float)x,(float)y,(float)h,(float)w,angle);
 }
 bool Done() {
  return expired>=duration;
 }
};

class fx_FilmPivoted : public FAFProce55or {
public:
 Zp<Film> film;
 Zfloat h2,w2,angle,spin;
 Vertexd direction;
 Zfloat px,py;
 Crayon tint;
 Blends blend;
 fx_FilmPivoted( Film *f, float duration, float angle ) : FAFProce55or() {
  expired=0.0f;
  film=f;
  blend=additive;
  tint.Pick(alabaster);
  this->angle=angle;
  this->duration=duration;
  px=py=0.0f;
  h2=w2=32.0f;
  spin=0.0f;
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
 }
 void Render( GLWindow *surface ) {
  PivotedRotatedRectf(film->frame(expired/duration),tint,blend,(float)x,(float)y,px,py,h2,w2,angle);
 }
 bool Done() {
  return expired>=duration;
 }
};

class fx_FilmTrap : public FAFProce55or {
public:
 Zp<Film> film;
 Zfloat angle,spin,w1,w2;
 Vertexd direction;
 Crayon tint;
 Blends blend;
 fx_FilmTrap( Film *f, float duration, float angle ) : FAFProce55or() {
  film=f;
  blend=additive;
  tint.Pick(alabaster);
  this->angle=angle;
  this->duration=duration;
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
 }
 void Render( GLWindow *surface ) {
  RotatedIsotrapf(film->frame(expired/duration),tint,blend,(float)x,(float)y,(float)h,w1,w2,angle);
 }
 bool Done() {
  return expired>=duration;
 }
};

class fx_IsoTrap : public FAFProce55or {
public:
 Zp<GLImage> i;
 Zdouble angle,spin,w1,w2,in,out;
 Vertexd direction;
 Crayon color;
 Crayon tint;
 Blends blend;
 Zbool fade;
 fx_IsoTrap( GLImage *i, float duration, float angle ) : FAFProce55or() {
  this->i=i;
  blend=additive;
  color.Pick(alabaster);
  tint.Pick(alabaster);
  this->angle=angle;
  this->duration=duration;
  in=0.1f;
  out=0.25f;
  fade=true;
 }
 void Tint( Crayon c ) {
  color.fromCrayon(c);
  tint.fromCrayon(c);
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
  if ( fade ) {
   double tenthdur=duration*in;
   if ( expired < tenthdur ) {
     tint.fromCrayon(color);
     tint.ScaleByAlphaf( (float)( expired/tenthdur ) );
   } else {
    double qtrdur=duration*out;
    double uplimit=duration-qtrdur;
    if ( expired > uplimit ) {
     tint.fromCrayon(color);
     tint.ScaleByAlphaf( 1.0f-(float)((expired-uplimit)/qtrdur) );
    }
   }
  }
 }
 void Render( GLWindow *surface ) {
  RotatedIsotrapd(i,tint,blend,(double)x,(double)y,(double)h,w1,w2,angle);
 }
 bool Done() {
  return expired>=duration;
 }
};

class fx_FilmRotatedRect : public FAFProce55or {
public:
 Zp<Film> film;
 Zfloat angle,spin,w2;
 Vertexd direction;
 Cartesian pivot;
 Crayon tint;
 Blends blend;
 fx_FilmRotatedRect( Film *f, float duration, float angle ) : FAFProce55or() {
  film=f;
  blend=additive;
  tint.Pick(alabaster);
  this->angle=angle;
  this->duration=duration;
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
 }
 void Render( GLWindow *surface ) {
  PivotedRotatedRectf(
   film->frame(
   expired/duration),
   tint,blend,
   (float)x,(float)y,pivot.fx,pivot.fy,(float)(h/2),w2,angle);
 }
 bool Done() {
  return expired>=duration;
 }
};

class fx_FilmTrapLoop : public FAFProce55or {
public:
 Zp<Film> film;
 Zdouble angle,spin,w1,w2;
 Zdouble loopDuration;
 Zint numLoops,totalLoops;
 Vertexd direction;
 Crayon tint;
 Blends blend;
 fx_FilmTrapLoop( Film *f, float loopDuration, int loops, float angle ) : FAFProce55or() {
  expired=0.0f;
  film=f;
  blend=additive;
  tint.Pick(alabaster);
  this->loopDuration=loopDuration;
  this->angle=angle;
  this->duration=loopDuration*(float)loops;
  numLoops=loops;
  totalLoops=loops;
  w1=w2=0.0f;
  spin=0.0f;
 }
 void Between() {
  angle+=spin;
  MoveByd(direction.x,direction.y);
  expired+=FRAMETIME;
  if ( expired >= loopDuration ) {
   expired=0.0f;
   numLoops--;
  }
 }
 void Render( GLWindow *surface ) {
  if ( numLoops == 1 ) {
   Crayon color;
   color.fromCrayon(tint);
   float perc=1.0f-(float)(expired/loopDuration);
   if ( blend == transparency ) {
    color.Alpha(perc);
   } else {
    color.ScaleByAlphaf((float)perc);
   }
   RotatedIsotrapd(film->frame(expired/loopDuration),color,blend,(double)x,(double)y,(double)h*perc,w1*perc,w2*perc,angle);
  } else if ( numLoops == totalLoops ) {
   double perc=(expired/loopDuration);
   Crayon color;
   color.fromCrayon(tint);
   if ( blend == transparency ) {
    color.Alpha(perc);
   } else {
    color.ScaleByAlphaf((float)perc);
   }
   RotatedIsotrapd(film->frame(expired/loopDuration),color,blend,(double)x,(double)y,(double)h*perc,w1*perc,w2*perc,angle);
  } else if ( numLoops != 0 ) {
   RotatedIsotrapd(film->frame(expired/loopDuration),tint,blend,(double)x,(double)y,(double)h,w1,w2,angle);
  }
 }
 bool Done() {
  return numLoops<=0;
 }
};

class ZInlineRenderFAF : public FAFProce55or {
public:
 Zp<ScreenPosition> screen;
 ZInlineRenderFAF() : FAFProce55or() { screen=null; }
 virtual void RenderRelative( GLWindow *surface, ScreenPosition *offset ) {
  screen=offset;
  if ( !hidden ) RenderThis();
 }
 virtual void Between() {}
 virtual void RenderThis() {}
 virtual int Y() {  return screen ? (int) ((screen->y-y)+(screen->h)) : (display.h2); }
};

class ZonedZInlineRenderFAF : public ZonedFAFProce55or {
public:
 Zp<ScreenPosition> screen;
 ZonedZInlineRenderFAF() : ZonedFAFProce55or() { screen=null; }
 virtual void RenderRelative( GLWindow *surface, ScreenPosition *offset ) {
  screen=offset;
  if ( !hidden ) RenderThis();
 }
 virtual void Between() {}
 virtual void RenderThis() {}
 virtual int Y() {  return screen ? (int) ((screen->y-y)+(screen->h)) : (display.h2); }
};