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
#include "WireframeShader.h"
#include "PLY.h"
#include "LookAtPerspective.h"
#include "VBO.h"

extern PLYs lowPLYprimitives;

class WireframeTest : public GLWindow {
public:
 VBOStaticVw vboVw;
 LookAtPerspective lap;
 PLY *p;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  p=lowPLYprimitives.Random();
  vboVw.fromPLY(p,true);
 }
 void Between() {
  if ( input->KeyDown(DX_SPACE) ) {
   p=lowPLYprimitives.Random();
   vboVw.Clear();
   vboVw.fromPLY(p,true);
  }
 }
 void Render() {
  glPush();
  glEnable(GL_DEPTH_TEST);
  lap.Apply();
  glDisable(GL_TEXTURE_2D);
  glColor3d(1.0,0.0,1.0);
  Blending(none);
//  wireframeShader.UpdateUniforms();
  vboVw.Render();
//  wireframeShader.Disable();
  glDisable(GL_DEPTH_TEST);
  glPop();
 }
};

