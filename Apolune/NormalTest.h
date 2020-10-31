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
#include "Vertex.h"
#include "GLWindow.h"
#include "LookAtPerspective.h"
#include "Polygon.h"
#include "UglyFont.h"

class NormalTest : public GLWindow  {
public:
 LookAtPerspective lap;
 mPolygon tri;
 Vertex A,B,C,N;
 Vertex a,b;
 Vertex rotation;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  A.x=0.0;
  A.y=1.0;
  tri.points.Add(0.0,1.0,0.0);
  B.x=1.0;
  B.y=1.0;
  tri.points.Add(1.0,1.0,0.0);
  tri.points.Add(0.0,0.0,0.0);
  tri.TriNormal(); tri.normal->normalize();
  tri.Barycenter();
  N.Set(tri.normal.pointer);
  N.Add(tri.bary.pointer);
  lap.Near=0.1;
  lap.Far=10.0;
 }
 void Between() {
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
 }
 void Render() {
  Blending(none);
  glLineWidth(1);
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
  glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEnable(GL_POLYGON_SMOOTH);
  glBegin(GL_TRIANGLES);
   glColor3d(1.0,0.0,0.0);
   glVertex3f(A.x,A.y,A.z);
   glNormal3f(tri.normal->x,tri.normal->y,tri.normal->z);
   glColor3d(0.0,1.0,0.0);
   glVertex3f(B.x,B.y,B.z);
   glNormal3f(tri.normal->x,tri.normal->y,tri.normal->z);
   glColor3d(0.0,0.0,1.0);
   glVertex3f(C.x,C.y,C.z); glNormal3f(tri.normal->x,tri.normal->y,tri.normal->z);
  glEnd();
  glPushMatrix();
  glTranslatef(A.x*1.25f,A.y*1.25f,A.z*1.25f);
  glColor3d(0.0,1.0,0.0);
  Text(FORMAT("A (%1.2f,%1.2f,%1.2f)",A.x,A.y,A.z).c_str(),0,0,0.025,0.025,true,0);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(B.x*1.25f,B.y*1.25f,B.z*1.25f);
  glColor3d(0.0,1.0,0.0);
  Text(FORMAT("B (%1.2f,%1.2f,%1.2f)",B.x,B.y,B.z).c_str(),0,0,0.025,0.025,true,0);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(C.x*1.25f,C.y*1.25f,C.z*1.25f);
  glColor3d(0.0,1.0,0.0);
  Text(FORMAT("C (%1.2f,%1.2f,%1.2f)",C.x,C.y,C.z).c_str(),0,0,0.025,0.025,true,0);
  glPushMatrix();
  glTranslatef(N.x,N.y,N.z);
  glColor3d(0.0,1.0,0.0);
  Text(FORMAT("N (%1.2f,%1.2f,%1.2f)",N.x,N.y,N.z).c_str(),0,0,0.025,0.025,true,0);
  glPopMatrix();
  glPopMatrix();
  glColor3d(1.0,1.0,1.0);
  glBegin(GL_LINES);
   glColor3d(1.0,0.0,1.0);
   glVertex3f(tri.bary->x,tri.bary->y,tri.bary->z);
   glColor3d(0.0,1.0,1.0);
   glVertex3f(tri.normal->x+tri.bary->x,tri.normal->y+tri.bary->y,tri.normal->z+tri.bary->z);
  glEnd();
  glPopMatrix();
  Coordinator();
  glPop();
  glColor3d(1.0,1.0,1.0);
  Text(FORMAT("Rotation_x = %1.4f   Rotation_y = %1.4f",rotation.x,rotation.y).c_str(),16,16,12,16,false,0);
 }
};

