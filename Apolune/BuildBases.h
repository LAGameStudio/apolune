/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "Strings.h"
#include "Parallax2D.h"

#include "fx_RGBAPal.h"
#include "fx_Buttons.h"
#include "fx_IconSelect.h"
#include "VisualAid.h"
#include "LimitedProce55Viewer2d.h"
#include "SpherizeShader.h"

#include "LightManager2d.h"

#include "Vehicle.h"

extern Zbool builderignoreclick;
extern Zbool builderActive,builderBottom,builderTop;

#define BB_tilesize    256
#define BB_tilesizef   256.0f

#define BB_tilesize2  (BB_tilesize/2)
#define BB_tilesize2f (BB_tilesize2f/2.0f)

#define BB_2tilesize  (BB_tilesize*2)

#define BB_totalW     (BB_tilesize*BB_tilesize)
#define BB_totalH     (BB_tilesize*BB_tilesize)

#define BB_totalW2    (BB_totalW/2)
#define BB_totalH2    (BB_totalH/2)

enum BuildModes {
 SpaceBuilding=0,
 PlanetBuilding=1
};

extern BuildModes buildMode;

/*
 * For building bases in space.
 */

extern char *strut_image_files[50];
extern char *strut_tile_names[50];

class BuiltBasesLightManager2d : public LightManager2d {
public:
 Zp<GLImage> atten,atten2;
 BuiltBasesLightManager2d() : LightManager2d() {
  atten=library.find("data/images/flares/neoflare1.png");
  atten2=library.find("data/images/flares/neoflare14.png");
 }
 void PreProcessing() {
  art.Pivot(atten2,crayons.Pick(alabaster),additive,input.mxi,input.myi,0.0,0.0,accumulator->h/2,accumulator->h/2,input.mxi+input.myi);
  art.Stretch(atten,crayons.Pick(alabaster),additive,0,0,accumulator->w,accumulator->h);
 }
};

ONE(BuiltBase,{
})
 Zint base_id,base_id_group,built;
 Zstring locale;
 Zp<TileSet2d> tiles;
 MassiveTileMapParallax2d maps;
