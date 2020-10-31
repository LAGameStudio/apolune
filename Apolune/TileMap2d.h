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

#include "macros.h"
#include "Display.h"
#include "DisplayList.h"
#include "Indexed.h"

#include "Strings.h"
#include "Proce55or.h"
#include "Presentation.h"
#include "CompositeImage.h"
#include "Film.h"
#include "Trigger.h"

#include "TexFont.h"

class MassiveTileMap2d;
class MassiveTileMap2dd;

class Tile2dInfo : public ListItem {
public:
 Zp<MassiveTileMap2d> parent;
 Zp<MassiveTileMap2dd> parentd;
 virtual void OnCollide( void *actor, int type ) {}
 virtual void Between() {}
 virtual void Setup() {}
 virtual void Action() {}
 virtual void Cut() {}
};

template<class T>
struct Tile2dVector : public ListItem {
public:
 Cartesian tileCoord,tileOffset; // tileCoord is the tile index coordinate, tileOffset is the coordinate relative tile top-left
 ScreenPosition screenCoord; // screenCoord is a related screen coordinate
 Vertexd queryCoord;  // queryCoord is the originating coordinate
 Zp<T> tile;
 // Used to store distance and other stuff.
 Zint i;
 Zdouble d;
 Numbers values;
 void DrawDebug( GLWindow *target, const char *name, int x, int y, int tilesize ) {
  if ( tile && (tile->image.i || tile->i) ) {
   tile->Render(target,x,y,tilesize,tilesize,tileCoord.x,tileCoord.y);
   QuadiNoAlpha(tile->image.i?tile->image.i:tile->i,crayons.Pick(alabaster),none,x,y,tilesize,tilesize);
  }
  else
  Rectangle(crayons.Pick(red255),(double)x,(double)y,(double)(x+tilesize),(double)(y+tilesize));
  crayons.gl(alabaster);
  Text(FORMAT(buf,123,"%d,%d",tileCoord.x,tileCoord.y),x+5,y+20,6.0,8.0,false);
  Text(name,x+5,y+30,6.0,8.0,false);
}
 Tile2dVector& operator = ( T *t ) {
  tile=t;
  return *this;
 }
 Tile2dVector& operator = ( Tile2dVector& t ) {
  tileCoord.Set(&t.tileCoord);
  screenCoord.Set(&t.screenCoord);
  tile=t.tile;
  i=t.i;
  d=t.d;
  values.Duplicate(&t.values);
  return *this;
 }
};

class Tile2d : public ListItem {
public:
 Zstring name;
 CompositeFilm film;
 CompositeImage image;
 Zp<GLImage> i;
 Cartesians boundaries;
 Zbool solid,circular;
 Zint w,h;
 Tile2d() : ListItem() {}
 void BinaryRead( BinaryFile *open ) {
  open->read(&w);
  open->read(&h);
  int temp;
  open->read(&temp);
  solid=temp==1;
  open->read(&temp);
  circular=temp==1;
  open->read(&name);
  film.BinaryRead(open);
  image.BinaryRead(open);
  open->read(&temp);
  if ( temp != 0 ) {
   string im; open->read(&im);
   i=library.find((char *) im.c_str());
  }
  boundaries.BinaryRead(open);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->write(&w);
  open->write(&h);
  int temp;
  temp=solid?1:0;
  open->write(&temp);
  temp=circular?1:0;
  open->write(&temp);
  open->write(name);
  film.BinaryWrite(open);
  image.BinaryWrite(open);
  if ( i ) {
   temp=1;
   open->write(&temp);
   string im; im=string(i->filename);
   open->scrawl(im);
  } else {
   temp=0;
   open->write(&temp);
  }
  boundaries.BinaryWrite(open);
 }
 KEYWORDS({
  TXTWORD("name",name)
  SUBWORD("film",film)
  SUBWORD("image",image)
  IMGWORD("i",i)
  SUBWORD("boundaries",boundaries)
  TAGWORD("solid",solid)
  TAGWORD("circular",circular)
  NUMWORD("w",w)
  NUMWORD("h",h)
 })
 WORDKEYS({
  TXTKEY("name",name)
  SSUBKEY("film",film)
  SSUBKEY("image",image)
  IMGKEY("i",i)
  SSUBKEY("boundaries",boundaries)
  TAGKEY("solid",solid)
  TAGKEY("circular",circular)
  NUMBKEY("w",w)
  NUMBKEY("h",h)
 })
 virtual void OnCollide( void *actor, int type ) {}
 virtual bool Colliding( int relX, int relY, int tilesize ) { return false; }
 virtual bool Colliding( double relX, double relY, int tilesize ) { return false; }
 virtual void Pre( int x, int y ) {}
 virtual void Post( int x, int y ) {}
 virtual void Pre( double x, double y ) {}
 virtual void Post( double x, double y ) {}
 virtual void Between() {}
 virtual void Render( GLWindow *target, int x, int y, int tile_w, int tile_h, int Ri, int Rj ) {
  if ( film.film )   film.RenderOn(target,x,y,tile_w,tile_h);
  if ( image.i ) image.BlendOn(target,x,y,tile_w,tile_h);
 }
 virtual void Render( GLWindow *target, double x, double y, int tile_w, int tile_h, int Ri, int Rj ) {
  if ( film.film )   film.RenderOn(target,x,y,tile_w,tile_h);
  if ( image.i ) image.BlendOn(target,x,y,tile_w,tile_h);
 }
 virtual Tile2d *Duplicate() { return _Duplicate(); }
 Tile2d *_Duplicate() {
  Tile2d *n=new Tile2d;
  n->name=name;
  n->image.Duplicate(&image);
  n->i=i;
  n->boundaries.Duplicate(&boundaries);
  n->solid=solid;
  n->circular=circular;
  n->w=w;
  n->h=h;
  return n;
 }
};

HANDLED(Tile2d,Tile2dHandle,Tile2dHandles,Tile2dHandlesHandle,Tile2dHandlesHandles);

class Tile2dResult : public Tile2dVector<Tile2d> {
public:
 Tile2dResult() {}
 Tile2dResult( int tx, int ty, Tile2d *tile ) {
  tileCoord.Set(tx,ty);
  this->tile=tile;
 }
};

class Tile2dResults : public LinkedList {
public:
 void Add( int tx, int ty, Tile2d *tile ) {
  Append(new Tile2dResult(tx,ty,tile));
 }
 void DrawDebug( GLWindow *target, int x, int y, int tilesize ) {
  int i=0;
  FOREACH(Tile2dResult,t) {
   t->DrawDebug(target,"",x+i*tilesize+i*2,y,tilesize);
   i++;
  }
 }
 void DrawDebug( GLWindow *target, int x, int y, int tilesize, bool vertical ) {
  int i=0;
  FOREACH(Tile2dResult,t) {
   t->DrawDebug(target,"",x,y+i*tilesize+i*2,tilesize);
   i++;
  }
 }
 CLEARLISTRESET(Tile2dResult);
};

class Tile2dAround : public ListItem {
public:
 Indexed<Tile2dResult> tile;
 Tile2dAround() : ListItem() {}
 void Clear() { tile.Size(0); }
 void DrawDebug( GLWindow *target, int x, int y, int tilesize ) {
  for ( int i=0; i<(int)tile.w; i++ ) for ( int j=0; j<(int)tile.h; j++ ) {
   tile(i,j).DrawDebug(target,"",x+i*tilesize+i*2,y+j*tilesize+j*2,tilesize);
  }
 }
};

