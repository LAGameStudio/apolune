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
#include "VBO.h"
#include "PLY.h"
#include "LookAtPerspective.h"

extern PLYs lowPLYprimitives;

class ModernVBOTest : public GLWindow {
public:
 ImmediateLighting lights;
 LookAtPerspective lap;
 VBOStaticVNT vbo;
 GLImage *tex;
 Cartesiand position;
 Cartesiand rotation;
 PLY *test;
 void OnLoad() {
  background.Pick(indigo);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  test=lowPLYprimitives.find("Bubby");
  vbo.fromPLY(test);
 }
 void Render() {
  glPush();
  glDisable(GL_CULL_FACE);  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef((GLfloat) position.x,(GLfloat) position.y,(GLfloat) position.z);
  glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
  glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
  glColor3d(1.0,0.0,0.0);
//  lights.On();
  vbo.Render();
  Blending(transparency);
  glColor4d(0.0,1.0,0.0,0.25);
  test->model.Render();
//  lights.Off();
  glPopMatrix();
  glPop();
//  glEnable(GL_CULL_FACE);
 }
 void Between() {
  rotation.x+=0.5;
  rotation.y+=1.0;
 }
};

