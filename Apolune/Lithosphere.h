#pragma once
#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "VBO.h"
#include "FBO.h"
#include "MultiPointSprite.h"
#include "MultiCirclePlacer.h"
#include "ForeverWrappingQuadLayers.h"
#include "ClassicPlanetSurfacer.h"
#include "ZIndexed.h"

// Lithosphere (and surface)
 // - fbmShader (non-repeating mountains)
 // - fwql surface texturing (additive)
 // - fwql subtracted color overlay from planet surface map box
 // - fwql surface texturing (transparency)
class LithoRegion : public ListItem {
public:
// Zbool mountains;
// Zp<float> erosionTime;
// Zfloat factor;
// Zfloat amp;
// Zfloat mountainRoughness,mountainScale,mountainSeed;
 ForeverWrappingQuadLayers h_fwql,c_fwql;
 Zp<FBOColor> height,color;
 Zp<ClassicPlanetSurfaceProfile> profile;
 LithoRegion( ClassicPlanetSurfaceProfile *p, int idx ) : ListItem() {
  profile=p;
  switch ( idx ) {
  case 0:
    height=&fbo512a;
    color=&fbo512b;
   break;
  case 1:
    height=&fbo512c;
    color=&fbo512d;
   break;
  case 2:
    height=&fboScreen12;
    color=&fboScreen11;
   break;
  case 3:
    height=&fboScreen10;
    color=&fboScreen9;
   break;
  case 4:
    height=&fboScreen8;
    color=&fboScreen7;
   break;
  case 5:
    height=&fboScreen6;
    color=&fboScreen5;
   break;
  case 6:
    height=&fboScreen4;
    color=&fboScreen3;
   break;
  case 7:
    height=&fboScreen2;
    color=&fboScreen1;
   break;
  default:
    OUTPUT("LithoRegion: Warning, limit reached.\n");
   break;
  }
//  mountains=true;
//  factor=10.0f*abs(randoms.Unique(seed,idx));
//  amp=1.5f*abs(randoms.Unique(seed,idx));
  h_fwql.screen.Set(0.0,0.0,(double)height->w,(double)height->h);
  h_fwql.MoveBy(0.0,0.0);
  c_fwql.screen.Set(0.0,0.0,(double)color->w,(double)color->h);
  c_fwql.MoveBy(0.0,0.0);
  Setup(p,idx);
 }
 void Setup( ClassicPlanetSurfaceProfile *p, int idx ) {
  Random *setA=randoms.GetSet(idx+p->regionsSeed);
  int clitholayers=0;
  if ( p->cImage1 ) clitholayers++;
  if ( p->cImage2 ) clitholayers++;
  if ( p->cImage3 ) clitholayers++;
  if ( p->cImage4 ) clitholayers++;
  int hlitholayers=0;
  if ( p->hImage1 ) hlitholayers++;
  if ( p->hImage2 ) hlitholayers++;
  if ( p->hImage3 ) hlitholayers++;
  if ( p->hImage4 ) hlitholayers++;
  ZIndexed<Zp<GLImage>> cimages;
  cimages.Size(4);
  cimages[0]=p->cImage1;
  cimages[1]=p->cImage2;
  cimages[2]=p->cImage3;
  cimages[3]=p->cImage4;
  ZIndexed<Crayon *> ctints;
  ctints.Size(4);
  ctints[0]=&p->cTint1;
  ctints[1]=&p->cTint2;
  ctints[2]=&p->cTint3;
  ctints[3]=&p->cTint4;
  ZIndexed<Zp<GLImage>> himages;
  himages.Size(4);
  himages[0]=p->cImage1;
  himages[1]=p->cImage2;
  himages[2]=p->cImage3;
  himages[3]=p->cImage4;
  ZIndexed<Crayon *> htints;
  htints.Size(4);
  htints[0]=&p->hTint1;
  htints[1]=&p->hTint2;
  htints[2]=&p->hTint3;
  htints[3]=&p->hTint4;
  h_fwql.Clear();
  c_fwql.Clear();
  int totalc=clitholayers;
  int totalh=hlitholayers;
  for ( int i=0; i<totalc; i++ ) {
   int coffset=(idx+p->regionsSeed+i)%totalc;
   int coffset2=(idx+p->regionsSeed+i*3)%totalc;
   c_fwql.Add( cimages[coffset].pointer, additive, *ctints[coffset] );
   int hoffset=(idx+p->regionsSeed+i)%totalh;
   if ( h_fwql.count == 0 || setA->Query(coffset+hoffset) ) { // Duplicate last on the heightmap
    h_fwql.Add( cimages[coffset].pointer, additive, *ctints[coffset2] );
   } else if ( setA->Query(coffset+hoffset+coffset2) ) {
    totalh--;
    h_fwql.Add( himages[coffset].pointer, additive, *htints[coffset2]  );
   }
  }
  c_fwql.MoveBy(0.0,0.0);
  for ( int j=0; j<totalh; j++ ) {
   int hoffset=(idx+p->regionsSeed+j)%totalh;
   int hoffset2=(idx+p->regionsSeed+j*3)%totalh;
   if ( (j!=(totalh-1) || setA->Query(j+totalh+p->regionsSeed+idx)) )
   h_fwql.Add( himages[hoffset].pointer, additive, *htints[hoffset] );
   else
   h_fwql.Add( himages[hoffset2].pointer, additive, *htints[hoffset2] );
  }
  h_fwql.MoveBy(0.0,0.0);
 }
 void Render() {  
  RenderHeight();
  RenderColor();
 }
 void RenderHeight() {
  // Generate heightmap
  height->background.Pick(black);
  height->Bind();
  Blending(none);
  h_fwql.RenderUpsideDown();
  height->Unbind();
 }
 void RenderColor() {
  // Generate background / color layer
  color->Bind();
  Blending(none);
  c_fwql.RenderUpsideDown();
  color->Unbind();
 }
 void MoveBy( double x, double y ) {
  h_fwql.MoveBy(x,y);
  c_fwql.MoveBy(x,y);
 }
};

