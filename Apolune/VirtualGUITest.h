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

