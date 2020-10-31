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