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

#include "GLWindow.h"
#include "TileMap2d.h"

class DLTilemapTest : public GLWindow {
public:
 MassiveTileMap2d map;
 Cartesian position;
 TileSet2d tiles;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  CompositeImages images;
  DisplayListTile2d *dl;
  int i,j;

  for ( i=0; i<16; i++ ) {
   for ( j=0; j<16; j++ ) {
    CompositeImage *c=new CompositeImage;
    c->RenderTo(this);
    c->position.x=i*10;
    c->position.y=j*10;
    c->Overlay( library.random(), crayons.Any() );
    images.Append(c);
   }
  }

  dl=new DisplayListTile2d;
  dl->Create(&images);
  dl->name="SX";
  tiles.Add(dl);
  images.Clear();

  for ( i=0; i<16; i++ ) {
   for ( j=0; j<16; j++ ) {
    CompositeImage *c=new CompositeImage;
    c->RenderTo(this);
    c->position.x=i*10;
    c->position.y=j*10;
    c->Overlay( library.random(), crayons.Any() );
    images.Append(c);
   }
  }
  
  dl=new DisplayListTile2d;
  dl->Create(&images);
  dl->name="PF";
  tiles.Add(dl);
  images.Clear();

  for ( i=0; i<16; i++ ) {
   for ( j=0; j<16; j++ ) {
    CompositeImage *c=new CompositeImage;
    c->RenderTo(this);
    c->position.x=i*10;
    c->position.y=j*10;
    c->Overlay( library.random(), crayons.Any() );
    images.Append(c);
   }
  }
  
  dl=new DisplayListTile2d;
  dl->Create(&images);
  dl->name="S5";
  tiles.Add(dl);
  images.Clear();

  for ( i=0; i<16; i++ ) {
   for ( j=0; j<16; j++ ) {
    CompositeImage *c=new CompositeImage;
    c->RenderTo(this);
    c->position.x=i*10;
    c->position.y=j*10;
    c->Overlay( library.random(), crayons.Any() );
    images.Append(c);
   }
  }
  
  dl=new DisplayListTile2d;
  dl->Create(&images);
  dl->name="CO";
  tiles.Add(dl);
  images.Clear();

  for ( i=0; i<16; i++ ) {
   for ( j=0; j<16; j++ ) {
    CompositeImage *c=new CompositeImage;
    c->RenderTo(this);
    c->position.x=i*10;
    c->position.y=j*10;
    c->Overlay( library.random(), crayons.Any() );
    images.Append(c);
   }
  }
  
  dl=new DisplayListTile2d;
  dl->Create(&images);
  dl->name="IF";
  tiles.Add(dl);
  images.Clear();

  
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