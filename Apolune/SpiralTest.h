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

#include "GLWindow.h"
#include "Spiral.h"
#include "UglyFont.h"

class SpiralTest : public GLWindow {
public:
 Vertices *points;
 double r,t,z;
 int slices;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
  spirals.scale.Set(512.0,512.0,512.0);
  points=spirals.vertices();
  r=spirals.rScale;
  t=spirals.tScale;
  z=spirals.zScale;
  slices=spirals.slices;
 }
 void Reset() {
  delete points;
  points=spirals.vertices();
 }
 void Render() {
  glColor3d(1.0,1.0,1.0);
  glBegin(GL_LINE_LOOP);
  EACH(points->first,Vertex,v) glVertex2f( display->wf/2.0f+v->x, display->hf/2.0f+v->y );
  glEnd();
  Text(spirals.ModeName(),10.0,20.0,10.0,10.0,false,0.0);
  char buf[128];
  FORMAT(buf,128,"t=%1.2f, r=%1.2f, z=%1.2f, slices=%d", 
   (float) spirals.tScale, (float) spirals.rScale, (float) spirals.zScale, (int) spirals.slices );
  Text(buf,10.0,40.0,10.0,10.0,false,0.0);
 }
 void LeftUp() {
  spirals.scale.Set((double) input->mxi, (double) input->myi, 0.0);
  Reset();
 }
 void RightUp() {
  spirals.Mode(spiral_loxodrome);
  Reset();
 }
 void WheelUp() {
  if ( input->KeyDown(DX_LALT) ) {
   t+=1.0;
   spirals.SetT(t);
   Reset();
   return;
  }
  if ( input->KeyDown(DX_LSHIFT) ) {
   r+=1.0;
   spirals.SetR(r);
   Reset();
   return;
  }
  if ( input->KeyDown(DX_LCONTROL) ) {
   slices+=1;
   spirals.slices=slices;
   Reset();
   return;
  }
  spirals.NextMode();
  Reset();
 }
 void WheelDown() {
  if ( input->KeyDown(DX_LALT) ) {
   t-=1.0;
   spirals.SetT(t);
   Reset();
   return;
  }
  if ( input->KeyDown(DX_LSHIFT) ) {
   r-=1.0;
   spirals.SetR(r);
   Reset();
   return;
  }
  if ( input->KeyDown(DX_LCONTROL) ) {
   slices-=1;
   spirals.slices=slices;
   Reset();
   return;
  }
  spirals.Set(spirals.mode,100,TWO_PI,TWO_PI,1.0);
  Reset();
 }
};

