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

#if defined(USE_PBUFFER)

#include "pBuffer.h"
#include "GLWindow.h"
#include "Primitives2d.h"
#include "Camera.h"

/*
 * A stretchable windowed surface rendered to a pBuffer.
 */

class pBufferWindow : public GLWindow
{
public:
 gCamera camera;
 pBuffer *pbuffer;
 virtual void PreProcess() {}
 virtual void pBufferRender() {}
 virtual void PostProcess() {}
 void Render() {
  PreProcess();
  pbuffer->MakeCurrent();
  pBufferRender();
  display->MakeCurrent();
  PostProcess();
 }
 pBufferWindow( Display *d, Interface *i, int W, int H, Crayon bg ) {
  this->display = d;
  this->input = i;
  this->InitGLWindow();
  pbuffer = new pBuffer();
  pbuffer->CreateDefault(d,W,H,&camera,bg);
 }
 ~pBufferWindow(void) {
  this->DeinitGLWindow();
 }
};

#endif