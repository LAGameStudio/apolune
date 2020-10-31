#pragma once

#include "RectPlacer.h"
#include "EditGUI.h"

class RectPackTest : public GLWindow {
public:
 Cartesian maximumSize;
 Zint w,h;
 Zint counter;
// RectPlacer placer;
// Cartesians rects;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  maximumSize.Set(2048,2048);
//  placer.Reset(maximumSize.x,maximumSize.y);
  rectPack.growth.Set(8,0);
 }
 void Render() {
  Blending(additive);
  Crayon tint;
  Blending(none);
  w=fast.hslider("width",w,1.0,1024.0,8,8,1024,16,true);
  h=fast.hslider("height",h,1.0,1024.0,8,8+32,1024,16,true);
  if ( fast.button("Place",8,8+64,64,16) ) {
   AddRect(w,h);
  }
  if ( fast.button("Grow Horiz",8+64+8,8+64,96,16) ) {
   rectPack.growth.Set(8,0);
   rectPack.Grow();
  }
  if ( fast.button("Grow Vert",8+64+8+96+8,8+64,96,16) ) {
   rectPack.growth.Set(0,8);
   rectPack.Grow();
  }
  if ( fast.button("Grow",8+64+8+96+8+96+8,8+64,64,16) ) {
   rectPack.growth.Set(8,8);
   rectPack.Grow();
  }
  if ( fast.button("Clear",8,8+64+24,64,16) ) {
   rectPack.inputs.Clear();
//   rects.Clear();
 //  placer.Reset(maximumSize.x,maximumSize.y);
  }
  Blending(additive);
  glPushMatrix();
  glTranslated(128.0,128.0,0.0);
  int i=0;
  EACH(rectPack.outputs.first,PackedRectList,pl) {
   EACH(pl->rects.first,PackedRect,pr) {
    Cartesian *r=&pr->r;
    Crayon tint;
    tint.x_Navy();
    Area(tint,r);
    tint.x_Brown();
    Rectangle(tint,r);
    Text(FORMAT("R%d %d",i,pr->id).c_str(),r->x+r->w/2,r->y+r->h/2,8.0,11.0,true);
   }
   Text(FORMAT("%d",i).c_str(),8.0,8.0,8.0,11.0,true);
   glTranslated((double)rectPack.packSize.x,0.0,0.0);
   i++;
  }
  glPopMatrix();
  glPushMatrix();
  glTranslated(128.0,128.0,0.0);
   Rectangle(crayons.Pick(green255),0,0,rectPack.grownSize.x,rectPack.grownSize.y);
   Rectangle(crayons.Pick(red255),0,0,rectPack.packSize.x,rectPack.packSize.y);
  glPopMatrix();

//  Rectangle(crayons.Pick(magenta),&placer.bounds);
  Blending(none);
 }
 void AddRect( int w, int h ) {
//  placer.FindCoords(w,h);
//  if ( placer.result ) {
//   Cartesian *c=new Cartesian;
//   c->SetRect(placer.result->y,placer.result->x,w,h);
//   rects.Append(c);
//  } else OUTPUT("Could not place rect!\n");
  rectPack.inputs.Add(0,0,w,h);
  rectPack.Pack();
 }
};

