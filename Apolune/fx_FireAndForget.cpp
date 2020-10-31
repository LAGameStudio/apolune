#include "fx_FireAndForget.h"
#include "Globals.h"
#include "Tweens.h"

 void FAFProce55or::EmitPuff( const char *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy ) {
  fx_ExpandContractFader *puff =new fx_ExpandContractFader(
     library.Load(image),
     blend,
     tint,
     A,B,
     0.1f,0.2f);
  puff->duration=duration;
  puff->in=puff->duration/3.0f;
  puff->out=puff->in*2;
  puff->h=h;
  puff->w=w;
  //z*cos q - x*sin q
  //z*sin q + x*cos q
  puff->x=emx;
  puff->y=emy;
  proce55ors->Add(puff);
  puff->direction.Set(dx,dy);
  puff->Init();
 }

 void FAFProce55or::EmitPuff( GLImage *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy ) {
  fx_ExpandContractFader *puff =new fx_ExpandContractFader(
     image,
     blend,
     tint,
     A,B,
     0.1f,0.2f);
  puff->duration=duration;
  puff->in=puff->duration/3.0f;
  puff->out=puff->in*2;
  puff->h=h;
  puff->w=w;
  //z*cos q - x*sin q
  //z*sin q + x*cos q
  puff->x=emx;
  puff->y=emy;
  proce55ors->Add(puff);
  puff->direction.Set(dx,dy);
  puff->Init();
 }

  void FAFProce55or::EmitCinder( GLImage *image, Crayon tint, Blends blend, float in, float out, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy ) {
  fx_ExpandContractBurnFader *puff =new fx_ExpandContractBurnFader(
     image,
     blend,
     tint,
     A,B,
     0.1f,0.2f);
  puff->duration=duration;
  puff->in=in;
  puff->out=out;
  puff->h=h;
  puff->w=w;
  //z*cos q - x*sin q
  //z*sin q + x*cos q
  puff->x=emx;
  puff->y=emy;
  proce55ors->Add(puff);
  puff->direction.Set(dx,dy);
  puff->Init();
 }

 void FAFProce55or::EmitLegacyPuff( GLImage *image, Crayon tint, Blends blend, float duration, int emx, int emy, int w, int h, Tween *A, Tween *B, double dx, double dy ) {
  fx_LegacyExpandContractFader *puff =new fx_LegacyExpandContractFader(
     image,
     blend,
     tint,
     A,B,
     0.1f,0.2f);
  puff->duration=duration;
  puff->in=puff->duration/3.0f;
  puff->out=puff->in*2;
  puff->h=h;
  puff->w=w;
  //z*cos q - x*sin q
  //z*sin q + x*cos q
  puff->x=emx;
  puff->y=emy;
  proce55ors->Add(puff);
  puff->direction.Set(dx,dy);
  puff->Init();
 }

 void FAFProce55or::EmitFilmTrap( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w1, float w2 ) {
  fx_FilmTrap *f=new fx_FilmTrap(film,duration,angle);
  f->blend=blend;
  f->tint.fromCrayon(tint);
  f->spin=spin;
  f->x=emx;
  f->y=emy;
  f->h=h;
  f->w1=w1;
  f->w2=w2;
  proce55ors->Add(f);
  f->Init();
 }

 void FAFProce55or::EmitFilmRotatedRect( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w2 ) {
  fx_FilmRotatedRect *f=new fx_FilmRotatedRect(film,duration,angle);
  f->blend=blend;
  f->tint.fromCrayon(tint);
  f->spin=spin;
  f->x=emx;
  f->y=emy;
  f->h=h;
  f->w2=w2;
  proce55ors->Add(f);
  f->Init();
 }

 void FAFProce55or::EmitSpinner( GLImage *image, Crayon tint, Blends blend, float duration, float in, float out, int emx, int emy, int w, int h, Tween *A, Tween *B, float spin, double dx, double dy ) {
  fx_SpinningExpandingFader *sef=new fx_SpinningExpandingFader;
  sef->image=image;
  sef->blend=blend;
  sef->tint.fromCrayon(tint);
  sef->MoveTo(emx,emy);
  sef->h=h;
  sef->w=w;
  sef->spin=spin;
  sef->xin=A;
  sef->xout=B;
  sef->duration=duration;
  sef->in=in;
  sef->out=out;
  sef->velocity.Set(dx,dy);
  proce55ors->Add(sef);
  sef->Init();
 }

 fx_SpinningExpandingFader::fx_SpinningExpandingFader() : FAFProce55or() {
  tint.Pick(alabaster);
  blend=additive;
  xin=tweens.find("Humanized Gradual Rise");
  xout=tweens.find("Humanized Gradual Fall");
 }

 void fx_ExpandContractFader::Between() {
  expired+=FRAMETIME;
  MoveByd(direction.x,direction.y);
  if ( expired < in ) {
   double instant=expired/in;
   if ( xin ) instant=(double)xin->tweend(instant,true);
   alpha=URANGE(mina,instant,1.0);
  } else if ( expired >= out ) {
   double instant=(expired-out)/(duration-out);
   if ( xout ) instant=(double)xout->tweend(instant,true);
   alpha=URANGE(mina,instant,1.0);
  } else alpha=1.0f;
  if ( blend==transparency ) {
   color.fromCrayon(tint);
   color.Alpha(alpha*(double)tint.af);
  } else {
   color.fromCrayon(tint);
   color.ScaleByAlpha(alpha*(double)tint.af);
  }
//  color.ScaleByAlphaf(expired/duration);
  W=alpha*(double)w; W=CLAMPTO(0.0,W,(double)w);
  H=alpha*(double)h; H=CLAMPTO(0.0,H,(double)h);
  X=(double)x-W/2.0;
  Y=(double)y-H/2.0;
 }
 
 void fx_ExpandContractFader::Render( GLWindow *surface ) {
  if ( expired < duration ) {
   StretchdNoAlpha(image,color,blend,(float)surface->x+X,(float)surface->y+Y,(int)W,(int)H);
  }
 }
 
 void fx_ExpandContractFader::Render() {
  if ( expired < duration ) {
   StretchdNoAlpha(image,color,blend,X,Y,(int)W,(int)H);
  }
 }
 
 void fx_ExpandContractBurnFader::Between() {
  expired+=FRAMETIME;
  angle+=spin;
  MoveByd(direction.x,direction.y);
  if ( expired < in ) {
   double instant=expired/in;
   if ( xin ) instant=(double)xin->tweend(instant,true);
   alpha=(float)URANGE((float)mina,(float)instant,1.0f);
  } else if ( expired >= out ) {
   double instant=(expired-out)/(duration-out);
   if ( xout ) instant=(double)xout->tweend(instant,true);
   alpha=(float)URANGE((float)mina,(float)instant,1.0f);
   if ( burnDelay < 0.0 ) {
    burnDelay=0.1;
    float d=float_range( 0.1f, 0.5f );
    /*
    EmitLegacyPuff( fireballs.AnyIndex(upto(1234)), burnColor, additive,
     float_range(0.1f,0.75f),
     screen.x,screen.y,burnSize,burnSize,
     tweens.find("Humanized Gradual Rise"),
     tweens.find("Humanized Regulated Diminishing Sine"),
     0.0,0.0
    );
    */
//    EmitSpinner( fireballs.AnyIndex(upto(1234)), burnColor, additive,
//     d, d/10.0f, d-(d/10.0f),
//     screen.cx, screen.cy, burnSize,burnSize, xin, xout, float_range(0.1f,3.0f), 0.0, 0.0 );
   } else burnDelay-=FRAMETIME;
  } else alpha=1.0f;
  if ( blend==transparency ) {
   color.fromCrayon(tint);
   color.Alpha(alpha*(double)tint.af);
  } else {
   color.fromCrayon(tint);
   color.ScaleByAlpha(alpha*(double)tint.af);
  }
//  color.ScaleByAlphaf(expired/duration);
  screen.Size(CLAMPTO(0.0,(float)alpha*(float)w,(double)w), CLAMPTO(0.0,(float)alpha*(float)h,(double)h));
  screen.Set((float)x-screen.fw2,(float)y-screen.fh2);
 }