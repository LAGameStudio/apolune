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
#include "Matter.h"
#include "LookAtPerspective.h"

class GLFBOColorDepthStencilTest : public GLWindow {
public:
 LookAtPerspective lap;
 PLY *p,*q;
 FBOColorDepthStencil fbo;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  fbo.Create(display->w,display->h);
  p=lowPLYprimitives.Random();
  q=lowPLYprimitives.Random();
 }
 void Between() {
  if ( input->KeyDown(DX_SPACE) ) {
   p=lowPLYprimitives.Random();
   q=lowPLYprimitives.Random();
  }
  int mx,my;
  MousePosition(&mx,&my);
  if ( input->left ) {
   if ( WITHIN(mx,my,5,5,5+512,5+512) ) {
    fbo.BindWithoutClearing();
    fbo.ReadPixel(gl_color0,
     (int)(iratiod(mx-5,512)*display->w),
     (int)(iratiod(my-5,512)*display->h)
    );
    fbo.Unbind();
   } else
   if ( WITHIN(mx,my,5+512+5,5,5+512+5+512,5+512) ) {
    fbo.BindWithoutClearing();
    fbo.ReadPixel(gl_stencil,
     (int)(iratiod(mx-(5+512+5),512)*display->w),
     (int)(iratiod(my-5,512)*display->h)
    );
    fbo.Unbind();
   }
  }
 }
 void Render() {
  fbo.background.Pick(red255);
  fbo.blend=transparency;
  fbo.Bind();
  glPush();
  glEnable(GL_DEPTH_TEST);
  fbo.StartStenciling();
  lap.Apply();
  glColor3d(1.0,0.0,1.0);
  Blending(none);
  glDisable(GL_TEXTURE_2D);  
  p->model.Render();
  glColor3d(1.0,1.0,0.0);
  q->model.Render();
  fbo.StopStenciling();
  glPop();
  fbo.Unbind();
  fbo.Render(5,5,5+512,5+512);
  fbo.RenderStencil(5+512+5,5,5+512+5+512,5+512);

  Blending(none);
  Area(this,crayons.jami(0,fbo.ub_bgra[2],fbo.ub_bgra[1],fbo.ub_bgra[0],fbo.ub_bgra[3]),5+512+32,5+512+32,5+512+32+32,5+512+32+32);
//  Area(this,crayons.jami(1,fbo.ui_bgra[0],fbo.ui_bgra[1],fbo.ui_bgra[2],fbo.ui_bgra[3]),5+512+32+32,5+512+32,5+512+32+32+32,5+512+32+32);
  char buf[128];
  glColor3d(0.0,1.0,0.0);
  FORMAT(buf,128,"r_Stencil: %d, r_Depth: %d f_Depth: %f\nub_b: %d ub_g: %d ub_r: %d ub_a: %d",
   (int) fbo.r_stencil, (int) fbo.r_depth, (float) fbo.f_depth,
   (unsigned char) fbo.ub_bgra[0],
   (unsigned char) fbo.ub_bgra[1],
   (unsigned char) fbo.ub_bgra[2],
   (unsigned char) fbo.ub_bgra[3] );
  Text(buf,5.0,700.0,5,7,4,false);
 }
};

