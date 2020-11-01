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
#include <Windows.h>

#include "GLWindow.h"
#include "LinkedList.h"

class GLWindowTest3 : public GLWindow
{
public:
 bool dragging;
 int sx,sy;
 virtual void OnLoad(void) {
  dragging=false;
  background.Float(1.0f,0.5f,0.7f,0.8f);
 }
 virtual void BetweenFrames(void) {
  background.SubWrapf( 1.0f/FPS );
 }
 virtual void LeftDown(void) {
  dragging=true;
  sx=input->mxi;
  sy=input->myi;
 }
 virtual void MouseMove(void) {
  if ( dragging ) {
   sx=input->mxi-sx; 
   sy=input->myi-sy; 
   MoveBy(sx,sy);
   sx=input->mxi;
   sy=input->myi;
  }
 }
 virtual void MouseLeave(void) {
  dragging=false;
 }
 virtual void LeftUp(void) { 
  dragging=false;
 }
};

