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

#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"

#include "TileMap2d.h"
#include "Parallax2D.h"

#include "TextureLibrary.h"


class TileMapTest : public GLWindow {
public:
 TileSet2d tiles;
 TileMap2d map;
 Cartesian position;

 void OnLoad() {
  Tile2d *tile;
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_dukeluke_Skull_human_X-ray_1.png" ));
  tile->name="SX";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Inlay_Parquet_Floor_Design_120306.png" ));
  tile->name="IF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->name="PF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->name="S5";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  Strings asciiMap;

  asciiMap.Push( "  SXSXPFS5" );
  asciiMap.Push( "  SXIFCOS5" );
  asciiMap.Push( "  SXSXPFS5" );
  asciiMap.Push( "  SXSXPF  " );
  asciiMap.Push( "  SXSXPFIF" );
  asciiMap.Push( "  S5IFPFIF" );
  asciiMap.Push( "  SXIFCOS5" );

  map.tiles=&tiles;
  map.placements.fromStrings( &tiles, &asciiMap, 128, 128, 2 );
  map.RenderTo(this);

  map.placements.AssignOffset( &position.x, &position.y );

  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
 }

 void Between() {
  if ( input->KeyDown(DX_RIGHT) ) position.MoveBy(1,0);
  if ( input->KeyDown(DX_LEFT) ) position.MoveBy(-1,0);
  if ( input->KeyDown(DX_UP) ) position.MoveBy(0,-1);
  if ( input->KeyDown(DX_DOWN) ) position.MoveBy(0,1);
  map.Between();
 }

 void Render() {
  map.RenderOffset();
 }
};


class TileMapTest2 : public GLWindow {
public:
 MassiveTileMap2d map;
 
 TileSet2d tiles;
 Cartesian position;

 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  Tile2d *tile;
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_dukeluke_Skull_human_X-ray_1.png" ));
  tile->image.blend=transparency;
  tile->name="SX";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Inlay_Parquet_Floor_Design_120306.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->image.blend=transparency;
  tile->name="PF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  Strings asciiMap;

  asciiMap.Push( "  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5" );
  asciiMap.Push( "  COIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5" );
  asciiMap.Push( "  SXSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  SXSXPFS5" );
  asciiMap.Push( "  SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF    COSXPF  " );
  asciiMap.Push( "  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF" );
  asciiMap.Push( "  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCO" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );

  position.x=32;
  position.y=32;
  map.Init( clampTile, translated, this, &position.x, &position.y, &tiles, &asciiMap, 256,256, 64,64, 2 );
 }

 void Between() {
  bool repo=false;
  if ( input->KeyDown(DX_RIGHT) ) { position.MoveBy(1,0);  repo=true; }
  if ( input->KeyDown(DX_LEFT)  ) { position.MoveBy(-1,0); repo=true; }
  if ( input->KeyDown(DX_UP)    ) { position.MoveBy(0,-1); repo=true; }
  if ( input->KeyDown(DX_DOWN)  ) { position.MoveBy(0,1);  repo=true; }
  if ( input->KeyDown(DX_ADD) ) {
   map.tile_w++;
   map.tile_h++;
   map.Resized();
   repo=true;
  }
  if ( input->KeyDown(DX_SUBTRACT) ) {
   if ( map.tile_w > 1 ) map.tile_w--;
   if ( map.tile_h > 1 ) map.tile_h--;
   map.Resized();
   repo=true;
  }
  if ( repo ) map.Reposition();
  
 }

 void Render() {
  map.Render();
 }
};



class TileMapTest3 : public GLWindow {
public:
 MassiveTileMapParallax2d maps;

 GLImage *bg;
 
 TileSet2d tiles;
 float zoom,zoomRate;

 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  bg=library.Load("data/images/encyclopedia/main_moonr.jpg");

