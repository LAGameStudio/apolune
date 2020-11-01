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

#include "StarNestShader.h"

class StarNestTest : public GLWindow {
public:
 Zp<GLImage> sampler;
 Zp<FBOColor> out;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  out=&fbo1080p2;
  sampler=library.Load("data/images/white.png",tllto_Tiling);
 }
 Zfloat time;
 void Render() {
  time+=FRAMETIME;
  int drawing=0;
  if ( input->KeyDown(DX_1) ) drawing=1;
  if ( input->KeyDown(DX_2) ) drawing=2;
  if ( input->KeyDown(DX_3) ) drawing=3;
  switch ( drawing ) {
   case 0:        ShadedStarNest( time, 12345.0f+(float) input->mxd, 12345.0f+(float) input->myd, 1.0f, 1.0f, out );        break;// draws to FBO
   case 1:       ShadedStarNest2( time, 12345.0f+(float) input->mxd, 12345.0f+(float) input->myd, 1.0f, 1.0f, out );       break;// draws to FBO
   case 2:  ShadedStarNestSimple( time, 12345.0f+(float) input->mxd, 12345.0f+(float) input->myd, 1.0f, 1.0f, out  );  break;// draws to FBO
   case 3: ShadedStarNestSimple2( time, 12345.0f+(float) input->mxd, 12345.0f+(float) input->myd, 1.0f, 1.0f, out  ); break;// draws to FBO
   default: break;
  }
  out->background.Pick(alabaster);
  out->tint.Pick(alabaster);
  out->tint.gl();
  Blending(additive);
  out->Render(out->tint,additive,0.0,0.0,display->wd,display->hd);
#if defined(DEBUG)
  if ( input->KeyDown(DX_F8) ) FBODebug();
#endif
  Blending(none);
 }
};

