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

class NormalMapTest : public GLWindow {
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
  normalmappingShader.vbo=&vbo;//&vboQuadVNTTanBi;//&vbo;
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
    normalmappingShader.lightPosition.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmappingShader.lightPosition.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmappingShader.lightPosition.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmappingShader.lightPosition.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmappingShader.lightPosition.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmappingShader.lightPosition.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmappingShader.lightPower+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmappingShader.lightPower-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    normalmappingShader.lightColor.Any();
   }
   if ( input->KeyDown(DX_SPACE) ) { normalmappingShader.lightPosition.Set(0.0f,0.0f,0.0f); }
  }

  if ( input->KeyDown(DX_1) ) normalmappingShader.vbo=&vbo;
  if ( input->KeyDown(DX_2) ) normalmappingShader.vbo=&vbo2;
  if ( input->KeyDown(DX_3) ) {
   normalmappingShader.camera.scale=glm::vec3(display->aspect,1.0f,1.0f);
   normalmappingShader.vbo=&vboQuadVNTTanBi;
  }
  if ( input->KeyDown(DX_4) ) normalmappingShader.vbo=&vbo3;
  if ( input->KeyDown(DX_5) ) normalmappingShader.vbo=&vbo4;

  if ( input->KeyDown(DX_COMMA) ) {
   normalmappingShader.normal=&fboScreen2.texture;
  }
  if ( input->KeyDown(DX_PERIOD) ) normalmappingShader.normal=&nMap->texture;


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
   Blending(none);
   glColor3d(1.0,1.0,1.0);
   normalmappingShader.camera.position.x=0.5f*display->aspect;
   normalmappingShader.camera.position.y=0.5f;
   normalmappingShader.camera.position.z=1.0f+(display->aspect-1.0f)/2.0f;
   normalmappingShader.camera.direction.x=0.0f;
   normalmappingShader.camera.direction.y=0.0f;
   normalmappingShader.camera.direction.z=-1.0f;
   normalmappingShader.camera.right.x=1.0f;
   normalmappingShader.camera.right.y=0.0f;
   normalmappingShader.camera.right.z=0.0f;
   normalmappingShader.camera.up.x=0.0f;
   normalmappingShader.camera.up.y=1.0f;
   normalmappingShader.camera.up.z=0.0f;
   normalmappingShader.camera.UpdateMatrices();
   normalmappingShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
   normalmappingShader.diffuse=&colorMap->texture;
   normalmappingShader.normal=&nMap->texture;
   normalmappingShader.specular=&glossMap->texture;
   normalmappingShader.UpdateUniforms();
   normalmappingShader.Disable();
   glReportError(glGetError());
  } else
   normalmappingShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
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


class NormalMap3LightTest : public GLWindow {
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
  normalmapping3lightShader.vbo=&vbo;//&vboQuadVNTTanBi;//&vbo;
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
  if ( input->KeyDown(DX_1) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmapping3lightShader.lightPosition1.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmapping3lightShader.lightPosition1.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmapping3lightShader.lightPosition1.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmapping3lightShader.lightPosition1.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmapping3lightShader.lightPosition1.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmapping3lightShader.lightPosition1.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmapping3lightShader.lightPower1+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmapping3lightShader.lightPower1-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    normalmapping3lightShader.lightColor1.Any();
   }
   if ( input->KeyDown(DX_SPACE) ) { normalmapping3lightShader.lightPosition1.Set(0.0f,0.0f,0.0f); }
  }

  if ( input->KeyDown(DX_2) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmapping3lightShader.lightPosition2.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmapping3lightShader.lightPosition2.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmapping3lightShader.lightPosition2.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmapping3lightShader.lightPosition2.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmapping3lightShader.lightPosition2.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmapping3lightShader.lightPosition2.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmapping3lightShader.lightPower2+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmapping3lightShader.lightPower2-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    normalmapping3lightShader.lightColor2.Any();
   }
   if ( input->KeyDown(DX_SPACE) ) { normalmapping3lightShader.lightPosition2.Set(0.0f,0.0f,0.0f); }
  }

  if ( input->KeyDown(DX_3) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmapping3lightShader.lightPosition3.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmapping3lightShader.lightPosition3.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmapping3lightShader.lightPosition3.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmapping3lightShader.lightPosition3.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmapping3lightShader.lightPosition3.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmapping3lightShader.lightPosition3.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmapping3lightShader.lightPower3+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmapping3lightShader.lightPower3-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    normalmapping3lightShader.lightColor3.Any();
   }
   if ( input->KeyDown(DX_SPACE) ) { normalmapping3lightShader.lightPosition3.Set(0.0f,0.0f,0.0f); }
  }

  if ( input->KeyDown(DX_5) ) normalmapping3lightShader.vbo=&vbo;
  if ( input->KeyDown(DX_6) ) normalmapping3lightShader.vbo=&vbo2;
  if ( input->KeyDown(DX_7) ) {
   normalmapping3lightShader.camera.scale=glm::vec3(display->aspect,1.0f,1.0f);
   normalmapping3lightShader.vbo=&vboQuadVNTTanBi;
  }
  if ( input->KeyDown(DX_8) ) normalmapping3lightShader.vbo=&vbo3;
  if ( input->KeyDown(DX_9) ) normalmapping3lightShader.vbo=&vbo4;

  if ( input->KeyDown(DX_COMMA) ) {
   normalmapping3lightShader.normal=&fboScreen2.texture;
  }
  if ( input->KeyDown(DX_PERIOD) ) normalmapping3lightShader.normal=&nMap->texture;


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
   Blending(none);
   glColor3d(1.0,1.0,1.0);
   normalmapping3lightShader.camera.position.x=0.5f*display->aspect;
   normalmapping3lightShader.camera.position.y=0.5f;
   normalmapping3lightShader.camera.position.z=1.0f+(display->aspect-1.0f)/2.0f;
   normalmapping3lightShader.camera.direction.x=0.0f;
   normalmapping3lightShader.camera.direction.y=0.0f;
   normalmapping3lightShader.camera.direction.z=-1.0f;
   normalmapping3lightShader.camera.right.x=1.0f;
   normalmapping3lightShader.camera.right.y=0.0f;
   normalmapping3lightShader.camera.right.z=0.0f;
   normalmapping3lightShader.camera.up.x=0.0f;
   normalmapping3lightShader.camera.up.y=1.0f;
   normalmapping3lightShader.camera.up.z=0.0f;
   normalmapping3lightShader.camera.UpdateMatrices();
//   normalmappingShader.camera.MouseChangeOrientation();
   normalmapping3lightShader.diffuse=&colorMap->texture;
   normalmapping3lightShader.normal=&nMap->texture;
   normalmapping3lightShader.specular=&glossMap->texture;
   normalmapping3lightShader.UpdateUniforms();
   normalmapping3lightShader.Disable();
   glReportError(glGetError());
  } else
   normalmapping3lightShader.camera.MouseChangeOrientation(FRAMETIME*0.1f);
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
    normalmappingShader.camera.Upload();
    Coordinator();
    normalmapping3lightShader.vbo->DebugRender(
     glm::vec3(normalmapping3lightShader.lightPosition1.x,
     normalmapping3lightShader.lightPosition1.y,
     normalmapping3lightShader.lightPosition1.z),
     &normalmapping3lightShader.camera
    );
    Blending(additive);
    glColor3d(0.5,0.5,0.5);
    if ( input->KeyDown(DX_X) ) normalmapping3lightShader.vbo->Render();
    else normalmapping3lightShader.vbo->DebugRender();
   glPop();
   normalmapping3lightShader.camera.Debug();
  }
 }
};