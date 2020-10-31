#pragma once

#include "ZeroTypes.h"

#include "BendShader.h"
#include "FBMShader.h"
#include "ConvolverShader.h"
#include "BlendShader.h"
#include "ParallaxShader.h"
#include "PlasmaShader.h"

#include "LandfallEnvironment.h"
#include "RegionMapMaker.h"
#include "Sunlamps.h"
///#include "ForestPrimeval.h"
///#include "PlanetaryBuiltBases.h"

#include "ClassicPlanetSurfacer.h"

class ClassicPlanetSurface : public ListItem {
public:
 Zp<ClassicPlanetSurfaceProfile> profile;
 LandfallEnvironment environment;
 Zint seed;
 Zbool hasTech,isJovian;
 Zbool bendClouds;
 Cartesian quake;
 Zfloat bloomMod;
 Zdouble bgchangeDelay;
 Zp<FBOColor> out;

 // Lithosphere (and surface) - generates unblemished color maps and height maps, which are later combined using the combiner stage
 // - FAF stamps (geological morphologies)
 // - ruins FAF stamps (similar to geological morphologies)
 LithoRegions regions;
 RegionMapMaker mapper;

 // Plow (height etch)
 FireAndForgetLimitedWrappingProce55Viewer2d plow;

 // Shimmer etch
 ///FireAndForgetLimitedWrappingProce55Viewer2d waves;

 // Color etch
 FireAndForgetLimitedWrappingProce55Viewer2d landmark;

 ///Foliage foliage;
 ///Zp<PlanetaryBuiltBases> bases;

 Sunlamps lamps;

 Zp<GLImage> noiseTex;
 Zp<FBOColor> heightPlusFBM,aboveSeaLevel,snowAccumulation,heightWaterSurface,heightNormal;
 Zp<FBOColor> waterSurfaceColormap,waterSurface,shimmerFBM,compositeColorMap;
 Zp<FBOColor> surfaceColor,surfaceOverdub,surfaceOverdubBlur;
 Zp<FBOColor> cloudFBM,finalClouds;
 Zp<FBOColor> shadowBasemap;
 Zp<FBOColor> lightMap;
 Zp<FBOColor> transparentAccumulator;
 Zp<FBOColor> fabricSoftener;
 Zp<FBOColor> heightModifier,cloudDeformer;
 Zp<FBOColor> finalComposite;

 ClassicPlanetSurface() : ListItem() {
  regions.erosionTime=&environment.erosionTime.value;
  mapper.regions=&regions;
  Init();
  PickFBOs();
 }

 void Setup( ClassicPlanetSurfaceProfile *p ) {
  Init();
  profile=p;
  lamps.Set(&p->lamps);
  environment.Setup(p);
  noiseTex=p->waterNoise;
  regions.Setup(p);
  mapper.Attach(&regions);
  mapper.Setup(p);
  bloomMod+=p->bloomMod;
 }

 /*
 void Seed( SolarSystem *s, Planet *p, int seed ) {
  this->seed=seed;
  lamps.Set(s,p);
  this->isJovian=p->type==gas_giant; //isJovian;
  this->hasTech=hasTech;
  noiseTex=geo_water_noise.Any(seed);
  regions.Generate(seed,isJovian,hasTech);
  mapper.Attach(&regions);
  mapper.Generate(upto(seed,123456));
  environment.Seed(seed);
  Random *setB=randoms.GetSet(seed+52);
  Random *setC=randoms.GetSet(seed+57);
  bloomMod=0.001f+0.9f*(setC->aunique(33+seed)+setB->unique(55+seed));;
  if ( isJovian && seed % 3 == 1 ) {
   environment.tectonics=FRAMETIME*setC->unique(444+seed);
   environment.shimmerSpeed+=FRAMETIME;
   environment.shimmerFactor*=10.0f;
  }
  ///foliage.Seed(s->system_catalog_no,p->planet_catalog_no);
 }*/

 void Init() {
  plow.Init();
  ///waves.Init();
  landmark.Init();
  environment.Init();
  noiseTex=library.Load("data/images/noise1.png",true,GL_REPEAT,GL_REPEAT);
 }

 void PickFBOs();

