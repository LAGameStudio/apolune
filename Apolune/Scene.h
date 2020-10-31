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

#include "macros.h"
#include "Lights.h"
#include "GLWindow.h"
#include "Material.h"
#include "Camera.h"
#include "Film.h"
#include "pBuffer.h"
#include "dhpowareMath.h"

#include "Sphere.h"
#include "Matter.h"

class WorldShader;

enum SceneControlModes {
 fixed, objectInspect, freeLook
};

class Scene : public GLWindow {
public:
 GLfloat ambient[4];
 Lights lights;
 GLuint fogMode;         // 0=off, GL_EXP, GL_EXP2, GL_LINEAR
 gCamera *camera;
 Films films;
 GLfloat fogColor[4];    // Fog color
 GLfloat fogDensity;
 GLfloat fogStart;
 GLfloat fogEnd;
 GLenum fogHint;        // GL_DONT_CARE, GL_NICEST, GL_FASTEST
 Matrix4 projectionMatrix;
 float rotationDegrees;
 float rotationSpeed;
 Vector3 rotationAxis;
 Vector3 up, target;
 Vertex zero;
 bool showAxis, showCoords, drawLights;
 Matters sphere;
 Matter *sphereVBO;  
 WorldShader *color;
 Quaternionf q;

 PROGRAMMABLE(Scene,RenderScenePtr,renderscene,RenderScene);
 PROGRAMMABLE(Scene,PostScenePtr,postscene,PostScene);
 PROGRAMMABLE(Scene,InitScenePtr,initscene,InitScene);
/*
 PROGRAMMABLE(Scene,LeftDownPtr,leftdown,PLeftDown);
 PROGRAMMABLE(Scene,RightDownPtr,rightdown,PRightDown);
 PROGRAMMABLE(Scene,MiddleDownPtr,mousedown,PMiddleDown);

 PROGRAMMABLE(Scene,LeftUpPtr,leftup,PLeftUp);
 PROGRAMMABLE(Scene,RightUpPtr,rightup,PRightUp);
 PROGRAMMABLE(Scene,MiddleUpPtr,middleup,PMiddleUp);

 SceneControlModes currentMode;
 void SceneControlMode( SceneControlModes modes ) {
 }
 */
 Scene(void) { SceneConstructor(); this->InitGLWindow(); }
 Scene( bool init ) { SceneConstructor(); if ( init ) this->InitGLWindow(); }
 void SceneConstructor() {
  initscene=&Scene::DefaultInit;
  renderscene=&Scene::DefaultRender;
  postscene=&Scene::CleanPost;
  showAxis=false;
  showCoords=true;
  drawLights=true;
  ambient[0]=ambient[1]=ambient[2]=ambient[3]=0.0f;
  fogMode=0;
  camera=null;
  fogColor[0]=fogColor[1]=fogColor[2]=fogColor[3]=0.0f;
  fogDensity=0.0f; // 20.0f
  fogStart=0.0f;   // 1.0f
  fogEnd=0.0f;     // 20000.0f
  fogHint=GL_FASTEST;  // or GL_NICEST
  rotationSpeed=0.4f;
  up.set(0.0f,1.0f,0.0f);
  target.set(0.0f,0.0f,0.0f);
  rotationAxis.set(0.0f,0.0f,0.0f);
  rotationDegrees=0.0f;
  /*
  {
   Substance *s;
   PolygonList *pL=sphere.Add(&geosphere3d, s=substances.find("Solid Color"),&sphereVBO); delete pL;
   sphere.PrepareVBOs();
   color=(WorldShader *) s->m;
  }
  */
 }

 virtual void CleanPost() {}

 void DefaultInit() {
  camera->Setup(w,h,&(camera->origin));
 }