  Tile2d *tile;
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_dukeluke_Skull_human_X-ray_1.png" ));
  tile->image.blend=transparency;
  tile->name="SX";
  tile->w=256;
  tile->h=256;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/encyclopedia/NASA_70s_colony_3lrg.jpg" ));
  tile->image.blend=transparency;
  tile->name="HI";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/encyclopedia/Nasa_space_elev.jpg" ));
  tile->image.blend=transparency;
  tile->name="UL";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Inlay_Parquet_Floor_Design_120306.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->image.blend=transparency;
  tile->name="PF";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles.Add(tile);

  Strings asciiMap;

  asciiMap.Push( "COSXSXPFS5  SXSXPFS5  SXSXPFS5  SX                  SXSXPFS5  SX    S5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPFS5  SXSXPF  SXSXPFS5" );
  asciiMap.Push( "  COIFCOS5  SXIFCOS5  SXIFCOS5                      SXIFCOS5  SX    S5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCO  SXIFCOS5" );
  asciiMap.Push( "  SXSXPFS5  COSXPFS5  COSXPFS5                      COSXPFS5  CO    S5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPFS5  COSXPF  SXSXPFS5" );
  asciiMap.Push( "  SXSXPF    SXSXPF    SXSXPF                        SXSXPF    SX    IF  SXSXPF    SXSXPF    SXSXPF    SXSXPF    SXSXPF  COSXPF  " );
  asciiMap.Push( "  SXSXPFIF  SXSXPFIF  SXSXPFIF                      SXSXPFIF  SX    IF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPFIF  SXSXPF  SXSXPFIF" );
  asciiMap.Push( "  S5IFPFIF  S5IFPFIF  S5IFPFIF                      S5IFPFIF  S5    IF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPFIF  S5IFPF  S5IFPFIF" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCOS5  SX    S5  SX  CO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5SXSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S  5S5S5S5S5S5S5S5S5S5S5SXIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SX    S5  SX    S5  SX    S5  SXIFCOS5  SX    S5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S  5S5S5S5S5S5S5S5S5S5S5COSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SX    S5  SX    S5  SX    S5  SXIFCOS5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXSXPFS5  SXSXPFS5  SXIFCOS5  SX    S5  SX    S5  SX    S5  SXIFCOS5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S  5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S  5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  COSXPFS5  COSXPFS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S  5S5S5XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  XSXPF  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXSXPF    SXSXPF    SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  XIFCO  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXSXPFIF  SXSXPFIF  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  OSXPF  " );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  S5IFPFIF  S5IFPFIF  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSXPF  SXSX" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCOCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIFCO  SXIF" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSXPF  SOSX" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5                      SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "  SXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SX    S5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );
  asciiMap.Push( "COSXIFCOS5  SXIFCOUL  HIIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCOS5  SXIFCO  SXIFCOS5S5S5S5S5S5S5S5S5S5S5S5IFS5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5S5" );

  MassiveTileMapParallaxLayer2d *layer;
//  layer=maps.AddLayer(1.0f,  this, &tiles, &asciiMap, 256,256,  16,  16, 2 ); 
//  layer=maps.AddLayer(2.0f,  this, &tiles, &asciiMap, 256,256,  32,  32, 2 ); 
//  layer=maps.AddLayer(4.0f,  this, &tiles, &asciiMap, 256,256,  64,  64, 2 ); 
//  layer=maps.AddLayer(8.0f,  this, &tiles, &asciiMap, 256,256, 128, 128, 2 ); 
//  layer=maps.AddLayer(16.0f, this, &tiles, &asciiMap, 256,256, 256, 256, 2 ); 
//  layer=maps.AddLayer(32.0f, this, &tiles, &asciiMap, 256,256, 512, 512, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 1.0f,   this, &tiles, &asciiMap, 256,256,   32,   32, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 2.0f,   this, &tiles, &asciiMap, 256,256,   64,   64, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 4.0f,   this, &tiles, &asciiMap, 256,256,  128,  128, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 8.0f,   this, &tiles, &asciiMap, 256,256,  256,  256, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 16.0f,  this, &tiles, &asciiMap, 256,256,  512,  512, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 32.0f,  this, &tiles, &asciiMap, 256,256, 1024, 1024, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 64.0f,  this, &tiles, &asciiMap, 256,256, 2048, 2048, 2 ); 
  layer=maps.AddLayer( clampTile, translated, 128.0f, this, &tiles, &asciiMap, 256,256, 4096, 4096, 2 ); 

  zoom=1.0f;
  zoomRate=(1.0f/FPS)*2;
 }

