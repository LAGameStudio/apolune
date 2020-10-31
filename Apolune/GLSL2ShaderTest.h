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

#include "Art.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "GLSL2Shader.h"

#include "MonjoriShader.h"
#include "ClodShader.h"
#include "PlasmaShader.h"
#include "VortexShader.h"
#include "DeformerShader.h"
#include "PulseShader.h"
#include "GlasscaveShader.h"
#include "LuxShader.h"
#include "RadialShader.h"
#include "NautilusShader.h"
#include "TVShader.h"

#include "TexFont.h"
#include "fx_Shader2d.h"
#include "VisualAid.h"

#include "LimitedProce55Viewer2d.h"

extern Interface input;

void LoadGLSL2ShaderTest();

class GLSL2ShaderTest : public LimitedProce55Viewer2d {
public:
 fx_Shader2d *shader;
 fx_Shader2d *shader2;
 fx_Shader2d *shader3;
 fx_Shader2d *shader4;
 int w2;
 Cartesian position;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
  GL_Assert( "GLSL2ShaderTest::OnLoad()" );
  shader=new fx_Shader2d;
  shader->w=w;
  shader->h=h;
  shader->Select(monjori);
  shader->position.Setf(-(float)w/4.0f,-(float)h/4.0f);
  shader->blend=solarize;
  shader->size=256.0f;
 // Add(shader);
  shader2=new fx_Shader2d;
  shader2->w=w;
  shader2->h=h;
  shader2->Select(radial);
  shader2->position.Set(-(float)w/2.0f+(float)w/4.0f,-(float)h/2.0f+(float)h/3.0f);
  shader2->pTex0=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
  shader2->pTex1=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  shader2->blend=additive;
  shader2->size=256.0f;
  //Add(shader2);
  shader3=new fx_Shader2d;
  shader3->w=w;
  shader3->h=h;
  shader3->Select(tunnel);
  shader3->offset.Setf(0.51f,0.5f); // values of interest: 0.5, 0.5001 (pulsing), 0.501 (solar flares), 0.55 
  shader3->position.Set(-(float)w/2.0f+(float)w/4.0f,-(float)h/2.0f+(float)h/3.0f);
  shader3->pTex0=library.LoadGLuint("data/images/dust/dust12.png",tllto_Tiling);
  shader3->pTex1=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
  shader3->blend=solarize;
  shader3->size=256.0f;
  Add(shader3);
  shader4=new fx_Shader2d;
  shader4->w=w;
  shader4->h=h;
  shader4->Select(nautilus);
  shader4->resolution.Setf(1024.0f,1024.0f);
  shader4->offset.Setf(0.0f,0.0f); // values of interest: 0.5, 0.5001 (pulsing), 0.501 (solar flares), 0.55 
  shader4->position.Set(-(float)w/2.0f+(float)w/4.0f,-(float)h/2.0f+(float)h/3.0f);
  shader4->pTex0=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  shader4->pTex1=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
  shader4->blend=additive;
  shader4->size=256.0f;
  //Add(shader4);
  VisualAid2 *va2=new VisualAid2;
  va2->sx=0;
  va2->sy=0;
  va2->w=va2->w2=w;
  va2->h=va2->h2=h;
  va2->image=library.find("data/images/elements/white.png");
  va2->blend=multiply;
  va2->duration=10.0f;
  va2->Tint(crayons.Pick(red255));
  Add(va2);
  Init();
 }
 
 void BetweenFrames() {
  shader4->offset.fx=iratiof(input->mxi,w)*shader4->resolution.fx;
  shader4->offset.fy=iratiof(input->myi,h)*shader4->resolution.fy;
  if ( input->KeyDown(DX_LEFT) ) {
   shader->position.fx--;
   shader2->position.fx--;
   shader2->offset.fx-=1.0f/FPS;
//   if ( shader2->offset.fx < -PIf ) shader2->offset.fx=PIf;
   shader3->position.fx--;
   shader4->position.fx--;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   shader->position.fx++;
   shader2->position.fx++;
   shader2->offset.fx+=1.0f/FPS;
//   if ( shader2->offset.fx > PIf ) shader2->offset.fx=-PIf;
   shader3->position.fx++;
   shader4->position.fx++;
  }
  if ( input->KeyDown(DX_UP) ) {
   shader->position.fy--;
   shader2->position.fy--;
   shader2->offset.fy-=1.0f/FPS;
//   if ( shader2->offset.fy < -PIf ) shader2->offset.fy=PIf;
   shader3->position.fy--;
   shader4->position.fy--;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   shader->position.fy++;
   shader2->position.fy++;
   shader2->offset.fy+=1.0f/FPS;
//   if ( shader2->offset.fy > PIf ) shader2->offset.fy=-PIf;
   shader3->position.fy++;
   shader4->position.fy++;
  }
 }

 void PostProcessing() {
  char buf[64];
  FORMAT(buf,64,"%f,%f, %f,%f",
   (float) shader->position.fx,
   (float) shader->position.fy,
   (float) shader->offset.fx,
   (float) shader->offset.fy );
  WriteTexFont(this,microFont,crayons.Pick(green),none,10,10,7,16,2,buf);
  FORMAT(buf,64,"%f,%f, %f,%f", 
   (float) shader2->position.fx,
   (float) shader2->position.fy,
   (float) shader2->offset.fx,
   (float) shader2->offset.fy );
  WriteTexFont(this,microFont,crayons.Pick(green),none,30,30,7,16,2,buf);
 }
};
