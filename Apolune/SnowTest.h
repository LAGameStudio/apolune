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

#include "Snow2d.h"

class SnowTest : public GLWindow {
public:
 Snowd2d snow1,snow2;
 void OnLoad() {
  background.Pick(clear);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  snow1.bounds.SetRect(0,0,w2/2,h);
  snow1.colors.Hold(alabaster);
  snow1.colors.Hold(alabaster);
  snow1.colors.Hold(alabaster);
  snow1.colors.Hold(robinEggBlue);
  snow1.colors.Hold(silver);
  snow1.gravity.x=0.0;
  snow1.gravity.y=4.8;
  snow1.wind.x=-10.0;
  snow1.wind.y=0.0;
  snow1.movement.x=4.0;
  snow1.movement.y=1.0;
  snow1.Create(20,10,100,2.0f,0.10,3.0f,0.025f,0.035f);
  snow1.offset.x=-w2;
  snow1.offset.y=-200;
  snow1.images.addIfUnique(library.Load("data/images/flares/neoflare8.png"));
  snow2.bounds.SetRect(w2,0,w2,h);
  snow2.colors.Hold(alabaster);
  snow2.colors.Hold(brown);
  snow2.colors.Hold(alabaster);
  snow2.colors.Hold(robinEggBlue);
  snow2.colors.Hold(silver);
  snow2.gravity.x=0.0;
  snow2.gravity.y=4.8;
  snow2.wind.x=-10.0;
  snow2.wind.y=0.0;
  snow2.movement.x=4.0;
  snow2.movement.y=1.0;
  snow2.Create(20,10,100,2.0,0.10,3.0,0.025,0.035);
  snow2.offset.x=-w2;
  snow2.offset.y=-200;
  snow2.images.addIfUnique(library.Load("data/images/flares/neoflare8.png"));
 }
 void Between() {
  snow1.Between();
  snow2.Between();
 }
 void Render() {
  snow1.Render(this);
  snow2.Render(this);
 }
 void LeftDown() {
  snow1.wind.x=double_range(0.0,20.0)-10.0;
  snow2.wind.x=double_range(0.0,20.0)-10.0;
 }
 void RightDown() { 
 }
 void MouseMove() {
  int mx,my;
  MousePosition(&mx,&my);
  snow1.MoveTo(mx,my);
 }
};
