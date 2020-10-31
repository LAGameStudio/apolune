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
#include "moremath.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "CubeMappedSphere.h"
#include "FBO.h"
#include "LookAtPerspective.h"
#include "UglyFont.h"
#include "Indexed.h"
#include "ZeroTypes.h"

#define EIGHTH   0.125
#define QUARTER  0.25
#define HALF     0.5
#define THREEQ   0.75
#define CIRCLE   1.0

#define SPHEREANIMATOR_FACEWIDTHPIXELS 512
#define SPHEREANIMATOR_FACEWIDTHHALF SPHEREANIMATOR_FACEWIDTHPIXELS/2
#define SPHEREANIMATOR_CIRCUMFERENCE SPHEREANIMATOR_FACEWIDTHPIXELS*4

class QuadCubeFBOs {
public:
 FBOColor F,L,B,R,D,U;
 int width,height;
 bool created;
 QuadCubeFBOs() {
  width=512;
  height=512;
  created=false;
 }
 void Create() { Create(512); }
 void Create( int wh ) {
  width=height=wh;
  if ( created ) {
   F.Release();
   L.Release();
   B.Release();
   R.Release();
   D.Release();
   U.Release();
  }
  F.background.Pick(orange);
  F.Create(wh,wh);
  L.background.Pick(magenta);
  L.Create(wh,wh);
  B.background.Pick(electricBlue);
  B.Create(wh,wh);
  R.background.Pick(oliveDrab);
  R.Create(wh,wh);
  D.background.Pick(yellow);
  D.Create(wh,wh);
  U.background.Pick(red255);
  U.Create(wh,wh);
 }
};

extern QuadCubeFBOs quadcubefbos512r,quadcubefbos512,quadcubefbos256;

/*
 * Similar to CompositeImages, this class holds a single full-side texture set,
 * programmed with rendering information.
 */
class SphereTextureDefinition : public ListItem {
public:
 Blends blendHint;
 Indexed<Zpointer<GLImage>>textures;
 Indexed<Zstring> files;
 int loaded;
 SphereTextureDefinition() : ListItem() {
  blendHint=none;
  textures.Size(6);
  files.Size(6);
  loaded=0;
 }
 //  F R B L U D - not sure what the problem is
 SphereTextureDefinition( Blends blend, char *f, char *r, char *b, char *l, char *u, char *d, bool altLoader ) : ListItem() {
  blendHint=blend;
  textures.Size(6);
  files.Size(6);
  loaded=false;
  files[0]=string(f);
  files[1]=string(r);
  files[2]=string(b);
  files[3]=string(l);
  files[4]=string(u);
  files[5]=string(d);
 }
 // F L B R D U (legacy)
 SphereTextureDefinition( Blends blend, char *f, char *l, char *b, char *r, char *d, char *u ) : ListItem() {
  blendHint=blend;
  textures.Size(6);
  files.Size(6);
  loaded=false;
  files[0]=string(f);
  files[1]=string(l);
  files[2]=string(b);
  files[3]=string(r);
  files[4]=string(d);
  files[5]=string(u);
 }
 void Load() {
  textures[0]=library.Load((const char *) files[0]);
  textures[1]=library.Load((const char *) files[1]);
  textures[2]=library.Load((const char *) files[2]);
  textures[3]=library.Load((const char *) files[3]);
  textures[4]=library.Load((const char *) files[4]);
  textures[5]=library.Load((const char *) files[5]);
  loaded++;
 }
 void Unload() {
  loaded--;
//  if ( loaded==0 ) {
//   for ( int i=0; i<6; i++ ) library.Delete(textures[i]);
//   textures[0]=textures[1]=textures[2]=textures[3]=textures[4]=textures[5]=null;
//  }
 }
 bool matches( const char *partial ) {
  for ( int i=0; i<6; i++ ) if ( textures[i] && textures[i]->matches(partial) ) return true;
  return false;
 }
 ~SphereTextureDefinition() {
  if ( loaded>0 ) Unload();
 }
};

class SphereTextureDefinitions : public LinkedList {
 CLEARLISTRESET(SphereTextureDefinition);
 ~SphereTextureDefinitions() { Clear(); }
};

NAMED(
 SphereTextureDefinitions,
 SphereTextureDefinitionsHandle,
 SphereTextureDefinitionsCollection
);