class DisplayListTile2d : public Tile2d {
public:
 DisplayList list;
 virtual void Create( CompositeImages *images ) { 
  // consumes a composite image list, converts it into a DL for use as a tile
  if ( list.uploaded ) list.Delete();
  list.StartRecording();
  EACH(images->first,CompositeImage,i) {
   i->DrawImage(i->i,i->tint,i->blend,i->position.x,i->position.y,i->position.w,i->position.h);
  }
  list.StopRecording();
 }
 virtual void Render( GLWindow *target, int x, int y, int tile_w, int tile_h, int Ri, int Rj ) {
  glPushMatrix();
  glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
  glScalef((GLfloat) tile_w, (GLfloat) tile_h, 1.0f); // 1 or 0?
  list.Call();
  glPopMatrix();
 }
};

typedef std::vector<std::vector<Tile2d*>> TileMap;

class TileSet2d : public LinkedList {
public:
 void Between() {
  FOREACH(Tile2d,t) t->Between();
 }
 void ReadAppend( BinaryFile *open ) {
  Tile2d *n=new Tile2d; 
  Append(n);
  n->BinaryRead(open);
 }
 VSKEYSTART
  SKEY("tile", { SKEYPAIR; Tile2d *n=new Tile2d; n->fromString(key.c_str()); Append(n); })
 SKEYEND("TileSet2d");
 virtual string toString() {
  string out=string("");
  incdent();
  FOREACH(Tile2d,t) out+=indention+string("tile {\n")+t->toString()+indention+string("}\n");
  decdent();
  return out;
 }
 Tile2d *Add( Tile2d *t ) {
  Append(t);
  return t;
 }
 Tile2d *find( const char *s ) {
  FOREACH(Tile2d,t) if ( !str_cmp(s,t->name.c_str()) ) return t;
  return null;
 }
 Tile2d *find( string s ) {
  FOREACH(Tile2d,t) if ( !str_cmp(s.c_str(),t->name.c_str()) ) return t;
  return null;
 }
 // Generates an N-letter ASCII code for the tile by checking existing tile names (for storing maps to a file or string).
 // 1 is limited to < 60ish, 2 is < 1000, 3 is < 10000 or so perhaps
 string unique( int hashLength ) {
  string s;
  bool found=true;
  while ( found ) {
   s=Hash(hashLength);
   found=false;
   FOREACH(Tile2d,t) if ( !str_cmp(t->name.c_str(),s.c_str()) ) found=true;
  }
  return s;
 }
 TileSet2d *resizeAndDuplicate( float factor ) {
  TileSet2d *tiles=new TileSet2d;
  FOREACH(Tile2d,t) {
   Tile2d *tile=t->Duplicate();
   tile->w=(int) ((float)t->w * factor);
   tile->h=(int) ((float)t->h * factor);
   tiles->Append(tile);
  }
 }
 TileSet2d *Scale( float factor ) {
  TileSet2d *tiles=new TileSet2d;
  FOREACH(Tile2d,t) {
   t->w=(int) ((float)t->w * factor);
   t->h=(int) ((float)t->h * factor);
  }
 }
 CLEARLISTRESET(Tile2d);
 ~TileSet2d() { Clear(); }
};

class Tile2dPlacement : public ListItem {
public:
 Zp<Tile2d> tile;
 Zint x, y, w, h;
 Cartesians boundaries;
 PROGRAMMABLE(Tile2dPlacement,CollidePtr,collide,Collide);
 Zbool collision_result;
 Zp<int> offset_x,offset_y;
 Zp<int> colliding_x,colliding_y;
 Zp<GLWindow> target;
 Tile2dPlacement( int X, int Y, Tile2d *t ) : ListItem() {
  Constructor();
  MoveBy(X,Y);
  Assign(t);
 }
 void Constructor() {
  collide=&Tile2dPlacement::BoundingBox;
 }
 void BoundingBox() {
  bool accumulator=false;
  EACH(boundaries.first,Cartesian,c) if ( WITHIN(*colliding_x,*colliding_y,c->x,c->y,c->x2,c->y2) ) accumulator=true;
  collision_result=accumulator;
 }
 void Circular() {
  bool accumulator=false;
  EACH(boundaries.first,Cartesian,c) if ( c->Distancei(*colliding_x,*colliding_y) < c->w ) accumulator=true;
  collision_result=accumulator;
 }
 void TestAgainst( int *X, int *Y ) {
  colliding_x=X;
  colliding_y=Y;
 }
 bool isColliding() {
  this->Collide();
  return collision_result;
 }
 void MoveBy( int X, int Y ) {
  x+=X; y+=Y;
  boundaries.MoveBy(x,y);
 }
 void Assign( Tile2d *t ) {
  tile=t;
  boundaries.Clear();
  boundaries.Duplicate(&t->boundaries);
  boundaries.MoveBy(x,y);
  if ( t->circular ) collide=&Tile2dPlacement::Circular;
  else collide=&Tile2dPlacement::BoundingBox;
  collision_result=false;
 }
 void RenderTo( GLWindow *T ) { target=T; }
 void Render() {
  if ( tile->film.film ) tile->film.RenderOfs(target,x,y,tile->w,tile->h);
  if ( tile->image.i ) tile->image.RenderOfs(target,x,y,tile->w,tile->h);
 }
 void RenderOffset() {
  if ( tile->film.film ) tile->film.RenderOfs(target,*offset_x+x,*offset_y+y,tile->w,tile->h);
  if ( tile->image.i ) tile->image.RenderOfs(target,*offset_x+x,*offset_y+y,tile->w,tile->h);
 }
};

class Tile2dPlacements : public LinkedList {
public:
 void RenderTo( GLWindow *T ) { FOREACH(Tile2dPlacement,t) t->RenderTo(T); }
 void Render() {
  FOREACH(Tile2dPlacement,t) {
   t->Render();
  }
 }
 void RenderOffset() {
  FOREACH(Tile2dPlacement,t) {
   t->RenderOffset();
  }
 }
 void AssignOffset( int *x, int *y ) {
  FOREACH(Tile2dPlacement,t) {
   t->offset_x=x;
   t->offset_y=y;
  }
 }
 void MoveBy( int x, int y ) {
  FOREACH(Tile2dPlacement,t) t->MoveBy(x,y);
 }
 CLEARLISTRESET(Tile2dPlacement);
 void Add( int X, int Y, Tile2d *t ) {
  Append( new Tile2dPlacement( X, Y, t ) );
 }
 void fromString( TileSet2d *t2d, char *s, char eol, int w, int h, int charLength ) {
  Strings lines;
  lines.appendFromString(s,eol);
  fromStrings(t2d,&lines,w,h,charLength);
 }
 void fromStrings( TileSet2d *t2d, Strings *lines, int w, int h, int charWidthPerTile ) {
  char buf[80];
  int y=0,j;
  buf[charWidthPerTile]='\0';
  for ( String *s=(String *) (lines->first); s; s=(String *) s->next ) {
   int x=0;
   const char *c=s->c_str();
   while ( *c!='\0' ) {
    j=0;
    while ( *c!='\0' && j<charWidthPerTile ) { buf[j++]=*c; c++; }
    Tile2d *t=t2d->find(buf);
    if ( t ) {
     Tile2dPlacement *tp=new Tile2dPlacement(x,y,t);
     tp->Assign(t);
     Append(tp);
    }
    x+=w;
   }
   y+=h;
  }
 }
 Tile2dPlacement *isColliding( int *x, int *y ) {
  FOREACH(Tile2dPlacement,t) {
   t->colliding_x=x;
   t->colliding_y=y;
   if ( t->isColliding() ) return t;
  }
  return null;
 }
 ~Tile2dPlacements() { Clear(); }
};

