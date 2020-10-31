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
#include "Vertex.h"
/*
 * To initialize the camera:
 * SimpleObjectiveCamera::Viewport(x,y,w,h) <- set to GLWindow coordinates
 * SimpleObjectiveCamera::Front,Back,etc
 * SimpleObjectiveCamera::Aim()
 * Then in Between():
 * SimpleObjectiveCamera::Mouse(mx,my)
 * SimpleObjectiveCamera::Aim()
 */
class SimpleObjectiveCamera
{
public:
 //  Camera position, look at position, up vector,
 //  object position, rotation, scale.
 Vertexd eye,center,up,rotation,scale,translate;
 Cartesian renderCenter;
 Vertexd frustumOffset; 
 double aspect;

 SimpleObjectiveCamera() {
  up.Set(0.0,1.0,0.0);
  scale.Set(1.0,1.0,1.0);
 }

// Set the center of the GLWindow we are drawing to
// ie: camera.Viewport( win->x, win->y, win->w, win->h );
 void Viewport( int x, int y, int w, int h ) {
  renderCenter.Set(x+w/2,y+h/2); // represents the window center
  frustumOffset.x=iratiod(renderCenter.x,display.w);
  frustumOffset.y=iratiod(renderCenter.y,display.h);
  aspect=iratiod(w,h);
 }

 void Aim() {
  //  Set Perspective
  glMatrixMode(GL_PROJECTION);
  glOrtho(-1, 1, -1, 1, -2, 2); // Normalized tight matrix
  glFrustum(
   -frustumOffset.x,1.0-frustumOffset.x, // left right
   -frustumOffset.y,1.0-frustumOffset.y, // bottom top
   0.1,10000);

  //  Set camera position and view vector
  glMatrixMode(GL_MODELVIEW);

  gluLookAt(
      eye.x, eye.y, eye.z,
      center.x, center.y, center.z,
      up.x, up.y, up.z
  );

  //  apply translation
  glTranslatef((GLfloat) translate.x,(GLfloat) translate.y,(GLfloat) translate.z);

  // apply rotation
  glRotatef((GLfloat)rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef((GLfloat)rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef((GLfloat)rotation.z, 0.0f, 0.0f, 1.0f);

  //  Apply scaling
  glScalef((GLfloat)scale.x, (GLfloat)scale.y, (GLfloat)scale.z);
 }

 /* Aiming Functions Which Turn and Position the Camera Around An Object  */
 //-------------------------------------------------------------------------
 // Front
 //-------------------------------------------------------------------------
 void Front (void) {
  eye.Set(0,0,1);
  up.Set(0,1,0);
 }
 
 //-------------------------------------------------------------------------
 // Top
 //-------------------------------------------------------------------------
 void Top (void) {
  eye.Set(0,1,0);
  up.Set(0,0,-1);
 }
 
 //-------------------------------------------------------------------------
 //  Left
 //-------------------------------------------------------------------------
 void Left (void) {
  eye.Set(-1,0,0);
  up.Set(0,1,0);
 }
 
 //-------------------------------------------------------------------------
 // Back
 //-------------------------------------------------------------------------
 void Back (void) {
  eye.Set(0,0,-1);
  up.Set(0,1,0);
 }
 
 //-------------------------------------------------------------------------
 // Bottom
 //-------------------------------------------------------------------------
 void Bottom (void) {
  eye.Set(0,-1,0);
  up.Set(0,0,1);
 }
 
 //-------------------------------------------------------------------------
 // Right
 //-------------------------------------------------------------------------
 void Right (void) {
  eye.Set(0,-1,0);
  up.Set(0,1,0);
 }
};

