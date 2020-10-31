#include "ClassicPlanetSurface.h"

#include "GameOptions.h"

// Picking of our FBOs based on detail level
void ClassicPlanetSurface::PickFBOs() {
 out=&fbo1080p3;
 transparentAccumulator=&fboScreen14;
 switch ( options ? (OptionsSetting) (int) options->o_PlanetDetail : o_med ) {
   case o_lo:
     heightPlusFBM=&fboScreen13;
     snowAccumulation=&fbo512c;
     aboveSeaLevel=&fboCDS512; // need more
     heightWaterSurface=&fboCDS512a;
     waterSurfaceColormap=&fboCDS512b;
     waterSurface=&fboCDS512b;
     shimmerFBM=&fboScreen2;
     cloudFBM=&fbo512d;
     surfaceColor=&fbo512c;
     surfaceOverdub=&fbo512d;
//     surfaceOverdubBlur=&fboCDS512;
     fabricSoftener=&fbo512d;
     shadowBasemap=&fbo512c;
     transparentAccumulator=&fboCDS512b; // unused?
     heightModifier=&fboCDS512a;
     finalComposite=&fboScreen12;
     compositeColorMap=&fboScreen13;
     lightMap=&fbo512a;
    break;
  case o_med:
     snowAccumulation=&fboScreen1;
     aboveSeaLevel=&fboScreen2;
     heightWaterSurface=&fboScreen3;
     waterSurface=&fboScreen4;
     surfaceColor=&fboScreen5;
     surfaceOverdub=&fboScreen6;
     surfaceOverdubBlur=&fboScreen7;
     fabricSoftener=&fboScreen8;
     shadowBasemap=&fboScreen9;
     finalComposite=&fboScreen10;
     heightPlusFBM=&fboScreen11;
     heightNormal=&fboScreen12; // willNormal
     heightModifier=&fboScreen15;
     compositeColorMap=&fboScreen13;
     waterSurfaceColormap=&fbo512;
     shimmerFBM=&fbo512a;
     cloudFBM=&fbo512b;
     lightMap=&fboScreen2a;
     transparentAccumulator=&fboScreen14;
    break;
   case o_hi:
     transparentAccumulator=&fboScreen3; // unused?
     snowAccumulation=&fboScreen4;
     heightPlusFBM=&fboScreen5;
     aboveSeaLevel=&fboScreen6;
     surfaceColor=&fboScreen7;
     heightWaterSurface=&fboScreen8;     
     waterSurfaceColormap=&fboScreen9;
     surfaceOverdub=&fboScreen10;
     waterSurface=&fboScreen5;
     shimmerFBM=&fboScreen12;
     fabricSoftener=&fboScreen11;
     finalComposite=&fboScreen12;
     shadowBasemap=&fboScreen13;
     heightNormal=&fboScreen14; // willNormal
     cloudFBM=&fboScreen6a;
     lightMap=&fboScreen7a;
     transparentAccumulator=&fboScreen14;
//     finalClouds=&fboCDSScreen;
//     surfaceOverdubBlur=&fboScreen8;
     heightModifier=&fboScreen15;
//     cloudDeformer=&fboScreen4a;
    break;
     default:
     snowAccumulation=&fboScreen4;
     heightPlusFBM=&fboScreen5;
     aboveSeaLevel=&fboScreen5;
     surfaceColor=&fboScreen5;
     heightWaterSurface=&fboScreen6;
     waterSurfaceColormap=&fboScreen7;
     surfaceOverdub=&fboScreen7;
     waterSurface=&fboScreen9;
     shimmerFBM=&fboScreen10;
     heightNormal=&fboScreen11; // willNormal
     cloudFBM=&fbo512d;
     compositeColorMap=&fboScreen12;
     finalComposite=&fboScreen13;
//     finalClouds=&fbo512;
//     surfaceOverdubBlur=&fboScreen8;
     fabricSoftener=&fboScreen1a;
     shadowBasemap=&fboScreen2a;
     lightMap=&fboScreen3a;
     heightModifier=&fboScreen15;
//     cloudDeformer=&fboScreen4a;
    break;
 }
}

