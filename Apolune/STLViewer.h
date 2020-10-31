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
#include "STLFile.h"

#include "VBOGroup.h"

class STLViewer : public GLWindow {
public:
 Zp<STLFile> stl;
 VBONormalsPolysGroup vbos;
 LookAtPerspective lap;
 Cartesiand position;
 Cartesiand rotation;
 Cartesiand offset;
 Zfloat keyDelay;
 STLViewer( STLFile *toView ) : GLWindow() {
  stl=toView;
 }
 void OnLoad() {
  stl->toVBOGroup(&vbos);
  background.Pick(indigo);
  Fullscreen();
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  offset.x=-stl->facets.extents.x/2.0;
  offset.z=-stl->facets.extents.z/2.0;
 }
 void Between() {
  if ( input->KeyDown(LAlt) ) {
   rotation.z=360.0f*(input->mxi-display->w2)/display->wf;
  } else {
   rotation.x=360.0f*(input->myi-display->h2)/display->hf;
   rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  }
  if ( keyDelay > 0.0f ) keyDelay-=(float)FRAMETIME;
  else {
   if ( input->KeyDown(Left) ) position.x-=1.0;
   if ( input->KeyDown(Right) ) position.x+=1.0;
   if ( input->KeyDown(Up) ) position.y-=1.0;
   if ( input->KeyDown(Down) ) position.y+=1.0;
   if ( input->KeyDown(Spacebar) || input->KeyDown(NumpadDecimal) ) { position.Set(0,0); }
   if ( input->KeyDown(NumpadPlus) ) { lap.eye.Set(0.0,0.0,lap.eye.z/2); keyDelay=0.25f; }
   if ( input->KeyDown(NumpadMinus) ) { lap.eye.Set(0.0,0.0,lap.eye.z*2); keyDelay=0.25f; }
  }
 }
 void Render() {
  Blending(none);
  glPush();
  lap.Apply();
  glTranslatef((GLfloat) position.x, 0.0, (GLfloat) position.y);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();///
//   glEnable(GL_DEPTH_TEST);
//   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0);
//   glDepthFunc(GL_GEQUAL);
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
//   glTranslatef((GLfloat) offset.x, (GLfloat) offset.y, (GLfloat) offset.z);
   vbos.RenderPolys();
//   glDisable(GL_DEPTH_TEST);
  glPopMatrix();
  glPushMatrix();///
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glLineWidth(4.2f);
   Coordinator();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) (stl->facets.extents.x*0.7), 0.0f,1.0f);
    Text("RIGHT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) -(stl->facets.extents.x*0.7), 0.0f,1.0f);
    Text("LEFT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) -(stl->facets.extents.y*0.7), 1.0f);
    Text("BACK",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) (stl->facets.extents.y*0.7), 1.0);
    Text("FRONT",0,0,2,3,true,0);
   glPopMatrix();
   glLineWidth(1.2f);
   glPushMatrix();//
    glTranslatef((GLfloat) -(stl->facets.extents.x/2)-10.0f, (GLfloat) -(stl->facets.extents.y*0.5), 0.0f);
    GLfloat cm=0.0f;
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("%-2d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > stl->facets.extents.y ) break;
    }
   glPopMatrix();//
   glPushMatrix();//
    glTranslatef( (GLfloat) -(stl->facets.extents.x*0.5), (GLfloat) -(stl->facets.extents.y*0.5), 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("____ %-2d cm",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > stl->facets.extents.x ) break;
    }
   glPopMatrix();//
  glPopMatrix();///
  glPop();
  Text(
   FORMAT("STL File: %s\nFacets: %d\nExtents (WxHxD): %s",
    stl->filename.c_str(), (int) stl->facets.count, stl->facets.extents.toString().c_str()
   ),crayons.Pick(green255),transparency,32,32,10,14,6,false);
 }
};