 void Between() {
  bool repo=false;
  if ( input->KeyDown(DX_RIGHT) ) { maps.MoveBy(5,0);  }
  if ( input->KeyDown(DX_LEFT)  ) { maps.MoveBy(-5,0); }
  if ( input->KeyDown(DX_UP)    ) { maps.MoveBy(0,-5); }
  if ( input->KeyDown(DX_DOWN)  ) { maps.MoveBy(0,5);  }
  if ( input->KeyDown(DX_ADD) ) {
   zoom+=zoomRate;
   maps.layers.Zoom(zoom,zoom);  
  }
  if ( input->KeyDown(DX_SUBTRACT) ) {
   zoom-=zoomRate;
   if ( zoom < zoomRate ) zoom=zoomRate;
   maps.layers.Zoom(zoom,zoom);
  }
  if ( input->KeyDown(DX_NUMPAD5) ) {
   zoom=1.0f;
   maps.layers.Zoom(zoom,zoom);
  }
 }

 void Render() {
  StretchiNoAlpha(this,bg,crayons.Pick(alabaster),none,0,0,this->w,this->h);
  maps.layers.Render();
 // ((MassiveTileMapParallaxLayer2d *) maps.layers.first)->map.PrintDebugInfo(10,10);
 }
};


class TileMapTest4 : public GLWindow {
public:
 MassiveTileMapParallax2d maps;

 GLImage *bg;
 
 TileSet2d tiles1,tiles2,tiles3,tiles4;
 float zoom,zoomRate;

 void OnLoad() {
  MassiveTileMapParallaxLayer2d *layer;
  Strings asciiMap;

  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);

 // bg=library.Load("data/images/encyclopedia/main_moonr.jpg");

  Tile2d *tile;
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_dukeluke_Skull_human_X-ray_1.png" ));
  tile->image.blend=transparency;
  tile->name="SX";
  tile->w=256;
  tile->h=256;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/encyclopedia/NASA_70s_colony_3lrg.jpg" ));
  tile->image.blend=transparency;
  tile->name="HI";
  tile->w=128;
  tile->h=128;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/encyclopedia/Nasa_space_elev.jpg" ));
  tile->image.blend=transparency;
  tile->name="UL";
  tile->w=128;
  tile->h=128;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Inlay_Parquet_Floor_Design_120306.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=32;
  tile->h=32;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->image.blend=transparency;
  tile->name="PF";
  tile->w=123;
  tile->h=123;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=32;
  tile->h=32;
  tiles1.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=16;
  tile->h=16;
  tiles1.Add(tile);

  asciiMap.Push( "S5S5S5  COSX  PFS5  SXSXPFS5  SXSXPFS5" );
  asciiMap.Push( "S5  S5  COSX  PFS5  SXSXPFS5  SXSXPFS5" );
  asciiMap.Push( "S5S5S5  COSX  PFS5  SXSXPFS5  SXSXPFS5" );

  layer=maps.AddLayer( clampConstrain, translated, 1.0f,   this, &tiles1, &asciiMap, 256,256,   64,   64, 2 ); 

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=32;
  tile->h=32;
  tiles2.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles2.Add(tile);
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=64;
  tile->h=64;
  tiles2.Add(tile);

