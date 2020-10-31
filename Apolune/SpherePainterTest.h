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
#include "SpherePainter.h"

class SpherePainterTest : public GLWindow {
public:
 SphereStrokes strokes;
 SpherePainter sphere;
 void OnLoad(){
  Anchor(0.0f, 0.0f, 0.0f, 0.0f);
  sphere.TestPattern();
  sphere.coordinator=true;
//  sphere.position.Set(-1.0,-1.0,-1.0);
  //sphere.TestStrokes();
 }
 void Render(){
  //sphere.Paint();
  fboScreen.background.Pick(clear);
  fboScreen.blend=transparency;
  fboScreen.Bind();
  sphere.Render();
  fboScreen.Unbind();
  fboScreen.Render(x,y,w,h);
  char buf[512];
  glColor3d(0.0,1.0,0.0);
  FORMAT(buf,512,"%1.2f, %1.2f, %1.2f",
   (float)sphere.position.x,(float)sphere.position.y,(float)sphere.position.z);
  Text(buf,10.0,20.0,10.0,14.0,false);
  FORMAT(buf,512,"%1.2f, %1.2f, %1.2f",
   (float)sphere.rotation.x,(float)sphere.rotation.y,(float)sphere.rotation.z);
  Text(buf,10.0,40.0,10.0,14.0,false);
  FORMAT(buf,512,"%1.2f, %1.2f, %1.2f",
   (float)sphere.scale.x,(float)sphere.scale.y,(float)sphere.scale.z);
  Text(buf,10.0,60.0,10.0,14.0,false);
 }
 void Between() {
  if ( input->KeyDown(DX_LEFT) ) {
   if ( input->wheelDown ) {
    sphere.position.x+=0.1;
   } else if ( input->wheelUp ) {
    sphere.position.x-=0.1;
   }
  }
  if ( input->KeyDown(DX_DOWN) ) {
   if ( input->wheelDown ) {
    sphere.position.y+=0.1;
   } else if ( input->wheelUp ) {
    sphere.position.y-=0.1;
   }
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   if ( input->wheelDown ) {
    sphere.position.z+=0.1;
   } else if ( input->wheelUp ) {
    sphere.position.z-=0.1;
   }
  }
 }
 void MouseMove(){
  sphere.rotation.Set(
   (double) iratiof(input->mxi-display->w/2, display->w/2)*180.0,
   (double) iratiof(input->myi-display->h/2, display->h/2)*180.0,
   0.0 );
 }
};