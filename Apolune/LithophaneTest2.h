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

/**************************************************************
  Author: H. Elwood Gilliland III
  Lithophane test case #2
 **************************************************************/

#include "GLWindow.h"
#include "Lithophane.h"
#include "LookAtPerspective.h"

class LithophaneTest2 : public GLWindow {
public:
 Zdisposable<Lithophane> litho;
 ImmediateLighting lights;
 LookAtPerspective lap;
 Cartesiand position;
 Cartesiand rotation;
 Zbool wireframe;
 VBONormalsPolysGroup vbos;
 STLFile out;
 void OnLoad() {
  background.Pick(indigo);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  lights.SampleLighting();
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,-200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  litho.Recycle( new Lithophane("data/images/heightbrushes/jimmy.png",96,96,100,100,5,1) );
  litho->topography->Export(&vbos,&out);
//  position.x=-litho->size.x/2.0;
//  position.y=-litho->size.y/2.0;
 }
 void Between() {
  if ( input->KeyDown(W) ) wireframe=!wireframe;
  if ( input->KeyDown(NumpadPlus) ) lap.eye.Set(0.0,0.0,lap.eye.z/2);
  if ( input->KeyDown(NumpadMinus) ) lap.eye.Set(0.0,0.0,lap.eye.z*2);
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
 }
 void Render() {
  Blending(none);
  glPush();
  if ( wireframe ) {
   glDisable(GL_CULL_FACE);
   glLineWidth(2);
   glEnable(GL_LINE_SMOOTH);
  } else {
   glEnable(GL_CULL_FACE);
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glEnable(GL_POLYGON_SMOOTH);
   //glDisable(GL_CULL_FACE);  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef((GLfloat) position.x,(GLfloat) position.y,(GLfloat) position.z);
  glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
  glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
  glColor3d(1.0,1.0,0.0);
//  lights.On();
//  litho->Render();
  vbos.Render();
//  lights.Off();
//  Blending(transparency);
//  glColor4d(0.0,1.0,0.0,1.0);//0.25);
  glLineWidth(1);
  Coordinator();
  glPopMatrix();
  glPop();
  Blending(none);
 }

};

