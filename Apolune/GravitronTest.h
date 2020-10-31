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
#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Display.h"
#include "Interface.h"

#include "Art.h"
#include "Presentation.h"
#include "Particles.h"
#include "TextureLibrary.h"

#include "Gravitron2d.h"

class GravitronTest : public GLWindow {
public:
 GravitronEmitterf2d ge;
 Zpointer<GLImage> A;
 Zbool dragging;
 GravitronTest() : GLWindow() {}
 void OnLoad() {
  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  ge.Create(200,this->w,this->h);
  ge.gravitant.Set(1.0f,1.0f,1.0f,1.0f,1.0f);
 }
 void Between() {
  ge.Between();
 }
 void Render() { 
  ge.Render();
 }
 void LeftDown( ) {
  ge.Create(200,this->w,this->h);
 }
 void MouseMove( ) {
  ge.gravitant.x=(float)(input->mxi-w2);
  ge.gravitant.y=(float)(input->myi-h2);
 }
 void LeftUp( ) {
 }
};

