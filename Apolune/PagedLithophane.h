#pragma once
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
/* IN collaboration with: */
/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    * (c) 2014 PieceMaker Technologies
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        *----------------------------------
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * Paged lithophane for high res
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * generative geometry based on
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * luminance heightmaps
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/

#include "BoxBool.h"
#include "Lithophane.h"
#include "Vertex.h"
#include "ParallaxShader.h"
#include "STLFile.h"
#include "ImageProcess.h"
#include "TextureLibrary.h"
#include "ImageResizer.h"
#include "ImageCombiner.h"

class Lithoize : public ImageProcess {
public:
 Zint levels;
 Crayon lo,hi,replace; // MILO
 Zbool invert,weightedLum;
 Lithoize() : ImageProcess() {
  postfix=string(".lithoized");
  levels=4;
 }
 void Init() {
  invert ? replace.Int(255,255,255,255) : replace.Int(0,0,0,255);
  lo.Float( 0.0f,0.5f, 0.0f,1.0f);
  hi.Float(0.75f,1.0f,0.75f,1.0f);
 }
 void Loop(){
  // Acquire pixel
  Crayon pixel;
  Get(_x,_y,&pixel);
  // Eliminate "Green Screen" MILO
  if (
      pixel.r >= lo.r && pixel.r <= hi.r
   && pixel.g >= lo.g && pixel.g <= hi.g
   && pixel.b >= lo.b && pixel.b <= hi.b
   && pixel.a >= lo.a && pixel.a <= hi.a
   ) {
   bgra_out[0]=(byte)replace.r;
   bgra_out[1]=(byte)replace.g;
   bgra_out[2]=(byte)replace.b;
   bgra_out[3]=(byte)replace.a;
   return;
  }
  // Cheap Posterize
  pixel.Red(   (int)( (255.0/(double)levels)*(double)iround((float)((double)pixel.rf*(double)levels)) ) );
  pixel.Green( (int)( (255.0/(double)levels)*(double)iround((float)((double)pixel.gf*(double)levels)) ) );
  pixel.Blue(  (int)( (255.0/(double)levels)*(double)iround((float)((double)pixel.bf*(double)levels)) ) );
  // Convert to Luminance Map
  float lum=invert ? (weightedLum ? pixel.LuminanceWeighted() : pixel.Luminance())
                   : (weightedLum ? 1.0f-pixel.LuminanceWeighted() : 1.0f-pixel.Luminance());
  pixel.Float(lum,lum,lum,pixel.af);
  // Save in output pixel buffer
  bgra_out[0]=(byte)pixel.r;
  bgra_out[1]=(byte)pixel.g;
  bgra_out[2]=(byte)pixel.b;
  bgra_out[3]=(byte)pixel.a;
 }
};

extern Lithoize ip_Lithoize;

