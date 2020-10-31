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

#include "LinkedList.h"

#define BOXTEST2D(x,y,x1,y1,x2,y2)  ( x > x1 && y > y1 && x < x2 && y < y2 ) 

class Box2df : public ListItem {
public:
 float x,y,w,h,x2,y2;
 Box2df() : ListItem() {
  x=y=w=h=x2=y2=0.0f;
 }
 Box2df( float X, float Y, float W, float H ) : ListItem() {
  x=X;
  y=Y;
  w=W;
  h=H;
  x2=x+w;
  y2=y+h;
 }
};

class BoxCollision2df : public LinkedList {
public:
 bool Collides( float x1, float y1, float x2, float y2 ) {
  return ( Collides( x1, y1 ) || Collides( x2, y2 ) );
 }
 bool Collides( float x, float y ) {
  EACH(first,Box2df,b) if ( BOXTEST2D( x, y, b->x, b->y, b->x2, b->y2 ) ) return true;
  return false;
 }
 bool OffsetCollides( float ox, float oy, float x, float y ) {
  EACH(first,Box2df,b) if ( BOXTEST2D( x, y, ox+b->x, oy+b->y, ox+b->x2, oy+b->y2 ) ) return true;
  return false;  
 }
 bool OffsetCollides( float ox, float oy, float x1, float y1, float x2, float y2 ) {
  return ( OffsetCollides( ox, oy, x1, y1 ) || OffsetCollides( ox, oy, x2, y2 ) );
 }
 CLEARLISTRESET(Box2df);
 ~BoxCollision2df(void) {
  Clear();
 }
};


class Box2di : public ListItem {
public:
 int x,y,w,h,x2,y2;
 Box2di() : ListItem() {
  x=y=w=h=x2=y2=0;
 }
 Box2di( int X, int Y, int W, int H ) : ListItem() {
  x=X;
  y=Y;
  w=W;
  h=H;
  x2=x+w;
  y2=y+h;
 }
};

class BoxCollision2di : public LinkedList {
public:
 bool Collides( float x1, float y1, float x2, float y2 ) {
  return ( Collides( x1, y1 ) || Collides( x2, y2 ) );
 }
 bool Collides( float x, float y ) {
  EACH(first,Box2df,b) if ( BOXTEST2D( x, y, b->x, b->y, b->x2, b->y2 ) ) return true;
  return false;
 }
 bool OffsetCollides( float ox, float oy, float x, float y ) {
  EACH(first,Box2df,b) if ( BOXTEST2D( x, y, ox+b->x, oy+b->y, ox+b->x2, oy+b->y2 ) ) return true;
  return false;  
 }
 bool OffsetCollides( float ox, float oy, float x1, float y1, float x2, float y2 ) {
  return ( OffsetCollides( ox, oy, x1, y1 ) || OffsetCollides( ox, oy, x2, y2 ) );
 }
 CLEARLISTRESET(Box2di);
 ~BoxCollision2di(void) {
  Clear();
 }
};
