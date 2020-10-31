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
#include "TextureLibrary.h"
#include "QuadGridVBO.h"
#include "UglyFont.h"

class QuadGridVBOTest : public GLWindow {
public:
 Zp<GLImage> texture;
 Zp<QuadGridVBO> quadgrid;
 Blends blend;
 Vertexd scale;
 void OnLoad() {
  Fullscreen();
  background.Pick(hotPink);
  texture=library.Load("data/images/computer/accessdenied.png");
  quadgrid=quadGrids.find("bigtest");
  blend=transparency;
  scale.Set(1.0,1.0,1.0);
 }
 Zdouble keyDelay;
 void Between() {
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else
  if ( input->KeyDown(DX_RIGHT) ) {
   quadgrid.pointer=(QuadGridVBO *) quadgrid->prev;
   if ( quadgrid.pointer == null ) quadgrid.pointer=(QuadGridVBO *) quadGrids.last;
   keyDelay=0.2;
  } else if ( input->KeyDown(DX_LEFT) ) {
   quadgrid.pointer=(QuadGridVBO *) quadgrid->next;
   if ( quadgrid.pointer == null ) quadgrid.pointer=(QuadGridVBO *) quadGrids.first;
   keyDelay=0.2;
  }
  if ( input->KeyDown(DX_A) ) blend=additive;
  if ( input->KeyDown(DX_T) ) blend=transparency;
  if ( input->KeyDown(DX_N) ) blend=none;
  if ( input->wheelUp ) { 
   scale.x+=input->KeyDown(DX_LCTRL) ? 1.0 : 0.1;
   scale.y+=input->KeyDown(DX_LCTRL) ? 1.0 : 0.1;
  }
  if ( input->wheelDown ) {
   scale.x-=input->KeyDown(DX_LCTRL) ? 1.0 : 0.1;
   scale.y-=input->KeyDown(DX_LCTRL) ? 1.0 : 0.1;
  }
  if ( input->KeyDown(DX_SPACE) ) scale.Set(1.0,1.0,1.0);
 }
 void Render() {
  glColor3d(0.5,0.0,0.0);
  Text(quadgrid->name.c_str(),10.0,128.0,10.0,8.0,false);
  QuadiNoAlpha(texture,crayons.Pick(alabaster),none,10,10,64,64);

  glReportError( glGetError() );
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture->texture);
  glPushMatrix();
  glTranslatef((float)input->mxi,(float)input->myi,0.0f);
  glScaled(scale.x,scale.y,scale.z);
  quadgrid->vbo.Render();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  glPopAttrib();

  Blending(none);
 }
};

