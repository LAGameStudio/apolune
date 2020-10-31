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
#include "ListItem.h"
#include "Tween.h"
#include "Tweens.h"

extern Tweens tweens;

class Camera2d : public ListItem {
public:
 float w,h,w2,h2;
 float x,y,x2,y2;
 float sx,sy,cx,cy,ex,ey,dx,dy;
 float szoom,zoom,ezoom,dzoom;
 float duration,elapsed;
 Zpointer<Tween> tween;
 Camera2d(void) : ListItem() {
  x=y=w=h=w2=h2=sx=ex=dx=sy=ey=dy=duration=elapsed=0.0f;
  x2=y2=0.0f;
  cx=cy=0.0f;
  szoom=ezoom=zoom=dzoom=1.0f;
  tween=tweens.find("Cosine -1-0 Normalized to 1");
 }
 void SetTarget( float fx, float fy ) {
  sx=cx;
  sy=cy;
  ex=fx;
  ey=fy;
  dx=ex-sx;
  dy=ey-sy;
 }
 void SetTargetDuration( float fx, float fy, float s ) {
  ex=fx;
  ey=fy;
  duration=s;
 }
 void SetTargetDurationZoom( float fx, float fy, float s, float z ) {
  ex=fx;
  ey=fy;
  duration=s;
  szoom=zoom;
  ezoom=z;
  dzoom=ezoom-szoom;
 }
 void ZoomTo( float fz ) {
  zoom=fz;
 }
 void MoveTo( float fx, float fy ) {
  cx=fx;
  cy=fy;
  x=cx-w2;
  y=cy-h2;
  x2=cx+w2;
  y2=cx+h2;
 }
 void Between() {
  float perc=tween->tweenf(elapsed/duration);
  elapsed+=FPS;
  if ( elapsed > duration-FPS && elapsed < duration ) { // this may cause sometimes a visual artifact
   MoveTo(ex,ey);
   zoom=ezoom;
  } else
  if ( elapsed < duration ) {
   if ( zoom != ezoom ) {
    zoom=szoom+perc*dzoom;
   }
   if ( cx != ex ) {
    cx=sx+perc*dx;
   }
   if ( cy != ey ) {
    cy=sy+perc*dy;
   }
   MoveTo(cx,cy);
  }
 }
};