 // Construct a projection matrix based on the horizontal field of view
 // 'fovx' rather than the more traditional vertical field of view 'fovy'.
 void Perspective(float fovx, float aspect, float znear, float zfar) {

  float e = 1.0f / tanf(dhpowareMath::degreesToRadians(fovx) / 2.0f);
  float aspectInv = 1.0f / aspect;
  float fovy = 2.0f * atanf(aspectInv / e);
  float xScale = 1.0f / tanf(0.5f * fovy);
  float yScale = xScale / aspectInv;
 
  projectionMatrix[0][0] = xScale;
  projectionMatrix[0][1] = 0.0f;
  projectionMatrix[0][2] = 0.0f;
  projectionMatrix[0][3] = 0.0f;
 
  projectionMatrix[1][0] = 0.0f;
  projectionMatrix[1][1] = yScale;
  projectionMatrix[1][2] = 0.0f;
  projectionMatrix[1][3] = 0.0f;
 
  projectionMatrix[2][0] = 0.0f;
  projectionMatrix[2][1] = 0.0f;
  projectionMatrix[2][2] = (zfar + znear) / (znear - zfar);
  projectionMatrix[2][3] = -1.0f;
 
  projectionMatrix[3][0] = 0.0f;
  projectionMatrix[3][1] = 0.0f;
  projectionMatrix[3][2] = (2.0f * zfar * znear) / (znear - zfar);
  projectionMatrix[3][3] = 0.0f;
 }

 void Between() {
  materials.Update();
//  camera->Update();
 }

 void DefaultRender();

 virtual void BetweenFrames() {
  // Update AI, physics and time-based animations / tweens on MapObjects.
 }
 virtual void Init() {
  InitScene();
 }
 virtual void Render() {
  RenderScene();
  PostScene();
 }
 void Geometry();

 ~Scene(void) {
 }
};

enum CameraMode { cameraTrack, cameraOrbit, cameraDolly };

class MatterInspector : public Scene {
public:
 Matrix4 rotation;
 bool dragging, orbitting;
 Euler cameraDirection;
 int cx,cy,mx,my;
 CameraMode mode;
 Vertex v;
 Quaternion q;

 void OnLoad() {
  mode=cameraOrbit;
  dragging=false;
  orbitting=true;
  v.Set(0.0f,10.0f,20.5f);
  camera->AdjustPosition(&v);
 }

 void LeftDown() {
  dragging=true;
  cx=input->mxi-this->x;
  cy=input->myi-this->y;
 }

 void RightDown() {
  dragging=true;
  cx=input->mxi-this->x;
  cy=input->myi-this->y;
 }

 void LeftUp() {
  dragging=false;
 }

 void RightUp() {
  dragging=false;
 }

 void BetweenFrames() {
  if ( dragging ) {
   int dx,dy;
   mx=input->mxi-this->x;
   my=input->myi-this->y;
   dx=cx-mx;
   dy=cy-my;
   cx=mx;
   cy=my;
   OUTPUT("%d,%d\n", (int) dx, (int) dy);
   switch ( mode ) {
   case cameraTrack:     
    break;
   case cameraDolly:
    break;
   case cameraOrbit:   
      camera->eyeVector.x+=iratiof(dx,w2)*TWO_PIf;
      camera->eyeVector.y+=iratiof(dy,h2)*TWO_PIf;
    break;
   }
  }
  
  if ( input->KeyDown(DX_A) ) {
   target.x-=1.0f;
  }
  if ( input->KeyDown(DX_W) ) {
   target.y+=1.0f;
  }
  if ( input->KeyDown(DX_S) ) {
   target.y-=1.0f;
  }
  if ( input->KeyDown(DX_D) ) {
   target.x+=1.0f;
  }
  if ( input->KeyDown(DX_LEFT) ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.x-=1.0f;
   camera->AdjustPosition(&v);
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.x+=1.0f;
   camera->AdjustPosition(&v);
  }
  if ( input->KeyDown(DX_UP) ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.y+=1.0f;
   camera->AdjustPosition(&v);
  }
  if ( input->KeyDown(DX_DOWN) ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.y-=1.0f;
   camera->AdjustPosition(&v);
  }
  if ( input->wheelUp ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.z-=1.0f;
   camera->AdjustPosition(&v);
  } else
  if ( input->wheelDown ) {
   v.Set( camera->cPosition.x, camera->cPosition.y, camera->cPosition.z );
   v.z+=1.0f;
   camera->AdjustPosition(&v);
  }
  
 }

 void MiddleDown() {
 }

};


class Scenes : public LinkedList {
public:
 CLEARLISTRESET(GLWindow);
 ~Scenes() { Clear(); }
};