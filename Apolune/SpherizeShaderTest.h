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

#include "TextureLibrary.h"
#include "UglyFont.h"
#include "SpherizeShader.h"

class SpherizeShader2Test : public GLWindow {
public:
 GLImage *test;
// float tolerance;
 float dispersion;
// float blurAmount;
 float k,kcube,/*vignette_size,*/scale;
 float center[2], canvas[2], disperseComponents[3];
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(red255);
  Reset();
  test=library.Load("data/images/encyclopedia/NASA_70s_colony_lrg.jpg");
 }
 void Reset() {
//  tolerance=0.2f;
  dispersion=0.01f;
//  blurAmount=1.0f;
  k=0.2f;
  kcube=0.3f;
//  vignette_size=0.5f;
  scale=0.85f;
  center[0]=0.5f;
  center[1]=0.5f;
  canvas[0]=1.0f;
  canvas[1]=1.0f;
  disperseComponents[0]=0.9f;
  disperseComponents[1]=0.6f;
  disperseComponents[2]=0.3f;
 }
 void Render() {
//  spherizeShader2.tolerance=tolerance;
  spherizeShader2.dispersion=dispersion;
//  spherizeShader2.blurAmount=blurAmount;
  spherizeShader2.k=k;
  spherizeShader2.kcube=kcube;
  spherizeShader2.scale=scale;
//  spherizeShader2.vignette_size=vignette_size;
  spherizeShader2.center[0]=center[0];
  spherizeShader2.center[1]=center[1];
  spherizeShader2.canvas[0]=canvas[0];
  spherizeShader2.canvas[1]=canvas[1];
  spherizeShader2.disperseComponents[0]=disperseComponents[0];
  spherizeShader2.disperseComponents[1]=disperseComponents[1];
  spherizeShader2.disperseComponents[2]=disperseComponents[2];
  ShadedSpherize2( &test->texture, &fboScreen1 );
  fboScreen1.RenderUpsideDown(0.0,0.0,display->wd,display->hd);
  char buf[80];
  FORMAT(buf,80,"center: %f, %f", (float) center[0], (float) center[1]);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"canvas: %f, %f", (float) canvas[0], (float) canvas[1]);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+20.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+20.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"dispersion %f", (float) dispersion); //"tolerance= %f, dispersion=%f", tolerance, dispersion );//, blurAmount=%f", tolerance, dispersion, blurAmount );
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+40.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+40.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"k= %f, kcube=%f", (float) k, (float) kcube);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+60.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+60.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"scale: %f", (float) scale ); // vignette: %f", scale, vignette_size);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+80.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+80.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"RGB: %f - %f - %f", (float) disperseComponents[0], (float) disperseComponents[1], (float) disperseComponents[2] );
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+100.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+100.0-1.0,10.0,8.0,false);
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  if ( input->right ) {
   center[0]=iratiof(mx,display->w)*2.0f-1.0f;
   center[1]=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->left ) {
   canvas[0]=iratiof(mx,display->w)*2.0f-1.0f;
   canvas[1]=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->KeyDown(DX_R) ) {
   disperseComponents[0]-=FRAMETIME; if ( disperseComponents[0] < 0.0f ) disperseComponents[0]=5.0f;
  }
  if ( input->KeyDown(DX_G) ) {
   disperseComponents[1]-=FRAMETIME; if ( disperseComponents[1] < 0.0f ) disperseComponents[1]=5.0f;
  }
  if ( input->KeyDown(DX_B) ) {
   disperseComponents[2]-=FRAMETIME; if ( disperseComponents[2] < 0.0f ) disperseComponents[2]=5.0f;
  }
  if ( input->KeyDown(DX_A) ) {
   k=iratiof(mx,display->w);
  }
  /*
  if ( input->KeyDown(DX_Z) ) {
   vignette_size=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->KeyDown(DX_S) ) {
   tolerance=iratiof(my,display->h)*2.0f-1.0f;
  }
  */
  if ( input->KeyDown(DX_X) ) {
   kcube=iratiof(my,display->h)*400.0f-200.0f;
  }
  if ( input->KeyDown(DX_D) ) {
   dispersion=iratiof(my,display->h)*20.0f-10.0f;
  }
  /*
  if ( input->KeyDown(DX_C) ) {
   blurAmount=iratiof(my,display->h)*2.0f-1.0f;
  }
  */
  if ( input->KeyDown(DX_V) ) {
   scale=iratiof(my,display->h)*100.0f;
  }
  if ( input->KeyDown(DX_SPACE) ) {
   Reset();
  }
 }
};