/// MapObjects structures; // structures placed on the underlying tile map
 Cartesian extents1,extents2,baseCoord;
 Cartesiand baseOffset;
 Zp<MassiveTileMapParallaxLayer2d> back,middle;
 KEYWORDS_POST({
  TXTWORD("name",name)
  else TXTWORD("locale",locale)
  else NUMWORD("base_id",base_id)
  else NUMWORD("built",built)
  else NUMWORD("bidg",base_id_group)
  else SUBWORD("baseoffset",baseOffset)
  else SUBWORD("extents1",extents1)
  else SUBWORD("extents2",extents2)
  else SUBWORD("basecoord",baseCoord)
///  SUBWORD("structures",structures)
  else KEYWORD("map",{LoadMap(string(w));})
  else BADKEYWORD
  NEXTWORD
  // tiles are not saved instead are defined by context and must be set prior to loading for lookups
 },{
  MoveBydNoStructures( baseOffset.x, baseOffset.y );
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("locale",locale)
  NUMBKEY("base_id",base_id)
  NUMBKEY("built",built)
  NUMBKEY("bidg",base_id_group)
  SSUBKEY("baseoffset",baseOffset)
  SSUBKEY("extents1",extents1)
  SSUBKEY("extents2",extents2)
  SSUBKEY("basecoord",baseCoord)
///  SUBWORD("structures",structures)
  WORDKEY("map",SaveMap())
 })
 bool OnScreen() {
  return ( extents2.x < display.w && extents2.x2 > -3*BB_tilesize )
      && ( extents2.y < display.h && extents2.y2 > -3*BB_tilesize );
 }
 bool BackgroundOnScreen() {
  return ( extents2.x < display.w*2 && extents2.x2 > -3*BB_tilesize*2 )
      && ( extents2.y < display.h*2 && extents2.y2 > -3*BB_tilesize*2 );
 }
 void LoadMap( string s ) {
  Init(true);
  int j=0;
  const char *q=s.c_str();
  string line;
  while ( *q != '\0' ) {
   q=string_argument_case(q,&line);
   for ( int i=0; i<BB_tilesize; i++ ) {
    const char *p=line.c_str();
    char buf[4];
    buf[0]=p[i*3];
    buf[1]=p[i*3+1];
    buf[2]=p[i*3+2];
    buf[3]='\0';
    Tile2d *tile=tiles->find(string(buf));
    if (tile) {
     back->map.map(i,j)=tile;
     middle->map.map(i,j)=tile;
    }
   }
   j++;
  }
 }
 string SaveMap() {
  string out=string("");
  for ( int j=0; j<BB_tilesize; j++ ) {
   char buf[3*256+1];
   buf[3*256]='\0';
   for ( int i=0; i<BB_tilesize; i++ ) {
    if ( back->map.map(i,j) == null ) {
     buf[i*3]='.';
     buf[i*3+1]='.';
     buf[i*3+2]='.';     
    } else {
     Tile2d *tile=back->map.map(i,j);
     const char *p=tile->name.c_str();
     buf[i*3]=p[0];
     buf[i*3+1]=p[1];
     buf[i*3+2]=p[2];
    }
   }
   out+=string(buf)+string("\n");
  }
  return out;
 }
 void Init( bool generate );
 void MoveBy( int x, int y ) {
  extents1.MoveBy(x,y);
  extents2.MoveBy(x*2,y*2);
  maps.MoveBy(-x,-y);
///  structures.Movebyd((double) x*2.0, (double) y*2.0);
 }
 Vertexd excess;
 void MoveByd( double fx, double fy ) {
  int ix,iy;
  fx+=(float) excess.x;
  fy+=(float) excess.y;
  ix=(int) fx;
  iy=(int) fy;
  excess.Set( fx - (float) ix, fy - (float) iy );
  MoveBy(ix,iy);
//  structures.Movebyd(fx,fy);
 }
 void MoveByNoStructures( int x, int y ) {
  extents1.MoveBy(x,y);
  extents2.MoveBy(x*2,y*2);
  maps.MoveBy(-x,-y);
 }
 Vertexd excess2;
 void MoveBydNoStructures( double fx, double fy ) {
  int ix,iy;
  fx+=(float) excess2.x;
  fy+=(float) excess2.y;
  ix=(int) fx;
  iy=(int) fy;
  excess2.Set( fx - (float) ix, fy - (float) iy );
  MoveByNoStructures(ix,iy);
 }
MANY(BuiltBase,BuiltBaseHandle,BuiltBaseHandles,"BuiltBase",BuiltBases,{
  screen.SetRect(0,0,display.w,display.h);
///  hovering=null;
  expired=0.0f;
  LoadStruts();
})
 Zint base_id_group;
 Zbool planetarySurface;
 Zfloat expired;
 Zbool changedSinceLastSave;
