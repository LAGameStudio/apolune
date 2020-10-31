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
#pragma once

#include "ImageProcess.h"
#include "GLImage.h"
#include "Vertexd.h"
#include "Indexed.h"
#include "Cartesian.h"
#include "VBOGroup.h"
#include "VBO.h"

struct LithoHeight {
public:
 Vertexd position;
 Zdouble luminance;
 Zint contributers;
 void Average() {
  if ( contributers > 1 ) {
   position.y/=((double)contributers);
   contributers=1;
  }
 }
};

class Lithotopography {
public:
 Indexed<LithoHeight> heights;
 Zdouble floor;
 Vertexd extents;
 Vertexd offset;
 Lithotopography() {}
 Lithotopography( GLImage *image, unsigned int samplew, unsigned int sampleh, Vertexd *unitExtents, double floor ) {
  this->floor=floor;
  heights.Size((size_t)samplew,(size_t)sampleh); // Determines number of output polygons to build topography
  Survey(image,unitExtents);
 }
 void Survey( GLImage *image, Vertexd *extents ) {
  unsigned int w=image->width;
  unsigned int h=image->height;
  unsigned int i,j;
  Crayon pixel;
  this->extents.Set(extents);
  for ( i=0; i<w; i++ ) {
   for ( j=0; j<h; j++ ) {
    image->image->GetPixel(i,j,&pixel);
    double lum=extents->z-floor+floor*pixel.Luminance();
    unsigned int m=(unsigned int)(iratiod(i,w)*(double)heights.w);
    unsigned int n=(unsigned int)(iratiod(j,h)*(double)heights.h);
    heights.list[m+n*heights.w].position.Add(0,extents->z*lum,0);
    heights.list[m+n*heights.w].contributers++;
    //OUTPUT( "lum: %f m,n: %i,%i i,j: %i,%i\n", lum, m,n, i,j );
   }
  }
  for ( i=0; i<heights.w; i++ ) {
   for ( j=0; j<heights.h; j++ ) {
    heights.list[i+j*heights.w].Average();
    heights.list[i+j*heights.w].position.Set(
     (extents->x / (double) heights.w) * i,
     heights.list[i+j*heights.w].position.y,
     (extents->y / (double) heights.h) * j
    );
   }
  }
 }
 void Export( VBOStaticVNT *vbo ) {
  Vertex vert;
  unsigned int i,j;
  // Push Topography
  if ( vbo->uploaded ) vbo->Release();
  vbo->Clear();
  for ( i=0; i<heights.w-1; i++ ) {
   for ( j=0; j<heights.h-1; j++ ) {
    Vertexd a(&heights.list[    i+j*heights.w].position);
    Vertexd b(&heights.list[(i+1)+j*heights.w].position);
    Vertexd c(&heights.list[(i+1)+(j+1)*heights.w].position);
    Vertexd d(&heights.list[    i+(j+1)*heights.w].position);
    vert.x=(GLfloat)a.x; vert.y=(GLfloat)a.y; vert.z=(GLfloat)a.z; vert.q=0.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
    vert.x=(GLfloat)b.x; vert.y=(GLfloat)b.y; vert.z=(GLfloat)b.z; vert.q=1.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
    vert.x=(GLfloat)c.x; vert.y=(GLfloat)c.y; vert.z=(GLfloat)c.z; vert.q=1.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
    vert.x=(GLfloat)a.x; vert.y=(GLfloat)a.y; vert.z=(GLfloat)a.z; vert.q=0.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
    vert.x=(GLfloat)d.x; vert.y=(GLfloat)d.y; vert.z=(GLfloat)d.z; vert.q=1.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
    vert.x=(GLfloat)c.x; vert.y=(GLfloat)c.y; vert.z=(GLfloat)c.z; vert.q=1.0; vert.r=0.0; vert.Normal(); vbo->PushVertex(&vert);
   }
  }
  vbo->Create();
 }
 void Export( VBONormalsPolysGroup *vbos, STLFile *stl, bool keep=false ) {
  Vertex vert;
  unsigned int i,j;
  // Push Topography
  vbos->Clear();
  Crayon dark; dark.Pick(black);
  Crayon ca,cb,cc,cd;
  Vertexd a,b,c,d,a0,b0,c0,d0;
  Crayon white;
  white.Pick(alabaster);
  Vertexd origin;
  origin.Set(&extents);
  origin.Divide(0.5);
  for ( i=0; i<heights.w-1; i++ ) {
   for ( j=0; j<heights.h-1; j++ ) {
    a.Set(&heights.list[    i+j*heights.w].position);
    ca.Pick(alabaster); ca.ScaleByAlpha(a.z);
    b.Set(&heights.list[(i+1)+j*heights.w].position);
    cb.Pick(alabaster); cb.ScaleByAlpha(b.z);
    c.Set(&heights.list[(i+1)+(j+1)*heights.w].position);
    cc.Pick(alabaster); cc.ScaleByAlpha(c.z);
    d.Set(&heights.list[    i+(j+1)*heights.w].position);
    cd.Pick(alabaster); cd.ScaleByAlpha(d.z);
    vbos->PushQuad(&a,&ca,&b,&cb,&c,&cc,&d,&cd,stl);
    if ( i == 0 ) { // Left
     a0.Set(&a); a.z=0;
     d0.Set(&d); d.z=0;
     vbos->PushQuad(&a0,&dark,&d0,&dark,&d,&cd,&a,&ca); //(&a,&ca,&d,&cd,&d0,&dark,&a0,&dark);
     vbos->PushTri(&a0,&dark,&d0,&dark,&origin,&white,stl); // Underside / Left
    } else if ( i == heights.w-1 ) { // Right
     b0.Set(&b); b.z=0;
     c0.Set(&c); c.z=0;
     vbos->PushQuad(&a0,&dark,&d0,&dark,&d,&cd,&a,&ca); //(&a,&ca,&d,&cd,&d0,&dark,&a0,&dark);
     vbos->PushTri(&b0,&dark,&c0,&dark,&origin,&white,stl); // Underside / Right
    }
    if ( j == 0 ) { // Front
     a0.Set(&a); a.z=0;
     b0.Set(&b); b.z=0;
     vbos->PushQuad(&a0,&dark,&d0,&dark,&d,&cd,&a,&ca); //(&a,&ca,&d,&cd,&d0,&dark,&a0,&dark);
     vbos->PushTri(&a0,&dark,&b0,&dark,&origin,&white,stl); // Underside / Front
    } else if ( j == heights.h-1 ) { // Back
     c0.Set(&c); c.z=0;
     d0.Set(&d); d.z=0;
     vbos->PushQuad(&a0,&dark,&d0,&dark,&d,&cd,&a,&ca); //(&a,&ca,&d,&cd,&d0,&dark,&a0,&dark);
     vbos->PushTri(&c0,&dark,&d0,&dark,&origin,&white,stl); // Underside / Back
    }
   }
  }
  if ( keep ) vbos->Create();
  else vbos->CreateClear();
 }
};

class Lithophane : public ListItem {
public:
 Zpointer<GLImage> image;
 Zdisposable<Lithotopography> topography;
 Vertexd size;
 Zdouble floor;
 Coord2i heights;
 VBOStaticVNT vbo;
 Lithophane() {}
 Lithophane( const char *filename, unsigned int samplesW, unsigned int samplesH, double unitW, double unitH, double total_thickness, double etch_floor ) {
  Load(filename);
  SetResolution(samplesW,samplesH,unitW,unitH,total_thickness,etch_floor);
  Survey();
 }
 void SetResolution( unsigned int sx, unsigned int sy, double w, double h, double d, double lowz ) {
  heights.Set(sx,sy);
  size.Set(w,h,d);
  floor=lowz;
 }
 void Load( const char *filename ) {
  image.Delete();
  image = new GLImage(filename);
 }
 void Survey() {
  topography.Deinstantiate();
  topography.Recycle(new Lithotopography(image.pointer,heights.x,heights.y,&size,floor.value));
  topography->Export(&vbo);
 }
 void Render() { vbo.Render(); }
};