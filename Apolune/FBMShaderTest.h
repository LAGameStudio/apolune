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
#include "FBMShader.h"
#include "FBO.h"
#include "StencilShader.h"
#include "NightvisionShader.h"
#include "TextureLibrary.h"
#include "BendShader.h"
#include "ConvolverShader.h"
#include "ForeverWrappingQuadLayers.h"
#include "BlendShader.h"
#include "UglyFont.h"
#include "Globals.h"
#include "ColorNetwork.h"

class FBMShadedLayer : public ListItem {
public:
 Crayon background;
 Crayon tint;
 Blends blend;
 Zp<FBOColor> target;
 Zfloat resolution[2],position[2];
 Zfloat pattern1[4];
 Zfloat pattern2[4];
 Zfloat pattern3[4];
 Zfloat pattern4[4];
 Zfloat fade[3];
 Zfloat time,factor,colorAmp;
 FBMShadedLayer() : ListItem() {
  Reset();
 }
 void Reset() {
  blend=none;
  tint.Pick(alabaster);
  target=&fboScreen;
  position[0]=0.0f;
  position[1]=0.0f;
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  time=0.0f;
  factor=41.0f;
  colorAmp=3.5f;
  pattern1[0]=1.7f;
  pattern1[1]=9.2f;
  pattern1[2]=8.3f;
  pattern1[3]=2.8f;
  pattern2[0]=0.0f;
  pattern2[1]=0.0f;
  pattern2[2]=5.2f;
  pattern2[3]=1.3f;
  pattern3[0]=2.5f;
  pattern3[1]=0.4f;
  pattern3[2]=2.3f;
  pattern3[3]=0.4f;
  pattern4[0]=2.3f;
  pattern4[1]=34.0f;
  pattern4[2]=4.0f;
  pattern4[3]=289.0f; // mod value
  fade[0]=6.0f;
  fade[1]=15.0f;
  fade[2]=10.0f;
 }
 void Apply() {  
  fbmShader.position[0]=position[0];
  fbmShader.position[1]=position[1];
  fbmShader.resolution[0]=resolution[0];
  fbmShader.resolution[1]=resolution[1];
  fbmShader.time=time;
  fbmShader.factor=factor;
  fbmShader.colorAmp=colorAmp;
  fbmShader.pattern1[0]=pattern1[0];
  fbmShader.pattern1[1]=pattern1[1];
  fbmShader.pattern1[2]=pattern1[2];
  fbmShader.pattern1[3]=pattern1[3];
  fbmShader.pattern2[0]=pattern2[0];
  fbmShader.pattern2[1]=pattern2[1];
  fbmShader.pattern2[2]=pattern2[2];
  fbmShader.pattern2[3]=pattern2[3];
  fbmShader.pattern3[0]=pattern3[0];
  fbmShader.pattern3[1]=pattern3[1];
  fbmShader.pattern3[2]=pattern3[2];
  fbmShader.pattern3[3]=pattern3[3];
  fbmShader.pattern4[0]=pattern4[0];
  fbmShader.pattern4[1]=pattern4[1];
  fbmShader.pattern4[2]=pattern4[2];
  fbmShader.pattern4[3]=pattern4[3];
  fbmShader.fade[0]=fade[0];
  fbmShader.fade[1]=fade[1];
  fbmShader.fade[2]=fade[2];
 }
 virtual void Between() {}
 virtual void Render() {
  target->background.fromCrayon(background);
  target->Bind();
  Apply();
  Blending(blend);
  fbmShader.Tint(&tint);
  target->QuadYInverted(&fbmShader);
  target->Unbind();
 }
};

class FBMShadedLayers : public LinkedList {
public:
 void Between() {
  FOREACH(FBMShadedLayer,L) L->Between();
 }
 void Render() {
  FOREACH(FBMShadedLayer,L) L->Render();
 }
};

