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