/*
 * Simple tilemap good for a single layer, uses the Placements and a conventional C array.
 * This tilemap degrades in performance as the tilemap gets bigger, and does not use any
 * optimizations such as screen limitations.  Useful for drawing a series of images and/or
 * films, it was originally intended to be placed by an uber-class to make it easier to
 * store repeating sequences (animated, related tiles, etc).  However, it is better to use
 * a "tilemap section manager" class that manages part of a 2d surface.  This may be reused
 * by the sprite engine at a later time to describe complex tiled objects that have a limited
 * number of on-screen appearances.
 */
class TileMap2d : public ListItem {
public:
 Tile2dPlacements placements;
 TileSet2d *tiles;
 Cartesians boundaries;
 Cartesians mirrors;
 void Init() {
  tiles=null;
 }
 void Render() {
  placements.Render();
 }
 void RenderOffset() {
  placements.RenderOffset();
 }
 void Between() {
  // I would call tiles->Between() here but it should be managed elsewhere.
 }
 void RenderTo(GLWindow *surface) {
  placements.RenderTo(surface);
 }
 void MoveBy( int x, int y ) {
  placements.MoveBy(x,y);
  boundaries.MoveBy(x,y);
  mirrors.MoveBy(x,y);
 }
 void AssignOffset( int *x, int *y ) {
  placements.AssignOffset(x,y);
 }
 void MoveBoundariesAndMirrors( int x, int y ) {
  boundaries.MoveBy(x,y);
  mirrors.MoveBy(x,y);
 }
};

class TileMaps2d : public LinkedList {
public:
 void RenderTo( GLWindow *surface ) {
  FOREACH(TileMap2d,t) t->RenderTo(surface);
 }
 void Render() {
  FOREACH(TileMap2d,t) t->Render();
 }
 void AssignOffset( int *x, int *y ) {
  FOREACH(TileMap2d,t) t->AssignOffset(x,y);
 }
 void RenderOffset() {
  FOREACH(TileMap2d,t) t->RenderOffset();
 }
 void MoveBy( int x, int y ) {
  FOREACH(TileMap2d,t) t->MoveBy(x,y);
 }
 void MoveBoundariesAndMirrors( int x, int y ) {
  FOREACH(TileMap2d,t) t->MoveBoundariesAndMirrors(x,y);
 }
 CLEARLISTRESET(TileMap2d);
 ~TileMaps2d() { Clear(); }
};

enum TilemapDrawModes {
 translated,
 offset
};
enum TilemapRenderModes {
 clampTile,
 clampConstrain,
 wrapTile,
 wrapConstrain
};

/*******************************************************************************************************************
 * Multi-modal on-screen parallax-able high performance tilemap engine that uses ASCII as input,
 * maintains a polymorphic "info" layer, can provide collision feedback and may be interacted
 * with.  Comes in two flavors with two render options and two draw options.
 *
 * Main render modes
 *  Tiled       - Tiled drawing method scales input art to a standard size; when used with the parallax
 *                manager class, best effect is to set tile sizes at a standard increasing interval.  Can be "zoomed" easily
 *  Constrained - Draws input art at pre-determined sizes to alleviate the need to draw many tiles, yet provides
 *                flexibility; easiest implementation in parallax uses standard "tile" size, as tiles become
 *                grid snap points on which art is drawn; can be zoomed but requires additional translation
 *                calculations made on the "position_x","position_y" driving coordinate (a complex 2d camera).
 *
 * Render Options
 *  Clamp - Edges of the map can be exceeded, but no tiles are drawn
 *  Wrap  - Edges of the map wrap to expose corresponding tiles on the far side of the map,
 *          note position_x,position_y must be "wrapped" externally, as the calculations
 *          are discarded (this may change)
 *
 * Draw Options
 *  Translated - Draws the tilemap at the position
 *  Offset     - Draws the tilemap at the position translated by the offset (less efficient 
 *               than simply providing an offset position, not recommended)
 */