class FBMShaderTest : public GLWindow {
public:
 ColorGear cloudColors;
 Crayon snowColor;
 Zdouble accumulation;
 FBMShadedLayers layers;
 Zp<GLImage> backgroundImage,noiseTex,white;
 Zbool advanceTime;
 Zdouble keyDelay;
 ForeverWrappingQuadLayers fwql,fwql2;
 void OnLoad() {
  Init();
  Fullscreen();
  background.Pick(green255);
  backgroundImage=library.Load("data/images/titles/space-nebula.jpg");
  noiseTex=library.Load("data/images/noise2.png",tllto_Tiling);
  white=library.Load("data/images/elements/white.png",tllto_Tiling);
  uniform(true);
  cloudColors.Add(crayons.Pick(red255));
  cloudColors.Add(crayons.Pick(green255));
  cloudColors.Add(crayons.Pick(blue255));
  snowColor.Pick(alabaster);
  accumulation=0.8;
  for ( int i=0; i<20; i++ ) cloudColors.Add(crayons.Any());
  SetFQ();
//  FBMShadedLayer *layer=new FBMShadedLayer;
//  layer->target=&fboScreen4;
//  layer->tint.Any();
 }
 double AccumMod( double acc, double fadeIn=0.1 ) {
  if ( acc >= (1.0-fadeIn) ) {
   double t= 1.0-sin(((acc-(1.0-fadeIn))/fadeIn)*HALF_PI);
   return t;
  }
  return 1.0;
 }
 void SetFQ() {
  string a=string(geo_nontech_materials.AnyFilename());
  fwql.Clear();
  fwql.screen.Set(0.0,0.0,display->wd,display->hd);
  fwql.Add(library.Load(geo_nontech_materials.AnyFilename(),tllto_Tiling),additive,crayons.Any());
  fwql.Add(library.Load(a.c_str(),tllto_Tiling),additive,crayons.Pick(orange));
  fwql.MoveBy(0.0,0.0);
  fwql2.Clear();
  fwql2.screen.Set(0.0,0.0,display->wd,display->hd);
  fwql2.Add(library.Load(geo_nontech_materials.AnyFilename(),tllto_Tiling),none,crayons.Any());
  //fwql2.Add(library.Load("data/images/textures/Camouflage.png",tllto_Tiling),multiply,crayons.Any());
  fwql2.Add(library.Load(a.c_str(),tllto_Tiling),additive,crayons.Any());
  fwql2.Add(library.Load(geo_nontech_materials.AnyFilename(),tllto_Tiling),additive,crayons.Any());
  fwql2.MoveBy(0.0,0.0);
 }
 void Between() {
  if ( advanceTime ) fbmShader.time+=FRAMETIME*0.01f;
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  if ( keyDelay <= 0.0 && input->KeyDown(DX_SPACE) ) {
   advanceTime=!advanceTime;
   keyDelay=0.2;
  } else
  if ( keyDelay <= 0.0 && input->KeyDown(DX_RETURN) ) {
   SetFQ();
   keyDelay=0.2;
  } else if ( keyDelay <= 0.0 && input->KeyDown(DX_BACKSPACE) ) {
   Init();
   Randomize();
   keyDelay=0.2;
  } else if ( input->KeyDown(DX_NUMPADPLUS) ) {
   seaLevel+=iratiof(1,255);
  } else if ( input->KeyDown(DX_NUMPADMINUS) ) {
   seaLevel-=iratiof(1,255);
  } else if ( input->KeyDown(DX_PGUP) ) {
   accumulation+=iratiof(1,255);
  } else if ( input->KeyDown(DX_PGDN) ) {
   accumulation-=iratiof(1,255);
  } else
  if ( input->KeyDown(DX_LSHIFT) ) {
   if ( input->left ) {
    fbmShader.pattern1[0]=(float)((double)input->mxi/100.0);
    fbmShader.pattern1[1]=(float)((double)input->myi/100.0);
   }
   if ( input->right ) {
    fbmShader.pattern1[2]=(float)((double)input->mxi/100.0);
    fbmShader.pattern1[3]=(float)((double)input->myi/100.0);
   }
  } else if ( input->KeyDown(DX_RSHIFT) ) {
   if ( input->left ) {
    fbmShader.pattern2[0]=(float)((double)input->mxi/100.0);
    fbmShader.pattern2[1]=(float)((double)input->myi/100.0);
   }
   if ( input->right ) {
    fbmShader.pattern2[2]=(float)((double)input->mxi/100.0);
    fbmShader.pattern2[3]=(float)((double)input->myi/100.0);
   }
  } else if ( input->KeyDown(DX_LCTRL) ) {
   if ( input->left ) {
    fbmShader.fade[0]=(float)((double)input->mxi/100.0);
    fbmShader.fade[1]=(float)((double)input->myi/100.0);
   }
   if ( input->right ) {
    fbmShader.fade[2]=(float)((double)input->mxi/100.0);
    fbmShader.pattern4[3]=(float)((double)input->myi/100.0);
   }
  } else if ( input->KeyDown(DX_RCTRL) ) {
   if ( input->left ) {
    fbmShader.resolution[0]=(float)(input->mxi*100.0);
    fbmShader.resolution[1]=(float)(input->myi*100.0);
   }
   if ( input->right ) {
    fbmShader.position[0]=(float)(input->mxi);
    fbmShader.position[1]=(float)(input->myi);
   }
  } else if ( input->KeyDown(DX_LALT) ) {
   if ( input->left ) {
    fbmShader.pattern3[0]=(float)((double)input->mxi/100.0);
    fbmShader.pattern3[1]=(float)((double)input->myi/100.0);
   }
   if ( input->right ) {
    fbmShader.pattern3[2]=(float)((double)input->mxi/100.0);
    fbmShader.pattern3[3]=(float)((double)input->myi/100.0);
   }
  } else if ( input->KeyDown(DX_RALT) ) {
   if ( input->left ) {
    fbmShader.pattern4[0]=(float)((double)input->mxi/100.0);
    fbmShader.pattern4[1]=(float)((double)input->myi/100.0);
   }
   if ( input->right ) {
    fbmShader.pattern4[2]=(float)((double)input->mxi/100.0);
    fbmShader.pattern4[3]=(float)((double)input->myi/100.0);
   }
  } else {
   if ( input->left ) {
    fbmShader.colorAmp=(float)((double)input->mxi/100.0);
   }
   if ( input->right ) {
    fbmShader.factor=(float)((double)input->mxi/10.0);
   }
  }
  if ( input->KeyDown(DX_Q) ) {
   if ( input->KeyDown(DX_LEFT)  ) { fbmShader.position[0]+=10.0f; fwql.MoveBy(10.0,0.0);  fwql2.MoveBy(10.0,0.0);  }
   if ( input->KeyDown(DX_RIGHT) ) { fbmShader.position[0]-=10.0f; fwql.MoveBy(-10.0,0.0); fwql2.MoveBy(-10.0,0.0); }
   if ( input->KeyDown(DX_UP)    ) { fbmShader.position[1]+=10.0f; fwql.MoveBy(0.0,-10.0); fwql2.MoveBy(0.0,-10.0); }
   if ( input->KeyDown(DX_DOWN)  ) { fbmShader.position[1]-=10.0f; fwql.MoveBy(0.0,10.0);  fwql2.MoveBy(0.0,10.0);  }
  } else {
   if ( input->KeyDown(DX_LEFT)  ) { fbmShader.position[0]+=1.0f; fwql.MoveBy(1.0,0.0);  fwql2.MoveBy(1.0,0.0);  }
   if ( input->KeyDown(DX_RIGHT) ) { fbmShader.position[0]-=1.0f; fwql.MoveBy(-1.0,0.0); fwql2.MoveBy(-1.0,0.0); }
   if ( input->KeyDown(DX_UP)    ) { fbmShader.position[1]+=1.0f; fwql.MoveBy(0.0,-1.0); fwql2.MoveBy(0.0,-1.0); }
   if ( input->KeyDown(DX_DOWN)  ) { fbmShader.position[1]-=1.0f; fwql.MoveBy(0.0,1.0);  fwql2.MoveBy(0.0,1.0);  }
  }
  cloudColor.fromCrayon(cloudColors.Request(fbmShader.position[0],fbmShader.position[1]));
 }
 Zdouble bgchangeDelay;
 Zfloat waterTime;
 Zfloat erosionTime;
 Zfloat windTime;
 Zfloat mountainHeight,mountainFactor;
 Zfloat waterShimmer;
 Zfloat shimmerThickness,shimmerFactor;
 Zfloat cloudThickness,cloudFactor;
 Crayon cloudColor,cloudShadowThickness;
 Crayon waterColor,waterReflection;
 Zfloat windSpeed,shimmerSpeed;
 Zint cloudHeight;
 Zfloat seaLevel,tideExtremes,tideSpeed,tide,tidalMovement;
 Zfloat windDirection;
 void Init() {
  fbmShader.position[0]=1000000.0; // We set this far into the positive,
  fbmShader.position[1]=1000000.0; // to avoid odd behavior in other quadrants.
  mountainHeight=2.0f;
  mountainFactor=7.2f;
  shimmerFactor=cloudFactor=3.0f;
  shimmerThickness=3.5f;
  cloudThickness=4.0f;
  windSpeed=0.01f;
  shimmerSpeed=0.005f;
  windDirection=1.0f;
  seaLevel=iratiof(100,255);
  tideExtremes=iratiof(5,255);
  tideSpeed=PIf/100.0f;
  cloudHeight=128;
  cloudColor.Float(1.0f,1.0f,1.0f,1.0f);
  cloudShadowThickness.Float(0.0f,0.0f,0.0f,0.25f);
  waterColor.Pick(blue);
  waterReflection.Pick(cyan);
 }
 void Randomize() {
  noiseTex=geo_water_noise.Any();
  mountainHeight=2.0f*(float)uniform();
  mountainFactor=7.2f*(float)uniform();
  shimmerFactor=cloudFactor=3.0f*(float)uniform();
  shimmerThickness=3.5f*(float)uniform();
  cloudThickness=4.0f*(float)(uniform()+uniform());
  windSpeed=0.01f*(float)uniform();
  shimmerSpeed=0.005f*(float)uniform();
  windDirection=1.0f;
  seaLevel=iratiof(100,255)*(float)uniform();
  tideExtremes=iratiof(5,255)*(float)uniform();
  tideSpeed=PIf/100.0f*(float)uniform();
  cloudHeight=128;
  cloudColor.Any();
  waterColor.Any();
  waterReflection.Any();
 }
 void Render() {
  tidalMovement+=tideSpeed;
  tide=sin(tidalMovement)*tideExtremes;
  waterTime+=FRAMETIME*shimmerSpeed*windDirection;
  windTime+=FRAMETIME*windSpeed*windDirection;
  bgchangeDelay-=FRAMETIME;
  if ( bgchangeDelay < 0.0 ) {
   background.Any();
   bgchangeDelay=2.0;
  }

  crayons.Pick(alabaster).gl();

  // Generate heightmap FBM and apply the heightmap colormap as a secondary influence
  fbmShader.time=erosionTime;
  fbmShader.factor=mountainFactor;
  fbmShader.colorAmp=mountainHeight;
  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();  
  Blending(none);
  fwql2.RenderUpsideDown();
  Blending(additive);
  fboScreen1.QuadYInverted(&fbmShader);
  fboScreen1.Unbind();

  // Create the "water surface" heightmap using noise and emboss
  ShadedEmboss(&noiseTex->texture,&fboScreen2);
  
  // Create the "water surface" colormap
  fbmShader.time=waterTime;
  fbmShader.factor=shimmerFactor;
  fbmShader.colorAmp=shimmerThickness;
  ShadedFBM(&fbo512);
  fboScreen8.background.fromCrayon(waterColor);
  ShadedBend(&fbo512.texture,&fboScreen2.texture,
   &fboScreen8);

  fbmShader.time=windTime;
  fbmShader.factor=cloudFactor;
  fbmShader.colorAmp=cloudThickness;
  ShadedFBM(&fbo512b);

  // Cut the heightmap to create a height transparency map
  Crayon sea;
  sea.Float(seaLevel+tide,seaLevel+tide,seaLevel+tide,1.0f);
  ShadedColorBelow(sea,crayons.Pick(clear),&fboScreen1.texture,
   &fboScreen4);

  // Cut the heightmap a second time to create the snow accumulator
  Crayon snow;
  snow.Float((float)accumulation,(float)accumulation,(float)accumulation,1.0f);
  ShadedColorBelow(snow,crayons.Pick(clear),&fboScreen1.texture,
   &fboScreen12);
  ShadedColorAbove(snow,snowColor,&fboScreen12.texture,&fboScreen);
//  ShadedSigma5Blur(&fboScreen11.texture,&fboScreen10,&fboScreen);

  // Apply the cut-out heightmap over the water surface heightmap
  fboScreen5.background.Pick(black);
  fboScreen5.Bind();
  glEnable(GL_TEXTURE_2D);
  Blending(additive);
  sea.gl();
  glBindTexture(GL_TEXTURE_2D,fboScreen2.texture);
  fboScreen5.QuadYInverted();
  crayons.Pick(alabaster).gl();
  Blending(transparency);
  glBindTexture(GL_TEXTURE_2D,fboScreen4.texture);
  fboScreen5.QuadYInverted();
  glDisable(GL_TEXTURE_2D);
  fboScreen5.Unbind();

  // 

//  fboScreen2.blend=transparency;
//  fboScreen2.RenderUpsideDown(0,0,display->w,display->h);

  // Generate the surface color layer
  fboScreen6.Bind();
  Blending(none);
  fwql.RenderUpsideDown();
  fboScreen6.Unbind();

  // Bend the color layer by the composite height layer to create the surface colormap
  ShadedBend(&fboScreen6.texture,&fboScreen5.texture,&fboScreen7);

  // Cut the color layer by the composite height layer to create the rivers 
  ShadedBlendAlpha(&fboScreen4.texture,&fboScreen7.texture,&fboScreen11);

  // Generate an embossed version of the original height layer as the shadow basemap
  ShadedEmboss(&fboScreen1.texture,&fboScreen9);

  // Blur the color map to soften river edges
  ShadedSigma9Blur(&fboScreen11.texture,&fboScreen12,&fboScreen3);

  // Take the cut water color layer and apply it over the water colormap
//  fboScreen10.background.fromCrayon(waterColor);
  fboScreen10.background.fromCrayon(waterColor);
  fboScreen10.Bind();
  glEnable(GL_TEXTURE_2D);
  waterReflection.gl();
  Blending(additive);
  glBindTexture(GL_TEXTURE_2D,fboScreen8.texture);
  fboScreen10.QuadYInverted();
  crayons.Pick(alabaster).gl();
  // surface
  Blending(multiply);
  glBindTexture(GL_TEXTURE_2D,fboScreen9.texture);
  // Draw land
  Blending(transparency);
  glBindTexture(GL_TEXTURE_2D,fboScreen3.texture); // blurred
  fboScreen10.QuadYInverted();
  Blending(multiply);
  glBindTexture(GL_TEXTURE_2D,fboScreen9.texture);
  fboScreen10.QuadYInverted();
  glColor3d(0.25,0.25,0.25); // brightener
  Blending(additive);
  glBindTexture(GL_TEXTURE_2D,fboScreen11.texture);
  fboScreen10.QuadYInverted();
  Blending(transparency);
  glColor4d(1.0,1.0,1.0,AccumMod(accumulation)); // snow depth
  glBindTexture(GL_TEXTURE_2D,fboScreen.texture);
  fboScreen10.QuadYInverted();
  glDisable(GL_TEXTURE_2D);
  fboScreen10.Unbind();

  fboScreen10.blend=none;
  fboScreen10.RenderUpsideDown(0,0,display->w,display->h);  

//  crayons.Pick(blue).gl();
//  fboScreen8.blend=transparency;
//  fboScreen8.RenderUpsideDown(0,0,display->w,display->h); 

  // Draw cloud shadows
  fbo512b.tint.fromCrayon(cloudShadowThickness); //Float(0.0f,0.0f,0.0f,0.25f);
  fbo512b.blend=transparency;
  fbo512b.RenderUpsideDown(-cloudHeight,0,display->w,display->h+cloudHeight);

  // Draw clouds
  fbo512b.tint.fromCrayon(cloudColor);//Pick(alabaster); //Float(0.5f,0.5f,0.0f,0.25f);
  fbo512b.blend=transparency;
  fbo512b.RenderUpsideDown(0,0,display->w,display->h);

  // Display all of the layers for debugging
  
  Area(crayons.Pick(green255),10,10, 10+64,10+64 ); crayons.Pick(alabaster).gl(); Text("0", 90,8+10, 8,10,true); fboScreen.blend  =transparency;   fboScreen.RenderUpsideDown  (10,10, 10+64,10+64 );
  Area(crayons.Pick(green255),10,74, 10+64,74+64 ); crayons.Pick(alabaster).gl(); Text("1", 90,8+74, 8,10,true); fboScreen1.blend =transparency;  fboScreen1.RenderUpsideDown (10,74, 10+64,74+64 );
  Area(crayons.Pick(green255),10,138,10+64,138+64); crayons.Pick(alabaster).gl(); Text("2", 90,8+138,8,10,true); fboScreen2.blend =transparency;  fboScreen2.RenderUpsideDown (10,138,10+64,138+64);
  Area(crayons.Pick(green255),10,202,10+64,202+64); crayons.Pick(alabaster).gl(); Text("3", 90,8+202,8,10,true); fboScreen3.blend =transparency;  fboScreen3.RenderUpsideDown (10,202,10+64,202+64);
  Area(crayons.Pick(green255),10,268,10+64,268+64); crayons.Pick(alabaster).gl(); Text("4", 90,8+268,8,10,true); fboScreen4.blend =transparency;  fboScreen4.RenderUpsideDown (10,268,10+64,268+64);
  Area(crayons.Pick(green255),10,332,10+64,332+64); crayons.Pick(alabaster).gl(); Text("5", 90,8+332,8,10,true); fboScreen5.blend =transparency;  fboScreen5.RenderUpsideDown (10,332,10+64,332+64);
  Area(crayons.Pick(green255),10,400,10+64,400+64); crayons.Pick(alabaster).gl(); Text("6", 90,8+400,8,10,true); fboScreen6.blend =transparency;  fboScreen6.RenderUpsideDown (10,400,10+64,400+64);
  Area(crayons.Pick(green255),10,464,10+64,464+64); crayons.Pick(alabaster).gl(); Text("7", 90,8+464,8,10,true); fboScreen7.blend =transparency;  fboScreen7.RenderUpsideDown (10,464,10+64,464+64);
  Area(crayons.Pick(green255),10,536,10+64,536+64); crayons.Pick(alabaster).gl(); Text("8", 90,8+536,8,10,true); fboScreen8.blend =transparency;  fboScreen8.RenderUpsideDown (10,536,10+64,536+64);
  Area(crayons.Pick(green255),10,600,10+64,600+64); crayons.Pick(alabaster).gl(); Text("9", 90,8+600,8,10,true); fboScreen9.blend =transparency;  fboScreen9.RenderUpsideDown (10,600,10+64,600+64);
  Area(crayons.Pick(green255),10,666,10+64,666+64); crayons.Pick(alabaster).gl(); Text("10",90,8+666,8,10,true); fboScreen10.blend=transparency; fboScreen10.RenderUpsideDown(10,666,10+64,666+64);
  Area(crayons.Pick(green255),10,730,10+64,730+64); crayons.Pick(alabaster).gl(); Text("11",90,8+730,8,10,true); fboScreen11.blend=transparency; fboScreen11.RenderUpsideDown(10,730,10+64,730+64);

  if ( WITHIN(input->mxi,input->myi,10,10, 10+64,10+64 ) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen.blend  =transparency; fboScreen.RenderUpsideDown  (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,74, 10+64,74+64 ) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen1.blend =transparency; fboScreen1.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,138,10+64,138+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen2.blend =transparency; fboScreen2.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,202,10+64,202+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen3.blend =transparency; fboScreen3.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,268,10+64,268+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen4.blend =transparency; fboScreen4.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,332,10+64,332+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen5.blend =transparency; fboScreen5.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,400,10+64,400+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen6.blend =transparency; fboScreen6.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,464,10+64,464+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen7.blend =transparency; fboScreen7.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,536,10+64,536+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen8.blend =transparency; fboScreen8.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,600,10+64,600+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen9.blend =transparency; fboScreen9.RenderUpsideDown (display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,666,10+64,666+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen10.blend=transparency; fboScreen10.RenderUpsideDown(display->w-512,display->h-512,display->w,display->h); }
  if ( WITHIN(input->mxi,input->myi,10,730,10+64,730+64) ) { Area(crayons.Pick(green255),display->w-512,display->h-512,display->w,display->h); fboScreen11.blend=transparency; fboScreen11.RenderUpsideDown(display->w-512,display->h-512,display->w,display->h); }
  
  Blending(none);
  crayons.Pick(alabaster).gl();
  Text(FORMAT(buf,127,"sea level: %1.2f tide: %1.2f",(float)seaLevel,(float)tide),128,10,10,10,false);
  Text(FORMAT(buf,127,"accumulation: %1.2f",(float)accumulation),128,30,10,10,false);
//  ShadedEmbossPreserveA(&fboScreen5.texture,&fboScreen6);
//  ShadedBlendMultiplyAlpha(&fboScreen5.texture,&fboScreen6.texture,&fboScreen7);

//  fboScreen7.blend=transparency;
//  fboScreen7.RenderUpsideDown(0,0,display->w,display->h);  

//  fboScreen6.blend=transparency;
//  fboScreen6.RenderUpsideDown(0,0,display->w,display->h);  

 }
};
