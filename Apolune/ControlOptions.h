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

#include "FormUI.h"
#include"HIDControlMapping.h"

class ControlOptions {
};

extern ControlOptions controlOptions;

class ControlOptionsEditor : public MoveableEditorWindow {
public:
 Strings modes;
 void OnSetup() {
  clip=false;
  v.Editing(&controls);
  v.background.Pick(black);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noTitleBar=false;
  v.noBacking=false;
  v.alwaysOnTop=false;
  v.noClose=false;
  v.noCenter=false;
  v.noClipTitle=true;
  v.minimumSize.Set(1024,600);
  v.maximumSize.Set(1024,600);
  v.SizeAndPosition(1920/2-300+600+8,1080/2-300,1024,600);
  Name("Control Settings");
 }
 void Between() {
 }
 void RenderWindow() {
  Cartesian pen;
  pen.Set(x+16,y+h-192);
  fast.text(FORMAT("Available Controllers\n\nLegacy Joystick: %s\nLogitech Device: %s\nKeyboard: %s\nMouse: %s\nTouch: %s\nX-Input Gamepads: %d\n",
   input.hasJoystick ? "Yes" : "No",
   input.hasKeyboard ? "Yes" : "No",
   input.hasLogitech ? "Yes" : "No",
   input.hasMouse    ? "Yes" : "No",
   input.hasTouch    ? "Yes" : "No",
   input.gamepads.Connected()
  ).c_str(),pen.x,pen.y);
  pen.Set(x+16,y+24);
  if ( fast.button("Reset to Defaults",pen.x,pen.y,256,32) ) {
   controls.Defaults();
  }
 }
};