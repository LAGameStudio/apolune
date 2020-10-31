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
  Generic viewer for VBOs used as a
  Proce55or, utilizing the specialized
  vbo class that handles large numbers
  of polygons (VBONormalsPolysGroup)
 **************************************************************/

#include "ListItem.h"
#include "LookAtPerspective.h"
#include "VBOGroup.h"
#include "Proce55or.h"
#include "Vertexd.h"

class fx_VBOView : public Proce55or {
public:
 Cartesian screen;
 LookAtPerspective lap;
 Zpointer<VBONormalsPolysGroup> vbo;
 Zpointer<FBOColor> fbo;
 Zbool coordinator,ruler;
 Vertexd extents, position, rotation, zoom, scale;
 fx_VBOView() : Proce55or() {
  fbo=&fboC512;
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
 }
 void OnSetExtents() {}
 void Between() {
 }
 void Ruler() {
  glLineWidth(1.2f);
  glPushMatrix();//
   glTranslatef((GLfloat) -(extents.x/2)-10.0f, (GLfloat) -(extents.y*0.5), 0.0f);
   GLfloat cm=0.0f;
   for ( unsigned int t=0; t<30; t++ ) {
    cm=t*10.0f;
    glPushMatrix();
     glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
     glTranslatef(0.0f, -cm, 0.0f);
     Text(FORMAT("%-3d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
    glPopMatrix();
    if ( cm > extents.y ) break;
   }
  glPopMatrix();//
  glPushMatrix();//
   glTranslatef((GLfloat) -(extents.x/2), (GLfloat) -(extents.y*0.5), 0.0f);
   glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
   for ( unsigned int t=0; t<30; t++ ) {
    cm=t*10.0f;
    glPushMatrix();
     glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
     glTranslatef(0.0f, -cm, 0.0f);
     Text(FORMAT("____ %-3d cm",(int)t).c_str(),0,0,1,1.6,false,0);
    glPopMatrix();
    if ( cm > extents.x ) break;
   }
  glPopMatrix();//
  glLineWidth(1);
 }
 void Render() {
  if ( vbo ) {
   fbo->Bind();
   glPush();
   lap.Apply();
   glMatrixMode(GL_MODELVIEW);
   glTranslatef((GLfloat) position.x, 0.0, (GLfloat) position.y);
   glScalef( (GLfloat) scale.x, (GLfloat) scale.y, (GLfloat) scale.z );
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0);
   glDepthFunc(GL_LEQUAL);
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
   vbo->Render();
   if ( coordinator ) Coordinator();
   if ( ruler ) Ruler();
   glPop();
   fbo->Unbind();
   fbo->Render(x.value,y.value,x+w,y+h);
  }
 }
};

