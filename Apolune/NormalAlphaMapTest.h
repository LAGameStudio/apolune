/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "ConvolverShader.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"
#include "ParallaxShader.h"
//#include "ProtoShader.h"
#include "LookAtPerspective.h"
#include "VBO.h"
#include "Model.h"
#include "QuadGridVBO.h"
#include "Cube.h"

class NormalAlphaMapTest : public GLWindow {
public:
 GLImage *colorMap,*baseMap, *glossMap, *nMap;
 ImmediateLighting lights;
 LookAtPerspective lap;
 ImmediateModel cylinder,teapot;//,box;
 VBOStaticVNTTanBi vbo,vbo2,vbo3,vbo4,vbo5;
 Zp<QuadGridVBO> qgvbo;
 Cube3d box;
 PolygonList pList;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  colorMap=library.Load("data/materials/default/floor_tile_color_map.png",tllto_Tiling);
  baseMap=library.Load("data/materials/default/floor_tile_height_map.png",tllto_Tiling);
  glossMap=library.Load("data/materials/default/floor_tile_gloss_map.png",tllto_Tiling);
  nMap=library.Load("data/materials/default/floor_tile_normal_map.png",tllto_Tiling);
  lap.eye.Set(0,0,15);
  lap.Near=0.0001;
  lap.Far=50.0;
  cylinder.LoadOBJ("data/materials/cylinder.obj");
  vbo.fromModel(&cylinder);
  vbo.Create();
  OUTPUT("Generating qgvbo on vbo2\n");
  qgvbo=new QuadGridVBO( 10, 10, crayons.Pick(alabaster), &vbo2 );
  teapot.LoadOBJ("data/materials/teapot.obj");
  vbo3.fromModel(&teapot);
  vbo3.Create();
  box.pList=&pList;
  box.VertexBufferAndImmediate();
  vbo4.fromVBO(&box.vbo16);
  box.pList=null;
  vbo4.Create();
  normalmappingalphaShader.camera.scale=glm::vec3(display->aspect,1.0f,1.0f);
  normalmappingalphaShader.vbo=&vboQuadVNTTanBi;
  normalmappingalphaShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
  /*
  lights.Dark();
  lights.light[0].spot=true;
  lights.light[0].on=true;
  lights.light[0].position.Set(0.0f,0.0f,4.0f,1.0f);
  lap.eye.Set(0.0f,0.0f,-4.0f);
  lap.center.Set(0.0f,0.0f,0.0f);
  lap.Near=0.1;
  lap.Far=100.0;
  lap.halfFOV=45.0;
  */
 }
 void Between() {
  // Move the lightsource
  if ( input->KeyDown(DX_LCTRL) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmappingalphaShader.lightPosition.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmappingalphaShader.lightPosition.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmappingalphaShader.lightPosition.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmappingalphaShader.lightPosition.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmappingalphaShader.lightPosition.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmappingalphaShader.lightPosition.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmappingalphaShader.lightPower+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmappingalphaShader.lightPower-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    normalmappingalphaShader.lightColor.Any();
   }
   if ( input->KeyDown(DX_SPACE) ) { normalmappingalphaShader.lightPosition.Set(0.0f,0.0f,0.0f); }
  }

  if ( input->KeyDown(DX_1) ) normalmappingalphaShader.vbo=&vbo;
  if ( input->KeyDown(DX_2) ) normalmappingalphaShader.vbo=&vbo2;
  if ( input->KeyDown(DX_3) ) {
   normalmappingalphaShader.camera.scale=glm::vec3(display->aspect,1.0f,1.0f);
   normalmappingalphaShader.vbo=&vboQuadVNTTanBi;
  }
  if ( input->KeyDown(DX_4) ) normalmappingalphaShader.vbo=&vbo3;
  if ( input->KeyDown(DX_5) ) normalmappingalphaShader.vbo=&vbo4;

  if ( input->KeyDown(DX_COMMA) ) {
   normalmappingalphaShader.normal=&fboScreen2.texture;
  }
  if ( input->KeyDown(DX_PERIOD) ) normalmappingalphaShader.normal=&nMap->texture;


 }
 void Render() {
  normalmapShader.pTex0=&baseMap->texture;
  normalmapShader.resolution[0]=(float)baseMap->width;
  normalmapShader.resolution[1]=(float)baseMap->height;
  fboScreen2.Bind();
  fboScreen2.QuadYInverted(&normalmapShader);
  fboScreen2.Unbind();

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
   Blending(transparency);
   glColor3d(1.0,1.0,1.0);
   normalmappingalphaShader.camera.position.x=0.5f*display->aspect;
   normalmappingalphaShader.camera.position.y=0.5f;
   normalmappingalphaShader.camera.position.z=1.0f+(display->aspect-1.0f)/2.0f;
   normalmappingalphaShader.camera.direction.x=0.0f;
   normalmappingalphaShader.camera.direction.y=0.0f;
   normalmappingalphaShader.camera.direction.z=-1.0f;
   normalmappingalphaShader.camera.right.x=1.0f;
   normalmappingalphaShader.camera.right.y=0.0f;
   normalmappingalphaShader.camera.right.z=0.0f;
   normalmappingalphaShader.camera.up = glm::cross(
    normalmappingalphaShader.camera.right,
    normalmappingalphaShader.camera.direction
   );
   normalmappingalphaShader.camera.UpdateMatrices();
//   normalmappingalphaShader.camera.MouseChangeOrientation();
   normalmappingalphaShader.diffuse=&colorMap->texture;
   normalmappingalphaShader.normal=&nMap->texture;
   normalmappingalphaShader.specular=&glossMap->texture;
   normalmappingalphaShader.UpdateUniforms();
   normalmappingalphaShader.Disable();
   glReportError(glGetError());
  } else
   normalmappingalphaShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
  glPop();
  fboScreen3.Unbind();

  fboScreen3.RenderUpsideDown(0,0,display->w,display->h);

  if ( input->KeyDown(DX_LALT) ) {
   fboScreen2.Render(none,16,16,16+64,16+64);
   QuadiNoAlpha(glossMap,crayons.Pick(alabaster),none,16+64+16,16,64,64);
   QuadiNoAlpha(colorMap,crayons.Pick(alabaster),none,16+64+16+64+16,16,64,64);
   QuadiNoAlpha(nMap,crayons.Pick(alabaster),none,16+64+16+64+16+64+16,16,64,64);
   glPush();
    Blending(none);
    normalmappingalphaShader.camera.Upload();
    Coordinator();
    normalmappingalphaShader.vbo->DebugRender(
     glm::vec3(normalmappingalphaShader.lightPosition.x,
     normalmappingalphaShader.lightPosition.y,
     normalmappingalphaShader.lightPosition.z),
     &normalmappingalphaShader.camera
    );
    Blending(additive);
    glColor3d(0.5,0.5,0.5);
    if ( input->KeyDown(DX_X) ) normalmappingalphaShader.vbo->Render();
    else normalmappingalphaShader.vbo->DebugRender();
   glPop();
   normalmappingalphaShader.camera.Debug();
  }
 }
};