// Zp<MapObject> hovering;
 TileSet2d tiles;
 Cartesiand moved1,moved2;
 Cartesian screen;
 BuiltBasesLightManager2d lighting;
 /*
 void SetCollisions( Collisions2d *collisions ) {
  FOREACH(BuiltBase,b) {
   b->structures.SetCollisions(collisions);
  }
 }
*/
 void Load( int base_id_group );
 void Save( int base_id_group );
 // Adds a new region for a base to be built.  Test before this to determine if a region is there.
 // Perform any other limiting prior to this (such as a test for distance from star)
 BuiltBase *Add( int base_id_group, double x, double y, Cartesian *bc );
 BuiltBase *within( int x, int y ) {
  FOREACH(BuiltBase,b)
   if ( WITHIN(x,y,
    b->extents2.x, b->extents2.y,
    b->extents2.x2,b->extents2.y2)
   ) return b;
  return null;
 }
 /*MapObject *getAtPosition( int x, int y ) {
  FOREACH(BuiltBase,b) EACH(b->structures.first,MapObject,s) if ( s->screen.within(x,y) ) return s;
  return null;
 }
 MapObject *getAtPosition( MapObject *hovered, int x, int y ) {
  if ( hovered->screen.within(x,y) ) return hovered;
  return getAtPosition(x,y);
 }*/
 BuiltBase *sameBC( int bx, int by ) {
  FOREACH(BuiltBase,b) if ( b->baseCoord.x==bx && b->baseCoord.y==by ) return b;
  return null;
 }
 void LoadStruts() {
  Tile2d *tile;
  for ( int i=0; i<50; i++ ) {
   tile=new Tile2d;
   tile->image.Overlay(library.Load(strut_image_files[i]));
   tile->image.blend=transparency;
   tile->name=string(strut_tile_names[i]);
   tile->w=BB_tilesize;
   tile->h=BB_tilesize;
   tiles.Add(tile);
  }
 }
/*
 void MoveBy( int x, int y ) {
  x/=2;
  y/=2;
  moved.x+=x;
  moved.y+=y;
  FOREACH(BuiltBase,b) b->MoveBy(x,y);
 }
 */
 void MoveByd( double x, double y ) {
  moved1.x+=x;
  moved1.y+=y;
  moved2.x+=x*2.0;
  moved2.y+=y*2.0;
  FOREACH(BuiltBase,b) b->MoveByd(x,y);
 }
 void RenderBackground() {
  if ( count == 0 ) return;
  lighting.RenderWithoutDisplaying();
  fboScreen3.background.Pick(clear);
  fboScreen3.Bind();
  FOREACH(BuiltBase,b) if ( b->BackgroundOnScreen() ) {
   b->back->map.Render();
  }
  lighting.RenderAccumulatorFullScreen();
  fboScreen3.Unbind();
  spherizeShader.pTex0=&fboScreen3.texture;
  Blending(transparency);
  art.Stretch(&spherizeShader,0,0,display.w,display.h);
  spherizeShader.Disable();
 }
 void RenderForeground() {
  if ( count == 0 ) return;
  fboScreen3.Bind();
  FOREACH(BuiltBase,b) if ( b->OnScreen() ) {
   b->middle->map.Render();
  }
  lighting.RenderAccumulatorFullScreen();
  fboScreen3.Unbind();
  spherizeShader.pTex0=&fboScreen3.texture;
  Blending(transparency);
  art.Stretch(&spherizeShader,0,0,display.w,display.h);
  expired+=FRAMETIME;
 }
#if defined(DEBUG)
 void Debug() {
  double dy=0.0;
  int i=0;
  Cartesian rect;
  Blending(none);
  FOREACH(BuiltBase,b) {
   glColor4f(1.0f,1.0f,1.0f,1.0f);
   char buf[80];
   FORMAT(buf,80,"#%d E1: %d, %d, %d, %d",
    (int) i, (int) b->extents1.x, (int) b->extents1.y, (int) b->extents1.x2, (int) b->extents1.y2);
   Text(buf,300.0,dy+25.0,10.0,16.0,false);
   FORMAT(buf,80,"E2: %d, %d, %d, %d",
    (int) b->extents2.x, (int) b->extents2.y, (int) b->extents2.x2, (int) b->extents2.y2);
   Text(buf,600.0,dy+25.0,10.0,16.0,false);
   rect.Corners(
    display.w2+b->extents1.x,
    display.h2+b->extents1.y,
    display.w2+b->extents1.x2,
    display.h2+b->extents1.y2 );
   Rectangle(crayons.Pick(green), &rect);
   rect.Corners(
    display.w2+(b->extents2.x),
    display.h2+(b->extents2.y),
    display.w2+(b->extents2.x2),
    display.h2+(b->extents2.y2) );
   Rectangle(crayons.Pick(cyan), &rect);
   FORMAT(buf,80,"%d", (int) i);
   glColor4f(1.0f,0.0f,0.0f,0.0f);
   Text(buf,
    (double)(b->extents1.x+display.w2+BB_totalW/4),
    (double)(b->extents1.y+display.h2+BB_totalW/4),
    5.0,8.0,true);
   dy+=20.0;
   i++;
  }
  rect.Corners(display.w2-10,display.h2-10,display.w2+10,display.h2+10);
  Rectangle(crayons.Pick(red255),&rect);
 }
