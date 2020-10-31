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
#include "CubeMappedSphere.h"
#include "Model.h"
#include "OldVBO.h"
#include "Material.h"
#include "Sphere.h"

class CubeMappedSphereTest: public GLWindow{
  CubeMappedSphere3d sphere;
  Crayon color;
  Substance s;
  float yAngle;
  float xAngle;

  void OnLoad(){
   yAngle=0;
   xAngle=0;
   color.Pick(indigo);
   Anchor(0.0f, 0.0f, 0.0f, 0.0f);
   sphere.r=0.5f;
   sphere.res=10;
   sphere.VertexBufferAndImmediate();
  }
  void Render(){
   int i=0;
   int j=0;
   glPushMatrix();
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glLoadIdentity();
   makeFrustum(120.0, (double)(w/h), -2.0, 2.0);
   Blending(transparency);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_CULL_FACE);
   glRotatef((GLfloat) xAngle, 0.0f, 1.0f, 0.0f);
   glRotatef((GLfloat) yAngle, 1.0f, 0.0f, 0.0f);
   EACH(sphere.model.polygons.first, mPolygon, p){
    i++;
    if (p==sphere.model.polygons.first) color.Pick(alabaster);
    color.Float(
     iratiof(i,sphere.model.polygons.count),
     iratiof(i,sphere.model.polygons.count),
     iratiof(i,sphere.model.polygons.count),
     1.0f);
    glColor4fv(color.floats);
    glBegin((GLenum) p->draw_method);
    EACH(p->points.first, Vertex, v){
     j++;
     glVertex3f(v->x, v->y, v->z);
   //OUTPUT ("x:%f y:%f z:%f\ni:%d j:%d\n", v->x, v->y, v->z, i, j);
    }
    glEnd();
   }
   glPopAttrib();
   glPopMatrix();
  }
  void makeFrustum(double fovY, double aspectRatio, double front, double back)
  {
   double tangent = tan(deg2rad((float)(fovY/2.0)));   // tangent of half fovY
   double height = front * tangent;          // half height of near plane
   double width = height * aspectRatio;      // half width of near plane
   // params: left, right, bottom, top, near, far
   glFrustum(-width, width, -height, height, front, back);
  }
  void MouseMove(){
   xAngle=iratiof(input->mxi-display->w/2, display->w/2)*180.0f;
   yAngle=iratiof(input->myi-display->h/2, display->h/2)*180.0f;
  }
};