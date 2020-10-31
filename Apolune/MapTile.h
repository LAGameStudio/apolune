/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "TextureLibrary.h"
#include "AtlasTileset.h"

ONE(MapTile,{
 tint.Pick(alabaster);
 emiTint.Pick(alabaster);
})
 Zp<GLImage> img,height,emission;
 Zp<AtlasTileset> ats;
 Zp<AtlasTile> at;
 Zstring atlasTilesetName,atlasTileName;
 Cartesians colliders;
 Crayon tint,emiTint;
 KEYWORDS({
  NEARESTWORD("img",img)
  else IMGWORD("hei",height)
  else IMGWORD("emi",emission)
  else TINTWORD("tint",tint)
  else TINTWORD("etint",emiTint)
  else SUBWORD("b",colliders)
  else TXTWORD("atSet",atlasTilesetName)
  else TXTWORD("atTile",atlasTileName)
 })
 WORDKEYS({
  IMGKEY("img",img)
  IMGKEY("hei",height)
  IMGKEY("emi",emission)
  TINTKEY("tint",tint)
  TINTKEY("etint",emiTint)
  SSUBKEY("b",colliders)
  if ( at ) {
   TXTKEY("atSet",atlasTilesetName)
   TXTKEY("atTile",atlasTileName)
  }
 })
 CLONE(MapTile,{
  DUPE(img)
  DUPE(height)
  DUPE(emission)
  DUPE(tint)
  DUPE(emiTint)
  DUPE(ats)
  DUPE(at)
  DUPE(atlasTilesetName)
  DUPE(atlasTileName)
  a->colliders.Duplicate(&colliders);
 })
 void SetAtlasTile( AtlasTileset *ats, AtlasTile *at ) {
  if ( !ats || !at ) { ats=null; at=null; return; }
  this->ats=ats;
  this->atlasTilesetName=ats->name;
  this->at=at;
  this->atlasTileName=at->name;
 }
 void GetReferencedTile() {
  if ( atlasTilesetName.length > 0 && atlasTileName.length > 0 ) {
   ats=atlasTilesets.GetTileset(atlasTilesetName.c_str());
   at=atlasTilesets.GetTile(ats,atlasTileName.c_str());
  }
 }
MANY(MapTile,MapTileHandle,MapTileHandles,"tile",MapTiles,{})
 CALLEACH(MapTile,GetReferencedTile)
 KEYSWORDSGroups("tile",MapTile)
 CLONES(MapTile,MapTiles)
DONE(MapTile)

ONE(MapTileset,{})
 MapTiles tiles;
 KEYWORDS({
  SUBWORD("tiles",tiles)
 })
 WORDKEYS({
  SSUBKEY("tiles",tiles)
 })
 CLONE(MapTileset,{
  a->tiles.Duplicate(&tiles);
 })
 void Set(MapTileset *in) {
  tiles.Duplicate(&in->tiles);
 }
 void GetReferencedTile() { tiles.GetReferencedTile(); }
MANY(MapTileset,MapTilesetHandle,MapTilesetHandles,"tileset",MapTilesets,{})
 CALLEACH(MapTile,GetReferencedTile)
 CLONES(MapTileset,MapTilesets)
 KEYSWORDSGroups("tileset",MapTileset)
DONE(MapTileset)
