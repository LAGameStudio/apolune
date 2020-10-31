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
#include "UglyFont.h"
#include "ScreenPosition.h"
#include "GLWindow.h"
#include "Interface.h"

class ScreenPositionTest : public GLWindow {
public:
 ScreenPosition screen;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Int(12,18,96,255);
 }
 void Between() {
  if ( input->KeyDown(LCtrl) ) {
   if ( input->left ) screen.Set(input->mx,input->my);
   else
   if ( input->middle ) screen.Center(input->mx,input->my);
  } else {
   if ( input->left ) screen.Set(input->mxi,input->myi);
   else
   if ( input->middle ) screen.Center(input->mxi,input->myi);
   else
   if ( input->right ) {
    int neww=input->mxi > screen.x ? input->mxi-screen.x : screen.x-input->mxi;
    int newh=input->myi > screen.y ? input->myi-screen.y : screen.y-input->myi;
    screen.Size(neww,newh);
   }
  }
 }
 void Render() {
  screen.DrawDebug();
  char buf[64];
  FORMAT(buf,64,"mouse: %d,%d",(int)input->mxi,(int)input->myi);
  Blending(none);
  glColor3d(0.0,1.0,0.0);
  Text(buf,10,10,8,8,false);
 }
};