class LithophanePage {
public:
 VBOStaticVCLines normals;
 VBOStaticVC debugVbo;
 VBOStaticVNTTanBi vbo;
 Vertexd entireExtents,extents,position,delta;
 Coord2ui topLeftSample;
 Zdouble floor;
 BoxBool exterior;
 Indexed<LithoHeight> heights;
 Zpointer<STLFile> stl;
 // CCW: a=top left, b=top right, c=bottom right, d=bottom left
 void PushQuad( Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *d ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); /*B->Color(0.0,1.0,1.0,1.0);*/ B->ColorAbs(b->x/entireExtents.x/2.0,b->y/entireExtents.y/2.0,b->z/entireExtents.z/2.0,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  C=tri.points.Add(c->x,c->y,c->z); /*C->Color(1.0,0.0,1.0,1.0);*/ C->ColorAbs(c->x/entireExtents.x/2.0,c->y/entireExtents.y/2.0,c->z/entireExtents.z/2.0,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  A=tri.points.Add(a->x,a->y,a->z); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(a->x/entireExtents.x/2.0,a->y/entireExtents.y/2.0,a->z/entireExtents.z/2.0,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  debugVbo.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  tri.points.Clear();
  B=tri.points.Add(c->x,c->y,c->z); /*B->Color(0.0,0.0,1.0,1.0);*/ B->ColorAbs(b->x/entireExtents.x/2.0,b->y/entireExtents.y/2.0,b->z/entireExtents.z/2.0,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  C=tri.points.Add(d->x,d->y,d->z); /*C->Color(1.0,1.0,0.0,1.0);*/ C->ColorAbs(d->x/entireExtents.x/2.0,d->y/entireExtents.y/2.0,d->z/entireExtents.z/2.0,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  A=tri.points.Add(a->x,a->y,a->z); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(a->x/entireExtents.x/2.0,a->y/entireExtents.y/2.0,a->z/entireExtents.z/2.0,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  debugVbo.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
 }
 void PushQuad0z( Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *d ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,0.0); /*B->Color(0.0,1.0,1.0,1.0);*/ B->ColorAbs(0.0,0.0,0.0,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  C=tri.points.Add(c->x,c->y,0.0); /*C->Color(1.0,0.0,1.0,1.0);*/ C->ColorAbs(0.0,0.0,0.0,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  A=tri.points.Add(a->x,a->y,0.0); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(0.0,0.0,0.0,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  debugVbo.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  tri.points.Clear();
  B=tri.points.Add(c->x,c->y,0.0); /*B->Color(0.0,0.0,1.0,1.0);*/ B->ColorAbs(0.0,0.0,0.0,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  C=tri.points.Add(d->x,d->y,0.0); /*C->Color(1.0,1.0,0.0,1.0);*/ C->ColorAbs(0.0,0.0,0.0,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  A=tri.points.Add(a->x,a->y,0.0); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(0.0,0.0,0.0,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  debugVbo.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
 }
 void PushTri( Vertexd *a, Vertexd *b, Vertexd *c ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); /*B->Color(0.0,1.0,1.0,1.0);*/ B->ColorAbs(b->x/entireExtents.x/2.0,b->y/entireExtents.y/2.0,b->z/entireExtents.z/2.0,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  C=tri.points.Add(c->x,c->y,c->z); /*C->Color(1.0,0.0,1.0,1.0);*/ C->ColorAbs(c->x/entireExtents.x/2.0,c->y/entireExtents.y/2.0,c->z/entireExtents.z/2.0,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  A=tri.points.Add(a->x,a->y,a->z); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(a->x/entireExtents.x/2.0,a->y/entireExtents.y/2.0,a->z/entireExtents.z/2.0,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  debugVbo.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
 }
 void Export() {
  if ( normals.uploaded ) normals.Clear();
  //if ( vbo.uploaded ) vbo.Clear();
  if ( debugVbo.uploaded ) debugVbo.Clear();
  unsigned int i,j;
  Vertexd a,b,c,d;
  if ( heights.w != 0 && heights.h != 0 )
  for ( i=0; i<heights.w-1; i++ ) {
   for ( j=0; j<heights.h-1; j++ ) {
    a.Set(&heights.list[    i+j*heights.w].position);
    b.Set(&heights.list[(i+1)+j*heights.w].position);
    c.Set(&heights.list[(i+1)+(j+1)*heights.w].position);
    d.Set(&heights.list[    i+(j+1)*heights.w].position);
    PushQuad(&d,&c,&b,&a);
    a.z=0.0;
    b.z=0.0;
    c.z=0.0;
    d.z=0.0;
    PushQuad(&a,&b,&c,&d);
   }
  }
  // Leave VBO open, handled in Export of PagedLithotopography
 }
};

class PagedLithotopography {
public:
 Zbool underside;
 Zbool invert;
 Coord2ui samplesPerPage;
 Vertexd scale,extents,imageExtents,unitsPerSample,unitsPerPage;
 Zdouble floor,depth;
 Zpointer<GLImage> in;
 Coord2ui samples;
 Indexed<LithophanePage> pages;
 STLFile stl;
 PagedLithotopography() { underside=true; }
 PagedLithotopography( GLImage *in, Vertexd *imageScale, Coord2ui *samplesPerPage, Vertexd *unitExtents, double floor, bool invert, bool underside ) {
  this->underside=underside;
  this->invert=invert;
  this->in=in;
  this->samplesPerPage.Set(samplesPerPage->x<2?2:samplesPerPage->x,samplesPerPage->y<2?2:samplesPerPage->y);
  scale.Set(imageScale);
  extents.Set(unitExtents);
  depth=extents.z-floor;
  this->floor=floor;
  Paging();
  ProcessImage();
 }
 void Paging() {
  imageExtents.Set( scale.x * (double) in->width, scale.y * (double) in->height );
  double pw=imageExtents.x / (double) samplesPerPage.x;
  double ph=imageExtents.y / (double) samplesPerPage.y;
  Zint chopped=(int) pw;
  if ( pw > (double) chopped ) pw+=1.0;
  chopped=(int) ph;
  if ( ph > (double) chopped ) ph+=1.0;
  pages.Size( (size_t)pw, (size_t)ph );
  Vertexd lastPage((pw-(double)(int)pw),(ph-(double)(int)ph));
  Coord2ui lastPageSamples(
   (unsigned int)(lastPage.x * (double) samplesPerPage.x)+1,
   (unsigned int)(lastPage.y * (double) samplesPerPage.y)+1
  );
  if ( pages.w > 0 && pages.h > 0 ) {
   if ( pages.w == 1 ) {
    samples.x=lastPageSamples.x;
   } else {
    samples.x=(pages.w-1)*samplesPerPage.x+lastPageSamples.x;
   }
   if ( pages.h == 1 ) {
    samples.y=lastPageSamples.y;
   } else {
    samples.y=(pages.h-1)*samplesPerPage.y+lastPageSamples.y;
   }
  }
  unsigned int i,j;
  for ( i=0; i<pages.w; i++ ) {
   for ( j=0; j<pages.h; j++ ) {
    Coord2ui s(samplesPerPage.x,samplesPerPage.y);
    if ( i == pages.w-1 ) {
     s.x=lastPageSamples.x;
     if ( s.x < 2 ) s.x=2;
    }
    if ( j == pages.h-1 ) {
     s.y=lastPageSamples.y;
     if ( s.y < 2 ) s.y=2;
    }
    unsigned int v=i+j*pages.w;
    LithophanePage *page=&pages.list[v];
    page->stl=&stl;
    page->entireExtents.Set(&extents);
    page->heights.Size((size_t)s.x,(size_t)s.y);
    page->topLeftSample.Set(i*samplesPerPage.x,j*samplesPerPage.y);
    OUTPUT("Page %d style %dx%d (%d samples)\n",
     i+j*pages.w,
     page->heights.w, page->heights.h,
     page->heights.length
    );
   }
  }
  unitsPerSample.Set(
   extents.x/(double)(samples.x-1),
   extents.y/(double)(samples.y-1)
  );
  unitsPerPage.Set(unitsPerSample.x*(double)samplesPerPage.x,unitsPerSample.y*(double)samplesPerPage.y);
  OUTPUT("Samples: %dx%d Pages: %dx%d\nUnits per page: %1.4f,%1.4f\nunitsPerSample: %1.4f,%1.4f", 
   samples.x, samples.y, pages.w, pages.h,
   unitsPerPage.x,unitsPerPage.y,
   unitsPerSample.x,unitsPerSample.y
  );
 }
 void ProcessImage() {
  { // Single pass
   Crayon pixel;
   Vertexd image;
   image.x=(double)(in->width.value-1);
   image.y=(double)(in->height.value-1);
   unsigned int j,k,p,q;
   for ( p=0; p<pages.w; p++ ) {
    for ( q=0; q<pages.h; q++ ) {
     unsigned int v=p+q*pages.w;
     LithophanePage *page=&pages.list[v];
     page->position.Set((double)p*unitsPerPage.x,(double)q*unitsPerPage.y);
     page->extents.Set(unitsPerPage.x,unitsPerPage.y);
     page->delta.Set(unitsPerSample.x,unitsPerSample.y);
     page->exterior.Set(false,false,(p==0)?true:false,(p==pages.w-1)?true:false,(q==0)?true:false,(q==pages.h-1)?true:false);
     for ( j=0; j<page->heights.w; j++ ) {
      for ( k=0; k<page->heights.h; k++ ) {
       double xlerp=iratiod((int)(page->topLeftSample.x+j),(int)samples.x-1);
       double ylerp=iratiod((int)(page->topLeftSample.y+k),(int)samples.y-1);
       unsigned int x=CLAMP((unsigned int) ( xlerp*image.x ),(unsigned int) in->width-1);
       unsigned int y=CLAMP((unsigned int) ( (1.0-ylerp)*image.y ),(unsigned int) in->height-1);
       in->image->GetPixel(x,y,&pixel);
       LithoHeight *LH=&page->heights.list[j+k*page->heights.w];
       LH->luminance=invert ? 1.0-pixel.Luminance() : pixel.Luminance();
       LH->position.Set(
        xlerp*extents.x-extents.x*0.5,
        ylerp*extents.y-extents.y*0.5,
        depth*LH->luminance+floor
       );
      }
     }
    }
   }
  }
 }
 void Export() {
  unsigned int p,q;
  for ( p=0; p<pages.w; p++ ) {
   for ( q=0; q<pages.h; q++ ) {
    unsigned int v=p+q*pages.w;
    pages.list[v].Export();
   }
  }
  unsigned int i;
  Vertexd a,b,c,d;
  for ( p=0; p<pages.w; p++ ) {
   for ( q=0; q<pages.h; q++ ) {
    LithophanePage *Lp=&(pages.list[p+q*pages.w]);
    // If exterior, append sides
    if ( Lp->exterior.Left() ) {
     for ( i=0; i<Lp->heights.h-1; i++ ) {
      a.Set(&(Lp->heights(0,i).position));
      b.Set(&(Lp->heights(0,i+1).position));
      c.Set(&b); c.z=0.0;
      d.Set(&a); d.z=0.0;
      Lp->PushQuad(&d,&c,&b,&a);
     }
    }
    if ( Lp->exterior.Right() ) {
     for ( i=0; i<Lp->heights.h-1; i++ ) {
      a.Set(&(Lp->heights(Lp->heights.w-1,i).position));
      b.Set(&(Lp->heights(Lp->heights.w-1,i+1).position));
      c.Set(&b); c.z=0.0;
      d.Set(&a); d.z=0.0;
      Lp->PushQuad(&a,&b,&c,&d);
     }
    }
    if ( Lp->exterior.Front() ) {
     for ( i=0; i<Lp->heights.w-1; i++ ) {
      a.Set(&(Lp->heights(i,0).position));
      b.Set(&(Lp->heights(i+1,0).position));
      c.Set(&b); c.z=0.0;
      d.Set(&a); d.z=0.0;
      Lp->PushQuad(&a,&b,&c,&d);
     }
    }
    if ( Lp->exterior.Back() ) {
     for ( i=0; i<Lp->heights.w-1; i++ ) {
      a.Set(&(Lp->heights(i,Lp->heights.h-1).position));
      b.Set(&(Lp->heights(i+1,Lp->heights.h-1).position));
      c.Set(&b); c.z=0.0;
      d.Set(&a); d.z=0.0;
      Lp->PushQuad(&d,&c,&b,&a);
     }
    }
   }
  }
  // Stitching
  if ( pages.w > 1 && pages.h > 1 ) {
    LithophanePage *LpA, *LpB, *LpC, *LpD;
    for ( p=0; p<pages.w-1; p++ ) {
    for ( q=0; q<pages.h-1; q++ ) {
     LpA=&(pages.list[p+q*pages.w]);
     LpB=&(pages.list[p+1+q*pages.w]);    
     LpC=&(pages.list[(p+1)+(q+1)*pages.w]);
     LpD=&(pages.list[p+(q+1)*pages.w]);
     for ( i=0; i<LpA->heights.w-1; i++ ) {
      a.Set(&LpA->heights(i,LpA->heights.h-1).position);
      b.Set(&LpA->heights(i+1,LpA->heights.h-1).position);
      c.Set(&LpD->heights(i+1,0).position);
      d.Set(&LpD->heights(i,0).position);
      LpA->PushQuad(&d,&c,&b,&a);
      LpA->PushQuad0z(&a,&b,&c,&d);
     }
     for ( i=0; i<LpA->heights.h-1; i++ ) {
      d.Set(&LpB->heights(0,i).position);
      c.Set(&LpB->heights(0,i+1).position);
      b.Set(&LpA->heights(LpA->heights.w-1,i+1).position);
      a.Set(&LpA->heights(LpA->heights.w-1,i).position);
      LpA->PushQuad0z(&a,&d,&c,&b);
      LpA->PushQuad(&b,&c,&d,&a);
     }
     // Corner
     a.Set(&LpA->heights(LpA->heights.w-1,LpA->heights.h-1).position);
     b.Set(&LpB->heights(0,LpA->heights.h-1).position);
     c.Set(&LpC->heights(0,0).position);
     d.Set(&LpD->heights(LpD->heights.w-1,0).position);
     LpA->PushQuad(&d,&c,&b,&a);
     LpA->PushQuad0z(&a,&b,&c,&d);
    }    
   }
   unsigned int k;
   p=pages.w-1;
   for ( k=0; k<pages.h-1; k++ ) { 
    LpA=&(pages.list[p+k*pages.w]);
    LpB=&(pages.list[p+1+k*pages.w]);    
    LpC=&(pages.list[(p+1)+(k+1)*pages.w]);
    LpD=&(pages.list[p+(k+1)*pages.w]);
    for ( i=0; i<LpA->heights.w-1; i++ ) {// Right edge
     a.Set(&LpA->heights(i,LpA->heights.h-1).position);
     b.Set(&LpA->heights(i+1,LpA->heights.h-1).position);
     c.Set(&LpD->heights(i+1,0).position);
     d.Set(&LpD->heights(i,0).position);
     LpA->PushQuad(&d,&c,&b,&a);
     LpA->PushQuad0z(&a,&b,&c,&d);
    }
    if ( LpD->exterior.Right() ) {
     a.Set(&LpA->heights(LpA->heights.w-1,LpA->heights.h-1).position);
     b.Set(&LpD->heights(LpD->heights.w-1,0).position);
     c.Set(&b); c.z=0.0;
     d.Set(&a); d.z=0.0;
     LpA->PushQuad(&a,&b,&c,&d);
    }
   }
   p=0;
   for ( k=0; k<pages.h-1; k++ ) { // Lefts
    LpA=&(pages.list[p+k*pages.w]);
    LpD=&(pages.list[p+(k+1)*pages.w]);
    a.Set(&LpA->heights(0,LpA->heights.h-1).position);
    b.Set(&LpD->heights(0,0).position);
    c.Set(&b); c.z=0.0;
    d.Set(&a); d.z=0.0;
    LpA->PushQuad(&d,&c,&b,&a);
   }
   q=pages.h-1;
   for ( k=0; k<pages.w-1; k++ ) {
    LpA=&(pages.list[k+q*pages.w]);
    LpB=&(pages.list[k+1+q*pages.w]);    
    LpC=&(pages.list[(k+1)+(q+1)*pages.w]);
    LpD=&(pages.list[k+(q+1)*pages.w]);
    for ( i=0; i<LpA->heights.h-1; i++ ) {
     d.Set(&LpB->heights(0,i).position);
     c.Set(&LpB->heights(0,i+1).position);
     b.Set(&LpA->heights(LpA->heights.w-1,i+1).position);
     a.Set(&LpA->heights(LpA->heights.w-1,i).position);
     LpA->PushQuad(&b,&c,&d,&a);
     LpA->PushQuad0z(&a,&d,&c,&b);
    }
    if ( LpA->exterior.Back() ) {
     a.Set(&LpA->heights(LpA->heights.w-1,LpA->heights.h-1).position);
     b.Set(&LpB->heights(0,LpB->heights.h-1).position);
     c.Set(&b); c.z=0.0;
     d.Set(&a); d.z=0.0;
     LpA->PushQuad(&d,&c,&b,&a);
    }
   }   
   for ( k=0; k<pages.w-1; k++ ) { // Fronts
    LpA=&(pages.list[k]);
    LpD=&(pages.list[k+1]);
    a.Set(&LpA->heights(LpA->heights.w-1,0).position);
    b.Set(&LpD->heights(0,0).position);
    c.Set(&b); c.z=0.0;
    d.Set(&a); d.z=0.0;
    LpA->PushQuad(&a,&b,&c,&d);
   }
  }
  // Close all VBOs and discard CPU data
  for ( p=0; p<pages.w; p++ ) {
   for ( q=0; q<pages.h; q++ ) {
    LithophanePage *Lp=&(pages.list[p+q*pages.w]);
    Lp->debugVbo.Create();
    Lp->debugVbo.ClearCPU();
    Lp->normals.Create();
    Lp->normals.ClearCPU();
   }
  }
 }
 void Render() {
  unsigned int p,q;
  for ( p=0; p<pages.w; p++ ) {
   for ( q=0; q<pages.h; q++ ) {
    unsigned int v=p+q*pages.w;
    normalmappingShader.vbo=&pages.list[v].vbo;//&vboQuadVNTTanBi;//&vbo;
    normalmappingShader.UpdateUniforms();
    normalmappingShader.Disable();
   }
  }
 }
 void RenderWireframe() {
  unsigned int p,q;
  for ( p=0; p<pages.w; p++ ) {
   for ( q=0; q<pages.h; q++ ) {
    unsigned int v=p+q*pages.w;
    pages.list[v].debugVbo.Render();
    pages.list[v].normals.Render();
   }
  }
 }
};

class PagedLithophane : public ListItem {
public:
 Zbool underside,invert;
 Zpointer<GLImage> image;
 Vertexd imageScale;
 Zdisposable<PagedLithotopography> topography;
 Vertexd size;
 Zdouble floor;
 Coord2ui samplesPerPage;
 PagedLithophane() {}
 PagedLithophane( const char *filename, double imagex, double imagey, unsigned int samplesW, unsigned int samplesH, double unitW, double unitH, double total_thickness, double etch_floor, bool invert=true, bool underside=true ) {
  Load(filename);
  this->underside=underside;
  this->invert=true;
  imageScale.Set(imagex,imagey);
  SetResolution(samplesW,samplesH,unitW,unitH,total_thickness,etch_floor);
  Survey();
 }
 PagedLithophane( GLImage *i, double imagex, double imagey, unsigned int samplesW, unsigned int samplesH, double unitW, double unitH, double total_thickness, double etch_floor, bool invert=true, bool underside=true ) {
  image=i;
  this->underside=underside;
  this->invert=true;
  imageScale.Set(imagex,imagey);
  SetResolution(samplesW,samplesH,unitW,unitH,total_thickness,etch_floor);
  Survey();
 }
 void SetResolution( unsigned int sx, unsigned int sy, double w, double h, double d, double lowz ) {
  samplesPerPage.Set(sx,sy);
  size.Set(w,h,d);
  floor=lowz;
 }
 void Load( const char *filename ) {
  image = new GLImage(filename);
 }
 void Survey() {
  topography.Deinstantiate();
  topography.Recycle(new PagedLithotopography(image.pointer,&imageScale,&samplesPerPage,&size,floor.value,invert.value,underside.value));
  topography->Export();
 }
 void Render() { topography->Render(); }
 void RenderWireframe() { topography->RenderWireframe(); }
};