// Lithoregions come together in a combiner
class LithoRegions : public LinkedList {
public:
 Zp<float> erosionTime;
 LithoRegions() : LinkedList() {}
 CLEARLISTRESET(LithoRegion);
 void Append( LithoRegion *r ) { _Append(r); }
 void Render() { FOREACH(LithoRegion,r) r->Render(); }             // Render each region to its respective maps
 void RenderColor() { FOREACH(LithoRegion,r) r->RenderColor(); }   // Render each region to its respective maps
 void RenderHeight() { FOREACH(LithoRegion,r) r->RenderHeight(); } // Render each region to its respective maps
 Zp<ClassicPlanetSurfaceProfile> profile;
 void Setup( ClassicPlanetSurfaceProfile *p ) {
  Clear();
  profile=p;
  // Count images available
  int clitholayers=0;
  if ( p->cImage1 ) clitholayers++;
  if ( p->cImage2 ) clitholayers++;
  if ( p->cImage3 ) clitholayers++;
  if ( p->cImage4 ) clitholayers++;
  int hlitholayers=0;
  if ( p->hImage1 ) hlitholayers++;
  if ( p->hImage2 ) hlitholayers++;
  if ( p->hImage3 ) hlitholayers++;
  if ( p->hImage4 ) hlitholayers++;
  int smaller=UMIN(clitholayers,hlitholayers);
  for ( int i=0; i<smaller; i++ ) {
   Append(new LithoRegion(p,i));
  }
 }
 /*
 void Generate( int seed, bool isJovian, bool hasTech ) {
  Clear();
  if ( isJovian ) {
   int total=(seed%3)+2;
   for ( int i=0; i<total; i++ ) Append(new LithoRegion(seed+total+i,i,seed+123,hasTech));
  } else {
   int total=(seed%3)+1;
   for ( int i=0; i<total; i++ ) Append(new LithoRegion(seed+total+i,i,seed+1,hasTech));
  }
 }
 */
 void MoveBy( double x, double y ) { FOREACH(LithoRegion,r) r->MoveBy(x,y); }
};
