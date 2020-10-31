#pragma once
#include "ConvolverShader.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"
#include "ParallaxShader.h"
#include "Model.h"
#include "Cube.h"

class NormalMapTest2 : public GLWindow {
public:
 GLImage *colorMap,*baseMap, *glossMap, *nMap;
 VBOStaticVNT vbo1,vbo2,vbo3;
 ImmediateModel cylinder,teapot;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  cylinder.LoadOBJ("data/materials/cylinder.obj");
  vbo1.FromOBJ(&cylinder);
  vbo1.Create();
  teapot.LoadOBJ("data/materials/teapot.obj");
  vbo2.FromOBJ(&teapot);
  vbo2.Create();
  Cube3d box;
  PolygonList pList;
  box.pList=&pList;
  box.VertexBufferAndImmediate();
  vbo3.fromVBO(&box.vbo16);
  box.pList=null;
  vbo3.Create();
  normalmappingShader2.vbo=&vboQuadVNT;
  colorMap=library.Load("data/materials/default/floor_tile_color_map.png",true,GL_REPEAT,GL_REPEAT,false);
  baseMap=library.Load("data/materials/default/floor_tile_height_map.png",true,GL_REPEAT,GL_REPEAT,false);
  glossMap=library.Load("data/materials/default/floor_tile_gloss_map.png",true,GL_REPEAT,GL_REPEAT,false);
  nMap=library.Load("data/materials/default/floor_tile_normal_map.png",true,GL_REPEAT,GL_REPEAT,false);
 }
 void Between() {
  if ( input->KeyDown(DX_1) ) {
   normalmappingShader2.vbo=&vbo1;
  }
  if ( input->KeyDown(DX_2) ) {
   normalmappingShader2.vbo=&vbo2;
  }
  if ( input->KeyDown(DX_3) ) {
   normalmappingShader2.vbo=&vboQuadVNT;
  }
  if ( input->KeyDown(DX_4) ) {
   normalmappingShader2.vbo=&vbo3;
  }
  if ( input->KeyDown(DX_0) ) {
   normalmappingShader2.ambient.Any();
  }
  if ( input->KeyDown(DX_9) ) {
   normalmappingShader2.diffuse.Any();
  }
  if ( input->KeyDown(DX_8) ) {
   normalmappingShader2.specular.Any();
  }
  if ( input->KeyDown(DX_LCTRL) ) {
   if ( input->KeyDown(DX_J) ) {
    normalmappingShader2.lightPosition.x-=FRAMETIME;
   }
   if ( input->KeyDown(DX_L) ) {
    normalmappingShader2.lightPosition.x+=FRAMETIME;
   }
   if ( input->KeyDown(DX_I) ) {
    normalmappingShader2.lightPosition.y-=FRAMETIME;
   }
   if ( input->KeyDown(DX_K) ) {
    normalmappingShader2.lightPosition.y+=FRAMETIME;
   }
   if ( input->KeyDown(DX_O) ) {
    normalmappingShader2.lightPosition.z-=FRAMETIME;
   }
   if ( input->KeyDown(DX_P) ) {
    normalmappingShader2.lightPosition.z+=FRAMETIME;
   }
   if ( input->KeyDown(DX_U) ) {
    normalmappingShader2.perturbance+=FRAMETIME;
   }
   if ( input->KeyDown(DX_H) ) {
    normalmappingShader2.perturbance-=FRAMETIME;
   }
  }
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
   normalmappingShader2.camera.MouseChangeOrientation();
   normalmappingShader2.colorMap=&colorMap->texture;
   normalmappingShader2.normalMap=&nMap->texture;
   normalmappingShader2.UpdateUniforms();
   normalmappingShader2.Disable();
   glReportError(glGetError());
  } else
   normalmappingShader2.camera.MouseChangeOrientation();

  glPop();

  if ( input->KeyDown(DX_COMMA) ) {
   normalmappingShader2.normalMap=&fboScreen2.texture;
  }
  if ( input->KeyDown(DX_PERIOD) ) normalmappingShader2.normalMap=&nMap->texture;

  if ( input->KeyDown(DX_LALT) ) {
   fboScreen2.Render(none,16,16,16+64,16+64);
   QuadiNoAlpha(glossMap,crayons.Pick(alabaster),none,16+64+16,16,64,64);
   QuadiNoAlpha(colorMap,crayons.Pick(alabaster),none,16+64+16+64+16,16,64,64);
   QuadiNoAlpha(nMap,crayons.Pick(alabaster),none,16+64+16+64+16+64+16,16,64,64);
   glPush();
    Blending(none);
    normalmappingShader2.camera.Upload();
    Coordinator();
    normalmappingShader2.vbo->DebugRender(
     glm::vec3(normalmappingShader2.lightPosition.x,
     normalmappingShader2.lightPosition.y,
     normalmappingShader2.lightPosition.z),
     &normalmappingShader2.camera
    );
    Blending(additive);
    glColor3d(0.5,0.5,0.5);
    if ( input->KeyDown(DX_X) ) normalmappingShader2.vbo->Render();
    else normalmappingShader2.vbo->DebugRender();
   glPop();
   normalmappingShader2.camera.Debug();
  }
 }
};