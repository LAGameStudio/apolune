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
#include "TextureLibrary.h"
#include "FBO.h"
#include "GLWindow.h"
#include "BlendShader.h"

class Blend8WayTest : public GLWindow {
public:
 Zp<GLImage> one,two,three,four,five,six,seven,eight;
 Zp<GLImage> one2,two2,three2,four2,five2,six2,seven2;
 void OnLoad() {
  Fullscreen();
  background.Pick(green255);

  /*
  one  =library.Load("data/images/textures/colors/red.jpg",tllto_Tiling);
  two  =library.Load("data/images/textures/colors/grassgreen.jpg",tllto_Tiling);
  three=library.Load("data/images/textures/colors/huntergreen.jpg",tllto_Tiling);
  four =library.Load("data/images/textures/colors/TEX-150_100_0.jpg",tllto_Tiling);
  five =library.Load("data/images/textures/colors/TEX-150_150_0.jpg",tllto_Tiling);
  six  =library.Load("data/images/textures/colors/TEX-200_100_100.jpg",tllto_Tiling);
  seven=library.Load("data/images/textures/colors/TEX-150_250_150.jpg",tllto_Tiling);
  eight=library.Load("data/images/textures/colors/250_250_0.jpg",tllto_Tiling);
  */

  /*
  one  =library.Load("data/images/geology/c/c1.png",tllto_Tiling);
  two  =library.Load("data/images/geology/c/c2.png",tllto_Tiling);
  three=library.Load("data/images/geology/c/c3.png",tllto_Tiling);
  four =library.Load("data/images/geology/c/c4.png",tllto_Tiling);
  five =library.Load("data/images/geology/c/c5.png",tllto_Tiling);
  six  =library.Load("data/images/geology/c/c6.png",tllto_Tiling);
  seven=library.Load("data/images/geology/c/c7.png",tllto_Tiling);
  eight=library.Load("data/images/geology/c/c8.png",tllto_Tiling);
  */

  one  =library.Load("data/images/geology/m/m1.png",tllto_Tiling);
  two  =library.Load("data/images/geology/m/m2.png",tllto_Tiling);
  three=library.Load("data/images/geology/m/m3.png",tllto_Tiling);
  four =library.Load("data/images/geology/m/m4.png",tllto_Tiling);
  five =library.Load("data/images/geology/m/m5.png",tllto_Tiling);
  six  =library.Load("data/images/geology/m/m6.png",tllto_Tiling);
  seven=library.Load("data/images/geology/m/m7.png",tllto_Tiling);
  eight=library.Load("data/images/geology/m/m8.png",tllto_Tiling);

  one2  =library.Load("data/images/geology/c/c1.png",tllto_Tiling);
  two2  =library.Load("data/images/geology/c/c2.png",tllto_Tiling);
  three2=library.Load("data/images/geology/c/c3.png",tllto_Tiling);
  four2 =library.Load("data/images/geology/c/c4.png",tllto_Tiling);
  five2 =library.Load("data/images/geology/c/c5.png",tllto_Tiling);
  six2  =library.Load("data/images/geology/c/c6.png",tllto_Tiling);
  seven2=library.Load("data/images/geology/c/c7.png",tllto_Tiling);
 }
 void Render() {
  Crayon channel1,channel2,channel3,channel4,channel5,channel6,channel7;
  channel1.Float(1.0,0.0,0.0,1.0);
  channel2.Float(0.0,1.0,0.0,1.0);
  channel3.Float(1.0,0.0,1.0,1.0);
  channel4.Float(0.0,0.0,0.0,1.0);
  channel5.Float(0.0,1.0,1.0,1.0);
  channel6.Float(1.0,1.0,0.0,1.0);
  channel7.Float(1.0,0.0,1.0,1.0);
  Crayon threshold;
  threshold.Float(0.85f,0.85f,0.85f,1.0f);
  Crayon blank(clear);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(one2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel1,blank,&fboScreen1.texture,&fboScreen2);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(two2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel2,blank,&fboScreen1.texture,&fboScreen3);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(three2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel3,blank,&fboScreen1.texture,&fboScreen4);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(four2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel4,blank,&fboScreen1.texture,&fboScreen5);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(five2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel5,blank,&fboScreen1.texture,&fboScreen6);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(six2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel6,blank,&fboScreen1.texture,&fboScreen7);
  fboScreen1.Bind(); fboScreen1.QuadYInverted(seven2,50.0,50.0); fboScreen1.Unbind();
  ShadedColorAboveBelow(threshold,channel7,blank,&fboScreen1.texture,&fboScreen8);

  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();
  glColor3d(1.0,1.0,1.0);
  Blending(transparency);
  fboScreen1.QuadYInverted(&fboScreen2);
  fboScreen1.QuadYInverted(&fboScreen3);
  fboScreen1.QuadYInverted(&fboScreen4);
  fboScreen1.QuadYInverted(&fboScreen5);
  fboScreen1.QuadYInverted(&fboScreen6);
  fboScreen1.QuadYInverted(&fboScreen7);
  fboScreen1.QuadYInverted(&fboScreen8);
  fboScreen1.Unbind();

  ShadedBlend8Way(
   &fboScreen1.texture,
   &eight->texture,
   &seven->texture,
   &six->texture,
   &five->texture,
   &four->texture,
   &three->texture,
   &two->texture,
   &one->texture,
   &fboScreen
  );
  fboScreen.RenderUpsideDown(0,0,display->w,display->h);

#if defined(DEBUG)
  FBODrawDebug();
  FBODebugKeys();
#endif
 }
};