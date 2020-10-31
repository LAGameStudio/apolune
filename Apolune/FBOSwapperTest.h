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
#include "GLWindow.h"
#include "FBO.h"
#include "Art.h"
#include "TextureLibrary.h"

class FBOSwapperTest : public GLWindow {
public:
 FBOColor *f1,*f2;
 GLImage *test;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  test=library.Load("data/images/elements/rounder_minigrid.png");
  f1=&fboScreen1;
  f2=&fboScreen2;  
 }
 void Render() {
  f1->Bind();
  FBOTexturedQuad( test, crayons.Pick(green), additive,0,0,f1->w,f1->h);
  f1->Unbind();
  f2->Bind();
  f1->Render(none,0,0,w,h);
  f2->Unbind();
  f2->Render(none,0,0,w,h);
  //f1->Renderf(0.0f,0.0f,(float)f2->w,(float)f2->h);
  //f2->Unbind();
  //f2->Render(none,0,0,w,h);
 }
};