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
/* IN collaboration with: */
/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    * (c) 2014 PieceMaker Technologies
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        *----------------------------------
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * Paged Lithophane example/test
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * 
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * 
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/
#pragma once

#include "GLWindow.h"
#include "PagedLithophane.h"
#include "LookAtPerspective.h"
#include "ParallaxShader.h"
#include "ConvolverShader.h"
#include "TextureLibrary.h"
#include "Cube.h"

class PagedLithophaneTest : public GLWindow {
public:
 Zdisposable<PagedLithophane> litho;
 // Object
 Cartesiand position;
 Cartesiand rotation;
 // Normal mapping
 GLImage *colorMap,*baseMap, *glossMap, *nMap;
 ImmediateLighting lights;
 LookAtPerspective lap;
 Cube3d box;
 PolygonList pList;
 Zbool wireframe;
 Zfloat keyDelay;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  colorMap=library.Load("data/materials/default/floor_tile_color_map.png",tllto_Tiling);
  baseMap=library.Load("data/materials/default/floor_tile_height_map.png",tllto_Tiling);
  glossMap=library.Load("data/materials/default/floor_tile_gloss_map.png",tllto_Tiling);
  nMap=library.Load("data/materials/default/floor_tile_normal_map.png",tllto_Tiling);
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  litho.Recycle( new PagedLithophane("data/images/heightbrushes/lion.png",1.0,1.0,8,8,100.0,62.0,3.0,0.4) );
  litho->topography->stl.ascii=true;
  litho->topography->stl.Write("data/out/test.stl");
  position.x=-litho->size.x/2.0;
  position.z=-litho->size.y/2.0;
  WorldReset();
  wireframe=true;
 }
 void WorldReset() {
  normalmappingShader.diffuse=&colorMap->texture;
  normalmappingShader.normal=&nMap->texture;
  normalmappingShader.specular=&glossMap->texture;
  normalmappingShader.lightColor.Pick(alabaster);
  normalmappingShader.lightPosition.y=100.0f;
  normalmappingShader.lightPower=100000.0f;
  normalmappingShader.camera.position.x=0;//(float) position.x;
  normalmappingShader.camera.position.y=-100.0;
  normalmappingShader.camera.position.z=0;//(float) position.z;
  normalmappingShader.camera.direction.x=0.0f;
  normalmappingShader.camera.direction.y=0.0f;
  normalmappingShader.camera.direction.z=0.0f;
  normalmappingShader.camera.right.x=1.0f;
  normalmappingShader.camera.right.y=0.0f;
  normalmappingShader.camera.right.z=0.0f;
  normalmappingShader.camera.up.x=0.0f;
  normalmappingShader.camera.up.y=1.0f;
  normalmappingShader.camera.up.z=0.0f;
  normalmappingShader.camera.Far=1000.0f;
  normalmappingShader.camera.Near=0.1f;
  normalmappingShader.camera.UpdateMatrices();
 }
 void Between() {  
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  if ( keyDelay > 0.0f ) keyDelay-=(float)FRAMETIME;
  else {
   if ( input->KeyDown(DX_W) ) { wireframe=!wireframe; keyDelay=0.25f; }
   if ( wireframe ) {
    if ( input->KeyDown(NumpadPlus) ) { lap.eye.Set(0.0,0.0,lap.eye.z/2); keyDelay=0.25f; }
    if ( input->KeyDown(NumpadMinus) ) { lap.eye.Set(0.0,0.0,lap.eye.z*2); keyDelay=0.25f; }
   }
   // Move the lightsource
   if ( input->KeyDown(DX_LCTRL) ) {
    if ( input->KeyDown(DX_J) ) { keyDelay=0.125f; normalmappingShader.lightPosition.x-=FRAMETIME; }
    if ( input->KeyDown(DX_L) ) { keyDelay=0.125f; normalmappingShader.lightPosition.x+=FRAMETIME; }
    if ( input->KeyDown(DX_K) ) { keyDelay=0.125f; normalmappingShader.lightPosition.y-=FRAMETIME; }
    if ( input->KeyDown(DX_I) ) { keyDelay=0.125f; normalmappingShader.lightPosition.y+=FRAMETIME; }
    if ( input->KeyDown(DX_O) ) { keyDelay=0.125f; normalmappingShader.lightPosition.z-=FRAMETIME; }
    if ( input->KeyDown(DX_P) ) { keyDelay=0.125f; normalmappingShader.lightPosition.z+=FRAMETIME; }
    if ( input->KeyDown(DX_U) ) { keyDelay=0.125f; normalmappingShader.lightPower+=FRAMETIME; }
    if ( input->KeyDown(DX_H) ) { keyDelay=0.125f; normalmappingShader.lightPower-=FRAMETIME; }
    if ( input->KeyDown(DX_Y) ) { normalmappingShader.lightColor.Any(); }
    if ( input->KeyDown(DX_SPACE) ) { normalmappingShader.lightPosition.Set(0.0f,0.0f,0.0f); }
   }
 
   if ( input->KeyDown(DX_COMMA) ) { normalmappingShader.normal=&fboScreen2.texture; }
   if ( input->KeyDown(DX_PERIOD) ) { normalmappingShader.normal=&nMap->texture; }
  }
  normalmappingShader.camera.OrbitAcclimate( 200.0f, (input->mx/display->wf - 0.5f)*2.0f, 0.0f, true, (input->my/display->hf - 0.5f)*2.0f );
 }
 void Render() {
  if ( wireframe ) RenderWireframe();
  else RenderNormalMap();
 }
 void RenderWireframe() {
  Blending(none);
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();///
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0);
   glDepthFunc(GL_LEQUAL);
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
   litho->RenderWireframe();
