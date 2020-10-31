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
#include "ListItem.h"
#include "LinkedList.h"
#include "Art.h"
#include "GLSetup.h"
#include "GLImage.h"
#include "Trigger.h"
#include "Cartesian.h"
#include "ZIndexed.h"
#include "VBO_VT.h"

ONE(SpriteSheetSprite,{})
 Cartesian pixels;
 Cartesiand onSheet;
 VBOStaticVT vbo;
 SpriteSheetSprite( int sx, int sy, int sw, int sh, int sheetw, int sheeth ) : ListItem() {
  Create(sx,sy,sw,sh,sheetw,sheeth);
 }
 SpriteSheetSprite( const char *name, int sx, int sy, int sw, int sh, int sheetw, int sheeth ) : ListItem() {
  Create(name,sx,sy,sw,sh,sheetw,sheeth);
 }
 void Create( const char *name, int sx, int sy, int sw, int sh, int sheetw, int sheeth ) {
  this->name=name;
  Create(sx,sy,sw,sh,sheetw,sheeth);
 }
 void Create( int sx, int sy, int sw, int sh, int sheetw, int sheeth ) {
  pixels.SetRect(sx,sy,sw,sh);
  onSheet.SetRect(
   iratiod(pixels.x,sheetw),
   iratiod(pixels.y,sheeth),
   iratiod(pixels.x2,sheetw),
   iratiod(pixels.y2,sheeth)
  );
  vbo.Clear();
  Vertex a,b,c,d;
  a.Set(-sw/2,-sh/2); a.TexCoord(pixels.x,pixels.y);
  b.Set(sw/2,-sh/2);  b.TexCoord(pixels.x2,pixels.y);
  c.Set(sw/2,sh/2);   c.TexCoord(pixels.x2,pixels.y2);
  d.Set(-sw/2,sh/2);  d.TexCoord(pixels.x,pixels.y2);
  vbo.PushQuad(&a,&b,&c,&d);
  vbo.Create();
 }
MANY(SpriteSheetSprite,SpriteSheetSpriteHandle,SpriteSheetSpriteHandles,"SpriteSheetSprite",SpriteSheetSprites,{})
DONE(SpriteSheetSprite);

ONE(SpriteSheetFilmFrame,{})
 Zp<SpriteSheetSprite> sprite;
MANY(SpriteSheetFilmFrame,SpriteSheetFilmFrameHandle,SpriteSheetFilmFrameHandles,"SpriteSheetFilmFrame",SpriteSheetFilmFrames,{})
DONE(SpriteSheetFilmFrame);

ONE(SpriteSheetFilm,{})
 SpriteSheetFilmFrames frames;
 Zp<SpriteSheetFilmFrame> current;
 Zdouble expired,duration;
MANY(SpriteSheetFilm,SpriteSheetFilmHandle,SpriteSheetFilmHandles,"SpriteSheetFilm",SpriteSheetFilms,{})
DONE(SpriteSheetFilm);

ONE(SpriteSheet,{})
public:
 ZIndexed<SpriteSheetSprite> sprites;
 Zint tw,th,gx,gy; // tile size (pixels), grid size (tiles, matches index.w,index.h)
 Zint lastAdded;
 Zp<GLImage> sheet,normal,height;
 SpriteSheetFilms films;
 SpriteSheet( const char *name, GLImage *s, int tile_w, int tile_h ) : ListItem() {
  this->name=name;
  sheet=s;
  normal=null;
  height=null;
  Slice(s->width,s->height,tile_w,tile_h);
 }
 SpriteSheet( const char *name, GLImage *s, GLImage *n, int tile_w, int tile_h ) : ListItem() {
  this->name=name;
  sheet=s;
  normal=n;
  height=null;
  Slice(s->width,s->height,tile_w,tile_h);
 }
 SpriteSheet( const char *name, GLImage *s, GLImage *h, int tile_w, int tile_h, bool heightToNormal ) : ListItem() {
  this->name=name;
  sheet=s;
  height=h;
  normal=null;
  Slice(s->width,s->height,tile_w,tile_h);
 }
 SpriteSheet( const char *name, GLImage *s, GLImage *n, GLImage *h, int tile_w, int tile_h ) : ListItem() {
  this->name=name;
  sheet=s;
  normal=n;
  height=h;
  Slice(s->width,s->height,tile_w,tile_h);
 }
 void Slice( int sheet_w, int sheet_h, int tile_w, int tile_h ) {
  unsigned int gx=(unsigned int)(sheet_w/tile_w)+1;
  unsigned int gy=(unsigned int)(sheet_h/tile_h)+1;
  sprites.Size((size_t)gx,(size_t)gy);
  int k=0;
  for ( unsigned int i=0; i<gx; i++ ) {
   for ( unsigned int j=0; j<gy; j++ ) {
    k++;
    sprites(i,j).Create(
     name.length>0?FORMAT("%s %d",name.c_str(),(int)k).c_str():FORMAT("%d",(int)k).c_str(),
     i*tile_w,i*tile_h,tile_w,tile_h,sheet_w,sheet_h
    );
   }
  }
 }
 void Render( SpriteSheetSprite *sprite ) {
  sprite->vbo.Render();
 }
 void Render( SpriteSheetFilmFrame *frame ) {
  frame->sprite->vbo.Render();
 }
MANY(SpriteSheet,SpriteSheetHandle,SpriteSheetHandles,"SpriteSheet",SpriteSheets,{})
DONE(SpriteSheet);
