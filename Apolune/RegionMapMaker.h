#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "ForeverWrappingQuadLayers.h"
#include "BendShader.h"
#include "FBMShader.h"
#include "ConvolverShader.h"
#include "BlendShader.h"
#include "ColorNetwork.h"
#include "BloomShader.h"
#include "Lithosphere.h"
#include "ZIndexed.h"

//#define MAX_REGION_MAP_LAYERS 8
#define MAX_REGION_MAP_LAYERS 4

class RegionMapMaker {
public:
 Crayon blank;
 Indexed<Crayon> threshold;
 Indexed<Crayon> channel;
 Indexed<ForeverWrappingQuadLayers> fwql;
 Zp<LithoRegions> regions;
 Zp<FBOColor> cfbo1,cfbo2,cfbo3,cfbo4,cfbo5,cfbo6,cfbo7,cfbo8;
 Zp<FBOColor> hfbo1,hfbo2,hfbo3,hfbo4,hfbo5,hfbo6,hfbo7,hfbo8;
 Zp<FBOColor> colorBlended,heightBlended,map,maps,work;
 Zuint R;
 RegionMapMaker() {
  colorBlended=&fboScreen1a;
  heightBlended=&fboScreen2a;
  map=&fboScreen3a;
  maps=&fboScreen4a;
  work=&fboScreen5a;
  fwql.Size(8);
  for ( unsigned int i=0; i<8; i++ ) {
   fwql[i].screen.Set(
    5.0*(double)(-work->w),
    5.0*(double)(-work->h),
    5.0*(double)(work->w)+(double)(work->w),
    5.0*(double)(work->h)+(double)(work->h)
   );
  }
  blank.Pick(clear);
  channel.Size(8);
  channel[0].Float(1.0,0.0,0.0,1.0);
  channel[1].Float(0.0,1.0,0.0,1.0);
  channel[2].Float(1.0,0.0,1.0,1.0);
  channel[3].Float(0.0,0.0,0.0,1.0);
  channel[4].Float(0.0,1.0,1.0,1.0);
  channel[5].Float(1.0,1.0,0.0,1.0);
  channel[6].Float(1.0,0.0,1.0,1.0);
  channel[7].Float(0.0,0.0,0.0,0.0);
  threshold.Size(8);
  for ( unsigned int i=0; i<8; i++ ) threshold[i].Float(0.85f,0.85f,0.85f,1.0f);
 }
 void Attach( LithoRegions *regions ) {
  R=regions->count>MAX_REGION_MAP_LAYERS ? MAX_REGION_MAP_LAYERS-1 : regions->count;
  cfbo1=cfbo2=cfbo3=cfbo4=cfbo5=cfbo6=cfbo7=cfbo8=&fboScreen14;
  hfbo1=hfbo2=hfbo3=hfbo4=hfbo5=hfbo6=hfbo7=hfbo8=&fboScreen15;
  LithoRegion *one  =(LithoRegion *) regions->Element(0);
  LithoRegion *two  =(LithoRegion *) regions->Element(1);
  LithoRegion *three=(LithoRegion *) regions->Element(2);
  LithoRegion *four =(LithoRegion *) regions->Element(3);
  LithoRegion *five =(LithoRegion *) regions->Element(4);
  LithoRegion *six  =(LithoRegion *) regions->Element(5);
  LithoRegion *seven=(LithoRegion *) regions->Element(6);
  LithoRegion *eight=(LithoRegion *) regions->Element(7);
  switch ( (int) regions->count ) {
  case 0: break;
  case 1:
      cfbo1=(one  )->color; hfbo1=(one  )->height;
   break;
  case 2:
    cfbo1=(one  )->color; hfbo1=(one  )->height;
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
   break;
  case 3:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
   break;
  case 4:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
    cfbo4=(four )->color; hfbo4=(four )->height; 
   break;
  case 5:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
    cfbo4=(four )->color; hfbo4=(four )->height; 
    cfbo5=(five )->color; hfbo5=(five )->height; 
   break;
  case 6:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
    cfbo4=(four )->color; hfbo4=(four )->height; 
    cfbo5=(five )->color; hfbo5=(five )->height; 
    cfbo6=(six  )->color; hfbo6=(six  )->height; 
   break;
  case 7:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
    cfbo4=(four )->color; hfbo4=(four )->height; 
    cfbo5=(five )->color; hfbo5=(five )->height; 
    cfbo6=(six  )->color; hfbo6=(six  )->height; 
    cfbo7=(seven)->color; hfbo7=(seven)->height; 
   break;
  default:
    cfbo1=(one  )->color; hfbo1=(one  )->height; 
    cfbo2=(two  )->color; hfbo2=(two  )->height; 
    cfbo3=(three)->color; hfbo3=(three)->height; 
    cfbo4=(four )->color; hfbo4=(four )->height; 
    cfbo5=(five )->color; hfbo5=(five )->height; 
    cfbo6=(six  )->color; hfbo6=(six  )->height; 
    cfbo7=(seven)->color; hfbo7=(seven)->height; 
    cfbo8=(eight)->color; hfbo8=(eight)->height; 
   break;
  }
 }
 void Render() {
  map->background.Pick(clear);
  maps->background.Pick(clear);
  maps->Bind(); maps->Unbind();
  if ( R.value>0 ) for ( unsigned int i=0; i<(R-1); i++ ) {
   map->Bind();
   fwql[i].RenderUpsideDown();
   map->Unbind();
   ShadedColorAboveBelow(threshold[i],channel[i],blank,&map->texture,work);
   maps->BindWithoutClearing();
   glColor3d(1.0,1.0,1.0);
   Blending(transparency);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,work->texture);
   maps->QuadYInverted();
   glDisable(GL_TEXTURE_2D);
   maps->Unbind();
  }
  switch ( (int) regions->count ) {
   case 0:
     colorBlended->background.Pick(black);
     colorBlended->Bind();
     colorBlended->Unbind();
     heightBlended->background.Pick(black);
     heightBlended->Bind();
     heightBlended->Unbind();
    break;
   case 1:
     colorBlended->background.Pick(black);
     colorBlended->Bind();
     Blending(none);
     cfbo1->RenderUpsideDown(0,0,heightBlended->w,heightBlended->h);
     colorBlended->Unbind();
     heightBlended->background.Pick(black);
     heightBlended->Bind();
     Blending(none);
     hfbo1->RenderUpsideDown(0,0,heightBlended->w,heightBlended->h);
     heightBlended->Unbind();
    break;
   case 2:
    ShadedBlend2Way(
     &maps->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend2Way(
     &maps->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 3:
    ShadedBlend3Way(
     &maps->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend3Way(
     &maps->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 4:
    ShadedBlend4Way(
     &maps->texture,
     &cfbo4->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend4Way(
     &maps->texture,
     &hfbo4->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 5:
    ShadedBlend5Way(
     &maps->texture,
     &cfbo5->texture,
     &cfbo4->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend5Way(
     &maps->texture,
     &hfbo5->texture,
     &hfbo4->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 6:
    ShadedBlend6Way(
     &maps->texture,
     &cfbo6->texture,
     &cfbo5->texture,
     &cfbo4->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend6Way(
     &maps->texture,
     &hfbo6->texture,
     &hfbo5->texture,
     &hfbo4->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 7:
    ShadedBlend7Way(
     &maps->texture,
     &cfbo7->texture,
     &cfbo6->texture,
     &cfbo5->texture,
     &cfbo4->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend7Way(
     &maps->texture,
     &hfbo7->texture,
     &hfbo6->texture,
     &hfbo5->texture,
     &hfbo4->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
   case 8:
    ShadedBlend8Way(
     &maps->texture,
     &cfbo8->texture,
     &cfbo7->texture,
     &cfbo6->texture,
     &cfbo5->texture,
     &cfbo4->texture,
     &cfbo3->texture,
     &cfbo2->texture,
     &cfbo1->texture,
     colorBlended
    );
    ShadedBlend8Way(
     &maps->texture,
     &hfbo8->texture,
     &hfbo7->texture,
     &hfbo6->texture,
     &hfbo5->texture,
     &hfbo4->texture,
     &hfbo3->texture,
     &hfbo2->texture,
     &hfbo1->texture,
     heightBlended
    );
   break;
  }
 }
 void MoveBy( double x, double y ) {
  for ( unsigned int i=0; i<R; i++ ) {
   fwql[i].MoveBy(x,y);
  }
 }
 Zp<ClassicPlanetSurfaceProfile> profile;
 void Setup( ClassicPlanetSurfaceProfile *p ) {
  profile=p;
  ZIndexed<Zp<GLImage>> images;
  images.Size(4);
  images[0]=p->rImage1;
  images[1]=p->rImage2;
  images[2]=p->rImage3;
  images[3]=p->rImage4;
  ZIndexed<Crayon *> thresholds;
  thresholds.Size(4);
  thresholds[0]=&p->rThreshold1;
  thresholds[1]=&p->rThreshold2;
  thresholds[2]=&p->rThreshold3;
  thresholds[3]=&p->rThreshold4;
  for ( unsigned int i=0; i<R; i++ ) {
   fwql[i].Clear();
   fwql[i].Add( images[i].pointer,additive,crayons.Pick(alabaster) );
   fwql[i].MoveBy(0.0,0.0);
   threshold[i].fromCrayon(*thresholds[i]); // alpha must be 1.0?
  }
 }
 /*
 void Generate( int seed ) {
  int marcher=seed;
  int marcher2=seed/3+seed;
  int marcher3=seed+3;
  for ( unsigned int i=0; i<R; i++ ) {
   Random *set=randoms.GetSet(marcher);
   fwql[i].Clear();
   fwql[i].Add(
    library.Load(geo_high_contrast.AnyString(marcher).c_str(),true,GL_REPEAT,GL_REPEAT),
    additive,crayons.Pick(alabaster)
   );
   fwql[i].MoveBy(0.0,0.0);
   threshold[i].Float(
    set->arange(marcher2,0.5f,0.75f),
    set->arange(marcher3,0.5f,0.75f),
    set->arange(marcher, 0.5f,0.75f),
    1.0f
   );
   marcher++;
   marcher2++;
   marcher3+=3;
  }
 }
 */
};