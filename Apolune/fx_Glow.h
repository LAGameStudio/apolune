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

#include "GLWindow.h"
#include "GLImage.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55Controller.h"
#include "Proce55or.h"
#include "Art.h"
#include "Presentation.h"

extern Tweens tweens;

extern int num_glows;

class fx_Glow : public Proce55or {
public:
 Zpointer<GLImage> flare,flare2,flare3;
 Zbool erratic_flaresource, tweenW, tweenH;
 Crayon tint;
 Zint fx,fy,fxT,fyT,
      dx,dy,width,height,w2,h2, tweenLR, tweenUD;
 Crayon c;
 Zdouble maxOp;
 Zdouble H;
 Zpointer<Variable> v, vv;
 Zpointer<Presentation> p;

 fx_Glow() : Proce55or() {
  maxOp=255.0f;
 }

 void Setup()
 {
  p=(Presentation *) parent.pointer;
  H=1.0f/FPS;
  num_glows++;
  if ( maxOp == 0.0f ) maxOp=255.0f;
  if ( (w==0 || h==0) && (w2==0||h2==0) && flare ) {
   w=w2=flare->width;
   h=h2=flare->height;
  }
  if ( erratic_flaresource ) {
   Variable *v;
   if ( !(v=controller.variables.Get("flare")) ) v=controller.variables.Newf("flare",0.0f,2.4f);
   if ( v && !v->tween) v->tween=tweens.find("Humanized In Out Slow");
  }
  if ( !controller.variables.Get("X") ) controller.variables.Newf("X",-1.0f,1.0f);
  if ( !controller.variables.Get("Y") ) controller.variables.Newf("Y",-1.0f,1.0f);
  if ( !controller.variables.Get("Z") ) controller.variables.Newf("Z",0.0f,(float)maxOp);
  if ( !controller.variables.Get("W") ) controller.variables.Newf("W",-1.0f,1.0f);
  if ( !controller.variables.Get("H") ) controller.variables.Newf("H",-1.0f,1.0f);
  v=this->controller.variables.Get("Z");
  vv=this->controller.variables.Get("flare");
 }

 void Between() {
  double perc=(p->expired-delay)/duration;
  this->controller.Updatef();
  width=iround((double)w+(double)(w2-w)*perc); 
  height=iround((double)h+(double)(h2-h)*perc);
  if ( tweenW ) width=(int) ((float)width*this->controller.variables.Getf("W"));
  if ( tweenH ) height=(int) ((float)height*this->controller.variables.Getf("H"));
  fx=iround( (double)x+(double) dx*perc ); //OUTPUT( "Glow perc: %f %f/%f\n", perc, expired, duration ); 
  fy=iround( (double)y+(double) dy*perc );
  fxT=(int) (fx+(float)tweenLR* this->controller.variables.Getf("X"));
  fyT=(int) (fy+(float)tweenUD* this->controller.variables.Getf("Y"));
 }

 void Draw(GLWindow *surface)
 {
   //if ( !flare.bitmap ) { OUTPUT( "MISSING BITMAP!!!!\n" ); return; } // <- boo!
//   if ( v && v->tween ) {
    Crayon t;
    t.fromCrayon(tint);    
    t.ScaleByAlpha( v->f );
    c.fromCrayon(t);
  //  printf ( "%f :: ", controller.Zf );
  //  OUTPUT( "%d, %d, %d : ", (byte) tint.r, (byte) tint.g, (byte) tint.b );
  //  OUTPUT( "%d, %d, %d\n", (byte) c.r, (byte) c.g, (byte) c.b );
//   } else c=tint;
   if ( /*vv &&*/ erratic_flaresource ) {
    GLImage *myImage;
    switch ( ((int) vv->f) % 10 ) {
      case 1: myImage=flare2; break;
      case 2: myImage=flare3; break;
     default: myImage=flare; break;
    }
    if ( myImage == null ) myImage=flare;
    if (myImage) Stretchi(surface,myImage,c,v->f,additive,fxT,fyT,width,height);
   } else 
   if ( flare ) StretchiNoAlpha(surface,flare,c,additive,fxT,fyT,width,height);   
 }

 void Clean() {
 }
};


class fx_Glow2 : public Proce55or {
public:
 Zpointer<GLImage> flare,flare2,flare3;
 Zbool erratic_flaresource, tweenW, tweenH;
 Crayon tint;
 Zint fx,fy,fxT,fyT,
      dx,dy,width,height,w2,h2, tweenLR, tweenUD;
 Crayon c;
 Zdouble maxOp;
 Zdouble H;
 Zpointer<Variable> v, vv;
 Blends blend;

 fx_Glow2() : Proce55or() {
  maxOp=255.0;
  blend=additive;
 }

