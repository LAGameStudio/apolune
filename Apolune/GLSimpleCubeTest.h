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
#include "DisplayList.h"
#include "FBO.h"
#include "Art.h"

extern GLfloat light_diffuse[];
extern GLfloat light_position[];
extern GLfloat n[6][3];
extern GLint faces[6][4];
extern GLfloat v[8][3];

class GLSimpleCubeTest : public GLWindow
{
public:
 Cartesian rotation;
 FBOColorDepth f;

 void OnLoad() {
  background.Pick(indigo);
  SetPositionAndSize(15,15,500,500);
//  Anchor(0.5f,0.5f,0.25f,0.25f);// see the issue?
//  Anchor(0.0f,0.0f,0.0f,0.0f);
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
  rotation.Set(60.0f,-20.0f);
  f.blend=transparency;
  f.Create(512,512);
 }

 void Between() {
  rotation.fx+=FRAMETIME;
  rotation.fy+=FRAMETIME;
 }

 void drawBox();

 void Render() {
  f.Bind();
  glPush();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrustum(-.10,.10,-.10,.10,0.1,10000);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, -1, 0, 0, 1, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
 
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
//  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(
    /* field of view in degree */ 40.0,
    /* aspect ratio */ iratiof(w,h),
    /* Z near */ 1.0, 
    /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position, rotation. */
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(rotation.fx, 1.0, 0.0, 0.0);
  glRotatef(rotation.fy, 0.0, 0.0, 1.0);

  drawBox();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  glPop();
  f.Unbind();

  Crayon crayon;
  crayon.Pick(alabaster);
  DrawLine(this,crayon,w2,0,w2,h);
  DrawLine(this,crayon,0,h2,w,h2);
  f.Render(x,y,x2,y2);
 }

};

