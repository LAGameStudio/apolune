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
#include "Crayon.h"
#include "Tweens.h"
#include "Film.h"
#include "Proce55or.h"

extern Tweens tweens;

class fx_Film : public Proce55or {
public:
 Zp<Film> film;
};

class fx_FilmLoop : public Proce55or {
public:
 Zp<Film> film;
};

class fx_FilmLoopFader : public Proce55or {
public:
 Zp<Film> film;
};

class fx_FilmLineFader : public Proce55or {
public:
 Zpointer<Tween> in, out;
 Zdouble inTime,outTime,instant;
 Blends blend;
 Crayon tint;
 Cartesian calculator;
 Zpointer<Film> film;
 Zpointer<GLImage> frame;
 Zint w2;
 fx_FilmLineFader() : Proce55or() {
  in=tweens.find("Humanized Gradual Rise");
  out=tweens.find("Humanized Showdown");
  inTime=0.25;
  outTime=0.75;
  duration=1.0;
 }
 bool Done() {
  return ( expired > duration );
 }
 Crayon c;
 void Recalc() {
  w2=w/2;
  calculator.x=x;
  calculator.y=y;
  calculator.x2=x2=x+w;
  calculator.y2=y2=y+h;
  calculator.w=w/2;
  calculator.h=h;
  calculator.a=calculator.LineAngle();
  calculator.fh=fdistance( (float) calculator.x, (float) calculator.y, (float) calculator.x2, (float) calculator.y2 );
 }
 void Setup() {
  instant=0.0f;
  Recalc();
 }
 void Reset() {
  instant=0.0f;
 }
 void Between() {
  expired+=FRAMETIME;
  instant=
   ( duration < inTime ? in->tween[(int)((inTime-expired)*(double)in->tween.length)] 
   : duration < outTime ? 1.0f
   : out->tween[(int)((expired-outTime)*(double)out->tween.length)] ) 
   ;
  c.fromCrayon(tint);
  c.ScaleByAlpha(instant);
  frame=!film?null:film->frames.element((int)((expired/duration)*(double)film->frames.count));
 }
 void Render() {
  Blending(blend);
  if ( frame ) TextureLine(this->parent,frame,tint,blend,&calculator,w2);
 }
 void Render( GLWindow *surface ) {
  Blending(blend);
  if ( frame ) TextureLine(surface,frame,tint,blend,&calculator,w2);
 }
 void MoveBy( int xx, int yy ) {
  x+=xx;
  y+=yy;
  Recalc();
 }
};

class fx_MultiFilm : public Proce55or {
public:
};

class fx_FilmIsotrapf : public Proce55or {
public:
 Zp<Film> film;
};