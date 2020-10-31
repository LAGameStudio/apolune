#pragma once

#include "Shadows2DShader.h"
#include "FastGUI.h"

class Shadow2DTest : public GLWindow {
public:
 Shadow2DProfile s2dProfile;
 void OnLoad() {
  Fullscreen();
  background.Pick(red255);
  s2dProfile.colormap=library.Load("data/images/elements/shadow2dtest_h.png",tllto_Tiling);
  s2dProfile.heightmap=library.Load("data/images/elements/shadow2dtest_h.png",tllto_Tiling);
  s2dProfile.lightmap=library.Load("data/images/elements/shadow2dtest_light.png",tllto_Tiling);
  s2dProfile.backlight=library.Load("data/images/elements/white.png",tllto_Tiling);
  s2dProfile.ambience.Double(0.25,0.25,0.25,1.0);
  s2dProfile.pos.z=0.95;
 }
 void Between() {
  if ( input->wheelUp ) s2dProfile.pos.z+=0.01;
  if ( input->wheelDown ) s2dProfile.pos.z-=0.01;
  s2dProfile.pos.x=1.0+(input->mxd-0.5)*2.0;
  s2dProfile.pos.y=1.0+(input->myd-0.5)*2.0;
 }
 void Render() {
  ShadedShadow2D(&fbo1080p,&s2dProfile);
  fbo1080p.Render(8,8,display->w-8,display->h-8);
  fast.text(s2dProfile.pos.toString().c_str(),16,16);
 }
};