class MassiveTileMap2d : public ListItem {
public:
 matrix2d<Zp<Tile2d>> map;
 matrix2d<Zp<Tile2dInfo>> info;
 Tile2dResults tileQuery;
 Tile2dAround tilesAroundQuery;
 Zint Ri,Rj;
 Zint w,h,wm1,hm1,size;
 Zint orig_tile_w,orig_tile_h,tile_w,tile_h,tile_wm1,tile_hm1,
  pixel_w,pixel_h,pixel_w2,pixel_h2,pixelSize,tw4,th4,tileconstrainofsw,tileconstrainofsh,
  totalw,totalh,transofsw,transofsh,pixconstranofsw,pixconstranofsh;
 Zp<int> position_x,position_y,mouse_x,mouse_y,offset_x,offset_y;
 Zp<GLWindow> target;
 Zp<Tile2d> t;
 int X,Y;
 SKEYSTART // It is recommended that the meta-class handle i/o for this feature.
 SKEYEND("MassiveTileMap2d");
 string toString() {
  return string("");
 }
 MassiveTileMap2d() : ListItem() {
  mouse_x=&input.mxi;
  mouse_y=&input.myi;
 }
 PROGRAMMABLE(MassiveTileMap2d,RenderModePtr,renderMode,Render);
 PROGRAMMABLE(MassiveTileMap2d,RepositionModePtr,repositionMode,Reposition);
 PROGRAMMABLE(MassiveTileMap2d,ResizedModePtr,resizedMode,Resized);
 PROGRAMMABLE(MassiveTileMap2d,DrawModePtr,drawMode,Draw);
 PROGRAMMABLE(MassiveTileMap2d,DrawConstrainedModePtr,drawConstrainedMode,DrawConstrained);
 void RenderMode( TilemapRenderModes mode ) {
  switch ( mode ) {
  default:
  case clampTile:  
    renderMode=&MassiveTileMap2d::RenderTiledClamp;
    repositionMode=&MassiveTileMap2d::RepositionTiledClamp;
    resizedMode=&MassiveTileMap2d::ResizedTiled;
   break;
  case clampConstrain:
    renderMode=&MassiveTileMap2d::RenderConstrainedClamp;
    repositionMode=&MassiveTileMap2d::RepositionConstrainedClamp;
    resizedMode=&MassiveTileMap2d::ResizedConstrained;
   break;
  case wrapTile:
   break;
  case wrapConstrain:
   break;
  }
 }
 void DrawMode( TilemapDrawModes mode ) {
  switch ( mode ) {
  default:
  case translated:
    drawMode=&MassiveTileMap2d::TranslatedTiled;
    drawConstrainedMode=&MassiveTileMap2d::TranslatedConstrained;
   break;
  case offset:
    drawMode=&MassiveTileMap2d::OffsetTiled;
    drawConstrainedMode=&MassiveTileMap2d::OffsetConstrained;
   break;
  }
 }
 void Info( Tile2dInfo *t, int x, int y ) {
  info(x,y)=t;
  t->parent=this;
 }
 // Returns tile info for all tiles around a given tile (in tile coordinate space)
 Tile2dAround *TileAroundQuery( int tx, int ty, int tradius=1 ) {
  tilesAroundQuery.tile.Size((size_t)(tradius*2+1),(size_t)(tradius*2+1));
  int midx=tradius;
  int midy=tradius;
  int Tx=tx-tradius;
  for ( int i=0; i<(int)tilesAroundQuery.tile.w; i++ ) {
   int Ty=ty-tradius;
   for ( int j=0; j<(int)tilesAroundQuery.tile.h; j++ ) {
    if ( map.withinBounds(Tx,Ty) ) tilesAroundQuery.tile(i,j).tile=map(Tx,Ty);
    tilesAroundQuery.tile(i,j).tileCoord.Set(Tx,Ty);
    Ty++;
   }
   Tx++;
  }
  return &tilesAroundQuery;
 }
 // Returns tile info for all tiles on a line (in map position space)
 Tile2dResults *TileLineQuery( Vertexd *start, Vertexd *end ) {
  tileQuery.Clear();
  Cartesian here,there,temp;
  here.x=(int)(start->x/tile_w);
  here.y=(int)(start->y/tile_h);
  there.x=(int)(end->x/tile_w);
  there.y=(int)(end->y/tile_h);
  if ( here.x > there.x ) {
   temp.x=here.x;
   here.x=there.x;
   there.x=temp.x;
  }
  if ( here.y > there.y ) {
   temp.y=here.y;
   here.y=there.y;
   there.y=temp.y;
  }
  // Create a small box collision profile between start and end
  Boxes boxes;
  for ( int i=here.x; i<=there.x; i++ ) for ( int j=here.y; j<=there.y; j++ ) {
   double x1=i*tile_w;
   double y1=j*tile_h;
   boxes.Add(x1,y1,x1+tile_w,y1+tile_h);
  }
  Line line(start->x,start->y,end->x,end->y);
  double ratio=0.0;
  BoxCollisionResults bcr; // during the later collision check, needs to have results sorted by first occurance
  EACH(boxes.first,Box,b) if ( line.Collides(b,&bcr) ) {
   int tx=(int)(b->x/(double)tile_w);
   int ty=(int)(b->y/(double)tile_h);
   if ( map.withinBounds(tx,ty) ) tileQuery.Add(tx,ty,map(tx,ty));
  }
  return &tileQuery;
 }
 Tile2dResults *TileRectangleQuery( int stx, int sty, int etx, int ety ) {
  tileQuery.Clear();
  for ( int i=stx; i<=etx; i++ ) for ( int j=sty; j<=ety; j++ ) if ( map.withinBounds(i,j) ) {
   tileQuery.Add(i,j,map(i,j));
  }
  return &tileQuery;
 }
 Tile2dResults *TileDistanceQuery( int ctx, int cty, int r ) {
  tileQuery.Clear();
  int sx=ctx-r;
  int ex=ctx+r;
  int sy=cty-r;
  int ey=cty+r;
  for ( int i=sx-r; i<=ex; i++ ) for ( int j=sy; j<=ey; j++ ) if ( idistance(i,j,ctx,cty) <= r && map.withinBounds(i,j) ) {
   tileQuery.Add(i,j,map(i,j));
  }
  return &tileQuery;
 }
 void Create( TilemapRenderModes mode, TilemapDrawModes draw, int Tiles_W, int Tiles_H, int Tile_W, int Tile_H ) {
  w=Tiles_W;
  wm1=w-1;
  h=Tiles_H;
  hm1=h-1;
  size=w*h;
  orig_tile_w=tile_w=Tile_W;
  tile_wm1=tile_w-1;
  orig_tile_h=tile_h=Tile_H;
  tile_hm1=tile_h-1;
  pixel_w=tile_w*w;
  pixel_w2=pixel_w/2;
  pixel_h=tile_h*h;
  pixel_h2=pixel_h/2;
  pixelSize=pixel_w*pixel_h;
  map.resize( (unsigned) w, (unsigned) h);
  info.resize( (unsigned) w, (unsigned) h);
  for ( int i=0; i<w; i++ )
   for ( int j=0; j<h; j++ ) map( (unsigned) i, (unsigned) j)=null;
  for ( int i=0; i<w; i++ )
   for ( int j=0; j<h; j++ ) info( (unsigned) i, (unsigned) j)=null;
  RenderMode(mode);
  DrawMode(draw);
  t=null;
  X=Y=tile_render_w=tile_render_h=0;
 }
 inline void Assign( Tile2d *t, unsigned x, unsigned y ) {
  if ( x >= (unsigned) w ) {
   OUTPUT("MassiveTileMap2d::Assign x=%d bigger than w=%d", (int) x, (int) w );
   return;
  } else if ( y >= (unsigned) h ) {
   OUTPUT("MassiveTileMap2d::Assign y=%d bigger than h=%d", (int) y, (int) h );
   return;
  }
  map(x,y)=t;
 }
 void fromStrings(
  TilemapRenderModes mode,
  TilemapDrawModes draw,
  TileSet2d *t2d, Strings *lines,
  unsigned W, unsigned H,
  unsigned Tile_W, unsigned Tile_H,
  unsigned charWidthPerTile
 ) {
  Create(mode,draw,W,H,Tile_W,Tile_H);
  char buf[80];
  unsigned y=0,j;
  buf[charWidthPerTile]='\0';
  for ( String *s=(String *) (lines->first); s; s=(String *) s->next ) {
   const char *c=s->c_str();
   unsigned x=0;
   while ( *c!='\0' ) {
    j=0;
    while ( *c!='\0' && j<charWidthPerTile ) { buf[j++]=*c; c++; }
    t=t2d->find(buf);
    if ( t ) {
     Assign(t,x,y);
    }
    x+=1;
   }
   y+=1;
  }
 }
 void AssignPosition( int *x, int *y ) {
  position_x=x;
  position_y=y;
  Reposition();
 }
 void OffsetTiled() {
  int x=*offset_x+X,y=*offset_y+Y;
  t->Pre(x,y);
  t->Render(target,x,y,tile_w,tile_h,Ri,Rj);
  t->Post(x,y);
 }
 void OffsetConstrained() {
  int x=*offset_x+X,y=*offset_y+Y;
  t->Pre(x,y);
  t->Render(target,x,y,t->w,t->h,Ri,Rj);
  t->Post(x,y);
 }
 void TranslatedTiled() {
  t->Pre(X,Y);
  t->Render(target,X,Y,tile_w,tile_h,Ri,Rj);
  t->Post(X,Y);
 }
 void TranslatedConstrained() {
  t->Pre(X,Y);
  t->Render(target,X,Y,t->w,t->h,Ri,Rj);
  t->Post(X,Y);
 }
 void RenderTo( GLWindow *surface ) {
  target=surface;
  mouse_x=&(target->input->mxi);
  mouse_y=&(target->input->myi);
  Resized();
 }
 void Zoom( float ww, float hh ) {
  tile_w=(int) ( (float) orig_tile_w * ww );
  tile_h=(int) ( (float) orig_tile_h * hh );
  if ( tile_w < 1 ) tile_w=1;
  if ( tile_h < 1 ) tile_h=1;
  tile_wm1=tile_w-1;
  tile_hm1=tile_h-1;
  Resized();
  Reposition();
 }
 void ZoomReset() {
  tile_w=orig_tile_w;
  tile_h=orig_tile_h;
  tile_wm1=tile_w-1;
  tile_hm1=tile_h-1;
 }
 void Init( TilemapRenderModes mode, TilemapDrawModes draw, GLWindow *T, int *x, int *y,
  TileSet2d *t2d, Strings *lines,
  unsigned W, unsigned H,
  unsigned Tile_W, unsigned Tile_H,
  unsigned charWidthPerTile
 ) {
  fromStrings( mode, draw, t2d, lines, W, H, Tile_W, Tile_H, charWidthPerTile );
  RenderTo(T);
  AssignPosition(x,y);
 }
 Tile2d *TileAtPosition( Tile2dInfo **d ) { // Note: this currently works properly only with tiled
  tile_at_position.x=*position_x/tile_w;
  tile_at_position.y=*position_y/tile_h;
  if ( !WITHIN(tile_at_position.x,tile_at_position.y,0,wm1,0,hm1) ) return null;
  *d=info(tile_at_position.x,tile_at_position.y);
  return map(tile_at_position.x,tile_at_position.y);
 }
 Tile2d *Tile( Tile2dInfo **d, int x, int y ) {
  int tx=*position_x/tile_w;
  int ty=*position_y/tile_h;
  if ( !WITHIN(tx,ty,0,wm1,0,hm1) ) return null;
  *d=info(tile_at_position.x,tile_at_position.y);
  return map(tx,ty);
 }
 Tile2d *MouseTile() { // Note: does not work
  int tx=((*position_x)+target->input->mxi-target->x-target->w2)/tile_w;
  int ty=((*position_y)+target->input->myi-target->y-target->h2)/tile_h;
  if ( !WITHIN(tx,ty,0,wm1,0,hm1) ) return null;
  return map(tx,ty);
 }
 virtual void Pre() { }
 virtual void Post() { }
//---------------------------- Mode: Tiled
 void ResizedTiled() {
  tile_render_w=target->w/tile_w+2;
  tile_render_h=target->h/tile_h+2;
 }
 void RepositionTiledClamp() {
  sx          =*position_x-target->w2;
  sy          =*position_y-target->h2;
  ex          =*position_x+target->w2;
  ey          =*position_y+target->h2;
  start_tile_x=sx/tile_w;
  start_draw_x=tile_w-sx%tile_w-tile_w;
  if ( start_tile_x < 0 ) {
   start_draw_x+=abs(start_tile_x)*tile_w;
   start_tile_x=0;
  }
  start_tile_y=sy/tile_h;
  start_draw_y=tile_h-sy%tile_h-tile_h;
  if ( start_tile_y < 0 ) {
   start_draw_y+=abs(start_tile_y)*tile_h;
   start_tile_y=0;
  }
  end_tile_x=start_tile_x+tile_render_w; 
  if ( end_tile_x >= w ) {
   end_tile_x=wm1;
  }
  end_tile_y=start_tile_y+tile_render_h; 
  if ( end_tile_y >= h ) {
   end_tile_y=hm1;
  }
 }
 void RenderTiledClamp() {
  if ( end_tile_x < 0 ) return;
  if ( end_tile_y < 0 ) return;
  if ( start_tile_x > w || start_tile_y > h ) return;
  X=start_draw_x;
  for ( Ri=start_tile_x; Ri<end_tile_x; Ri++ ) {
   Y=start_draw_y;
   for ( Rj=start_tile_y; Rj<end_tile_y; Rj++ ) {
    t=map(Ri,Rj);
    if ( t ) Draw();
    Y+=tile_h.value;
   }
   X+=tile_w.value;
  }
  Post();
 }
 void RenderTiledWrap() {
 }
//---------------------------- Mode: Constrained
 void ResizedConstrained() {
  tw4=target->w2/2;
  th4=target->h2/2;
  totalw=target->w+tw4;
  totalh=target->h+th4;
  transofsw=target->w2+tw4;
  transofsh=target->h2+th4;
  tile_render_w=(totalw)/tile_w+3;
  tile_render_h=(totalh)/tile_h+2;
  tileconstrainofsw=tw4/tile_w;
  tileconstrainofsh=th4/tile_h;  
  pixconstranofsw=tile_w-((4*tile_w)+tw4); // 4 is an aspect ratio thing
  pixconstranofsh=tile_h-((3*tile_h)+th4); // 3 is an aspect ratio thing
 }
 void RepositionConstrainedClamp() {
  sx          =*position_x-transofsw;
  sy          =*position_y-transofsh;
  ex          =*position_x+target->w2;
  ey          =*position_y+target->h2;
  start_tile_x=sx/tile_w-tileconstrainofsw;
  start_draw_x=-sx%tile_w+pixconstranofsw; // draws more to the left in case of constraints
  if ( start_tile_x < 0 ) {
   start_draw_x+=abs(start_tile_x)*tile_w;
   start_tile_x=0;
  }
  start_tile_y=sy/tile_h-tileconstrainofsh;
  start_draw_y=-sy%tile_h+pixconstranofsh;
  if ( start_tile_y < 0 ) {
   start_draw_y+=abs(start_tile_y)*tile_h;
   start_tile_y=0;
  }
  end_tile_x=start_tile_x+tile_render_w; 
  if ( end_tile_x >= w ) {
   end_tile_x=wm1;
  }
  end_tile_y=start_tile_y+tile_render_h; 
  if ( end_tile_y >= h ) {
   end_tile_y=hm1;
  }
 }
 void RenderConstrainedClamp() {
  Pre();
  if ( end_tile_x < 0 ) return;
  if ( end_tile_y < 0 ) return;
  if ( start_tile_x > w || start_tile_y > h ) return;
  X=start_draw_x;
  for ( Ri=start_tile_x; Ri<end_tile_x; Ri++ ) {
   Y=start_draw_y;
   for ( Rj=start_tile_y; Rj<end_tile_y; Rj++ ) {
    t=map(Ri,Rj);
    if ( t ) DrawConstrained();
    Y+=tile_h.value;
   }
   X+=tile_w.value;
  }
  Post();
 }
 void RenderWrapConstrained() {
 }
 // Get the map-coord center of a tile relative to 0,0 (top left of map)
 int TileCenterX( int tx ) { return tx*tile_w+tile_w/2; }
 int TileCenterY( int ty ) { return ty*tile_h+tile_h/2; }
 // Calculate the absolute (relative to 0,0 of map) offsets of a given coordinate from top/left of any grid block
 // mcx, mcy "map coord x, map coord y", top left is 0,0
 int TileRelativeX( int mcx ) { return mcx%tile_w; }
 int TileRelativeY( int mcy ) { return mcy%tile_h; }
 int TileCoordX( int mcx ) { return mcx/tile_w; }
 int TileCoordY( int mcy ) { return mcy/tile_h; }
 // Calculate the absolute (relative to 0,0 of map) top left of a tile based on its tile coordinate
 int TileTopLeftX( int tx ) { return tx*tile_w; }
 int TileTopLeftY( int ty ) { return ty*tile_h; }
 /*
  * Debug
  */
 void PrintDebugInfo( int x, int y ) {
  Strings s;
  s.printf( "W/H: %dx%d (wm1=%d,hm1=%d)  size=%d",
            (int)w,(int)h,(int)wm1,(int)hm1,(int)size );
  s.printf( "orig_tile_w/h=%dx%d  tile_w/h: %dx%d tilewm1/hm1:%d/%d", 
            (int)orig_tile_w,(int)orig_tile_h,(int)tile_w,(int)tile_h,(int)tile_wm1,(int)tile_hm1 );
  s.printf( "*p_x,y= %d,%d  mx,my=%d,%d",
   (int)(*position_x),(int)(*position_y),(int)(*mouse_x),(int)(*mouse_y) );
  // *offset_x,*offset_y,
  s.printf( "pixel: w/h: %dx%d w2,h2: %dx%d, size: %d",
   (int)pixel_w,(int)pixel_h,(int)pixel_w2,(int)pixel_h2,(int)pixelSize );
  s.printf( "Constrain: w/h:%dx%d tconofsw/h: %dx%d total w/h: %dx%d  transofsw/h: %dx%d",
   (int)tw4,(int)th4,(int)tileconstrainofsw,(int)tileconstrainofsh,(int)totalw,(int)totalh,(int)transofsw,(int)transofsh );
  s.printf( "s: %d,%d e: %d,%d", (int)sx,(int)sy,(int)ex,(int)ey );
  s.printf( "start_tile: %d,%d, end_tile:%d,%d",
   (int)start_tile_x, (int)start_tile_y, (int)end_tile_x, (int)end_tile_y );
  s.printf( "start_draw: %d,%d, tile_render_w/h: %dx%d ",    
   (int)start_draw_x, (int)start_draw_y, (int)tile_render_w, (int)tile_render_h );
  string content=s.Concatenate('\n');
  WriteTexFont(target,microFont,crayons.Pick(green),additive,x,y,10,10,5,content.c_str());
 }
 private:
 Cartesian tile_at_position;
 int sx,sy,ex,ey, start_tile_x, start_tile_y, end_tile_x, end_tile_y,
     start_draw_x, start_draw_y, tile_render_w, tile_render_h;
};

