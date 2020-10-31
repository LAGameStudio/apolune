#pragma once

#include "ConvolverShader.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"
#include "ParallaxShader.h"
//#include "ProtoShader.h"
#include "LookAtPerspective.h"
#include "ModernVBO.h"
#include "Model.h"
#include "QuadGridVBO.h"
#include "Cube.h"

class NormalMapTest3 : public GLWindow {
public:
 GLImage *colorMap,*baseMap, *glossMap, *nMap;
 ImmediateLighting lights;
 LookAtPerspective lap;
 ImmediateModel cylinder,teapot;
 VBOStaticVNTTanBi vbo,vbo2,vbo3,vbo4;
 Zp<QuadGridVBO> qgvbo;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  colorMap=library.Load("data/materials/default/floor_tile_color_map.png",true,GL_REPEAT,GL_REPEAT,false);
  baseMap=library.Load("data/materials/default/floor_tile_height_map.png",true,GL_REPEAT,GL_REPEAT,false);
  glossMap=library.Load("data/materials/default/floor_tile_gloss_map.png",true,GL_REPEAT,GL_REPEAT,false);
  nMap=library.Load("data/materials/default/floor_tile_normal_map.png",true,GL_REPEAT,GL_REPEAT,false);
  lap.eye.Set(0,0,15);
  lap.Near=0.0001;
  lap.Far=50.0;
  cylinder.LoadOBJ("data/materials/cylinder.obj");
  vbo.fromModel(&cylinder);
  vbo.Create();
  normalmappingShader3.vbo=&vbo;//&vboQuadVNTTanBi;//&vbo;
  OUTPUT("Generating qgvbo on vbo2\n");
  qgvbo=new QuadGridVBO( 10, 10, crayons.Pick(alabaster), &vbo2 );
  teapot.LoadOBJ("data/materials/teapot.obj");
  vbo3.fromModel(&teapot);
  vbo3.Create();
  Cube3d box;
  PolygonList pList;
  box.pList=&pList;
  box.VertexBufferAndImmediate();
  vbo4.fromVBO(&box.vbo16);
  box.pList=null;
  vbo4.Create();
 }
 void Between() {
  // Move the lightsource
  if ( input->KeyDown(DX_LCTRL) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmappingShader3.lightPosition.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmappingShader3.lightPosition.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmappingShader3.lightPosition.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmappingShader3.lightPosition.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmappingShader3.lightPosition.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmappingShader3.lightPosition.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
//    normalmappingShader3.lightPower+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
//    normalmappingShader.lightPower-=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
//    normalmappingShader3.lightColor.Any();
   }
  }

  if ( input->KeyDown(DX_1) ) normalmappingShader3.vbo=&vbo;
  if ( input->KeyDown(DX_2) ) normalmappingShader3.vbo=&vbo2;
  if ( input->KeyDown(DX_3) ) normalmappingShader3.vbo=&vboQuadVNTTanBi;
  if ( input->KeyDown(DX_4) ) normalmappingShader3.vbo=&vbo3;
  if ( input->KeyDown(DX_4) ) normalmappingShader3.vbo=&vbo4;

  if ( input->KeyDown(DX_COMMA) ) {
   normalmappingShader3.normalMap=&fboScreen2.texture;
  }
  if ( input->KeyDown(DX_PERIOD) ) normalmappingShader3.normalMap=&nMap->texture;
 }
 void Render() {
  normalmapShader.pTex0=&baseMap->texture;
  normalmapShader.resolution[0]=(float)baseMap->width;
  normalmapShader.resolution[1]=(float)baseMap->height;
  fboScreen2.Bind();
  normalmapShader.UpdateUniforms();
  fboScreen2.QuadYInverted();
  normalmapShader.Disable();
  fboScreen2.Unbind();

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
   normalmappingShader3.camera.MouseChangeOrientation();
   normalmappingShader3.colorMap=&colorMap->texture;
   normalmappingShader3.normalMap=&nMap->texture;
 //  normalmappingShader3.specular=&glossMap->texture;
   normalmappingShader3.UpdateUniforms();
   normalmappingShader3.Disable();
   glReportError(glGetError());
  } else
   normalmappingShader3.camera.MouseChangeOrientation();
  glPop();


  if ( input->KeyDown(DX_LALT) ) {
   fboScreen2.Render(none,16,16,16+64,16+64);
   QuadiNoAlpha(glossMap,crayons.Pick(alabaster),none,16+64+16,16,64,64);
   QuadiNoAlpha(colorMap,crayons.Pick(alabaster),none,16+64+16+64+16,16,64,64);
   QuadiNoAlpha(nMap,crayons.Pick(alabaster),none,16+64+16+64+16+64+16,16,64,64);
   glPush();
    Blending(none);
    normalmappingShader3.camera.Upload();
    Coordinator();
    normalmappingShader3.vbo->DebugRender(
     glm::vec3(normalmappingShader3.lightPosition.x,
     normalmappingShader3.lightPosition.y,
     normalmappingShader3.lightPosition.z),
     &normalmappingShader.camera
    );
    Blending(additive);
    glColor3d(0.5,0.5,0.5);
    if ( input->KeyDown(DX_X) )
    normalmappingShader3.vbo->Render();
    else normalmappingShader3.vbo->DebugRender();
   glPop();
   normalmappingShader3.camera.Debug();
  }
 }
};