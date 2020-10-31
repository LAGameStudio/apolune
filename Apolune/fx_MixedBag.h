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

#include "Polygon.h"

#include "moremath.h"

class fx_FadeIn : public Proce55or {
public:
 Crayon tint;
 Zpointer<Presentation> p;
 void Setup() {
  p=(Presentation *) parent.pointer;
 }

 void Draw(GLWindow *surface) {
  tint.Float( tint.rf, tint.gf, tint.bf, (255.0f*(float)((p->expired-delay)/duration)) );
  Area( surface, tint, 0,0,w,h );
 }
};

class fx_FadeOut : public Proce55or {
public:
 Crayon tint;
 Zpointer<Presentation> p;
 void Setup() {
  p=(Presentation *) parent.pointer;
 }

 void Draw(GLWindow *surface) {
  tint.Float( tint.rf, tint.gf, tint.bf, (255.0f*(float)((p->expired-delay)/duration)) );
  Area( surface, tint, 0,0,w,h );
 }
};

class fx_MovingLine : public Proce55or {
public:
 Zint xdx,xdy,x2dx2,y2dy2,a,b,c,d;
 Zfloat perc;
 Crayon tint;
 Zpointer<Presentation> p;

 fx_MovingLine() : Proce55or() {
  tint.Pick(alabaster);
 }

 void Setup() {
  p=(Presentation *) parent.pointer;
 }

 void Between()
 {
  perc=(p->expired-delay)/duration;
  a=ilerp(x,xdx,perc);
  b=ilerp(x,xdy,perc);
  c=ilerp(x2,x2dx2,perc);
  d=ilerp(y2,y2dy2,perc);
 }

 void Draw(GLWindow *surface)
 {
  DrawLine( surface, tint, a,b,c,d );
 }
};

class fx_Box : public Proce55or {
public:
 Zint thickness;
 Zfloat perc,pulsespeed,pulsing;
 Crayon tint_outside, tint_inside, tint_start, tint_end, interior;
 Zbool filled;
 Cartesians points;
 Crayon t,u;
 Zpointer<Presentation> p;

 fx_Box() {
  Constructor();
  thickness=5;
  pulsespeed=0.25f;
  tint_outside.Pick(alabaster);
  tint_inside.Pick(blue);
  tint_start.Pick(black);
  tint_end.Pick(alabaster);
  interior.Pick(black);
  t.Pick(black);
  u.Pick(black);
 }
 
 void Setup() {
  int i;
  p=(Presentation *) parent.pointer;
  points.Clear();
  for ( i=0; i<thickness; i++ ) { 
   Cartesian *p=new Cartesian;
   p->x=x+i;
   p->y=y+i;
   p->x2=x2-i;
   p->y2=y2-i;
   points.Append(p);
  }
 }

 void Between()
 {
  pulsing+=FPS;
  if ( pulsing > pulsespeed ) pulsing=0.0f;
  perc=pulsing/pulsespeed;
  t.ColorInterp( tint_start, tint_end, perc );
 }
 void Render() {
  Draw(parent);
 }
 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface)
 {
  int i=0;
  for( Cartesian *p=(Cartesian *) (points.first); p; p=(Cartesian *) (p->next) ) {
   u.ColorInterp( tint_outside, tint_inside, iratiof((i+1),thickness) );
   t.ColorInterp( t, u, 0.5f );
   Area(surface,t,p->x,p->y,p->x2,p->y2);
   i++;
  }
  Area(surface, interior, x+thickness, y+thickness, x2-thickness, y2-thickness);
 }
};

class fx_MatrixText : public Proce55or {/*
 Crayon textColor;
 bool settling;
 float settlingRate;
 float old;
 GLImage *rendered; 
 char *message;
 char *actual;
 int length;
 float alpha;
 char *fn;
 float size;
 Blends blend;

 void Text( char *s ) {
  delete message; message=CopyString(s); length=strlen(message); 
 }

 void TextFont( char *tfn, int pixels ) { 
  fn=tfn; 
  size=iratiof(pixels,(int) font_resolution);
 }

 void Setup() {
  blend=additive;
  alpha=1.0f;
  settling=true;
  settlingRate=1/30.0f * 15;
 }

 void Between() {
  if ( settling && (old-expired) > settlingRate ) {
   int i;
   delete actual; actual = new char[length+1];
   for ( i=0; i<length; i++ ) {
    actual[i] = ( message[i] == ' ' ? ' ' : (char) (message[i] + (number_range(0,(i%10)/2))) );
   }
   actual[length]='\0';
   OUTPUT( "Actual: %s", actual );
   delete rendered;
   rendered = calligraph( actual, fn, (blend == mask ? false : true), (blend == mask ? true : false) );
   rendered.MakeDD(desktop.displaySystem);
   old=expired;
  }
 }

 void Draw(Surface surface)
 {
  surface.blitTint = textColor; 
  if (rendered) Blend( surface, rendered, blend, x,y,0,0,rendered.width,rendered.height,rendered.width,rendered.height );   
 }

 void Clean()
 {
   delete message,actual;
 }*/
};

class fx_WriteText : public Proce55or {
public:/*
 char *label;
 int len;
 ColorAlpha color;
 void Setup() { len=strlen(label); }
 void Clean() { delete label; }
 void Draw(Surface surface)
 {
  surface.foreground=color;
  surface.WriteText(x,y,label,len);
 }*/
};

class fx_tText : public Proce55or {
public:/*
 char *message;
 int length;
 float alpha;
 char *fn;
 ColorRGBA textColor;
 Bitmap rendered; 
 float size;
 Blends blend;

 void Text( char *s ) {
  delete message; message=CopyString(s); length=strlen(message); 
 }

 void TextFont( char *tfn, int pixels ) { 
  fn=tfn; 
  size=iratiof(pixels,(int) font_resolution);
 }

 void Setup() {
  blend=additive;
  alpha=1.0f;
  delete rendered;
  rendered = calligraph( message, fn, (blend == mask ? false : true), (blend == mask ? true : false) );
  rendered.MakeDD(desktop.displaySystem);
 }

 void Between() { }

 void Draw(Surface surface)
 {
  surface.blitTint = textColor; 
  if ( rendered) Blend( surface, rendered, blend, x,y,0,0,rendered.width,rendered.height,rendered.width,rendered.height );   
 }

 void Clean() { delete message; }*/
};

class fx_Circle2d : public Proce55or {
public:
 mPolygon p;
 Zint segments;
 Zfloat radius;
 Crayon tint;
 Blends blend;
 void Setup() {
  float delta=(float) (TWO_PI / (double) segments);
  float X=(float) x;
  float Y=(float) y;
  for ( float f=0.0f; f<TWO_PIf; f+=delta ) {
   Vertex *v=new Vertex(X+sinf(f)*radius,Y+cosf(f)*radius,0.0f);
   v->Color(tint);
   p.addVertex(v);
  }  
 }
 void Draw(GLWindow *surface) {
  Vertex *v,*o=null;
  for ( v=(Vertex *) (p.points.first); v; v=(Vertex *) v->next ) {
   if ( o!=null ) DrawLine(surface,tint,
      (int) o->x,
      (int) o->y,
      (int) v->x,
      (int) v->y);
   o=v;
  }
  v=(Vertex *) (p.points.first);
  DrawLine(surface,tint,
      (int) o->x,
      (int) o->y,
      (int) v->x,
      (int) v->y);
 }
};