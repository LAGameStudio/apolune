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
#include "LookAtPerspective.h"

class FBOBoxCameraTest : public GLWindow {
public:
 ImmediateLighting lights;
 LookAtPerspective lap;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
//   glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0); // y is mapped to z
  lap.eye.Set(0,0,15);
  lap.Near=0.001;
  lap.Far=100.0;
 }
 void Render() {
  fbo512.Bind();
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0); // y is mapped to z
  // Unit 5.0
  
  for ( int j=0; j<10; j++ ) {
   double x=5.0*iratiod(j,10);
   glColor3d(x,x,x);
   glBegin(GL_LINES);
    glVertex2d(x,0.0);
    glVertex2d(x,5.0);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double y=5.0*iratiod(j,10);
   glColor3d(y,y,y);
   glBegin(GL_LINES);
    glVertex2d(0.0,y);
    glVertex2d(5.0,y);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double z=5.0*iratiod(j,10);
   glColor3d(z,z,z);
   glBegin(GL_LINES);
    glVertex3d(0.0,0.0,z);
    glVertex3d(5.0,0.0,z);
   glEnd();
  }
  // Unit 2.0
  for ( int j=0; j<10; j++ ) {
   double x=2.0*iratiod(j,10);
   glColor3d(x,x,0.0);
   glBegin(GL_LINES);
    glVertex2d(x,0.0);
    glVertex2d(x,2.0);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double y=2.0*iratiod(j,10);
   glColor3d(0.0,y,y);
   glBegin(GL_LINES);
    glVertex2d(0.0,y);
    glVertex2d(2.0,y);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double z=2.0*iratiod(j,10);
   glColor3d(z,0.0,z);
   glBegin(GL_LINES);
    glVertex3d(0.0,0.0,z);
    glVertex3d(2.0,0.0,z);
   glEnd();
  }
  // Unit 1.0
  for ( int j=0; j<10; j++ ) {
   double x=iratiod(j,10);
   glColor3d(x,0.0,0.0);
   glBegin(GL_LINES);
    glVertex2d(x,0.0);
    glVertex2d(x,1.0);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double y=iratiod(j,10);
   glColor3d(0.0,y,0.0);
   glBegin(GL_LINES);
    glVertex2d(0.0,y);
    glVertex2d(1.0,y);
   glEnd();
  }
  for ( int j=0; j<10; j++ ) {
   double z=iratiod(j,10);
   glColor3d(0.0,0.0,z);
   glBegin(GL_LINES);
    glVertex3d(0.0,0.0,z);
    glVertex3d(1.0,0.0,z);
   glEnd();
  }
  glPopMatrix();
  glPop();
  fbo512.Unbind();
  fbo512.Render(0,0,512,512);
 }
 Vertexd rotation;
 void Between() {
  rotation.Set(360.0*iratiod(input->mxi,display->w),360.0*iratiod(input->myi,display->h),360.0*iratiod(input->mxi,display->w));
 }
};