//   glDisable(GL_DEPTH_TEST);
  glPopMatrix();
  glPushMatrix();///
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glLineWidth(4.2f);
   Coordinator();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) (litho->size.x*0.7), 0.0f,1.0f);
    Text("RIGHT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) -(litho->size.x*0.7), 0.0f,1.0f);
    Text("LEFT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) -(litho->size.y*0.7), 1.0f);
    Text("BACK",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) (litho->size.y*0.7), 1.0);
    Text("FRONT",0,0,2,3,true,0);
   glPopMatrix();
   glLineWidth(1.2f);
   glPushMatrix();//
    glTranslatef((GLfloat) -(litho->size.x/2)-10.0f, (GLfloat) -(litho->size.y*0.5), 0.0f);
    GLfloat cm=0.0f;
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("%-2d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > litho->size.y ) break;
    }
   glPopMatrix();//
   glPushMatrix();//
    glTranslatef( (GLfloat) -(litho->size.x*0.5), (GLfloat) -(litho->size.y*0.5), 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("____ %-2d cm",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > litho->size.x ) break;
    }
   glPopMatrix();//
  glPopMatrix();///
  glPop();
  Text(
   FORMAT("Pages: %d by %d\nSamples: %d (%dx%d)",
    litho->topography->pages.w,
    litho->topography->pages.h,
    litho->topography->samples.x*litho->topography->samples.y,
    litho->topography->samples.x,
    litho->topography->samples.y
   ),crayons.Pick(green255),transparency,32,32,10,14,2,1,false);
 }
 void RenderNormalMap() {
  fboScreen3.Bind();
  glPush();
 	// Enable depth test
 	glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT); // <- This had to be done for it to work.
 	// Accept fragment if it closer to the camera than the former one
 	glDepthFunc(GL_LESS);//QUAL); //ESS); 
//  glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);

  if ( !input->KeyDown(DX_LALT) ) {
   Blending(none);
   glColor3d(1.0,1.0,1.0);
   litho->Render();
   Coordinator();
  } else
   normalmappingShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
  glPop();
  fboScreen3.Unbind();

  fboScreen3.Render(0,0,display->w,display->h);

  if ( input->KeyDown(DX_LALT) ) {
   fboScreen2.Render(none,16,16,16+64,16+64);
   QuadiNoAlpha(glossMap,crayons.Pick(alabaster),none,16+64+16,16,64,64);
   QuadiNoAlpha(colorMap,crayons.Pick(alabaster),none,16+64+16+64+16,16,64,64);
   QuadiNoAlpha(nMap,crayons.Pick(alabaster),none,16+64+16+64+16+64+16,16,64,64);
   glPush();
    Blending(none);
    normalmappingShader.camera.Upload();
    Coordinator();
    normalmappingShader.vbo->DebugRender(
     glm::vec3(normalmappingShader.lightPosition.x,
     normalmappingShader.lightPosition.y,
     normalmappingShader.lightPosition.z),
     &normalmappingShader.camera
    );
    Blending(additive);
    glColor3d(0.5,0.5,0.5);
    if ( input->KeyDown(DX_X) ) normalmappingShader.vbo->Render();
    else normalmappingShader.vbo->DebugRender();
   glPop();
   normalmappingShader.camera.Debug();
  }
 }
};