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
#include "TextureLibrary.h"
#include "Art2d.h"

#include "RippleShader.h"
#include "FrostedShader.h"
#include "SwirlShader.h"
#include "ShockwaveShader.h"

class NewShaderTest : public GLWindow {
public:
 GLImage * in;
 float time;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  in=library.Load("data/images/encyclopedia/saguaro.jpg");
  time=0.0f;
//  rippleShader.pTex0=&in->texture;
//  frostedShader.pTex1=&in->texture;
  shockwaveShader.pTex0=&in->texture;
 }
 void Between() {
  time+=FRAMETIME;
//  swirlShader.time=time;
//  swirlShader.resolution[0]=swirlShader.resolution[1]=512.0f;
//  swirlShader.center[0]=(512.0f)*iratiof(input->myi,display->h);
//  swirlShader.center[1]=(512.0f)*iratiof(input->mxi,display->w);
//  if ( input->left ) swirlShader.radius=(float)input->mxi;
//  if ( input->right ) swirlShader.angle=iratiof(input->myi,display->w)*5.0f;
//  frostedShader.scale=iratiof(input->mxi,display->w);
//  rippleShader.time=time;
//  rippleShader.resolution[0]=rippleShader.resolution[1]=512.0f;
//  rippleShader.scale=iratiof(input->mxi,display->w);
  shockwaveShader.time=time;
  shockwaveShader.center[0]=iratiof(input->mxi,display->w);
  shockwaveShader.center[1]=iratiof(input->myi,display->h);
  if ( input->left ) shockwaveShader.shock[0]=(float)input->mxi;
  if ( input->middle ) shockwaveShader.shock[1]=iratiof(input->myi,display->w)*5.0f;
  if ( input->right ) shockwaveShader.shock[2]=iratiof(input->myi,display->w)*5.0f;
  if ( input->KeyDown(DX_SPACE) ) time=0.0f;
 }
 void Render() {
  Blending(none);
  art.Stretch(&shockwaveShader,64,64,512,512);
 }
};

