/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "GLWindow.h"
#include "FBO.h"
#include "Shadows2DShader.h"
#include "TextureLibrary.h"
#include "Light2d.h"

class Shaded2dShadowTest : public GLWindow {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  fboScreen3.background.Pick(clear);
  fboScreen3.Bind();
  // draw obstructions
  glColor4f(1.0f,0.0f,0.0f,1.0f);
  glBegin(GL_QUADS);
  glVertex2f(123.0f,145.0f);
  glVertex2f(153.0f,145.0f);
  glVertex2f(153.0f,165.0f);
  glVertex2f(123.0f,165.0f);
  glEnd();
  glBegin(GL_QUADS);
  glVertex2f(423.0f,445.0f);
  glVertex2f(453.0f,445.0f);
  glVertex2f(453.0f,465.0f);
  glVertex2f(423.0f,465.0f);
  glEnd();
  fboScreen3.Unbind();
  shadows2dShader.position[0]=iratiof(display->w,2);
  shadows2dShader.position[1]=iratiof(display->h,2);
  shadows2dShader.color.Pick(green);
  shadows2dShader.obstructions=&fboScreen3.texture; //library.LoadGLuint("data/images/dust/dust1.png",tllto_Tiling); //
 // fboScreen3.TextureIDPtr(); //
 }
 void Render() {
  fboScreen2.background.Pick(blue);
  fboScreen2.Bind();
  fboScreen2.QuadYInverted(&shadows2dShader);
  fboScreen2.Unbind();
  fboScreen2.Renderf(none,0.0f,0.0f,display->wf,display->hf);
  //fboScreen3.Renderf(transparency,0.0f,0.0f,(float)display->w,(float)display->h);
  Rectangle(crayons.Pick(alabaster),7,7,129,97);
  fboScreen3.RenderUpsideDown(8,8,128,96);
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  shadows2dShader.position[0]=(float) iratiof(mx,display->w);
  shadows2dShader.position[1]=(float) iratiof(my,display->h);
 }
};