void ClassicPlanetSurface::CreateCompositeBackground() {
  /// In this step, we use all of the FBOs to create the combined region composite height and color maps
 regions.Render();
 mapper.Render();
 
 /// From these combined maps, we create the various layers and the height normal.
 ProcessShimmer();    // Creates ocean surface
 ProcessHeightFBM();  // Creates master heightmap and normal
 ProcessClouds();     // Generates cloud data
 ProcessSnow();       // Generates snow accumulation layer
 ProcessSeaLevel();   // Cuts the rivers
 ProcessHeightNormal();

 /// Assemble the ocean surface, land topography and snow accumulation layers to create the composite color map.
 compositeColorMap->background.fromCrayon(black);
 compositeColorMap->Bind();
 glEnable(GL_TEXTURE_2D);
 Blending(none);
 environment.sea.gl();
 glBindTexture(GL_TEXTURE_2D,waterSurface->texture);
 compositeColorMap->QuadYInverted();
 Blending(transparency);
 crayons.Pick(alabaster).gl();
 glBindTexture(GL_TEXTURE_2D,aboveSeaLevel->texture);
 compositeColorMap->QuadYInverted();
 glBindTexture(GL_TEXTURE_2D,snowAccumulation->texture);
 compositeColorMap->QuadYInverted();
 glDisable(GL_TEXTURE_2D);
 landmark.Render();
/// foliage.Render();
/// bases->Render();
 compositeColorMap->Unbind();

 /// Finalize
 lamps.Render(finalComposite,compositeColorMap,heightNormal,lightMap);

 finalComposite->BindWithoutClearing();
 if ( /*options->o_PlanetDetail > o_lo &&*/
   (isJovian || environment.radioactive) && seed % 7 == 1 ) {
  Crayon addition;
  addition.Brights(seed);
  addition.Multiply(0.25f);
  addition.gl();
  Blending(additive);
  glBindTexture(GL_TEXTURE_2D,surfaceOverdub->texture);
  finalComposite->QuadYInverted();
 }
 finalComposite->Unbind();
 
}

void ClassicPlanetSurface::DebugInfo() {
 int fwqls_color_regions=0;
 int fwqls_height_regions=0;
 EACH(regions.first,LithoRegion,f) {
  fwqls_color_regions+=(int)f->c_fwql.count;
  fwqls_height_regions+=(int)f->h_fwql.count;
 }
 Crayon color(green);
 color.gl();
 Blending(none);
 Text(FORMAT(buf,123,"%d regions, %d c_fwqls, %d h_fwqls",
  (int)regions.count,fwqls_color_regions,fwqls_height_regions),
  10,900,8,10,false);
 for ( int i=0; i< (int) mapper.R; i++ ) {
  fbo512.Bind();
  mapper.fwql[i].RenderUpsideDown();
  fbo512.Unbind();
  fbo512.Render(700+i*64,900,764+i*64,964);
 }

 heightPlusFBM->Render(none,100,436,100+128,436+128); 
 crayons.Pick(alabaster).gl(); Text("H+F",164+2,500+10+2,10,10,false);

 snowAccumulation->Render(none,228,500,228+64,564);
 crayons.Pick(alabaster).gl(); Text("S",228+2,500+10+2,10,10,false);

 heightNormal->Render(none,100,564,228,564+128);
 crayons.Pick(alabaster).gl(); Text("H",100+2,564+10+2,10,10,false);

 waterSurface->Render(none,164+256,564,164+256+128,628+64);
 crayons.Pick(alabaster).gl(); Text("W",164+256+2,564+10+2,10,10,false);

 lightMap->Render(none,364+256,500,364+256+64,564);
 crayons.Pick(alabaster).gl(); Text("LM",364+256+2,500+10+2,10,10,false);

 heightWaterSurface->Render(none,300,628,364,628+64);
 crayons.Pick(alabaster).gl(); Text("HW",300+2,628+10+2,10,10,false);

 shimmerFBM->Render(none,228,564,228+64,564+64);
 crayons.Pick(alabaster).gl(); Text("SF",228+2,628+10+2,10,10,false);

 shadowBasemap->Render(none,228,628,228+64,628+64);
 crayons.Pick(alabaster).gl(); Text("SB",228+2,628+10+2,10,10,false);

 compositeColorMap->Render(none,400,500,400+64,500+64);
 crayons.Pick(alabaster).gl(); Text("CC",400+2,500+10+2,10,10,false);

 heightModifier->Render(none,64,400,64+64,400+64);
 crayons.Pick(alabaster).gl(); Text("HM",64+2,400+10+2,10,10,false); 

 surfaceOverdub->Render(none,64,336,128,400);
 crayons.Pick(alabaster).gl(); Text("SO",64+2,336+10+2,10,10,false); 

 surfaceOverdubBlur->Render(none,64+64,336,128+64,400);
 crayons.Pick(alabaster).gl(); Text("SOB",128+2,336+10+2,10,10,false);

 lamps.DebugInfo();
}