 void ClearAccumulators() {
  lightMap->background.Pick(clear);
  lightMap->Bind();
  lightMap->Unbind();
  
  heightModifier->background.Pick(clear);
  heightModifier->Bind();
  heightModifier->Unbind();

  transparentAccumulator->background.Pick(clear);
  transparentAccumulator->Bind();
  transparentAccumulator->Unbind();
 }

 // Generate the unmixed cloud layer
 void ProcessClouds() {
  fbmShader.time=environment.windTime;
  fbmShader.factor=environment.cloudFactor;
  fbmShader.colorAmp=(float)environment.cloudFloor+(float)environment.cloudCover*(environment.cloudThickness-(float)environment.cloudFloor);
  ShadedFBM(cloudFBM.pointer);
 }

 // In this step, we add an FBM to the blended heightmap
 void ProcessHeightFBM() {
  // Generate heightmap FBM and apply the heightmap colormap as a secondary influence
  fbmShader.time=environment.erosionTime;
  fbmShader.factor=environment.mountainFactor;
  fbmShader.colorAmp=environment.mountainHeight;
  heightPlusFBM->background.Pick(clear);
  heightPlusFBM->Bind();
   if ( isJovian && seed.value % 4 == 1 ) {
    Blending(none);
    mapper.heightBlended->RenderUpsideDown(0,0,heightPlusFBM->w,heightPlusFBM->h);
    Blending(multiply);
    int uni=randoms.Upto(10,seed.value,seed+2);
    plasmaShader.Reset();
    if ( uni <=2 ) {
     plasmaShader.scale[0]=environment.waterTime*100.0f;
     plasmaShader.scale[1]=seed.value % 4 == 1 ? 5.0f+environment.waterTime*50.0f : environment.waterTime*100.0f;
     plasmaShader.factor[0]=100.0f+900.0f*(float)abs(randoms.Unique(seed,seed+(int)(lamps.elapsed*10000.0f)));
     plasmaShader.factor[1]=10.0f+90.0f*(float)abs(randoms.Unique(seed,seed+1)+cos(lamps.elapsed));
     plasmaShader.factor[2]=10.0f+30.0f*(float)abs(randoms.Unique(seed,seed+2)+sin(lamps.elapsed));
    } else if ( uni <= 4 ) {
     plasmaShader.factor[0]=100.0f+900.0f*(float)abs(randoms.Unique(seed,seed+(int)(lamps.elapsed*10000.0f)));
     plasmaShader.factor[1]=10.0f+90.0f*(float)abs(randoms.Unique(seed,seed+1)+cos(lamps.elapsed));
     plasmaShader.factor[2]=10.0f+30.0f*(float)abs(randoms.Unique(seed,seed+2)+sin(lamps.elapsed));
    } else if ( uni <= 6 ) {
     plasmaShader.factor[0]=900.0f+100.0f*(float)abs(randoms.Unique(seed,seed+(int)(lamps.elapsed*10000.0f)));
     plasmaShader.factor[1]=90.0f+10.0f*(float)abs(randoms.Unique(seed,seed+1)+cos(lamps.elapsed));
     plasmaShader.factor[2]=30.0f+10.0f*(float)abs(randoms.Unique(seed,seed+2)+sin(lamps.elapsed));
    } else if ( uni <= 8 ) {
     plasmaShader.factor[0]=900.0f+100.0f*(float)abs(randoms.Unique(seed,seed+(int)(lamps.elapsed*10000.0f)));
    } else {}
    plasmaShader.resolution[0]=display.wf;
    plasmaShader.resolution[1]=display.hf;
    plasmaShader.position[0]=(float) moved.x;
    plasmaShader.position[1]=(float) -moved.y;
    plasmaShader.time=environment.waterTime*100.0f;
    plasmaShader.UpdateUniforms();
    heightPlusFBM->QuadYInverted();
    plasmaShader.Disable();
   } else {
    Blending(none);
    mapper.heightBlended->RenderUpsideDown(0,0,heightPlusFBM->w,heightPlusFBM->h);
   }
   Blending(additive);
   fbmShader.UpdateUniforms();
   heightPlusFBM->QuadYInverted();
   fbmShader.Disable();
   glEnable(GL_TEXTURE_2D);
   if ( isJovian && seed.value % 2 == 1 ) {
    Blending(additive);
    glBindTexture(GL_TEXTURE_2D,shimmerFBM->texture);
    heightPlusFBM->QuadYInverted();
   }
//   Blending(transparency);
//   glBindTexture(GL_TEXTURE_2D,heightModifier->texture);
//   heightPlusFBM->QuadYInverted();
   glDisable(GL_TEXTURE_2D);
   plow.Render();
   heightPlusFBM->Unbind();
 }

