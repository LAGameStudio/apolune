#pragma once

#include "VirtualWindow.h"

class VirtualMoveableWindow : public Proce55or {
public:
 VirtualMoveableWindow() : Proce55or() { name="My Window!!"; }
 VirtualWindow v;
 Zint i;
 void Setup() {
  v.title=name;
  v.noResize=false;
  v.noClose=false;
  v.noMaximize=false;
  v.noMinimize=false;
  this->SetExtents(512,256,128,128);
  v.SizeAndPosition(x,y,w,h);
 }
 void Render() {
  v.Render();
  crayons.Pick(ANSIboldgreen).gl();
  Text(FORMAT("%1.1f,%1.1f X %1fx%1f",
    v.position.x,
    v.position.y,
    v.position.w,
    v.position.h
   ).c_str(),
   x+w/2,y+h/2,10.0,12.0,
   true
  );
 }
 void BetweenFrames() {
  if ( v.resizing || v.moving ) {
   this->SetExtents(
    (int)v.position.x,
    (int)v.position.y,
    (int)v.position.w,
    (int)v.position.h
   );
  }
 }
 void OnSetExtents() {
  OUTPUT("FOO");
 }
};

class VirtualGUITest : public GLWindow {
public:
 Proce55ors fx;
 void OnLoad() {
  Fullscreen();
  background.Pick(brown);
  virtualStyleDefault.Defaults();
  fx.Add(new VirtualMoveableWindow,this);
  fx.Setup();
 }
 void Render() {
  fx.Render();
 }
 void Between() {
  fx.Between();
 }
};

