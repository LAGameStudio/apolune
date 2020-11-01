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
#include "GLSL2Shader.h"

#include "fx_Shader2d.h"

#include "ColorShader.h"
#include "ThermalShader.h"
#include "Tunnel2Shader.h"
#include "SwirlShader.h"
//#include "Sphere2Shader.h"
#include "SniperLensShader.h"
#include "RippleShader.h"
#include "DreamBlurShader.h"
#include "PixelateShader.h"
#include "FrostedShader.h"
#include "Frosted2Shader.h"
#include "GammaShader.h"
#include "ShockwaveShader.h"
#include "Nebula4LightShader.h"
#include "NightvisionShader.h"
#include "CrosshatchShader.h"
#include "ConvolverShader.h"
#include "ImplodeShader.h"
#include "BloomShader.h"
#include "FBMShader.h"
#include "BendShader.h"
#include "BlendShader.h"
#include "ParallaxShader.h"
#include "ProtoShader.h"
#include "BillboardShader.h"
#include "StarNestShader.h"
#include "SpaceShaders.h"
#include "ASCIIShader.h"
#include "TexShader.h"


// Contains a sort-of "ultimate test" for shaders


class ShaderLab : public GLWindow {
public:
 Zp<GLImage> testImageClipped,testImageWrapping;
 Zp<GLImage> otherTestImage, noiseTex, sampler;
 Zbool editCreateMode;
 Zint testMode;
 Zp<FBOColor> fA,fB,fC;
 OldTVProfile otv;
 ASCIIProfile ascii;
 WaterAndGroundProfile wandg;
 Zdouble t;
 ElectroCloudsProfile electroClouds;
 ElectronCloudsProfile electronClouds;
 EmissionCloudsProfile emissionClouds;
 void OnLoad() {
  Fullscreen();
  fA = &fboScreen;
  fB = &fboScreen2;
  fC = &fboCDS512;
  testImageClipped = library.Load("data/images/splash2.png");
  testImageWrapping = library.Load("data/images/splash2.png",GL_LINEAR,GL_NEAREST,GL_TEXTURE_WRAP_S,GL_TEXTURE_WRAP_T);
  otherTestImage = library.Load("data/images/splash.png");
  otv.tex=&otherTestImage->texture;
  ascii.tex0=testImageClipped;
  noiseTex=library.Load("data/images/noise1.png",tllto_Tiling);
  sampler=library.Load("data/images/white.png",tllto_Tiling);
 }
 void Between() {
  t+=FRAMETIME;
  electroClouds.position.Set(input->mxd,input->myd);
  electronClouds.pos2d.Set(input->mxd,input->myd);
  emissionClouds.pos2d.Set(input->mxd,input->myd);
 }
 void Render() {
  //GLuint *tex = input->KeyDown(DX_LCTRL) ? &noiseTex->texture : &sampler->texture;
  //if ( input->KeyDown(DX_1) )      ShadedStarNest2(t,input->mxd*1.0,input->myd*1.0,display->wf,display->hf,fA);
  //else if ( input->KeyDown(DX_2) ) ShadedStarNestSimple(t,input->mxd*1.0,input->myd*1.0,display->wf,display->hf,fA);
  //else if ( input->KeyDown(DX_3) ) ShadedStarNestSimple2(t,input->mxd*1.0,input->myd*1.0,display->wf,display->hf,fA);
  //else if ( input->KeyDown(DX_4) ) ShadedStarNest(t,input->mxd*1.0,input->myd*1.0,display->wf,display->hf,fA);
  //else if ( input->KeyDown(DX_5) ) ShadedElectroClouds(fA,&electroClouds);
  //else if ( input->KeyDown(DX_6) ) ShadedElectronClouds(fA,&electronClouds);
  //else ShadedEmissionClouds(fA,&emissionClouds);
  //fA->Render(0.0,0.0,display->wd,display->hd);
  //Text(FORMAT("%1.1fs",(double)t).c_str(),crayons.Pick(macNcheese),16.0,display->hd-64.0);
 /* tvShader.time+=FRAMETIME;
  ShadedTV(&testImageClipped->texture,fA);
  fA->Render(15.0,15.0,655.0,495.0);
  otv.time+=FRAMETIME;
  ShadedOldTV(fB,&otv);
  fB->Render(0.0,0.0,display->wd,display->hd);
  swirlShader.angle = PIf;
  swirlShader.center[0] = (float) (input->mxd/display->wd * 512.0);
  swirlShader.center[1] = (float) (input->myd/display->hd * 512.0);
  swirlShader.resolution[0] = 1.0f;
  swirlShader.resolution[1] = 1.0f;
  ShadedSwirl(&testImageWrapping->texture,fC);
  fC->Render(15.0,500.0,15.0+512.0,500.0+512.0);*/
  /*
  wandg.pos2d.Set(input->mxd,input->myd);
  ShadedWaterAndGround(fA,&wandg);
  fA->Render(0.0,200.0,400.0,800.0);
  ShadedWaterAndGround(fB,&wandg);
  fB->Render(1000.0,800.0,1200.0,1000.0);
  ShadedWaterAndGround(fC,&wandg);
  fC->Render(1000.0,800.0,1200.0,1000.0);
  */
#if defined(DEBUG)
  FBODebug();
#endif
 }
};