#endif
DONE(BuiltBase);

// HUD-recalled menu for building in space

class StrutSelector : public fx_IconSelect {
public:
 StrutSelector() : fx_IconSelect() {
  blend=transparency;
  normal.Pick(clear);
  duration=20.0f;
  for ( int i=0; i<50; i++ ) items.Push( strut_image_files[i], i );
  Select(upto(50));
  this->SetExtents(4,64+4,128-10,128-10);
 }
 void WhenChanged() {
  builderignoreclick=true;
 }
};

class StrutRGBPal : public fx_CoolRGBPal3f {
public:
 Zp<StrutSelector> selector;
 void WhenChanged() {
  builderignoreclick=true;
  selector->t_normal.fromCrayon(this->C);
 }
};

class SpaceBuildMenu : public LimitedProce55Viewer2d {
public:
 Zp<StrutSelector> selector;
 Zp<StrutRGBPal> palette;
 Zp<GLImage> backing;
 Crayon bgcolor;
 void OnLoad() {
  SetPositionAndSize(256,display->h-64-16-(128+64),128,(128+64));
  background.Pick(clear);
  bgcolor.Pick(chocolate);
  bgcolor.ScaleByAlphaf(0.25f);
  Add(selector=new StrutSelector);
  selector->MoveBy((int)x,(int)y);
  palette=new StrutRGBPal;
  palette->selector=selector;
  palette->x=5;
  palette->y=0;
  palette->w=128-10;
  palette->h=64; 
  palette->duration=20.0f;
  Add(palette);
  palette->Setup();
  Init();
  backing = library.find("data/images/elements/button_1024.png");
  visible=false;
 }
 void PostProcessing() {
  QuadiNoAlpha(backing,bgcolor,additive,this->x,this->y,this->w,this->h);
 }
// void HideHUD();
};

extern Zp<SpaceBuildMenu> spacebuilder;