class SearchableSphereTextureDefinitionsCollection : public SphereTextureDefinitionsCollection {
public:
 SphereTextureDefinitions *findCollection( const char *match ) {
  FOREACH(SphereTextureDefinitionsHandle,stdh)
   if ( !str_infix(match, stdh->name.c_str()) ) return stdh->p;
  return null;
 }
 SphereTextureDefinitionsHandle *Add( const char *collection ) {
  SphereTextureDefinitionsHandle *stdh=new SphereTextureDefinitionsHandle;
  SphereTextureDefinitions *std=new SphereTextureDefinitions;
  stdh->p=std;
  stdh->name=string(collection);
  Append(stdh);
  return stdh;
 }
 void Add( const char *collection, SphereTextureDefinition *d ) {
  SphereTextureDefinitions *std=findCollection(collection);
  if ( !std ) {
   SphereTextureDefinitionsHandle *stdc=this->Add(collection);
   std=stdc->p;
  }
  std->Append(d);
 }
 SphereTextureDefinition *findByFilenamePartial( const char *match ) {
  FOREACH(SphereTextureDefinitionsHandle,stdh) {
   EACH(stdh->p->first,SphereTextureDefinition,std) {
    for ( int i=0; i<6; i++ )
     if ( !str_infix(match,(const char *) (std->files[i]) ) ) return std;
   }
  }
  OUTPUT("SphereTextureDefinitions: No match to keyword `%s`\n", match );
  return null;
 }
};

extern SearchableSphereTextureDefinitionsCollection sphereTextureLibrary;
extern SearchableSphereTextureDefinitionsCollection solsystemSphereTextureLibrary;

class SphereTexturePrinter : public ListItem {
public:
 SphereTextureDefinition *d;
 Blends blend;
 Crayon tint;
 SphereTexturePrinter( SphereTextureDefinition *d ) : ListItem() {
  blend=none;
  tint.Pick(alabaster);
  this->d=d;
  if ( d->loaded == 0 ) d->Load();
 }
 SphereTexturePrinter( Blends b, Crayon t, SphereTextureDefinition *d ) : ListItem() {
  blend=b;
  tint.fromCrayon(t);
  this->d=d;
  if ( d->loaded == 0 ) d->Load();
 }
 void Render( int face, int w ) {
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, d->textures[face]->texture);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(0,   0);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(w, 0);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(w, w);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,   w);
  glEnd();
  glDisable(GL_TEXTURE_2D);
 }
 virtual void Between() {
 }
};

class SphereTexturePrinters : public LinkedList {
public:
 int w;
 SphereTexturePrinters() : LinkedList() {
  w=512;
 }
 SphereTexturePrinter *Printing( const char *match ) {
  FOREACH(SphereTexturePrinter,p) {
   if ( !str_infix(match,(const char *)p->d->files[0]) ) return p;
  }
  return null;
 }
 // Also loads the textures if they are unloaded
 void Add( Blends blend, Crayon tint, SphereTextureDefinition *d ) {
  // Do not allow the same texture to be used multiple times.  This is to combat oversaturated planets.
  FOREACH(SphereTexturePrinter,p) {
   if ( d==p->d ) {
    OUTPUT("SphereTexturePrinters: Discarding duplicate SphereTextureDefinition.\n");
    return;
   }
  }
  if ( tint.af != 1.0f ) tint.Float(tint.rf,tint.gf,tint.bf,1.0f);
  while ( tint.rf+tint.gf+tint.bf < 1.0f ) tint.Any();
  if ( tint.af != 1.0f ) tint.Float(tint.rf,tint.gf,tint.bf,1.0f);
  if ( d->loaded < 1 ) d->Load();
  Append(new SphereTexturePrinter( blend, tint, d ));
 }
 void Render( int face ) {
  FOREACH(SphereTexturePrinter,s) s->Render(face,w);
 }
 void Between() {
  FOREACH(SphereTexturePrinter,s) s->Between();
 }
 CLEARLISTRESET(SphereTexturePrinter);
};

HANDLED(
 SphereTexturePrinter,
 SphereTexturePrinterHandle,
 SphereTexturePrinterHandles,
 SphereTexturePrinterHandlesHandle,
 SphereTexturePrinterHandlesHandles
);

/*
 * In an attempt to thwart top and bottom placement distortions, we are using
 * an approximated position using the average of the following two values.
 *
 * A better way to approach this problem would be to perhaps use the cube map's
 * texture coordinates somehow to smudge the position instead?
 */
