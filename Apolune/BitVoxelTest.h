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
  BitVoxel simple voxel test class
 **************************************************************/

#include "GLWindow.h"
#include "BitVoxel.h"
#include "LookAtPerspective.h"
#include "Interface.h"
#include "STLFile.h"

class BitVoxelTest : public GLWindow {
public:
 BitVoxel bv;
 ImmediateLighting lights;
 LookAtPerspective lap;
 Cartesiand position;
 Cartesiand rotation;
 Zbool wireframe;
 Zdouble keyDelay;
 STLFile brushSource;
 ImmediateModel brush;
 Vertexd positionAdd,positionScale;
 void OnLoad() {
  background.Pick(indigo);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  lights.SampleLighting();
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,-200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  // Apply test pattern to Bit Voxel
  bv.SetExtents(50.0,50.0,50.0,10,10,10,10,10,10);
  bv.Reslice();
//  brushSource.ascii=true;
  brushSource.Decode("data/stl/7x3Bot7x2TopRectPiece.stl");
  brushSource.toModel(&brush);
  positionScale.Set(1,1,1);
 }
 void Between() {
  if ( keyDelay > 0 ) { keyDelay-=FRAMETIMEd; }
  if ( input->KeyDown(Equals) ) {
   if ( keyDelay <= 0.0 ) {
    bv.Add(&brush,&positionAdd,&positionScale);
    bv.Reslice();
    keyDelay=0.5;
   }
  }
  if ( input->KeyDown(W) ) wireframe=!wireframe;
  if ( input->KeyDown(NumpadPlus) ) lap.eye.Set(0.0,0.0,lap.eye.z/2);
  if ( input->KeyDown(NumpadMinus) ) lap.eye.Set(0.0,0.0,lap.eye.z*2);
  rotation.x=360.0f*(input->myi/display->hf);
  rotation.y=360.0f*(input->mxi/display->wf);
 }
 void Render() {
  Blending(none);
  glPush();
  if ( wireframe ) {
   glDisable(GL_CULL_FACE);
   glLineWidth(5);
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
  bv.Render();
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