// The "HUD underlay" when in build mode (appears on top of BuiltBases).
// Shows: wireframe overlays on building grid, limits of base building, 
// context menus
// Actions: lets the player perform building manipulation (write/erase/rotate)
//          allows players to name a base
class SpaceBuiltBasesOverlay : public LimitedProce55Viewer2d {
public:
// Zp<MapPosition> map;
/// Zp<MapObject> hovering;
 Zp<BuiltBases> bases;
/// Zp<MapObject> placed,selected;
/// PreBuildPreview3d preview;
/// Zp<BLUEPRINT> blueprint; // the "brush"
 Zint mx,my; // mouse
 Zint px,py; // last placed object position
 Zbool clicked; // have we clicked
 Zp<GLImage> highlight;
 Zint base_id_group;
 SpaceBuiltBasesOverlay( int bidg ) : LimitedProce55Viewer2d() {
  highlight=library.find("data/images/elements/buttonglow_1024_1.png");
  base_id_group=bidg;
 }
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  Init();
 }
 void SetBases( BuiltBases *bases ) {
///  blueprint=null;
  this->bases=bases;
  this->bases->base_id_group=base_id_group;
 }
 Tile2d *GetTile() {
  // Add tile to the base using the active color,
  // first check to see if the exact tile (color, image) has already been created.
  Tile2d *tile=null;
  EACH(bases->tiles.first,Tile2d,t) 
   if ( t->image.i == spacebuilder->selector->selected->item
     && t->image.tint.Near(spacebuilder->palette->C,5) ) tile=t;
  // Oops, the tile is not in the list.
  if ( !tile ) {
   // Limit this list to 100 tiles.
   if ( bases->tiles.count > 100 ) {
    // Pick the "nearest" tile with the same image if available,
    EACH(bases->tiles.first,Tile2d,t) 
     if ( t->image.i == spacebuilder->selector->selected->item
      && t->image.tint.Near(spacebuilder->palette->C,5) ) tile=t;
    if ( !tile ) {
     EACH(bases->tiles.first,Tile2d,t) 
      if ( t->image.i == spacebuilder->selector->selected->item
       && t->image.tint.Near(spacebuilder->palette->C,5) ) tile=t;
     if ( !tile ) {
      EACH(bases->tiles.first,Tile2d,t) 
       if ( t->image.i == spacebuilder->selector->selected->item
        && t->image.tint.Near(spacebuilder->palette->C) <5 ) tile=t;
     }
    }
    if ( !tile ) {
     // otherwise pick any tile of a close color.
     EACH(bases->tiles.first,Tile2d,t) 
      if ( t->image.tint.Near(spacebuilder->palette->C,5) ) tile=t;
     if ( !tile ) EACH(bases->tiles.first,Tile2d,t) 
      if ( t->image.tint.Near(spacebuilder->palette->C) <5 ) tile=t;
    }
    // As a last resort, pick the last tile.
    if ( !tile ) tile=(Tile2d *) bases->tiles.last;
   } else { // If not more than 100, append a new tile to the list.  
    tile=new Tile2d;
    tile->image.i=spacebuilder->selector->selected->item;
    tile->image.tint.fromCrayon(spacebuilder->palette->C);
    tile->image.blend=transparency;
    tile->name=bases->tiles.unique(3);
    bases->tiles.Append(tile);
    bases->changedSinceLastSave=true;
   }
  }
  return tile;
 }
 Cartesian baseCoord,baseOffset1,baseOffset2; // the "base dialup" number
 void GetBaseCoordinate( int x, int y ) {
  baseCoord.x=x/BB_totalW;
  if ( x < 0 ) baseCoord.x-=1;
  baseCoord.y=y/BB_totalH;
  if ( y < 0 ) baseCoord.y-=1;
  baseOffset1.Set( // back
   (baseCoord.x*BB_totalW/2),
   (baseCoord.y*BB_totalH/2)
  );
  baseOffset2.Set( // mid
   (baseCoord.x*BB_totalW),
   (baseCoord.y*BB_totalH)
  );
 }
 BuiltBase *GetBase( int x, int y, bool bCreate, int base_id_group=0 ) {
  GetBaseCoordinate(x,y);
  BuiltBase *b=bases->sameBC(baseCoord.x,baseCoord.y);
  if ( b ) return b;
  if ( !bCreate || bases->count > 9 ) return null; //Limiter
  // Determine the upper left corner of this base.
  return bases->Add( base_id_group,
   (double)baseOffset1.x,
   (double)baseOffset1.y,
   &baseCoord
  );
 }
 Cartesian impact; // The impact tile coord, requires precalculated baseCoord
 void GetImpactCoord( int x, int y ) {
  impact.x=(int)(BB_tilesizef*iratiof(x-baseOffset2.x,BB_totalW));
  impact.y=(int)(BB_tilesizef*iratiof(y-baseOffset2.y,BB_totalH));
 }
 Cartesian p;
 void Between();
 void PostProcessing() {
  /*
  if ( !selected && !placed && blueprint ) {
   preview.Render(mx,my);
  }
  if ( hovering ) {
   Crayon drawing;
   drawing.Pick(cyan);
   drawing.ScaleByAlphaf(0.25f);
   QuadiNoAlpha(highlight,drawing,additive,hovering->screen.x,hovering->screen.y,hovering->screen.w,hovering->screen.h);
  }*/
 }
 void MouseMove() {}
 void LeftDown() { /* stay empty */ }
