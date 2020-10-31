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
#include "VBO.h"
#include "Art.h"
#include "GLWindow.h"

/*
  glTexCoord2i(0,0); glVertex2i(-5,-5);
  glTexCoord2i(0,1); glVertex2i(5,-5);
  glTexCoord2i(1,1); glVertex2i(5,5);
  glTexCoord2i(1,0); glVertex2i(-5,5);
 */

// Generates and maintains a VBO of grid points with a radial variation (to fade edges, etc)
struct QuadGridPoint {
public:
 Cartesiand position;
 Cartesiand tc;
 Crayon color;
 QuadGridPoint() {
  color.Pick(alabaster);
 }
 void Set( Vertex *v ) {
  v->x=(GLfloat) position.x;
  v->y=(GLfloat) position.y;
  v->q=(GLfloat) tc.x;
  v->r=(GLfloat) tc.y;
  v->rgba[0]=(GLfloat) color.rf;
  v->rgba[1]=(GLfloat) color.gf;
  v->rgba[2]=(GLfloat) color.bf;
  v->rgba[3]=(GLfloat) color.af;
 }
};

class QuadGridVBO : public ListItem {
public:
 Zstring name;
 Cartesiand center;
 Indexed<QuadGridPoint> grid;
 VBOStaticVNTC vbo;
 Zbool doNotOptimize;
 QuadGridVBO( int xEven, int yEven, Crayon color, VBOStaticVNTTanBi *target ) {
  Generate(xEven,yEven);
  for ( int i=0; i<xEven; i++ ) for ( int j=0; j<yEven; j++ ) {
   grid(i,j).color.fromCrayon(color);
   grid(i,j).tc.Set(iratiod(i,xEven-1),iratiod(j,yEven-1));
   grid(i,j).position.Set(iratiod(i,xEven-1),iratiod(j,yEven-1));
  }
  Upload(target);
 }
 QuadGridVBO( int xEven, int yEven ) {
  Generate(xEven,yEven);
  for ( int i=0; i<xEven; i++ ) for ( int j=0; j<yEven; j++ ) {
   if ( i==0 || i==(xEven-1) || j==0 || j==(yEven-1) ) grid(i,j).color.Alpha(0.0f);
   else grid(i,j).color.Alpha(1.0f);
  }
  Upload();
 }
 QuadGridVBO( const char *filename ) {
  Load(filename);
  Upload();
 }
 QuadGridVBO( const char *filename, double loX, double loY, double dX, double dY ) {
  Load(filename);
  Position(loX,loY,dX/(double)grid.w,dY/(double)grid.h);
  Upload();
 }
 QuadGridVBO( const char *filename, string name ) {
  Load(filename);
  Upload();
  this->name=name;
 }
 QuadGridVBO( const char *filename, string name, double loX, double loY, double dX, double dY ) {
  Load(filename);
  Position(loX,loY,dX/(double)grid.w,dY/(double)grid.h);
  Upload();
  this->name=name;
 }
 void Load( const char *filename ) {
  name=string(filename);
  byte *pixels;
  int pixelSize,bytesSize;
  int width, height;

#if defined(USE_CORONA)
  corona::Image *image;
  corona::PixelFormat format;
  try {
   image=corona::OpenImage(filename, corona::PF_B8G8R8A8);
  } catch (bad_alloc& ba) {
   OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
  }
  if ( !image ) { OUTPUT("Image file '%s' missing\n",filename); return; }
  width=image->getWidth();
  height=image->getHeight();
  pixels=(byte*)image->getPixels();
  format=image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  CPUImage *image=imageLoader.Load(filename);
  if ( !image ) {
   OUTPUT("QuadGridVBO: Image file '%s' missing\n",filename);
  } else if ( image->error ) {
   OUTPUT("QuadGridVBO: Image file '%s' was corrupt, or an unsupported format.\n");
  }
  if ( ( pixels=image->getPixels() ) == null ) {
   OUTPUT("QuadGridVBO: Image was blank (null pixels)\n");
  }
  width=image->getWidth();
  height=image->getHeight();
  pixels=image->getPixels();
  pixelSize=4;
  bytesSize=width*height*pixelSize;
  Generate(width,height);
  Position(0.0,0.0,iratiod(1,width),iratiod(1,height));
#endif
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   grid(x,y).color.Int((int)bgra[2],(int)bgra[1],(int)bgra[0],(int)bgra[3]);
  }
  delete image;
 }
 QuadGridVBO( int xEven, int yEven, double R ) { // Not yet completed, will let you set a circle
  Generate(xEven,yEven);
  center.Set( (double)xEven/2.0, (double)yEven/2.0 );
  for ( int i=0; i<xEven; i++ ) for ( int j=0; j<yEven; j++ ) {
  }
  Upload();
 }
 void Generate( int xEven, int yEven ) {
  if ( xEven<0 || xEven%2!=0 || yEven<0 || yEven%2!=0 ) {
   OUTPUT("QuadGridVBO:Generate() warns X or Y size was not a positive even number.\n");
  }
  grid.Size((size_t) xEven, (size_t) yEven);
 }
 void Position( double loX, double loY, double Xdelta, double Ydelta ) {
  double X=0,Y=0,tcX=0.0,tcY=0.0;
  double tcXDelta=iratiod(1,(int)grid.w);
  double tcYDelta=iratiod(1,(int)grid.h);
  X=loX;
  for ( int i=0; i<(int)grid.w; i++ ) {
   Y=loY;
   tcY=1.0;
   for ( int j=0; j<(int)grid.h; j++ ) {
    grid(i,j).position.Set(X,Y);
    grid(i,j).tc.Set(tcX,tcY);
    Y+=Ydelta;
    tcY-=tcYDelta;
   }
   X+=Xdelta;
   tcX+=tcXDelta;
  }
 }
 void SwapTexCoords( bool LeftRight, bool TopBottom ) {
  for ( int i=0; i<(int)grid.w; i++ ) {
   for ( int j=0; j<(int)grid.h; j++ ) {
    double tcX=grid(i,j).tc.x;
    double tcY=grid(i,j).tc.y;
    grid(i,j).tc.Set( LeftRight ? 1.0-tcX : tcX, TopBottom ? 1.0-tcY : tcY );
   }
  }
 }
 void Upload( VBOStaticVNTTanBi *vbovnttb ) {
  vbovnttb->Clear();
  Vertex a,b,c;
  int iend=(int)(grid.w);
  int jend=(int)(grid.h);
  for ( int i=0; i<iend-1; i+=1 ) for ( int j=0; j<jend-1; j+=1 ) {
   if ( !(grid(i,j).color.af == 0.0f
      && grid(i,j+1).color.af == 0.0f
      && grid(i+1,j).color.af == 0.0f) || doNotOptimize ) {
    grid(i,j).Set(&a);
    grid(i,j+1).Set(&b);
    grid(i+1,j).Set(&c);
    a.normal.Recycle(a.TriNormal(&a,&b,&c));
    a.TangentBasis(&b,&c);
    b.normal.Recycle(b.TriNormal(&a,&b,&c));
    b.TangentBasis(&a,&c);
    c.normal.Recycle(c.TriNormal(&a,&b,&c));
    c.TangentBasis(&a,&b);
    vbovnttb->PushTriangle(&a,&b,&c);
   }
   if ( !(grid(i+1,j).color.af == 0.0f
       && grid(i,j+1).color.af == 0.0f
       && grid(i+1,j+1).color.af == 0.0f) || doNotOptimize ) {    
    grid(i+1,j).Set(&a);
    grid(i,j+1).Set(&b);
    grid(i+1,j+1).Set(&c);
    a.normal.Recycle(a.TriNormal(&a,&b,&c));
    a.TangentBasis(&b,&c);
    b.normal.Recycle(b.TriNormal(&a,&b,&c));
    b.TangentBasis(&a,&c);
    c.normal.Recycle(c.TriNormal(&a,&b,&c));
    c.TangentBasis(&a,&b);
    vbovnttb->PushTriangle(&a,&b,&c);
   }
  }
  vbovnttb->Create();
 }
 void Upload() {
  vbo.Clear();
  Vertex a,b,c;
  int iend=(int)(grid.w);
  int jend=(int)(grid.h);
  for ( int i=0; i<iend-1; i+=1 ) for ( int j=0; j<jend-1; j+=1 ) {
   if ( !(grid(i,j).color.af == 0.0f
      && grid(i,j+1).color.af == 0.0f
      && grid(i+1,j).color.af == 0.0f) || doNotOptimize ) {
    grid(i,j).Set(&a);
    grid(i,j+1).Set(&b);
    grid(i+1,j).Set(&c);
    vbo.PushTriangle(&a,&b,&c);
   }
   if ( !(grid(i+1,j).color.af == 0.0f
       && grid(i,j+1).color.af == 0.0f
       && grid(i+1,j+1).color.af == 0.0f) || doNotOptimize ) {    
    grid(i+1,j).Set(&a);
    grid(i,j+1).Set(&b);
    grid(i+1,j+1).Set(&c);
    vbo.PushTriangle(&a,&b,&c);
   }
  }
  vbo.Create();
 }
};

class QuadGridVBOs : public LinkedList {
public:
 QuadGridVBO *find( string match ) {
  FOREACH(QuadGridVBO,q) if ( q->name == match ) return q;
  OUTPUT("QuadGridVBOs:find(`%s`) did not match any grids.\n", match.c_str() );
  return null;
 }
 void Add( const char *filename ) {
  Append(new QuadGridVBO(filename));
 }
 void Add( const char *filename, double loX, double loY, double dX, double dY ) {
  Append(new QuadGridVBO(filename,loX,loY,dX,dY));
 }
 void Add( const char *filename, const char *name ) {
  Append(new QuadGridVBO(filename,string(name)));
 }
 void Add( const char *filename, const char *name, double loX, double loY, double dX, double dY ) {
  Append(new QuadGridVBO(filename,string(name),loX,loY,dX,dY));
 }
 CLEARLISTRESET(QuadGridVBO);
};

extern QuadGridVBOs quadGrids;

void InitQuadGrids();