#define DOMECIRCLELONGRADIUS (isqrt((SPHEREANIMATOR_FACEWIDTHPIXELS)*(SPHEREANIMATOR_FACEWIDTHPIXELS)*2)/2.0f)
#define DOMECIRCLESHORTRADIUS SPHEREANIMATOR_FACEWIDTHHALF

class SphereStroke; // forward declaration
class SpherePlacement : public ListItem {
public:
 int face;
 Cartesian xywh;
 SphereStroke *stroke;
 SpherePlacement() : ListItem() {
  stroke=null;
  face=0;
 }
 void Render();
};

class SpherePlacements : public LinkedList {
public:
 void Render() {
  FOREACH(SpherePlacement,p) p->Render();
 }
 CLEARLISTRESET(SpherePlacement);
};

class SphereStroke : public ListItem {
public:
 int w,circumference;
 Indexed<Zpointer<SpherePlacements>> placements;
 Vertexd polar1; // "center" to draw on, normalized 1.0
 Crayon tint;
 GLImage *image;
 Blends blend;
 Vertexd topLeft,bottomRight,topRight,bottomLeft;
 int topLeftCorner, topRightCorner, bottomLeftCorner, bottomRightCorner; // on this->WhichFace() does each corner fall?
 double width,height;
 SphereStroke() : ListItem() {
  placements.Size(6);
  blend=none;
  image=null;
  w=512;
  circumference=512*4;
  topLeftCorner=topRightCorner=bottomLeftCorner=bottomRightCorner=0;
  width=height=512.0;
 }
 // Must be in 1.0 space, returns 0=F,1=L,2=B,3=R,4=D,5=U
 // X,Y is normalized to 1, where 1 Y goes up over the top pole from the x position along the equator
 int WhichFace( Vertexd *v ) {
  // Are we on one of the sides?
  if ( v->y < EIGHTH || v->y >= CIRCLE-EIGHTH ) {
   /* F */ if ( v->x < QUARTER ) return 0;
   /* L */ if ( v->x < HALF ) return 1;
   /* B */ if ( v->x < THREEQ ) return 2;
   /* R */ return 3;
  } // Otherwise, it's top or bottom depending on y value.
  if ( v->y >= HALF+EIGHTH ) return 4; /* D */
  return 5; /* U */
 }
 // Figure out what pixel this is in FBO space for placement of a texture quad on the FBO.
 // Incoming: target face, x, y, x2, y2 in "fbo space" (From F left side, x=0, goes around to right, Y goes up over the top pole starting at 0 at equator
 void FindFBO_XY( int face, Cartesian *c ) {
  double
   x=this->polar1.x,
   y=this->polar1.y,
   x2=this->polar1.x+width,
   y2=this->polar1.y+height;
  // Various face-specific translations.
  switch ( face ) {
   /* U */ case 5:
     x= (x- THREEQ);
     y= (y- THREEQ);
     x2=(x2-THREEQ);
     y2=(y2-THREEQ);
    break;
   /* D */ case 4:
     x= (x- THREEQ);
     y= (y- THREEQ);
     x2=(x2-THREEQ);
     y2=(y2-THREEQ);
    break;
   /* R */ case 3:
     x= (x- THREEQ);
     x2=(x2-THREEQ);
     y= (y < 0 ? y+EIGHTH : y);
     y2=(y2>=0 ? y2+EIGHTH : y2);
    break;
   /* L */ case 1:
     x= (x- QUARTER);
     x2=(x2-QUARTER);
     y= (y < 0 ? y+EIGHTH : y);
     y2=(y2>=0 ? y2+EIGHTH : y2);
    break;
   /* B */ case 2:
     x= (x- HALF);
     x2=(x2-HALF);
     y= (y < 0 ? y+EIGHTH : y);
     y2=(y2>=0 ? y2+EIGHTH : y2);
    break;
   /* F */ case 0:
     y= (y < 0 ? y+EIGHTH : y);
     y2=(y2>=0 ? y2+EIGHTH : y2);
    break;
  }
  c->x=(int)(x*circumference);
  c->y=(int)(y*circumference);
  c->x2=(int)(x2*circumference);
  c->y2=(int)(y2*circumference);
 }
 // Internally used
 void Place( 
   Vertexd polar, Vertexd extents, // <- both are normalized 1.0
   SpherePlacements *F,
   SpherePlacements *L,
   SpherePlacements *B,
   SpherePlacements *R,
   SpherePlacements *D,
   SpherePlacements *U 
  ) {
  placements[0]=F;
  placements[1]=L;
  placements[2]=B;
  placements[3]=R;
  placements[4]=D;
  placements[5]=U;
  width=extents.x;
  height=extents.y;
  double w2=width/2,h2=height/2;
  while( polar.x > 1.0 ) polar.x-=1.0; while( polar.x < 0 ) polar.x+=1.0;
  while( polar.y > 1.0 ) polar.y-=1.0; while( polar.y < 0 ) polar.y+=1.0;
        topLeft.Set( (polar.x), (polar.y), 0.0 ); // no Zs
    bottomRight.Set( (polar.x+width), (polar.y+height), 0.0 );
  topRight.Set(bottomRight.x,topLeft.y,0.0);
  bottomLeft.Set(topLeft.x,bottomRight.y,0.0);
  // calculate placement
  this->polar1.Set( polar.x, polar.y, 0.0 ); // store for later revue
  topLeftCorner=WhichFace(&topLeft);
  topRightCorner=WhichFace(&topRight);
  bottomLeftCorner=WhichFace(&bottomLeft);
  bottomRightCorner=WhichFace(&bottomRight);
  // All on same face?  If not, how many?
  Numbers cornerFaces;
  cornerFaces.AddIfUnique(topLeftCorner);
  cornerFaces.AddIfUnique(topRightCorner);
  cornerFaces.AddIfUnique(bottomLeftCorner);
  cornerFaces.AddIfUnique(bottomRightCorner);
  // All on same face
  EACH(cornerFaces.first,Number,n) {
   Number *face=(Number *) n;
   Cartesian faceXY;
   SpherePlacement *p=new SpherePlacement;
   p->face=face->i;
   p->stroke=this;
   FindFBO_XY(p->face,&p->xywh);
   placements[p->face]->Append(p);
  }
 }
 // Animation etc
 virtual void Between() {
 }
};