// Same as above, but uses doubles
class MassiveTileMap2dd : public ListItem {
public:
 matrix2d<Zp<Tile2d>> map;
 matrix2d<Zp<Tile2dInfo>> info;
 Tile2dResults tileQuery;
 Tile2dAround tilesAroundQuery;
 Zint Ri,Rj; // tile position while rendering, passed to the tile
 Zint w,h,wm1,hm1,size;
 Zint orig_tile_w,orig_tile_h,tile_w,tile_h,tile_wm1,tile_hm1,
  pixel_w,pixel_h,pixel_w2,pixel_h2,pixelSize,tw4,th4,
  tileconstrainofsw,tileconstrainofsh,totalw,totalh,
  transofsw,transofsh,pixconstranofsw,pixconstranofsh;
 Zp<int> mouse_x,mouse_y;
 Zp<double> position_x,position_y,offset_x,offset_y;
 Zp<GLWindow> target;
 Zp<Tile2d> t;
 Zdouble X,Y;
 SKEYSTART // It is recommended that the meta-class handle i/o for this feature.
 SKEYEND("MassiveTileMap2d");
 string toString() {
  return string("");
 }
 MassiveTileMap2dd() : ListItem() {
  mouse_x=&input.mxi;
  mouse_y=&input.myi;
 }
 PROGRAMMABLE(MassiveTileMap2dd,RenderModePtr,renderMode,Render);
 PROGRAMMABLE(MassiveTileMap2dd,RepositionModePtr,repositionMode,Reposition);
 PROGRAMMABLE(MassiveTileMap2dd,ResizedModePtr,resizedMode,Resized);
 PROGRAMMABLE(MassiveTileMap2dd,DrawModePtr,drawMode,Draw);
 PROGRAMMABLE(MassiveTileMap2dd,DrawConstrainedModePtr,drawConstrainedMode,DrawConstrained);
 void RenderMode( TilemapRenderModes mode ) {
  switch ( mode ) {
  default:
  case clampTile:  
    renderMode=&MassiveTileMap2dd::RenderTiledClamp;
    repositionMode=&MassiveTileMap2dd::RepositionTiledClamp;
    resizedMode=&MassiveTileMap2dd::ResizedTiled;
   break;
  case clampConstrain:
    renderMode=&MassiveTileMap2dd::RenderConstrainedClamp;
    repositionMode=&MassiveTileMap2dd::RepositionConstrainedClamp;
    resizedMode=&MassiveTileMap2dd::ResizedConstrained;
   break;
  case wrapTile:
   break;
  case wrapConstrain:
   break;
  }
 }
 void DrawMode( TilemapDrawModes mode ) {
  switch ( mode ) {
  default:
  case translated:
    drawMode=&MassiveTileMap2dd::TranslatedTiled;
    drawConstrainedMode=&MassiveTileMap2dd::TranslatedConstrained;
   break;
  case offset:
    drawMode=&MassiveTileMap2dd::OffsetTiled;
    drawConstrainedMode=&MassiveTileMap2dd::OffsetConstrained;
   break;
  }
 }
 void Info( Tile2dInfo *t, int x, int y ) {
  info(x,y)=t;
  t->parentd=this;
 }
 void Create( TilemapRenderModes mode, TilemapDrawModes draw, int Tiles_W, int Tiles_H, int Tile_W, int Tile_H ) {
  w=Tiles_W;
  wm1=w-1;
  h=Tiles_H;
  hm1=h-1;
  size=w*h;
  orig_tile_w=tile_w=Tile_W;
  tile_wm1=tile_w-1;
  orig_tile_h=tile_h=Tile_H;
  tile_hm1=tile_h-1;
  pixel_w=tile_w*w;
  pixel_w2=pixel_w/2;
  pixel_h=tile_h*h;
  pixel_h2=pixel_h/2;
  pixelSize=pixel_w*pixel_h;
  map.resize( (unsigned) w, (unsigned) h);
  info.resize( (unsigned) w, (unsigned) h);
  for ( int i=0; i<w; i++ )
   for ( int j=0; j<h; j++ ) map( (unsigned) i, (unsigned) j)=null;
  for ( int i=0; i<w; i++ )
   for ( int j=0; j<h; j++ ) info( (unsigned) i, (unsigned) j)=null;
  RenderMode(mode);
  DrawMode(draw);
  t=null;
  X=Y=0.0; 
  tile_render_w=tile_render_h=0;
 }
 inline void Assign( Tile2d *t, unsigned x, unsigned y ) {
  if ( x >= (unsigned) w ) {
   OUTPUT("MassiveTileMap2d::Assign x=%d bigger than w=%d", (int) x, (int) w );
   return;
  } else if ( y >= (unsigned) h ) {
   OUTPUT("MassiveTileMap2d::Assign y=%d bigger than h=%d", (int) y, (int) h );
   return;
  }
  map(x,y)=t;
 }
 void fromStrings(
  TilemapRenderModes mode,
  TilemapDrawModes draw,
  TileSet2d *t2d, Strings *lines,
  unsigned W, unsigned H,
  unsigned Tile_W, unsigned Tile_H,
  unsigned charWidthPerTile
 ) {
  Create(mode,draw,W,H,Tile_W,Tile_H);
  char buf[80];
  unsigned y=0,j;
  buf[charWidthPerTile]='\0';
  for ( String *s=(String *) (lines->first); s; s=(String *) s->next ) {
   const char *c=s->c_str();
   unsigned x=0;
   while ( *c!='\0' ) {
    j=0;
    while ( *c!='\0' && j<charWidthPerTile ) { buf[j++]=*c; c++; }
    t=t2d->find(buf);
    if ( t ) {
     Assign(t,x,y);
    }
    x+=1;
   }
   y+=1;
  }
 }
 void AssignPosition( double *x, double *y ) {
  position_x=x;
  position_y=y;
  Reposition();
 }
 void OffsetTiled() {
  double x=*offset_x+X,y=*offset_y+Y;
  t->Pre(x,y);
  t->Render(target,x,y,tile_w,tile_h,Ri,Rj);
  t->Post(x,y);
 }
 void OffsetConstrained() {
  double x=*offset_x+X,y=*offset_y+Y;
  t->Pre(x,y);
  t->Render(target,x,y,t->w,t->h,Ri,Rj);
  t->Post(x,y);
 }
 void TranslatedTiled() {
  t->Pre(X,Y);
  t->Render(target,X,Y,tile_w,tile_h,Ri,Rj);
  t->Post(X,Y);
 }
 void TranslatedConstrained() {
  t->Pre(X,Y);
  t->Render(target,X,Y,t->w,t->h,Ri,Rj);
  t->Post(X,Y);
 }
 void RenderTo( GLWindow *surface ) {
  target=surface;
  mouse_x=&(target->input->mxi);
  mouse_y=&(target->input->myi);
  Resized();
 }
 void Zoom( float ww, float hh ) {
  tile_w=(int) ( (float) orig_tile_w * ww );
  tile_h=(int) ( (float) orig_tile_h * hh );
  if ( tile_w < 1 ) tile_w=1;
  if ( tile_h < 1 ) tile_h=1;
  tile_wm1=tile_w-1;
  tile_hm1=tile_h-1;
  Resized();
  Reposition();
 }
 void ZoomReset() {
  tile_w=orig_tile_w;
  tile_h=orig_tile_h;
  tile_wm1=tile_w-1;
  tile_hm1=tile_h-1;
 }
 void Init( TilemapRenderModes mode, TilemapDrawModes draw, GLWindow *T, double *x, double *y,
  TileSet2d *t2d, Strings *lines,
  unsigned W, unsigned H,
  unsigned Tile_W, unsigned Tile_H,
  unsigned charWidthPerTile
 ) {
  fromStrings( mode, draw, t2d, lines, W, H, Tile_W, Tile_H, charWidthPerTile );
  RenderTo(T);
  AssignPosition(x,y);
 }
 // Returns tile info for all tiles in a square of tradius around a given tile (in tile coordinate space)
 Tile2dAround *TileAroundQuery( int tx, int ty, int tradius=1 ) {
  tilesAroundQuery.tile.Size((size_t)(tradius*2+1),(size_t)(tradius*2+1));
  int midx=tradius;
  int midy=tradius;
  int Tx=tx-tradius;
  for ( int i=0; i<(int)tilesAroundQuery.tile.w; i++ ) {
   int Ty=ty-tradius;
   for ( int j=0; j<(int)tilesAroundQuery.tile.h; j++ ) {
    if ( map.withinBounds(Tx,Ty) ) tilesAroundQuery.tile(i,j).tile=map(Tx,Ty);
    tilesAroundQuery.tile(i,j).tileCoord.Set(Tx,Ty);
    Ty++;
   }
   Tx++;
  }
  return &tilesAroundQuery;
 }
 // Returns tile info for all tiles on a line (in map position space)
 Tile2dResults *TileLineQuery( Vertexd *start, Vertexd *end ) {
  tileQuery.Clear();
  Cartesian here,there,temp;
  here.x=(int)(start->x/tile_w);
  here.y=(int)(start->y/tile_h);
  there.x=(int)(end->x/tile_w);
  there.y=(int)(end->y/tile_h);
  if ( here.x > there.x ) {
   temp.x=here.x;
   here.x=there.x;
   there.x=temp.x;
  }
  if ( here.y > there.y ) {
   temp.y=here.y;
   here.y=there.y;
   there.y=temp.y;
  }
  // Create a small box collision profile between start and end
  Boxes boxes;
  for ( int i=here.x; i<=there.x; i++ ) for ( int j=here.y; j<=there.y; j++ ) {
   double x1=i*tile_w;
   double y1=j*tile_h;
   boxes.Add(x1,y1,x1+tile_w,y1+tile_h);
  }
  Line line(start->x,start->y,end->x,end->y);
  double ratio=0.0;
  BoxCollisionResults bcr;
  EACH(boxes.first,Box,b) if ( line.Collides(b,&bcr) ) {
   int tx=(int)(b->minX()/tile_w);
   int ty=(int)(b->minY()/tile_h);
   if ( map.withinBounds(tx,ty) ) tileQuery.Add(tx,ty,map(tx,ty));
  }
  return &tileQuery;
 }
 Tile2dResults *TileRectangleQuery( int stx, int sty, int etx, int ety ) {
  tileQuery.Clear();
  for ( int i=stx; i<=etx; i++ ) for ( int j=sty; j<=ety; j++ ) if ( map.withinBounds(i,j) ) {
   tileQuery.Add(i,j,map(i,j));
  }
  return &tileQuery;
 }
 Tile2dResults *TileDistanceQuery( int ctx, int cty, int r ) {
  tileQuery.Clear();
  int sx=ctx-r;
  int ex=ctx+r;
  int sy=cty-r;
  int ey=cty+r;
  for ( int i=sx-r; i<=ex; i++ ) for ( int j=sy; j<=ey; j++ ) if ( idistance(i,j,ctx,cty) <= r && map.withinBounds(i,j) ) {
   tileQuery.Add(i,j,map(i,j));
  }
  return &tileQuery;
 }
 Tile2d *TileAtPosition( Tile2dInfo **d ) { // Note: this currently works properly only with tiled
  tile_at_position.x=(int)(*position_x/(double)tile_w);
  tile_at_position.y=(int)(*position_y/(double)tile_h);
  if ( !WITHIN(tile_at_position.x,tile_at_position.y,0,wm1,0,hm1) ) return null;
  *d=info(tile_at_position.x,tile_at_position.y);
  return map((int)tile_at_position.x,(int)tile_at_position.y);
 }
 Tile2d *Tile( Tile2dInfo **d, int x, int y ) {
  int tx=(int)(*position_x/(double)tile_w);
  int ty=(int)(*position_y/(double)tile_h);
  if ( !WITHIN(tx,ty,0,wm1,0,hm1) ) return null;
  *d=info(tile_at_position.x,tile_at_position.y);
  return map(tx,ty);
 }
 Tile2d *MouseTile() { // Note: does not work
  int tx=((*position_x)+target->input->mxi-target->x-target->w2)/tile_w;
  int ty=((*position_y)+target->input->myi-target->y-target->h2)/tile_h;
  if ( !WITHIN(tx,ty,0,wm1,0,hm1) ) return null;
  return map(tx,ty);
 }
 virtual void Pre() { }
 virtual void Post() { }
//---------------------------- Mode: Tiled
 void ResizedTiled() {
  tile_render_w=target->w/tile_w+2;
  tile_render_h=target->h/tile_h+2;
 }
 void RepositionTiledClamp() {
  sx          =*position_x-target->w2;
  sy          =*position_y-target->h2;
  ex          =*position_x+target->w2;
  ey          =*position_y+target->h2;
  start_tile_x=(int)sx/tile_w;
  start_draw_x=tile_w-(int)sx%tile_w-tile_w;
  if ( start_tile_x < 0 ) {
   start_draw_x+=abs(start_tile_x)*tile_w;
   start_tile_x=0;
  }
  start_tile_y=(int)sy/tile_h;
  start_draw_y=tile_h-(int)sy%tile_h-tile_h;
  if ( start_tile_y < 0 ) {
   start_draw_y+=abs(start_tile_y)*tile_h;
   start_tile_y=0;
  }
  end_tile_x=start_tile_x+tile_render_w; 
  if ( end_tile_x >= w ) {
   end_tile_x=wm1;
  }
  end_tile_y=start_tile_y+tile_render_h; 
  if ( end_tile_y >= h ) {
   end_tile_y=hm1;
  }
 }
 void RenderTiledClamp() {
  if ( end_tile_x < 0 ) return;
  if ( end_tile_y < 0 ) return;
  if ( start_tile_x > w || start_tile_y > h ) return;
  double startX=0;
  int startTileX=0;
  if ( start_tile_x > 0 ) { // Draw one tile more left for isometric tiles
   startX=start_draw_x-tile_w;
   startTileX=start_tile_x-1;
  } else {
   startX=start_draw_x;
   startTileX=0;
  }
  Y=start_draw_y+(end_tile_y-start_tile_y)*tile_h;
  for ( Rj=(end_tile_y); Rj>=start_tile_y; Rj-- ) { // Draw bottom-to-top to support stacked isometric tiles
   X=startX;
   for ( Ri=startTileX; Ri<end_tile_x; Ri++ ) { // Draw left to right to support isometric tiles
    t=map(Ri,Rj);
    if ( t ) Draw();
    X+=tile_w;
   }
   Y-=tile_h;
  }
  Post();
 }
 void RenderTiledWrap() {
 }
//---------------------------- Mode: Constrained
 void ResizedConstrained() {
  tw4=target->w2/2;
  th4=target->h2/2;
  totalw=target->w+tw4;
  totalh=target->h+th4;
  transofsw=target->w2+tw4;
  transofsh=target->h2+th4;
  tile_render_w=(totalw)/tile_w+3;
  tile_render_h=(totalh)/tile_h+2;
  tileconstrainofsw=tw4/tile_w;
  tileconstrainofsh=th4/tile_h;  
  pixconstranofsw=tile_w-((4*tile_w)+tw4); // 4 is an aspect ratio thing
  pixconstranofsh=tile_h-((3*tile_h)+th4); // 3 is an aspect ratio thing
 }
 void RepositionConstrainedClamp() {
  sx          =*position_x-(double)transofsw;
  sy          =*position_y-(double)transofsh;
  ex          =*position_x+(double)target->w2;
  ey          =*position_y+(double)target->h2;
  start_tile_x=(int)(sx/(double)tile_w)-tileconstrainofsw;
  start_draw_x=((int)(-sx)%tile_w)+pixconstranofsw;
  if ( start_tile_x < 0 ) {
   start_draw_x+=abs(start_tile_x)*tile_w;
   start_tile_x=0;
  }
  start_tile_y=(int)(sy/(double)tile_h)-tileconstrainofsh;
  start_draw_y=((int)(-sy)%tile_h)+pixconstranofsh;
  if ( start_tile_y < 0 ) {
   start_draw_y+=abs(start_tile_y)*tile_h;
   start_tile_y=0;
  }
  end_tile_x=start_tile_x+tile_render_w; 
  if ( end_tile_x >= w ) {
   end_tile_x=wm1;
  }
  end_tile_y=start_tile_y+tile_render_h; 
  if ( end_tile_y >= h ) {
   end_tile_y=hm1;
  }
 }
 void RenderConstrainedClamp() {
  Pre();
  if ( end_tile_x < 0 ) return;
  if ( end_tile_y < 0 ) return;
  if ( start_tile_x > w || start_tile_y > h ) return;
  double startX=0;
  int startTileX=0;
  if ( start_tile_x > 0 ) { // Draw one tile more left for isometric tiles
   startX=start_draw_x-tile_w;
   startTileX=start_tile_x-1;
  } else {
   startX=start_draw_x;
   startTileX=0;
  }
  Y=start_draw_y+(end_tile_y-start_tile_y)*tile_h;
  for ( Rj=(end_tile_y); Rj>=start_tile_y; Rj-- ) { // Draw bottom-to-top to support stacked isometric tiles
   X=startX;
   for ( Ri=startTileX; Ri<end_tile_x; Ri++ ) { // Draw left to right to support isometric tiles
    t=map(Ri,Rj);
    if ( t ) DrawConstrained();
    X+=tile_w;
   }
   Y-=tile_h;
  }
  Post();
 }
 void RenderWrapConstrained() {
 }
 // Get the map-coord center of a tile relative to 0,0 (top left of map)
 int TileCenterX( int tx ) { return tx*tile_w+tile_w/2; }
 int TileCenterY( int ty ) { return ty*tile_h+tile_h/2; }
 // Calculate the absolute (relative to 0,0 of map) offsets of a given coordinate from top/left of any grid block
 // mcx, mcy "map coord x, map coord y", top left is 0,0
 int TileRelativeX( int mcx ) { return mcx%tile_w; }
 int TileRelativeY( int mcy ) { return mcy%tile_h; }
 int TileCoordX( int mcx ) { return mcx/tile_w; }
 int TileCoordY( int mcy ) { return mcy/tile_h; }
 // Calculate the absolute (relative to 0,0 of map) top left of a tile based on its tile coordinate
 int TileTopLeftX( int tx ) { return tx*tile_w; }
 int TileTopLeftY( int ty ) { return ty*tile_h; }
 /*
  * Debug
  */
 void PrintDebugInfo( int x, int y ) {
  Strings s;
  s.printf( "W/H: %dx%d (wm1=%d,hm1=%d)  size=%d",
            (int)w,(int)h,(int)wm1,(int)hm1,(int)size );
  s.printf( "orig_tile_w/h=%dx%d  tile_w/h: %dx%d tilewm1/hm1:%d/%d", 
            (int)orig_tile_w,(int)orig_tile_h,(int)tile_w,(int)tile_h,(int)tile_wm1,(int)tile_hm1 );
  s.printf( "*p_x,y= %d,%d  mx,my=%d,%d",
   (int)(*position_x),(int)(*position_y),(int)(*mouse_x),(int)(*mouse_y) );
  // *offset_x,*offset_y,
  s.printf( "pixel: w/h: %dx%d w2,h2: %dx%d, size: %d",
   (int)pixel_w,(int)pixel_h,(int)pixel_w2,(int)pixel_h2,(int)pixelSize );
  s.printf( "Constrain: w/h:%dx%d tconofsw/h: %dx%d total w/h: %dx%d  transofsw/h: %dx%d",
   (int)tw4,(int)th4,(int)tileconstrainofsw,(int)tileconstrainofsh,(int)totalw,(int)totalh,(int)transofsw,(int)transofsh );
  s.printf( "s: %d,%d e: %d,%d", (int)sx,(int)sy,(int)ex,(int)ey );
  s.printf( "start_tile: %d,%d, end_tile:%d,%d",
   (int)start_tile_x, (int)start_tile_y, (int)end_tile_x, (int)end_tile_y );
  s.printf( "start_draw: %d,%d, tile_render_w/h: %dx%d ",    
   (int)start_draw_x, (int)start_draw_y, (int)tile_render_w, (int)tile_render_h );
  string content=s.Concatenate('\n');
  WriteTexFont(target,microFont,crayons.Pick(green),additive,x,y,10,10,5,content.c_str());
 }
 private:
 Cartesian tile_at_position;
 double sx,sy,ex,ey;
 int start_tile_x, start_tile_y, end_tile_x, end_tile_y,
     start_draw_x, start_draw_y, tile_render_w, tile_render_h;
};