  asciiMap.Clear();
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5       CO       IF" );
  asciiMap.Push( "S5  S5  COS5  S5    S5S5       CO       IF" );
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5       CO       IF" );

  layer=maps.AddLayer( clampConstrain, translated, 2.0f,   this, &tiles2, &asciiMap, 256,256,   64,   64, 2 ); 

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=32;
  tile->h=32;
  tiles3.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles3.Add(tile);
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=128;
  tile->h=128;
  tiles3.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->image.blend=transparency;
  tile->name="PF";
  tile->w=256;
  tile->h=256;
  tiles3.Add(tile);

  asciiMap.Clear();
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5  COPF  PFPFIFIFIFIF" );
  asciiMap.Push( "S5  S5  COS5  S5    S5S5  COPF  PFPFIFIFIFIF" );
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5  COPF  PFPFIFIFIFUL" );

  layer=maps.AddLayer( clampConstrain, translated, 4.0f,   this, &tiles3, &asciiMap, 256,256,  64,  64, 2 ); 

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/hairymnstr_Seasons_5.png" ));
  tile->image.blend=transparency;
  tile->name="S5";
  tile->w=32;
  tile->h=32;
  tiles4.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="CO";
  tile->w=128;
  tile->h=128;
  tiles4.Add(tile);
  
  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/ytknick_Computer.png" ));
  tile->image.blend=transparency;
  tile->name="IF";
  tile->w=128;
  tile->h=128;
  tiles4.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/flags/_lakeside_Parquet_Floor_4.png" ));
  tile->image.blend=transparency;
  tile->name="PF";
  tile->w=256;
  tile->h=256;
  tiles4.Add(tile);

  tile=new Tile2d;
  tile->image.Overlay(library.Load( "data/images/encyclopedia/Nasa_space_elev.jpg" ));
  tile->image.blend=transparency;
  tile->name="UL";
  tile->w=512;
  tile->h=512;
  tiles4.Add(tile);

  asciiMap.Clear();
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5  COPF  PFPFIFIFIFIF" );
  asciiMap.Push( "S5  S5  COS5  S5    S5S5  COPF  PFPFIFIFIFIF" );
  asciiMap.Push( "S5S5S5  COS5  S5    S5S5  COPF  PFPFIFIFIFIF" );

  layer=maps.AddLayer( clampConstrain, translated, 8.0f,   this, &tiles4, &asciiMap, 256,256,  64,  64, 2 ); 

  zoom=1.0f;
  zoomRate=(1.0f/FPS)*2;
 }

 void Between() {
  bool repo=false;
  if ( input->KeyDown(DX_RIGHT) ) { maps.MoveBy(1,0);  }
  if ( input->KeyDown(DX_LEFT)  ) { maps.MoveBy(-1,0); }
  if ( input->KeyDown(DX_UP)    ) { maps.MoveBy(0,-1); }
  if ( input->KeyDown(DX_DOWN)  ) { maps.MoveBy(0,1);  }
  if ( input->KeyDown(DX_ADD) ) {
   zoom+=zoomRate;
   maps.layers.Zoom(zoom,zoom);  
  }
  if ( input->KeyDown(DX_SUBTRACT) ) {
   zoom-=zoomRate;
   if ( zoom < zoomRate ) zoom=zoomRate;
   maps.layers.Zoom(zoom,zoom);
  }
  if ( input->KeyDown(DX_NUMPAD5) ) {
   zoom=1.0f;
   maps.layers.Zoom(zoom,zoom);
  }
 }

 void Render() {
  //StretchiNoAlpha(this,bg,crayons.Pick(alabaster),none,0,0,this->w,this->h);
  maps.layers.Render();
//  ((MassiveTileMapParallaxLayer2d *) maps.layers.first)->map.PrintDebugInfo(10,10);
//  ((MassiveTileMapParallaxLayer2d *) maps.layers.last)->map.PrintDebugInfo(10,300);
 }
};