/*
 * A list of strokes.
 */
class SphereStrokes : public LinkedList {
public:
 int w,circumference;
 Indexed<SpherePlacements> placements;
 QuadCubeFBOs *fbos;
 SphereTexturePrinters layers;
 Blends blend;
 Crayon tint;
 SphereStrokes() : LinkedList() {
  placements.Size(6);
  blend=none;
  tint.Pick(alabaster);
  SwitchTo512();
  fbos=null;
 }
 // Does not recalculate stroke placements
 void SwitchTo512r() {
  w=512;
  circumference=512*4;
 }
 void SwitchTo512() {
  w=512;
  circumference=512*4;
 }
 void SwitchTo256() {
  w=256;
  circumference=256*4;
 }
 // Searches for a particular stroke
 SphereStroke *Placed( const char *match ) {
  FOREACH(SphereStroke,s) if ( !str_infix(match,s->image->filename.c_str()) ) {
   return s;
  }
  return null;
 }
 SphereTexturePrinter *Printing( const char *match ) {
  return layers.Printing(match);
 }
 void Place( Vertexd sxy, Vertexd wh, Crayon t, GLImage *i, Blends b ) {
  SphereStroke *s=new SphereStroke;
  Append(s);
  s->image=i;
  s->blend=b;
  s->tint.fromCrayon(t);
  s->Place(sxy,wh,
   &(placements[0]),
   &(placements[1]),
   &(placements[2]),
   &(placements[3]),
   &(placements[4]),
   &(placements[5])
  );
 }
 void Place( SphereStroke *s, Vertexd sxy, Vertexd wh ) {
  Append(s);
  s->Place(sxy,wh,
   &(placements[0]),
   &(placements[1]),
   &(placements[2]),
   &(placements[3]),
   &(placements[4]),
   &(placements[5])
  );
 }
 // Add pre steps to a face (or all faces).
 virtual void PreProcessing( int face ) {
 }
 // Add post steps to a face (or all faces).
 virtual void PostProcessing( int face ) {
 }
 void RenderFace(FBOColor *fbo, int f) {
  fbo->background.Pick(black);
  fbo->Bind();
  PreProcessing(f);
  layers.Render(f);
  EACH(placements[f].first,SpherePlacement,s) s->Render();
  PostProcessing(f);
  fbo->Unbind();
 }
 // Renders to the FBO set
 void Paint() {
  RenderFace(&(fbos->F),0);
  RenderFace(&(fbos->L),1);
  RenderFace(&(fbos->B),2);
  RenderFace(&(fbos->R),3);
  RenderFace(&(fbos->D),4);
  RenderFace(&(fbos->U),5);
 }
 CLEARLISTRESET(SphereStroke);
 ~SphereStrokes() { Clear(); }
};