 void Setup()
 {
  H=FRAMETIME;
  num_glows++;
  if ( maxOp == 0.0 ) maxOp=255.0;
  if ( (w==0 || h==0) && (w2==0||h2==0) && flare ) {
   w=w2=flare->width;
   h=h2=flare->height;
  }
  if ( erratic_flaresource ) {
   Variable *v;
   if ( !(v=controller.variables.Get("flare")) ) v=controller.variables.Newf("flare",0.0f,2.4f);
   if ( !v->tween) v->tween=tweens.find("Humanized In Out Slow");
  }
  if ( !controller.variables.Get("X") ) controller.variables.Newf("X",-1.0f,1.0f);
  if ( !controller.variables.Get("Y") ) controller.variables.Newf("Y",-1.0f,1.0f);
  if ( !controller.variables.Get("Z") ) controller.variables.Newf("Z",0.0f,(float)maxOp);
  if ( !controller.variables.Get("W") ) controller.variables.Newf("W",-1.0f,1.0f);
  if ( !controller.variables.Get("H") ) controller.variables.Newf("H",-1.0f,1.0f);
  v=this->controller.variables.Get("Z");
  vv=this->controller.variables.Get("flare");
 }

 void Between() {
  if ( hidden ) return;
  expired+=FRAMETIME;
  double perc=(expired-delay)/duration;
  this->controller.Updatef();
  width=iround((double)w+(double)(w2-w)*perc); 
  height=iround((double)h+(double)(h2-h)*perc);
  if ( tweenW ) width=(int) ((float)width*this->controller.variables.Getf("W"));
  if ( tweenH ) height=(int) ((float)height*this->controller.variables.Getf("H"));
  fx=iround( (double)x+(double) dx*perc ); //OUTPUT( "Glow perc: %f %f/%f\n", perc, expired, duration ); 
  fy=iround( (double)y+(double) dy*perc );
  fxT=(int) (fx+(float)tweenLR* this->controller.variables.Getf("X"));
  fyT=(int) (fy+(float)tweenUD* this->controller.variables.Getf("Y"));
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface)
 {
  if ( hidden ) return;
   //if ( !flare.bitmap ) { OUTPUT( "MISSING BITMAP!!!!\n" ); return; } // <- boo!
//   if ( v && v->tween ) {
    Crayon t;
    t.fromCrayon(tint);    
    t.ScaleByAlpha( v->f );  // crashed here?  Was the parent class (probably Presentation) not Init()ed?
    c.fromCrayon(t);
  //  printf ( "%f :: ", controller.Zf );
  //  OUTPUT( "%d, %d, %d : ", (byte) tint.r, (byte) tint.g, (byte) tint.b );
  //  OUTPUT( "%d, %d, %d\n", (byte) c.r, (byte) c.g, (byte) c.b );
//   } else c=tint;
   if ( /*vv &&*/ erratic_flaresource ) {
    GLImage *myImage;
    switch ( ((int) vv->f) % 10 ) {
      case 1: myImage=flare2; break;
      case 2: myImage=flare3; break;
     default: myImage=flare; break;
    }
    if ( myImage == null ) myImage=flare;
   if ( myImage ) Stretchi(surface,myImage,c,v->f,blend,fxT,fyT,width,height);
   } else 
   if ( flare ) StretchiNoAlpha(surface,flare,c,blend,fxT,fyT,width,height);
   
 }

 void Clean() {
 }
};


class fx_Glow3 : public Proce55or {
public:
 Zpointer<GLImage> flare;
 Crayon tint;
 Zint fx,fy,
      dx,dy,width,height,w2,h2;
 Crayon c;
 Zdouble maxOp;
 Zdouble H;
 Blends blend;
 Zpointer<Variable> v;

 fx_Glow3() : Proce55or() {
  maxOp=255.0;
  blend=additive;
 }

 void Setup()
 {
  H=FRAMETIME;
  if ( maxOp == 0.0 ) maxOp=255.0;
  if ( (w==0 || h==0) && (w2==0||h2==0) && flare ) {
   w=w2=flare->width;
   h=h2=flare->height;
  }
  if ( !controller.variables.Get("Z") ) controller.variables.Newf("Z",0.0f,(float)maxOp);
  v=this->controller.variables.Get("Z");
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
  expired+=FRAMETIME;
  double perc=(expired-delay)/duration;
  width=iround((double)w+(double)(w2-w)*perc); 
  height=iround((double)h+(double)(h2-h)*perc);
  fx=iround( (double)x+(double) dx*perc ); //OUTPUT( "Glow perc: %f %f/%f\n", perc, expired, duration ); 
  fy=iround( (double)y+(double) dy*perc );
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface)
 {
  if ( hidden ) return;
  Crayon t;
  t.fromCrayon(tint);    
  t.ScaleByAlphaf( v->f );  // crashed here?  Was the parent class (probably Presentation) not Init()ed?
  if ( flare ) StretchiNoAlpha(surface,flare,t,blend,fx,fy,width,height);
 }

 void Clean() {
 }
};

class fx_Image : public Proce55or {
public:
 Zp<GLImage> image;
 Crayon tint;
 Blends blend;

 fx_Image() : Proce55or() {
  blend=additive;
 }

 void Setup()
 { 
  blend=additive;
 }

 void Between() {
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface)
 {
   if ( hidden ) return;
   if ( image ) StretchiNoAlpha(surface,image,tint,blend,x,y,w,h);
 }

 void Clean() {
 }
};