 // Cut the heightmap a second time to create the snow accumulator
 Crayon snow;
 void ProcessSnow() {
  snow.Float((float)environment.accumulation,(float)environment.accumulation,(float)environment.accumulation,1.0f);
  ShadedColorBelow(snow,crayons.Pick(clear),&heightPlusFBM->texture,
   fabricSoftener.pointer);
  ShadedColorAbove(snow,environment.snowColor,&fabricSoftener->texture,snowAccumulation.pointer);
 }

 // Create the "water surface" colormap
 void ProcessShimmer() {
  ShadedEmboss(&noiseTex->texture,waterSurfaceColormap.pointer);
  fbmShader.time=environment.waterTime;
  fbmShader.factor=environment.shimmerFactor;
  fbmShader.colorAmp=environment.shimmerThickness;
  ShadedFBM(shimmerFBM);
  ///shimmerFBM->BindWithoutClearing();
  ///waves.Render();
  ///shimmerFBM->Unbind();
  waterSurface->background.fromCrayon(environment.waterColor);
  ShadedBend(&shimmerFBM->texture,&waterSurfaceColormap->texture,waterSurface.pointer);
 }

 // Creates the appearance of a sea floor by creating a sea floor below
 // Cut the heightmap to create a height transparency map
 // (aboveSeaLevel= "clear")
 void ProcessSeaLevel() {
  ShadedColorBelow(environment.sea,crayons.Pick(clear),&heightPlusFBM->texture,aboveSeaLevel.pointer);
 }

 void ProcessHeightNormal() {
  // Create a normal from it
  if ( isJovian ) {
   normalmapShader.pTex0=&heightPlusFBM->texture;
   normalmapShader.resolution[0]=(float)aboveSeaLevel->w;
   normalmapShader.resolution[1]=(float)aboveSeaLevel->h;
   heightNormal->Bind();
    normalmapShader.UpdateUniforms();
    heightNormal->QuadYInverted();
    normalmapShader.Disable();
   heightNormal->Unbind();
  } else {
   normalmapShader.pTex0=&aboveSeaLevel->texture;
   normalmapShader.resolution[0]=(float)aboveSeaLevel->w;
   normalmapShader.resolution[1]=(float)aboveSeaLevel->h;
   heightNormal->Bind();
    normalmapShader.UpdateUniforms();
    heightNormal->QuadYInverted();
    normalmapShader.Disable();
   heightNormal->Unbind();
  }
 }

 void CreateCompositeBackground();

 void Between() {
  plow.BetweenFrames();
  ///waves.BetweenFrames();
  landmark.BetweenFrames();
  environment.Between();
  environment.cloudColor.fromCrayon(environment.cloudColors.Request(fbmShader.position[0],fbmShader.position[1]));
 }

 Cartesiand moved;
 void MoveBy( double x, double y ) {
  moved.x+=x;
  moved.y+=y;
  plow.MoveByd(x,y);
  ///waves.MoveByd(x,y);
  landmark.MoveByd(x,y);
  fbmShader.position[0]+=(float)x;
  fbmShader.position[1]+=(float)-y;
  regions.MoveBy(x,y);
  mapper.MoveBy(x,y);
  environment.MoveBy(x,y);
  ///foliage.MoveBy(x,y);
 }
 void MoveTo( double x, double y ) {
  MoveBy(-moved.x,-moved.y);
  MoveBy(x,y);
 }

 void Randomize() {
  noiseTex=geo_water_noise.Any();
  environment.Randomize();
 }

 void Render( Cartesiand *position, int x, int y, int w, int h ) {
  CreateCompositeBackground();
  finalComposite->RenderUpsideDown(x,y,w,h);
  if ( input.KeyDown(DX_1) ) FBODebug();
 }

 void DebugInfo();

};