/*
 * Each one of these uses a great deal of FBOs.  Recycle if possible.
 * If you have more than 100, your engine will crash.  Stick to < 20 for good results.
 */
class SpherePainter : public ListItem {
public:
 QuadCubeFBOs fbos;
 SphereStrokes strokes;
 LookAtPerspective lap;
 Vertexd scale,rotation,position;
 ProfiledCubeMappedSphere3d *sphere;
 Crayon tint;
 Blends blend;
 bool coordinator;
 SpherePainter() {
  coordinator=false;
  lap.Near=0.0001;
  lap.Far=20.0;
  sphere = (ProfiledCubeMappedSphere3d *) sphereModelsHiRes.first;
  scale.Set(2.0,2.0,2.0);
  position.Set(0.0,0.0,0.0);
  lap.eye.Set(0,0,-8.12); //-8.35); // calculated for EITS
  lap.center.Set(0.0,0.0,0.0);
  blend=none;
  strokes.fbos=&fbos;
  tint.Pick(alabaster);
 }
 void SwitchTo512r() {
  fbos.Create(512);
  strokes.SwitchTo512r();
 }
 void SwitchTo512() {
  fbos.Create(512);
  strokes.SwitchTo512();
 }
 void SwitchTo256() {
  fbos.Create(256);
  strokes.SwitchTo256();
 }
 void TestPattern() {
  strokes.layers.Add(
   none,
   crayons.Pick(alabaster),
   sphereTextureLibrary.findByFilenamePartial("dust2")
  );
  Paint();
 }
 void TestStrokes() {
  Vertexd sxy,wh;
  Crayon t; t.Pick(alabaster);
  for ( double yy =0.0; yy<1.0; yy+= 0.1 ) {
   sxy.Set(yy,0.0);
   wh.Set(0.1,0.1);
   strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),additive);
  }
  for ( double yy =0.0; yy<1.0; yy+= 0.05 ) {
   sxy.Set(yy,EIGHTH/2);
   wh.Set(0.02,0.01);
   strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),none);
  }
  // The following strokes are all seam or dome testers:
  sxy.Set(-0.0125,EIGHTH-0.025);
  wh.Set(0.5,0.1);
  t.Pick(red255);
  strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),none);
  t.Pick(green);
  sxy.Set(0.9,0.9);
  wh.Set(0.2,0.2);
  strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),none);
  t.Pick(blue);
  sxy.Set(0.5,0.5);
  wh.Set(0.5,0.5);
  strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),multiply);
  sxy.Set(1.0,1.0);
  wh.Set(0.05,0.05);
  strokes.Place(sxy,wh,t,library.find("data/images/elements/cube.png"),solarize);
  strokes.layers.Add( additive, crayons.Pick(alabaster), sphereTextureLibrary.findByFilenamePartial( "dust2" ) );
 }
 void Paint() {
  strokes.Paint();
 }
 void Render() {
  glPush();
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);
  lap.Apply();
  glRotated(rotation.y, 1.0, 0.0, 0.0); // x/y flipped
  glRotated(rotation.x, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  glTranslated(position.x,position.y,position.z); //0.0,0.0,-1.3);
 // glDisable(GL_CULL_FACE);
  //glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_NONE); 
  Blending(blend);
  glColor4fv(tint.floats);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,fbos.F.TextureID());
  sphere->vboVNT[0].Render();
  glBindTexture(GL_TEXTURE_2D,fbos.R.TextureID());
  sphere->vboVNT[1].Render();
  glBindTexture(GL_TEXTURE_2D,fbos.B.TextureID());
  sphere->vboVNT[2].Render();
  glBindTexture(GL_TEXTURE_2D,fbos.L.TextureID());
  sphere->vboVNT[3].Render();
  glBindTexture(GL_TEXTURE_2D,fbos.U.TextureID());
  sphere->vboVNT[4].Render();
  glBindTexture(GL_TEXTURE_2D,fbos.D.TextureID());
  sphere->vboVNT[5].Render();
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  if ( coordinator ) Coordinator();
  glPop();
 }
};

class SpherePainters : public LinkedList {
public:
 CLEARLISTRESET(SpherePainter);
};
NAMED(SpherePainter,SpherePainterHandle,SpherePainterHandles);