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

class MultiDepthFBOTest : public GLWindow {
public:
 FBOColorDepth a1,b2,c3,d4,e5,f6;
 void OnLoad() {
  a1.Create(32,32);
  b2.Create(64,64);
  c3.Create(128,128);
  d4.Create(256,256);
  e5.Create(512,512);
  int ww,hh;
  f6.GetMaxDimensions(&ww,&hh);
  f6.Create(ww,hh);
 }
 void Render() {
  a1.Bind();
  a1.RenderTestSceneOnFBO();
  a1.Unbind();
  b2.Bind();
  b2.RenderTestSceneOnFBO();
  b2.Unbind();
  c3.Bind();
  c3.RenderTestSceneOnFBO();
  c3.Unbind();
  d4.Bind();
  d4.RenderTestSceneOnFBO();
  d4.Unbind();
  e5.Bind();
  e5.RenderTestSceneOnFBO();
  e5.Unbind();
  f6.Bind();
  f6.RenderTestSceneOnFBO();
  f6.Unbind();
 }
};


class MultiFBOStackTest : public GLWindow {
public:
 FBOColorDepth outer;
 FBOColorDepthStencil inner;
 void OnLoad() {
  inner.Create(512,512);
  int ww,hh;
  outer.GetMaxDimensions(&ww,&hh);
  outer.Create(ww,hh);
 }
 void Render() {
  outer.Bind();
  inner.Bind();
  inner.RenderTestSceneOnFBO();
  inner.Unbind();
  inner.Render(512,512,700,700);
  outer.Unbind();
  outer.Render(64,64,768+64,768+64);
 }
};