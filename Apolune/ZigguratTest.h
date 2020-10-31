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
#include "UglyFont.h"
#include "Randoms.h"

class ZigguratTest : public GLWindow {
public:
 void OnLoad() {
  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);
 }
 void Render() {
  Blending(none);

  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  for ( int i=0; i<128; i++ ) {
   glVertex2f(256.0f*iratiof(i,128),256.0f*randoms.zig.fn[i]);
//   glVertex2f(256.0f,256.0f*iratiof(i,128));
   glColor3f(iratiof(i,128),iratiof(i,128),iratiof(i,128));
  }
  glColor3d(0.0,1.0,0.0);
  glVertex2f(256.0f,256.0f);
  glEnd();

  Text("fn",256,256,6,7,false);

  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  for ( int i=0; i<128; i++ ) {
   glVertex2f(256.0f+256.0f*iratiof(i,128),256.0f*randoms.zig.wn[i]);
//   glVertex2f(256.0f,256.0f*iratiof(i,128));
   glColor3f(iratiof(i,128),iratiof(i,128),iratiof(i,128));
  }
  glColor3d(0.0,1.0,0.0);
  glVertex2f(256.0f+256.0f,256.0f);
  glEnd();

  Text("wn",256+256,256,6,7,false);

  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  for ( int i=0; i<256; i++ ) {
   glVertex2f(256.0f*iratiof(i,256),256.0f+256.0f*randoms.zig.fe[i]);
//   glVertex2f(256.0f,256.0f*iratiof(i,128));
   glColor3f(iratiof(i,128),iratiof(i,128),iratiof(i,128));
  }
  glColor3d(0.0,1.0,0.0);
  glVertex2f(256.0f,256.0f+256.0f);
  glEnd();

  Text("fe",256,256+256,6,7,false);

  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  for ( int i=0; i<256; i++ ) {
   glVertex2f(256.0f+256.0f*iratiof(i,256),256.0f+256.0f*randoms.zig.we[i]);
//   glVertex2f(256.0f,256.0f*iratiof(i,128));
   glColor3f(iratiof(i,128),iratiof(i,128),iratiof(i,128));
  }
  glColor3d(0.0,1.0,0.0);
  glVertex2f(256.0f+256.0f,256.0f+256.0f);
  glEnd();

  Text("we",256+256,256+256,6,7,false);  

  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  for ( int i=0; i<128; i++ ) {
   glVertex2f(256.0f*iratiof(i,128),256.0f*randoms.zig.fn[i]);
//   glVertex2f(256.0f,256.0f*iratiof(i,128));
   glColor3f(iratiof(i,128),iratiof(i,128),iratiof(i,128));
  }
  glColor3d(0.0,1.0,0.0);
  glVertex2f(256.0f,256.0f);
  glEnd();
 }
};

