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
#include "VBO.h"
#include "FBO.h"
#include "TextureLibrary.h"
#include "Matter.h"
#include "MapPosition.h"
#include "UglyFont.h"
#include "StencilShader.h"

class TexturedFBOPLYTest : public GLWindow
{
public:
 Zp<GLImage> texture;
 Zp<Matter> m;
 Matters matters;
 ImmediateLighting lights;
 LookAtPerspective lap;
 MapPosition map;
 Zstring output;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Int(64,128,64,255);
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,-10.0);
  lap.Near=0.0001;
  lap.Far=80.0;
  Pick();
  map.scale.Set(1.0,1.0,1.0);
  if ( matter_detail_textures.count == 0 ) {
   matter_detail_textures.Add("data/images/details/1.png");
   matter_detail_textures.Add("data/images/details/2.png");
   matter_detail_textures.Add("data/images/details/3.png");
   matter_detail_textures.Add("data/images/details/4.png");
   matter_detail_textures.Add("data/images/details/5.png");
   matter_detail_textures.Add("data/images/details/6.png");
   matter_detail_textures.Add("data/images/details/7.png");
   matter_detail_textures.Add("data/images/details/8.png");
   matter_detail_textures.Add("data/images/details/9.png");
   matter_detail_textures.Add("data/images/details/10.png");
   matter_detail_textures.Add("data/images/details/11.png");
   matter_detail_textures.Add("data/images/details/12.png");
   matter_detail_textures.Add("data/images/details/13.png");
   matter_detail_textures.Add("data/images/details/14.png");
   matter_detail_textures.Add("data/images/details/15.png");
  }
 }
 void Pick() {
  m=new Matter( (PLY*)lowPLYprimitives.Any(), substances.Random() );
  matters.Append(m);
  switch ( upto(15) ) {
  case 0: texture=library.Load("data/images/details/1.png",tllto_Tiling); break;
  case 1: texture=library.Load("data/images/details/2.png",tllto_Tiling); break;
  case 2: texture=library.Load("data/images/details/3.png",tllto_Tiling); break;
  case 3: texture=library.Load("data/images/details/4.png",tllto_Tiling); break;
  case 4: texture=library.Load("data/images/details/5.png",tllto_Tiling); break;
  case 5: texture=library.Load("data/images/details/6.png",tllto_Tiling); break;
  case 6: texture=library.Load("data/images/details/7.png",tllto_Tiling); break;
  case 7: texture=library.Load("data/images/details/8.png",tllto_Tiling); break;
  case 8: texture=library.Load("data/images/details/9.png",tllto_Tiling); break;
  case 9: texture=library.Load("data/images/details/10.png",tllto_Tiling); break;
  case 10: texture=library.Load("data/images/details/11.png",tllto_Tiling); break;
  case 11: texture=library.Load("data/images/details/12.png",tllto_Tiling); break;
  case 12: texture=library.Load("data/images/details/13.png",tllto_Tiling); break;
  case 13: texture=library.Load("data/images/details/14.png",tllto_Tiling); break;
  case 14: texture=library.Load("data/images/details/15.png",tllto_Tiling); break;
  default: break;
  }
  OUTPUT("Picked `%s` on `%s`\n", m->ply->name.c_str(),
   m->substance->material->name.c_str()
  );
  output=string("");
  for ( int i=0; i< (int)m->ply->model.vboVNT->data.size() && i <100; i++ ) {
   output+=
    FORMAT(buf,1024,"Vert #%-3d  Pos: %1.2f  %1.2f  %1.2f  Normal: %1.2f  %1.2f  %1.2f  TC: %1.2f  %1.2f\n",
     i,
     (float) m->ply->model.vboVNT->data[i].pos[0],
     (float) m->ply->model.vboVNT->data[i].pos[1],
     (float) m->ply->model.vboVNT->data[i].pos[2],
     (float) m->ply->model.vboVNT->data[i].normal[0],
     (float) m->ply->model.vboVNT->data[i].normal[1],
     (float) m->ply->model.vboVNT->data[i].normal[2],
     (float) m->ply->model.vboVNT->data[i].texCoord[0],
     (float) m->ply->model.vboVNT->data[i].texCoord[1]
   );
  }
 }
 Zdouble typeDelay;
 Zbool lightsOff,layer1,layer2;
 void Between() {
  if ( typeDelay > 0.0 ) typeDelay-=FRAMETIME;
  if ( typeDelay > 0.0 ) return;
  if ( input->KeyDown(DX_SPACE) ) {
   Pick();
   typeDelay=0.25;
  }
  if ( input->KeyDown(DX_DELETE) ) {
   matters.Clear();
   Pick();
   matters.seed=upto(100000);
   typeDelay=0.25;
  }
  if ( input->KeyDown(DX_BACKSPACE) ) {
   lightsOff=!lightsOff;
   typeDelay=0.25;
  }
  if ( input->KeyDown(DX_RSHIFT) ) {
   layer1=!layer1;
   typeDelay=0.25;
  }
  if ( input->KeyDown(DX_RETURN) ) {
   layer2=!layer2;
   typeDelay=0.25;
  }
  if ( input->KeyDown(DX_LEFT) ) map.position.x-=0.1;
  if ( input->KeyDown(DX_RIGHT) ) map.position.x+=0.1;
  if ( input->KeyDown(DX_UP) ) map.position.y-=0.1;
  if ( input->KeyDown(DX_DOWN) ) map.position.y+=0.1;
  if ( input->KeyDown(DX_PGUP) ) map.position.z+=0.1;
  if ( input->KeyDown(DX_PGDN) ) map.position.z-=0.1;
  map.rotation.x=180.0*iratiod(input->mxi,display->w);
  map.rotation.y=180.0*iratiod(input->myi,display->h);
  if ( input->KeyDown(DX_NUMPADPLUS) ) map.scale.z+=0.1;
  if ( input->KeyDown(DX_NUMPADMINUS) ) map.scale.z-=0.1;
 }
 void Render() {
  Crayon c; c.Pick(alabaster);
  c.gl();
  Area(crayons.Pick(hotPink),100,100,500,500);
  glEnable(GL_TEXTURE_2D);
  int i=matters.seed; if ( i == 0 ) i=1;
  int j=0;
  EACH(matters.first,Matter,matter) {
   Cartesian rect;
   rect.SetRect(50+j*64+j*4,825,64,64);
   GLImage *tex=matter_detail_textures.Any(i);
   QuadiNoAlpha(tex,crayons.Pick(alabaster),none,rect.x,rect.y,rect.w,rect.h);
   Rectangle(c,rect.x,rect.y,rect.x2,rect.y2);
   i++;
   j++;
  }

  if ( layer1 ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   glPush();
   lap.Apply();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glTranslated(map.position.x,map.position.y,map.position.z);
   glRotated(map.rotation.x, 0.0, 1.0, 0.0);
   glRotated(map.rotation.y, 1.0, 0.0, 0.0);
   glRotated(map.rotation.z, 0.0, 0.0, 1.0);
   glScaled(map.scale.x,map.scale.y,map.scale.z);
   glColor3d(1.0,1.0,1.0);
   Blending(none);
   glEnable(GL_TEXTURE_2D);
   matters.RenderDetail();
   glPop();
   fboScreen2.Unbind();
   fboScreen2.Render(transparency,50+128+5,600,50+128+5+128,600+128);
  }

  if ( layer2 ) {
   fboScreen1.background.Pick(black);
   fboScreen1.Bind();
   glPush();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   lap.Apply();
   if ( !lightsOff ) lights.On();
   glTranslated(map.position.x,map.position.y,map.position.z);
   glRotated(map.rotation.x, 0.0, 1.0, 0.0);
   glRotated(map.rotation.y, 1.0, 0.0, 0.0);
   glRotated(map.rotation.z, 0.0, 0.0, 1.0);
   glScaled(map.scale.x,map.scale.y,map.scale.z);
   glColor3d(1.0,1.0,1.0);
   Blending(none);
   glEnable(GL_TEXTURE_2D);
   matters.Render();
   if ( !lightsOff ) lights.Off();
   glPop();
   fboScreen1.Unbind();
   fboScreen1.Render(none,50+128+5+128+5,600,50+128+5+128+5+128,600+128);
  }

//  QuadiNoAlpha(texture,crayons.Pick(alabaster),none,50,600,128,128);

  stencilMult2AlphaShader.pTex0=&fboScreen1.texture;
  stencilMult2AlphaShader.pTex1=&fboScreen2.texture;
  art.Stretch(&stencilMult2AlphaShader,100,100,400,400);

  Rectangle(crayons.Pick(blue),100,100,500,500);
  glColor3d(0.0,0.0,0.0);
  Text(output,600.0,50.0,5,6,2,1);

  Text(FORMAT(buf,123,"Matters: %d",(int)matters.count),50,800,8,10,false);
 }
};

