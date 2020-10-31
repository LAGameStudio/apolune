#pragma once

#include "GLWindow.h"

#include "SolarSystemStarfield.h"
#include "FastGUI.h"

class StarfieldTestView : public GLWindow {
public:
 Zp<SolarSystemStarfield> starfield;
 Vertexd starfieldVelocity;
 Cartesiand velocity;
 Zp<SolarSystem> system;
 Zdouble angle,speed;
 void OnLoad() {
  if ( universe.systems.count == 0 ) universe.BigBang();
  Fullscreen();
  background.Pick(clear);
  starfield=new SolarSystemStarfield;
  starfield->ChangeSystem(123);//(SolarSystem *) universe.systems.first);
  system=starfield->system;
  starfield->velocity=&starfieldVelocity;
  AddChild(starfield);
 }
 void Between() {
  starfieldVelocity.Set(velocity.x,velocity.y);
  if ( input->KeyDown(DX_F1) ) {
   DropChildren();
   system=(SolarSystem *) system->next;
   if ( !system ) system=(SolarSystem *) universe.systems.first;
   starfield=new SolarSystemStarfield;
   starfield->ChangeSystem(123);// system);
   starfield->velocity=&starfieldVelocity;
   AddChild(starfield);
  }
 }
 void Render() {
 }
 void RenderAfter() {
  fast.vdisplay.Set(display->w,display->h);
  fast.member=null;
  fast.vwindows=null;
  fast.ResponsiveRatio(display->w,display->h);
  fast.text(glowTermFont,system->name.c_str(),display->w2,8,crayons.Pick(skyBlue));
  angle=fast.angleDegrees(angle,128,128,64);
  speed=fast.decimal("Speed",speed,0.0,32.0,0.25,128,512,32,16,7,true);
  fast.text(glowTermFont,FORMAT("x %1.2f y %1.2f",(double)velocity.x,(double)velocity.y).c_str(),512,1024);
  velocity.Vector(angle,speed);
 }
};