#if defined(DEBUG)
 void Debug() {
  char buf[80];
  Blending(none);
  glColor3f(1.0f,0.0f,0.0f);
//  FORMAT(buf,80,"Map->position: %d,%d",(int)map->position.x,(int)map->position.y);
//  Text(buf,5.0, 20.0,10.0,14.0,false);
  FORMAT(buf,80,"       impact: %d,%d", (int)impact.x, (int)impact.y);
  Text(buf,515.0, 40.0,10.0,14.0,false);
  FORMAT(buf,80,"    baseCoord: %d,%d", (int)baseCoord.x, (int)baseCoord.y);
  Text(buf,515.0, 60.0,10.0,14.0,false);
  FORMAT(buf,80,"   baseOffset1: %d,%d", (int)baseOffset1.x, (int)baseOffset1.y);
  Text(buf,515.0, 80.0,10.0,14.0,false);
  FORMAT(buf,80,"   baseOffset2: %d,%d", (int)baseOffset2.x, (int)baseOffset2.y);
  Text(buf,515.0,100.0,10.0,14.0,false);
  FORMAT(buf,80,"            p: %d,%d", (int)p.x, (int)p.y);
  Text(buf,515.0,120.0,10.0,14.0,false);
  FORMAT(buf,80,"bases->moved1: %d,%d",(int)bases->moved1.x,(int)bases->moved1.y);
  Text(buf,515.0,140.0,10.0,14.0,false);
 }
#endif
};

extern Zp<SpaceBuiltBasesOverlay> spacebuilt;

#if defined(NEVER)
class BuiltBasesTest : public Engine {
public:
 MapPosition map;
 BuiltBases bases;
 void OnLoad() {
  buildengine=this;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);
  AddChild(spacebuilt=new SpaceBuiltBasesOverlay);
  AddChild(spacebuilder=new SpaceBuildMenu);
  spacebuilt->visible=true;
  spacebuilder->visible=true;
  spacebuilt->bases=&bases;
  spacebuilt->map=&map;
 }
 void Between() {
  Cartesian moved;
  if ( input->KeyDown(DX_RIGHT) ) moved.x=1;
  if ( input->KeyDown(DX_LEFT) ) moved.x=-1;
  if ( input->KeyDown(DX_DOWN) ) moved.y=1;
  if ( input->KeyDown(DX_UP) ) moved.y=-1;
  bases.MoveByd((double)moved.x,(double)moved.y);
  map.position.x+=(double) moved.x;
  map.position.y+=(double) moved.y;
 }
 void Render() {
  bases.Render();
#if defined(DEBUG)
  bases.Debug();
#endif
 }
};
#endif

#if defined(NEVER)
class EnterBaseButton : public fx_UglyButton {
public:
 Zp<SolarSystem> s;
 Zp<Planet> p,m;
 Zp<MapObject> building;
 EnterBaseButton() : fx_UglyButton() {}
 void Customize() {  
  normal   = crayons.Pick(chocolate);
  t_normal = crayons.Pick(alabaster);
  f_normal = crayons.Pick(chocolate);
  click    = crayons.Pick(red255);
  t_click  = crayons.Pick(alabaster);
  f_click  = crayons.Pick(red255);
  hover    = crayons.Pick(green);
  t_hover  = crayons.Pick(alabaster);
  f_hover  = crayons.Pick(green);
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  glow_c  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
  SetLabel("Enter");
  delay=0.0f;
  duration=20.0f;
  SetSounds("data/sounds/UI/bleep.wav","data/sounds/UI/bonk.wav");
  w=64;
  h=32;
  w2=32;
  h2=32;
 }
 void WhenClicked();
};
#endif

#if defined(NEVER)
class BasesOverlay : public LimitedProce55Viewer2d {
public:
 Zp<BuiltBases> bases;
// Zp<MapObject> hovering,open;
// Zp<EnterBaseButton> ebb;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);
 }
 void MouseMove();
};
#endif